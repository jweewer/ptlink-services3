/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************

  Description: shows last registered channels

 *  $Id: cs_ajoin.c,v 1.6 2005/10/29 11:22:29 jpinto Exp $
*/

#include "module.h"
#include "my_sql.h"
#include "chanserv.h"
#include "dbconf.h"
#include "nsmacros.h"
#include "cs_role.h"
/* lang files */
#include "lang/cscommon.lh"
#include "lang/cs_ajoin.lh"

SVS_Module mod_info =
 /* module, version, description */
{"cs_ajoin", "1.1",  "chanserv ajoin command" };

/* Change Log
  1.1 - #46: missing foreign key relations on channels and nicks
  	#36: CS AJOIN will INVITE the user if he/she has invite perm.
        #35: ability to set order on ajoins
        Apply cs_ajoin.2.sql for the above changes
  1.0 -	#1: Replace NS AUTOJOIN with CS AJOIN ADD/DEL
*/

#define DB_VERSION	2
  
/** functions and events we require **/
static ServiceUser* (*chanserv_suser)(void);
static int e_nick_identify;

MOD_REQUIRES
  DBCONF_FUNCTIONS
  MOD_FUNC(e_nick_identify)
  MOD_FUNC(chanserv_suser)
  MOD_FUNC(role_with_permission)
MOD_END

/** Internal functions **/
void ev_cs_ajoin_nick_identify(IRC_User* u, u_int32_t *snid);
static int sql_upgrade(int version, int post);

/* available commands from module */
void cs_ajoin(IRC_User *s, IRC_User *u);

/** Local config **/
static int MaxAjoinsPerUser;
DBCONF_PROVIDES
  DBCONF_INT(MaxAjoinsPerUser, "20",
    "How many ajoins a user can define?")
DBCONF_END

/** Local variables **/
static ServiceUser* csu;
static int cs_log;

/* this is called before load and at services rehash */
int mod_rehash(void)
{
  if(dbconf_get_or_build(mod_info.name, dbconf_provides) < 0 )
  {
    errlog("Error reading dbconf!");
    return -1;
  }
  return 0;
}

/** load code **/
int mod_load(void)
{
  if(sql_check_inst_upgrade(mod_info.name, DB_VERSION, sql_upgrade) < 0 )
    return -1;
    
  cs_log = log_handle("chanserv");
  
  csu = chanserv_suser();
  suser_add_cmd(csu, "AJOIN", cs_ajoin, CS_AJOIN_SUMMARY, CS_AJOIN_HELP);      
  suser_add_help(csu, "AJOIN ADD", CS_AJOIN_ADD_SYNTAX);
  suser_add_help(csu, "AJOIN DEL", CS_AJOIN_DEL_SYNTAX);  
  
  /* Add actions */
  mod_add_event_action(e_nick_identify, (ActionHandler) ev_cs_ajoin_nick_identify);
  return 0;
}

static int ajoins_count(u_int32_t snid)
{
  if(sql_singlequery("SELECT count(*) FROM cs_ajoin WHERE snid=%d", snid))
    return sql_field_i(0);
  return 0;
}

void cs_ajoin(IRC_User *s, IRC_User *u)
{
  u_int32_t source_snid;
  char *cmd;
  
  CHECK_IF_IDENTIFIED_NICK
  cmd = strtok(NULL, " ");
  /* check for required params */
  if(!cmd)
    send_lang(u, s, CS_AJOIN_HELP);
  else
  /* check command */
  if(strcasecmp(cmd, "ADD") == 0)
  {
    ChanRecord* cr = NULL;
    char *chname = strtok(NULL, " ");    
    /* check for required params */
    if(!chname)
      send_lang(u, s, CS_AJOIN_ADD_SYNTAX);
    else
    /* check MaxAjoinsPerUser */
    if(MaxAjoinsPerUser && (ajoins_count(source_snid) >= MaxAjoinsPerUser))
      send_lang(u, s, CS_AJOIN_MAX_X_REACHED, MaxAjoinsPerUser);
    else
    /* check if chan exists */
    if((cr = OpenCR(chname)) == NULL)
      send_lang(u, s, CHAN_X_NOT_REGISTERED, chname);
    else
    if(sql_singlequery("SELECT scid FROM cs_ajoin WHERE snid=%d AND scid=%d",
      source_snid, cr->scid))
        send_lang(u, s, CS_AJOIN_X_ALREADY_IN, chname);
    else
    {
      int order_id = 0;
      if(sql_singlequery("SELECT MAX(order_id) FROM cs_ajoin WHERE snid=%d",
        source_snid) && sql_field(0))
          order_id = sql_field_i(0)+1;
      sqlb_init("cs_ajoin");
      sqlb_add_int("snid", source_snid);
      sqlb_add_int("scid", cr->scid);
      sqlb_add_int("order_id", order_id);
      if(sql_execute(sqlb_insert()))
        send_lang(u, s, CS_AJOIN_ADDED_X, chname);
      else
        send_lang(u, s, UPDATE_FAIL);
    }
    CloseCR(cr);
  }
  else
  if(strcasecmp(cmd, "DEL") == 0)
  {
    int is_all = 0;
    char *chname = strtok(NULL, " ");
    ChanRecord* cr;

    if(chname && (strcasecmp(chname, "ALL") == 0))
      is_all = 1;
      
    /* check for required params */
    if(!chname)
      send_lang(u, s, CS_AJOIN_DEL_SYNTAX);
    else
    /* check if chan exists */
    if(!is_all && (cr = OpenCR(chname)) == NULL)
      send_lang(u, s, CHAN_X_NOT_REGISTERED, chname);
    else
    if(!is_all && sql_singlequery("SELECT scid FROM cs_ajoin WHERE snid=%d AND scid=%d",
      source_snid, cr->scid) == 0)
        send_lang(u, s, CS_AJOIN_X_NOT_IN, chname);
    else
    {
      if(is_all)
      {
        sql_execute("DELETE FROM cs_ajoin WHERE snid=%d",
          source_snid);
        send_lang(u, s, CS_AJOIN_DELETED_ALL, chname);
      } 
      else
      {
        sql_execute("DELETE FROM cs_ajoin WHERE snid=%d and scid=%d",
          source_snid, cr->scid);
        send_lang(u, s, CS_AJOIN_DELETED_X, chname);
      }
    }
  }
  else
  if(strcasecmp(cmd, "LIST") == 0)
  {
    MYSQL_RES *res;
    MYSQL_ROW row;
    res = sql_query("SELECT name FROM cs_ajoin a, chanserv c"
      " WHERE a.snid=%d AND c.scid=a.scid ORDER BY order_id",  source_snid);
    send_lang(u, s, CS_AJOIN_LIST_HEADER);    
    while((row = sql_next_row(res)))
      send_lang(u, s, CS_AJOIN_LIST_ITEM_X, row[0]);
    send_lang(u, s, CS_AJOIN_LIST_TAIL);
    sql_free(res);
  }  
  else
    send_lang(u, s, CS_AJOIN_HELP);
}

void ev_cs_ajoin_nick_identify(IRC_User* u, u_int32_t *snid)
{
  MYSQL_RES *res;
  MYSQL_ROW row;
  char ajoin[256];
  int i = 0;
  IRC_Chan* chan;
  
  ajoin[0] = '\0';
  res = sql_query("SELECT name FROM cs_ajoin a, chanserv c"
    " WHERE a.snid=%d AND c.scid=a.scid ORDER BY order_id", *snid);
  while((row = sql_next_row(res)) && (i + strlen(row[0]) < 255))
  {
    chan = irc_FindChan(row[0]);
    if(chan) /* channel exists */
    {
      ChanRecord* cr;
      IRC_ChanNode* cn;
      cr = chan->sdata;
      if(!cr) /* channel is not registered ? skip ajoin */
        continue;
      cn = irc_FindOnChan(chan, u);
      if(cn) /* we are already on chan, dont ajoin */
        continue;
      cr = chan->sdata;
      if(cr && irc_IsCMode(chan, (CMODE_i | CMODE_k | CMODE_A | CMODE_O)) 
        && role_with_permission(cr->scid, *snid, P_INVITE))
           irc_ChanInvite(chan, u, csu->u);
    }
    if(i>0)
      ajoin[i++] = ',';
    i += sprintf(&ajoin[i], "%s", row[0]);
  }
  sql_free(res);
  if(ajoin[0])
  {
    send_lang(u, csu->u, CS_AJOIN_IS_X, ajoin);  
    irc_SvsJoin(u, csu->u, ajoin);
  }
}

static int sql_upgrade(int version, int post)
{
  MYSQL_RES* res;
  MYSQL_ROW row;
  
  switch(version)
  {
    case 2:
      if(!post) /* Upgrading to version 2, need to delete lost snids/scids */
      {
      	int rowc = 0;
      	res = sql_query("SELECT cs_ajoin.snid FROM cs_ajoin"
          " LEFT JOIN nickserv ON (cs_ajoin.snid = nickserv.snid)"
          " WHERE cs_ajoin.snid IS NOT NULL AND nickserv.snid IS NULL");
      	while((row = sql_next_row(res)))
      	{
          log_log(cs_log, mod_info.name,
            "Deleting ajoins owned by deleted nick %s", row[0]);
          sql_execute("DELETE FROM cs_ajoin WHERE snid=%s", row[0]);
          ++rowc;
        }
        
        if(rowc)
          log_log(cs_log, mod_info.name, "Removed %d lost ajoin(s)", rowc);
      	sql_free(res);
      	rowc = 0;
      	res = sql_query("SELECT cs_ajoin.scid FROM cs_ajoin"
          " LEFT JOIN chanserv ON (cs_ajoin.scid = chanserv.scid)"
          " WHERE cs_ajoin.scid IS NOT NULL AND chanserv.scid IS NULL");
      	while((row = sql_next_row(res)))
      	{
          log_log(cs_log, mod_info.name,
            "Deleting ajoins on deleted chan %s", row[0]);
          sql_execute("DELETE FROM cs_ajoin WHERE scid=%s", row[0]);
          ++rowc;
        }
        if(rowc)
          log_log(cs_log, mod_info.name, "Deleted %d lost ajoins(s)", rowc);
        sql_free(res);
      }    
  }    
  return 1;
}
