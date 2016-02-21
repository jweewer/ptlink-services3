/******************************************************************
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License           *
 * Please read the file COPYING for copyright information.        *
 ******************************************************************
                                                                                
  File: bs_drop.c
  Description: botserv bot command
                                                                                
 *  $Id: bs_drop.c,v 1.4 2005/10/18 16:25:06 jpinto Exp $
*/

#include "module.h"
#include "my_sql.h"
#include "ns_group.h" /* is_soper( */
#include "nickserv.h"
#include "dbconf.h"
#include "nsmacros.h"
/* lang files */
#include "lang/common.lh"
#include "lang/bs_drop.lh"

SVS_Module mod_info =
/* module, version, description */
{"bs_drop", "1.0", "botserv create command" };
/* Change Log
  1.0 - 0000315: created botserv and its basic functionalities
*/

/* external functions we need */
ServiceUser* (*botserv_suser)(void);
u_int32_t (*find_group)(char *name);
int (*is_member_of)(IRC_User* user, u_int32_t sgid);

MOD_REQUIRES
  MOD_FUNC(dbconf_get)
  MOD_FUNC(botserv_suser)
  MOD_FUNC(is_sadmin)
  MOD_FUNC(find_group)
  MOD_FUNC(is_member_of)
MOD_END

/* internal functions */

/* available commands from module */
void bs_drop(IRC_User *s, IRC_User *u);

/* Local variables */
ServiceUser* bsu;
int bs_log;
u_int32_t bs_group = 0;

/* Conf settings */
char *AdminRole;

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
  suser_add_cmd(bsu, "DROP", bs_drop, BS_DROP_SUMMARY, BS_DROP_HELP);    
  return 0;
}

void mod_unload(void)
{
  suser_del_mod_cmds(bsu, &mod_info);
}

/* s = service the command was sent to
   u = user the command was sent from */
void bs_drop(IRC_User *s, IRC_User *u)
{
	char *bot_nick;
	u_int32_t bid;
	u_int32_t source_snid;

  CHECK_IF_IDENTIFIED_NICK

	if (!is_member_of(u, bs_group) && !is_sadmin(u->snid))
	{
		send_lang(u, s, PERMISSION_DENIED);
		return;
	}
	
	bot_nick = strtok(NULL, " ");
	
	if (!bot_nick)
		send_lang(u, s, BS_DROP_SYNTAX_INV);
	else
	/* check if bot already exists */
	if ((sql_singlequery("SELECT bid FROM botserv WHERE nick=%s", 
		sql_str(irc_lower_nick(bot_nick))) < 1) || !(bid = sql_field_i(0)))
			send_lang(u, s, BS_DROP_BOT_X_NOT_FOUND, bot_nick);
	else
	{
		log_log(bs_log, mod_info.name, "%s dopped bot %s!", u->nick, bot_nick);	
		/* check if it is online */
		if(irc_FindLocalUser(bot_nick))
		{
			IRC_User *user = irc_FindLocalUser(bot_nick);
			if(user)
				irc_QuitLocalUser(user, "Bot droped");				
		}
		sql_execute("DELETE FROM botserv WHERE bid=%d", bid);
		send_lang(u, s, BS_DROP_DROPPED_X, bot_nick);
	}
}

/* module version */
