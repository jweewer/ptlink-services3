/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************  

  Description: nickserv register command

 *  $Id: ns_register.c,v 1.13 2005/10/18 16:25:06 jpinto Exp $
*/

#include "nickserv.h"
#include "module.h"
#include "dbconf.h"
#include "encrypt.h"
#include "my_sql.h"
#include "email.h"
#include "nsmacros.h"
#include "lang/ns_register.lh"

SVS_Module mod_info =
/* module, version, description */
{"ns_register", "2.3","nickserv register command" };

/* Change Log
  2.3 - #61: MaxTimeForAuth setting to expire unauthenticated nicks
  2.2 - #24 : move e_nick_register event registration to nickserv
  2.1 - 0000340: WelcomeChan option to make new users join a channel
        0000332: ns_register missing call for the e_nick_register event
        0000330: nickserv links exchange option
  2.0 - 0000277: ns_blacklist to support email blacklists
        0000265: remove nickserv cache system
  1.2 - 0000255: new field to store nick expire time
  1.1 - 0000218: NickDefaultOptions to set nick default options
*/
            
/* functions/events we require */
ServiceUser* (*nickserv_suser)(void);
int (*forbidden_email)(char *email);
int mysql_connection;
int ns_log;
int e_nick_register;

MOD_REQUIRES
  MOD_FUNC(dbconf_get)
  MOD_FUNC(dbconf_get_or_build)
  MOD_FUNC(nickserv_suser)
  MOD_FUNC(mysql_connection)  
  MOD_FUNC(e_nick_register)
  EMAIL_FUNCTIONS
MOD_END

MOD_OPTIONS
  MOD_FUNC(forbidden_email)
MOD_END

/* functions and events we provide */

/* internal functions */
void ns_register(IRC_User *s, IRC_User *u);
void parse_nick_def_options(void);

/* Remote config */
static int StrongPasswords;
static int MaxNicksPerEmail;
static int NickSecurityCode;
static int SecurityCodeLenght;
static int ExpireTime;
static char* NickProtectionPrefix;
static int MaxTimeForAuth = 0;

DBCONF_REQUIRES
  DBCONF_GET("nickserv", StrongPasswords)
  DBCONF_GET("nickserv", MaxNicksPerEmail)
  DBCONF_GET("nickserv", NickSecurityCode)
  DBCONF_GET("nickserv", SecurityCodeLenght)
  DBCONF_GET("nickserv", ExpireTime)
  DBCONF_GET("nickserv", NickProtectionPrefix)
  DBCONF_GET("nickserv", MaxTimeForAuth)
DBCONF_END

/* Local config */
static char* NickDefaultOptions;
static int LinkExchange;
static char* WelcomeChan;
DBCONF_PROVIDES
  DBCONF_WORD_OPT(NickDefaultOptions, NULL, 
    "Nick default SET options (value1,value2...")
  DBCONF_SWITCH(LinkExchange, "off",
    "Welcome emails will include favorite links from random users")
  DBCONF_WORD_OPT(WelcomeChan, "#PTlink",
    "New registered users will be joined to this channel")
DBCONF_END

/* this is called before load and at services rehash */
int mod_rehash(void)
{
  if(dbconf_get(dbconf_requires) < 0 )
  {
    errlog("Error reading dbconf!");
    return -1;
  }
  if(dbconf_get_or_build(mod_info.name, dbconf_provides) < 0 )
  {
    errlog("Error reading dbconf!");
    return -1;
  }
  parse_nick_def_options();
  return 0;
}

static ServiceUser* nsu;
u_int32_t nick_def_options = 0;
char* welcome_emails[MAX_LANGS];

int mod_load(void)
{

  ns_log = log_handle("nickserv");
  
  if(email_load("welcome", welcome_emails) < 0)
    return -1;      
    
  nsu = nickserv_suser();
  suser_add_cmd(nsu,
    "REGISTER", ns_register, REGISTER_SUMMARY, REGISTER_HELP);
  return 0;
}

void
mod_unload(void)
{
   suser_del_mod_cmds(nsu, &mod_info);
   email_free(welcome_emails);
}
 
/* s = service the command was sent to
   u = user the command was sent from */
void ns_register(IRC_User *s, IRC_User *u)
{
  
  char *pass = strtok(NULL, " ");    
  char *email = strtok(NULL, " ");
  
  /* syntax validation */
  if((IsNull(pass)) || (IsNull(email)))
    send_lang(u, s, NICK_REGISTER_SYNTAX);    
  /* check requirements */
  else if(!is_email(email)) 
    send_lang(u, s, INVALID_EMAIL);
  else if(StrongPasswords && is_weak_passwd(pass))
    send_lang(u, s, WEAK_PASSWORD);
  else if(NickProtectionPrefix &&
    !strncasecmp(u->nick, NickProtectionPrefix, strlen(NickProtectionPrefix)))
      send_lang(u, s, NICK_CANNOT_BE_REGISTERED, u->nick);
  else if(nick2snid(u->nick) != 0)
    send_lang(u, s, NICK_ALREADY_REGISTERED);
  else if(forbidden_email && (forbidden_email(email) > 0))
    send_lang(u, s, FORBIDDEN_EMAIL);
  else if(MaxNicksPerEmail && (reg_count_for_email(email) >= MaxNicksPerEmail))
    send_lang(u, s, ALREADY_X_WITH_EMAIL, MaxNicksPerEmail);
  else /* execute operation */
    {
      u_int32_t snid = 0;
      char* securitycode;
      sqlb_init("nickserv");
      sqlb_add_str("nick", irc_lower_nick(u->nick));
      sqlb_add_str("email", email);
      sqlb_add_int("flags", nick_def_options);
      sqlb_add_int("status", NST_ONLINE);
      sqlb_add_int("t_reg", irc_CurrentTime);
      sqlb_add_int("t_ident", irc_CurrentTime);
      sqlb_add_int("t_seen", irc_CurrentTime);      
      if(MaxTimeForAuth && NickSecurityCode)
        sqlb_add_int("t_expire", irc_CurrentTime + MaxTimeForAuth);
      else
        sqlb_add_int("t_expire", irc_CurrentTime + ExpireTime);
      sqlb_add_int("lang", u->lang);
      securitycode = malloc(SecurityCodeLenght+1);
      rand_string(securitycode, SecurityCodeLenght, SecurityCodeLenght);

      snid = sql_execute("%s", sqlb_insert());
      if(snid == 0)
      {
        free(securitycode);
        send_lang(u, s, NICK_REGISTER_FAIL);
        return ;
      } 
      else
      {
        sqlb_init("nickserv_security");              
        sqlb_add_int("snid", snid);
        sqlb_add_str("pass", hex_str(encrypted_password(pass),16));
        sqlb_add_str("securitycode", 
          hex_str(encrypted_password(securitycode),16));
        sqlb_add_int("t_lset_pass", (int) irc_CurrentTime);
        sql_execute("%s", sqlb_insert());
      }
      u->snid = snid;
      u->flags = nick_def_options;
      u->status = NST_ONLINE;      
      if(NickSecurityCode)
        {
          email_init_symbols();
          email_add_symbol("nick", u->nick);
          email_add_symbol("email", email);
          email_add_symbol("securitycode", securitycode);
          if(LinkExchange)
          { /* pick a random favorite link */
            sql_singlequery("SELECT nick, favlink FROM nickserv"
              " WHERE favlink IS NOT NULL AND flags & %d "
              " ORDER BY RAND() LIMIT 1", NFL_AUTHENTIC);
            if(sql_field(0) && sql_field(1)) /* just to be safe */
              email_add_symbol("linkexchange", 
                lang_str(u, LINK_EXCHANGE_X_X, sql_field(0), sql_field(1)));
            else
              email_add_symbol("linkexchange", "");
          } else email_add_symbol("linkexchange", "");
          if(email_send(welcome_emails[u->lang]) < 0)
          {
            log_log(ns_log, mod_info.name, "Error sending welcome email to %s by %s",
              email, irc_UserMask(u));
            send_lang(u, s, WELCOME_ERROR);
          }
          else
          {
            log_log(ns_log, mod_info.name, "Welcome email was sent to %s by %s",
              email, irc_UserMask(u));
            send_lang(u, s, WELCOME_SENT);
            if(WelcomeChan)
              irc_SvsJoin(u, s, WelcomeChan);
          }
        }
      else /* no security code required */
        {
          send_lang(u, s, NICK_REGISTER_SUCCESS);
          irc_SvsMode(u, s, "+r");            
          if(WelcomeChan)
            irc_SvsJoin(u, s, WelcomeChan);
        }
      free(securitycode);
      log_log(ns_log, mod_info.name, "Nick %s [%s] registered by %s",
        u->nick, email, irc_UserSMask(u));
      mod_do_event(e_nick_register, u, &snid);
    }
}
/* parse NickDefaultOptions */
void parse_nick_def_options(void)
{
  char* options;
  char* opt;
  
  if(NickDefaultOptions == NULL)
    return;
    
  options = strdup(NickDefaultOptions);  
  opt = strtok(options,",");
  while(opt)
    {
      int i;
      i = mask_value(nick_options_mask, opt);
      if( i == 0)
      	errlog("Unknown NickDefaultOption %s", opt);
      else
      	nick_def_options |= i;
      opt = strtok(NULL,",");
    }
  FREE(options);
}

