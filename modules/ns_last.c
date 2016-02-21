/******************************************************************
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License           *
 * Please read the file COPYING for copyright information.        *
 ******************************************************************
                                                                                
  File: ns_last.c
  Description: nickserv last/slast command
                                                                                
 *  $Id: ns_last.c,v 1.6 2005/10/18 16:25:06 jpinto Exp $
*/

#include "module.h"
#include "dbconf.h"
#include "nsmacros.h"
#include "my_sql.h"
#include "ns_group.h"
/* lang files */
#include "lang/common.lh"
#include "lang/ns_last.lh"

SVS_Module mod_info =
/* module, version, description */
{ "ns_last", "1.0","nickserv last/slast command" };

/* Change Log
  1.0 -  #2 : ns_last module to record/display nicks login activity
*/

#define DB_VERSION 1

/* external functions/events we require */
ServiceUser* (*nickserv_suser)(void);
u_int32_t (*find_group)(char *name);
static int e_nick_identify;
static int e_nick_register;
MOD_REQUIRES
  DBCONF_FUNCTIONS
  MOD_FUNC(nickserv_suser)
  MOD_FUNC(is_sadmin)
  MOD_FUNC(find_group)  
  MOD_FUNC(e_nick_identify)
  MOD_FUNC(e_nick_register)
MOD_END

static int ExpireTime;

/* configuration items we provide */
DBCONF_PROVIDES
  DBCONF_TIME(ExpireTime, "1M", 
    "How long information from last logins will be kept ?\n"
    "Please note that old logins info is only deleted during nick logins")
DBCONF_END

/* internal functions */
int ev_ns_last_nick_identify(IRC_User* u, u_int32_t* snid);

/* available commands from module */

void ns_last(IRC_User *s, IRC_User *u);
void ns_slast(IRC_User *s, IRC_User *u);

/* Local settings */

/* Local vars */
ServiceUser* nsu;
int ns_log;

int mod_rehash(void)
{
  if(dbconf_get_or_build(mod_info.name, dbconf_provides) < 0 )
  {
    errlog("Error reading dbconf!");
    return -1;
  }
  return 0;
}

int mod_load(void)
{
  int r = sql_check_inst_upgrade(mod_info.name, DB_VERSION, NULL);  
  if(r < 0)
    return -1;  
  ns_log = log_handle("nickserv");
  nsu = nickserv_suser();  
  /* add commands */
  suser_add_cmd(nsu, "LAST", ns_last, NS_LAST_SUMMARY, NS_LAST_HELP);
  suser_add_cmd_g(nsu, "SLAST", ns_slast, NS_SLAST_SUMMARY, NS_SLAST_HELP,
    find_group("Admin"));
        
  /* add events */
  mod_add_event_action(e_nick_identify, 
    (ActionHandler) ev_ns_last_nick_identify);
  mod_add_event_action(e_nick_register, 
    (ActionHandler) ev_ns_last_nick_identify);
  return 0;
}

void mod_unload(void)
{
  suser_del_mod_cmds(nsu, &mod_info);     
}

/* s = service the command was sent to
   u = user the command was sent from */
void ns_last(IRC_User *s, IRC_User *u)
{
  MYSQL_RES *res;
  MYSQL_ROW row;
  u_int32_t source_snid;

  CHECK_IF_IDENTIFIED_NICK
  
  send_lang(u, s, NS_LAST_HEADER);
  res = sql_query("SELECT t_when, username, realhost, realname, web "
    "FROM ns_last WHERE snid=%d ORDER BY t_when DESC", source_snid);
  while((row = sql_next_row(res)))
  {
    char buf[64];
    char buf2[128];
    struct tm *tm;
    time_t t_when = atoi(row[0]);
    tm = localtime(&t_when);    
    strftime(buf, sizeof(buf), format_str(u, DATE_FORMAT), tm);
    if(row[1] && row[2])
      snprintf(buf2, sizeof(buf2), "%s@%s", row[1], row[2]);
    else if(row[2])
      snprintf(buf2, sizeof(buf2), "%s", row[2]);
    else
      buf2[0] = '\0';
    if(row[4] && (*row[4] == 'y'))
      strcat(buf2, " (Web)");
    send_lang(u, s, NS_LAST_ITEM_X_X_X,
      buf, buf2, row[3] ? row[3] : "");
  }
  send_lang(u, s, NS_LAST_TAIL);
  
  sql_free(res);  
}

/* s = service the command was sent to
   u = user the command was sent from */
void ns_slast(IRC_User *s, IRC_User *u)
{
  MYSQL_RES *res;
  MYSQL_ROW row;
  u_int32_t source_snid;
  char *nick;

  CHECK_IF_IDENTIFIED_NICK

  if(!is_sadmin(source_snid))
    send_lang(u, s, ONLY_FOR_SADMINS);  
  else
  if((nick = strtok(NULL, " ")) == NULL)
    send_lang(u,s, NS_SLAST_SYNTAX);
  else 
  if(sql_singlequery("SELECT snid FROM nickserv WHERE nick=%s",
    sql_str(irc_lower_nick(nick))) == 0)
      send_lang(u, s, NICK_X_NOT_REGISTERED, nick);
  else
  {   
    u_int32_t snid = sql_field_i(0);
    send_lang(u, s, NS_LAST_HEADER_X, nick);
    res = sql_query("SELECT t_when, username, realhost, realname, web "
      "FROM ns_last WHERE snid=%d ORDER BY t_when", snid);
    while((row = sql_next_row(res)))
    {
      char buf[64];
      char buf2[128];
      struct tm *tm;
      time_t t_when = atoi(row[0]);
      tm = localtime(&t_when);    
      strftime(buf, sizeof(buf), format_str(u, DATE_FORMAT), tm);
      if(row[1] && row[2])
        snprintf(buf2, sizeof(buf2), "%s@%s", row[1], row[2]);
      else 
      if(row[2])
        snprintf(buf2, sizeof(buf2), "%s", row[2]);
      else
        buf2[0] = '\0';
      if(row[4] && (*row[4] == 'y'))
      strcat(buf2, "Web");
      send_lang(u, s, NS_LAST_ITEM_X_X_X,
      buf, buf2, row[3] ? row[3] : "");
    }
    send_lang(u, s, NS_LAST_TAIL);
    sql_free(res);      
  }
}

/* nick has identified, record nick info */
int ev_ns_last_nick_identify(IRC_User* u, u_int32_t* snid)
{
  /* first lets delete the old login info */
  sql_execute("DELETE FROM ns_last WHERE snid=%d AND t_when < %d",
    *snid, irc_CurrentTime - ExpireTime);
  sqlb_init("ns_last");
  sqlb_add_int("snid", *snid);
  sqlb_add_str("web", "n");
  sqlb_add_int("t_when", (int) irc_CurrentTime);
  sqlb_add_str("username", u->username);
  sqlb_add_str("publichost", u->publichost);
  sqlb_add_str("realhost", u->realhost);
  sqlb_add_str("realname", u->info);      
  return sql_execute("%s", sqlb_insert());
}
