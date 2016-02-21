/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            
 **********************************************************************

  Description: operserv sline command

 *  $Id: os_sline.c,v 1.7 2005/11/03 22:21:38 jpinto Exp $
*/

#include "module.h"
#include "my_sql.h"
#include "nsmacros.h"
#include "path.h"
#include "ns_group.h"
#include "lang/common.lh"
#include "lang/os_sline.lh"

SVS_Module mod_info =
 /* module, version, description */
{"os_sline", "2.2",  "operserv sline module" };

/* Change Log
  2.2 - security fix
  2.1 - 0000349: os_sline should support list by line type
  2.0 - 0000265: remove nickserv cache system
*/

#define DB_VERSION      2

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
/* void internal_function(void); */
void os_sline(IRC_User *s, IRC_User *u);
void ev_os_new_server(IRC_Server* nserver, IRC_Server* from);


u_int32_t find_sline(char letter, char* mask);
u_int32_t insert_sline(char* who, char letter, char* mask, char* message);

/** Local variables **/
/* int my_local_variable; */
ServiceUser *osu;
    
/** load code **/
int mod_load(void)
{
  if(sql_check_inst_upgrade(mod_info.name, DB_VERSION, NULL) < 0)
    return -4;      
  osu = operserv_suser();
  suser_add_cmd(osu, "SLINE", (void*) os_sline, SLINE_SUMMARY, SLINE_HELP);  
  irc_AddEvent(ET_NEW_SERVER, (void*) ev_os_new_server);
  return 0;
}
    
/** unload code **/
void mod_unload(void)
{
  irc_DelEvent(ET_NEW_SERVER, (void*) ev_os_new_server);
  suser_del_mod_cmds(osu, &mod_info);
  return;
}

/** internal functions implementation starts here **/
void os_sline(IRC_User *s, IRC_User *u)
{
  u_int32_t source_snid;
  char *cmd;
  
  /* status validation */  
  CHECK_IF_IDENTIFIED_NICK

  if (!is_soper(u->snid))
  {
    send_lang(u, s, PERMISSION_DENIED);
    return;
  }
  
  /* syntax validation */
  cmd = strtok(NULL, " ");
  if(IsNull(cmd))
    send_lang(u, s, SLINE_SYNTAX);
  /* sub-command */
  else if(strcasecmp(cmd, "ADD") == 0)
    {
      u_int32_t id;      
      char* letter = strtok(NULL, " ");
      char* mask = strtok(NULL, " ");
      char* message = strtok(NULL, "");
      if(letter)
        letter[0] = toupper(letter[0]);
      if(letter && strlen(letter)>1)
        letter[1] = '\0';
      if(mask && strlen(mask)>128)
        letter[128] = '\0';
      if(message && strlen(message)>128)
        message[128] = '\0';
      /* syntax validation */
      if(IsNull(letter) || IsNull(mask) || IsNull(message))
        send_lang(u, s, SLINE_SYNTAX);
      /* avoid duplicates */
      else if(find_sline(*letter, mask) > 0)
        send_lang(u, s, SLINE_ALREADY_EXISTS_X_X, *letter, mask);
      /* execute operation */
      else if((id = insert_sline(u->nick ,*letter, mask, message)) != 0)
        {
          send_lang(u, s, ADDED_SLINE_X_X, *letter, id);
          irc_SendRaw(NULL, "S%cLINE %s :%s", *letter, mask, message);
        }
      else
        send_lang(u, s, UPDATE_FAIL);
    }
  /* sub-command */
  else if(strcasecmp(cmd, "DEL") == 0)
    {
      u_int32_t id = 0;
      char *strid;
      strid = strtok(NULL, " ");
      if(strid)
        id = atoi(strid);
      /* syntax validation */
      if(IsNull(strid))
        send_lang(u, s, SLINE_SYNTAX);
      else if(sql_singlequery("SELECT id, letter, mask FROM os_sline WHERE id=%d", id) == 0)
        send_lang(u, s, SLINE_X_NOT_FOUND, id);
      /* execute operation */
      else if(sql_execute("DELETE FROM os_sline WHERE id=%d", id) > 0)
        {
          send_lang(u, s, DELETED_SLINE_X, id);
          irc_SendRaw(NULL, "UNS%cLINE %s", *sql_field(1), sql_field(2));
        }
      else
        send_lang(u, s, UPDATE_FAIL);
        
    }    
  /* sub-command */
  else if(strcasecmp(cmd, "LIST") == 0)
    {
      MYSQL_RES* res;
      MYSQL_ROW row;
      int rowc = 0;
      char* letter = strtok(NULL, " ");
      if(letter)
        res = sql_query("SELECT "
        "id, letter, mask, message, who_nick, t_create FROM os_sline"
        " WHERE letter=%s", sql_str(letter));
      else
        res = sql_query("SELECT id, letter, mask, message, who_nick, t_create FROM os_sline");
      if(res)
        rowc = mysql_num_rows(res);      
      send_lang(u, s, SLINE_LIST_HEADER_X, rowc);        
      while((row = sql_next_row(res)))
        {
          send_lang(u, s, SLINE_LIST_FORMAT, 
            atoi(row[0]), *row[1], row[2], row[3], row[4], row[5]);
        }
      send_lang(u, s, SLINE_LIST_TAIL);
      sql_free(res);
    }  
  else 
    send_lang(u, s, SLINE_SYNTAX);
}

u_int32_t find_sline(char letter, char* mask)
{
  if(sql_singlequery("SELECT id FROM os_sline WHERE letter='%c' AND "
    "mask=%s", letter, sql_str(mask)) > 0)
    return atoi(sql_field(0));
  return 0;
}

u_int32_t insert_sline(char* who, char letter, char* mask, char* message)
{
  return sql_execute("INSERT INTO os_sline VALUES(0"
    ", '%c', %s, %s, NOW(), %s)",
    letter, sql_str(who), sql_str(mask), sql_str(message));
}

/* this is called when a new server is introduced */
void ev_os_new_server(IRC_Server* nserver, IRC_Server *from)
{
  static int already_loaded = 0;
  MYSQL_RES* res;
  MYSQL_ROW row;
  
  if(already_loaded)
    return;

  res = sql_query("SELECT letter, mask, message FROM os_sline");
  
  while((row = sql_next_row(res)))
      irc_SendRaw(NULL, "S%cLINE %s :%s", *row[0], row[1], row[2]);

  sql_free(res);
  already_loaded = -1;
  
  
}
    
/* End of module */
