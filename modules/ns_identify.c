/******************************************************************
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License           *
 * Please read the file COPYING for copyright information.        *
 ******************************************************************

  File: ns_identify.c
  Description: nickserv identify command

 *  $Id: ns_identify.c,v 1.12 2005/10/22 12:42:51 jpinto Exp $
*/

#include "module.h"
#include "nickserv.h"
#include "dbconf.h"
#include "encrypt.h"
#include "my_sql.h"
#include "nsmacros.h"
#include "lang/common.lh"
#include "lang/nickserv.lh"
#include "lang/ns_identify.lh"

SVS_Module mod_info =
/* module, version, description */
{ "ns_identify", "2.3", "nickserv identify command" };

/* Change Log
  2.2 - #21 : remove unused/moved fields from nickserv table
        #10 : add nickserv suspensions
  2.1 - 0000338: when the targer nick is used, login should behave like ns_identify
  2.0 - 0000276: nick password expire option
        0000272: move nickserv security info to a specific table
        0000265: remove nickserv cache system  
  1.2 - Added missing code for the issue 0000245
      - 0000255: new field to store nick expire time
  1.1 - 0000245: use snid to track previous identify
*/
  
/* functions and events we require */
ServiceUser* (*nickserv_suser)(void);
int (*update_nick_online_info)(IRC_User* u, u_int32_t snid, int lang);
int (*check_nick_security)
  (u_int32_t snid, IRC_User *u, char* pass, char* email, int flags);
int e_nick_identify;                                         
int ns_log;

MOD_REQUIRES
  MOD_FUNC(dbconf_get)
  MOD_FUNC(nickserv_suser)
  MOD_FUNC(e_nick_identify)
  MOD_FUNC(update_nick_online_info)
  MOD_FUNC(check_nick_security)
MOD_END

/* internal functions */
void ns_identify(IRC_User *s, IRC_User *u);

/* Local settings */
static int FailedLoginMax;
static int ExpireTime;
static int AgeBonusPeriod;
static int AgeBonusValue;

/* List of dbconf items we provide */
DBCONF_REQUIRES
  DBCONF_GET("nickserv", FailedLoginMax)
  DBCONF_GET("nickserv", ExpireTime)
  DBCONF_GET("nickserv", AgeBonusPeriod)
  DBCONF_GET("nickserv", AgeBonusValue)
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

/* Local variables */
ServiceUser* nsu;

int mod_load(void)
{

  ns_log = log_handle("nickserv");

  /* get service user */
  nsu = nickserv_suser();
          
  suser_add_cmd(nsu, "IDENTIFY", 
    ns_identify, IDENTIFY_SUMMARY, IDENTIFY_HELP);
  	
  return 0;
}

void
mod_unload(void)
{
  suser_del_mod_cmds(nsu, &mod_info);
}
 
/* s = service the command was sent to
   u = user the command was sent from */
void ns_identify(IRC_User *s, IRC_User *u)
{
  MYSQL_RES *res = NULL;
  MYSQL_ROW row;  
  char *pass = strtok(NULL, " ");
  char *extraopt = NULL;

  if(pass != NULL)
    extraopt = strtok(NULL, "");
    
  if(IsNull(pass))
    send_lang(u, s, IDENTIFY_SYNTAX);
  else if(u->snid)
    send_lang(u, s, ALREADY_IDENTIFIED);
  else if((res = sql_query("SELECT snid, flags, lang, email, vhost"
    " FROM nickserv WHERE nick=%s", sql_str(irc_lower_nick(u->nick))))
      && (row = sql_next_row(res)))
  {
    int c = 0;
    u_int32_t snid = atoi(row[c++]);    
    u_int32_t flags = atoi(row[c++]);    
    int lang = atoi(row[c++]);
    char *email = row[c++];
    char *vhost = row[c++];

    if((flags & NFL_SUSPENDED) &&
      sql_singlequery("SELECT reason FROM nickserv_suspensions WHERE snid=%d", snid))
    {
      send_lang(u,s, NICK_X_IS_SUSPENDED_X, u->nick, sql_field(0));
      return;
    }    
    if(check_nick_security(snid, u, pass, email, flags) == -1)
    {
      log_log(ns_log, mod_info.name, "Nick %s failed identify by %s",
        u->nick, irc_UserSMask(u));
      if(FailedLoginMax && ++u->fcount>FailedLoginMax)
      {
        log_log(ns_log, mod_info.name, 
          "Killing %s on too many failed identify attempts", u->nick);
        irc_Kill(u, s, "Too many failed identify attempts");
      }
      else
        send_lang(u, s, INCORRECT_PASSWORD);
      sql_free(res);    
      return;
    }
    else
    {
      send_lang(u, s, IDENTIFY_OK); 
      log_log(ns_log, mod_info.name, "Nick %s identified by %s",
        u->nick, irc_UserSMask(u));    
      update_nick_online_info(u, snid, lang);      
      if(vhost && irccmp(u->publichost, vhost)) /* we need to set the vhost */
        irc_ChgHost(u, vhost);      
      irc_CancelUserTimerEvents(u); /* delete the pending change nick event */
      mod_do_event(e_nick_identify, u, &snid);
    }
  }
  else
    send_lang(u, s, NICK_NOT_REGISTERED);    
  sql_free(res);
}

