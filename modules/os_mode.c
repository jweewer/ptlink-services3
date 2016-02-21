/******************************************************************
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License           *
 * Please read the file COPYING for copyright information.        *
 ******************************************************************
                                                                                
  Description: operserv mode command
                                                                                
 *  $Id: 
*/
#include "module.h"
#include "path.h"
#include "my_sql.h"
#include "nsmacros.h"
#include "ns_group.h"
#include "lang/cscommon.lh"
#include "lang/common.lh"
#include "lang/os_mode.lh"

SVS_Module mod_info =
/* module, version, description */
{"os_mode", "2.1", "operserv mode command" };

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
void os_mode(IRC_User *s, IRC_User *u);

ServiceUser* osu;


int mod_load(void)
{

  osu = operserv_suser();    
  suser_add_cmd(osu, "MODE", os_mode, MODE_SUMMARY, MODE_HELP);    
  return 0;
}

void
mod_unload(void)
{
  suser_del_mod_cmds(osu, &mod_info);
}

/* s = service the command was sent to
   u = user the command was sent from */
void os_mode(IRC_User *s, IRC_User *u)
{
  u_int32_t source_snid;
  IRC_Chan* chan;
  char *mode_str;
  char *chname;
  mode_str = NULL;
  
  chname = strtok(NULL, " ");  
  if(chname)
    mode_str = strtok(NULL, "");
    
  CHECK_IF_IDENTIFIED_NICK 
  
  if (!is_soper(u->snid))
  {
    send_lang(u, s, PERMISSION_DENIED);
    return;
  } 
  
  if(IsNull(chname) || IsNull(mode_str))
    send_lang(u, s, CHAN_MODE_SYNTAX);    
  else
  if((chan = irc_FindChan(chname)) == NULL)
    send_lang(u,s, CHAN_X_IS_EMPTY, chname);
  else /* everything is valid */
  {      
    irc_SendSanotice(s, "%s used MODE %s on channel %s", 
      u->nick, mode_str, chname);
    irc_ChanMode(osu->u, chan, "%s", mode_str);
  }

}
/* End of Module */
