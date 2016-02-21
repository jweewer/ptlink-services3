/******************************************************************
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999-2006 *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License           *
 * Please read the file COPYING for copyright information.        *
 ******************************************************************
                                                                                
  Description: nickserv suspend/unsuspend command
                                                                                
 *  $Id: ns_suspend.c,v 1.6 2006/10/27 22:27:02 jpinto Exp $
*/

#include "module.h"
#include "dbconf.h"
#include "nsmacros.h"
#include "my_sql.h"
#include "nickserv.h"
#include "ns_group.h"
/* lang files */
#include "lang/ns_suspend.lh"
#include "lang/common.lh"

SVS_Module mod_info =
/* module, version, description */
{ "ns_suspend", "1.1","nickserv suspend/unsuspend command" };

/* Change Log
  1.1 -	#60: ns_suspend must check and change the target if it is online
  1.0 - #10 : add nickserv suspensions 
*/

/* external functions/events we require */
ServiceUser* (*nickserv_suser)(void);
u_int32_t (*find_group)(char *name);
MOD_REQUIRES
  DBCONF_FUNCTIONS
  MOD_FUNC(nickserv_suser)
  MOD_FUNC(is_sadmin)
  MOD_FUNC(find_group)  
MOD_END

/* dbconf we require */
static int ExpireTime;
static char* NickProtectionPrefix;
static int MaxProtectionNumber;

DBCONF_REQUIRES
  DBCONF_GET("nickserv", ExpireTime)
  DBCONF_GET("nickserv", NickProtectionPrefix)
  DBCONF_GET("nickserv", MaxProtectionNumber)
DBCONF_END
  
/* available commands from module */
void ns_suspend(IRC_User *s, IRC_User *u);
void ns_unsuspend(IRC_User *s, IRC_User *u);

/* Local settings */

/* Local vars */
ServiceUser* nsu;
int ns_log;

int mod_rehash(void)
{
  if(dbconf_get(dbconf_requires) < 0 )
  {
    errlog("Error reading dbconf!");
    return -1;
  }              
  return 0;
}

int mod_load(void)
{
  ns_log = log_handle("nickserv");
  nsu = nickserv_suser();  
  /* add commands */
  suser_add_cmd_g(nsu, "SUSPEND", ns_suspend, 
    NS_SUSPEND_SUMMARY, NS_SUSPEND_HELP,  find_group("Admin"));
  suser_add_cmd_g(nsu, "UNSUSPEND", ns_unsuspend, 
    NS_UNSUSPEND_SUMMARY, NS_UNSUSPEND_HELP,  find_group("Admin"));    
  return 0;
}

void mod_unload(void)
{
  suser_del_mod_cmds(nsu, &mod_info);     
}

/*** modules functions implementation */

/* is_suspended
 * params:
 *	snid
 * returns:
 *	>0 if nick is suspended
 *	0 if not
 */
static 
int is_suspended(u_int32_t snid)
{
  return sql_singlequery("SELECT snid FROM nickserv_suspensions WHERE snid=%d",snid);
}

/* add_suspension
 * params:
 *   who adds the suspension
 *   snid to be added
 *   time (duration of the suspension)
 *   reason
 * returns:
 *   1 if succesfully added
 *   0 if there was an error
 */
static 
int add_suspension(char *who, u_int32_t snid, int itime, char *reason)
{
  sqlb_init("nickserv_suspensions");
  sqlb_add_int("snid", snid);
  sqlb_add_str("who", who);
  sqlb_add_int("t_when", irc_CurrentTime);
  sqlb_add_int("duration", itime);
  sqlb_add_str("reason", reason);
  if(sql_execute(sqlb_insert()) == 1)
  {
    sql_execute("UPDATE nickserv SET flags=(flags | %d), t_expire=%d " 
    "WHERE snid=%d", NFL_SUSPENDED, irc_CurrentTime + itime + ExpireTime, snid); 
    return 1;
  }
  return 0;
}

static
int del_suspension(u_int32_t snid)
{
  if(sql_execute("DELETE FROM nickserv_suspensions WHERE snid=%d", snid))
  {
    sql_execute("UPDATE nickserv SET flags = (flags & ~%d), t_expire=%d "
      "WHERE snid=%d", NFL_SUSPENDED, irc_CurrentTime+ExpireTime, snid);
    return 1;
  }
  return 0;
}
    
/* s = service the command was sent to
   u = user the command was sent from */
void ns_suspend(IRC_User *s, IRC_User *u)
{
  u_int32_t source_snid;
  u_int32_t snid;
  char *nick;
  char *reason;
  int duration;
  
  CHECK_IF_IDENTIFIED_NICK
  nick = strtok(NULL, " ");
  CHECK_DURATION(nick)
  reason = strtok(NULL, "");
    
  if(!is_sadmin(source_snid))
    send_lang(u, s, ONLY_FOR_SADMINS);
  else
  if(nick && (strcasecmp(nick, "LIST") == 0))
  { /* list command is implemented here */
    MYSQL_RES *res;
    MYSQL_ROW row;
    send_lang(u, s, NS_SUSPEND_LIST_HEADER);
    res = sql_query("SELECT n.nick, ns.who, ns.t_when, ns.duration, ns.reason "
      "FROM nickserv n, nickserv_suspensions ns "
      "WHERE n.snid=ns.snid ORDER BY ns.t_when DESC");
    while((row = sql_next_row(res)))
    {
      char buf[64];
      struct tm *tm;
      time_t t_when = atoi(row[2]);
      int durationt = atoi(row[3]);
      int to_expire = t_when + durationt - irc_CurrentTime;
      tm = localtime(&t_when);
      strftime(buf, sizeof(buf), format_str(u, DATE_FORMAT), tm);      
      if(durationt > 0)
        send_lang(u, s, NS_SUSPEND_LIST_ITEM_X_X_X_X_X,
          row[0], row[1], buf, row[4], (to_expire/(24*3600))+1);
      else
        send_lang(u, s, NS_SUSPEND_LIST_ITEM_X_X_X_X_FOREVER,
          row[0], row[1], buf, row[4]);
    }
    sql_free(res);
    send_lang(u, s, NS_SUSPEND_LIST_TAIL);
    return;
  }
  else
  if(!nick || !reason)
    send_lang(u,s, NS_SUSPEND_SYNTAX);
  else
  if((snid = nick2snid(nick)) == 0)
    send_lang(u, s, NICK_X_NOT_REGISTERED, nick);
  else
  if(is_suspended(snid))
     send_lang(u, s, NS_SUSPEND_X_ALREADY_SUSPENDED, nick);
  else
  {
    IRC_User *target_u = irc_FindUser(nick);
    if(target_u)
      irc_SvsGuest(target_u, nsu->u, NickProtectionPrefix, MaxProtectionNumber);

    if(add_suspension(u->nick, snid, duration, reason))
      send_lang(u, s, NICK_X_SUSPENDED, nick);
    else
      send_lang(u, s, UPDATE_FAIL);
  }
}

/* s = service the command was sent to
   u = user the command was sent from */
void ns_unsuspend(IRC_User *s, IRC_User *u)
{
  u_int32_t source_snid;
  u_int32_t snid;
  char *nick;
  
  CHECK_IF_IDENTIFIED_NICK
  nick = strtok(NULL, " ");
  
  if(!is_sadmin(source_snid))
    send_lang(u, s, ONLY_FOR_SADMINS);
  else
  if(!nick)
    send_lang(u,s, NS_SUSPEND_SYNTAX);
  else
  if((snid = nick2snid(nick)) == 0)
    send_lang(u, s, NICK_X_NOT_REGISTERED, nick);
  else
  if(!is_suspended(snid))
     send_lang(u, s, NS_SUSPEND_X_NOT_SUSPENDED, nick);
  else
  {
    if(del_suspension(snid))
      send_lang(u, s, NS_UNSUPEND_REMOVED_X, nick);
    else
      send_lang(u, s, UPDATE_FAIL);
  }
}
