/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2006 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            
 **********************************************************************

  Description: Memoserv send command

 *  $Id: ms_send.c,v 1.8 2005/10/18 16:25:06 jpinto Exp $
*/

#include "module.h"
#include "memoserv.h"
#include "nickserv.h" /* need IsAuthenticated() */
#include "my_sql.h"
#include "dbconf.h"
#include "email.h"
#include "nsmacros.h"
/* lang files */
#include "lang/common.lh"
#include "lang/ms_send.lh"

SVS_Module mod_info =
 /* module, version, description */
{"ms_send", "3.0",  "memoserv send command" };

/* Change Log
  3.0 - #69: memoserv expiration with save option
        #5: split memoserv with a memoserv options table
  2.0 - 0000265: remove nickserv cache system
*/

/** functions and events we require **/
/* void (*FunctionPointer)(void);*/
ServiceUser* (*memoserv_suser)(void);

MOD_REQUIRES
  DBCONF_FUNCTIONS
  MEMOSERV_FUNCTIONS
  EMAIL_FUNCTIONS
MOD_END

/** Internal functions declaration **/
/* void internal_function(void); */
void ms_send(IRC_User *s, IRC_User *u);
u_int32_t insert_memo(char* sender_name, u_int32_t sender_snid, u_int32_t owner_snid, char* message, u_int32_t flags);

const static char* c_forward_email = \
  "From: \"\%from_name\%\" <\%from\%>\n" \
  "To: \"\%nick\%\" <\%email\%>\n" \
  "Subject: \%subject\%\n\n" \
  "\%message\%";
  
int NickSecurityCode;


DBCONF_REQUIRES
  DBCONF_GET("nickserv", NickSecurityCode)
DBCONF_END

int mod_rehash(void)
{
  if(dbconf_get(dbconf_requires) < 0 )
  {
    errlog("Error reading dbconf!");
    return -1;
  }
  return 0;
} 

/** Local variables **/
/* int my_local_variable; */
ServiceUser* msu;
static int ms_log = -1;
static char* forward_email;

/** load code **/
int mod_load(void)
{
  msu = memoserv_suser();
  ms_log = log_handle("memoserv");
  suser_add_cmd(msu, "SEND", ms_send, SEND_SUMMARY, SEND_HELP);
  forward_email = strdup(c_forward_email);
  return 0;
}
    
/** unload code **/
void mod_unload(void)
{
  suser_del_mod_cmds(msu, &mod_info);
}

/** internal functions implementation starts here **/
void ms_send(IRC_User *s, IRC_User *u)
{
  u_int32_t source_snid;
  u_int32_t snid;
  u_int32_t id;
  char* target;
  char* message;
  int mcount = 0;
  int maxmemos;
  int bquota;
  u_int32_t flags;
  u_int32_t memo_flags = 0;
  
  /* status validation */
  CHECK_IF_IDENTIFIED_NICK
  
  target = strtok(NULL, " ");
  message =  strtok(NULL, "");
  
  if(target && (snid = nick2snid(target)) == 0)
  {
    send_lang(u, s, NICK_X_NOT_REGISTERED, target);
    return;
  }
  
  /* we need to read memo options first */
  memoserv_get_options(snid, &maxmemos, &bquota, &flags);
  if(flags && MOFL_AUTOSAVE)
    memo_flags = MFL_SAVED;
  if(NickSecurityCode  && !IsAuthenticated(u))
    send_lang(u, s, NEEDS_AUTH_NICK);
  else 
  /* syntax validation */  
  if(IsNull(target) || IsNull(message))
    send_lang(u, s, SEND_SYNTAX);
  /* check maxmemos */
  else 
  if(flags & MOFL_NOMEMOS)
    send_lang(u, s, MS_SEND_NOMEMOS);
  else
  if((mcount = memos_count(snid)) >= maxmemos)
    send_lang(u, s, MAX_MEMOS_REACHED_X_X, target, maxmemos);  
#if 0
  /* check buddy quota for non buddies */
  else 
  if(is_buddy && (maxmemos-mcount <= bquota) && !is_buddy(snid, source_snid))
    send_lang(u, s, MAX_MEMOS_REACHED_X_X, target, maxmemos-bquota);
#endif  
  /* execute operation */
  else 
  if((id = insert_memo(u->nick, source_snid, snid, message, memo_flags)) > 0)
  {
    IRC_User* tu;
    send_lang(u, s, SENT_MEMO_TO_X, target);
    tu = irc_FindUser(target);
    if(tu && tu->snid) /* target is online and identified */
    {
      char memoprev[MEMOPREVMAX+1];
      snprintf(memoprev, MEMOPREVMAX, "%s", message);
      send_lang(tu, s, YOU_GOT_MEMO_FROM_X_X_NUM_X,
        u->nick, memoprev, id);
    }
    if(flags &  MOFL_FORWARD)
    {
      MYSQL_RES *res;
      MYSQL_ROW row;
      res = sql_query("SELECT email, lang FROM nickserv WHERE snid=%d", snid);
      if(res && (row = sql_next_row(res)))
      {
        char* email = row[0];
        int lang = atoi(row[1]);
        email_init_symbols();
        email_add_symbol("nick",target);
        email_add_symbol("email", email);
        email_add_symbol("message", message);
        email_add_symbol("subject",
          lang_str_l(lang, MS_SEND_SUBJECT_X, u->nick));
        if(email_send(forward_email) < 0)
        {
          log_log(ms_log, mod_info.name, "Error sending forward email to %s by %s",
            email, irc_UserMask(u));
        }
      }
      sql_free(res);
    }
  }
  else
    send_lang(u, s, UPDATE_FAIL);
}

/* inserts a memo on the memoserv table */
u_int32_t insert_memo(char* sender_name, u_int32_t sender_snid, u_int32_t owner_snid, char* message, u_int32_t flags)
{
  int r;
  u_int32_t max = 1;
  if((sql_singlequery("SELECT MAX(id) FROM memoserv"
    " WHERE owner_snid=%d ORDER BY id DESC LIMIT 1", owner_snid)  > 0) && sql_field(0))
      max = atoi(sql_field(0))+1;
  r = sql_execute("INSERT INTO memoserv VALUES(%d,"
    "%d, %d, %s, %d, %d, %s)",
    max, owner_snid, sender_snid, sql_str(sender_name), flags | MFL_UNREAD, 
    time(NULL), sql_str(message));
  return r ? max : 0;
}


/* End of module */

