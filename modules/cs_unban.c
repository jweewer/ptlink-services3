/*****************************************************************
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999-2004 *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License          *
 * Please read the file COPYING for copyright information.       *
 *****************************************************************
                                                                                
  Description: chanserv unban command
                                                                                
 *  $Id: cs_unban.c,v 1.6 2005/10/18 16:25:06 jpinto Exp $
*/

#include "module.h"
#include "dbconf.h"
#include "chanrecord.h"
#include "my_sql.h"
#include "cs_role.h"
#include "nsmacros.h"
#include "nickserv.h"
/* lang files */
#include "lang/common.lh"
#include "lang/cscommon.lh"
#include "lang/cs_unban.lh"

SVS_Module mod_info =
/* module, version, description */
{"cs_unban", "2.0", "chanserv unban command" };
/* Change Log
  2.0 - 0000265: remove nickserv cache system
      - 0000281: no auth nick can't use chanserv
*/

/* external functions we need */
ServiceUser* (*chanserv_suser)(void);

MOD_REQUIRES
  MOD_FUNC(dbconf_get)
  MOD_FUNC(chanserv_suser)
  MOD_FUNC(role_with_permission)
MOD_END

/* Internal functions declaration */

/* core event handlers */

/* available commands from module */
void cs_unban(IRC_User *s, IRC_User *u);

ServiceUser* csu;
int cs_log;

/* Remote config */
static int NeedsAuth;
DBCONF_REQUIRES
  DBCONF_GET("chanserv", NeedsAuth)
DBCONF_END

/* this is called before load and at services rehash */
int mod_rehash(void)
{
  if(dbconf_get(dbconf_requires) < 0 )
  {
    errlog("Error reading dbconf!");
    return -1;
  }
  return 0;
}

int mod_load(void)
{
  csu = chanserv_suser();  
  suser_add_cmd(csu, "UNBAN", (void*) cs_unban, UNBAN_SUMMARY, UNBAN_HELP);    
  return 0;
}

void mod_unload(void)
{
  suser_del_mod_cmds(csu, &mod_info);
}

/* s = service the command was sent to
   u = user the command was sent from */
void cs_unban(IRC_User *s, IRC_User *u)
{
  u_int32_t source_snid;
  ChanRecord* cr;
  IRC_Chan* chan;
  char *chname;
  
  cr = NULL;
  chname = strtok(NULL, " ");  

  /* status validation */
  CHECK_IF_IDENTIFIED_NICK  
 
  /* syntax validation */ 
  if(NeedsAuth && !IsAuthenticated(u))
    send_lang(u, s, NEEDS_AUTH_NICK);
  else
  if(IsNull(chname))
    send_lang(u, s, UNBAN_SYNTAX);    
  /* check requirements */
  else if((chan = irc_FindChan(chname)) == NULL)
    send_lang(u,s, CHAN_X_IS_EMPTY, chname);
  else if((cr = chan->sdata) == NULL)
    send_lang(u, s, CHAN_X_NOT_REGISTERED, chname);    
  /* privileges validation */
  else if(role_with_permission(cr->scid, source_snid, P_UNBAN) == 0)
    send_lang(u, s, NO_UNBAN_PERM_ON_X, chname);
  /* execute operation */
  else
    {
      int r;
      r =  irc_ChanUnban(chan, u, chan->local_user ? chan->local_user : s);
      if(r == 0)
        send_lang(u, s, NO_BANS_ON_X, chname);
      else
        send_lang(u, s, REMOVED_X_BANS_ON_X, r, chname);
      irc_SvsJoin(u, s, chname);
    }

}

/* End of Module */
