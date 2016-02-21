/******************************************************************
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License           *
 * Please read the file COPYING for copyright information.        *
 ******************************************************************

  File: ns_getpass.c
  Description: nickserv auth command

 *  $Id: ns_getpass.c,v 1.5 2005/10/11 16:13:06 jpinto Exp $
*/

#include "module.h"
#include "nickserv.h"
#include "encrypt.h"
#include "email.h"
#include "my_sql.h"
#include "nsmacros.h"
#include "lang/common.lh"
#include "lang/ns_getpass.lh"

		      
SVS_Module mod_info = 
/* module, version, description */
{"ns_getpass", "1.0", "nickserv getpass/getsec command" };
/* Change Log
  1.0 - 
*/

/* external functions we need */
ServiceUser* (*nickserv_suser)(void);

MOD_REQUIRES
  MOD_FUNC(nickserv_suser)
  EMAIL_FUNCTIONS
MOD_END
                                                                                    
/* functions we provide */
void ns_getpass(IRC_User *s, IRC_User *u);
void ns_getsec(IRC_User *s, IRC_User *u);

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
  suser_add_cmd(nsu, "GETPASS", ns_getpass, GETPASS_SUMMARY, GETPASS_HELP);  
  suser_add_cmd(nsu, "GETSEC", ns_getsec, GETSEC_SUMMARY, GETSEC_HELP);    
  
  return 0;
}

void mod_unload(void)
{
  suser_del_mod_cmds(nsu, &mod_info);     
}
 
/* s = service the command was sent to
   u = user the command was sent from */
void ns_getpass(IRC_User *s, IRC_User *u)
{
  char *auth;
  char *target;
  char *email = NULL;
  char *nick = NULL;
  int diff = 1;
  u_int32_t snid;
  int lang = 0;

  target = strtok(NULL, " ");
  auth = strtok(NULL, " ");
  
  if(!target || !auth)
    send_lang(u, s, INVALID_GETPASS_SYNTAX);
  else
  if((snid = nick2snid(target)) == 0)
    send_lang(u, s, NICK_X_NOT_REGISTERED, target);
  else
  {
    u_int32_t flags = 0;    
    if(sql_singlequery("SELECT flags, email, nick, lang FROM nickserv WHERE snid=%d", snid) == 0)
      return ;
    flags = sql_field_i(0);
    lang = sql_field_i(3);
    if(((flags & NFL_AUTHENTIC) == 0) || (sql_field(1) == NULL))
    {
      send_lang(u, s, NICK_X_NOT_AUTHENTICATED, sql_field(2));
      return;
    }
    email = strdup(sql_field(1));    
    nick = strdup(sql_field(2));    
    if(sql_singlequery("SELECT securitycode FROM nickserv_security WHERE snid=%d",
      snid) && sql_field(0))
    {
      diff = memcmp(hex_bin(sql_field(0)), encrypted_password(auth), 16);
    }    
    if(diff != 0)
      send_lang(u, s, INVALID_SECURITY_CODE);
    else
    {
      char buf[512];
      char *email_body;
      log_log(ns_log, mod_info.name, "Nick %s used GETPASS for %s, %s", 
        u->nick, nick, email);   
      email_body = strdup(lang_str_l(lang, GETPASS_BODY_X_X, nick, sql_field(0)));      
      snprintf(buf, sizeof(buf), 
      "From: \"%%from_name%%\" <%%from%%>\r\nTo:\"%s\" <%s>\r\nSubject:%s\r\n\r\n%s",
        nick, email,
        lang_str_l(lang, GETPASS_SUBJECT), 
        email_body
      );
      free(email_body);
      email_init_symbols();
      email_add_symbol("email", email);
      email_send(buf);
      send_lang(u, s, GETPASS_CHECK_EMAIL_X, email);
    }      
    FREE(nick);
    FREE(email);
  }
}

/* s = service the command was sent to
   u = user the command was sent from */
void ns_getsec(IRC_User *s, IRC_User *u)
{
  char *email = NULL;
  u_int32_t source_snid;
  char *email_body;
  char buf[512];

  CHECK_IF_IDENTIFIED_NICK
  if(sql_singlequery("SELECT email FROM nickserv WHERE snid=%d", source_snid) == 0)
    return ;
    
  if(((u->flags & NFL_AUTHENTIC) == 0) || (sql_field(0) == NULL))
  {
    send_lang(u, s, NICK_X_NOT_AUTHENTICATED, u->nick);
    return;
  }

  email = strdup(sql_field(0));    
  if((sql_singlequery("SELECT securitycode FROM nickserv_security WHERE snid=%d",
    source_snid) == 0) || (sql_field(0)  == NULL))
  {
    FREE(email);
    return; /* this should never happen */
  }    
  
  log_log(ns_log, mod_info.name, "Nick %s used GETSEC, %s", 
    u->nick, email);   
  email_body = strdup(lang_str_l(u->lang, GETSEC_BODY_X_X, email, sql_field(0)));
  snprintf(buf, sizeof(buf), 
    "From: \"%%from_name%%\" <%%from%%>\r\nTo:\"%s\" <%s>\r\nSubject:%s\r\n\r\n%s",
       u->nick, email,
       lang_str_l(u->lang, GETSEC_SUBJECT), 
       email_body
      );
  free(email_body);
  email_init_symbols();
  email_add_symbol("email", email);
  email_send(buf);
  send_lang(u, s, GETSEC_CHECK_EMAIL_X, email);
  FREE(email);
}

