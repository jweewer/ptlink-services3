/******************************************************************
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License           *
 * Please read the file COPYING for copyright information.        *
 ******************************************************************
                                                                                
  File: ns_drop.c
  Description: nickserv drop command
                                                                                
 *  $Id: ns_drop.c,v 1.4 2005/10/11 16:13:06 jpinto Exp $
*/

#include "module.h"
#include "encrypt.h" /* we need encrypted_password() */
#include "nickserv.h"
#include "nsmacros.h"
#include "my_sql.h"
#include "ns_group.h"
/* lang files */
#include "lang/common.lh"
#include "lang/ns_drop.lh"

SVS_Module mod_info =
/* module, version, description */
{ "ns_drop", "2.1","nickserv drop command" };

/* Change Log
  2.1 - 0000305: foreign keys for data integrity
  2.0 - 0000265: remove nickserv cache system
  1.1 - 0000246: help display with group filter
*/
              
/* external functions we need */
ServiceUser* (*nickserv_suser)(void);
u_int32_t (*find_group)(char *name);
int e_nick_delete;

MOD_REQUIRES
   MOD_FUNC(nickserv_suser)
   MOD_FUNC(e_nick_delete)
   MOD_FUNC(is_sadmin)
   MOD_FUNC(find_group)
MOD_END

/* internal functions */
void drop_nick(u_int32_t snid, char* nick);

/* available commands from module */
void ns_drop(IRC_User *s, IRC_User *u);
void ns_sdrop(IRC_User *s, IRC_User *u);

/* Local settings */

/* Local vars */
ServiceUser* nsu;
int ns_log;

int mod_load(void)
{
  ns_log = log_handle("nickserv");
  nsu = nickserv_suser();
  suser_add_cmd(nsu, "DROP", ns_drop, DROP_SUMMARY, DROP_HELP);  
  suser_add_cmd_g(nsu, "SDROP", ns_sdrop, SDROP_SUMMARY, SDROP_HELP,
    find_group("Admin"));
  return 0;
}

void mod_unload(void)
{
  suser_del_mod_cmds(nsu, &mod_info);     
}

/* Internal Functions */
void drop_nick(u_int32_t snid, char* nick)
{
  log_log(ns_log, mod_info.name, "Dropping snid %d, nick %s", snid, nick);
  /* call related actions */
  mod_do_event(e_nick_delete, &snid, NULL);
  /* and delete it */
  sql_execute("DELETE FROM nickserv WHERE snid=%d", snid);  
}

/* s = service the command was sent to
   u = user the command was sent from */
void ns_drop(IRC_User *s, IRC_User *u)
{
  u_int32_t source_snid;
  char *nick_sec = NULL;
  char* securitycode = strtok(NULL, " ");

  CHECK_IF_IDENTIFIED_NICK

  if(sql_singlequery("SELECT securitycode FROM nickserv_security WHERE snid=%d",
    source_snid))
  {
    if(sql_field(0))
    {
      nick_sec = malloc(16);
      memcpy(nick_sec, hex_bin(sql_field(0)), 16);
    }
  }

  if(nick_sec && IsAuthenticated(u))
    {
      if(IsNull(securitycode))
        {
          send_lang(u, s, DROP_SECURITY_REQUIRED);
          FREE(nick_sec);
          return;
        }
      else if(memcmp(nick_sec, encrypted_password(securitycode), 16) != 0)
        {
          send_lang(u, s, INVALID_SECURITY_CODE);
          FREE(nick_sec);
          return;
        }
    }
  FREE(nick_sec);
  drop_nick(source_snid, u->nick);
  u->snid = 0;
  u->flags = 0;
  u->status = 0;
  irc_SvsMode(u, s, "-r");
  send_lang(u, s, NICK_DROPPED);
}

void ns_sdrop(IRC_User *s, IRC_User *u)
{
    u_int32_t source_snid;
    u_int32_t snid;
    char *nick;
    
    nick = strtok(NULL, " ");
    
    CHECK_IF_IDENTIFIED_NICK
    if(!is_sadmin(source_snid))
	send_lang(u, s, ONLY_FOR_SADMINS);
    else if(IsNull(nick))
	send_lang(u, s, NICK_SDROP_SYNTAX);
    else if((snid = nick2snid(nick)) == 0)
	send_lang(u, s, NICK_X_NOT_REGISTERED, nick);
    else
    {
	IRC_User *target = irc_FindUser(nick);    
        drop_nick(snid, nick);
	if(target && target->snid)
	{
	    target->snid = 0;
	    target->status = 0;
	    target->flags = 0;
    	    irc_SvsMode(target, s, "-r");
	}
	log_log(ns_log, mod_info.name, "%s SDROPPED nick: %s", u->nick, nick);
	send_lang(u, s, NICK_SDROPPED, nick);
    }
}

