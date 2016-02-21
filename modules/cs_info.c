/******************************************************************
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License           *
 * Please read the file COPYING for copyright information.        *
 ******************************************************************
                                                                                
  File: cs_info.c
  Description: chanserv info command
                                                                                
 *  $Id: cs_info.c,v 1.6 2005/10/18 16:25:06 jpinto Exp $
*/
#include "chanserv.h"
#include "module.h"
#include "my_sql.h"
#include "chanrecord.h"
#include "ns_group.h" /* is_soper( */
#include "nsmacros.h"
#include "nickserv.h"
#include "dbconf.h"
#include "lang/common.lh"
#include "lang/cscommon.lh"
#include "lang/cs_info.lh"

SVS_Module mod_info =
/* module, version, description */
{"cs_info", "2.0", "chanserv info command" };
/* Change Log
  2.0 - 0000278: secureops option on chanserv
      - 0000270: topiclock flag is not correct for the cs info
      - 0000265: remove nickserv cache system      
      - 0000281: No auth nicks can't use chanserv
  1.2 - 0000267: chan options are not displayed on the info for founder
  1.1 - 0000256: ago time split in larger time units
*/

/* external functions we need */
ServiceUser* (*chanserv_suser)(void);

MOD_REQUIRES
  MOD_FUNC(dbconf_get)
  MOD_FUNC(chanserv_suser)
  MOD_FUNC(is_soper)
MOD_END

/* internal functions */
char* ago_time(time_t t, IRC_User *u);

/* available commands from module */
void cs_info(IRC_User *s, IRC_User *u);

/* Remote config */
static int NeedsAuth;

DBCONF_REQUIRES
  DBCONF_GET("chanserv", NeedsAuth)
DBCONF_END

/* this is called before load and at services rehash */
int mod_rehash(void)
{
  if(dbconf_get(dbconf_requires) < 0)
  {
    errlog("Required configuration item is missing!");
    return -1;
  }
  return 0;
}    

/* Local variables */
ServiceUser* csu;
OptionMask options_mask[] =
  {
    { "private", CFL_PRIVATE, NULL },
    { "noexpire", CFL_NOEXPIRE, NULL },
    { "opnotice", CFL_OPNOTICE, NULL },
    { "restricted", CFL_RESTRICTED, NULL },
    { "topiclock", CFL_TOPICLOCK, NULL },
    { "secureops", CFL_SECUREOPS, NULL },
    { "suspended", CFL_SUSPENDED, NULL },
    { NULL }
};


int mod_load(void)
{
  csu = chanserv_suser();  
  suser_add_cmd(csu, "INFO", cs_info, INFO_SUMMARY, INFO_HELP);  
  return 0;
}

void mod_unload(void)
{
  suser_del_mod_cmds(csu, &mod_info);
}

char* ago_time(time_t t, IRC_User *u) 
{
  int years,months,days,hours,minutes,seconds;    

  t = irc_CurrentTime -t;
  years = t/(12*30*24*3600);
  t %= 12*30*24*3600;
  months = t/(30*24*3600);
  t %= 30*24*3600;
  days = t/(24*3600);
  t %= 24*3600;
  hours = t/3600;
  t %= 3600;
  minutes = t/60;
  t %= 60;
  seconds = t;
  if(years)
    return lang_str(u, AGO_TIME_Y,
      years,
      months,
      days);
  else
  if(months)
    return lang_str(u, AGO_TIME_M,
      months,
      days,
      hours);
  if(days)
    return lang_str(u, AGO_TIME_D,
      days,
      hours,
      minutes);
  else
    return lang_str(u,AGO_TIME,
      hours,
      minutes,
      seconds);                                                                                
}

/* s = service the command was sent to
   u = user the command was sent from */
void cs_info(IRC_User *s, IRC_User *u)
{
  ChanRecord* cr;
  char buf[64];
  struct tm *tm;
  char *target = strtok(NULL, " ");
  IRC_Chan *chan;
  u_int32_t source_snid = u->snid;
  
  if(NeedsAuth && !IsAuthenticated(u))
    send_lang(u, s, NEEDS_AUTH_NICK);
  else
  if(IsNull(target))
    send_lang(u, s, CHAN_INFO_SYNTAX);
  else
  if((cr = OpenCR(target)) == NULL)
    send_lang(u, s, CHAN_X_NOT_REGISTERED, target);
  else /* everything is valid */
    {    
      int sadmin = (is_soper(source_snid) != 0);
      
      send_lang(u, s, CHAN_INFO_HEADER);
      
      if(sadmin)
        send_lang(u, s, CHAN_SCID, cr->scid);
        
      send_lang(u, s, CHAN_NAME, target);
      if(IsPrivateChan(cr) && !sadmin &&
        ((source_snid == 0) || (source_snid && source_snid != cr->founder)))
        send_lang(u, s, CHAN_INFO_PRIVATE, target);
      else      
        {
          if(cr->cdesc)
            send_lang(u, s, CHAN_DESC, cr->cdesc);
          if(sql_singlequery("SELECT nick FROM nickserv WHERE snid=%d", 
            cr->founder))
              send_lang(u, s, CHAN_FOUNDER_X, sql_field(0));
          if(sql_singlequery("SELECT nick FROM nickserv WHERE snid=%d", 
            cr->successor))
	      send_lang(u, s, CHAN_SUCCESSOR_X, sql_field(0));
          tm = localtime(&cr->t_reg);
          strftime(buf, sizeof(buf), format_str(u, INFO_DATE_FORMAT), tm);
          send_lang(u, s, CHAN_REGDATE_X_X, buf, ago_time(cr->t_reg, u ));          
          tm = localtime(&cr->t_last_use);
          strftime(buf, sizeof(buf), format_str(u, INFO_DATE_FORMAT), tm);          
          send_lang(u, s, CHAN_LAST_USE_X_X, buf, ago_time(cr->t_last_use, u ));
          if(!IsNull(cr->email))
            send_lang(u, s, CHAN_EMAIL, cr->email);
          if(!IsNull(cr->url))
            send_lang(u, s, CHAN_URL, cr->url);
          if(!IsNull(cr->entrymsg))
            send_lang(u, s, CHAN_ENTRYMSG, cr->entrymsg);
          if(!IsNull(cr->last_topic))
            send_lang(u, s, CHAN_TOPIC_X, cr->last_topic);
          if(!IsNull(cr->last_topic_setter))
            send_lang(u, s, CHAN_TOPIC_SETTER_X, cr->last_topic_setter);
          chan = irc_FindChan(target);
          if(chan && chan->users_count)
            send_lang(u, s, CHAN_CURRUSERS_X, chan ? chan->users_count : 0);
          tm = localtime(&cr->t_maxusers);
          strftime(buf, sizeof(buf), format_str(u, INFO_DATE_FORMAT), tm);          
          send_lang(u, s, CHAN_USERS_REC_X_X_X, cr->maxusers,
            buf, ago_time(cr->t_maxusers, u));
          if((sadmin || (source_snid == cr->founder)) 
            && cr->mlock && cr->mlock[0])
            send_lang(u, s, CHAN_MLOCK_X, cr->mlock);
	  if(cr->flags && (sadmin || (source_snid == cr->founder)))
            send_lang(u, s, CHAN_OPTIONS_X,
              mask_string(options_mask, cr->flags));        
          if(cr->flags & NFL_SUSPENDED)
          {
            MYSQL_RES *res;
            MYSQL_ROW row;
            res = sql_query("SELECT t_when, duration, reason "
              "FROM chanserv_suspensions WHERE scid=%d", cr->scid);
            if(res && (row = sql_next_row(res)))
            {
              time_t t_when = atoi(row[0]);
              int remaining = -1;
              if(atoi(row[1]) != 0)
              {
                remaining = atoi(row[1]) - (irc_CurrentTime - t_when);
                remaining /= 24*3600;
                remaining++;              
                if(remaining < 0)
                  remaining = 0;
              }
              tm = localtime(&t_when);
	      strftime(buf, sizeof(buf), format_str(u, DATE_FORMAT), tm);
              if(remaining == -1)
                send_lang(u,s, CS_INFO_SUSPENDED_X_FOREVER, buf);
              else              
                send_lang(u,s, CS_INFO_SUSPENDED_X_X, buf, remaining);
              send_lang(u,s, CS_INFO_SUSPENDED_REASON, row[2]);
            }
            sql_free(res);
          }
          send_lang(u, s, CHAN_INFO_TAIL);      
        }
      CloseCR(cr);
    }
}

