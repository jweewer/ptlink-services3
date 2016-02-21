/******************************************************************
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999-2004 *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License           *
 * Please read the file COPYING for copyright information.        *
 ******************************************************************

  File: cs_register.c
  Description: chanserv register command

 *  $Id: cs_register.c,v 1.6 2005/10/18 16:25:06 jpinto Exp $
*/

#include "module.h"
#include "nickserv.h"
#include "chanserv.h"
#include "my_sql.h"
#include "chanrecord.h"
#include "nsmacros.h"
#include "dbconf.h"
/* lang files */
#include "lang/cscommon.lh"
#include "lang/cs_register.lh"

SVS_Module mod_info =
/* module, version, description */
{"cs_register", "2.1","chanserv register command" };

/* Change Log
   2.1 - 0000302: DefaultMlock add
   2.0 - 0000265: remove nickserv cache system
*/
/* functions and events we require */
ServiceUser*  (*chanserv_suser)(void);
int e_chan_register;

MOD_REQUIRES
  MOD_FUNC(dbconf_get)
  MOD_FUNC(chanserv_suser)
  MOD_FUNC(e_chan_register)  
MOD_END

/* internal functions */
int chans_count(u_int32_t snid);

void cs_register(IRC_User *s, IRC_User *u);


/* Local variables */
static ServiceUser* csu;
int cs_log;

/* Remote config */
static int NeedsAuth;
static int NickSecurityCode;
static int MaxChansPerUser;
static char* DefaultMlock;
DBCONF_REQUIRES
  DBCONF_GET("chanserv", NeedsAuth)
  DBCONF_GET("nickserv", NickSecurityCode)
  DBCONF_GET("chanserv", MaxChansPerUser)
  DBCONF_GET("chanserv", DefaultMlock)
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
  cs_log = log_handle("chanserv");
  csu = chanserv_suser();

  suser_add_cmd(csu,
    "REGISTER", cs_register, REGISTER_SUMMARY, REGISTER_HELP);  

  return 0;
}

void mod_unload(void)
{
  suser_del_mod_cmds(csu, &mod_info);
}
 
/* s = service the command was sent to
   u = user the command was sent from */
void cs_register(IRC_User *s, IRC_User *u)
{
  u_int32_t source_snid;
  ChanRecord* cr = NULL;
  IRC_Chan *chan;
  char *cname;
  cname = strtok(NULL, " ");

  CHECK_IF_IDENTIFIED_NICK
  
  if(NickSecurityCode  && !IsAuthenticated(u))
    send_lang(u, s, NEEDS_AUTH_NICK);
  else if(IsNull(cname))
    send_lang(u, s, CHAN_REGISTER_SYNTAX);    
  else if((chan = irc_FindChan(cname)) == NULL)
    send_lang(u, s, CHAN_X_IS_EMPTY, cname);      
  else if((cr = OpenCR(cname)))
    send_lang(u, s, CHAN_X_ALREADY_REGISTERED, cname);
  else if(!irc_IsChanOp(u, chan))
    send_lang(u, s, CHAN_NOT_OP);
  else if(chans_count(source_snid) >= MaxChansPerUser)
    send_lang(u, s, REACHED_MAX_CHANS_X, MaxChansPerUser);
  else /* everything is valid lets do the registraion */
    {
      log_log(cs_log, mod_info.name, "Channel %s registered by %s",
        cname, u->nick);
        
      cr = CreateCR(cname);
      if(IsNull(cr))
        {
          send_lang(u, s, UPDATE_FAIL);
          return;
        }
      cr->flags = 0;
      cr->t_reg = irc_CurrentTime;
      cr->t_last_use = irc_CurrentTime;
      cr->t_ltopic = irc_CurrentTime;
      cr->t_maxusers = irc_CurrentTime;
      cr->maxusers = chan->users_count;
      if(DefaultMlock)
        cr->mlock = strdup(DefaultMlock);
      cr->founder = u->snid;
      if(UpdateCR(cr) == 0)
        send_lang(u, s, UPDATE_FAIL);
      else 
        {
          irc_ChanMode(csu->u, chan, "+r");
          irc_ChanUMode(csu->u, chan, "+a" , u);
          send_lang(u, s, CHAN_X_REGISTER_SUCCESS, cname);
          mod_do_event(e_chan_register, u, cr);
        }
      chan->sdata = cr;
      if(cr->mlock)
      {
        irc_ChanMLockSet(s, chan, cr->mlock);
        irc_ChanMLockApply(s, chan);
      }
    }
}

int chans_count(u_int32_t snid)
{
  MYSQL_RES* res;
  MYSQL_ROW row;
  int count = 0;
  res = sql_query("SELECT COUNT(*) FROM chanserv WHERE founder=%d", snid);
  row = sql_next_row(res);
  if(row)
    count = atoi(row[0]);
  sql_free(res);
  return count;
}

/* MODULE FORMAT VERSION */
