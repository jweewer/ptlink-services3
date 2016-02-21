/******************************************************************
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License           *
 * Please read the file COPYING for copyright information.        *
 ******************************************************************

  File: cs_drop.c
  Description: chanserv drop module

 *  $Id: cs_drop.c,v 1.6 2005/10/18 16:25:06 jpinto Exp $
*/

#include "module.h"
#include "chanrecord.h"
#include "my_sql.h"
#include "nickserv.h"
#include "dbconf.h"
#include "nsmacros.h"
#include "ns_group.h"
#include "encrypt.h"
/* lang files */
#include "lang/common.lh"
#include "lang/cs_drop.lh"
#include "lang/cscommon.lh"


/* module, version, parentmodule, description, NULL , NULL */
SVS_Module mod_info =
{"cs_drop", "3.2",  "chanserv drop module" };

/* Change Log
  3.2 - #78, CS DROP does not check if channel is suspended
  3.1 - #65: Fixed Chan Drop & BotServ
  3.0 - 0000265: remove nickserv cache system
*/

/* external dependencies */
ServiceUser* (*chanserv_suser)(void);
int cs_log;
int e_chan_delete;

MOD_REQUIRES
  MOD_FUNC(dbconf_get)
  MOD_FUNC(chanserv_suser)
  MOD_FUNC(e_chan_delete)
  MOD_FUNC(is_sadmin)
MOD_END

/* internal functions */
void drop_channel(u_int32_t scid, char* chname);

/* Remote config */
static int NickSecurityCode;
DBCONF_REQUIRES
  DBCONF_GET("nickserv", NickSecurityCode)
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

/* service user */
ServiceUser* csu;

/* user related events */

/* commands */
void cs_drop(IRC_User *s, IRC_User *u);
void cs_sdrop(IRC_User *s, IRC_User *u);

/* module init */
int mod_load(void)
{
    cs_log = log_handle("chanserv");    
    csu = chanserv_suser();
    suser_add_cmd(csu, "DROP", cs_drop, DROP_SUMMARY, DROP_HELP);
    suser_add_cmd(csu, "SDROP", cs_sdrop, SDROP_SUMMARY, SDROP_HELP);
    return 0;
}

/* module unload */
void mod_unload(void)
{
  suser_del_mod_cmds(csu, &mod_info);
}
/* Internal functions */

void drop_channel(u_int32_t scid, char *chname)
{
  log_log(cs_log, mod_info.name, "Dropping scid %lu,  %s", scid, chname);
  
  /* first call related actions */
  mod_do_event(e_chan_delete, &scid, NULL);
 
  /* now really delete it */
  sql_execute("DELETE FROM chanserv WHERE scid=%lu", scid);
}

/* user related events */
/* commands */
void cs_drop(IRC_User *s, IRC_User *u)
{
    ChanRecord *cr;
    u_int32_t source_snid;
    char *chname = strtok(NULL, " ");
    IRC_Chan *chan;	        

    CHECK_IF_IDENTIFIED_NICK
    
    if(IsNull(chname))
      send_lang(u, s, CHAN_DROP_SYNTAX);
    else if((cr = OpenCR(chname)) == NULL)
      send_lang(u, s, CHAN_X_NOT_REGISTERED, chname);
    else if(cr->founder != source_snid)
      send_lang(u, s, CHAN_NOT_FOUNDER_X, chname); 
    else if(cr->flags & NFL_SUSPENDED)
      send_lang(u, s, CHAN_X_IS_SUSPENDED, chname);
    else
      { 
      	MYSQL_RES *res;
      	MYSQL_ROW row;
	u_int32_t scid = cr->scid;      
	char* nick_sec = NULL;
	res = sql_query("SELECT securitycode FROM nickserv_security WHERE snid=%d",
	  source_snid);
	row = sql_next_row(res);
	if(row && row[0])
	{
	  nick_sec = malloc(16);
	  memcpy(nick_sec, hex_bin(row[0]), 16);
	}
	sql_free(res);
        CloseCR(cr); cr = NULL; /* close here because we have returns below */
	if(NickSecurityCode && nick_sec && IsAuthenticated(u))
          {
            char* securitycode = strtok(NULL, " ");
            if(IsNull(securitycode))
              {
                send_lang(u, s, DROP_SECURITY_REQUIRED);
                return;
              }
           else if(memcmp(nick_sec, encrypted_password(securitycode), 16) != 0)
              {
                send_lang(u, s, INVALID_SECURITY_CODE);
                FREE(nick_sec);
                return;
              }      
           FREE(nick_sec);
	  }
	drop_channel(scid, chname);
	chan = irc_FindChan(chname);
	if(chan && chan->sdata)
	  {
	    irc_ChanMode(s, chan, "-r");
	    if(chan->sdata)
	      CloseCR(chan->sdata);
            chan->sdata = NULL;
            if(chan->local_user)
	      irc_ChanPart(chan->local_user, chan);
          }        
	send_lang(u, s, CHAN_X_DROPPED, chname);
    }
}

void cs_sdrop(IRC_User *s, IRC_User *u)
{
    ChanRecord *cr;
    u_int32_t source_snid;
    char *chname = strtok(NULL, " ");
    IRC_Chan *chan;
    
    CHECK_IF_IDENTIFIED_NICK
    
    if(!is_sadmin(u->snid))
	send_lang(u, s, ONLY_FOR_SADMINS);
    else if(IsNull(chname))
	send_lang(u, s, CHAN_SDROP_SYNTAX);
    else if((cr = OpenCR(chname)) == NULL)
	send_lang(u, s, CHAN_X_NOT_REGISTERED, chname);
    else
    {
	u_int32_t scid = cr->scid;
	
	drop_channel(scid, chname);
	chan = irc_FindChan(chname);
	if(chan && chan->sdata)
	{
	    irc_ChanMode(s, chan, "-r");
	    if(chan->sdata)
		CloseCR(chan->sdata);
	    chan->sdata = NULL;
	    if(chan->local_user)
	      irc_ChanPart(chan->local_user, chan);	   
	}
	log_log(cs_log, mod_info.name, "%s SDROPPED channel: %s", u->nick, chname);
	send_lang(u, s, CHAN_X_DROPPED, chname);
    }
    if(cr)
	CloseCR(cr);
}

