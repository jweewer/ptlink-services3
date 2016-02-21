/**********************************************************************
 * PTlink IRC Services is (C) Copyright PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************

  Description: nickserv set command

 *  $Id: ns_set.c,v 1.15 2005/10/18 16:25:06 jpinto Exp $
*/

#include "nickserv.h"
#include "module.h"
#include "dbconf.h"
#include "encrypt.h"
#include "my_sql.h"
#include "email.h"
#include "nsmacros.h"
#include "ns_group.h"	/* is_sadmin */
#include "lang/common.lh"
#include "lang/ns_set.lh"
#include "lang/ns_register.lh"

SVS_Module mod_info =
/* module, version, description */
{"ns_set", "2.4", "nickserv set/sset command" };
/* Change Log
  2.4 - Added support for th USEMSG setting
  2.3 - #1 : Replace NS AUTOJOIN with CS AJOIN ADD/DEL
  2.2 - #12:  sset vhost should check for a valid hostname
  2.1 - 0000329: option to set a favorite link (to be used for links exchange)
        0000327: sadmins SSET nick vhost to set a virtual hostname
  2.0 - 0000287: ns_getpass and ns_getsec to recover password and security code
        0000276: nick password expire option
        0000272: move nickserv security info to a specific table
        0000265: remove nickserv cache system
  1.4 - 0000269: potential crash on ns set email
  1.3 - 0000246: help display with group filter
  1.2 - 0000224: SSET on password sets and a nickserv notice      
      - 0000243: protected nick set option      
  1.1 - don't check for emails count when setting to the same email
        0000227: nickserv SSET PASSWORD not working
        0000225: unable to reSET email address with 3 nicks registered
*/

/* external functions we need */
ServiceUser* (*nickserv_suser)(void);
u_int32_t (*find_group)(char *name);
int (*check_nick_security)
  (u_int32_t snid, IRC_User *u, char* pass, char* email, int flags);
int (*forbidden_email)(char *email);


MOD_REQUIRES 
  MOD_FUNC(dbconf_get)
  MOD_FUNC(nickserv_suser)   
  MOD_FUNC(check_nick_security)
  MOD_FUNC(is_sadmin)
  MOD_FUNC(find_group)
  EMAIL_FUNCTIONS
MOD_END

MOD_OPTIONS
  MOD_FUNC(forbidden_email)
MOD_END   

/* internal functions */
void set_command(IRC_User *u, IRC_User *s, char* tnick, u_int32_t tsnid, char *option, char *value, int is_sset);
void ns_set(IRC_User *s, IRC_User *u);
void ns_sset(IRC_User *s, IRC_User *u); /* sadmin set */

/* Remote config */
static int StrongPasswords;
static int NickSecurityCode;
static int SecurityCodeLenght;
static int MaxNicksPerEmail;

DBCONF_REQUIRES
  DBCONF_GET("nickserv", StrongPasswords)
  DBCONF_GET("nickserv", NickSecurityCode)
  DBCONF_GET("nickserv", SecurityCodeLenght)
  DBCONF_GET("nickserv", MaxNicksPerEmail)
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

ServiceUser* nsu;
int ns_log;
char *setemail_emails[MAX_LANGS];

int mod_load(void)
{
  nsu = nickserv_suser();
  ns_log = log_handle("nickserv");

  if(email_load("setemail", setemail_emails) < 0)
    return -1;
  
  suser_add_cmd(nsu, "SET", ns_set, SET_SUMMARY, SET_HELP);
  suser_add_cmd_g(nsu, "SSET", ns_sset, SSET_SUMMARY, SSET_HELP,
    find_group("Admin"));      
  return 0;
}

void
mod_unload(void)
{
  suser_del_mod_cmds(nsu, &mod_info);
  email_free(setemail_emails);
}

#define FLAG_SET(x,y) \
  { \
    if(IsNull(value)) \
      send_lang(u, s, VALUE_ON_OR_OFF); \
    else \
    if(strcasecmp(value,"on") == 0) \
      { \
        log_log(ns_log, mod_info.name, "%s %s %s %s %s", \
          u->nick, is_sset ? "SSET" : "SET", tnick, y, value); \
        if(u->snid == tsnid) u->flags |= (x); \
        send_lang(u, s, OPTION_X_ON, (y)); \
        sql_execute("UPDATE nickserv SET flags=(flags | %d) "\
          "WHERE snid=%d", (x), tsnid);\
      } else \
    if(strcasecmp(value,"off")  == 0) \
      { \
        log_log(ns_log, mod_info.name, "%s %s %s %s %s", \
          u->nick, is_sset ? "SSET" : "SET", tnick, y, value); \
        if(u->snid == tsnid) u->flags &= ~(x); \
        send_lang(u, s, OPTION_X_OFF, (y)); \
        sql_execute("UPDATE nickserv SET flags=(flags & ~%d) " \
          "WHERE snid=%d", (x), tsnid);\
      } else \
        send_lang(u, s, VALUE_ON_OR_OFF); \
  }

#define STRING_SET(x,y,z) \
  { \
    if(IsNull(value)) \
      { \
        log_log(ns_log, mod_info.name, "%s %s %s %s", \
          u->nick, is_sset ? "SSET" : "SET", tnick, option);\
        send_lang(u, s, (y)); \
      } \
    else \
      { \
        log_log(ns_log, mod_info.name, "%s %s %s %s %s", \
          u->nick, is_sset ? "SSET" : "SET", tnick, option, value);\
        send_lang(u, s, (z), value); \
      } \
     sql_execute("UPDATE nickserv SET %s=%s "\
       "WHERE snid=%d", (x), sql_str(value), tsnid);\
  }  
  
/* handles a set command */
void set_command(IRC_User *u, IRC_User *s, char* tnick, u_int32_t tsnid, char *option, char *value, int is_sset)
{
  int li;
  
  if(strcasecmp(option,"URL") == 0)
  {
#if 0  
    if(value && strncasecmp(value, "http://", 7))
      send_lang(u, s, URL_NEEDS_HTTP);
    else
#endif
      STRING_SET("url", URL_UNSET, URL_CHANGED_TO_X)
  }
  else if(strcasecmp(option,"FAVLINK") == 0)
  {
#if 0  
    if(value && strncasecmp(value, "http://", 7))
      send_lang(u, s, URL_NEEDS_HTTP);  
    else
    {
#endif    
      if(NickSecurityCode  && !IsAuthenticated(u))
        send_lang(u, s, NEEDS_AUTH_NICK);
      else
        STRING_SET("favlink", FAVLINK_UNSET, FAVLINK_CHANGED_TO_X)    
#if 0        
    }
#endif
  }
  else if(strcasecmp(option,"LOCATION") == 0)
    STRING_SET("location", LOCATION_UNSET, LOCATION_CHANGED_TO_X)
  else if(strcasecmp(option,"IMID") == 0)
    STRING_SET("imid", IMID_UNSET, IMID_CHANGED_TO_X)
  else if(strcasecmp(option,"PRIVATE") == 0)
    FLAG_SET(NFL_PRIVATE, "PRIVATE")
  else if(strcasecmp(option,"NONEWS") == 0)
    FLAG_SET(NFL_NONEWS, "NONEWS")
  else if(strcasecmp(option,"PROTECTED") == 0)
    FLAG_SET(NFL_PROTECTED, "PROTECTED")    
  else if(strcasecmp(option,"HIDEEMAIL") == 0)
    FLAG_SET(NFL_HIDEEMAIL, "HIDEEMAIL")
  else if(strcasecmp(option,"USEMSG") == 0)
    FLAG_SET(NFL_USEMSG, "USEMSG")
  else if(strcasecmp(option,"EMAIL") == 0)
  {    
    int diff = 1;
    char *email = NULL;
    char *securitycode = strtok(NULL, " ");
    if(value && forbidden_email && (forbidden_email(value) > 0))
    {
      send_lang(u, s, FORBIDDEN_EMAIL);
      return;
    }
      
    if(sql_singlequery("SELECT email FROM nickserv WHERE snid=%d",
      tsnid) && sql_field(0))
      email = strdup(sql_field(0));
    if(sql_singlequery("SELECT securitycode FROM nickserv_security WHERE snid=%d",
      tsnid) && sql_field(0) && securitycode)
    {
      diff = memcmp(hex_bin(sql_field(0)), encrypted_password(securitycode), 16);
      if(diff != 0)
        diff = strcasecmp(sql_field(0), securitycode);
    }
    
    if(!is_sset && NickSecurityCode && IsAuthenticated(u))
    {
      if(IsNull(securitycode))
      {
        FREE(email);
        send_lang(u, s, SET_EMAIL_SECURITY_REQUIRED);
        return;
      }
      else if(diff != 0)
      {
        FREE(email);
        send_lang(u, s, INVALID_SECURITY_CODE);              
        return;
       }
     }
    if(IsNull(value))
      send_lang(u, s, CANT_EMAIL_UNSET);
    else if((email && strcasecmp(value, email)) && 
      MaxNicksPerEmail && (reg_count_for_email(value) >= MaxNicksPerEmail))
        send_lang(u, s, ALREADY_X_WITH_EMAIL, MaxNicksPerEmail);
    else
    {      
      if(!is_email(value))
        send_lang(u, s, INVALID_EMAIL);
      else
      {
        securitycode = malloc(SecurityCodeLenght+1);
        rand_string(securitycode, SecurityCodeLenght, SecurityCodeLenght);            
        sql_execute("UPDATE nickserv SET email=%s, flags = (flags & ~%d)"
          " WHERE snid=%d", sql_str(value), NFL_AUTHENTIC, tsnid);
        sql_execute("UPDATE nickserv_security SET securitycode='%s'" 
          " WHERE snid=%d", 
          hex_str(encrypted_password(securitycode),16), tsnid);
        u->flags &= ~NFL_AUTHENTIC;
        if(NickSecurityCode)
        {
          email_init_symbols();
          email_add_symbol("nick", u->nick);
          email_add_symbol("email", value);
          email_add_symbol("securitycode", securitycode);
          if(tsnid == u->snid)
            u->flags &= ~NFL_AUTHENTIC;
          send_lang(u, s, EMAIL_REQUEST_TO_X, value);
          email_send(setemail_emails[u->lang]);
          irc_SvsMode(u, s, "-r");
        }
        else
          send_lang(u, s, EMAIL_CHANGED_TO_X, value);
        free(securitycode);
        log_log(ns_log, mod_info.name, "%s %s %s %s %s",
          u->nick, is_sset ? "SSET" : "SET", tnick, option, value);
      } 
    FREE(email);
    }
  }
  else
  if(strcasecmp(option,"LANGUAGE") == 0)
    {
      if(value)
        {
          lang2index(value, li);
        }
      else
        li = -1;
      if(li==-1)
        {
          send_lang(u, s, INVALID_LANGUAGE_X, value ? value : "");
        }
      else
        {
          u->lang = li;
          send_lang(u, s, LANGUAGE_CHANGED_TO_X, value);
          sql_execute("UPDATE nickserv SET lang=%d WHERE snid=%d", 
            li, tsnid);          
        }
    }
  else 
  if(strcasecmp(option,"PASSWORD") == 0)
  {
    if(IsNull(value))
      send_lang(u, s, MANDATORY_PASSWORD);
    else
    if(StrongPasswords && is_weak_passwd(value))
      send_lang(u, s, WEAK_PASSWORD);
    else
    {
      sql_execute("UPDATE nickserv_security SET pass='%s', t_lset_pass=%d " 
        "WHERE snid=%d", hex_str(encrypted_password(value), 16), (int) irc_CurrentTime,
        tsnid);
      if(!is_sset)
        send_lang(u, s, PASSWORD_CHANGED_TO_X, value);
      else
        send_lang(u, s, PASSWORD_CHANGED_FOR_X_TO_X, tnick, value);
      log_log(ns_log, mod_info.name, "%s %s %s %s ******",
        u->nick, is_sset ? "SSET" : "SET", tnick, option);
      if(u->snid == tsnid);
      {
        char *email = NULL;
        if(sql_singlequery("SELECT email FROM nickserv WHERE snid=%d", tsnid))
          email = sql_field(0);
        check_nick_security(tsnid, u, NULL, email, u->flags);
      }
     }
  }
  else if(is_sset == 0)
    send_lang(u, s, UNKNOWN_OPTION_X, option);
  else
  if(strcasecmp(option,"NOEXPIRE") == 0)
    FLAG_SET(NFL_NOEXPIRE, "NOEXPIRE")
  else 
  if(strcasecmp(option, "VHOST") == 0)
  {
    if(value && !irc_IsValidHostname(value))
      send_lang(u, s, NS_SET_INVALID_HOST_X, value);
    else
      STRING_SET("vhost", VHOST_UNSET, VHOST_CHANGED_TO_X)
  }
  else
    send_lang(u, s, UNKNOWN_OPTION_X, option);
}

#undef STRING_SET
#undef FLAG_SET
 
/* s = service the command was sent to
   u = user the command was sent from */
void ns_set(IRC_User *s, IRC_User *u)
{
  u_int32_t source_snid;
  char *option, *value;

  CHECK_IF_IDENTIFIED_NICK
  
  option = strtok(NULL, " ");
  if(!IsNull(option) && (strcasecmp(option, "LOCATION") == 0))
    value = strtok(NULL, "");
  else
    value = strtok(NULL, " ");
            
  if(IsNull(option))
    {
      send_lang(u, s, NICK_SET_SYNTAX);
      return;
    }
    
  set_command(u, s, u->nick, u->snid, option, value, 0);
}
#undef STRING_SET

/* s = service the command was sent to
   u = user the command was sent from */
void ns_sset(IRC_User *s, IRC_User *u)
{
  u_int32_t source_snid;
  u_int32_t tsnid;
  char *nick, *option = NULL, *value = NULL;
  
  nick = strtok(NULL, " ");
  if(nick)
    option = strtok(NULL, " ");  

  CHECK_IF_IDENTIFIED_NICK
    
  if(!IsNull(nick) && !IsNull(option) && (strcasecmp(option,"LOCATION")==0))
    value = strtok(NULL, "");
  else
    value = strtok(NULL, " ");

  if(IsNull(nick) || IsNull(option))
      send_lang(u, s, NICK_SSET_SYNTAX);
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
