/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            
 **********************************************************************

  Desc: operserv raw command

 *  $Id: os_raw.c,v 1.5 2005/11/03 21:46:01 jpinto Exp $
*/

#include "module.h"
#include "ns_group.h"
#include "nsmacros.h"
#include "lang/os_raw.lh"

SVS_Module mod_info =
 /* module, version, description */
{"os_raw", "2.0",  "operserv module command" };

/* Change Log
  2.0 - 0000265: remove nickserv cache system
*/

/** functions/events we require **/
ServiceUser* (*operserv_suser)(void);

MOD_REQUIRES
  MOD_FUNC(operserv_suser)
  MOD_FUNC(is_sroot)
MOD_END

/** functions/events we provide **/
/* void my_function(void); */


/** Internal functions declaration **/
/* void internal_function(void); */
void os_raw(IRC_User *s, IRC_User *u);
    

/** Local variables **/
/* int my_local_variable; */
ServiceUser* osu;
IRC_User* tmp_user;
    
/** load code **/
int mod_load(void)
{
  osu = operserv_suser();
  suser_add_cmd(osu, "RAW", os_raw, RAW_SUMMARY, RAW_HELP);

  return 0;
}
    
/** unload code **/
void mod_unload(void)
{
  suser_del_mod_cmds(osu, &mod_info);
  return;
}
    
/** internal functions implementation starts here **/
/* s = service the command was sent to
   u = user the command was sent from */
void os_raw(IRC_User *s, IRC_User *u)
{
  u_int32_t source_snid;
  char *cmd;

  CHECK_IF_IDENTIFIED_NICK
  cmd = strtok(NULL , "");
  if(is_sroot(source_snid) == 0)
  {
    send_lang(u, s, RAW_NEEDS_ROOT); 
  }
  else if(IsNull(cmd))
  {
     send_lang(u, s, INVALID_RAW_SYNTAX);  
  }
  else irc_SendRaw(NULL, "%s", cmd);
}

/* End of module */
