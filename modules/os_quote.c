/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            
 **********************************************************************

  Desc: random quotes module

 *  $Id: os_quote.c,v 1.5 2005/11/03 21:46:01 jpinto Exp $
*/

#include "module.h"
#include "nsmacros.h"
#include "ns_group.h" /* is_soper( */
#include "my_sql.h"
#include "lang/os_quote.lh"

SVS_Module mod_info =
 /* module, version, description */
{"os_quote", "2.0",  "operserv random quots module" };

/* Change Log
  2.0 - 0000265: remove nickserv cache system
*/

#define DB_VERSION 	1

/** functions/events we require **/
/* void (*FunctionPointer)(void);*/
ServiceUser* (*operserv_suser)(void);

MOD_REQUIRES
  MOD_FUNC(operserv_suser)
  MOD_FUNC(is_soper)
MOD_END

/** functions/events we provide **/
/* void my_function(void); */

/** Internal functions declaration **/
void os_quote(IRC_User *s, IRC_User *u);
void ev_os_quote_new_user(IRC_User* u, void *s);

    

/** Local variables **/
/* int my_local_variable; */
ServiceUser* osu;
IRC_User* tmp_user;
    
/** load code **/
int mod_load(void)
{
  osu = operserv_suser();
  if(sql_check_inst_upgrade(mod_info.name, DB_VERSION, NULL) < 0)
    return -4;
        
  suser_add_cmd(osu, "QUOTE", (void*) os_quote, QUOTE_SUMMARY, QUOTE_HELP);
  
  irc_AddEvent(ET_NEW_USER, (void*) ev_os_quote_new_user); /* new user */
  
  return 0;
}
    
/** unload code **/
void mod_unload(void)
{
  irc_DelEvent(ET_NEW_USER, (void*) ev_os_quote_new_user);
  suser_del_mod_cmds(osu, &mod_info);
  return;
}
    
/** internal functions implementation starts here **/
/* s = service the command was sent to
   u = user the command was sent from */
void os_quote(IRC_User *s, IRC_User *u)
{
  u_int32_t source_snid;
  char *cmd;
  u_int32_t id;

  CHECK_IF_IDENTIFIED_NICK

  if(is_soper(source_snid) == 0)
  {
    send_lang(u, s, QUOTE_NEEDS_OPER); 
     return;
  }

  cmd = strtok(NULL, " ");
  if(IsNull(cmd))
    send_lang(u, s, INVALID_QUOTE_SYNTAX);
  else if(strcasecmp(cmd,"ADD") == 0)
    {
      char *quote = strtok(NULL, "");
      if(IsNull(quote))
        send_lang(u, s, INVALID_QUOTE_SYNTAX);
      else
        {
          id = sql_execute("INSERT INTO os_quote VALUES(0,%s,%s)",
            sql_str(u->nick), sql_str(quote));          
          if(id == 0)
            send_lang(u, s, UPDATE_FAIL);
          else
            send_lang(u, s, QUOTE_X_INSERTED, id);
        }
    }
  else  if(strcasecmp(cmd,"DEL") == 0)
    {
      char *sid = strtok(NULL, " ");
      if(IsNull(sid))
        send_lang(u, s, INVALID_QUOTE_SYNTAX);
      else
        {
          int r=sql_execute("DELETE FROM os_quote WHERE id=%d", atoi(sid));
          if(r == 1)
            send_lang(u, s, QUOTE_X_DELETED, atoi(sid));
          else
            send_lang(u, s, QUOTE_X_NOT_FOUND, atoi(sid));
        }
    }
  else  if(strcasecmp(cmd,"LIST") == 0)
    {
      MYSQL_RES *res;
      MYSQL_ROW row;
      char *sqlmask = strtok(NULL, "");
      send_lang(u, s, QUOTE_LIST_HEADER);
      if(sqlmask)
        res = sql_query("SELECT id, who, quote FROM os_quote"
          " WHERE quote LIKE %s", sql_str(sqlmask));
      else
        res = sql_query("SELECT id, who, quote FROM os_quote");
      while((row = sql_next_row(res)))
        send_lang(u, s, QUOTE_ITEM_X_X_X, atoi(row[0]), row[1], row[2]);
      
      send_lang(u, s, QUOTE_LIST_TAIL);
      sql_free(res);
    }
  else
    send_lang(u, s, INVALID_QUOTE_SYNTAX);
}

/* this will send a random quote when a new user connects */
void ev_os_quote_new_user(IRC_User* u, void *s)
{
  if(irc_ConnectionStatus() != 2) /* just send after connected */
    return;
  if(sql_singlequery("SELECT quote FROM os_quote ORDER BY RAND() LIMIT 1") > 0)
    {
      send_lang(u, osu->u, QUOTE_STR, sql_field(0));
    }  
}

/* End of module */
