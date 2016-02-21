/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************  
 
  Description: operserv shutdown command
                                                                                
 *  $Id: os_shutdown.c,v 1.5 2005/11/03 21:46:01 jpinto Exp $
*/
#include "module.h"
#include "ns_group.h" /* we need is_sroot() */
#include "nsmacros.h"
#include "lang/os_shutdown.lh"

SVS_Module mod_info =
/* module, version, description */
{ "os_shutdown", "2.0", "operserv shutdown command" };

/* Change Log
  2.0 - 0000265: remove nickserv cache system
*/

/* external functions we need */
ServiceUser* (*operserv_suser)(void);

MOD_REQUIRES 
  MOD_FUNC(operserv_suser)
  MOD_FUNC(is_sroot)
MOD_END

/* internal functions */
       
/* available commands from module */
void os_shutdown(IRC_User *s, IRC_User *u);

/* local variables */
ServiceUser *osu;
int os_log = 0;
  

int mod_load(void)
{
  os_log = log_handle("operserv");
  osu = operserv_suser();
  suser_add_cmd(osu, "SHUTDOWN", (void*) os_shutdown, SHUTDOWN_SUMMARY, SHUTDOWN_HELP);  
  return 0;
}

void mod_unload(void)
{
  suser_del_mod_cmds(osu, &mod_info);
}
 
/* s = service the command was sent to
   u = user the command was sent from */
void os_shutdown(IRC_User *s, IRC_User *u)
{
  u_int32_t source_snid;
  
  /* */
  CHECK_IF_IDENTIFIED_NICK
  
  if(is_sroot(source_snid) == 0)
  {
    send_lang(u, s, SHUTDOWN_NEED_ROOT);
    return;
  }    
  log_log(os_log, mod_info.name, "SHUTDOWN requested by %s", u->nick);
  exit(0);  
}
