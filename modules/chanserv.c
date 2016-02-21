/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2006 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************

  Description: chanserv module

*/

#include "chanserv.h"
#include "module.h"
#include "chanrecord.h"
#include "my_sql.h"
#include "dbconf.h"
#include "lang/common.lh"
#include "lang/chanserv.lh"

/* module, version, description */
SVS_Module mod_info =
{"chanserv", "5.1", "chanserv core module" };

#define DB_VERSION	4

/* ChangeLog
  5.1 - #65: Fixed Chan Drop & BotServ
  5.0 - #26, Added chanserv suspensions (DB_VERSION = 4)
  4.1 - 0000321: HelpChan option to set +h when ops join the help chan
  4.0 - 0000320: chanserv is not updating memory records during nick delete
        0000317: ChanServ doesn't keep on the logchan when RestrictedChans is enabled
        0000311: non registered nicks can bypass topiclock
        0000305: foreign keys for data integrity
        Added chanserv.3.sql changes
        0000302: Add dconf option for DefaultMlock          
  3.0 - 0000265: remove nickserv cache system
        0000261: mlock option
  2.2 - 0000258: chanserv topiclock option
  2.1 - sdata is now set/reset also for local users join/parts
  2.0 - Added chanserv.2.sql changes
        0000237: RestrictedChans option (to match the ircd option)
  */
            


/** functions/events we require **/
/* void (*FunctionPointer)(void);*/
int mysql_connection;
int e_nick_delete;
int e_expire;
int irc;

MOD_REQUIRES
  MOD_FUNC(irc)
  MOD_FUNC(dbconf_get_or_build)  
  MOD_FUNC(mysql_connection)
  MOD_FUNC(e_nick_delete)
  MOD_FUNC(e_expire)
MOD_END

/* functions we provide */
ServiceUser* chanserv_suser(void);
int e_regchan_join;
int e_chan_register;
int e_chan_delete;

MOD_PROVIDES                                                                                
  MOD_FUNC(chanserv_suser)
  MOD_FUNC(e_regchan_join)    
  MOD_FUNC(e_chan_register)
  MOD_FUNC(e_chan_delete)
MOD_END

/* core events */
void ev_cs_chan_join(IRC_Chan *chan , IRC_ChanNode *cnode);
void ev_cs_chan_part(IRC_Chan *chan , IRC_User *user);
void ev_cs_chan_topic(IRC_Chan *chan , IRC_User *user);
void ev_cs_deop(IRC_Chan *chan, IRC_User *user);
int ev_cs_nick_delete(int* snid, void *dummy);
int ev_cs_expire(void* dummy1, void* dummy2);
void ev_cs_new_server(IRC_Server* nserver, IRC_Server* from);
int sql_upgrade(int version, int post);

/* commands */
void cs_unknown(IRC_User* s, IRC_User* t);

/** Local config */
static char* Nick;
static char* Username;
static char* Hostname;
static char* Realname;
static char* LogChan;
static int ExpireTime;
static int MaxChansPerUser;
static int RestrictedChans;
static int NeedsAuth;
static char* DefaultMlock;

DBCONF_PROVIDES
  DBCONF_WORD(Nick,     "ChanServ", "Chanserv service nick")
  DBCONF_WORD(Username, "Services", "Chanserv service username")
  DBCONF_WORD(Hostname, "PTlink.net", "Chanserv service hostname")
  DBCONF_STR(Realname, 	"Chanserv Service", "Chanserv service real name")
  DBCONF_WORD_OPT(LogChan,  "#Services.log", "Chanserv log channel")
  DBCONF_TIME(ExpireTime,"15d", 
    "How long a channel will be kept without being used")
  DBCONF_INT(MaxChansPerUser, "20", "How many channels an user can register")
  DBCONF_SWITCH(RestrictedChans, "off", 
    "When services connect chanserv joins and leaves for all registered chans")
  DBCONF_SWITCH(NeedsAuth, "off",
    "Chanserv commands requires the nick to be authenticated")
  DBCONF_STR_OPT(DefaultMlock, "+nt", 
    "Default mlock to be set on new registered channels")
DBCONF_END
/* Local variables */
ServiceUser csu;
int cs_log;

/* this is called before load and at services rehash */
int mod_rehash(void)
{
  if(dbconf_get_or_build(mod_info.name, dbconf_provides) < 0 )
  {
    errlog("Error reading dbconf!");
    return -1;
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
  cs_log = log_open("chanserv","chanserv");
  
  if(cs_log<0)
    {
      errlog("Could not open chanserv log file!");
      return -1;
    }

  if(sql_check_inst_upgrade(mod_info.name, DB_VERSION, sql_upgrade) < 0)
    return -4;

  /* Create chanserv user */      
  csu.u = irc_CreateLocalUser(Nick, Username, Hostname, Hostname,
    Realname,"+ro");
  
  if(LogChan)
  {
    IRC_Chan *chan;
    log_set_irc(cs_log, Nick, LogChan);
    chan = irc_ChanJoin(csu.u, LogChan, CU_MODE_ADMIN|CU_MODE_OP);
    irc_ChanMode(csu.u, chan, "+Ostn");
  }
        
  irc_AddUMsgEvent(csu.u, "*", cs_unknown); /* any other msg handler */

  /* Add user events */
  irc_AddEvent(ET_CHAN_JOIN, ev_cs_chan_join);
  irc_AddEvent(ET_CHAN_PART, ev_cs_chan_part);
  irc_AddEvent(ET_CHAN_TOPIC, ev_cs_chan_topic);
/*  irc_AddCmodeChange("-o", ev_cs_deop); */
  
  /* reset chanserv status */
  sql_query("UPDATE chanserv SET status=0 WHERE status<>0");
  
  /* setup delete routines */
  mod_add_event_action(e_nick_delete, (ActionHandler) ev_cs_nick_delete);  
  mod_add_event_action(e_expire, (ActionHandler) ev_cs_expire);  
  
  if(RestrictedChans)
    irc_AddEvent(ET_NEW_SERVER, ev_cs_new_server);
    
  return 0;
}

void mod_unload(void)
{
  
  /* remove chanserv and all associated events */
  irc_QuitLocalUser(csu.u, "Removing service");

  /* delete events */
  irc_DelEvent(ET_CHAN_JOIN, ev_cs_chan_join);
  irc_DelEvent(ET_CHAN_PART, ev_cs_chan_part);  
  if(RestrictedChans)
    irc_DelEvent(ET_NEW_SERVER, ev_cs_new_server);
}
 
void cs_unknown(IRC_User* s, IRC_User* t)
{
  send_lang(t, s, UNKNOWN_COMMAND, irc_GetLastMsgCmd());
}

/* this is called after a join */
void ev_cs_chan_join(IRC_Chan* chan, IRC_ChanNode* cn)
{
  ChanRecord *cr;
  int remote_users;
  
  remote_users = chan->users_count - chan->lusers_count;
  cr = chan->sdata;
  if(cr == NULL)
    {
      cr = OpenCR(chan->name);
      chan->sdata = cr;
    }

  /* we don't do nothing for local users */
  if(irc_IsLocalUser(cn->user))
    return;
    
  /* first remote user joining the channel */
  if(remote_users == 1)
  {        
    /* first remote user joining a registered channel */    
    if(cr) 
    {
      irc_ChanMode(csu.u, chan, "+r");
      if(cr->mlock) /* we just set, the apply is called on next mode  */
        irc_ChanMLockSet(csu.u, chan, cr->mlock);
        
      if(irc_ConnectionStatus() == 2)
        send_lang(cn->user, csu.u, CHAN_X_IS_REGISTERED, chan->name);
      if(cr->last_topic_setter && cr->last_topic)
      {          
        if((chan->last_topic == NULL) || 
           (chan->last_topic && (strcmp(chan->last_topic, cr->last_topic)!=0)))
              irc_ChanTopic(chan, cr->last_topic_setter, cr->t_ltopic, cr->last_topic);
      }
    }
    else if(irc_ConnectionStatus() == 2)
      send_lang(cn->user, csu.u, CHAN_1ST_X_X_X, 
        cn->user->nick, chan->name, chan->name);    
  }  
  
  /* for all registered channels */
  if(cr)
    {
      if(irc_ConnectionStatus() == 2)
        {
          if(cr->url)
            irc_SendRaw(NULL, "328 %s %s : %s",
              cn->user->nick, cr->name, cr->url);
          if(cr->entrymsg)
            irc_SendNotice(cn->user, csu.u, "%s %s", 
              cr->name, cr->entrymsg);
        }
        
      /* update maxusers if required */
      if(remote_users > cr->maxusers) /* new record*/
        {
          cr->maxusers = remote_users;
          cr->t_maxusers = irc_CurrentTime;
          UpdateCR(cr);
        }        
      /* call the event */  
      mod_do_event(e_regchan_join, cr, cn);
    }
}

/* this is called when a user parts  */
void ev_cs_chan_part(IRC_Chan* chan, IRC_User *user)
{  
  if(chan->users_count == 1 && chan->sdata)
  {
    CloseCR(chan->sdata);  
    chan->sdata = NULL;
  }
}

void ev_cs_deop(IRC_Chan *chan, IRC_User *user)
{
 /* send_lang(user, csu.u, CHAN_DEOP, user->nick, chan->name); */
}

/* to return the nickserv client */
ServiceUser* chanserv_suser(void)
{
  return &csu;
} 

/* this is called when a nick is deleted */
int ev_cs_nick_delete(int *snid, void *dummy)
{
  MYSQL_RES* res;
  MYSQL_ROW row;
  ChanRecord* cr;
  
  /* remove the nick from successors */
  res = sql_query("SELECT scid, name FROM chanserv WHERE successor=%d", *snid);
  while((row = sql_next_row(res)))
  {  
    u_int32_t scid = atoi(row[0]);
    IRC_Chan *chan;
    log_log(cs_log, mod_info.name, "Removing successor on %s (was %d)",
      row[1], *snid);
    chan = irc_FindChan(row[1]);
    if(chan && (cr == chan->sdata))
      cr->successor = 0;
    sql_execute("UPDATE chanserv SET successor=NULL"
      " WHERE scid=%d", scid);
  }  
  sql_free(res);
  
  /* transfer owned channel to successors */
  res = sql_query("SELECT scid, name, successor FROM chanserv WHERE founder=%d"
    " AND successor IS NOT NULL", *snid);
  while((row = sql_next_row(res)))
  {
    u_int32_t scid = atoi(row[0]);
    IRC_Chan *chan;
    log_log(cs_log, mod_info.name, "Transfering channel %s (from %d to %s)",
      row[1], *snid, row[2]);
    chan = irc_FindChan(row[1]);
    if(chan && (cr == chan->sdata))
    {    
      cr->founder = cr->successor;
      cr->successor = 0;
    }
    sql_execute("UPDATE chanserv SET founder=successor, successor=NULL"
      " WHERE scid=%d", scid);
  }
  sql_free(res);
/*  
  sql_execute("UPDATE chanserv SET founder=successor, successor=NULL WHERE founder=%d"
    " AND successor IS NOT NULL", *snid);
*/
  /* now delete remaining owned channels */
  res = sql_query("SELECT scid, name FROM chanserv WHERE founder=%d",
    *snid);
    
  while((row = sql_next_row(res)))
  {
    u_int32_t scid = atoi(row[0]);
    IRC_Chan *chan;
    sql_execute("DELETE FROM chanserv WHERE scid=%d", scid);
    chan = irc_FindChan(row[1]);    
    if(chan && chan->sdata)
    {
      irc_ChanMode(csu.u, chan, "-r");
      CloseCR(chan->sdata);
      chan->sdata = NULL;
      if(chan->local_user)
        irc_ChanPart(chan->local_user, chan);
    }          
    log_log(cs_log, mod_info.name, "Deleted channel %d, %s from deleted nick %d",
      scid, row[1], *snid);
    mod_do_event(e_chan_delete, &scid, NULL);
  }   
  
  sql_free(res);
  return 0;
}

void ev_cs_chan_topic(IRC_Chan *chan , IRC_User *user)
{
  ChanRecord* cr = chan->sdata;
  u_int32_t snid = 0;
  if(user)
    snid = user->snid;
    
  if(cr)
  {
    if(IsTopicLock(cr) && ((snid==0) || (snid != cr->founder)))
    {
      if(cr->last_topic_setter && cr->last_topic)
      {          
        if((chan->last_topic == NULL) || 
          (chan->last_topic && (strcmp(chan->last_topic, cr->last_topic)!=0)))
            irc_ChanTopic(chan, cr->last_topic_setter, cr->t_ltopic, cr->last_topic);
      }
    }
    else
    {
      FREE(cr->last_topic);
      SDUP(cr->last_topic, chan->last_topic);
      FREE(cr->last_topic_setter);
      SDUP(cr->last_topic_setter, chan->last_topic_setter);  
      UpdateCR(cr);
    }
  }  
}

/*
 * Expires chans
 */
int ev_cs_expire(void* dummy1, void* dummy2)
{
  time_t expire_start;
  MYSQL_RES* res;
  MYSQL_ROW row;
  int rowc = 0;

  /* expire code goes here */
  res = sql_query ("SELECT scid, name FROM chanserv WHERE "
    "(flags & %d = 0) AND t_last_use < %d",
    CFL_NOEXPIRE, time(NULL)-ExpireTime);
  if(res)
    rowc = mysql_num_rows(res);
    
  if(rowc == 0)
    {
      sql_free(res);
      return 0;
    }

  log_log(cs_log, mod_info.name, "Will expire %d chan(s)", rowc);
  expire_start = time(NULL);
 
  rowc = 0; 
  /* fetch the data */
  while((row = sql_next_row(res)))
    {
      IRC_Chan* chan;
      u_int32_t scid = atoi(row[0]);
      rowc++;
      chan = irc_FindChan(row[1]);
      if(chan && chan->sdata)
        {
          irc_ChanMode(csu.u, chan, "-r");
          CloseCR(chan->sdata);
          chan->sdata = NULL;
          if(chan->local_user)
            irc_ChanPart(chan->local_user, chan);
        }        
      log_log(cs_log, mod_info.name, "Expiring scid %d,  %s", scid, row[1]);
      /* first call related actions */
      mod_do_event(e_chan_delete, &scid, NULL);
      /* now really delete it */
      sql_execute("DELETE FROM chanserv WHERE scid=%d", scid);
    }
  
  sql_free(res);
  log_log(cs_log, mod_info.name, "Expired %d channel(s), took %s",
    rowc, str_time(time(NULL) - expire_start));

  /* expire suspensions */
  res = sql_query("SELECT scid FROM chanserv_suspensions "
    "WHERE duration>0 AND t_when+duration<%d", irc_CurrentTime);
  while((row = sql_next_row(res)))
  {
    u_int32_t scid = atoi(row[0]);
    log_log(cs_log, mod_info.name, "Expiring chan suspension for %d",
      scid);
    sql_execute("DELETE FROM chanserv_suspensions WHERE scid=%d", scid);
    sql_execute("UPDATE chanserv SET flags = (flags & ~%d)"
      " WHERE scid=%d", CFL_SUSPENDED, scid);
/*
    sql_execute("UPDATE chanserv SET flags = (flags & ~%d), t_expire=%d"
      " WHERE snid=%d", CFL_SUSPENDED, irc_CurrentTime+ExpireTime, snid);
*/
  }
  sql_free(res);    
  return 0;
}

void ev_cs_new_server(IRC_Server* nserver, IRC_Server *from)
{
  static int already_loaded = 0;
  MYSQL_RES* res;
  MYSQL_ROW row;
  
  if(already_loaded)
    return;

  res = sql_query("SELECT name FROM chanserv");
  
  while((row = sql_next_row(res)))
    {
      IRC_Chan *chan = irc_FindChan(row[0]);
      if(chan)
        continue;
      chan = irc_ChanJoin(csu.u, row[0], 0);
      irc_ChanMode(csu.u, chan, "+r");
      irc_ChanPart (csu.u, chan);
    }
    
  sql_free(res);
  already_loaded = -1;   
}

/* this version takes care of sql upgrades */
int sql_upgrade(int version, int post)
{
  MYSQL_RES* res;
  MYSQL_ROW row;
  
  switch(version)
  {
    case 3: /* pre-validation of integrity rules */
    if(!post) /* we need to check for "lost" memos */
    {
      int rowc = 0;
      res = sql_query("SELECT"
        " chanserv.successor, chanserv.name, chanserv.scid FROM chanserv"
        " LEFT JOIN nickserv ON (chanserv.successor = nickserv.snid)"
        " WHERE chanserv.successor IS NOT NULL AND nickserv.snid IS NULL");
      while((row = sql_next_row(res)))
      {
        if(atoi(row[0]) == 0) /* this will be set to null */
          continue;
        log_log(cs_log, mod_info.name, "Removing lost successor %s on %s",
          row[0], row[1]);
        sql_execute("UPDATE chanserv SET successor=NULL WHERE scid=%s",
          row[2]);
        ++rowc;
      }
      if(rowc)
        log_log(cs_log, mod_info.name, "Removed %d lost successor(s)", rowc);
      sql_free(res);
      rowc = 0;
      res = sql_query("SELECT"
        " chanserv.founder, chanserv.name, chanserv.successor,"
        " chanserv.scid FROM chanserv"
        " LEFT JOIN nickserv ON (chanserv.founder = nickserv.snid)"
        " WHERE chanserv.successor IS NOT NULL AND nickserv.snid IS NULL");
      while((row = sql_next_row(res)))
      {
        if(atoi(row[0]) == 0) /* this will be set to null */
          continue;
        if(row[2] && atoi(row[2]) != 0)
        {
          log_log(cs_log, mod_info.name, 
            "Transfering lost channel %s to %s", row[1], row[2]);
          sql_execute("UPDATE chanserv SET founder=%s, successor=NULL"
            " WHERE scid=%s", row[2], row[3]);
        }
        else
        {
          log_log(cs_log, mod_info.name, 
            "Deleting lost channel %s, founder was %s", row[3], row[0]);
          sql_execute("DELETE FROM chanserv WHERE scid=%s",
            row[3]);
          ++rowc;
        }
      }
      if(rowc)
        log_log(cs_log, mod_info.name, "Deleted %d lost channels(s)", rowc);
      sql_free(res);            
    }
  }    
  return 1;
}

