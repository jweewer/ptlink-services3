/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************  

  Description: operserv global command
                                                                                
 *  $Id: os_global.c,v 1.5 2005/11/03 21:46:01 jpinto Exp $
*/
#include "module.h"
#include "ns_group.h"
#include "lang/os_global.lh"
#include "lang/common.lh"

SVS_Module mod_info =
/* module, version, description */
{ "os_global", "1.2", "operserv global/globalmsg command" };

/* Change Log
  1.2 - security fix
  1.1 - 0000285: os_globalmsg to send global with private message
*/
  
/* external functions we need */
ServiceUser* (*operserv_suser)(void);

MOD_REQUIRES 
  MOD_FUNC(operserv_suser)
  MOD_FUNC(is_soper)
MOD_END

/* internal functions */
       
/* available commands from module */
void os_global(IRC_User *s, IRC_User *u);
void os_globalmsg(IRC_User *s, IRC_User *u);

ServiceUser *osu;

int mod_load(void)
{
  osu = operserv_suser();
  suser_add_cmd(osu, "GLOBAL", os_global, GLOBAL_SUMMARY, GLOBAL_HELP);  
  suser_add_cmd(osu, "GLOBALMSG", os_globalmsg, GLOBALMSG_SUMMARY, GLOBALMSG_HELP);    
  return 0;
}

void mod_unload(void)
{
  suser_del_mod_cmds(osu, &mod_info);
}
 
/* s = service the command was sent to
   u = user the command was sent from */
void os_global(IRC_User *s, IRC_User *u)
{
  char *hostmask = strtok(NULL, " ");  
  char *msg = strtok(NULL, "");

  if (!is_soper(u->snid))
  {
    send_lang(u, s, PERMISSION_DENIED);
    return;
  }
  
  if(IsNull(hostmask) || *hostmask=='\0' || IsNull(msg)|| *msg=='\0')
      send_lang(u, s, GLOBAL_SYNTAX);      
  else 
      irc_GlobalNotice(s, hostmask, "%s", msg);
}

/* s = service the command was sent to
   u = user the command was sent from */
void os_globalmsg(IRC_User *s, IRC_User *u)
{
  char *hostmask = strtok(NULL, " ");  
  char *msg = strtok(NULL, "");

  if (!is_soper(u->snid))
  {
    send_lang(u, s, PERMISSION_DENIED);
    return;
  }

  
  if(IsNull(hostmask) || *hostmask=='\0' || IsNull(msg)|| *msg=='\0')
      send_lang(u, s, GLOBAL_SYNTAX);      
  else 
      irc_GlobalMessage(s, hostmask, "%s", msg);
}
