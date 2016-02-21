/******************************************************************
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License           *
 * Please read the file COPYING for copyright information.        *
 ******************************************************************
                                                                                
  File: bs_help.c
  Description: botserv help command
                                                                                
 *  $Id:
*/
#include "module.h"
#include "encrypt.h"
#include "nickserv.h"
#include "lang/help.lh"

SVS_Module mod_info =
/* module, version, description */
{"bs_help", "1.0", "botserv help command" };

/* external functions we need */
ServiceUser* (*botserv_suser)(void);

Module_Function mod_requires[] =
{
   { "botserv_suser", &botserv_suser },
   { NULL }
};

/* internal functions */

/* available commands from module */
void bs_help(IRC_User *s, IRC_User *u);

/* Local settings */

ServiceUser* bsu;
int mod_load(void)
{
  bsu = botserv_suser();
  suser_add_cmd(bsu, "HELP", bs_help, HELP_SUMMARY, HELP_HELP);  
  return 0;
}

void mod_unload(void)
{
  suser_del_mod_cmds(bsu, &mod_info);
}
 
/* s = service the command was sent to
   u = user the command was sent from */
void bs_help(IRC_User *s, IRC_User *u)
{
  Suser_cmd *b;
  char *cmd = strtok(NULL, "");  


  b = bsu->cmds;  
  if(IsNull(cmd) || *cmd=='\0')
    {
      send_lang(u, s, HELP_LIST);      
      while(b)
        {
          if(b->summary)
          {
            if(WantsMsg(u))
              irc_SendMsg(u, s,"%-15s %s", b->cmd, b->summary[u->lang]);
            else
              irc_SendNotice(u, s,"%-15s %s", b->cmd, b->summary[u->lang]);
          }
          b = b->next;
        }
      send_lang(u, s, HELP_END_OF_LIST);
    }
  else /* help for a specific item */
    {
      while(b)
        {
          if(b->help && (strcasecmp(b->cmd, cmd) == 0))
            {
              send_lang(u, s, b->help);              
              return ;
            }
           b = b->next;           
        }
      send_lang(u, s, NO_HELP_FOR_COMMAND_X, cmd);
    }
}
