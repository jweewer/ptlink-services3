/******************************************************************
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License           *
 * Please read the file COPYING for copyright information.        *
 ******************************************************************
                                                                                
  File: ns_list.c
  Description: nickserv list command
                                                                                
 *  $Id: ns_list.c,v 1.6 2005/10/17 16:57:50 jpinto Exp $
*/
#include "module.h"
#include "encrypt.h"
#include "nickserv.h"
#include "nsmacros.h"
#include "lang/common.lh"
#include "lang/ns_list.lh"
#include "my_sql.h"
#include "ns_group.h"

SVS_Module mod_info =
/* module, version, description */
{ "ns_list", "2.1","nickserv list command" };

/* Change Log
  2.1 - #21 : remove unused/moved fields from nickserv table
  2.0 - 0000279: nickserv list support for flags (auth, noexpire, forbidden ...)
      - 0000265: remove nickserv cache system
  1.2 - we don't need irc_lower()
  1.1 - 0000246: help display with group filter
  */
  
/* external functions we need */
ServiceUser* (*nickserv_suser)(void);
u_int32_t (*find_group)(char *name);
 
MOD_REQUIRES
   MOD_FUNC(nickserv_suser)
   MOD_FUNC(is_sadmin)
   MOD_FUNC(find_group)
MOD_END

/* internal functions */

/* available commands from module */
void ns_list(IRC_User *s, IRC_User *u);

/* Local vars */
ServiceUser* nsu;
int ns_log;

int mod_load(void)
{
  ns_log = log_handle("nickserv");
  nsu = nickserv_suser();
  suser_add_cmd_g(nsu, "LIST", ns_list, LIST_SUMMARY, LIST_HELP,
    find_group("Admin"));
  return 0;
}

void mod_unload(void)
{
  suser_del_mod_cmds(nsu, &mod_info);     
}


/* s = service the command was sent to
   u = user the command was sent from */
void ns_list(IRC_User *s, IRC_User *u)
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
    char *crit;
    char *c;
    u_int32_t flags = 0;
    
    if(mask[0] == '@')
    {      
      crit = "email";
      mask++;
    }
    else
    {
      crit = "nick";
    }        
        
    /* replace '*' with '%' for sql */
    while((c = strchr(mask,'*'))) *c='%';
    snprintf(sql, sizeof(sql), "SELECT nick, email"
      " FROM nickserv WHERE %s LIKE %s", crit, sql_str(mask));

    if(options)
      {
        if(strstr(options, "auth"))
          flags |= NFL_AUTHENTIC;
        if(strstr(options, "noexpire"))
          flags |= NFL_NOEXPIRE;
        if(strstr(options, "suspended"))
          flags |= NFL_SUSPENDED;
        if(strstr(options, "nonews"))
          flags |= NFL_NONEWS;                  
      }
    if(flags)
    {
      snprintf(buf, sizeof(buf), " AND (flags & %d) ", flags);
      strcat(sql, buf);
    }
    strcat(sql, "ORDER BY nick");
      
    res = sql_query("%s", sql);
    if(res)
      send_lang(u, s, NICK_LIST_HEADER_X,
        mysql_num_rows(res));
    while((row = sql_next_row(res)))
    {
      send_lang(u, s, NICK_LIST_X_X, 
        row[0] , row[1] ? row[1]: "");
      if(++count >= 50) /* avoid flood */
      {
        send_lang(u, s, LIST_STOPPED);
          break;
      }
    }
    send_lang(u, s, NICK_LIST_TAIL);
    sql_free(res);
  }
}
