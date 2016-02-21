/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************  
 
  Description: operserv stats command
                                                                                
 *  $Id: os_stats.c,v 1.7 2005/11/03 21:46:01 jpinto Exp $
*/
#include "module.h"
#include "chanrecord.h"
#include "ns_group.h" /* we need is_sroot() */
#include "nsmacros.h"
#include "lang/common.lh"
#include "lang/os_stats.lh"

SVS_Module mod_info =
/* module, version, description */
{ "os_stats", "2.1", "operserv stats command" };

/* Change Log
  2.1 - security fix
  2.0 - 0000265: remove nickserv cache system
*/

/* external functions we need */
ServiceUser* (*operserv_suser)(void);

MOD_REQUIRES 
  MOD_FUNC(operserv_suser)
  MOD_FUNC(is_sroot)
  MOD_FUNC(is_soper)
MOD_END

/* internal functions */
       
/* available commands from module */
void os_stats(IRC_User *s, IRC_User *u);
void uptime_stats(IRC_User *s, IRC_User *u);

/* local variables */
ServiceUser *osu;
int os_log = 0;
static time_t start_time = 0;  

int mod_load(void)
{
  os_log = log_handle("operserv");
  osu = operserv_suser();
  suser_add_cmd(osu, "STATS", os_stats, STATS_SUMMARY, STATS_HELP);  
  start_time = time(NULL); /* for the uptime count */
  return 0;
}

void
mod_unload(void)
{
  suser_del_mod_cmds(osu, &mod_info);
}

void uptime_stats(IRC_User *s, IRC_User *u)
{
  int days, hours, mins, secs;
  time_t uptime = time(NULL) - start_time;
  days = uptime/86400;
  hours = (uptime/3600)%24;
  mins = (uptime/60)%60;
  secs = uptime%60;
  
  log_log(os_log, mod_info.name, "STATS requested by %s", u->nick);
  irc_SendNotice(u, s, "******* Stats, Uptime %i days, %i hours, %i minutes, %i seconds",
    days, hours, mins, secs);
  irc_TimerStats(u, s);
  irc_UserStats(u, s);
  irc_SendNotice(u, s, "%s", chanrecord_stats());
  irc_SendNotice(u, s, "******* End of Stats");
}
  
/* s = service the command was sent to
   u = user the command was sent from */
void os_stats(IRC_User *s, IRC_User *u)
{
  if (!is_soper(u->snid))
  {
    send_lang(u, s, PERMISSION_DENIED);
    return;
  }  
  uptime_stats(s, u);
}
