/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2006 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************

  Description: nickserv module

*/

#include "module.h"
#include "path.h"
#include "encrypt.h"
#include "my_sql.h"
#include "dbconf.h"
#define NICKSERV
#include "nickserv.h"
#include "nsmacros.h"
/* lang files */
#include "lang/common.lh"
#include "lang/nickserv.lh"

SVS_Module mod_info = 
  {"nickserv", "7.2", "nickserv core module" };
/* Change Log
  7.2 - #87: +p is not set on private nicks using ns_login
  7.1 -	#62: suspensions replace the forbid setting 
          Added nickserv.9.sql changes
        #61: MaxTimeForAuth setting to expire unauthenticated nicks
  7.0 - #50: nickserv login option to override nick language
        #10 : add nickserv suspensions
        Applied nickserv.8.sql for the above changes
  6.0 - #24 : move e_nick_register event registration to nickserv
        #22 : e_nick_recognize to distinguish +r nick recognitions
        #21 : remove unused/moved fields from nickserv table
        #19 : DefaultLang configuration item is required
        Added nickserv.7.sql for the above changes
  5.1 - 0000340: option NSRegChan to make new users join a channel
        0000330: nickserv links exchange option
        0000327: sadmins SSET nick vhost to set a virtual hostname
        Added nickserv.6.sql changes
  5.0 - 0000305: foreign keys for data integrity 
        Added nickserv.5.sql changes
  4.0 - 0000276: nick password expire option
        0000272: move nickserv security info to a specific table
        0000265: remove nickserv cache system  
        Added nickserv.4.sql changes
  3.0 - Added nickserv.3.sql changes
        0000255: new field to store nick expire time
  2.0 - Added nickserv.2.sql changes
        0000218: NickDefaultOptions to set nick default options
        0000243: protected nick set option
        0000245: use snid to track previous identify  
  1.1 - ghost recognition was not setting +r with DisableNickSecurityCode
*/
          
#define DB_VERSION	9

/** functions and events we require **/
/* void (*FunctionPointer)(void);*/
int mysql_connection;
int e_expire;
int irc;

MOD_REQUIRES
  MOD_FUNC(dbconf_get_or_build)
  MOD_FUNC(mysql_connection)
  MOD_FUNC(irc)
  MOD_FUNC(e_expire) /* we need this to run the expire routines */
MOD_END

/* functions we provide */
ServiceUser* nickserv_suser(void);
int e_nick_identify;
int e_nick_recognize;
int e_nick_register;
int e_nick_delete;
int e_nick_info;
int update_nick_online_info(IRC_User* u, u_int32_t snid, int lang);
int check_nick_security(u_int32_t snid, IRC_User *u, char* pass, char *email, int flags);

MOD_PROVIDES                                                                                
  MOD_FUNC(nickserv_suser)
  MOD_FUNC(update_nick_online_info)
  MOD_FUNC(check_nick_security)
  MOD_FUNC(e_nick_register)
  MOD_FUNC(e_nick_identify)
  MOD_FUNC(e_nick_recognize)
  MOD_FUNC(e_nick_delete)  
  MOD_FUNC(e_nick_info)
MOD_END

/* core events */
void ev_ns_new_user(IRC_User* u, void *null); /* needs to be fixed with IRC_Server */
void ev_ns_quit(IRC_User* u, char* reason);
void ev_ns_kill(IRC_User* u, IRC_User *killer);
void ev_ns_nick_change(IRC_User* u, char* lastnick);
void timer_ns_not_identifed(IRC_User* u, int tag);
int ev_ns_expire(void* dummy1, void* dummy2);
                          
/* commands */
void ns_unknown(IRC_User* s, IRC_User* t);

/* internal functions */
void apply_prefix_nchange(IRC_User *u);
int valid_for_registration(char *nick);
void set_offline_info(IRC_User *u);
int sql_upgrade(int version, int post);
/* int update_nick_online_info(IRC_User* u, u_int32_t snid, int lang); */

/** Local config */
static char* Nick;
static char* Username;
static char* Hostname;
static char* Realname;
static char* LogChan;
static char* NickProtectionPrefix;
static int MaxProtectionNumber;
static int MaxNickChanges;
static int MaxNicksPerEmail;
static int ExpireTime;
static int AgeBonusPeriod;
static int AgeBonusValue;
static int PassExpireTime;
static int NickSecurityCode;
static char* Root;
static int FailedLoginMax;
static int StrongPasswords;
static int SecurityCodeLenght;
static char* DefaultLang;
static int default_lang;
static int MaxTimeForAuth = 0;

DBCONF_PROVIDES
  DBCONF_WORD(Nick,     "NickServ", "Nickserv service nick")
  DBCONF_WORD(Username, "Services", "Nickserv service username")
  DBCONF_WORD(Hostname, "PTlink.net", "Nickserv service hostname")
  DBCONF_STR(Realname,  "Nickserv Service", "Nickserv service real name")
  DBCONF_WORD_OPT(LogChan,  "#Services.log", "Nickserv log channel")
  DBCONF_WORD(NickProtectionPrefix, "PTlink-",
    "Prefix to be applied on forced nick changes (Guest alike)")
  DBCONF_INT(MaxProtectionNumber, "9999",
    "Max. value to accept on random number for forced nick generation")
  DBCONF_INT(MaxNickChanges, "5",
    "Max. forced nick changes before a nick is killed")
  DBCONF_INT(MaxNicksPerEmail, "3",
    "How many nicks can be registered with the same email")
  DBCONF_TIME(ExpireTime, "60d",	
    "How long a nick will be ketp registed whitout logging in")
  DBCONF_TIME(AgeBonusPeriod, "60d",
    "After this time the nick expire time will be increased by AgeBonusValue\n"
    "for each number of AgeBonusPeriod times contained on the nick age")
  DBCONF_TIME(AgeBonusValue, "10d",
    "Bonus increase value for each AgeBonusPeriod contained on nick age")
  DBCONF_TIME(PassExpireTime, "0d", 
    "How long takes for the nick password to expire")
  DBCONF_SWITCH(NickSecurityCode, "on", 
    "Using nick security code will require the users to validate their email\n"
    "using a code received via email. Please check the docs dir for complete\n"
    "documentation on this")
  DBCONF_INT(SecurityCodeLenght, "10", "Security code lengh")
  DBCONF_WORD_OPT(Root, NULL,
    "Root nick, should only be used to add the first nick to the Root group")
  DBCONF_INT(FailedLoginMax, "3",
    "How many failed logins are allowed before killing the user")
  DBCONF_SWITCH(StrongPasswords, "off",
    "Passwords must be at least 8 chars long and contain a non alphabetic char")
  DBCONF_WORD(DefaultLang, "en_us",
    "Default language for unidentifed and new users")
  DBCONF_TIME(MaxTimeForAuth, "15d",
    "How long can a nick be kept without identying ?")
DBCONF_END
              
/* Local variables */
ServiceUser nsu;
int ns_log;

int mod_rehash(void)
{
  if(dbconf_get_or_build(mod_info.name, dbconf_provides) < 0 )
  {
    errlog("Error reading dbconf!");
    return -1;
  }
  lang2index(DefaultLang, default_lang)
  if(default_lang == -1)
  {
    log_log(ns_log, mod_info.name, 
      "Unknownn DefaultLang %s , assuming en_us", DefaultLang);
    default_lang = 0;
  }
  return 0;
}
int mod_load(void)
{

  if(ExpireTime> 0 && ExpireTime < 86400)
    {
      errlog("ExpireTime is too low, minimum is 1 day!");
      return -6;
    }
    
  /* first try to open the log file */
  ns_log = log_open("nickserv","nickserv");
  
  if(ns_log<0)
    {
      errlog("Could not open nickserv log file!");
      return -1;
    }

  if(sql_check_inst_upgrade(mod_info.name, DB_VERSION, sql_upgrade) < 0)
    return -4;
    
  /* Create the nickserv client */
  nsu.u = irc_CreateLocalUser(Nick, Username, Hostname, Hostname,
    Realname,"+ro");
  /* NS should join the log chan */  
  if(LogChan)
    {
      IRC_Chan *chan;
      log_set_irc(ns_log, Nick, LogChan);
      chan = irc_ChanJoin(nsu.u, LogChan, CU_MODE_ADMIN | CU_MODE_OP );
      irc_ChanMode(nsu.u, chan, "+Osnt");
    }
  /* Add unknown command handler */
  irc_AddUMsgEvent(nsu.u, "*", ns_unknown); /* any other msg handler */
  
  /* Add user events and register functions */
  irc_AddEvent(ET_NEW_USER, ev_ns_new_user); /* new user */
  irc_AddEvent(ET_NICK_CHANGE, ev_ns_nick_change); /* nick change */
  irc_AddEvent(ET_QUIT, ev_ns_quit); /* user quit */
  irc_AddEvent(ET_KILL, ev_ns_kill); /* user was killed */

  /* reset nick status */
  sql_query("UPDATE nickserv SET status=0");

  if(ExpireTime == 0)
    stdlog(L_INFO, "ExpireTime is not set, no nicks will expire");      
  else    
    mod_add_event_action(e_expire, (ActionHandler) ev_ns_expire);
  
  return 0;    
}

void
mod_unload(void)
{

  /* remove nickserv and all associated events */
  irc_QuitLocalUser(nsu.u, "Removing service");

  /* remove irc events */
  irc_DelEvent(ET_NEW_USER, ev_ns_new_user);
  irc_DelEvent(ET_NICK_CHANGE, ev_ns_nick_change);
  irc_DelEvent(ET_QUIT, ev_ns_quit);
  irc_DelEvent(ET_KILL, ev_ns_kill);
  lang_delete_assoc(); /* delete lang associations */
}

/* s = service the command was sent to
   u = user the command was sent from */
void ns_unknown(IRC_User* s, IRC_User* t)
{
  send_lang(t, s, UNKNOWN_COMMAND, irc_GetLastMsgCmd());
}


void timer_ns_not_identifed(IRC_User* u, int tag)
{ 
  
  if(MaxNickChanges && u->guest_count >= MaxNickChanges)
  {
    log_log(ns_log, mod_info.name,
      "Killing %s , too many nick changes", irc_UserMask(u));
    irc_Kill(u, nsu.u, "Too many guest nick changes");
    return;
  }
  apply_prefix_nchange(u);
}

/* new user was introduced
  Note: This is also called after a nick change for the new nick
*/
void ev_ns_new_user(IRC_User* u, void *null)
{
  u_int32_t snid = 0;
  u_int32_t flags = 0;
  int lang = 0;
  char *email = NULL;
  char *vhost = NULL;
  
  /* Set language according to the hostname */
  /* u->lang = lang_for_host(u->publichost); */
  u->lang = default_lang;
  lang = u->lang;
  if( sql_singlequery("SELECT snid, flags, lang, email, vhost"
    " FROM nickserv WHERE nick=%s",
    sql_str(irc_lower_nick(u->nick))) )
  {
    int c = 0;
    snid = sql_field_i(c++);
    flags = sql_field_i(c++);
    sql_field_i(c++);
    /* lang = sql_field_i(c++); */
    email = sql_field(c++);    
    vhost = sql_field(c++);
  }
    
  if(snid == 0) /* nick not registered or record not found */
  {
    if(irc_IsUMode(u, UMODE_IDENTIFIED)) /* remove +r just to be safe */
      irc_SvsMode(u, nsu.u, "-r");    
    if(valid_for_registration(u->nick))
      send_lang(u, nsu.u, NICK_IS_NOT_REGISTERED);
    u->flags = 0;
    u->status = 0;
  }
  else if((flags & NFL_SUSPENDED) && 
    sql_singlequery("SELECT reason FROM nickserv_suspensions WHERE snid=%d", snid))
  {
    send_lang(u, nsu.u, NICK_X_IS_SUSPENDED_X, u->nick, sql_field(0));
    apply_prefix_nchange(u);
  }
  else if((u->use_snid == snid) || (u->req_snid == snid)) 
  { /* user already identifed for this nick */
    if(vhost && irccmp(u->publichost, vhost)) /* we need to set the vhost */
      irc_ChgHost(u, vhost);         
    check_nick_security(snid, u, NULL, email, flags);    
    update_nick_online_info(u, snid, lang);
    if(u->req_snid == snid)
      mod_do_event(e_nick_identify, u, &snid);
    else
      mod_do_event(e_nick_recognize, u, &snid);
    u->req_snid = 0;      
  }
  else if(irc_IsUMode(u, UMODE_IDENTIFIED)) /* nick was identified */
  {
    u->flags = flags;  
    update_nick_online_info(u, snid, lang);
    mod_do_event(e_nick_recognize, u, &snid);      
  }
  else if(flags & NFL_PROTECTED)
  {  
    send_lang(u, nsu.u, NICK_IS_PROTECTED);
    apply_prefix_nchange(u);
  }
  else
  {
    u->flags = 0;
    u->status = 0;
    if(irc_IsUMode(u, UMODE_IDENTIFIED)) /* maybe nick was dropped ? */
      irc_SvsMode(u, nsu.u, "-r");
    send_lang(u, nsu.u, NICK_IS_REGISTERED);
    send_lang(u, nsu.u, CHANGE_IN_1M);
    irc_AddUTimerEvent(u, 60 , timer_ns_not_identifed, 0);
  }
}

void ev_ns_nick_change(IRC_User* u, char *lastnick)
{
  irc_CancelUserTimerEvents(u); /* we don't need events for changed nicks */
  set_offline_info(u);
  ev_ns_new_user(u, NULL);    /* Handle as usual nick logon */
}

void ev_ns_quit(IRC_User* u, char* reason)
{
  set_offline_info(u);
}

void ev_ns_kill(IRC_User* u, IRC_User *killer)
{
  set_offline_info(u);
}

/*
 Apply guest nick change (change nick to _nick-number)
 */ 
void apply_prefix_nchange(IRC_User *u)
{
  irc_SvsGuest(u, nsu.u, NickProtectionPrefix, MaxProtectionNumber);
}

/* check if nick can be registered */
int valid_for_registration(char *nick)
{
  static int nlen = 0;
  if(nlen==0)
    nlen = strlen(NickProtectionPrefix);
  if(ircncmp(NickProtectionPrefix, nick, nlen) == 0)
    return 0;
  return -1;
}

void set_offline_info(IRC_User *u)
{
  int i;
  char sql_expire[64];
  
  
  if(AgeBonusPeriod && AgeBonusValue)
    snprintf(sql_expire, sizeof(sql_expire)-1, "%d+FLOOR((%d-t_reg)/%d)*%d",
      (int)irc_CurrentTime+ExpireTime, (int)irc_CurrentTime, AgeBonusPeriod, AgeBonusValue);
  else
    snprintf(sql_expire, sizeof(sql_expire)-1,"%d",
      (int) irc_CurrentTime+ExpireTime);

  if(u->snid && (!MaxTimeForAuth || !NickSecurityCode || IsAuthenticated(u)))
    sql_execute("UPDATE nickserv SET status=0, t_expire=%s, t_seen=%d WHERE snid=%d",
      sql_expire, (int)irc_CurrentTime, u->snid);

  u->snid = 0;
  u->status = 0;
  u->flags = 0;

  for(i = 0; i < 16; ++i)
  {
    array_free(u->extra[i]);
    u->extra[i] = NULL;
  }
}

/* to return the nickserv client */
ServiceUser* nickserv_suser(void)
{
  return &nsu;
}

/* here we are going to use some raw mysql functions
 * because event actions for e_nick_delete may call sql_* functions
 * and override our query state
 */
int ev_ns_expire(void* dummy1, void* dummy2)
{
  time_t expire_start;
  MYSQL_RES* res;
  MYSQL_ROW row;
  int rowc = 0;

  /* expire code goes here */
  res = sql_query("SELECT snid, nick FROM nickserv "
      "WHERE (flags & %d = 0) AND (status & %d = 0) AND "
      "t_expire<%d", 
      NFL_NOEXPIRE | NFL_SUSPENDED, NST_ONLINE, irc_CurrentTime);      

  if(res)
    rowc = mysql_num_rows(res);

  if(rowc)
    log_log(ns_log, mod_info.name, "Will expire %d nick(s)", rowc);
    
  expire_start = time(NULL);    
  /* fetch the data */
  while((row = sql_next_row(res)))
  {
    IRC_User *u;
    u_int32_t snid = atoi(row[0]);
    u = irc_FindUser(row[1]);
    if(u && u->snid) /* nick is online and identified */
    {
      irc_SvsMode(u, nsu.u, "-r");
      u->snid = 0;
    }        
    log_log(ns_log, mod_info.name, "Expiring snid %d, nick %s", snid, row[1]);
    /* call related actions */
    mod_do_event(e_nick_delete, &snid, NULL);
    /* and delete it */
    sql_execute("DELETE FROM nickserv WHERE snid=%d", snid);
  }
  sql_free(res);
  if(rowc)
    log_log(ns_log, mod_info.name, 
      "Expire routine terminated, took %s to expire %d nick(s)", 
      str_time(time(NULL) - expire_start), rowc);
    
  /* expire suspensions */
  res = sql_query("SELECT snid FROM nickserv_suspensions "
    "WHERE duration>0 AND t_when+duration<%d", irc_CurrentTime);
  while((row = sql_next_row(res)))
  {
    u_int32_t snid = atoi(row[0]);
    log_log(ns_log, mod_info.name, "Expiring nick suspension for %d",
      snid);
    sql_execute("DELETE FROM nickserv_suspensions WHERE snid=%d", snid);
    sql_execute("UPDATE nickserv SET flags = (flags & ~%d), t_expire=%d"
      " WHERE snid=%d", NFL_SUSPENDED, irc_CurrentTime+ExpireTime, snid);
  }
  sql_free(res);
  
  return 0;
}

/* this function takes care of sql upgrades */
int sql_upgrade(int version, int post)
{
  if(version == 3 && post)/* Upgrading to version 3, need to set t_expire */
  {
    log_log(ns_log, mod_info.name, "Updating t_expire");    
                    
    if(AgeBonusPeriod && AgeBonusValue)
      sql_execute("UPDATE nickserv "
        "SET t_expire=t_seen+%d+FLOOR((%d-t_reg)/%d)*%d",
        ExpireTime, (int) time(NULL), AgeBonusPeriod, AgeBonusValue);
    else
      sql_execute("UPDATE nickserv "
        "SET t_expire=t_seen+%d,", ExpireTime);
  }
  return 0;
}

int update_nick_online_info(IRC_User* u, u_int32_t snid, int lang)
{
  char sql_expire[64];

  /* update nick info */
  u->snid = snid;
  /* u->lang = lang; */
  u->use_snid = snid;    
  u->status |= NST_ONLINE;
  /* do not update expire time until nick is authenticated */
  if(MaxTimeForAuth && NickSecurityCode && !IsAuthenticated(u))
  {
    send_lang(u, nsu.u, NS_EXPIRE_ON_OLD);
    snprintf(sql_expire, sizeof(sql_expire)-1, "t_expire");
  }
  else
  if(AgeBonusPeriod && AgeBonusValue)
    snprintf(sql_expire, sizeof(sql_expire)-1, "%d+FLOOR((%d-t_reg)/%d)*%d",
      (int)irc_CurrentTime+ExpireTime, (int)irc_CurrentTime, AgeBonusPeriod, AgeBonusValue);
  else
    snprintf(sql_expire, sizeof(sql_expire)-1,"%d",
      (int) irc_CurrentTime+ExpireTime);
        
  return sql_execute("UPDATE nickserv "
   "SET t_ident=%d, t_seen=%d, t_expire=%s,"
   "status = %d WHERE snid=%d",
     irc_CurrentTime, irc_CurrentTime, sql_expire,
     u->status, snid);  
}

/* 
 * Checks a nick security settings
 *  - If the password matches (optional)
 *  - If the email is authenticated
 *  - If the password expired
 *  - If the authentication expired
 * Returns -1 if the password doesn't match
 */
int check_nick_security(u_int32_t snid, IRC_User *u, char* pass, char* email, int flags)
{
  char umodes[10];
  int i = 0;        /* umodes index */
  int diff = 1;
  int full_reg = 0; /* is nick fully registered ? */
  time_t t_lset_pass;
  time_t t_lset_answer;
  time_t t_lauth;
  MYSQL_RES *res;
  MYSQL_ROW row;
  
  res = sql_query("SELECT pass, t_lset_pass, t_lset_answer, t_lauth"
           " FROM nickserv_security WHERE snid=%d", snid);
           
  if(!res || !(row = sql_next_row(res)))
  {
    sql_free(res);
    log_log(ns_log, mod_info.name, "Missing nick security record for %d",
      snid);                  
    return -1;
  }
    
  if(pass)
  {
    if(row[0])
      diff = memcmp(hex_bin(row[0]), encrypted_password(pass), 16);  
    if(diff != 0)
    {
      sql_free(res);
      return -1;
    }
  }

  t_lset_pass = atoi(row[1]);
  t_lset_answer = atoi(row[2]);
  t_lauth = atoi(row[3]);

  if(!NickSecurityCode)
    full_reg = 1;
  else
  {
    if(IsNull(email))
      send_lang(u, nsu.u, MISSING_SET_EMAIL);
    else if(!(flags & NFL_AUTHENTIC))
      send_lang(u, nsu.u, MISSING_AUTH);
    else full_reg = 1;
  }
  
  if(PassExpireTime && (irc_CurrentTime - t_lset_pass > PassExpireTime))
  {
    send_lang(u, nsu.u, NICK_PASSWORD_EXPIRED);    
    full_reg = 0;
  }
  
  if(full_reg) /* nick is fully registered */
  {
    /* set the umodes on nick */
    umodes[i++] = '+';
    if(flags & NFL_PRIVATE)
      umodes[i++] = 'p';
    umodes[i++] = 'r';
    umodes[i] = '\0';
    if(i > 1)
      irc_SvsMode(u, nsu.u, umodes);
    u->status |= NST_FULLREG;
  } else u->status &= ~NST_FULLREG;
  u->flags = flags;
  sql_free(res);
  return 0;
}
