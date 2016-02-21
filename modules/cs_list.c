/******************************************************************
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License           *
 * Please read the file COPYING for copyright information.        *
 ******************************************************************
                                                                                
  File: cs_list.c
  Description: chanserv list command
                                                                                
 *  $Id: cs_list.c,v 1.4 2005/10/11 16:13:06 jpinto Exp $
*/

#include "module.h"
#include "encrypt.h"
#include "chanserv.h"
#include "nsmacros.h"
#include "my_sql.h"
#include "ns_group.h"
/* lang files */
#include "lang/common.lh"
#include "lang/cs_list.lh"


SVS_Module mod_info =
/* module, version, description */
{ "cs_list", "1.0","chanserv list command" };

/* Change Log
  */
  
/* external functions we need */
ServiceUser* (*chanserv_suser)(void);
u_int32_t (*find_group)(char *name);
 
Module_Function mod_requires[] =
{
   MOD_FUNC(chanserv_suser)
   MOD_FUNC(is_sadmin)
   MOD_FUNC(find_group)
   { NULL }
};

/* internal functions */

/* available commands from module */
void cs_list(IRC_User *s, IRC_User *u);

/* Local vars */
ServiceUser* csu;
int cs_log;

int mod_load(void)
{
  cs_log = log_handle("chanserv");
  csu = chanserv_suser();
  suser_add_cmd_g(csu, "LIST", cs_list, LIST_SUMMARY, LIST_HELP,
    find_group("Admin"));
  return 0;
}

void mod_unload(void)
{
  suser_del_mod_cmds(csu, &mod_info);     
}


/* s = service the command was sent to
   u = user the command was sent from */
void cs_list(IRC_User *s, IRC_User *u)
{
  u_int32_t source_snid;
  char* mask;
  char *options;
  int count = 0;
  char sql[1024];
  char buf[128];
  
  mask = strtok(NULL, " ");
  options = strtok(NULL, " ");
    
  CHECK_IF_IDENTIFIED_NICK
    
  if(!is_sadmin(source_snid))
    send_lang(u, s, ONLY_FOR_SADMINS);
  else if(IsNull(mask))
    send_lang(u, s, LIST_SYNTAX);
  else
  {
    MYSQL_RES* res;
    MYSQL_ROW row;
    char *c;
    u_int32_t flags = 0;
    
    /* replace '*' with '%' for sql */
    while((c = strchr(mask,'*'))) *c='%';
    snprintf(sql, sizeof(sql), "SELECT name, ifnull(cdesc,'')"
      " FROM chanserv WHERE name LIKE %s", sql_str(mask));

    if(options)
      {
        if(strstr(options, "noexpire"))
          flags |= CFL_NOEXPIRE;
        if(strstr(options, "suspended"))
          flags |= CFL_SUSPENDED;
      }
    if(flags)
    {
      snprintf(buf, sizeof(buf), " AND (flags & %d) ", flags);
      strcat(sql, buf);
    }
    strcat(sql, "ORDER BY name");
      
    res = sql_query("%s", sql);
    if(res)
      send_lang(u, s, CHAN_LIST_HEADER_X,
        mysql_num_rows(res));
    while((row = sql_next_row(res)))
    {
      send_lang(u, s, CHAN_LIST_X_X, 
        row[0], row[1]);
      if(++count >= 50) /* avoid flood */
      {
        send_lang(u, s, LIST_STOPPED);
          break;
      }
    }
    send_lang(u, s, CHAN_LIST_TAIL);
    sql_free(res);
  }
}

