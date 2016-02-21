/******************************************************************
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License           *
 * Please read the file COPYING for copyright information.        *
 ******************************************************************
                                                                                
  Description: chanserv clear command
                                                                                
 *  $Id: cs_clear.c,v 1.8 2005/10/18 16:25:06 jpinto Exp $
*/
#include "module.h"
#include "chanserv.h"
#include "chanrecord.h"
#include "my_sql.h"
#include "cs_role.h"
#include "nsmacros.h"
#include "nickserv.h"
#include "dbconf.h"
/* lang files */
#include "lang/common.lh"
#include "lang/cscommon.lh"
#include "lang/cs_clear.lh"

SVS_Module mod_info =
/* module, version, description */
{"cs_clear", "2.1", "chanserv clear command" };

/* Change Log
  2.1 - 0000351: cs_clear not enforcing mlock
  2.0 - 0000265: remove nickserv cache system
      - 0000281: No auth nicks can't use chanserv
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
void cs_clear(IRC_User *s, IRC_User *u);

/* rempote configuration */
static int NeedsAuth = 0;

DBCONF_REQUIRES
  DBCONF_GET("chanserv", NeedsAuth)
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

ServiceUser* csu;
int cs_log;

int mod_load(void)
{

  csu = chanserv_suser();  
  suser_add_cmd(csu, "CLEAR", cs_clear, CLEAR_SUMMARY, CLEAR_HELP);
  return 0;
}

void mod_unload(void)
{
  suser_del_mod_cmds(csu, &mod_info);
}

/* s = service the command was sent to
   u = user the command was sent from */
void cs_clear(IRC_User *s, IRC_User *u)
{
  u_int32_t source_snid;
  ChanRecord* cr;
  IRC_Chan* chan;
  char *chname = NULL;
  char *cltype = NULL; /* clear type */
  
  cr = NULL;
  chname = strtok(NULL, " ");
  if(chname)  
   cltype = strtok(NULL, " ");

  /* status validation */
  CHECK_IF_IDENTIFIED_NICK  
 
  /* syntax validation */ 
  if(NeedsAuth && !IsAuthenticated(u))
    send_lang(u, s, NEEDS_AUTH_NICK);
  else
  if(IsNull(chname) || IsNull(cltype))
    send_lang(u, s, CLEAR_SYNTAX);    
  /* check requirements */
  else if((chan = irc_FindChan(chname)) == NULL)
    send_lang(u,s, CHAN_X_IS_EMPTY, chname);
  else if((cr = chan->sdata) == NULL)
    send_lang(u, s, CHAN_X_NOT_REGISTERED, chname);    
  /* privileges validation */
  else if(role_with_permission(cr->scid, source_snid, P_CLEAR) == 0)
    send_lang(u, s, NO_CLEAR_PERM_ON_X, chname);
  /* check clear type */
  else if(strcasecmp(cltype, "OPS") == 0)
  {
    IRC_ChanNode* cn;
    cn = chan->userlist;
    while(cn)
    {
      /* if its op, remove it */
      if(!irc_IsUMode(cn->user, UMODE_STEALTH) && 
        !irc_IsLocalUser(cn->user) && 
        (cn->user != u) && cn->cumodes & CU_MODE_OP)
          irc_ChanUMode(chan->local_user ? chan->local_user : s, chan, "-o" , cn->user);
        cn = cn->next;
    }
    send_lang(u, s, OPS_CLEARED_X, chname); 
  }
  else if(strcasecmp(cltype, "VOICES") == 0)
  {
    IRC_ChanNode* cn;
    cn = chan->userlist;
    while(cn)
    {
    /* if its op, remove it */
      if(!irc_IsUMode(cn->user, UMODE_STEALTH) &&
        !irc_IsLocalUser(cn->user) && 
        (cn->user != u) && cn->cumodes & CU_MODE_VOICE)
          irc_ChanUMode(chan->local_user ? chan->local_user : s, chan, "-v" , cn->user);
      cn = cn->next;
    }
  send_lang(u, s, VOICES_CLEARED_X, chname); 
  }    
  else 
  if(strcasecmp(cltype, "USERS") == 0)
  {
    IRC_ChanNode* cn;
    IRC_ChanNode* next_cn;
    char* reason = strtok(NULL, "");      
    cn = chan->userlist;
    while(cn)
    {
      next_cn = cn->next;
      /* if its op, remove it */
      if(!irc_IsLocalUser(cn->user) &&
        !irc_IsUMode(cn->user, UMODE_STEALTH) 
        && (cn->user != u))
      {
        if(reason)
          irc_Kick(chan->local_user ? chan->local_user : s, chan, cn->user, "%s", reason);
        else
          irc_Kick(chan->local_user ? chan->local_user : s, chan, cn->user, NULL); 
      }
      cn = next_cn;
    }
    send_lang(u, s, USERS_CLEARED_X, chname);
  }    
  else 
  if(strcasecmp(cltype, "BANS") == 0)
  {
    IRC_BanList *ban;
    IRC_BanList *next_ban;
      ban = chan->bans;
      while(ban)
        {
          next_ban = ban->next;
          irc_ChanMode(chan->local_user ? chan->local_user : s, chan, "-b %s", ban->value);
          ban = next_ban;
        }
      send_lang(u, s, BANS_CLEARED_X, chname);
    }
  else 
  if(strcasecmp(cltype, "MODES") == 0)
  {  
    irc_ChanMode(chan->local_user ? chan->local_user : s, chan, "-ABCcdfikKlmnNOpqRsSt");
    irc_ChanMLockApply(chan->local_user ? chan->local_user : s, chan);
    send_lang(u, s, MODES_CLEARED_X, chname);
  }
  else
    send_lang(u, s, INVALID_CLEAR_TYPE_X, cltype);    
}

/* End of Module */
