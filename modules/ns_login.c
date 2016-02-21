/******************************************************************
 * PTlink Services is (C) Copyright PTlink IRC Software 1999-2005 *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License           *
 * Please read the file COPYING for copyright information.        *
 ******************************************************************
                                                                                
  File: ns_login.c
  Description: nickserv login command
                                                                                
 *  $Id: ns_login.c,v 1.15 2005/12/11 16:51:17 jpinto Exp $
*/
#include "module.h"
#include "nickserv.h"
#include "dbconf.h"
#include "encrypt.h"
#include "my_sql.h"
#include "nsmacros.h"
#include "lang/common.lh"
#include "lang/nickserv.lh"
#include "lang/ns_login.lh"

SVS_Module mod_info =
/* module, version, description */
{ "ns_login", "2.3","nickserv login command" };
/* Change Log
  2.3 - #50: nickserv login option to override nick language
        #21 : remove unused/moved fields from nickserv table
        #10 : add nickserv suspensions
  2.2 - #23 : e_nick_identify is not called on ns_login when the nick is online
  2.1 - 0000352: nickserv login is not informing the user with "Password accepted" 
  2.0 - 0000287: ns_getpass and ns_getsec to recover password and security code
        0000272: move nickserv security info to a specific table
        0000265: remove nickserv cache system
  1.2 - 0000248: nick login log message using wrong string pointer
      - 0000249: add nickserv REGAIN alias to LOGIN
  1.1 - call the nick_identify event on login
*/
          

/* external functions we need */
ServiceUser* (*nickserv_suser)(void);
int (*update_nick_online_info)(IRC_User* u, u_int32_t snid, int lang);
int (*check_nick_security)
  (u_int32_t snid, IRC_User *u, char* pass, char* email, int flags);  
int e_nick_identify;
 
MOD_REQUIRES
  MOD_FUNC(dbconf_get)
  MOD_FUNC(nickserv_suser)
  MOD_FUNC(e_nick_identify)
  MOD_FUNC(update_nick_online_info)
  MOD_FUNC(check_nick_security)
MOD_END

/* internal functions */

/* available commands from module */
void ns_login(IRC_User *s, IRC_User *u);
void ns_recover(IRC_User *s, IRC_User *u);

/* Local settings */
int FailedLoginMax;

/*
 * List of dbconf items we require
 */
DBCONF_REQUIRES
  DBCONF_GET("nickserv", FailedLoginMax)
DBCONF_END

/* this is called before load and at services rehash */
int mod_rehash(void)
{
  if(dbconf_get(dbconf_requires) < 0 )
  {
    errlog("Error reading dbconf!");
    return -1;
  }
  return 0;
}

ServiceUser *nsu;
int ns_log;

static int is_recover = 0; /* internal flag for the recover command */
int mod_load(void)
{
  nsu = nickserv_suser();
  ns_log = log_handle("nickserv");  
  suser_add_cmd(nsu, "LOGIN", ns_login, LOGIN_SUMMARY, LOGIN_HELP);
  suser_add_cmd(nsu, "GHOST", ns_login, NULL, NULL);
  suser_add_cmd(nsu, "REGAIN", ns_login, NULL, NULL);
  suser_add_cmd(nsu, "RECOVER", ns_recover, NULL, NULL);
  return 0;
}

void mod_unload(void)
{
  suser_del_mod_cmds(nsu, &mod_info);   
}
 
/* s = service the command was sent to
   u = user the command was sent from */
void ns_login(IRC_User *s, IRC_User *u)
{
  int diff;
  int lang = -1;
  char *target = strtok(NULL, " ");
  char *pass = strtok(NULL, " ");
  char *langstr = strtok(NULL, " ");
  
  if(langstr)
  {
    lang2index(langstr, lang);
  }        
  if(IsNull(target) || IsNull(pass)) /* all parts filled in ? */
    send_lang(u, s, NICK_LOGIN_SYNTAX);
  else if(sql_singlequery("SELECT snid, flags, lang, vhost, email"
    " FROM nickserv WHERE nick=%s", sql_str(irc_lower_nick(target))) == 0)
    send_lang(u, s, NICK_NOT_REGISTERED);
  else  /* ok to validate password */
  {
    char *check;  
    u_int32_t snid = sql_field_i(0);
    u_int32_t flags = sql_field_i(1);
    char *vhost = NULL;
    char *email = NULL;
    if(lang == -1)
      lang = sql_field_i(2);
      
    if(sql_field(3))
      vhost = strdup(sql_field(3));
    if(sql_field(4))
      email = strdup(sql_field(4));

    if((flags & NFL_SUSPENDED) &&
      sql_singlequery("SELECT reason FROM nickserv_suspensions WHERE snid=%d", snid))
    {
      FREE(vhost);
      FREE(email);
      send_lang(u,s, NICK_X_IS_SUSPENDED_X, target, sql_field(0));
      return;
    }    
    check = is_recover ? "securitycode" : "pass";
    if(sql_singlequery("SELECT %s"
      " FROM nickserv_security WHERE snid=%d", check, snid) == 0)
    {
      send_lang(u, s, INCORRECT_PASSWORD);
      log_log(ns_log, mod_info.name, "Missing nick security record for %d",
        snid);
      FREE(vhost);      
      FREE(email);        
      return;
    }      
    if(sql_field(0))
    {
      if(is_recover)
      { /* case insentive for md5 validation */
        diff = strcasecmp(sql_field(0), pass);
        is_recover = 0;
      }
      else
      {
        diff = memcmp( hex_bin(sql_field(0)), encrypted_password(pass), 16);
      }
    }
      
    if(diff !=0 )
    {
      log_log(ns_log, mod_info.name, "Failed login for %s by %s",
        target, irc_UserMask(u));            
      if(FailedLoginMax && ++u->fcount > FailedLoginMax)
      {
        log_log(ns_log, mod_info.name, 
          "Killing %s after too many failed identifies", u->nick);
        irc_Kill(u, s, "Too many invalid identify attempts");
      }
      else
        send_lang(u, s, INCORRECT_PASSWORD);
    }
    else
    {
      IRC_User* ku; /* we may beed to kill the current used */      
      u->lang = lang;
      log_log(ns_log, mod_info.name, "Nick %s login by %s",
        target, irc_UserMask(u));      
      send_lang(u, s, NS_LOGIN_OK);        
      ku = irc_FindUser(target);
      if(ku == u) /* nick is alreading using the proper nick */
      {
        int was_identified = irc_IsUMode(u, UMODE_IDENTIFIED);
        check_nick_security(snid, u, NULL, email, flags);
        update_nick_online_info(u, snid, lang);
        if(vhost && irccmp(u->publichost, vhost)) /* we need to set the vhost */
          irc_ChgHost(u, vhost);      
        irc_CancelUserTimerEvents(u); /* delete the pending change nick event */
        if(!was_identified)
        {
          mod_do_event(e_nick_identify, u, &snid);
        }        
      }      
      else
      {
        u->req_snid = snid; /* for auto-identify */
        /* kill the user */
        if(ku)
        {
          char killmsg[128];
          snprintf(killmsg, sizeof(killmsg), 
            "LOGIN command used by %s", u->nick);
          irc_Kill(ku, s, killmsg);
        }
        irc_SvsNick(u, s, target);
      }
      
    }
    FREE(vhost);
    FREE(email);    
  }
}

/* s = service the command was sent to
   u = user the command was sent from */
void ns_recover(IRC_User *s, IRC_User *u)
{
  is_recover = 1;
  ns_login(s, u);
  return;
}

