/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            
 **********************************************************************

  Description: Memoserv cancel command

 *  $Id: ms_cancel.c,v 1.5 2005/10/11 16:13:06 jpinto Exp $
*/

#include "module.h"
#include "memoserv.h"
#include "my_sql.h"
#include "nsmacros.h"
/* lang files */
#include "lang/common.lh"
#include "lang/ms_cancel.lh"

SVS_Module mod_info =
 /* module, version, description */
{"ms_cancel", "2.0",  "memoserv cancel command" };
/* Change Log
  2.0 - 0000265: remove nickserv cache system 
  1.1 -	0000266: ms_cancel deletes unread memos from all users  
*/

/** functions and events we require **/
/* void (*FunctionPointer)(void);*/
ServiceUser* (*memoserv_suser)(void);

MOD_REQUIRES
  MOD_FUNC(memoserv_suser)
MOD_END

/** Internal functions declaration **/

/* void internal_function(void); */
void ms_cancel(IRC_User *s, IRC_User *u);

    
/** Local variables **/
/* int my_local_variable; */
ServiceUser* msu;
    
/** load code **/
int mod_load(void)
{
  msu = memoserv_suser();
  suser_add_cmd(msu, "CANCEL", ms_cancel, CANCEL_SUMMARY, CANCEL_HELP);
  return 0;
}
    
/** unload code **/
void mod_unload(void)
{
  suser_del_mod_cmds(msu, &mod_info);     
}
    
/** internal functions implementation starts here **/
void ms_cancel(IRC_User *s, IRC_User *u)
{
  u_int32_t source_snid;
  u_int32_t snid;
  char* target;
  int count;
  
  /* status validation */
  CHECK_IF_IDENTIFIED_NICK
  
  target = strtok(NULL, " ");
  
  /* syntax validation */
  if(IsNull(target))
    send_lang(u, s, CANCEL_SYNTAX);
  /* check requirements */
  else if((snid = nick2snid(target)) == 0)
    send_lang(u, s, NICK_X_NOT_REGISTERED, target);
  /* privileges validation */
  else 
    {
      count = sql_execute("DELETE FROM memoserv"
        " WHERE sender_snid=%d AND owner_snid=%d AND (flags & %d)", 
        source_snid, snid, MFL_UNREAD);
      send_lang(u, s, X_MEMOS_WHERE_CANCELED, count);
    }
}
    
/* End of module */
