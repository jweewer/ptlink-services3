/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************  

  Description: operserv module

 *  $Id: operserv.c,v 1.9 2005/10/18 16:25:06 jpinto Exp $
*/

#include "module.h"
#include "ns_group.h" /* we need is_oper/admin() */
#include "dbconf.h"
#include "path.h"
#include "lang/common.lh"
#include "lang/operserv.lh"

/* module, version, description */
SVS_Module mod_info =
{"operserv", "2.1", "operserv core module" };

/* Change Log
  2.1 - #14: setting OperChan is not optional as it should
  2.0 - 0000273: +a only set on sadmins if OperChan is defined
        0000265: remove nickserv cache system
*/

static int irc;

/* functions we require */
MOD_REQUIRES
  MOD_FUNC(dbconf_get)
  MOD_FUNC(dbconf_get_or_build)
  MOD_FUNC(irc)
  MOD_FUNC(is_soper)
  MOD_FUNC(is_sadmin)
MOD_END
/* functions we provide */
ServiceUser* operserv_suser(void);                                                                                
MOD_PROVIDES
  MOD_FUNC(operserv_suser)
MOD_END

/* internal functions */
void ev_os_oper(IRC_User *u);

/* core events */
void ev_os_new_user(IRC_User* u, void *s);

/* commands */
void os_unknown(IRC_User* s, IRC_User* t);

/** Local config */
static char* Nick;
static char* Username;
static char* Hostname;
static char* Realname;
static char* LogChan;
static char* OperChan;
static char* SAdminChan;
static int OperControl;

dbConfItem dbconf_provides[] = {
  DBCONF_WORD(Nick,     "OperServ", "Operserv service nick")
  DBCONF_WORD(Username, "Services", "Operserv service username")
  DBCONF_WORD(Hostname, "PTlink.net", "Operserv service hostname")
  DBCONF_STR(Realname,  "Operserv Service", "Operserv service real name")
  DBCONF_WORD_OPT(LogChan,  "#Services.log", "Operserv log channel")
  DBCONF_WORD_OPT(OperChan, "#Opers", "Operators auto join channel")
  DBCONF_WORD_OPT(SAdminChan,"#Services.log", "SAdmins auto join channel")
  DBCONF_SWITCH(OperControl, "off",
    "Remove +o from opers which are note on the Oper group")
  {NULL}
};

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

ServiceUser osu;
int os_log;

int mod_load(void)
{
  /* open a log file */
  os_log = log_open("operserv","operserv");
  
  if(os_log<0)
  {
    errlog("Could not open operserv log file!");
    return -1;
  }

  /* Create operserv user */      
  osu.u = irc_CreateLocalUser(Nick, Username, Hostname, Hostname,
    Realname,"+ro");
  
  if(LogChan)
  {
    IRC_Chan *chan;
    log_set_irc(os_log, Nick, LogChan);
    chan = irc_ChanJoin(osu.u, LogChan, CU_MODE_ADMIN|CU_MODE_OP);
    irc_ChanMode(osu.u, chan, "+Ostn");
  }
          
  /* Add msg events for child modules */
  irc_AddUMsgEvent(osu.u, "*", (void*) os_unknown); /* any other msg handler */
    
  /* Add user events */
  
  /* New user for hostrule functions */
  irc_AddEvent(ET_NEW_USER, (void*) ev_os_new_user); /* new user */
  
  /* Mode change for "on oper" functions */
  irc_AddUmodeChange("+o", ev_os_oper);
  
  return 0;
}

void
mod_unload(void)
{

  /* remove operserv and all associated events */
  irc_QuitLocalUser(osu.u, "Removing service");

  /* remove irc events */
  irc_DelEvent(ET_NEW_USER, (void*) ev_os_new_user);
}
 
void os_unknown(IRC_User* s, IRC_User* t)
{
  send_lang(t, s, UNKNOWN_COMMAND, irc_GetLastMsgCmd());
}


void ev_os_new_user(IRC_User* u, void *s)
{
 /* maybe we will need something here */
}

void ev_os_oper(IRC_User *u)
{  
  if(OperControl && (is_soper(u->snid) == 0))
  {
    send_lang(u, osu.u, NOT_REGISTERED_OPER);
    irc_SvsMode(u, osu.u, "-o");
    return;
  }
  if(OperChan)
  {
    irc_CNameInvite(OperChan, u, osu.u);
    irc_SvsJoin(u, osu.u, OperChan);
  }
  if(is_sadmin(u->snid) != 0) /* is an sadmin */
  {
    irc_SvsMode(u, osu.u, "+a");
    if(SAdminChan)
    {
      irc_CNameInvite(SAdminChan, u, osu.u);
      irc_SvsJoin(u, osu.u, SAdminChan);            
    }            
  }  
}

/* to return the operserv client */
ServiceUser* operserv_suser(void)
{
  return &osu;
}

