/******************************************************************
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License           *
 * Please read the file COPYING for copyright information.        *
 ******************************************************************
                                                                                
  File: cs_help.c
  Description: chanserv help command
                                                                                
 *  $Id: cs_help.c,v 1.5 2005/10/11 16:13:06 jpinto Exp $
*/
#include "module.h"
#include "encrypt.h"
#include "nickserv.h"
#include "lang/help.lh"

SVS_Module mod_info =
/* module, version, description */
{"cs_help", "1.0", "chanserv help command" };

/* external functions we need */
ServiceUser* (*chanserv_suser)(void);

MOD_REQUIRES
  MOD_FUNC(chanserv_suser)
MOD_END

/* internal functions */

/* available commands from module */
void cs_help(IRC_User *s, IRC_User *u);

/* Local settings */

ServiceUser* csu;
int mod_load(void)
{
  csu = chanserv_suser();
  suser_add_cmd(csu, "HELP", cs_help, HELP_SUMMARY, HELP_HELP);  
  return 0;
}

void mod_unload(void)
{
  suser_del_mod_cmds(csu, &mod_info);
}
 
/* s = service the command was sent to
   u = user the command was sent from */
void cs_help(IRC_User *s, IRC_User *u)
{
  Suser_cmd *c;
  char *cmd = strtok(NULL, "");  


  c = csu->cmds;  
  if(IsNull(cmd) || *cmd=='\0')
    {
      send_lang(u, s, HELP_LIST);      
      while(c)
        {
          if(c->summary)
          {
            if(WantsMsg(u))
              irc_SendMsg(u, s,"%-15s %s", c->cmd, c->summary[u->lang]);
            else
              irc_SendNotice(u, s,"%-15s %s", c->cmd, c->summary[u->lang]);
          }
          c = c->next;
        }
      send_lang(u, s, HELP_END_OF_LIST);
    }
  else /* help for a specific item */
    {
      while(c)
        {
          if(c->help && (strcasecmp(c->cmd, cmd) == 0))
            {
              send_lang(u, s, c->help);              
              return ;
            }
           c = c->next;           
        }
      send_lang(u, s, NO_HELP_FOR_COMMAND_X, cmd);
    }
}
