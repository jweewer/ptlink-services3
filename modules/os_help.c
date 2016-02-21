/*****************************************************************
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999-2004 *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License          *
 * Please read the file COPYING for copyright information.       *
 *****************************************************************
                                                                                
  File: os_help.c
  Description: operserv help command
                                                                                
 *  $Id: os_help.c,v 1.7 2005/11/05 10:56:04 jpinto Exp $
*/
#include "module.h"
#include "encrypt.h"
#include "ns_group.h"
#include "nickserv.h"
#include "lang/help.lh"
#include "lang/common.lh"

SVS_Module mod_info =
/* module, version, description */
{ "os_help", "1.1", "operserv help command" };

/* external functions we need */
ServiceUser* (*operserv_suser)(void);

MOD_REQUIRES
  MOD_FUNC(operserv_suser)
  MOD_FUNC(is_soper)
MOD_END

/* internal functions */
void os_help(IRC_User *s, IRC_User *u);


ServiceUser *osu;

int mod_load(void)
{
  osu = operserv_suser();
  suser_add_cmd(osu, "HELP", (void*) os_help, HELP_SUMMARY, HELP_HELP);  
  return 0;
}

void mod_unload(void)
{
  suser_del_mod_cmds(osu, &mod_info);
}
 
/* s = service the command was sent to
   u = user the command was sent from */
void os_help(IRC_User *s, IRC_User *u)
{
  char *cmd = strtok(NULL, "");  
  Suser_cmd *c;
  
  if (!is_soper(u->snid))
  {
    send_lang(u, s, PERMISSION_DENIED);
    return;
  }
  
  c = osu->cmds;  
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
