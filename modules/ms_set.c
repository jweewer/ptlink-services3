/**********************************************************************
 * PTlink IRC Services is (C) Copyright PTlink IRC Software 1999-2006 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************

  Description: memoserv set command

 *  $Id: ms_set.c,v 1.15 2005/10/18 16:25:06 jpinto Exp $
*/

#include "module.h"
#include "memoserv.h"
#include "dbconf.h"
#include "my_sql.h"
#include "nsmacros.h"
#include "ns_group.h"	/* is_sadmin */
#include "lang/common.lh"
#include "lang/ms_set.lh"

SVS_Module mod_info =
/* module, version, description */
{"ms_set", "1.0", "memoserv set/sset command" };
/* Change Log
  1.0 - #5: split memoserv with a memoserv options table
*/

/* external functions we need */
ServiceUser* (*memoserv_suser)(void);
u_int32_t (*find_group)(char *name);

MOD_REQUIRES 
  DBCONF_FUNCTIONS
  MOD_FUNC(memoserv_suser)
  MOD_FUNC(is_sadmin)
  MOD_FUNC(find_group)
  MOD_FUNC(memoserv_get_options)    
MOD_END

/* internal functions */
void set_command(IRC_User *u, IRC_User *s, char* tnick, u_int32_t tsnid, char *option, char *value, int is_sset);
void ms_set(IRC_User *s, IRC_User *u);
void ms_sset(IRC_User *s, IRC_User *u); /* sadmin set */

ServiceUser* msu;
int ms_log;

int mod_load(void)
{
  msu = memoserv_suser();
  ms_log = log_handle("memoserv");

  suser_add_cmd(msu, "SET", ms_set, MS_SET_SUMMARY, MS_SET_HELP);
  suser_add_cmd_g(msu, "SSET", ms_sset, MS_SSET_SUMMARY, MS_SSET_HELP,
    find_group("Admin"));      
  return 0;
}

void
mod_unload(void)
{
  suser_del_mod_cmds(msu, &mod_info);
}

#define FLAG_SET(x,y) \
  { \
    if(IsNull(value)) \
      send_lang(u, s, VALUE_ON_OR_OFF); \
    else \
    if(strcasecmp(value,"on") == 0) \
      { \
        log_log(ms_log, mod_info.name, "%s %s %s %s %s", \
          u->nick, is_sset ? "SSET" : "SET", tnick, y, value); \
        if(u->snid == tsnid) u->flags |= (x); \
        send_lang(u, s, OPTION_X_ON, (y)); \
        sql_execute("UPDATE memoserv_options SET flags=(flags | %d) "\
          "WHERE snid=%d", (x), tsnid);\
      } else \
    if(strcasecmp(value,"off")  == 0) \
      { \
        log_log(ms_log, mod_info.name, "%s %s %s %s %s", \
          u->nick, is_sset ? "SSET" : "SET", tnick, y, value); \
        if(u->snid == tsnid) u->flags &= ~(x); \
        send_lang(u, s, OPTION_X_OFF, (y)); \
        sql_execute("UPDATE memoserv_options SET flags=(flags & ~%d) " \
          "WHERE snid=%d", (x), tsnid);\
      } else \
        send_lang(u, s, VALUE_ON_OR_OFF); \
  }
  
/* handles a set command */
void set_command(IRC_User *u, IRC_User *s, char* tnick, u_int32_t tsnid, char *option, char *value, int is_sset)
{
  int maxmemos;
  int bquota;
  u_int32_t flags;
  
  /* we need to read memo options first*/
  if(memoserv_get_options(tsnid, &maxmemos, &bquota, &flags) == 0)
  {
    /* we could had some problem to insert the options row, 
    if it's the case lets abort operation */
    send_lang(u, s, UPDATE_FAIL);
    return;
  }    
  if(strcasecmp(option,"AUTOSAVE") == 0)
    FLAG_SET(MOFL_AUTOSAVE, "AUTOSAVE")
  else
  if(strcasecmp(option,"FORWARD") == 0)
    FLAG_SET(MOFL_FORWARD, "FORWARD")
  else
  if(strcasecmp(option,"NOMEMOS") == 0)
    FLAG_SET(MOFL_NOMEMOS, "NOMEMOS")
  else
  if(is_sset == 0)
    send_lang(u, s, UNKNOWN_OPTION_X, option);
/** MS SSET commands start here **/
  else
  if(strcasecmp(option,"MAXMEMOS") == 0)
  {
    int new_maxmemos = atoi(value);
    if(sql_execute("UPDATE memoserv_options SET maxmemos=%d "
      "WHERE snid=%d", new_maxmemos, tsnid) > 0)
      send_lang(u, s, MS_SET_MAXMEMOS_TO_X, new_maxmemos);
    else
      send_lang(u, s, UPDATE_FAIL);
  }
  else
    send_lang(u, s, UNKNOWN_OPTION_X, option);
}

#undef STRING_SET
#undef FLAG_SET
 
/* s = service the command was sent to
   u = user the command was sent from */
void ms_set(IRC_User *s, IRC_User *u)
{
  u_int32_t source_snid;
  char *option, *value;

  CHECK_IF_IDENTIFIED_NICK
  
  option = strtok(NULL, " ");
  value = strtok(NULL, " ");
            
  if(IsNull(option) || IsNull(value))
  {
    send_lang(u, s, MS_SET_SYNTAX);
    return;
  }
    
  set_command(u, s, u->nick, u->snid, option, value, 0);
}

/* s = service the command was sent to
   u = user the command was sent from */
void ms_sset(IRC_User *s, IRC_User *u)
{
  u_int32_t source_snid;
  u_int32_t tsnid;
  char *nick, *option = NULL, *value = NULL;
  
  nick = strtok(NULL, " ");
  if(nick)
    option = strtok(NULL, " ");  

  CHECK_IF_IDENTIFIED_NICK    
  
  value = strtok(NULL, " ");

  if(IsNull(nick) || IsNull(option))
    send_lang(u, s, MS_SSET_SYNTAX);
  else
  if(!is_sadmin(source_snid))
    send_lang(u, s, ONLY_FOR_SADMINS);
  else 
  if( (tsnid = nick2snid(nick)) == 0 )
    send_lang(u, s, NICK_X_NOT_REGISTERED, nick);
  else
    {
      set_command(u, s, nick,tsnid, option, value, 1);
    }
}
