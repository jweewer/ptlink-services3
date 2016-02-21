/******************************************************************
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License           *
 * Please read the file COPYING for copyright information.        *
 ******************************************************************
                                                                                
  File: bs_assign.c
  Description: botserv assign command
                                                                                
 *  $Id: bs_assign.c,v 1.13 2005/10/18 16:25:06 jpinto Exp $
*/

#include "module.h"
#include "dbconf.h"
#include "my_sql.h"
#include "ns_group.h" /* is_soper( */
#include "nickserv.h"
#include "nsmacros.h"
/* lang files */
#include "lang/bs_assign.lh"

SVS_Module mod_info =
/* module, version, description */
{"bs_assign", "1.1", "botserv assign command" };
/* Change Log
	1.1 - #18 : bots are not getting chanmode +o on join
  1.0 - 0000315: created botserv and its basic functionalities
*/

/* external functions we need */
ServiceUser* (*botserv_suser)(void);
int (*is_member_of)(IRC_User* user, u_int32_t sgid);
u_int32_t (*find_group)(char *name);

MOD_REQUIRES
  DBCONF_FUNCTIONS
	MOD_FUNC(botserv_suser)
	MOD_FUNC(is_member_of)
	MOD_FUNC(is_sadmin)
	MOD_FUNC(find_group)
MOD_END

/* internal functions */

/* available commands from module */
void bs_assign(IRC_User *s, IRC_User *u);
void bs_unassign(IRC_User *s, IRC_User *u);

/* Local variables */
ServiceUser* bsu;
char *BotServControlRole;
u_int32_t bs_group;

/* Conf settings */
static char *AdminRole;
static int MaxChansPerBot;
DBCONF_REQUIRES
  DBCONF_GET("botserv", AdminRole)
  DBCONF_GET("botserv", MaxChansPerBot)
DBCONF_END

int mod_rehash(void)
{
  if(dbconf_get(dbconf_requires) < 0)
  {
    errlog("Required configuration item is missing!");
    return -1;
  }
  return 0;
}

int mod_load(void)
{
	bsu = botserv_suser(); 
	suser_add_cmd(bsu, "ASSIGN", bs_assign, BS_ASSIGN_SUMMARY, BS_ASSIGN_HELP);  
	suser_add_cmd(bsu, "UNASSIGN", bs_unassign, BS_UNASSIGN_SUMMARY, BS_UNASSIGN_HELP);  
	bs_group = find_group(AdminRole);
	return 0;
}

void mod_unload(void)
{
  suser_del_mod_cmds(bsu, &mod_info);
}

/* count of chans assigned to a bot */
static int bot_chan_count(u_int32_t bid)
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	int count = 0;
	res = sql_query("SELECT count(*) FROM botserv_chans WHERE bid=%d", bid);
	if(res && (row = sql_next_row(res)))
		count = atoi(row[0]);
	sql_free(res);
	return count;
}

/* s = service the command was sent to
   u = user the command was sent from */
void bs_assign(IRC_User *s, IRC_User *u)
{
	u_int32_t source_snid;
	char *chan_name, *bot_nick;
	ChanRecord *cr = NULL;
	u_int32_t bid = 0;
	int is_super;
	IRC_Chan *chan;
	
	CHECK_IF_IDENTIFIED_NICK
	is_super = is_member_of(u, bs_group) || is_sadmin(u->snid);
	
	bot_nick = strtok(NULL, " ");	
	chan_name = strtok(NULL, " ");

  /* I was unable to do this on the usual condition cascade so
  I am doing the lookup here */
  if(bot_nick && 
  	sql_singlequery("SELECT bid FROM botserv WHERE nick=%s", sql_str(bot_nick)))
 			bid = sql_field_i(0);
 	
	if(!bot_nick || !chan_name)
		send_lang(u, s, BS_ASSIGN_SYNTAX);
	else
	if((cr = OpenCR(chan_name)) == NULL)
		send_lang(u, s, BS_ASSIGN_NO_SUCH_CHANNEL, chan_name);
	else
	if(!is_super && ((cr->founder != source_snid) ||
		(sql_singlequery("SELECT owner_snid FROM botserv WHERE bid=%d", bid)
		&& (sql_field_i(0) != source_snid))))
			send_lang(u, s, BS_ASSIGN_NOT_ALLOWED, chan_name);
	else
	/* check for MaxChansPerBot */
	if(!is_super && MaxChansPerBot && bot_chan_count(bid)>=MaxChansPerBot)
		send_lang(u, s, BS_ASSIGN_MAX_X, MaxChansPerBot);
	else
	/* check for service already on channel or on the db */
	if(((chan = irc_FindChan(chan_name)) && chan->local_user) || 
		sql_singlequery("SELECT bid FROM botserv_chans WHERE "
			"scid = %d", cr->scid))
				send_lang(u, s, BS_ASSIGN_ALREADY_ASSIGNED_X, chan_name);
	else
	{
		sqlb_init("botserv_chans");
		sqlb_add_int("scid", cr->scid);
		sqlb_add_int("bid", bid);
/* this will come later		
		sqlb_add_int("kick", 0);
		sqlb_add_int("ttb", 0);
		sqlb_add_int("capsmin", 0);
		sqlb_add_int("capspercent", 0);
		sqlb_add_int("floodlines", 0);
		sqlb_add_int("floodsecs", 0);
		sqlb_add_int("repeattimes", 0);
		sqlb_add_int("bantype", 0);
		sqlb_add_int("banlast", 0);
*/	
		if(sql_execute("%s", sqlb_insert()))
		{
			IRC_User *user = irc_FindLocalUser(bot_nick);
  		if (user)
  		{
				chan = irc_ChanJoin(user, chan_name, 0);
				irc_ChanMode(bsu->u, chan, "+ao %s %s", user->nick, user->nick);
			}
			send_lang(u, s, BS_ASSIGN_DONE, bot_nick, chan_name);
		}
		else
		send_lang(u, s, UPDATE_FAIL);
	}
	CloseCR(cr);
	return;
}

/* s = service the command was sent to
   u = user the command was sent from */
void bs_unassign(IRC_User *s, IRC_User *u)
{
	char *chan_name;
	char *bot_nick;
	ChanRecord *cr = NULL;
	u_int32_t bid;
	u_int32_t source_snid;
	int is_super;
	IRC_Chan *chan;
  
  CHECK_IF_IDENTIFIED_NICK	
  is_super = is_member_of(u, bs_group) || is_sadmin(u->snid);
  
  bot_nick = strtok(NULL, " ");
	chan_name = strtok(NULL, " ");
	
  /* I was unable to do this on the usual condition cascade so
  I am doing the lookup here */
  if(bot_nick && 
  	sql_singlequery("SELECT bid FROM botserv WHERE nick=%s", sql_str(bot_nick)))
 			bid = sql_field_i(0);
 			
	if(!bot_nick || !chan_name)
		send_lang(u, s, BS_UNASSIGN_SYNTAX);
	else 
	if((cr = OpenCR(chan_name)) == NULL)
		send_lang(u, s, BS_UNASSIGN_NO_SUCH_CHANNEL, chan_name);
	else
	if(bid == 0)
		send_lang(u, s, BS_ASSIGN_NO_SUCH_BOT, bot_nick);
	else	
	if(!is_super && ((cr->founder != source_snid) ||
		(sql_singlequery("SELECT owner_snid FROM botserv WHERE bid=%d", bid) 
		&& (sql_field_i(0) != source_snid))))
			send_lang(u, s, BS_UNASSIGN_NOT_ALLOWED, chan_name);
	else
	if(sql_singlequery("SELECT scid, bid FROM botserv_chans "
		"WHERE scid=%d AND bid=%d", cr->scid, bid) == 0)
		send_lang(u, s, BS_UNASSIGN_X_NOT_ASSIGNED_TO_X, bot_nick, chan_name);
  else
  {
		IRC_User *user = irc_FindLocalUser(bot_nick);
  	chan = irc_FindChan(chan_name);
  	if(user && chan)
  		irc_ChanPart(user, chan);
		sql_execute("DELETE FROM botserv_chans WHERE bid=%d and scid=%d", 
			bid, cr->scid);
		send_lang(u, s, BS_UNASSIGN_X_DONE_X, bot_nick, chan_name);
	}
	CloseCR(cr);
	return;
}

