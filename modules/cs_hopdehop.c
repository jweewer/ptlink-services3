/******************************************************************
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999-2006 *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License           *
 * Please read the file COPYING for copyright information.        *
 ******************************************************************
                                                                                
  Description: chanserv hop command
                                                                                
 *  $Id: cs_hopdehop.c,v 1.2 2005/10/18 16:25:06 jpinto Exp $
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
#include "lang/cs_hopdehop.lh"

SVS_Module mod_info =
/* module, version, description */
{"cs_hopdehop", "1.2", "chanserv hopdehop command" };

/* Change Log
  1.2 - #74, do not allow HOP/DEHOP unless halfops is enabled
  1.1 - #66, fixed cs hop/dehop status check
  1.0 - created, copy from cs_opdeop.c
*/

            
/* external functions we need */
ServiceUser* (*chanserv_suser)(void);
int (*role_with_permission)(u_int32_t scid, u_int32_t snid, int permission);

MOD_REQUIRES
  MOD_FUNC(dbconf_get)
  MOD_FUNC(chanserv_suser)
  MOD_FUNC(role_with_permission)
MOD_END

/* Internal functions declaration */
int mod_rehash(void);
int mod_load(void);
void mod_unload(void);

/* core event handlers */

/* available commands from module */
void cs_hop(IRC_User *s, IRC_User *u);
void cs_dehop(IRC_User *s, IRC_User *u);

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
  suser_add_cmd(csu, "HOP", cs_hop, HOP_SUMMARY, HOP_HELP);
  suser_add_cmd(csu, "DEHOP", cs_dehop, DEHOP_SUMMARY, DEHOP_HELP);  
  return 0;
}

void mod_unload(void)
{
  suser_del_mod_cmds(csu, &mod_info);
}

/* s = service the command was sent to
   u = user the command was sent from */
void cs_hop(IRC_User *s, IRC_User *u)
{
  u_int32_t source_snid;
  ChanRecord* cr;
  IRC_User* targetu;
  IRC_Chan* chan;
  IRC_ChanNode* cn;

  char *chname, *target;

#ifndef HALFOPS
  send_lang(u, s, FEATURE_NOT_SUPPORTED);
  return;
#endif
  
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
    send_lang(u, s, CHAN_HOP_SYNTAX);    
  else
  if((chan = irc_FindChan(chname)) == NULL)
    send_lang(u,s, CHAN_X_IS_EMPTY, chname);
  else
  if((targetu = irc_FindUser(target)) == NULL)
    send_lang(u,s, NICK_X_NOT_ONLINE, target);
  else
  if((cn = irc_FindOnChan(chan, targetu)) == NULL)
    send_lang(u, s, NICK_X_NOT_ON_X, target, chname);
  else 
/* needs libircservice support */ 
#if 0  
  if(cn->cumodes & CU_MODE_HOP)
    send_lang(u, s, NICK_X_ALREADY_HOP_ON_X, target, chname);
  else
#endif  
  if((cr = chan->sdata) == NULL)
    send_lang(u, s, CHAN_X_NOT_REGISTERED, chname);
  else /* everything is valid */
  if(role_with_permission(cr->scid, source_snid, P_HOPDEHOP) == 0)
    send_lang(u, s, NO_HOPDEHOP_PERM_ON_X, chname);
  else
    {
      if(IsOpNotice(cr))
        irc_SendONotice(chan, s, "%s used HOP command for %s on %s",
          u->nick, target, chname);      
      irc_ChanUMode(chan->local_user ? chan->local_user : csu->u, chan, "+h" , targetu);    
    }
    
}

/* s = service the command was sent to
   u = user the command was sent from */
void cs_dehop(IRC_User *s, IRC_User *u)
{
  u_int32_t source_snid;
  ChanRecord* cr;
  IRC_User* targetu;
  IRC_Chan* chan;
  IRC_ChanNode* cn;

  char *chname, *target;

#ifndef HALFOPS
  send_lang(u, s, FEATURE_NOT_SUPPORTED);
  return;
#endif
  
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
    send_lang(u, s, CHAN_HOP_SYNTAX);    
  else
  if((chan = irc_FindChan(chname)) == NULL)
    send_lang(u,s, CHAN_X_IS_EMPTY, chname);
  else
  if((targetu = irc_FindUser(target)) == NULL)
    send_lang(u,s, NICK_X_NOT_ONLINE, target);
  else
  if((cn = irc_FindOnChan(chan, targetu)) == NULL)
    send_lang(u, s, NICK_X_NOT_ON_X, target, chname);
  else
/* needs libircservice support */  
#if 0  
  if(!(cn->cumodes & CU_MODE_HOP))
    send_lang(u, s, NICK_X_NOT_HOP_ON_X, target, chname);
  else
#endif  
  if(cn->cumodes & CU_MODE_ADMIN)
    send_lang(u, s, CANT_DEHOP_ADMIN_X_ON_X, target, chname);
  else  
  if((cr = chan->sdata) == NULL)
    send_lang(u, s, CHAN_X_NOT_REGISTERED, chname);
  else /* everything is valid */
  if(role_with_permission(cr->scid, source_snid, P_HOPDEHOP) == 0)
    send_lang(u, s, NO_HOPDEHOP_PERM_ON_X, chname);
  else
    {
      if(IsOpNotice(cr))
        irc_SendONotice(chan, s, "%s used DEHOP command for %s on %s",
          u->nick, target, chname);                            
      irc_ChanUMode(chan->local_user ? chan->local_user : csu->u, chan, "-h" , targetu);   
    }  
    
}
