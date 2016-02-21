/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            
 **********************************************************************

  Description: Memoserv read command

 *  $Id: ms_read.c,v 1.5 2005/10/11 16:13:06 jpinto Exp $
*/

#include "module.h"
#include "memoserv.h"
#include "my_sql.h"
#include "nsmacros.h"
#include "dbconf.h"
/* lang files */
#include "lang/common.lh"
#include "lang/ms_read.lh"

SVS_Module mod_info =
 /* module, version, description */
{"ms_read", "2.1",  "memoserv read command" };

/* Change Log
  2.1 - #69, memoserv expiration with save option
  2.0 - 0000265: remove nickserv cache system
*/  

/** functions and events we require **/
/* void (*FunctionPointer)(void);*/
ServiceUser* (*memoserv_suser)(void);

MOD_REQUIRES
  MOD_FUNC(memoserv_suser)
  DBCONF_FUNCTIONS
MOD_END


/** Internal functions declaration **/
/* void internal_function(void); */
void ms_read(IRC_User *s, IRC_User *u);
void set_read(u_int32_t id, u_int32_t owner_snid);

/** Local variables **/
/* int my_local_variable; */
ServiceUser* msu;
static int ExpireTime = 0;

DBCONF_REQUIRES
  DBCONF_GET("memoserv", ExpireTime)
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
    
/** load code **/
int mod_load(void)
{
  msu = memoserv_suser();
  suser_add_cmd(msu, "READ", ms_read, READ_SUMMARY, READ_HELP);
  return 0;
}
    
/** unload code **/
void mod_unload(void)
{
  suser_del_mod_cmds(msu, &mod_info);
}
    
/** internal functions implementation starts here **/
void ms_read(IRC_User *s, IRC_User *u)
{
  u_int32_t source_snid;
  char* memolist;
  
  /* status validation */
  CHECK_IF_IDENTIFIED_NICK
  
  memolist = strtok(NULL, " ");
  
  /* syntax validation */
  if(IsNull(memolist))
    send_lang(u, s, READ_SYNTAX);
  /* check requirements */
  else if(sql_singlequery("SELECT id, flags, t_send, sender_name, message FROM memoserv"
    " WHERE owner_snid=%d AND id=%d", source_snid, atoi(memolist)
      ) == 0)
    send_lang(u, s, NO_SUCH_MEMO_X, atoi(memolist));
  /* execute operation */
  else 
    {
      char buf[64];
      struct tm *tm;
      u_int32_t id;
      u_int32_t flags;
      time_t t_send = atoi(sql_field(2));
      id = atoi(sql_field(0));
      flags = atoi(sql_field(1));
      tm = localtime(&t_send);
      strftime(buf, sizeof(buf), format_str(u, DATE_FORMAT), tm);      
      send_lang(u, s, MEMO_READ_X, atoi(sql_field(0)), buf, sql_field(3), sql_field(4));
      set_read(id, source_snid);
      if(ExpireTime && !(flags & MFL_SAVED))
        send_lang(u, s, MS_READ_WILL_EXPIRE_ON_X_X, 
        ((ExpireTime-(irc_CurrentTime-t_send))/(24*3600))+1, id);
    }
}

/* set a message as read */
void set_read(u_int32_t id, u_int32_t owner_snid)
{
  sql_execute("UPDATE memoserv SET FLAGS=FLAGS & %d WHERE owner_snid=%d AND id=%d",
    ~MFL_UNREAD, owner_snid, id);
}


/* End of module */
