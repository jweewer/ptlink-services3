/******************************************************************
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License           *
 * Please read the file COPYING for copyright information.        *
 ******************************************************************
                                                                                
  Description: operserv kick command
                                                                                
 *  $Id: os_kick.c,v 1.6 2005/11/03 22:21:38 jpinto Exp $
*/
#include "module.h"
#include "path.h"
#include "my_sql.h"
#include "nsmacros.h"
#include "ns_group.h"
#include "lang/cscommon.lh"
#include "lang/common.lh"
#include "lang/os_kick.lh"

SVS_Module mod_info =
/* module, version, description */
{"os_kick", "2.1", "operserv kick command" };

/* Change Log
  2.1 - security fix
  2.0 - 0000265: remove nickserv cache system
*/

/* external functions we need */
ServiceUser* (*operserv_suser)(void);

MOD_REQUIRES
  MOD_FUNC(operserv_suser)
  MOD_FUNC(is_soper)
MOD_END

/* Internal functions declaration */
int mod_install(void);

/* core event handlers */

/* available commands from module */
void os_kick(IRC_User *s, IRC_User *u);

ServiceUser* osu;
int os_log;


int mod_load(void)
{

  os_log = log_handle("operserv");
  osu = operserv_suser();  
  suser_add_cmd(osu, "KICK", (void*) os_kick, KICK_SUMMARY, KICK_HELP);    
  return 0;
}

void
mod_unload(void)
{
  suser_del_mod_cmds(osu, &mod_info);
}

/* s = service the command was sent to
   u = user the command was sent from */
void os_kick(IRC_User *s, IRC_User *u)
{
  u_int32_t source_snid;
  IRC_User* targetu;
  IRC_Chan* chan;
  IRC_ChanNode* cn;
  char *reason;
  char *chname, *target;

  if (!is_soper(u->snid))
  {
    send_lang(u, s, PERMISSION_DENIED);
    return;
  }
  
  target = NULL;
  reason = NULL;
  chname = strtok(NULL, " ");  
  if(chname)
    target = strtok(NULL, " ");
  if(target)
    reason = strtok(NULL, "");

  CHECK_IF_IDENTIFIED_NICK  
  
  if(IsNull(chname) || IsNull(target))
    send_lang(u, s, CHAN_KICK_SYNTAX);    
  else
  if((chan = irc_FindChan(chname)) == NULL)
    send_lang(u,s, CHAN_X_IS_EMPTY, chname);
  else
  if((targetu = irc_FindUser(target)) == NULL)
    send_lang(u, s, NICK_X_NOT_ONLINE, target);
  else
  if((cn = irc_FindOnChan(chan, targetu)) == NULL)
    send_lang(u, s, NICK_X_NOT_ON_X, target, chname);
  else /* everything is valid */
    {      
      irc_SendSanotice(s, "%s used KICK on nick %s on channel %s", 
        u->nick, target, chname);
      log_log(os_log, mod_info.name, "%s used KICK on nick %s on channel %s", 
        u->nick, target, chname); 
      if(reason) 
        irc_Kick(chan->local_user ? chan->local_user : osu->u, chan, targetu, "%s", reason);
      else
        irc_Kick(chan->local_user ? chan->local_user : osu->u, chan, targetu, "");
    }

}
/* End of Module */
