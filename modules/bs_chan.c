/******************************************************************
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License           *
 * Please read the file COPYING for copyright information.        *
 ******************************************************************
                                                                                
  File: bs_assign.c
  Description: botserv assign command
                                                                                
 *  $Id: bs_chan.c,v 1.4 2005/10/18 16:25:06 jpinto Exp $
*/

#include "module.h"
#include "my_sql.h"
#include "ns_group.h" /* is_soper( */
#include "nickserv.h"
#include "dbconf.h"
#include "lang/common.lh"
#include "lang/bs_assign.lh"

SVS_Module mod_info =
/* module, version, description */
{"bs_assign", "1.0", "botserv assign command" };
/* Change Log
  1.0 - 0000315: created botserv and its basic functionalities
*/

/* external functions we need */
ServiceUser* (*botserv_suser)(void);
int (*is_member_of)(IRC_User* user, u_int32_t sgid);
u_int32_t (*find_group)(char *name);
static int e_bot_introduce;

MOD_REQUIRES
  MOD_FUNC(dbconf_get)
  MOD_FUNC(botserv_suser)
  MOD_FUNC(is_member_of)
  MOD_FUNC(find_group)
  MOD_FUNC(e_bot_introduce)
MOD_END

/* internal functions */

/* available commands from module */
void bs_assign(IRC_User *s, IRC_User *u);
void bs_unassign(IRC_User *s, IRC_User *u);

/* Local variables */
ServiceUser* bsu;
u_int32_t bs_group;

/* DBconf */
static char *AdminRole;

DBCONF_REQUIRES
  DBCONF_GET("botserv", AdminRole)
DBCONF_END

/* Configuration load */
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

/* s = service the command was sent to
   u = user the command was sent from */
void bs_assign(IRC_User *s, IRC_User *u)
{
	char *channame, *bot_nick;
	MYSQL_RES *res;
	MYSQL_ROW row;
	ChanRecord *cr;
	IRC_User *user;
	int bid;
	
	channame = strtok(NULL, " ");
	bot_nick = strtok(NULL, "");

	if(IsNull(channame) || *channame=='\0' || IsNull(bot_nick) || *bot_nick=='\0')
	{
		send_lang(u, s, BS_ASSIGN_SYNTAX);
		return;
	}
	else
	{
		cr = OpenCR(channame);

		if (!cr)
		{
			send_lang(u, s, BS_ASSIGN_NO_SUCH_CHANNEL, channame);
			return;
		}

		bs_group = find_group(AdminRole);
		
		if ((u->snid != cr->founder) && !is_sadmin(u->snid) && !is_member_of(u, bs_group))
		{
			send_lang(u, s, BS_ASSIGN_NOT_ALLOWED, channame);
			return;
		}
		
		res = sql_query("SELECT bid FROM bs_chan WHERE scid=%d", cr->scid);
		row = sql_next_row(res);
		if (row)
		{
			send_lang(u, s, BS_ASSIGN_ALREADY_ASSIGNED, channame);
			sql_free(res);
			return;
		}
		sql_free(res);

		res = sql_query("SELECT bid FROM botserv WHERE nick=%s", sql_str(bot_nick));
		row = sql_next_row(res);
		if (!row)
		{
			send_lang(u, s, BS_ASSIGN_NO_SUCH_BOT, bot_nick);
			sql_free(res);
			return;
		}
		
		bid = atoi(row[0]);
	
		sqlb_init("bs_chan");
		sqlb_add_int("scid", cr->scid);
		sqlb_add_int("bid", bid);
		sqlb_add_int("kick", 0);
		sqlb_add_int("ttb", 0);
		sqlb_add_int("capsmin", 0);
		sqlb_add_int("capspercent", 0);
		sqlb_add_int("floodlines", 0);
		sqlb_add_int("floodsecs", 0);
		sqlb_add_int("repeattimes", 0);
		sqlb_add_int("bantype", 0);
		sqlb_add_int("banlast", 0);
		sql_execute("%s", sqlb_insert());

		res = sql_query("SELECT bid,scid FROM bs_chan WHERE scid=%d AND bid=%d", cr->scid, bid);
		if (!res)
		{
			send_lang(u, s, BS_ASSIGN_FAILED, bot_nick, bid, channame);
			return;
		}
		sql_free(res);
		
		user = irc_FindLocalUser(bot_nick);
		if (user)
		{
			IRC_Chan *chan = irc_ChanJoin(user, channame, 0);
			irc_ChanMode(bsu->u, chan, "+ao %s %s", user->nick, user->nick);
		}
												
		send_lang(u, s, BS_ASSIGN_DONE, bot_nick, channame);
		return;
	}
	return;
}

/* s = service the command was sent to
   u = user the command was sent from */
void bs_unassign(IRC_User *s, IRC_User *u)
{
	char *channame;
	MYSQL_RES *res;
	MYSQL_ROW row;
	ChanRecord *cr;
	IRC_User *user;
	int bid;
	
	channame = strtok(NULL, "");

	if(IsNull(channame) || *channame=='\0')
	{
		send_lang(u, s, BS_UNASSIGN_SYNTAX);
		return;
	}

	else
	{
		cr = OpenCR(channame);

		if (!cr)
		{
			send_lang(u, s, BS_UNASSIGN_NO_SUCH_CHANNEL, channame);
			return;
		}

		bs_group = find_group(AdminRole);
		
		if ((u->snid != cr->founder) && !is_sadmin(u->snid) && !is_member_of(u, bs_group))
		{
			send_lang(u, s, BS_UNASSIGN_NOT_ALLOWED, channame);
			return;
		}
		
		res = sql_query("SELECT bid FROM bs_chan WHERE scid=%d", cr->scid);
		row = sql_next_row(res);
		if (!row)
		{
			send_lang(u, s, BS_UNASSIGN_NOT_ASSIGNED, channame);
			sql_free(res);
			return;
		}
		sql_free(res);

		bid = atoi(row[0]);
		res = sql_query("SELECT nick FROM botserv WHERE bid=%d", bid);
		row = sql_next_row(res);

		user = irc_FindLocalUser(row[0]);
		if (user)
		{
			IRC_Chan *chan = irc_FindChan(channame);
			irc_ChanPart(user, chan);
		}
		
		sql_execute("DELETE FROM bs_chan WHERE bid=%d AND scid=%d", bid, cr->scid);
		
		send_lang(u, s, BS_UNASSIGN_DONE, user->nick, channame);
		return;
	}
	return;
}

