/******************************************************************
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License           *
 * Please read the file COPYING for copyright information.        *
 ******************************************************************

  File: ns_auth.c
  Description: nickserv auth command

 *  $Id: ns_auth.c,v 1.4 2005/10/11 16:13:06 jpinto Exp $
*/

#include "module.h"
#include "nickserv.h"
#include "encrypt.h"
#include "email.h"
#include "my_sql.h"
#include "nsmacros.h"
#include "lang/common.lh"
#include "lang/ns_auth.lh"

		      
SVS_Module mod_info = 
/* module, version, description */
{"ns_auth", "2.1", "nickserv auth command" };

/* Change Log
  2.1 - #61: MaxTimeForAuth setting to expire unauthenticated nicks
  2.0 - 0000265: remove nickserv cache system
*/

/* external functions we need */
ServiceUser* (*nickserv_suser)(void);
int (*update_nick_online_info)(IRC_User* u, u_int32_t snid, int lang);


MOD_REQUIRES
  MOD_FUNC(nickserv_suser)
  MOD_FUNC(update_nick_online_info)
MOD_END
                                                                                    
/* functions we provide */
void ns_auth(IRC_User *s, IRC_User *u);

/* local variables */
static ServiceUser* nsu;
int ns_log;

/* module initialization */
int mod_load(void)
{ 

  ns_log = log_handle("nickserv");
    
  /* get service user */    
  nsu = nickserv_suser();
  
  /* add command */
  suser_add_cmd(nsu, "AUTH", ns_auth, AUTH_SUMMARY, AUTH_HELP);  
  
  return 0;
}

void mod_unload(void)
{
  suser_del_mod_cmds(nsu, &mod_info);     
}
 
/* s = service the command was sent to
   u = user the command was sent from */
void ns_auth(IRC_User *s, IRC_User *u)
{
  u_int32_t source_snid;
  char *auth;
  char *nick_sec = NULL;
  char *email = NULL;

  /* status validation */
  CHECK_IF_IDENTIFIED_NICK  
  
  auth = strtok(NULL, " ");
  if(sql_singlequery("SELECT email FROM nickserv WHERE snid=%d",
    source_snid) && sql_field(0))
    email = strdup(sql_field(0));
  if(sql_singlequery("SELECT securitycode FROM nickserv_security WHERE snid=%d",
    source_snid) && sql_field(0))
  {
    nick_sec = malloc(16);
    memcpy(nick_sec, hex_bin(sql_field(0)), 16);
  }
  
  /* syntax validation */
  if(IsNull(auth))
    send_lang(u, s, NICK_AUTH_SYNTAX);    
  /* check requirements */
  else if((email == NULL)|| (nick_sec == NULL)
    || IsAuthenticated(u))
    send_lang(u, s, NO_PENDING_AUTH);
  /* privileges validation */
  else if(memcmp(nick_sec, encrypted_password(auth), 16) != 0)
    send_lang(u, s, INVALID_SECURITY_CODE);
  /* execute operation */
  else 
    {
      log_log(ns_log, mod_info.name, "Nick %s authenticated email %s", 
        u->nick, email);
      send_lang(u, s, AUTH_OK);                    
      irc_SvsMode(u, s, "+r");
      SetAuthenticated(u);
      update_nick_online_info(u, u->snid, u->lang);
      sql_execute("UPDATE nickserv SET flags=(flags | %d) WHERE snid=%d",
        NFL_AUTHENTIC, source_snid);
    }
  FREE(email);
  FREE(nick_sec);
}

