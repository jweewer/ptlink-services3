/******************************************************************
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License           *
 * Please read the file COPYING for copyright information.        *
 ******************************************************************
                                                                                
  File: bs_create.c
  Description: botserv bot command
                                                                                
 *  $Id: bs_create.c,v 1.10 2005/10/18 16:25:06 jpinto Exp $
*/

#include "module.h"
#include "my_sql.h"
#include "ns_group.h" /* is_soper( */
#include "nickserv.h"
#include "dbconf.h"
#include "nsmacros.h"
/* lang files */
#include "lang/common.lh"
#include "lang/bs_create.lh"

SVS_Module mod_info =
/* module, version, description */
{"bs_create", "1.1", "botserv create command" };
/* Change Log
  1.1 - #40: bs_create should use nick2snid to get the nick snids
  1.0 - 0000315: created botserv and its basic functionalities
*/

/* external functions we need */
ServiceUser* (*botserv_suser)(void);
u_int32_t (*find_group)(char *name);
int (*is_member_of)(IRC_User* user, u_int32_t sgid);

MOD_REQUIRES
  DBCONF_FUNCTIONS
  MOD_FUNC(botserv_suser)
  MOD_FUNC(is_sadmin)
  MOD_FUNC(find_group)
  MOD_FUNC(is_member_of)
MOD_END

/* internal functions */

/* available commands from module */
void bs_create(IRC_User *s, IRC_User *u);

/* Local variables */
ServiceUser* bsu;
int bs_log;
u_int32_t bs_group = 0;

/* Conf settings */
static char *AdminRole;

DBCONF_REQUIRES
  DBCONF_GET("botserv", AdminRole)
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
  bs_log = log_handle("botserv");
  bs_group = find_group(AdminRole);	  
  suser_add_cmd(bsu, "CREATE", bs_create, BS_CREATE_SUMMARY, BS_CREATE_HELP);    
  return 0;
}

void mod_unload(void)
{
  suser_del_mod_cmds(bsu, &mod_info);
}

/* s = service the command was sent to
   u = user the command was sent from */
void bs_create(IRC_User *s, IRC_User *u)
{
	char *bot_owner, *bot_nick, *bot_username, *bot_hostname, *bot_info;
	char *bot_time;
	int expire_time;
	u_int32_t owner_snid;
	u_int32_t source_snid;

	CHECK_IF_IDENTIFIED_NICK

	if (!is_member_of(u, bs_group) && !is_sadmin(u->snid))
	{
		send_lang(u, s, PERMISSION_DENIED);
		return;
	}
	
	bot_owner = strtok(NULL, " ");
	bot_time = strtok(NULL, " ");
	if(bot_time)
		expire_time = ftime_str(bot_time);
	bot_nick = strtok(NULL, " ");
	bot_username = strtok(NULL, " ");
	bot_hostname = strtok(NULL, " ");
	bot_info = strtok(NULL, "");
	
	if (!bot_owner || !bot_time || !bot_nick || !bot_username || !bot_hostname
		|| !bot_info || (expire_time==-1))
			send_lang(u, s, BS_CREATE_SYNTAX_INV);
	else
	/* check if the nickname is valid */
	if(!irc_IsValidNick(bot_nick))
		send_lang(u, s, BS_CREATE_INVALID_NICK_X, bot_nick);	
	else
	/* check if the username is valid */
	if(!irc_IsValidUsername(bot_username))
		send_lang(u, s, BS_CREATE_INVALID_USER_X, bot_username);
	else 
	/* check if the hostname is valid */
	if(!irc_IsValidHostname(bot_hostname))
		send_lang(u, s, BS_CREATE_INVALID_HOST_X, bot_hostname);
	else 	
	/* check if bot already exists */
	if (sql_singlequery("SELECT bid FROM botserv WHERE nick=%s", 
		sql_str(irc_lower_nick(bot_nick))) > 0)
			send_lang(u, s, BS_CREATE_X_EXISTS, bot_nick);
	else
	/* this should never happen, bot in mem but not on the db */
	if(irc_FindLocalUser(bot_nick))
	{
		 send_lang(u, s, BS_CREATE_X_EXISTS, bot_nick);
		 log_log(bs_log, mod_info.name, 
		 	"Bot %s was found in mem but not on the db !", bot_nick);
	}
	else
	/* check if the nick is registered */
	/* need to fix this, we should not access tables from other modules	 */
	if(nick2snid(bot_nick))
	  send_lang(u, s, BS_CREATE_NICK_X_IS_REG_X, bot_nick);
	else
	/* check if the owner exists */
	if((owner_snid = nick2snid(bot_owner)) == 0)
	  send_lang(u, s, NICK_X_NOT_REGISTERED, bot_owner);
	else
	/* all conditions were checked, lets proceed */
	{
		sqlb_init("botserv");
		sqlb_add_int("owner_snid", owner_snid);
		sqlb_add_str("nick", bot_nick);
		sqlb_add_str("username", bot_username);
		sqlb_add_str("publichost", bot_hostname);
		sqlb_add_str("realname", bot_info);
		sqlb_add_int("t_create", irc_CurrentTime);
		sqlb_add_int("t_expire", 
			expire_time ? irc_CurrentTime+expire_time : 0);
		if(sql_execute("%s", sqlb_insert()) < 0)
			send_lang(u, s, UPDATE_FAIL);
		else
		{
			log_log(bs_log, mod_info.name, "%s created bot %s %s %s %s",
				u->nick, bot_nick, bot_username, bot_hostname, bot_info);
			irc_CreateLocalUser(bot_nick, bot_username, bot_hostname,
				bot_hostname, bot_info, "+r");
			send_lang(u, s, BS_CREATE_CREATED_X, bot_nick);
		}
	}
}

/* module version */
