/*****************************************************************
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                http://software.pt-link.net                    *
 * This program is distributed under GNU Public License          *
 * Please read the file COPYING for copyright information.       *
 *****************************************************************
                                                                                
  Description : chanserv voice and devoice commands

 *  $Id: cs_voicedevoice.c,v 1.10 2005/10/18 16:25:06 jpinto Exp $
*/

#include "module.h"
#include "chanrecord.h"
#include "my_sql.h"
#include "cs_role.h"
#include "nsmacros.h"
#include "chanserv.h"
#include "nickserv.h"
#include "dbconf.h"
/* lang files */
#include "lang/common.lh"
#include "lang/cscommon.lh"
#include "lang/cs_voicedevoice.lh"

SVS_Module mod_info =
/* module, version, description */
{"cs_voicedevoice", "2.1", "chanserv voicedevoice command" };
/* Change Log
  2.1 - 0000334: review code to use local bot when possible
  2.0 - 0000265: remove nickserv cache system
      - 0000281: no auth nick can't use chanserv
  1.1 - 0000229: The cs_voice is not supporting the OpNotice option
        Added target initialization        
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
void cs_voice(IRC_User *s, IRC_User *u);
void cs_devoice(IRC_User *s, IRC_User *u);

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
  suser_add_cmd(csu, "VOICE", (void*) cs_voice, VOICE_SUMMARY, VOICE_HELP);
  suser_add_cmd(csu, "DEVOICE", (void*) cs_devoice, DEVOICE_SUMMARY, DEVOICE_HELP);  
  return 0;
}

void mod_unload(void)
{
  suser_del_mod_cmds(csu, &mod_info);
}

/* s = service the command was sent to
   u = user the command was sent from */
void cs_voice(IRC_User *s, IRC_User *u)
{
  u_int32_t source_snid;
  ChanRecord* cr;
  IRC_User* targetu;
  IRC_Chan* chan;
  IRC_ChanNode* cn;

  char *chname, *target;
  
  cr = NULL;
  target = NULL;
  chname = strtok(NULL, " ");  
  if(chname)
    target = strtok(NULL, " ");

  CHECK_IF_IDENTIFIED_NICK  
  if(NeedsAuth && !IsAuthenticated(u))
    send_lang(u, s, NEEDS_AUTH_NICK);
  else
  if(IsNull(chname) || IsNull(target))
    send_lang(u, s, CHAN_VOICE_SYNTAX);    
  else
  if((chan = irc_FindChan(chname)) == NULL)
    send_lang(u,s, CHAN_X_IS_EMPTY, chname);
  else
  if((targetu = irc_FindUser(target)) == NULL)
    send_lang(u,s, NICK_X_NOT_ONLINE, target);
  else
  if(irc_IsUMode(targetu, UMODE_STEALTH) || (cn = irc_FindOnChan(chan, targetu)) == NULL)
    send_lang(u, s, NICK_X_NOT_ON_X, target, chname);
  else  
  if(cn->cumodes & CU_MODE_VOICE)
    send_lang(u, s, NICK_X_ALREADY_VOICE_ON_X, target, chname);
  else
  if((cr = chan->sdata) == NULL)
    send_lang(u, s, CHAN_X_NOT_REGISTERED, chname);
  else /* everything is valid */
  if(role_with_permission(cr->scid, source_snid, P_VOICEDEVOICE) == 0)
    send_lang(u, s, NO_VOICEDEVOICE_PERM_ON_X, chname);
  else
    {
      if(IsOpNotice(cr))
        irc_SendONotice(chan, s, "%s used VOICE command for %s on %s",
          u->nick, target, chname);
      irc_ChanUMode(chan->local_user ? chan->local_user : csu->u, chan, "+v" , targetu);    
    }
    
}

/* s = service the command was sent to
   u = user the command was sent from */
void cs_devoice(IRC_User *s, IRC_User *u)
{
  u_int32_t source_snid;
  ChanRecord* cr;
  IRC_User* targetu;
  IRC_Chan* chan;
  IRC_ChanNode* cn;

  char *chname, *target;
  
  cr = NULL;
  chname = strtok(NULL, " ");  
  if(chname)
    target = strtok(NULL, " ");

  CHECK_IF_IDENTIFIED_NICK  
  if(NeedsAuth && !IsAuthenticated(u))
    send_lang(u, s, NEEDS_AUTH_NICK);
  else
  if(IsNull(chname) || IsNull(target))
    send_lang(u, s, CHAN_DEVOICE_SYNTAX);    
  else
  if((chan = irc_FindChan(chname)) == NULL)
    send_lang(u,s, CHAN_X_IS_EMPTY, chname);
  else
  if((targetu = irc_FindUser(target)) == NULL)
    send_lang(u,s, NICK_X_NOT_ONLINE, target);
  else
  if(irc_IsUMode(targetu, UMODE_STEALTH) || (cn = irc_FindOnChan(chan, targetu)) == NULL)
    send_lang(u, s, NICK_X_NOT_ON_X, target, chname);
  else  
  if(!(cn->cumodes & CU_MODE_VOICE))
    send_lang(u, s, NICK_X_NOT_VOICE_ON_X, target, chname);
  else
  if(cn->cumodes & CU_MODE_ADMIN)
    send_lang(u, s, CANT_DEVOICE_ADMIN_X_ON_X, target, chname);
  else  
  if((cr = chan->sdata) == NULL)
    send_lang(u, s, CHAN_X_NOT_REGISTERED, chname);
  else /* everything is valid */
  if(role_with_permission(cr->scid, source_snid, P_VOICEDEVOICE) == 0)
    send_lang(u, s, NO_VOICEDEVOICE_PERM_ON_X, chname);
  else
    {
      if(IsOpNotice(cr))
        irc_SendONotice(chan, s, "%s used DEVOICE command for %s on %s",
          u->nick, target, chname);    
      irc_ChanUMode(chan->local_user ? chan->local_user : csu->u, chan, "-v" , targetu);   
    }  
}

/* End of Module */
