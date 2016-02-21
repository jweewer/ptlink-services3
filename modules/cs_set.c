/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2006 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************  

  Description: chanserv set command

*/

#include "module.h"
#include "chanserv.h"
#include "nickserv.h"  /* need IsAuthenticated() */
#include "encrypt.h"
#include "chanrecord.h"
#include "nsmacros.h"
#include "cs_role.h" /* we need P_SET */
#include "ns_group.h" /* we need the is_sadmin() */
#include "my_sql.h"
#include "dbconf.h"
/* lang files */
#include "lang/common.lh"
#include "lang/cscommon.lh"
#include "lang/cs_set.lh"

SVS_Module mod_info =
/* module, version, description */
{"cs_set", "2.3", "chanserv set/sset command" };

/* Change Log
  2.3   #80 Chanserv sset on entrymsg now does not show only the first word
        #81 Chanserv sset with mlock now also acepts extra parameters    
  2.2 - 0000334: review code to use local bot when possible
  2.1 - 0000295: mlock support for +k,+l,+f
  2.0 - 0000278: secureops option on chanserv
        0000265: remove nickserv cache system
        0000261: mlock option
	0000281: no auth nick can't use chanserv
  1.2 - 0000258: chanserv topiclock option
  1.1 - 0000246: help display with group filter
*/
  
/* external functions we need */
ServiceUser* (*chanserv_suser)(void);
u_int32_t (*find_group)(char *name);

int DisableNickNickSecurityCode = 0; 
int ChanServNeedsAuth = 0;

MOD_REQUIRES
  MOD_FUNC(dbconf_get)
  MOD_FUNC(chanserv_suser)
  MOD_FUNC(role_with_permission)  
  MOD_FUNC(is_sadmin)
  MOD_FUNC(find_group)
MOD_END

/* internal functions */
void set_command(IRC_User *u, IRC_User *s, ChanRecord *cr, char *option, char *value, int is_sset);

void cs_set(IRC_User *s, IRC_User *u);
void cs_sset(IRC_User *s, IRC_User *u); /* sadmin set */

/* local variables */
ServiceUser* csu;
int cs_log;

/* Remote config */
static int NeedsAuth;
static int NickSecurityCode;
DBCONF_REQUIRES
  DBCONF_GET("chanserv", NeedsAuth)
  DBCONF_GET("nickserv", NickSecurityCode)
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
  suser_add_cmd(csu, "SET", cs_set, SET_SUMMARY, SET_HELP);
  suser_add_cmd_g(csu, "SSET", cs_sset, SSET_SUMMARY, SSET_HELP,
    find_group("Admin"));
  /* we take care of the mlock, lets set the mlocker here */
  irc_SetChanMlocker(csu->u);
  return 0;
}

void mod_unload(void)
{
  suser_del_mod_cmds(csu, &mod_info);
}

#define FLAG_SET(x,y) \
  { \
    if(IsNull(value)) \
      send_lang(u, s, VALUE_ON_OR_OFF); \
    else \
    if(strcasecmp(value,"on") == 0) \
      { \
        log_log(cs_log, mod_info.name, "%s %s %s %s %s", \
          u->nick, is_sset ? "SSET" : "SET", cr->name, y, value); \
        cr->flags |= (x); \
        UpdateCR(cr); \
        send_lang(u, s, OPTION_X_ON, (y)); \
      } else \
    if(strcasecmp(value,"off")  == 0) \
      { \
        log_log(cs_log, mod_info.name, "%s %s %s %s %s", \
          u->nick, is_sset ? "SSET" : "SET", cr->name, y, value); \
        cr->flags &= ~(x); \
        UpdateCR(cr); \
        send_lang(u, s, OPTION_X_OFF, (y)); \
      } else \
      send_lang(u, s, VALUE_ON_OR_OFF); \
  }

#define STRING_SET(x,y,z) \
  { \
    FREE((x)); \
    if(IsNull(value)) \
      { \
        send_lang(u, s, (y)); \
        log_log(cs_log, mod_info.name, "%s %s %s %s", \
          u->nick, is_sset ? "SSET" : "SET", cr->name, option); \
      } \
    else \
      { \
        log_log(cs_log, mod_info.name, "%s %s %s %s %s", \
          u->nick, is_sset ? "SSET" : "SET", cr->name, option, value); \
        (x) = strdup(value); \
        send_lang(u, s, (z), value); \
      } \
    UpdateCR(cr); \
  }  
  
/* handles a set command */
void set_command(IRC_User *u, IRC_User *s, ChanRecord *cr, char *option, char *value, int is_sset)
{
  
  if(strcasecmp(option,"URL") == 0)
    STRING_SET(cr->url, URL_UNSET, URL_CHANGED_TO_X)
  else if(strcasecmp(option,"ENTRYMSG") == 0)
    STRING_SET(cr->entrymsg, ENTRYMSG_UNSET, ENTRYMSG_CHANGED_TO_X)    
  else if(strcasecmp(option,"DESC") == 0)
    STRING_SET(cr->cdesc, DESC_UNSET, DESC_CHANGED_TO_X)
  else if(strcasecmp(option,"PRIVATE") == 0)
    FLAG_SET(CFL_PRIVATE, "PRIVATE")
  else if(strcasecmp(option,"OPNOTICE") == 0)
    FLAG_SET(CFL_OPNOTICE, "OPNOTICE")
  else if(strcasecmp(option,"RESTRICTED") == 0)
    FLAG_SET(CFL_RESTRICTED, "RESTRICTED")
  else if(strcasecmp(option,"TOPICLOCK") == 0)
    FLAG_SET(CFL_TOPICLOCK, "TOPICLOCK")
  else if(strcasecmp(option,"SECUREOPS") == 0)
    FLAG_SET(CFL_SECUREOPS, "SECUREOPS")    
  else if(strcasecmp(option,"MLOCK") == 0)
  {
    int r = 0;
    IRC_Chan *chan;
    if(value) /* lets remove duplicates from the string */
    {
      char *c = value;
      char *d;
      char *save = save = strchr(value, ' ');
      if(save)
        *save = '\0';
      while(*c && *c!=' ') /* don't check dup letters on params */
      {
        while((d = strchr(c+1, *c)))
        {
          int i = strlen(value)-1;
          *d = value[i];
          value[i] = '\0';
        }
        ++c;
      }
    if(save)
      *save = ' ';
    }

    chan = irc_FindChan(cr->name);
    r = irc_ChanMLockSet(s, chan, value ? value : "");
    switch(r)
    {
      case -1: send_lang(u, s, INVALID_MLOCK_LETTER); break;
      case -2: send_lang(u, s, MISSING_MLOCK_PARAMETER); break;      
      case -3: send_lang(u, s, INVALID_MLOCK_PARAMETER); break;
      case -4: send_lang(u, s, EXTRA_MLOCK_PARAMETER); break;
      case -5: send_lang(u, s, INVALID_MLOCK_CONFLICT); break;

      default:
        STRING_SET(cr->mlock, MLOCK_UNSET, MLOCK_CHANGED_TO_X)
        if(chan)
          irc_ChanMLockApply(chan->local_user ? chan->local_user : s, chan);
      break;
    }
  }
  else if(strcasecmp(option,"FOUNDER") == 0)
    {
      u_int32_t snid = u->snid;
      char* nick_sec = NULL;
      if(!is_sset && cr->founder != snid)
        {
          send_lang(u, s, ONLY_FOUNDER_X, cr->name);
          return;
        }    
      if(sql_singlequery("SELECT securitycode FROM nickserv_security WHERE snid=%d",
      	u->snid))
      {
      	if(sql_field(0))
        {
          nick_sec = malloc(16);
          memcpy(nick_sec, hex_bin(sql_field(0)), 16);
        }
      } 
      if(!is_sset && NickSecurityCode && nick_sec && IsAuthenticated(u))
        {
          char* securitycode = strtok(NULL, " ");
          if(IsNull(securitycode))
            {
              send_lang(u, s, SET_FOUNDER_SECURITY_REQUIRED);
	      FREE(nick_sec);
              return;
            }
          else if(memcmp(nick_sec, encrypted_password(securitycode), 16) != 0)
            {
              send_lang(u, s, INVALID_SECURITY_CODE);
	      FREE(nick_sec);
              return;
            }
        }
      FREE(nick_sec);
      /* syntax validation */ 
      if(IsNull(value))
        send_lang(u, s, SET_FOUNDER_SYNTAX);
      /* check requirements */
      else if((snid = nick2snid(value)) == 0)
        send_lang(u, s, NICK_X_NOT_REGISTERED, value);
      else
        {
	  if(snid == cr->successor)
            send_lang(u, s, ALREADY_SUCCESSOR);          
          else if(snid == cr->founder)
            send_lang(u, s, ALREADY_FOUNDER);
          else 
	    {
	      cr->founder = snid;
	      UpdateCR(cr);
	      send_lang(u, s, FOUNDER_X_CHANGED_X, cr->name, value);
	    }
        }
    }
  else if(strcasecmp(option,"SUCCESSOR") == 0)
    {
      char *nick_sec = NULL;
      u_int32_t snid = u->snid;
      if(!is_sset && cr->founder != snid)
      	{
	  send_lang(u, s, ONLY_FOUNDER_X, cr->name);
	  return;
	}   
      if(sql_singlequery("SELECT securitycode FROM nickserv_security WHERE snid=%d",
      	u->snid))
      {
      	if(sql_field(0))
        {
          nick_sec = malloc(16);
          memcpy(nick_sec, hex_bin(sql_field(0)), 16);
        }
      } 
      if(!is_sset && NickSecurityCode && nick_sec && IsAuthenticated(u))
        {
          char* securitycode = strtok(NULL, " ");
          if(IsNull(securitycode))
            {
              send_lang(u, s, SET_SUCCESSOR_SECURITY_REQUIRED);
	      FREE(nick_sec);
              return;
            }
          else if(memcmp(nick_sec, encrypted_password(securitycode), 16) != 0)
            {
              send_lang(u, s, INVALID_SECURITY_CODE);
	      FREE(nick_sec);
              return;
            }
        }
      FREE(nick_sec); 

    if(IsNull(value))
      send_lang(u, s, SET_SUCCESSOR_SYNTAX);
    else if((snid = nick2snid(value)) == 0)
      send_lang(u, s, NICK_X_NOT_REGISTERED, value);
    else
    {
    	if(snid == cr->founder)
    	  send_lang(u, s, ALREADY_FOUNDER);
	else if(snid == cr->successor)
	  send_lang(u, s, ALREADY_SUCCESSOR);
	else
	  {
	    cr->successor =snid;
	    UpdateCR(cr);
	    send_lang(u, s, SUCCESSOR_X_CHANGED_X, cr->name, value);
	  }
    }
  }
  else if(strcasecmp(option,"EMAIL") == 0)
    {
      if(value && !is_email(value))
        {
          send_lang(u, s, INVALID_EMAIL);
          return ;
        }
      FREE(cr->email);
      if(IsNull(value))
        {
          cr->email = NULL;
          send_lang(u, s, EMAIL_UNSET);
        }
      else
        {
          cr->email = strdup(value);
          send_lang(u, s, EMAIL_CHANGED_TO_X, value);
        }
      UpdateCR(cr);
    }
  else if(is_sset == 0)
    send_lang(u, s, UNKNOWN_OPTION_X, option);
  else if(strcasecmp(option,"NOEXPIRE") == 0)
    FLAG_SET(CFL_NOEXPIRE, "NOEXPIRE")     
  else
    send_lang(u, s, UNKNOWN_OPTION_X, option);
}

#undef STRING_SET
#undef FLAG_SET
 
/* s = service the command was sent to
   u = user the command was sent from */
void cs_set(IRC_User *s, IRC_User *u)
{
  u_int32_t source_snid;
  ChanRecord *cr;
  char *option, *value;
  char *chname;

  CHECK_IF_IDENTIFIED_NICK  
  chname = strtok(NULL, " ");
  option = strtok(NULL, " ");
  if(!IsNull(option) && (
    (strcasecmp(option, "DESC") == 0) ||
    (strcasecmp(option, "ENTRYMSG") == 0) ||
    (strcasecmp(option, "MLOCK") == 0)
    ))
    value = strtok(NULL, "");
  else
    value = strtok(NULL, " ");
            
  if(ChanServNeedsAuth && !IsAuthenticated(u))
    send_lang(u, s, NEEDS_AUTH_NICK);
  else
  if(IsNull(chname) || IsNull(option))
    send_lang(u, s, CHAN_SET_SYNTAX);
  else
  if((cr = OpenCR(chname)) == NULL)
    send_lang(u, s, CHAN_X_NOT_REGISTERED, chname);
  else
    {
      if(role_with_permission(cr->scid, source_snid, P_SET) == 0)
          send_lang(u, s, NO_SET_PERM_ON_X, chname); 
      else /* everything is valid lets check the auth */
        {
          set_command(u, s, cr, option, value, 0);
        }
      CloseCR(cr);
    }
}

/* s = service the command was sent to
   u = user the command was sent from */
void cs_sset(IRC_User *s, IRC_User *u)
{
  ChanRecord *cr;
  u_int32_t source_snid;
  char *chname, *option, *value;
  
  chname = strtok(NULL, " ");
  option = strtok(NULL, " ");  
  
  if(!IsNull(chname) && !IsNull(option) && (
    (strcasecmp(option, "DESC") == 0) ||
    (strcasecmp(option, "ENTRYMSG") == 0) ||
    (strcasecmp(option, "MLOCK") == 0)
    ))
    value = strtok(NULL, "");
  else
    value = strtok(NULL, " ");

  CHECK_IF_IDENTIFIED_NICK
  
  if(IsNull(chname) || IsNull(option))
    send_lang(u, s, CHAN_SSET_SYNTAX);
  else
  if(!is_sadmin(source_snid))
    send_lang(u, s, ONLY_FOR_SADMINS);
  else  
  if((cr = OpenCR(chname)) == NULL)
    send_lang(u, s, CHAN_X_NOT_REGISTERED, chname);
  else
    {
      set_command(u, s, cr, option, value, 1);
      CloseCR(cr);
    }
}

