/**********************************************************************
 * PTlink IRC Services is (C) Copyright PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************

  Description: memoserv set command

 *  $Id: ms_info.c,v 1.15 2005/10/18 16:25:06 jpinto Exp $
*/

#include "module.h"
#include "memoserv.h"
#include "dbconf.h"
#include "my_sql.h"
#include "nsmacros.h"
#include "ns_group.h"	/* is_sadmin */
#include "lang/common.lh"
#include "lang/ms_info.lh"

SVS_Module mod_info =
/* module, version, description */
{"ms_info", "1.0", "memoserv info command" };
/* Change Log
  1.0 - #5: split memoserv with a memoserv options table
*/

/* external functions we need */
ServiceUser* (*memoserv_suser)(void);
u_int32_t (*find_group)(char *name);

MOD_REQUIRES 
  DBCONF_FUNCTIONS
  MEMOSERV_FUNCTIONS
  MOD_FUNC(is_sadmin)
MOD_END

/* internal functions */
void ms_info(IRC_User *s, IRC_User *u);

ServiceUser* msu;
int ms_log;

int mod_load(void)
{
  msu = memoserv_suser();
  ms_log = log_handle("memoserv");

  suser_add_cmd(msu, "INFO", ms_info, MS_INFO_SUMMARY, MS_INFO_HELP);
  return 0;
}

void
mod_unload(void)
{
  suser_del_mod_cmds(msu, &mod_info);
}

/* s = service the command was sent to
   u = user the command was sent from */
void ms_info(IRC_User *s, IRC_User *u)
{
  u_int32_t source_snid;
  u_int32_t tsnid;
  char *nick;

  CHECK_IF_IDENTIFIED_NICK
  
  nick = strtok(NULL, " ");
  if(nick == NULL)
    nick = u->nick;
  if((tsnid = nick2snid(nick)) == 0 )
    send_lang(u, s, NICK_X_NOT_REGISTERED, nick);
  else
  if((source_snid != tsnid) && !is_sadmin(source_snid))
    send_lang(u, s, MS_INFO_DENIED);
  else
  {
    int maxmemos;
    int bquota;
    u_int32_t flags;
    int mc, umc;
    if(memoserv_get_options(tsnid, &maxmemos, &bquota, &flags) == 0)
    {
      send_lang(u, s, UPDATE_FAIL);
      return;
    }
    umc = unread_memos_count(tsnid);
    mc = memos_count(tsnid);
    send_lang(u, s, MS_INFO_HEADER);
    send_lang(u, s, MS_INFO_MAXMEMOS_X, maxmemos);
    if(umc)
      send_lang(u, s, MS_INFO_IN_USE_X_X, mc, umc);
    else
      send_lang(u, s, MS_INFO_IN_USE_X, mc);
    /* send_lang(u, s, MS_INFO_BQUITA_X, bquota); */
    if(flags)
      send_lang(u, s, MS_INFO_OPTIONS,
       mask_string(memoserv_options, flags));
    send_lang(u, s, MS_INFO_TAIL);
  }
}
