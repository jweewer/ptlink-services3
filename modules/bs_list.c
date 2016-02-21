/******************************************************************
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License           *
 * Please read the file COPYING for copyright information.        *
 ******************************************************************
                                                                                
  File: bs_list.c
  Description: botserv bot command
                                                                                
 *  $Id: bs_list.c,v 1.6 2005/10/18 16:25:06 jpinto Exp $
*/

#include "module.h"
#include "my_sql.h"
#include "ns_group.h" /* is_soper( */
#include "nickserv.h"
#include "dbconf.h"
#include "nsmacros.h"
/* lang files */
#include "lang/common.lh"
#include "lang/bs_list.lh"

SVS_Module mod_info =
/* module, version, description */
{"bs_list", "1.0", "botserv create command" };
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
void bs_list(IRC_User *s, IRC_User *u);

/* Local variables */
ServiceUser* bsu;
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
  bs_group = find_group(AdminRole);	  
  suser_add_cmd(bsu, "LIST", bs_list, BS_LIST_SUMMARY, BS_LIST_HELP);    
  return 0;
}

void mod_unload(void)
{
  suser_del_mod_cmds(bsu, &mod_info);
}

/* s = service the command was sent to
   u = user the command was sent from */
void bs_list(IRC_User *s, IRC_User *u)
{
	u_int32_t source_snid;
	MYSQL_RES *res;
	MYSQL_ROW row;
	u_int32_t owner_snid = 0;
	char* owner;
	int is_super;

  CHECK_IF_IDENTIFIED_NICK
	owner = strtok(NULL, " ");
	is_super = is_member_of(u, bs_group) || is_sadmin(u->snid);
	
	if(owner)
	{
		if (!is_super)
		{
			send_lang(u, s, PERMISSION_DENIED);
			return;
		}	
		if(sql_singlequery("SELECT snid FROM nickserv WHERE nick=%s", 
			sql_str(irc_lower_nick(owner))))
				owner_snid = sql_field_i(0);
		else
		{
			send_lang(u, s, NICK_X_NOT_REGISTERED, owner);
			return;
		}		
	} 
	else 
		owner_snid = is_super ? 0 : source_snid;
		
	if(owner_snid)
		res = sql_query("SELECT nick, username, publichost, realname "
  		" FROM botserv WHERE owner_snid=%d", owner_snid);	
	else
		res = sql_query("SELECT nick, username, publichost, realname "
  		" FROM botserv");
	send_lang(u, s, BS_LIST_HEADER);
	while((row = sql_next_row(res)))
	{
		char buf[128];
		snprintf(buf, sizeof(buf), "%s %s@%s %s", 
			row[0], row[1], row[2], row[3]);
		send_lang(u, s, BS_LIST_X, buf);
	}
	sql_free(res);
	send_lang(u, s, BS_LIST_TAIL);
}

/* module version */
