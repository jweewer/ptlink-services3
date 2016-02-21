/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2006 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************

  Description: module sessionlimit

*/

#include "module.h"
#include "dbconf.h"
#include "my_sql.h"
#include "ns_group.h"
#include "nsmacros.h"
#include "lang/sessionlimit.lh"
#include "lang/common.lh"


SVS_Module mod_info =
 /* module, version, description */
{"sessionlimit", "1.2",  "session limit module" };

/* Change Log 
  1.2 - #68: OS EXCEPTION CHANGE for exception owners
        #67: OS EXCEPTION VIEW displaying connections in use
  1.1 - Added missing is_soper() import on the sessionlimit module
        Set +B on users connecting with exception
  1.0 - #8: os_session with better session handling
*/
#define DB_VERSION	1

static int DefaultMaxUsers;
static int MaxHits;
static int GlineTime;

DBCONF_PROVIDES
  DBCONF_INT(DefaultMaxUsers,"3", 
    "Default allowed max. number of connections per host")
  DBCONF_INT(MaxHits, "5",
    "How many times times users from a host will be killed before getting glined")
  DBCONF_TIME(GlineTime, "5h", 
    "Time to be used on the session glined when MaxHits is reached")
DBCONF_END
 
/** functions/events we require **/
ServiceUser* (*operserv_suser)(void);
static int e_expire = 0;

MOD_REQUIRES
  DBCONF_FUNCTIONS
  MOD_FUNC(operserv_suser)
  MOD_FUNC(is_soper)
  MOD_FUNC(is_sadmin)
  MOD_FUNC(e_expire) /* we need this to run the expire routines */
MOD_END

/* host record structure */
typedef struct HostRecord_s HostRecord;
struct HostRecord_s
{
  char host[HOSTLEN];
  int count;
  int limit;
  int limit_hits;
  HostRecord *hnext;
};

/* session hosts hash size */
#define HOST_HSIZE 16384

/* internal data */
static HostRecord *sessionTable[HOST_HSIZE];
static DBMem* dbm_exceptions;

/* internal functions */
static void os_session(IRC_User *s, IRC_User *u);
static void os_exception(IRC_User *s, IRC_User *u);
static int ev_sessionlimit_new_user(IRC_User* u, void *s);
static int ev_sessionlimit_quit(IRC_User* u, char *lastquit);
static int ev_sessionlimit_kill(IRC_User* u, IRC_User *killer);
static HostRecord* add_to_session(char *host);
static void del_from_session(char *host);
static HostRecord* find_session(char *host);
static unsigned int hash_host(const char* host);
static int ev_exceptions_expire(void* dummy1, void* dummy2);

/* Local variables */
static ServiceUser *osu;
static int os_log;

/* Local functions  - commands */
static void os_session_list(IRC_User *s, IRC_User *u);
static void os_session_view(IRC_User *s, IRC_User *u);
static void os_exception_add(IRC_User *s, IRC_User *u);
static void os_exception_del(IRC_User *s, IRC_User *u);
static void os_exception_list(IRC_User *s, IRC_User *u);
static void os_exception_change(IRC_User *s, IRC_User *u);

/* Local functions - utility */
static int import_bot_hostrules(void);

/** rehash code **/
int mod_rehash(void)
{
  if(dbconf_get_or_build(mod_info.name, dbconf_provides) < 0 )
  {
    errlog("Error reading dbconf!");
    return -1;
  }
  return 0;
}

/** load code **/
int mod_load(void)
{
  int r;
  /* initialize dbmem for exceptions */
  dbm_exceptions = dbmem_init("session_exceptions", 128);
  r = sql_check_inst_upgrade(mod_info.name, DB_VERSION, NULL);
  
  if(r < 0)
    return -1;

  /* try to load existing exceptions */
  if(dbmem_load(dbm_exceptions) < 0 )
    return -3;
    
  /* module is being installed, see if we have hostrules to import */
  if((r == 1) && sql_find_module("os_hostrule"))
  {
    if(import_bot_hostrules() < 0)
      return -2;
  }

  os_log = log_handle("operserv");
  osu = operserv_suser();  
  
  suser_add_cmd(osu, "SESSION", os_session, 
    OS_SESSION_SUMMARY, OS_SESSION_HELP);
    
  suser_add_cmd(osu, "EXCEPTION", os_exception, 
    OS_EXCEPTION_SUMMARY, OS_EXCEPTION_HELP);    
    
  /* Add user events */
  irc_AddEvent(ET_NEW_USER, ev_sessionlimit_new_user); /* new user */
  irc_AddEvent(ET_QUIT, ev_sessionlimit_quit); /* user quit */
  irc_AddEvent(ET_KILL, ev_sessionlimit_kill); /* user kill */
  
  mod_add_event_action(e_expire, (ActionHandler) ev_exceptions_expire);
  return 0;
}

void mod_unload(void)
{
  irc_DelEvent(ET_NEW_USER, ev_sessionlimit_new_user);
  irc_DelEvent(ET_QUIT, ev_sessionlimit_quit);
  irc_DelEvent(ET_KILL, ev_sessionlimit_kill);
  mod_del_event_action(e_expire, (ActionHandler) ev_exceptions_expire);
  dbmem_free(dbm_exceptions);
}
        
void os_session(IRC_User *s, IRC_User *u)
{
  char* cmd;
  u_int32_t source_snid;

  CHECK_IF_IDENTIFIED_NICK

  if (!is_soper(u->snid))
  {
    send_lang(u, s, PERMISSION_DENIED);
    return;
  }

  cmd = strtok(NULL, " ");
  /* check syntax */
  if(!cmd)
    send_lang(u, s, OS_SESSION_SYNTAX);
  else
  /* list command */
  if(!strcasecmp(cmd, "LIST"))
    os_session_list(s, u);
  else
  if(!strcasecmp(cmd, "VIEW"))
    os_session_view(s, u);
  else
    send_lang(u, s, OS_SESSION_SYNTAX);
    
}

/* list sessions with connections above a given number */
void os_session_list(IRC_User *s, IRC_User *u)
{
  char *nstr = strtok(NULL, " ");
  if(!nstr || !isdigit(*nstr))
    send_lang(u, s, OS_SESSION_LIST_SYNTAX);
  else
  {
    int hashv;
    HostRecord *hr;
    int num = atoi(nstr);
    send_lang(u, s, OS_SESSION_LIST_HEADER_X, num);
    for(hashv = 0; hashv < HOST_HSIZE; ++hashv)
    {
      hr = sessionTable[hashv]; 
      while(hr)
      {
        if(hr->count > num)
        {
          if(hr->limit_hits)
            send_lang(u, s, OS_SESSION_LIST_ITEM_X_X_X_X, 
              hr->host, hr->count, hr->limit, hr->limit_hits);
          else
            send_lang(u, s, OS_SESSION_LIST_ITEM_X_X_X, 
              hr->host, hr->count, hr->limit);
        }
        hr = hr ->hnext;
      }
    }
    send_lang(u, s, OS_SESSION_LIST_TAIL);
  }
}

void os_session_view(IRC_User *s, IRC_User *u)
{
  char *host = strtok(NULL, " ");
  HostRecord *hr;
  if(host == NULL)
    send_lang(u, s, OS_SESSION_VIEW_SYNTAX);
  else
  if((hr = find_session(host)) == NULL)
    send_lang(u, s, OS_SESSION_VIEW_X_NOT_FOUND, host);
  else
  {
    IRC_UserList gl;
    IRC_User* list_u = irc_GetGlobalList(&gl);
    send_lang(u, s, OS_SESSION_VIEW_HEADER_X, hr->host);
    while(list_u)
    {
      if(strcmp(hr->host, list_u->realhost) == 0) /* we have a match*/
      	send_lang(u, s, OS_SESSION_VIEW_ITEM_X_X_X,
      	  list_u->nick, irc_UserMaskP(list_u), list_u->info);
      list_u = irc_GetNextUser(&gl);
    }
    send_lang(u, s, OS_SESSION_VIEW_TAIL);
  }
}



/* handle a new user event */
int ev_sessionlimit_new_user(IRC_User* u, void *s)
{
  HostRecord *hrec;

  /* check for SessionLimit */  
  hrec = add_to_session(u->realhost);
  if(hrec->limit > DefaultMaxUsers)
    irc_SvsMode(u, osu->u, "+B");
  /* we reached session limit for this host */
  if(hrec->limit && (hrec->count > hrec->limit)) 
  {
    if(!MaxHits || ((++hrec->limit_hits) < MaxHits))
    {
      log_log(os_log, mod_info.name, 
        "Killing %s , exceeded connections limit %d/%d",
      irc_UserMask(u), hrec->count, hrec->limit);
      irc_Kill(u, osu->u, 
        "Exceeded maximum number of connections for this host");  
      /* user was deleted, we need to abort the event*/        
      irc_AbortThisEvent(); 
    }
    else
    {
      char gmask[HOSTLEN+3];        
      snprintf(gmask, HOSTLEN+3, "*@%s", u->realhost);                  
      log_log(os_log, mod_info.name, 
        "Glining  %s, exceeded connections limit %d/%d",
        gmask, hrec->count, hrec->limit);            
      irc_Gline(osu->u, osu->u->nick, gmask, GlineTime,
        "Exceeded maximum number of connections for this host");
    }
  }
  return 0;
}

int ev_sessionlimit_quit(IRC_User* u, char *lastquit)
{
  del_from_session(u->realhost);
  return 0;
}

int ev_sessionlimit_kill(IRC_User* u, IRC_User *killer)
{
  del_from_session(u->realhost);
  return 0;
}  

/* returns a session in memory */
HostRecord* find_session(char *host)
{
  HostRecord* hr;
  u_int32_t hashv;

  hashv = hash_host(host);
  hr = sessionTable[hashv];
  while(hr && strcasecmp(hr->host, host) != 0)
      hr = hr->hnext;

  return hr;
}
                  
/* adds a session to the hash table */
HostRecord* add_to_session(char *host)
{
  HostRecord* hr;
  u_int32_t hashv;

  hr = find_session(host); /* check if its already in mem */
  if(IsNull(hr))
    {
      char **row;
      row = dbmem_find_exact(dbm_exceptions, host, 0);

      hr = malloc(sizeof(HostRecord));
      bzero(hr,sizeof(HostRecord));    
      if(row)
        hr->limit = atoi(row[5]);
      else
        hr->limit = DefaultMaxUsers;
      hashv = hash_host(host);
      hr->hnext = sessionTable[hashv];
      strncpy(hr->host, host, HOSTLEN);
      sessionTable[hashv] = hr;
    }
  hr->count++;
  return hr;
}

void del_from_session(char *host)
{
  HostRecord *hr, *prev;
  int hashv;
  
  hashv = hash_host(host);
  hr = sessionTable[hashv];
  prev = NULL;
  
  if(IsNull(hr))
    return;
    
  while(hr && strcasecmp(hr->host, host) != 0)
    {
      prev = hr;    
      hr = hr->hnext;
    }
        
  if(IsNull(hr))
    return; /* not found */
    
  if(--(hr->count)==0) /* zero count, remove record from mem */
    {
      if(prev)
        prev->hnext= hr->hnext;
      else
        sessionTable[hashv] = hr->hnext;
      free(hr);
    }
}

unsigned int hash_host(const char* host)
{
  unsigned int h = 0;
                                                                                
  while (*host)
    {
      h = (h << 4) - (h + (unsigned char)ToLower(*host++));
    }
                                                                                
  return(h & (HOST_HSIZE - 1));
}

void os_exception(IRC_User *s, IRC_User *u)
{

  char *cmd = strtok(NULL, " ");
  
  /* check syntax */
  if(!cmd)
    send_lang(u, s, OS_EXCEPTION_SYNTAX);
  else
  /* list command */
  if(!strcasecmp(cmd, "ADD"))
    os_exception_add(s, u);
  else
  if(!strcasecmp(cmd, "DEL"))
    os_exception_del(s, u);
  else  
  if(!strcasecmp(cmd, "LIST"))
    os_exception_list(s, u);
  else
  if(!strcasecmp(cmd, "CHANGE"))
    os_exception_change(s, u);  
  else
    send_lang(u, s, OS_EXCEPTION_SYNTAX);
    
}

static void os_exception_add(IRC_User *s, IRC_User *u)
{
  u_int32_t owner_snid;
  char **row;
  char *owner = strtok(NULL, " ");
  char *ex_time = strtok(NULL, " ");
  char *mask = strtok(NULL, " ");
  char *limit = strtok(NULL, " ");
  char *reason = strtok(NULL, "");  

  /* check for required privilege */
  if(!is_sadmin(u->snid))
    send_lang(u, s, PERMISSION_DENIED);
  else  
  if(!owner || !ex_time | !mask | !limit || !reason || (limit && atoi(limit)<1))
    send_lang(u, s, OS_EXCEPTION_SYNTAX);
  else
  /* check if hostname is valid */
  if(!irc_IsValidHostname(mask))
    send_lang(u, s, OS_EXCEPTION_INVALID_HOST_X, mask);
  else
  /* check if time is valid */
  if(ftime_str(ex_time) == -1)
    send_lang(u, s, INVALID_TIME_X, ex_time);
  else
  /* check if the owner exists */
  if((owner_snid = nick2snid(owner)) == 0)
    send_lang(u, s, NICK_X_NOT_REGISTERED, owner);
  else
  /* check if exception already exists */
  if((row = dbmem_find_exact(dbm_exceptions, mask, 0)))
    send_lang(u, s, OS_EXCEPTION_ADD_ALREADY_EXISTS_X, mask);
  else
  {
    char* d_row[7];
    int i = 0;
    d_row[i++] = mask; /* hostmask */
    d_row[i++] = itoa(u->snid); /* who_snid */
    d_row[i++] = itoa(owner_snid); /* owner_snid */
    d_row[i++] = itoa(irc_CurrentTime); /* t_when */
    d_row[i++] = itoa(ftime_str(ex_time)); 	 /* duration */
    d_row[i++] = itoa(atoi(limit)); /* limit */
    d_row[i++] = reason; /* reason */
    if(dbmem_insert(dbm_exceptions, d_row)<0)
      send_lang(u, s, UPDATE_FAIL);
    else
    {
      HostRecord *hr = find_session(mask);
      send_lang(u, s, OS_EXCEPTION_ADD_OK_X, mask);
      if(hr)
        hr->limit = atoi(limit);
    }
  }
}

static void os_exception_del(IRC_User *s, IRC_User *u)
{

  char *mask = strtok(NULL, " ");

  /* check for required privilege */
  if(!is_sadmin(u->snid))
    send_lang(u, s, PERMISSION_DENIED);
  else  
  if(!mask)
    send_lang(u, s, OS_EXCEPTION_SYNTAX);
  else
  /* check if exception already exists */
  if(dbmem_find_exact(dbm_exceptions, mask, 0) == NULL)
    send_lang(u, s, OS_EXCEPTION_NOT_FOUND_X, mask);
  else
  if(dbmem_delete_current(dbm_exceptions) < 0)
      send_lang(u, s, UPDATE_FAIL);
  else
  {
    HostRecord *hr = find_session(mask);
    send_lang(u, s, OS_EXCEPTION_DELETED_X, mask);
    if(hr)
      hr->limit = DefaultMaxUsers;
  }      
}

static void os_exception_list(IRC_User *s, IRC_User *u)
{
  char **row;
  int privilege = is_sadmin(u->snid);
  /* we run the expire before the list, to make a clean list */
  ev_exceptions_expire(NULL, NULL);
  send_lang(u, s, OS_EXCEPTION_LIST_HEADER);
  row = dbmem_first_row(dbm_exceptions);
  while(row)
  {
    if(privilege || (u->snid == atoi(row[2])))
    {
      HostRecord* hr = find_session(row[0]);
      if(hr)
        send_lang(u, s, OS_EXCEPTION_LIST_X_X_X_X,
    	  row[0], row[5], hr->count, row[6]);
      else
        send_lang(u, s, OS_EXCEPTION_LIST_X_X_X,
    	  row[0], row[5], row[6]);
    }
    row = dbmem_next_row(dbm_exceptions);
  };
  send_lang(u, s, OS_EXCEPTION_LIST_TAIL);
}

static void os_exception_change(IRC_User *s, IRC_User *u)
{

  int privilege = is_sadmin(u->snid);
  char *old_host = strtok(NULL, " ");
  char *new_host = strtok(NULL, " ");

  /* check for required privilege */
  if(!old_host || !new_host)
    send_lang(u, s, OS_EXCEPTION_CHANGE_SYNTAX);
  else 
  /* check if hostname is valid */
  if(!irc_IsValidHostname(new_host))
    send_lang(u, s, OS_EXCEPTION_INVALID_HOST_X, new_host);
  else
  /* check if exception already exists */
  if(dbmem_find_exact(dbm_exceptions, new_host, 0))
    send_lang(u, s, OS_EXCEPTION_ALREADY_X_EXISTS, new_host);
  else
  if(dbmem_find_exact(dbm_exceptions, old_host, 0) == NULL)
    send_lang(u, s, OS_EXCEPTION_NOT_FOUND_X, old_host);
  else
  {
    char** row = dbmem_current_row(dbm_exceptions);
    if(!privilege && (u->snid != atoi(row[2])))
      send_lang(u, s, OS_EXCEPTION_CHANGE_DENIED, old_host);
    else
    {
      if(dbmem_replace_key(dbm_exceptions, new_host) > 0)
      {
        log_log(os_log, mod_info.name, "%s CHANGED exception host from %s to %s",
          u->nick, old_host, new_host); 
        HostRecord *old_hr = find_session(old_host);
        HostRecord *new_hr = find_session(new_host);

        if(old_hr)
          old_hr->limit = DefaultMaxUsers;
        if(new_hr)
          new_hr->limit = atoi(row[5]);
        if(dbmem_find_exact(dbm_exceptions, old_host, 0))
          dbmem_delete_current(dbm_exceptions);
        send_lang(u, s, OS_EXCEPTION_CHANGED);          
        irc_SendSanotice(s, "%s CHANGED exception host from %s to %s",
          u->nick, old_host, new_host);
      }
      else
        send_lang(u, s, UPDATE_FAIL);
    }
  }
}
/* import_bot_hostrules - import bot type hostules from hostrule table 
 *  returns:
 *	Number of imported rows, <0 for error
 */
static int import_bot_hostrules(void)
{
  MYSQL_RES *res;
  MYSQL_ROW row;
  char* d_row[7];
  int count = 0;
  res = sql_query("SELECT host, UNIX_TIMESTAMP(t_create), param, message FROM os_hostrule "
  	"WHERE rtype=512");
  if(!res)
    return -1;
  while((row = sql_next_row(res)))
  {
    int i = 0;
    d_row[i++] = row[0]; /* hostmask */
    d_row[i++] = NULL; /* who_snid */
    d_row[i++] = NULL; /* owner_snid */
    d_row[i++] = row[1]; /* t_when */
    d_row[i++] = "0"; 	 /* duration */
    d_row[i++] = row[2]; /* limit */
    d_row[i++] = row[3]; /* reason */
    if(dbmem_insert(dbm_exceptions, d_row) <0 )
    {
      sql_free(res);
      return -2;
    }
    ++count;
 }
 sql_free(res); 
 /* delete imported hostrules */
 sql_execute("DELETE FROM os_hostrule WHERE rtype=512");
  
 return count;
}

/* ev_exceptions_expire - deleted expired exceptions
 *  returns:
 *	Number of imported rows, <0 for error
 */
int ev_exceptions_expire(void* dummy1, void* dummy2)
{
  dbmem_expire(dbm_exceptions, 3, 4);
  return 0;
}
