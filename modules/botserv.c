/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************

  Description: botserv core module

 *  $Id: botserv.c,v 1.15 2005/10/18 16:25:06 jpinto Exp $
*/

#include "chanserv.h"
#include "module.h"
#include "path.h"
#include "my_sql.h"
#include "dbconf.h"
#include "lang/common.lh"

/* module, version, description */
SVS_Module mod_info =
{"botserv", "1.2", "botserv core module" };

#define DB_VERSION	1

/*  ChangeLog:
  1.2 - #65: Fixed Chan Drop & BotServ
  1.1 - #18 - bots are not getting chanmode +o on join
  1.0 - 0000315: created botserv core module and basic functionalities
*/ 

/** functions/events we require **/
/* void (*FunctionPointer)(void);*/
int mysql_connection;
ServiceUser* (*chanserv_suser)(void);
static int irc;
static int e_chan_delete;

MOD_REQUIRES
  MOD_FUNC(dbconf_get_or_build)
  MOD_FUNC(irc)
  MOD_FUNC(mysql_connection)
  MOD_FUNC(chanserv_suser)
  MOD_FUNC(e_chan_delete)
MOD_END

/* functions we provide */
ServiceUser* botserv_suser(void);
static int e_bot_introduce;

MOD_PROVIDES
  MOD_FUNC(e_bot_introduce)
  MOD_FUNC(botserv_suser)
MOD_END

/* core events */
void ev_bs_new_server(IRC_Server* nserver, IRC_Server* from);
int sql_upgrade(int version, int post);

/* commands */
void bs_unknown(IRC_User* s, IRC_User* t);

int ev_botserv_chan_delete(u_int32_t *snid, void *dummy);

ServiceUser bsu;
int bs_log;

/* Local config */
static char* Nick;
static char* Username;
static char* Hostname;
static char* Realname;
static char* AdminRole;
static int DefaultExpireTime;
static int MaxChansPerBot;
DBCONF_PROVIDES
  DBCONF_WORD(Nick, 	"BotServ", "Botserv service nick")
  DBCONF_WORD(Username, "Services", "Botserv service username")
  DBCONF_WORD(Hostname, "PTlink.net", "Botserv service hostname")
  DBCONF_STR(Realname, "Botserv Service", "Botserv service real name")
  DBCONF_WORD_OPT(AdminRole,"Admin", "Botserv admin role")
  DBCONF_TIME(DefaultExpireTime, "0d", "Botserv default expire time")
  DBCONF_INT(MaxChansPerBot, "1",
    "The max number of channels a bot can be assigned to")  
DBCONF_END

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
  /* first try to open the log file */
  bs_log = log_open("botserv","botserv");

  if(bs_log < 0)
  {
    errlog("Could not open botserv log file!");
    return -1;
  }
  
  if(sql_check_inst_upgrade(mod_info.name, DB_VERSION, sql_upgrade) < 0)
	  return -4;

  /* Create botserv user */
  bsu.u = irc_CreateLocalUser(Nick, Username, Hostname, Hostname,
    Realname,"+ro");

  irc_AddUMsgEvent(bsu.u, "*", bs_unknown); /* any other msg handler */

  /* Server events - to introduce local bots */
  irc_AddEvent(ET_NEW_SERVER, ev_bs_new_server);
  
  /* botserv needs to getout when channels are deleted */
  mod_add_event_action(e_chan_delete,
  	(ActionHandler) ev_botserv_chan_delete);
          
   
  return 0;
}

void mod_unload(void)
{
  MYSQL_RES *res;
  MYSQL_ROW row;
  
  /* remote pseudo users before disconnecting BotServ */
  res = sql_query("SELECT nick FROM botserv");

  while((row = sql_next_row(res)))
    {
      IRC_User *user = irc_FindUser(row[0]);
      if(user)
	      irc_QuitLocalUser(user, "Removing service");
    }
  sql_free(res);
  
  /* remove botserv and all associated events */
  irc_QuitLocalUser(bsu.u, "Removing service");

}

/* to return the nickserv client */
ServiceUser* botserv_suser(void)
{
	  return &bsu;
}

void bs_unknown(IRC_User* s, IRC_User* t)
{
  send_lang(t, s, UNKNOWN_COMMAND, irc_GetLastMsgCmd());
}

/* this is called when a new server is introduced,
we just care for the first server when services connect to the hub 
so that we create the bots and join them to their channels
*/
void ev_bs_new_server(IRC_Server* nserver, IRC_Server *from)
{
  static int already_loaded = 0;
  MYSQL_RES *res;
  MYSQL_ROW row;

  if (already_loaded)
    return;
	
  res = sql_query("SELECT nick,username,publichost,realname,bid FROM botserv");

  while((row = sql_next_row(res)))
  {
    MYSQL_RES *res_chan;
    MYSQL_ROW row_chan;
    IRC_User *user;
    u_int32_t bid = atoi(row[4]);
    user = irc_CreateLocalUser(row[0], row[1], row[2], row[2], row[3], "+r");
    mod_do_event(e_bot_introduce, &bid, NULL);
    res_chan = sql_query("SELECT c.name FROM chanserv c, botserv_chans bc "
      "WHERE bc.bid=%d AND c.scid=bc.scid", bid);
    while((row_chan = sql_next_row(res_chan)))
    {
      IRC_Chan *chan = irc_ChanJoin(user, row_chan[0], 0);
      irc_ChanMode(bsu.u, chan, "+ao %s %s", user->nick, user->nick);
    }
    sql_free(res_chan);
  }

  sql_free(res);
	
  already_loaded = -1;
}

/* this version takes care of sql upgrades */
int sql_upgrade(int version, int post)
{
	return 1;
}

int ev_botserv_chan_delete(u_int32_t *scid, void *dummy)
{
  sql_execute("DELETE FROM botserv_chans WHERE scid=%d", *scid);
  return 0;
}

