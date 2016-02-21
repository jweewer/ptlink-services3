/*****************************************************************
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999-2004*
 *                http://software.pt-link.net                    *
 * This program is distributed under GNU Public License          *
 * Please read the file COPYING for copyright information.       *
 *****************************************************************
                                                                                
  File: suser.h
  Description: service user

 *  $Id: suser.c,v 1.2 2005/09/21 22:20:59 jpinto Exp $
*/
#include "stdinc.h"
#include "ircservice.h"
#include "modules.h" /* we need it for the owner */
#include "suser.h"
#include "strhand.h"



/* add a command to a service module 
  add it sorted to make sure we get a nice help list
  */
void suser_add_cmd_g(ServiceUser *su, char *cmd, void* func, 
                  const char **summary, const char **help, u_int32_t sgid)
{
  Suser_cmd* modcmd = malloc(sizeof(Suser_cmd));
  Suser_cmd* m;  
  modcmd->cmd = strdup(cmd);
  modcmd->func = func; 
  modcmd->summary = summary;
  modcmd->help = help;
  modcmd->owner = CurrentModule;
  modcmd->sgid = sgid;

  /* Lets add the irc msg command handler */ 
  if(func)
    irc_AddUMsgEvent(su->u, modcmd->cmd, modcmd->func);
  
  /* Now add it to the commands list */
  if(IsNull(su->cmds))
    {
      modcmd->next = NULL;
      su->cmds =  modcmd;
      return;
    }    
  m = su->cmds;  
  while((m->next != NULL) && (strcasecmp(cmd,m->next->cmd)>0)) /* look where to insert it */
      m = m->next;
      
  if((m==su->cmds) && strcasecmp(cmd, m->cmd)<0)
    {
      su->cmds = modcmd;
      modcmd->next=m;
    }
  else
    {
      modcmd->next=m->next;
      m->next=modcmd;
    }
}

void suser_add_cmd(ServiceUser *su, char *cmd, void* func, const char **summary,
  const char **help)
{
  suser_add_cmd_g(su, cmd, func , summary, help, 0);
}
/*
 * Deletes all commands added by a specific module *
 */
void suser_del_mod_cmds(ServiceUser *su, SVS_Module* owner)
{
  Suser_cmd *cmd, *next, *prev;
  cmd = su->cmds;
  prev = NULL; 
  while(cmd)
    {
      if(cmd->owner == owner)
        {
          next = cmd->next;
          if(prev)
            prev->next = next;
          else
            su->cmds = next;
          /* delete data and event */
          irc_DelUMsgEvent(su->u, cmd->cmd, cmd->func);
          free(cmd->cmd);
          free(cmd);          
          cmd = next;
        }
      else
        {
          prev = cmd;
          cmd = cmd->next;
        }
    }
}

void suser_add_help(ServiceUser *su, char *helpstring, const char **help)
{
  suser_add_cmd(su, helpstring, NULL, NULL, help);
}

/* To Do */
void suser_del_help(ServiceUser *su, const char **help) {
}

#if 0
SVS_Module*  module_find(char *modname)
{
  int i;
  for(i=0;i<modules_count;++i)
    if(strcasecmp(svs_modules[i].name, modname) == 0)
      return &svs_modules[i];
  return NULL;
}

/* add msg event handlers for all module commands */
void module_add_cmd_events(SVS_Module *pmodule, IRC_User *u)
{
  Suser_cmd *modcmd = pmodule->cmds;
  while(modcmd)
    {
      irc_AddUMsgEvent(u, modcmd->cmd, modcmd->func);
      modcmd = modcmd->next;
    }
}
#endif
