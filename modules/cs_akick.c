/******************************************************************
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License           *
 * Please read the file COPYING for copyright information.        *
 ******************************************************************
                                                                                
  File: cs_akick.c
  Description: chanserv akick command
                                                                                
 *  $Id: cs_akick.c,v 1.7 2005/10/18 16:25:06 jpinto Exp $
*/

#include "module.h"
#include "chanserv.h"
#include "chanrecord.h"
#include "my_sql.h"
#include "cs_role.h"  /* We need P_AKICK */
#include "nsmacros.h"
#include "nickserv.h" /* Authenticated ? */
#include "dbconf.h"
/* lang files */
#include "lang/common.lh"
#include "lang/cscommon.lh"
#include "lang/cs_akick.lh"

SVS_Module mod_info =
/* module, version, description */
{"cs_akick", "3.2", "chanserv akick command" };

#define DB_VERSION	2

/* Change Log
  3.2 - #73, crash on cs_akick del all on chan without akicks
  3.1 - #53: CS AKICK DEL ALL
  3.0 - 0000305: foreign keys for data integrity
        Added cs_akick.2.sql changes          
  2.0 - 0000293: invalid sql string on cs_akick list with mask
      - 0000282: akick mask is not on the standard nick!user@host format
      - 0000265: remove nickserv cache system
      - 0000281: No auth nicks can't use chanserv
      - 0000280: expire date on akick list
*/

/* external functions we need */
ServiceUser* (*chanserv_suser)(void);
int e_regchan_join = 0;
int e_expire = 0;

MOD_REQUIRES
  MOD_FUNC(dbconf_get)
  MOD_FUNC(dbconf_get_or_build)
  MOD_FUNC(e_expire) /* we need this to run the expire routines */
  MOD_FUNC(role_with_permission)
  MOD_FUNC(chanserv_suser)
  MOD_FUNC(e_regchan_join)    
MOD_END

/* internal functions */
int ev_cs_akick_chan_join(ChanRecord *cr, IRC_ChanNode* cn);
void load_akicks_for(ChanRecord* cr);
int find_akick(ChanRecord* cr, char *mask);
int mysql_insert_akick(u_int32_t scid, char* mask, char* reason, int duration, char *who);
int mysql_delete_akick(u_int32_t scid, char* mask);
char* match_akick(ChanRecord *cr, char *usermask);
void ev_cs_akick_timer_part(IRC_Chan* chan, int tag);
int ev_cs_akick_expire(void* dummy1, void* dummy2);
int akick_count(u_int32_t scid);
int sql_upgrade(int version, int post);
        
/* available commands from module */
void cs_akick(IRC_User *s, IRC_User *u);

/* Local settings */
int ChanServNeedsAuth = 0;

/* Local config */
static int MaxAkicksPerChan;

DBCONF_PROVIDES
  DBCONF_INT(MaxAkicksPerChan, "20", "Max. number of of akicks per channel")
DBCONF_END
/* Remote config */
static int NeedsAuth;
DBCONF_REQUIRES
  DBCONF_GET("chanserv", NeedsAuth)
DBCONF_END

/* this is called before load and at services rehash */
int mod_rehash(void)
{
  if(dbconf_get(dbconf_requires) < 0)
  {
    errlog("Required configuration item is missing!");
    return -1;
  }
  if(dbconf_get_or_build(mod_info.name, dbconf_provides) < 0 )
  {
    errlog("Error reading dbconf!");
    return -1;
  }
  return 0;
}

/* Local variables */
ServiceUser* csu;
int cs_log = 0;


int mod_load(void)
{
  int r;
  r = sql_check_inst_upgrade(mod_info.name, DB_VERSION, sql_upgrade);
  if(r == 1) /* module was installed */
  { /* lets add the permission to the admin role */
    sql_execute("UPDATE cs_role SET perms=(perms | %d) "
      "WHERE master_rid=0", P_AKICK);
  }
  
  if( r < 0 )
    return -1;

  cs_log = log_handle("chanserv");
  csu = chanserv_suser();  
  /* Add action handlers */
  mod_add_event_action(e_regchan_join, (ActionHandler) ev_cs_akick_chan_join);
  mod_add_event_action(e_expire, (ActionHandler) ev_cs_akick_expire);
  ev_cs_akick_expire(NULL, NULL);
  /* Add command */
  suser_add_cmd(csu, "AKICK", cs_akick, AKICK_SUMMARY, AKICK_HELP);

  return 0;
}

void mod_unload(void)
{
  suser_del_mod_cmds(csu, &mod_info);
}

/* s = service the command was sent to
   u = user the command was sent from */
void cs_akick(IRC_User *s, IRC_User *u)
{
  u_int32_t source_snid;
  char *chname, *cmd;
  ChanRecord *cr = NULL;
  
  chname = strtok(NULL, " ");
  cmd = strtok(NULL, " "); 

  /* status validation */   
  CHECK_IF_IDENTIFIED_NICK
  
  /* base syntax validation */
  if(ChanServNeedsAuth && !IsAuthenticated(u))
    send_lang(u, s, NEEDS_AUTH_NICK);
  else
  if(IsNull(chname) || IsNull(cmd))
    send_lang(u, s, CHAN_AKICK_SYNTAX);
  else if((cr = OpenCR(chname)) == NULL)
    send_lang(u, s, CHAN_X_NOT_REGISTERED, chname);
  else
  if(role_with_permission(cr->scid, source_snid, P_AKICK) == 0)
    send_lang(u, s, NO_AKICK_PERM_ON_X, chname);
  else if(strcasecmp(cmd, "ADD") == 0) /* Add akick */
  {
    char buf[NICKLEN+HOSTLEN+USERLEN+3];
    char *mask, *reason;
    char *nick = NULL, *user = NULL, *host  = NULL;
    int duration = 0;
    buf[0] = '\0'; /* just to be safe */
    mask = NULL;
    mask = strtok(NULL, " ");   
    if(mask)
      reason = strtok(NULL, "");
    else
      reason = NULL;
      
    if(mask)
    {
      host = strchr(mask,'@');
      if(host)
      {
        *host ='\0';
        host++;
      }
      user = strchr(mask, '!');
      if(user)
      {	
        *user ='\0';
        user++;
        nick = mask;
      } 
      else 
      {
        if(host == NULL)
          nick = mask; 
        else
          user =mask;
      }
      if(host == NULL || (*host == '\0'))
        host = "*";
      if((user == NULL) || (*user == '\0'))
        user = "*";
      if((nick == NULL) || (*nick == '\0'))
        nick = "*";        
      snprintf(buf, sizeof(buf), "%s!%s@%s", nick, user, host);
      collapse(buf);
    }
    
    
    if(reason)
      duration = strip_reason(&reason);
    else
      duration = 5*24*3600; /* default akick time is one week */
      
    /* syntax validation */
    if( IsNull(mask))
      send_lang(u, s, CHAN_AKICK_ADD_SYNTAX);
    /* privileges validation */
    /* check requirements */
    else if(MaxAkicksPerChan && akick_count(cr->scid) >= MaxAkicksPerChan)
      send_lang(u, s, REACHED_MAX_AKICK_X, MaxAkicksPerChan);
    else if(find_akick(cr, buf))
      send_lang(u, s, ALREADY_AKICK_X_X, mask, cr->name);
    else
    {
      
      if(mysql_insert_akick(cr->scid, buf, reason, duration, u->nick) > 0)
      {
        send_lang(u, s, ADDED_AKICK_X_X, buf, cr->name);
        if(cr->extra[ED_AKICKS] == NULL) /* it may be the first akick */
        {
          cr->extra[ED_AKICKS] = malloc(sizeof(darray));
          array_init(cr->extra[ED_AKICKS], 1, DA_STRING);
        }
        array_add_str(cr->extra[ED_AKICKS], buf);
      }
      else
        send_lang(u, s, UPDATE_FAIL);
    }    
  }
  else if(strcasecmp(cmd, "DEL") == 0) /* Del akick */
  {
    int is_all = 0;
    char *mask = strtok(NULL , " ");    
    
    if(mask && (strcasecmp(mask, "ALL") == 0))
      is_all = 1;
    
    /* syntax validation */
    if(IsNull(mask))
      send_lang(u, s, CHAN_AKICK_DEL_SYNTAX);
    /* privileges validation */
    /* check requirements */
    else if(!is_all && (find_akick(cr, mask) == 0))
      send_lang(u, s, AKICK_X_X_NOT_FOUND, mask, cr->name);
    else
      {
        mysql_delete_akick(cr->scid, mask);
        if(is_all)
        {
          send_lang(u, s, DELETED_AKICK_X_ALL, cr->name);
          if(cr->extra[ED_AKICKS])
            array_delall_str(cr->extra[ED_AKICKS]);
        }
        else
        {
          send_lang(u, s, DELETED_AKICK_X_X, mask, cr->name);
          array_del_str(cr->extra[ED_AKICKS], mask);
        }
      }
    }
  else if(strcasecmp(cmd, "LIST") == 0) /* List akicks */
  {  
    char *mask;
    MYSQL_RES* res;
    MYSQL_ROW row;
    char *c;
    
    mask = strtok(NULL, " ");
    if(mask)
    {
      /* replace '*' with '%' for sql */
      while((c = strchr(mask,'*'))) *c='%';                
      res = sql_query("SELECT mask, message, t_when+duration FROM cs_akick "
        "WHERE scid=%d AND "
        "mask LIKE %s", cr->scid, sql_str(mask));
    }
    else
      res = sql_query("SELECT mask, message, t_when+duration FROM cs_akick WHERE scid=%d",
        cr->scid);
    if(res)
       send_lang(u, s, AKICK_LIST_HEADER_X, mysql_num_rows(res));
    
    while((row = sql_next_row(res)))
    {
      char buf[64];
      struct tm *tm;
      time_t t_expire = atoi(row[2]);
	
      tm=localtime(&t_expire);
      strftime(buf, sizeof(buf), format_str(u, DATE_FORMAT), tm);
      send_lang(u, s, AKICK_LIST_X_X_X, row[0], row[1] ? row[1]: "", buf);
    }
    send_lang(u, s, AKICK_LIST_TAIL);
    sql_free(res);
  }
  else
    send_lang(u, s, CHAN_AKICK_SYNTAX);
  CloseCR(cr);
}

/* this is called when a nick joins a registered channel */
int ev_cs_akick_chan_join(ChanRecord *cr, IRC_ChanNode* cn)
{
  char* akick;
  if(cr->extra[ED_AKICKS] == NULL)
    load_akicks_for(cr);
  
  akick = match_akick(cr, irc_UserMaskP(cn->user));
  if(akick) /* we have an akick for the user joining */
  {
    char* reason = NULL;	    
    IRC_Chan* chan;
     
    if((chan = irc_FindChan(cr->name)) == NULL)
      abort(); /* this should never happen */
          
    if(sql_singlequery("SELECT message FROM cs_akick WHERE "
        "scid=%d AND mask=%s", cr->scid, sql_str(akick)) < 1)    
    {
      /* this akick is no longer on the db */
      array_del_str(cr->extra[ED_AKICKS], akick);
      return 0;
    }
      
    reason = sql_field(0);
        
    if(chan->users_count == 1)
    {
      irc_ChanJoin(csu->u, cr->name, CU_MODE_ADMIN|CU_MODE_OP);
      irc_AddCTimerEvent(chan, 30 , ev_cs_akick_timer_part, 0);
    }
    irc_ChanMode(chan->local_user ? chan->local_user : csu->u, chan,
      "+b %s", akick);      
    irc_Kick(chan->local_user ? chan->local_user : csu->u, chan, cn->user,
      reason ? reason : "AKICK");
    mod_abort_event();
  }
  return 0;
}

/* Load akicks for a given channel */
void load_akicks_for(ChanRecord* cr)
{
  MYSQL_RES* res;
  MYSQL_ROW row;
  int rowc = 0;
  res = sql_query("SELECT mask FROM cs_akick WHERE scid=%d",
    cr->scid);
  if(res)
    rowc = mysql_num_rows(res);
  if(cr->extra[ED_AKICKS] != NULL)
    array_free(cr->extra[ED_AKICKS]);
  cr->extra[ED_AKICKS] = malloc(sizeof(darray));
  array_init(cr->extra[ED_AKICKS], rowc, DA_STRING);
  while((row = sql_next_row(res)))
    array_add_str(cr->extra[ED_AKICKS], row[0]);
  sql_free(res);  
}

/* find for a given akick mask on a given channel */
int find_akick(ChanRecord* cr, char *mask)
{
  if(cr->extra[ED_AKICKS] == NULL)
  {
    load_akicks_for(cr);
    return (array_find_str(cr->extra[ED_AKICKS], mask) != -1);
  }
  else
  {
    if(array_find_str(cr->extra[ED_AKICKS], mask) != -1)
    {
      if(sql_singlequery("SELECT mask FROM cs_akick WHERE "
        "scid=%d AND mask=%s", cr->scid, sql_str(mask)) < 1)
      {
        /* this akick is no longer on the db */
        array_del_str(cr->extra[ED_AKICKS], mask);
        return 0;
      }
      return 1;
    }
  }
  return 0;
}

char* match_akick(ChanRecord *cr, char *usermask)
{
  if(cr->extra[ED_AKICKS] == NULL)
    load_akicks_for(cr);
  return array_match_str(cr->extra[ED_AKICKS], usermask);
}

int mysql_insert_akick(u_int32_t scid, char* mask, char* message, int duration, 
  char *who)
{
  return sql_execute("INSERT INTO cs_akick VALUES(%d, %s, %s, %s, %d, %d)",
    scid, sql_str(mask), sql_str(message), sql_str(who), 
      irc_CurrentTime, duration);
}

int mysql_delete_akick(u_int32_t scid, char* mask)
{
  if(strcasecmp(mask, "all") == 0)
    return sql_execute("DELETE FROM cs_akick WHERE scid=%d",
      scid);  
  else
    return sql_execute("DELETE FROM cs_akick WHERE scid=%d AND mask=%s",
      scid, sql_str(mask));
}

void ev_cs_akick_timer_part(IRC_Chan* chan, int tag)
{
  /* check if we are still on chan */
    if(chan->local_user == csu->u)
        irc_ChanPart(csu->u, chan);
}

int ev_cs_akick_expire(void* dummy1, void* dummy2)
{
  return sql_execute("DELETE FROM cs_akick WHERE t_when+duration<%d",
    irc_CurrentTime);
}


/**
  returns the number of roles that exist for a given channel
*/
int akick_count(u_int32_t scid)
{
  int count;
  count = sql_singlequery("SELECT count(*) FROM cs_akick WHERE scid=%d", scid);
  if(count > 0)
    return atoi(sql_field(0));
  return 0;
}


/* this version takes care of sql upgrades */
int sql_upgrade(int version, int post)
{
  MYSQL_RES* res;
  MYSQL_ROW row;
  
  switch(version)
  {
    case 2: /* pre-validation of integrity rules */
    if(!post) /* we need to check for "lost" memos */
    {
      int rowc = 0;
      res = sql_query("SELECT cs_akick.mask, cs_akick.scid FROM cs_akick"
        " LEFT JOIN chanserv ON (cs_akick.scid = chanserv.scid)"
        " WHERE cs_akick.scid IS NOT NULL AND chanserv.scid IS NULL");
      while((row = sql_next_row(res)))
      {
        if(row[0] == 0) /* this will be set to null */
          continue;
        log_log(cs_log, mod_info.name, "Removing lost akick %s on %s",
          row[0], row[1]);
        sql_execute("DELETE FROM cs_akick WHERE scid=%s",
          row[1]);
        ++rowc;
      }
      if(rowc)
        log_log(cs_log, mod_info.name, "Removed %d lost akick(s)", rowc);
      sql_free(res);
    }
  }    
  return 1;
}
/* End of Module */
