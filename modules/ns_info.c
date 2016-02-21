/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************  

  Description: nickserv info command
                                                                                
 *  $Id: ns_info.c,v 1.10 2005/10/18 16:25:06 jpinto Exp $
*/
#include "module.h"
#include "nickserv.h"
#include "ns_group.h"  /* is_soper */
#include "my_sql.h"
#include "nsmacros.h"
#include "lang/common.lh"
#include "lang/nickserv.lh"
#include "lang/ns_info.lh"

SVS_Module mod_info =
/* module, version, description */
{"ns_info", "2.3", "nickserv info command" };

/* Change Log
  2.3 - #10 : add nickserv suspensions
  2.2 - 0000327: sadmins SSET nick vhost to set a virtual hostname
  2.1 - 0000306: nickserv info shows nick as private everytime it is online
      - 0000304: nickserv last seen only on info when nick is offline
      - 0000298: nickserv info now displays userhost
      - 0000303: expire time not displayed if nick has noexpire flag
  2.0 - 0000274: empty options field on ns info for authenticated nicks
      - 0000265: remove nickserv cache system
      - when no target is given assume is "self" info
  1.1 - 0000256: ago time split in larger time units
*/
  
/* external functions we need */
ServiceUser* (*nickserv_suser)(void);
int e_nick_info;

MOD_REQUIRES
  MOD_FUNC(e_nick_info)
  MOD_FUNC(nickserv_suser)
  MOD_FUNC(is_soper)
MOD_END

/* available commands from module */
void ns_info(IRC_User *s, IRC_User *u);

/* Local settings */
int AgeBonusPeriod = 0;
int AgeBonusValue = 0;
int NSExpire = 0;

/* internal functions */
char* ago_time(time_t t, IRC_User *u);

/* Local variables */
ServiceUser* nsu;

int mod_load(void)
{
  nsu = nickserv_suser();  
  suser_add_cmd(nsu, "INFO", (void*) ns_info, INFO_SUMMARY, INFO_HELP);  
  return 0;
}

void
mod_unload(void)
{
  /* delete all our commands*/
  suser_del_mod_cmds(nsu, &mod_info);
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
void ns_info(IRC_User *s, IRC_User *u)
{
  char buf[64];
  struct tm *tm;
  char *target; 
  char *langstr = NULL;  
  int sadmin = 0;
  u_int32_t source_snid = u->snid;    
  
  target = strtok(NULL, " ");
  
  /* assume "self" info */
  if(IsNull(target) && u->snid)
    target = u->nick;
  
  if(IsNull(target))
    send_lang(u, s, NICK_INFO_SYNTAX);
  else if(sql_singlequery("SELECT "
    "snid,"
    "flags,"
    "status,"
    "t_reg,"
    "t_ident,"
    "t_seen,"
    "t_expire,"
    "lang,"
    "email,"
    "vhost"			
    " FROM nickserv WHERE nick=%s", sql_str(irc_lower_nick(target))
      ) == 0)
    send_lang(u, s, NICK_X_NOT_REGISTERED, target);
  else /* everything is valid lets check the auth */
    {
      int c = 0;
      int privilege;
      u_int32_t snid = sql_field_i(c++);
      u_int32_t flags = sql_field_i(c++);
      u_int32_t status = sql_field_i(c++);
      time_t t_reg  = sql_field_i(c++);
      time_t t_ident  = sql_field_i(c++);
      time_t t_seen = sql_field_i(c++);
      time_t t_expire = sql_field_i(c++);
      int lang = sql_field_i(c++);
      char *email = sql_field(c++);
      char *vhost = sql_field(c++);      
      sadmin = (is_soper(source_snid) != 0);
      privilege = (sadmin || (source_snid == snid));
      send_lang(u, s, NICK_INFO_HEADER);
      send_lang(u, s, NICK_NICK_X_X, target, 
        (status & NST_ONLINE) ? "(ONLINE)" : "");
      if((flags & NFL_PRIVATE) && !privilege)
        send_lang(u, s, NICK_INFO_PRIVATE, target);
      else
        {
          if(sadmin)
            send_lang(u, s, NICK_SNID, snid);	 
	        tm = localtime(&t_reg);
          strftime(buf, sizeof(buf), format_str(u, DATE_FORMAT), tm);
          send_lang(u, s, NICK_REGDATE_X_X, buf, ago_time(t_reg, u));
          tm = localtime(&t_ident);
          strftime(buf, sizeof(buf), format_str(u, DATE_FORMAT), tm);
          send_lang(u, s, NICK_IDDATE_X_X, buf, ago_time(t_ident, u));          
          if(!(status & NST_ONLINE))
	  {
	    tm = localtime(&t_seen);
            strftime(buf, sizeof(buf), format_str(u, DATE_FORMAT), tm);
            send_lang(u, s, NICK_SEENDATE_X_X, buf, ago_time(t_seen, u));          
          }
	  if(!(flags & (NFL_NOEXPIRE|NFL_SUSPENDED)))
	  {
	    tm = localtime(&t_expire);
            strftime(buf, sizeof(buf), format_str(u, DATE_FORMAT), tm);
            send_lang(u, s, NICK_EXPIRES_X, buf);                    
          }
      index2lang(lang, langstr);
      if(langstr)
        send_lang(u, s, NICK_LANGUAGE, langstr);
          if(privilege || !(flags & NFL_HIDEEMAIL))
            {
              if(!IsNull(email))
                {
                  if(flags & NFL_AUTHENTIC)
                    send_lang(u, s, NICK_EMAIL, email);
                  else
                    send_lang(u, s, NICK_EMAIL_NOAUTH, email);
                }
            }
          if((flags &~ NFL_AUTHENTIC) && privilege)
            send_lang(u, s, NICK_OPTIONS_X, 
              mask_string(nick_options_mask, flags));
          if(vhost && privilege)
            send_lang(u, s, NICK_VHOST_X, vhost);
          if(flags & NFL_SUSPENDED)
          {
            MYSQL_RES *res;
            MYSQL_ROW row;
            res = sql_query("SELECT t_when, duration, reason "
              "FROM nickserv_suspensions WHERE snid=%d", snid);
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
                send_lang(u,s, NS_INFO_SUSPENDED_X_FOREVER, buf);
              else              
                send_lang(u,s, NS_INFO_SUSPENDED_X_X, buf, remaining);
              send_lang(u,s, NS_INFO_SUSPENDED_REASON, row[2]);
            }
            sql_free(res);
          }
        }
      mod_do_event(e_nick_info, u, &snid);
      send_lang(u, s, NICK_INFO_TAIL);        
    }
}

