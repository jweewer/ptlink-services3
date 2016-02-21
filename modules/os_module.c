/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            
 **********************************************************************

  Description: modules management module

 *  $Id: os_module.c,v 1.5 2005/10/11 16:13:06 jpinto Exp $
*/

#include "module.h"
#include "ns_group.h"
#include "nsmacros.h"
#include "lang/os_module.lh"

SVS_Module mod_info =
 /* module, version, description */
{"os_module", "2.0",  "operserv module command" };

/* Change Log
  2.0 - 0000265: remove nickserv cache system
*/

/** functions/events we require **/
/* void (*FunctionPointer)(void);*/
ServiceUser* (*operserv_suser)(void);

MOD_REQUIRES
  MOD_FUNC(operserv_suser)
  MOD_FUNC(is_sroot)
MOD_END

/** Internal functions declaration **/
/* void internal_function(void); */
void os_module(IRC_User *s, IRC_User *u);
void send_to_user(char* message);
    

/** Local variables **/
/* int my_local_variable; */
ServiceUser* osu;
IRC_User* tmp_user;
    
/** load code **/
int mod_load(void)
{
  osu = operserv_suser();
  suser_add_cmd(osu, "MODULE", (void*) os_module, MODULE_SUMMARY, MODULE_HELP);

  return 0;
}
    
/** unload code **/
void mod_unload(void)
{
  suser_del_mod_cmds(osu, &mod_info);
}
    
/** internal functions implementation starts here **/
/* s = service the command was sent to
   u = user the command was sent from */
void os_module(IRC_User *s, IRC_User *u)
{
  u_int32_t source_snid;
  char *cmd;

  CHECK_IF_IDENTIFIED_NICK

  if(is_sroot(source_snid) == 0)
  {
    send_lang(u, s, MODULE_NEED_ROOT); 
     return;
  }

  cmd = strtok(NULL, " ");
  if(IsNull(cmd))
    send_lang(u, s, INVALID_MODULE_SYNTAX);
  else if(strcasecmp(cmd, "LOAD") == 0)
    {
      char *modname = strtok(NULL, " ");
      if(IsNull(modname))
        send_lang(u, s, INVALID_MODULE_SYNTAX);
      else if(module_find(modname))
        send_lang(u, s, MODULE_ALREADY_LOADED_X, modname);
      else
        {
          tmp_user = u;
          set_log_aux(send_to_user);
          module_load(modname, 0);
          set_log_aux(NULL);
          tmp_user = NULL;
        }
    }
  else if(strcasecmp(cmd, "LIST") == 0)
    {
      int i;
      char *mask;
      mask = strtok(NULL, "");
      send_lang(u, s, MODULE_LIST_HEAD);
      for(i=0; i<modules_count; ++i) 
        {
          if(mask && !match(mask, svs_modules[i]->name)) /* check for match */
            continue;
          send_lang( u, s, MODULE_LIST_X_X_X, 
            svs_modules[i]->name, svs_modules[i]->version, svs_modules[i]->desc);
        }
      send_lang(u, s, MODULE_LIST_TAIL);
    }
  else if(strcasecmp(cmd, "UNLOAD") == 0)
    {
      char *modname;
      modname = strtok(NULL, " ");
      
      if(IsNull(modname))
        send_lang(u, s, INVALID_MODULE_SYNTAX);
      else if(strcmp(modname,"os_module") == 0)
        send_lang(u, s, CANT_UNLOAD_SELF);
      else
        {
          SVS_Module* mod; 
          char *fname;
          char *mname;
          mod = module_find(modname);
          if(mod == NULL)
            send_lang(u, s, MODULE_X_NOT_LOADED, modname);
/*            
          else if(mod_check_events(mod, &fname, &mname) != 0)
            send_lang(u, s, MODULE_EVENT_IN_USE_X_X, fname, mname);          
*/            
          else if(check_for_functions(mod, &fname, &mname) != 0)
            send_lang(u, s, MODULE_FUNC_IN_USE_X_X, fname, mname);
          else
            {
              send_lang(u, s, MODULE_UNLOADING_X, modname);
              /* it's safe to unload the module now */
              module_unload(mod);              
              send_lang(u, s, MODULE_UNLOADED);
              /*
              tmp_user = u;
              set_log_aux(send_to_user);
              set_log_aux(NULL);
              tmp_user = NULL;
              */
              }
        }
    }    
  else
    send_lang(u, s, INVALID_MODULE_SYNTAX);
}

void send_to_user(char* message)
{
  if(tmp_user)
    irc_SendNotice(tmp_user, osu->u, "%s", message);
}  

/* End of module */
