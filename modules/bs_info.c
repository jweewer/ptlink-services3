/******************************************************************
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License           *
 * Please read the file COPYING for copyright information.        *
 ******************************************************************
                                                                                
  File: bs_info.c
  Description: botserv bot command
                                                                                
 *  $Id: bs_info.c,v 1.4 2005/10/18 16:25:06 jpinto Exp $
*/

#include "module.h"
#include "my_sql.h"
#include "ns_group.h" /* is_soper( */
#include "nickserv.h"
#include "dbconf.h"
#include "nsmacros.h"
/* lang files */
#include "lang/common.lh"
#include "lang/bs_info.lh"

SVS_Module mod_info =
/* module, version, description */
{"bs_info", "1.0", "botserv create command" };
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
void bs_info(IRC_User *s, IRC_User *u);

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
  suser_add_cmd(bsu, "INFO", bs_info, BS_INFO_SUMMARY, BS_INFO_HELP);    
  return 0;
}

void mod_unload(void)
{
  suser_del_mod_cmds(bsu, &mod_info);
}

/* s = service the command was sent to
   u = user the command was sent from */
void bs_info(IRC_User *s, IRC_User *u)
{
	char *bot_nick;
	u_int32_t source_snid;

  CHECK_IF_IDENTIFIED_NICK

	bot_nick = strtok(NULL, " ");
	
	if (!bot_nick)
		send_lang(u, s, BS_INFO_SYNTAX_INV);
	else
	/* check if bot already exists */
	if (sql_singlequery("SELECT "
		"n.nick, b.nick, b.username, b.publichost, b.realname, b.t_create, b.t_expire, b.bid "
		"FROM botserv b, nickserv n WHERE b.nick=%s AND n.snid=b.owner_snid", 
		sql_str(irc_lower_nick(bot_nick))) == 0)
			send_lang(u, s, BS_INFO_BOT_X_NOT_FOUND, bot_nick);
	else
	{
		MYSQL_RES* res;
		MYSQL_ROW row;
		char buf[128];
		struct tm *tm;
		time_t t_create = sql_field_i(5);
		time_t t_expire = sql_field_i(6);
		u_int32_t bid = sql_field_i(7);
		send_lang(u, s, BS_INFO_HEADER);		
		send_lang(u, s, BS_INFO_NICK, bot_nick);
		snprintf(buf, sizeof(buf), "%s@%s", sql_field(2), sql_field(3));
		send_lang(u, s, BS_INFO_MASK, buf);
		send_lang(u, s, BS_INFO_REALNAME, sql_field(4));
	  send_lang(u, s, BS_INFO_OWNER, sql_field(0));
    tm = localtime(&t_create);
    strftime(buf, sizeof(buf), format_str(u, DATE_FORMAT), tm);
		send_lang(u, s, BS_INFO_CREATED, buf);
		if(t_expire)
		{
    	tm = localtime(&t_expire);
    	strftime(buf, sizeof(buf), format_str(u, DATE_FORMAT), tm);				
    	send_lang(u, s, BS_INFO_EXPIRES, buf);
		}
		res = sql_query("SELECT c.name FROM chanserv c, botserv_chans bc "
			"WHERE bc.bid=%d and c.scid=bc.scid", bid);
		while((row = sql_next_row(res)))
			send_lang(u,s, BS_INFO_ASSIGNED_TO_X, row[0]);
		sql_free(res);
		send_lang(u, s, BS_INFO_TAIL);
	}
}

/* module version */
