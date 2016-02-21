/******************************************************************
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public Licensie          *
 * Please read the file COPYING for copyright information.        *
 ******************************************************************
                                                                                
  Description: chanserv suspend/unsuspend command
                                                                                
*/

#include "module.h"
#include "dbconf.h"
#include "nsmacros.h"
#include "my_sql.h"
#include "chanserv.h"
#include "ns_group.h"
/* lang files */
#include "lang/cs_suspend.lh"
#include "lang/common.lh"
#include "lang/cscommon.lh"

SVS_Module mod_info =
/* module, version, description */
{ "cs_suspend", "1.0","chanserv suspend/unsuspend command" };

/* Change Log
  1.0 -	  #26: Added chanserv suspensions
*/

/* external functions/events we require */
ServiceUser* (*chanserv_suser)(void);
u_int32_t (*find_group)(char *name);
MOD_REQUIRES
  DBCONF_FUNCTIONS
  MOD_FUNC(chanserv_suser)
  MOD_FUNC(is_sadmin)
  MOD_FUNC(find_group)  
MOD_END

/* dbconf we require */
static int ExpireTime;
DBCONF_REQUIRES
  DBCONF_GET("chanserv", ExpireTime)
DBCONF_END
  
/* available commands from module */
void cs_suspend(IRC_User *s, IRC_User *u);
void cs_unsuspend(IRC_User *s, IRC_User *u);

/* Local settings */

/* Local vars */
ServiceUser* csu;
int cs_log;

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
  cs_log = log_handle("chanserv");
  csu = chanserv_suser();  
  /* add commands */
  suser_add_cmd_g(csu, "SUSPEND", cs_suspend, 
    CS_SUSPEND_SUMMARY, CS_SUSPEND_HELP,  find_group("Admin"));
  suser_add_cmd_g(csu, "UNSUSPEND", cs_unsuspend, 
    CS_UNSUSPEND_SUMMARY, CS_UNSUSPEND_HELP,  find_group("Admin"));    
  return 0;
}

void mod_unload(void)
{
  suser_del_mod_cmds(csu, &mod_info);     
}

/*** modules functions implementation */

/* is_suspended
 * params:
 *	scid
 * returns:
 *	>0 if chan is suspended
 *	0 if not
 */
static 
int is_suspended(u_int32_t scid)
{
  return sql_singlequery("SELECT scid FROM chanserv_suspensions WHERE scid=%d",scid);
}

/* add_suspension
 * params:
 *   who adds the suspension
 *   scid to be added
 *   time (duration of the suspension)
 *   reason
 * returns:
 *   1 if succesfully added
 *   0 if there was an error
 */
static 
int add_suspension(char *who, u_int32_t scid, int itime, char *reason)
{
  sqlb_init("chanserv_suspensions");
  sqlb_add_int("scid", scid);
  sqlb_add_str("who", who);
  sqlb_add_int("t_when", irc_CurrentTime);
  sqlb_add_int("duration", itime);
  sqlb_add_str("reason", reason);
  if(sql_execute(sqlb_insert()) == 1)
  {
    sql_execute("UPDATE chanserv SET flags=(flags | %d) "
      "WHERE scid=%d", CFL_SUSPENDED, scid);
/*      
    sql_execute("UPDATE chanserv SET flags=(flags | %d), t_expire=%d "
      "WHERE scid=%d", CFL_SUSPENDED, irc_CurrentTime + itime + ExpireTime, scid);
*/      
    return 1;
  }
  return 0;
}

static
int del_suspension(u_int32_t scid)
{
  if(sql_execute("DELETE FROM chanserv_suspensions WHERE scid=%d", scid))
  {
    sql_execute("UPDATE chanserv SET flags = (flags & ~%d) "
      "WHERE scid=%d", CFL_SUSPENDED, scid);
    /*
    sql_execute("UPDATE chanserv SET flags = (flags & ~%d), t_expire=%d "
      "WHERE scid=%d", CFL_SUSPENDED, irc_CurrentTime+ExpireTime, scid);      
    */
    return 1;
  }
  return 0;
}

/* s = service the command was sent to
   u = user the command was sent from */
void cs_suspend(IRC_User *s, IRC_User *u)
{
  u_int32_t source_snid;
  u_int32_t scid;
  char *chan;
  char *reason;
  int duration;
  
  CHECK_IF_IDENTIFIED_NICK
  chan = strtok(NULL, " ");
  CHECK_DURATION(chan)
  reason = strtok(NULL, "");
    
  if(!is_sadmin(source_snid))
    send_lang(u, s, ONLY_FOR_SADMINS);
  else
  if(chan && (strcasecmp(chan, "LIST") == 0))
  { /* list command is implemented here */
    MYSQL_RES *res;
    MYSQL_ROW row;
    send_lang(u, s, CS_SUSPEND_LIST_HEADER);
    res = sql_query("SELECT c.name, cs.who, cs.t_when, cs.duration, cs.reason "
      "FROM chanserv c, chanserv_suspensions cs "
      "WHERE c.scid=cs.scid ORDER BY cs.t_when DESC");
    while((row = sql_next_row(res)))
    {
      char buf[64];
      struct tm *tm;
      time_t t_when = atoi(row[2]);
      int iduration = atoi(row[3]);
      int to_expire = t_when + iduration - irc_CurrentTime;
      tm = localtime(&t_when);
      strftime(buf, sizeof(buf), format_str(u, DATE_FORMAT), tm);
      if(iduration > 0)
        send_lang(u, s, CS_SUSPEND_LIST_ITEM_X_X_X_X_X,
          row[0], row[1], buf, row[4], (to_expire/(24*3600))+1);
      else
        send_lang(u, s, CS_SUSPEND_LIST_ITEM_X_X_X_X_FOREVER,
          row[0], row[1], buf, row[4]);
    }
    sql_free(res);
    send_lang(u, s, CS_SUSPEND_LIST_TAIL);
    return;
  }
  else
  if(!chan || !reason)
    send_lang(u,s, CS_SUSPEND_SYNTAX);
  else
  if((scid = chan2scid(chan)) == 0)
    send_lang(u, s, CHAN_X_NOT_REGISTERED, chan);
  else
  if(is_suspended(scid))
     send_lang(u, s, CS_SUSPEND_X_ALREADY_SUSPENDED, chan);
  else
  {
    if(add_suspension(u->nick, scid, duration, reason))
    {
      ChanRecord *cr = OpenCR(chan);
      if(cr)
        SetSuspendedChan(cr);
      send_lang(u, s, CHAN_X_SUSPENDED, chan);
    }
    else
      send_lang(u, s, UPDATE_FAIL);
  }
}

/* s = service the command was sent to
   u = user the command was sent from */
void cs_unsuspend(IRC_User *s, IRC_User *u)
{
  u_int32_t source_snid;
  u_int32_t scid;
  char *chan;
  
  CHECK_IF_IDENTIFIED_NICK
  chan = strtok(NULL, " ");
  
  if(!is_sadmin(source_snid))
    send_lang(u, s, ONLY_FOR_SADMINS);
  else
  if(!chan)
    send_lang(u,s, CS_SUSPEND_SYNTAX);
  else
  if((scid = chan2scid(chan)) == 0)
    send_lang(u, s, CHAN_X_NOT_REGISTERED, chan);
  else
  if(!is_suspended(scid))
     send_lang(u, s, CS_SUSPEND_X_NOT_SUSPENDED, chan);
  else
  {
    if(del_suspension(scid))
    {
      ChanRecord* cr = OpenCR(chan);
      if(cr)
        ClearSuspendedChan(cr);    
      send_lang(u, s, CS_UNSUPEND_REMOVED_X, chan);
    }
    else
      send_lang(u, s, UPDATE_FAIL);
  }
}
