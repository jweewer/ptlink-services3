/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2006 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************  

  Description: memoserv module
*/

#include "module.h"
#define MEMOSERV
#include "memoserv.h"
#include "path.h"
#include "my_sql.h"
#include "dbconf.h"
#include "lang/common.lh"
#include "lang/memoserv.lh"

/* module, version, description */
SVS_Module mod_info = {"memoserv", "5.0", "memoserv core module" };

#define DB_VERSION      4

/* ChangeLog
  5.0 - #5: split memoserv with a memoserv options table
        Added memoserv.4.sql changes
  4.0 - 0000305: foreign keys for data integrity
  3.0 - 0000265: remove nickserv cache system
*/
  
/* functions and events we require */
int e_nick_identify;
int mysql_connection;
int irc;
int e_expire = -1;

MOD_REQUIRES
  MOD_FUNC(dbconf_get_or_build)
  MOD_FUNC(irc)
  MOD_FUNC(mysql_connection)
  MOD_FUNC(e_nick_identify)
  MOD_FUNC(e_expire) /* we need this to run the expire routines */
MOD_END

/* functionsa and events we provide */
ServiceUser* memoserv_suser(void); /* the memoserv user */

MOD_PROVIDES
  MEMOSERV_FUNCTIONS
MOD_END

/* Local config */
static char* Nick;
static char* Username;
static char* Hostname;
static char* Realname;
static char* LogChan;
static int MaxMemosPerUser;
static int DefaultBuddyQuota;
static char* DefaultOptions;
static int ExpireTime = 0;

/* Local config flags */
static u_int32_t default_options = 0;

DBCONF_PROVIDES
  DBCONF_WORD(Nick,     "MemoServ", "Memoserv service nick")
  DBCONF_WORD(Username, "Services", "Memoserv service username")
  DBCONF_WORD(Hostname, "PTlink.net", "Memoserv service hostname")
  DBCONF_STR(Realname,  "Memoserv Service", "Memoserv service real name")
  DBCONF_WORD_OPT(LogChan,  "#Services.log", "Memoserv log channel")
  DBCONF_INT(MaxMemosPerUser, "20","Max number of memos per user")
  DBCONF_INT(DefaultBuddyQuota, "10", 
    "Number of memos to be kept exclusive for buddies")
  DBCONF_TIME(ExpireTime, "90d",
    "How long memos will be kept before expiring ?\n"
    "Please note that save memos will not expire")
  DBCONF_STR(DefaultOptions, "", "Default memoserv options")
DBCONF_END

/* internal variables */
ServiceUser msu;
int ms_log;

/* internal functions */
static int sql_upgrade(int version, int post);

/* core events */
void ev_ms_nick_identify(IRC_User* u, u_int32_t *snid);
int ev_ms_expire(void* dummy1, void* dummy2);

/* commands */
void ms_unknown(IRC_User* s, IRC_User* t);

int mod_rehash(void)
{
  if(dbconf_get_or_build(mod_info.name, dbconf_provides) < 0 )
  {
    errlog("Error reading dbconf!");
    return -1;
  }
  if(DefaultOptions)
  {    
    char *wrong = validate_options(DefaultOptions, memoserv_options, &default_options);
    if(wrong)
      errlog("Ignoring unknown memoserv default option: %s", wrong);
  }
  return 0;
}

int mod_load(void)
{
  /* first try to open the log file */
  ms_log = log_open("memoserv","memoserv");
  
  if(ms_log<0)
    {
      errlog("Could not open memoserv log file!");
      return -1;
    }

  if(sql_check_inst_upgrade(mod_info.name, DB_VERSION, sql_upgrade) < 0 )
    return -4;
    
  /* Create the memoserv client */
  msu.u = irc_CreateLocalUser(Nick, Username, Hostname, Hostname,
    Realname,"+ro");
    
  /* MS should join the log chan */  
  if(LogChan)
    {
      IRC_Chan *chan;
      log_set_irc(ms_log, Nick, LogChan);
      chan = irc_ChanJoin(msu.u, LogChan, CU_MODE_ADMIN | CU_MODE_OP );
      irc_ChanMode(msu.u, chan, "+Ostn");
    }  

  irc_AddUMsgEvent(msu.u, "*", (void*) ms_unknown); /* any other msg handler */
    
  /* Add actions  */
  mod_add_event_action(e_nick_identify, (ActionHandler) ev_ms_nick_identify);

  if(ExpireTime == 0)
    stdlog(L_INFO, "ExpireTime is not set, memos will not expire");
  else
    mod_add_event_action(e_expire, (ActionHandler) ev_ms_expire);
  
  return 0;
}

void
mod_unload(void)
{
  
  mod_del_event_action(e_expire, (ActionHandler) ev_ms_nick_identify);
  if(ExpireTime)
    mod_del_event_action(e_expire, (ActionHandler) ev_ms_expire);
  
  /* remove memoserv and all associated events */
  irc_QuitLocalUser(msu.u, "Removing service");  

}
  
void ms_unknown(IRC_User* s, IRC_User* t)
{
  send_lang(t, s, UNKNOWN_COMMAND, irc_GetLastMsgCmd());
}

void ev_ms_nick_identify(IRC_User* u, u_int32_t *snid)
{
  int nc = unread_memos_count(*snid);
  if(nc > 0)
    send_lang(u, msu.u, YOU_HAVE_X_UNREAD_MEMOS, nc);
}

/* to return the memoserv client */
ServiceUser* memoserv_suser(void)
{
  return &msu;
}

/* return count of memos for a given user */
int memos_count(u_int32_t snid)
{
  MYSQL_RES* res;
  MYSQL_ROW row;
  int count = 0;
  res = sql_query("SELECT count(*) FROM memoserv WHERE owner_snid=%d", snid);
  row = sql_next_row(res);
  if(row)
    count = atoi(row[0]);  
  sql_free(res);
  return count;  
}

/* return count of unread memos for a given user */
int unread_memos_count(u_int32_t snid)
{
  MYSQL_RES* res;
  MYSQL_ROW row;
  int count = 0;
  res = sql_query("SELECT count(*) FROM memoserv WHERE owner_snid=%d AND flags & %d", 
    snid, MFL_UNREAD);
  row = sql_next_row(res);
  if(row)
    count = atoi(row[0]);  
  sql_free(res);
  return count;  
}

/* this version takes care of sql upgrades */
int sql_upgrade(int version, int post)
{
  MYSQL_RES* res;
  MYSQL_ROW row;
  
  switch(version)
  {
    case 2:
      if(post) /* Upgrading to version 2, need to regenerate ids */
      {
        u_int32_t id = 1;
        u_int32_t osnid = 0;
        u_int32_t smid = 0;
        u_int32_t maxid = 0;
        log_log(ms_log, mod_info.name, "Regenerating memo ids");
        /* we need this to avoid id collisions */
        res = sql_query("SELECT id FROM memoserv ORDER BY id DESC LIMIT 1");
        row = sql_next_row(res);
        if(row)
          maxid = atoi(row[0]);
        sql_free(res);
        sql_execute("UPDATE memoserv SET id=id+%d WHERE id<%d", 
          maxid, MaxMemosPerUser+2);
        res = sql_query("SELECT id, owner_snid FROM memoserv ORDER BY owner_snid");
        while((row = sql_next_row(res)))
        {
          smid = atoi(row[0]);
          if(osnid != atoi(row[1]))
            id = 1;
          osnid = atoi(row[1]);
          sql_execute("UPDATE memoserv SET id=%d"
            " WHERE id=%d AND owner_snid=%d", id, smid, osnid);
          ++id;
        }
        sql_free(res);
        log_log(ms_log, mod_info.name, "Memo ids were generated");
      }
    break;
    case 3: /* pre-validation of integrity rules */
    if(!post) /* we need to check for "lost" memos */
    {
      int rowc = 0;
      res = sql_query("SELECT memoserv.owner_snid FROM memoserv"
        " LEFT JOIN nickserv ON (memoserv.owner_snid = nickserv.snid)"
        " WHERE memoserv.owner_snid IS NOT NULL AND nickserv.snid IS NULL");
      while((row = sql_next_row(res)))
      {
        log_log(ms_log, mod_info.name,
          "Deleting memos owned by deleted nick %s", row[0]);
        sql_execute("DELETE FROM memoserv WHERE owner_snid=%s", row[0]);
        ++rowc;
      }
      if(rowc)
        log_log(ms_log, mod_info.name, "Removed %d lost memos(s)", rowc);
      sql_free(res);
      rowc = 0;
      res = sql_query("SELECT memoserv.sender_snid FROM memoserv"
        " LEFT JOIN nickserv ON (memoserv.sender_snid = nickserv.snid)"
        " WHERE memoserv.sender_snid IS NOT NULL AND nickserv.snid IS NULL");
      while((row = sql_next_row(res)))
      {
        if(atoi(row[0]) == 0) /* this is corrected by the upgrade sql */
          continue;
        log_log(ms_log, mod_info.name,
          "Removing sender from lost nick %s", row[0]);
        sql_execute("UPDATE memoserv SET sender_snid=NULL"
          " WHERE sender_snid=%s", row[0]);
        ++rowc;
      }
      if(rowc)
        log_log(ms_log, mod_info.name, "Removed %d lost sender(s)", rowc);
      sql_free(res);      
    }
  }    
  return 1;
}

/*
 * Returns the options associated with a nick
 */
int memoserv_get_options(u_int32_t snid, int* maxmemos, int* bquota, u_int32_t* flags)
{
  MYSQL_RES* res;
  MYSQL_ROW row;
  *maxmemos = MaxMemosPerUser;
  *bquota = DefaultBuddyQuota;
  *flags = default_options;
  
  res = sql_query("SELECT maxmemos, bquota, flags "
    "FROM memoserv_options WHERE snid=%d", snid);
  /* upgrade/memoserv was unloaded ?  lets try to insert it */
  if((row = sql_next_row(res)))
  {
    int c = 0;
    *maxmemos = atoi(row[c++]);
    *bquota = atoi(row[c++]);    
    *flags = atoi(row[c++]);
    sql_free(res);
  }
  else /* upgrade/memoserv was unloaded ?  lets try to insert it */
  {
    sql_free(res);
    sqlb_init("memoserv_options");
    sqlb_add_int("snid", snid);
    sqlb_add_int("maxmemos", *maxmemos);
    sqlb_add_int("bquota", *bquota);
    sqlb_add_int("flags", *flags);
    if(sql_execute("%s", sqlb_insert()) < 0)
    {
      log_log(ms_log, mod_info.name, "Unable to insert options for nick %d", snid);
      return 0;
    }
  }
  return 1;
}

int ev_ms_expire(void* dummy1, void* dummy2)
{
  sql_execute("DELETE FROM memoserv WHERE (flags & %d) = 0"
  " AND t_send<%d", MFL_SAVED, irc_CurrentTime - ExpireTime);
  return 1;
}

