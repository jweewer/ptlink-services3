/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2006 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************

  Description: module os_uevent

 *  $Id: os_uevent.c,v 1.9 2005/12/11 16:51:17 jpinto Exp $
*/

#include "module.h"
#include "dbconf.h"
#include "my_sql.h"
#include "ns_group.h"
#include "nsmacros.h"
#include "lang/os_uevent.lh"
#include "lang/common.lh"


SVS_Module mod_info =
 /* module, version, description */
{"os_uevent", "1.1",  "session limit module" };

/* Change Log
  1.1 - #71, UEVENT GLINE using wrong gline author
        #70, UEVENT KILL action
  1.0 - Initial version
*/

#define DB_VERSION	1

/** functions/events we require **/
ServiceUser* (*operserv_suser)(void);
static int e_expire = -1;
static int e_nick_identify = -1;
static int e_nick_register = -1;

MOD_REQUIRES
  DBCONF_FUNCTIONS
  MOD_FUNC(operserv_suser)
  MOD_FUNC(is_sadmin)
  MOD_FUNC(e_nick_identify)
  MOD_FUNC(e_nick_register)
  MOD_FUNC(e_expire) /* we need this to run the expire routines */  
MOD_END

/* internal data */
static DBMem* dbm_uevents;

typedef struct EventType_s EventType;
struct EventType_s
{
  char *str;
  int e_type;
};

enum 
{
  ON_CONNECT = 0,
  ON_JOIN,
  ON_LOGIN,
  ON_REGISTER
};

#define EVENT_NUM 4

const static EventType event_types[] = 
{
  { "ON_CONNECT" , ON_CONNECT},
  { "ON_JOIN", ON_JOIN},
  { "ON_LOGIN", ON_LOGIN},
  { "ON_REGISTER", ON_REGISTER},
  { NULL }
};

/* 
 * We use dbmem to make it simple to keep the events on mem/db
 * The event index was added to avoid a full scan of the event table
 *  - Lamego
 */
static u_int32_t event_index[EVENT_NUM][1024];
static int event_index_size[EVENT_NUM];

typedef struct ActionType_s ActionType;
struct ActionType_s
{
  char *str;
  int a_type;
  ActionHandler action;
};


#define A_MESSAGE	0
#define A_NOTICE	1
#define A_GLINE		2
#define A_UMODE		3
#define A_JOIN		4
#define A_KILL		5


static int a_message(IRC_User *u, char *param);
static int a_notice(IRC_User *u, char *param);
static int a_gline(IRC_User *u, char *param);
static int a_umode(IRC_User *u, char *param);
static int a_join(IRC_User *u, char *param);
static int a_kill(IRC_User *u, char *param);

const static ActionType action_types[] =
{
  { "MESSAGE", A_MESSAGE , (ActionHandler) a_message},
  { "NOTICE", A_NOTICE, (ActionHandler) a_notice },
  { "GLINE", A_GLINE, (ActionHandler) a_gline },
  { "UMODE", A_UMODE, (ActionHandler) a_umode },
  { "JOIN", A_UMODE, (ActionHandler) a_join },
  { "KILL", A_UMODE, (ActionHandler) a_kill },
  { NULL },
};

/* internal functions */
static int ev_uevents_expire(void* dummy1, void* dummy2);
static int find_event_type(char* event);
static int find_action_type(char* action);
static void ev_uevents_new_user(IRC_User* u, void *s);
static void ev_uevents_chan_join(IRC_Chan *chan , IRC_ChanNode *cnode);
static void build_event_index();

/* Local variables */
static ServiceUser *osu;
static int os_log;

/* Local functions  - commands */
static void os_uevent(IRC_User *s, IRC_User *u);
static void os_uevent_add(IRC_User *s, IRC_User *u);
static void os_uevent_del(IRC_User *s, IRC_User *u);
static void os_uevent_list(IRC_User *s, IRC_User *u);

/* Local functions - utility */
static void ev_os_uevent_nick_identify(IRC_User* u, u_int32_t *snid);
static void ev_os_uevent_nick_register(IRC_User *u, u_int32_t *snid);

/** load code **/
int mod_load(void)
{
  int r;
  /* initialize dbmem for uevents */
  dbm_uevents = dbmem_init("user_events", 128);
  r = sql_check_inst_upgrade(mod_info.name, DB_VERSION, NULL);
  
  if(r < 0)
    return -1;

  /* try to load existing exceptions */
  if(dbmem_load(dbm_uevents) < 0 )
    return -3;
    
  build_event_index();
  
  os_log = log_handle("operserv");
  osu = operserv_suser();  
  
  suser_add_cmd(osu, "UEVENT", os_uevent, 
    OS_UEVENT_SUMMARY, OS_UEVENT_HELP);

  /* Add help */
  suser_add_help(osu, "UEVENT EVENTS", OS_UEVENT_EVENT_LIST);
  suser_add_help(osu, "UEVENT ACTIONS",OS_UEVENT_ACTION_LIST);
  
  /* Add user events */
  irc_AddEvent(ET_NEW_USER, ev_uevents_new_user); /* new user */
  irc_AddEvent(ET_CHAN_JOIN, ev_uevents_chan_join);

  /* Add event actions */ 
  mod_add_event_action(e_expire, 
    (ActionHandler) ev_uevents_expire);
  mod_add_event_action(e_nick_identify, 
    (ActionHandler) ev_os_uevent_nick_identify);
  mod_add_event_action(e_nick_register,
    (ActionHandler) ev_os_uevent_nick_register);  
  return 0;
}

void mod_unload(void)
{
  irc_DelEvent(ET_NEW_USER, ev_uevents_new_user);
  irc_DelEvent(ET_CHAN_JOIN, ev_uevents_chan_join);
  mod_del_event_action(e_expire, (ActionHandler) ev_uevents_expire);
  mod_del_event_action(e_nick_identify, (ActionHandler) ev_os_uevent_nick_identify);
  mod_del_event_action(e_nick_register, (ActionHandler) ev_os_uevent_nick_register);  
  dbmem_free(dbm_uevents);
  suser_del_mod_cmds(osu, &mod_info);
}

void os_uevent(IRC_User *s, IRC_User *u)
{
  char* cmd;
  u_int32_t source_snid;

  CHECK_IF_IDENTIFIED_NICK

  if (!is_sadmin(u->snid))
  {
    send_lang(u, s, PERMISSION_DENIED);
    return;
  }

  cmd = strtok(NULL, " ");
  /* check syntax */
  if(!cmd)
    send_lang(u, s, OS_UEVENT_SYNTAX);
  else
  if(!strcasecmp(cmd, "ADD"))
    os_uevent_add(s, u);    
  else
  if(!strcasecmp(cmd, "DEL"))
    os_uevent_del(s, u);
  else  
  if(!strcasecmp(cmd, "LIST"))
    os_uevent_list(s, u);
  else
    send_lang(u, s, OS_UEVENT_SYNTAX);    
}

void os_uevent_add(IRC_User *s, IRC_User *u)
{
  char *event, *event_parm = NULL;
  char* time_string = "";  
  int duration = 0;  
  char *action, *action_parm = NULL;
  int event_i, action_i;
  
  event  = strtok(NULL, " ");
  if(event && event[0]=='+')
  {
    time_string = event;
    duration = ftime_str(event);
    event  = strtok(NULL, " ");
  }
  if(event)
  {
    char* e = strchr(event, '(');
    if(e)
    {
      *e = '\0';
      event_parm = e+1;
      e = strchr(event_parm, ')');
      if(e)
       *e = '\0';
    }
  }
  
  action = strtok(NULL, " ");
  action_parm = strtok(NULL, "");
  if(!event || ! action || !action_parm)
    send_lang(u, s, OS_UEVENT_ADD_INVALID_SYNTAX);
  else
  /* check if time is valid */
  if(duration == -1)
    send_lang(u, s, INVALID_TIME_X, time_string);
  else
  if((event_i = find_event_type(event)) == -1)
    send_lang(u, s, OS_UEVENT_INVALID_EVENT_X, event);
  else
  if((action_i = find_action_type(action)) == -1)
    send_lang(u, s, OS_UEVENT_INVALID_ACTION_X, action);
  else
  {
    int i = 0;
    char* d_row[7];
    d_row[i++] = itoa(0); /* id */
    d_row[i++] = itoa(irc_CurrentTime); /* t_when */
    d_row[i++] = itoa(duration);       /* duration */
    d_row[i++] = itoa(event_i);
    d_row[i++] = event_parm;
    d_row[i++] = itoa(action_i);
    d_row[i++] = action_parm;
    if(dbmem_insert(dbm_uevents, d_row) < 0)
      send_lang(u, s, UPDATE_FAIL);
    else
    {
      send_lang(u, s, OS_UEVENT_ADD_OK);    
      build_event_index();
    }
  }
}

/* OS UEVENT DEL command */
void os_uevent_del(IRC_User *s, IRC_User *u)
{
  char* str;
  u_int32_t id = 0;
  str = strtok(NULL, " ");
  if(str)
    id = atoi(str);
  else
  {
    send_lang(u, s, OS_UEVENT_DEL_SYNTAX);
    return;
  }
  if(dbmem_find_exact(dbm_uevents, itoa(id), 0) == NULL)
    send_lang(u, s, OS_UEVENT_DEL_NOT_FOUND_X, id);
  else
  if(dbmem_delete_current(dbm_uevents) < 0)
    send_lang(u, s, UPDATE_FAIL);
  else
  {
    send_lang(u, s, OS_UEVENT_DEL_DELETED_X, id);
    build_event_index();
  }
}

void os_uevent_list(IRC_User *s, IRC_User *u)
{
  char **row;
  ev_uevents_expire(NULL, NULL);
  send_lang(u, s, OS_UEVENT_LIST_HEADER);
  row = dbmem_first_row(dbm_uevents);
  while(row)
  {
    char* event = event_types[atoi(row[3])].str;
    char* action = action_types[atoi(row[5])].str;
    send_lang(u, s, OS_UEVENT_LIST_X_X_X_X,
      atoi(row[0]), event, row[4] ? row[4] : "", action, row[6]);
    row = dbmem_next_row(dbm_uevents);
  };
  send_lang(u, s, OS_UEVENT_LIST_TAIL);  
}

static void build_event_index()
{
  char **row;
  int e_type;
  int i;
  row = dbmem_first_row(dbm_uevents);
  for(i = 0; i < EVENT_NUM; ++i)
    event_index_size[i] = 0;
    
  i = 0;  
  while(row)
  {
    e_type = atoi(row[3]);
    if(event_index_size[e_type] == 1023)
    {
      errlog("Exceeded hash capacity on build_event_index() !");
      return;
    }
    event_index[e_type][event_index_size[e_type]++] = i++;
    row = dbmem_next_row(dbm_uevents);
  }
}
/* ev_exceptions_expire - deleted expired exceptions
 *  returns:
 *	Number of imported rows, <0 for error
 */
int ev_uevents_expire(void* dummy1, void* dummy2)
{
  dbmem_expire(dbm_uevents, 1, 2);
  build_event_index();
  return 0;
}

static int a_message(IRC_User *u, char *param)
{
  irc_SendMsg(u, osu->u, "%s", param);
  return 0;
}

static int a_notice(IRC_User *u, char *param)
{
  irc_SendNotice(u, osu->u, "%s", param);
  return 0;
}

static int a_gline(IRC_User *u, char *param)
{
  irc_Gline(osu->u, osu->u->nick, u->realhost, 24*3600,
          param ? param : "");
  return 0;
}

static int a_umode(IRC_User *u, char *param)
{
  irc_SvsMode(u, osu->u, param);
  return 0;
}

static int a_join(IRC_User *u, char *param)
{
  irc_SvsJoin(u, osu->u, param);
  return 0;
}

static int a_kill(IRC_User *u, char *param)
{
  irc_Kill(u, osu->u, param ? param : "");
  irc_AbortThisEvent();
  return 0;
}

static int find_event_type(char* event)
{
 int i = 0;
 while(event_types[i].str)
 {
   if(strcasecmp(event_types[i].str, event) == 0)
     return i;
   i++;
 }
 return -1;
}

static int find_action_type(char* action)
{
 int i = 0;
 while(action_types[i].str)
 {
   if(strcasecmp(action_types[i].str, action) == 0)
     return i;
   i++;
 }
 return -1;
}

/* a new user is connecting, we need to trigger the ON_CONNECT events  */
static void ev_uevents_new_user(IRC_User* u, void *s)
{
  char **row;
  char* mask = irc_UserMask(u);
  char* maskp = irc_UserMaskP(u);
  int i;
  for(i = 0; i < event_index_size[ON_CONNECT]; ++i)
  {
    row = dbmem_row_at(dbm_uevents, event_index[ON_CONNECT][i]);
    if((row[4] == NULL) || (row[4] && 
      (match(row[4], mask) || match(row[4], maskp))))
      {
        /* do the action here */
        action_types[atoi(row[5])].action(u, row[6]);
      }
  }
}

static void ev_uevents_chan_join(IRC_Chan *chan , IRC_ChanNode *cnode)
{
  char **row;
  int i;
  for(i = 0; i < event_index_size[ON_JOIN]; ++i)
  {
    row = dbmem_row_at(dbm_uevents, event_index[ON_JOIN][i]);
    if((row[4] == NULL) || (row[4] && match(row[4], chan->name)))
      {
        /* do the action here */
        action_types[atoi(row[5])].action(cnode->user, row[6]);
      }
  }
}

static void ev_os_uevent_nick_identify(IRC_User* u, u_int32_t *snid)
{
  char **row;
  char* mask = irc_UserMask(u);
  char* maskp = irc_UserMaskP(u);
  int i;
  for(i = 0; i < event_index_size[ON_LOGIN]; ++i)
  {
    row = dbmem_row_at(dbm_uevents, event_index[ON_LOGIN][i]);
    if((row[4] == NULL) || (row[4] && 
      (match(row[4], mask) || match(row[4], maskp))))
      {
        /* do the action here */
        action_types[atoi(row[5])].action(u, row[6]);
      }
  }
}

static void ev_os_uevent_nick_register(IRC_User *u, u_int32_t *snid)
{
  char **row;
  char* mask = irc_UserMask(u);
  char* maskp = irc_UserMaskP(u);
  int i;
  for(i = 0; i < event_index_size[ON_REGISTER]; ++i)
  {
    row = dbmem_row_at(dbm_uevents, event_index[ON_REGISTER][i]);
    if((row[4] == NULL) || (row[4] && 
      (match(row[4], mask) || match(row[4], maskp))))
      {
        /* do the action here */
        action_types[atoi(row[5])].action(u, row[6]);
      }
  }
}
