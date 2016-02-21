/*****************************************************************
 * libircservice is (C) CopyRight PTlink IRC Software 1999-2005  *
 *                    http://software.pt-link.net                *
 * This program is distributed under GNU Public License          *
 * Please read the file COPYING for copyright information.       *
 *****************************************************************

  Description: ircservice header file

 *  $Id: ircservice.h,v 1.7 2005/10/17 16:57:50 jpinto Exp $
*/

/** @file 
 libircservice include file.
 core libircservices
*/
#include "stdinc.h"

#ifndef _IRCSERVICE_H_
#define _IRCSERVICE_H_

/** library version */
#define	SVSLIBVER	"0.5"


/**  maximum nick lenght */
#define NICKLEN		20

/**  maximum hostname lenght */
#define HOSTLEN         63

/**  maximum username lenght */
#define USERLEN         10

/**  maximum realname lenght */
#define REALLEN         50

/**  maximum channel name lenght */
#define CHANNELLEN	32

/*
 * pre declare structs
 */
struct IRC_UserList;
struct IRC_ChanList;

typedef struct IrcMsg_s IrcMsg;
typedef struct IRC_User_s IRC_User;
typedef struct IRC_User_s IRC_Server;
typedef struct IRC_Chan_s IRC_Chan;
typedef struct IRC_ChanNode_s IRC_ChanNode;
typedef struct IRC_UserNode_s IRC_UserNode;
typedef struct IRC_BanList_s IRC_BanList;

typedef void (*RawHandler)(int, char*[]);
typedef void (*MsgHandler)(void *dest, void *src);
typedef void (*EventHandler)(void *data1, void *data2);


/**  User data */
struct IRC_User_s
{
  /** nick snid */
  u_int32_t snid;
  u_int32_t flags;  
  u_int32_t status;
  u_int32_t iflags; /* internal flags */
  char nick[NICKLEN + 1];
  /** nick before nick change */
  char lastnick[NICKLEN + 1];
  char username[USERLEN + 1];
  char realhost[HOSTLEN + 1];
  char publichost[HOSTLEN + 1];  
/*  char vhost[HOSTLEN + 1]; */
  /** user realname/info/gecos */
  char info[REALLEN + 1];
  int hopcount;
  /** user signon time stampe */
  int ts;
  /** user modes */
  int umodes;

  /** list of channels the user is joining */
  IRC_UserNode* chanlist;
  /** server on which the user is on */
  IRC_Server* server;
  IRC_Server* from;
  /** below are for lists/hash structures, dont use them */
  struct IRC_User_s* hnext;	/* for user hash */
  struct IRC_User_s* glprev;  
  struct IRC_User_s* glnext;
  IrcMsg* msglist;	/* message handling functions */
  int timer_count;
  char *sname;		/* server name, only used by servers */
  char *vlink;		/* virtual link the server is on */
/** services related fields */
  int guest_count; 	/* how many times the nick was changed to guest */  
  /* void *sdata;	*/	/* generic pointer for services data */
  void *extra[32];	/* to keep extra data associated with user */
  /** snid this user as used */
  u_int32_t use_snid;	
  /** snid this user as requested */
  u_int32_t req_snid;
  /** language setting */
  int lang;		/* language setting */
  /** failed count, to be used on failed passwords */
  int fcount;  
};

/** Ban list */
struct IRC_BanList_s
{
  IRC_BanList* next;
  char* value;
};

/** Channel data */
struct IRC_Chan_s
{
  /** name */
  char name[CHANNELLEN + 1];
  /** services id */
  u_int32_t scid;    
  /** channel modes */
  int cmodes;
  /** maxusers limit */
  int limit;
  /** channel key */
  char* key;
  /** channel creation time stamp */
  time_t ts;
  int users_count;  /** number of users on channel */
  int lusers_count; /** number of local users on channel */
  /** list of users on the channel */
  IRC_ChanNode* userlist;
  /** last topic set */
  char* last_topic;
  char* last_topic_setter;
  /** time when the last topic was set */
  time_t t_ltopic;
  int max_msgs;
  int max_msgs_time;
  IRC_BanList* bans;
  struct IRC_Chan_s* hnextch;
  /** services data, points to a services data struture */
  void *sdata;          /* generic pointer for services data */      
  IRC_User *local_user; /* a pointer to a local user present on channel */
  int timer_count;	/* for channel timers */
  u_int32_t mlock_on;
  u_int32_t mlock_off;
  int mlock_max_msgs;
  int mlock_max_msgs_time;
  int mlock_limit;
  char* mlock_key;
};



/** Channel node *
  keeps user/user's chanflags (+o,+a,+v)
*/
struct IRC_ChanNode_s
{
  IRC_User *user;
  int cumodes;
  IRC_ChanNode *next;
};

struct IRC_UserNode_s
{
 IRC_Chan *chan;
 int cumodes;
 IRC_UserNode *next;
};

struct dlnode_s
{
   struct dlnode_s* prev;
   void* value;
   struct dlnode_s* next;
};
typedef struct dlnode_s dlnode;

/** List of users */
typedef struct
{
   int ltype;
   IRC_User* currpos;
} IRC_UserList;

/** List of channels */
typedef struct
{
   IRC_Chan* prev;
   IRC_Chan* current;
   IRC_Chan* next;
} IRC_ChanList;

/** private message data, to keep command handlers */
struct IrcMsg_s
{
  char *msg;
  MsgHandler func;
  IrcMsg* next;
};

/* external variables */

/** ircd type, we only support PTlink6 for now */
extern int ircd_type;

/** local time */
extern time_t irc_CurrentTime;

/** local irc server */
extern IRC_Server* irc_LocalServer;

/** local mlock client */
extern IRC_User* irc_ChanMlocker;

/** 
  * Sets debug mode, data sent/received from server will be sent
  * to the stdio
  */
extern void irc_SetDebug(int dval);

/**
 * Sets the local address to bind to before doing the connecting\n
 * Params:\n
 *  local address
 */
void irc_SetLocalAddress(char *la);

/**
 * Initializes the internal data structures\n
 * Params:\n
 * stype = server type (we only support PTLINK6 for now)\n
 * sname = server name\n
 * sinfo = server info (description)\n
 * logfd = log file (NULL if you dont want any log)\n
 */
extern int irc_Init(int stype, char *sname, char *sinfo, FILE *logfd);
/**
 * Sets the server version string\n
 * Params:\n
 * version = server version\n
 */
extern void irc_SetVersion(char *version);
 
/**
 * Starts connection to the irc server\n
 * Params:\n
 * ircserver = hostname of the irc server\n
 * ircport = port of the server of the irc server\n
 * connectpass = password for connection (ircd.conf C/N)\n
 * options mask = for later use\n
 *\n
 *Note: Unlike irc_FullConnect() StartConnect will return just after the connection
 *has been established without waiting for the netjoin data\n
 */
extern int irc_StartConnect(char* ircserver,int ircport, char* connectpass, 
	 int options);
	 
/**
 * See irc_StartConnect()\n
 * The difference from StartConnect is that the function will only return 
 * after all the data has been received from the remote server
 */
extern int irc_FullConnect(char* ircserver,int port, char* connectpass, 
	 int options);	 
/** 
  * Loop until the connection is terminated\n
  * Check irc_GetLastMsg() for the reason.
  */
extern void irc_LoopWhileConnected(void);

/**
  * Returns wether the connection to the ircd was already established\n
  * Returns:\n
  *  0 for disconnected\n
  *  1 if SERVER was sent but we are still doing the netjoin\n
  *  2 if netjoin operation is complete\n
  */
int irc_ConnectionStatus(void);

/**
 * Will return the server structure for a given name\n
 * NULL if not found
 */
extern IRC_Server* irc_FindServer(char *name);

/** Do the ircd socket I/O and execute any associated events */
extern int irc_DoEvents(void);
/** Call functions bind to a given event\n
 * Params:\n
 *   evt = event type\n
 *   edata1 = event parameter 1\n
 *   edata2 = event parameter 2\n
 * \n
 * Note: This function is used internally on the library from the raw handlers
 * to trigger the events bind to the respective event type.\n
 * You shouldn't need to use it.\n
 */
extern int irc_CallEvent(int evt, void* edata1, void* edata2);

/** Cancels other events pending on the current called event type\n
 * Note: This function should only be used from event handlers\n
 */
extern void irc_AbortThisEvent(void);

/**
 * Add an event handler associated with an event type\n
 * Params:\
 *   evt = event type\n
 *   efunc = event function handler
 */
extern int irc_AddEvent(int evt, void* efunc);
/**
 * Removes an event handler associated with an event type\n
 * Params:\
 *   evt = event type\n
 *   efunc = event function handler
 */
extern void irc_DelEvent(int evt, void* efunc);
/**
 * Returns the last raw message received from server
 */
extern char *irc_GetLastMsg(void);
/**
 * Returns the last message sent to a local client (service)\n
 * Usefull to parse commands on generic "*" private message handlers.
 */
extern char *irc_GetLastMsgCmd(void);

/** Adds an event for messages starting with a given string and whch
 * are sent to a given local user\n
 * Params:\n
 *  u = local user\n
 *  msg = start string\m
 *  func = function handler
 */
int irc_AddUMsgEvent(IRC_User *lu, char* msg, void* func);

/** Deletes an event for messages starting with a given string and which
 * are sent to a given local user\n
 * Params:\n
 *  u = local user\n
 *  msg = start string\m
 *  func = function handler
 */
void irc_DelUMsgEvent(IRC_User *u, char* msg, void* func);

/** Adds a timer event associated to a user\n
 *  Params:\n
 *   u = user target of the event\n
 *   ttime = seconds for the event to be triggered\n
 *   func = timer event handler\n
 *   tag = an id which will be used when the event is called\n
 */
void irc_AddUTimerEvent(IRC_User *u, int ttime, void* func, int tag);

/** Adds a timer event associated to a channel\n
 *  Params:\n
 *   u = user target of the event\n
 *   ttime = seconds for the event to be triggered\n
 *   func = timer event handler\n
 *   tag = an id which will be used when the event is called\n
 */
void irc_AddCTimerEvent(IRC_Chan *c, int ttime, void* func, int tag);

/**
 * Cancels all timer associated with a given user
 */
void irc_CancelUserTimerEvents(IRC_User *ru);

/**
 * Cancels all timer associated with a given channel
 */
void irc_CancelChanTimerEvents(IRC_Chan *c);


/* User/UserList related */
char* irc_UserMask(IRC_User* user);
char* irc_UserMaskP(IRC_User* user);
char* irc_UserSMask(IRC_User* user);
IRC_User* irc_FindUser(char *name);
IRC_User* irc_GetGlobalList(IRC_UserList* ul);
IRC_User* irc_GetNextUser(IRC_UserList* ul);
extern IRC_User*
irc_CreateLocalUser(char *nick, char *username, char *host, char *phost,
        char *info, char* umode);                
        
IRC_User* irc_FindLocalUser(char* name);
void irc_QuitLocalUser(IRC_User* u, char *msg);

void irc_IntroduceUser(IRC_User* u);
void irc_SendRaw(char *source, const char *fmt, ...);
void irc_SendNotice(IRC_User* from, IRC_User* to, char *fmt, ...);
void irc_SendMsg(IRC_User *to, IRC_User *from, char *fmt, ...);
void irc_SendCNotice(IRC_Chan *to, IRC_User *from, char *fmt, ...);
void irc_SendCMsg(IRC_Chan *to, IRC_User *from, char *fmt, ...);
void irc_SendONotice(IRC_Chan *to, IRC_User *from, char *fmt, ...);
void irc_GlobalNotice(IRC_User *from, char *mask, char *fmt, ...);
void irc_GlobalMessage(IRC_User *from, char *mask, char *fmt, ...);
void irc_AddUmodeChange(char *mchange, void* efunc);
void irc_Kill(IRC_User* u, IRC_User *s, char *msg);
void irc_Gline(IRC_User* s, char *who, char *mask, int duration, char *msg);
void irc_SvsMode(IRC_User* u, IRC_User *s, char *mchange);
void irc_SvsJoin(IRC_User* u, IRC_User *s, char *channel);
void irc_SvsNick(IRC_User* u, IRC_User *s, char *newnick);
void irc_SvsGuest(IRC_User *u, IRC_User *s, char *prefix, int number);
void irc_SendSanotice(IRC_User* from, char *fmt, ...);
void irc_ChgHost(IRC_User* to, char *vhost);
int irc_IsValidNick(const char* nick);
int irc_IsValidHostname(const char *hostname);
int irc_IsValidUsername(const char *username);

/* Channel functions */
IRC_Chan* irc_FindChan(char *name);
int irc_IsChanOp(IRC_User* user, IRC_Chan* chan);
void irc_AddCmodeChange(char *mchange, void* efunc);
IRC_ChanNode* irc_FindOnChan(IRC_Chan *chan, IRC_User *user);
void irc_ChanInvite(IRC_Chan *chan, IRC_User *target, IRC_User *source);
void irc_CNameInvite(char *chname, IRC_User *target, IRC_User *source);
int irc_ChanUnban(IRC_Chan* chan, IRC_User* target, IRC_User* source);
int irc_ChanMLockSet(IRC_User* from, IRC_Chan* chan, char *mlock);
void irc_ChanMLockApply(IRC_User *from, IRC_Chan *chan);


/** Makes a local user join a channel setting the given modes */
IRC_Chan* irc_ChanJoin(IRC_User* user, char* chname, int cumodes);
/** Makes a local user part from a channel */
void irc_ChanPart(IRC_User* user, IRC_Chan* chan);
/** Changes a channel topic */
IRC_Chan* irc_ChanTopic(IRC_Chan* chan, char* who, int t_topic, char *topic);
void irc_ChanMode(IRC_User* from, IRC_Chan* chan, char *fmt, ...);
void irc_ChanUMode(IRC_User* from, IRC_Chan* chan, char *modestr, IRC_User* to);
IRC_Chan* irc_NextChan(int start);
void irc_Kick(IRC_User* from, IRC_Chan* chan, IRC_User *to, char *fmt, ...);
void irc_SetChanMlocker(IRC_User *user);

/* raw handler, for ircd compatibility don't use raw handlers */
extern int irc_AddRawHandler(char *rawmsg, void   (* func)(int parc, char *argv[]));

/* statistical functions */
void irc_TimerStats(IRC_User *to, IRC_User* from);
void irc_UserStats(IRC_User *to, IRC_User* from);

/* stats functions */
u_int32_t irc_InByteCount(void);
u_int32_t irc_OutByteCount(void);

/* cmodes macros */
#define irc_IsCMode(x,y)	((x)->cmodes & y)

/* umodes macros */
#define irc_IsUMode(x,y)	((x)->umodes & y)
#define irc_IsCNAdmin(x)	((x)->cumodes & CU_MODE_ADMIN)
#define irc_IsCNOp(x)		((x)->cumodes & CU_MODE_OP)
#define irc_IsCNHOp(x)		((x)->cumodes & CU_MODE_HOP)
#define irc_IsCNVoice(x)	((x)->cumodes & CU_MODE_VOICE)

/* flags macros*/
#define irc_IsLocalUser(x)	((x)->iflags & IFL_LOCAL)

/* event types */
#define ET_NEW_SERVER   1
#define ET_NEW_USER	2
#define ET_QUIT		3
#define ET_KILL		4
#define ET_NICK_CHANGE	5
#define ET_LOOP		6
#define ET_CHAN_JOIN	7
#define ET_CHAN_MODE	8
#define ET_CHAN_KICK	9
#define ET_CHAN_PART	10
#define ET_CHAN_TOPIC	11

/* IRCD_TYPES */
#define PTLINK6		1

#define UMODE_ADMIN		0x00000001
#define UMODE_BOT		0x00000002
#define UMODE_HIDEOPER		0x00000004
#define UMODE_NETADMIN		0x00000008
#define UMODE_LOCOP		0x00000010
#define UMODE_REGMSG		0x00000020
#define UMODE_STEALTH		0x00000040
#define UMODE_TECHADMIN 	0x00000080
#define UMODE_SADMIN		0x00000100
#define UMODE_FLOODEX		0x00000200
#define UMODE_HELPER		0x00000400
#define UMODE_INVISIBLE 	0x00000800
#define UMODE_OPER		0x00001000
#define UMODE_PRIVATE		0x00002000
#define UMODE_IDENTIFIED	0x00004000
#define UMODE_SSL		0x00008000
#define UMODE_NODCC		0x00010000
#define UMODE_WALLOP		0x00020000
#define UMODE_SPY		0x00040000
#define UMODE_ZOMBIE		0x00080000

/* User flags*/
#define IFL_LOCAL		0x00000001 	/* is a local user */

/* Channel User Modes */
#define CU_MODE_ADMIN 		0x01
#define CU_MODE_OP		0x02
#define CU_MODE_VOICE		0x04
#define CU_MODE_HOP		0x08

#define CMODE_p         0x00000008
#define CMODE_s         0x00000010
#define CMODE_m         0x00000020
#define CMODE_t         0x00000040
#define CMODE_i         0x00000080
#define CMODE_n         0x00000100
#define CMODE_k         0x00000200
#define CMODE_b         0x00000400
#define CMODE_l         0x00001000
#define CMODE_r         0x00002000  /* channel is registered */
#define CMODE_R         0x00004000  /* registered nicks only */
#define CMODE_O         0x00008000  /* Opers only channel */
#define CMODE_A         0x00010000  /* Server Admins only channel */
#define CMODE_S         0x00020000  /* no cpam messages channel */
#define CMODE_d         0x00040000  /* no repetead messages channel */
#define CMODE_c         0x00080000  /* no colors channel */
#define CMODE_q         0x00100000  /* no quit messages channel */
#define CMODE_K         0x00200000  /* knock notice on failed join */
#define CMODE_f         0x00400000  /* limit messages per time */
#define CMODE_B         0x00800000  /* no bots allowed */
#define CMODE_N         0x01000000  /* No nickname changes */
#define CMODE_C		0x02000000  /* ssl mode */


#endif

