/*****************************************************************
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999-2003 *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License          *
 * Please read the file COPYING for copyright information.       *
 *****************************************************************
                                                                                
  File: suser.h
  Desc: service user header file
                                                                                
 *  $Id: suser.h,v 1.2 2005/09/21 22:20:59 jpinto Exp $
*/

#include "modules.h"

/* structures */
struct  Suser_cmd_s
{
  char *cmd;
  void *func;
  struct Suser_cmd_s* next;
  const char **summary;
  const char **help;
  void* owner;
  u_int32_t sgid;
};
typedef struct Suser_cmd_s Suser_cmd;

struct ServiceUser_s
{
  IRC_User *u;
  Suser_cmd *cmds;
};
typedef struct ServiceUser_s ServiceUser;

/* functions */
void suser_add_cmd_g(ServiceUser* su, char *cmd, void* func, const char **summary, 
                  const char **help, u_int32_t sgid);
void suser_add_cmd(ServiceUser* su, char *cmd, void* func, const char **summary,
                  const char **help);
void suser_del_mod_cmds(ServiceUser *su, SVS_Module* module);
void suser_del_cmd(ServiceUser* su, void* func);
void suser_add_help(ServiceUser *su, char *helpstring, const char **help);
void suser_del_help(ServiceUser *su, const char **help);
