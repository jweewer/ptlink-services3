/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            
 **********************************************************************

  Description: Memoserv cancel command

 *  $Id: ms_save.c,v 1.5 2005/10/11 16:13:06 jpinto Exp $
*/

#include "module.h"
#include "memoserv.h"
#include "my_sql.h"
#include "nsmacros.h"
/* lang files */
#include "lang/common.lh"
#include "lang/ms_save.lh"

SVS_Module mod_info =
 /* module, version, description */
{"ms_save", "2.0",  "memoserv cancel command" };
/* Change Log
  2.0 - 0000265: remove nickserv cache system 
  1.1 -	0000266: ms_save deletes unread memos from all users  
*/

/** functions and events we require **/
/* void (*FunctionPointer)(void);*/
ServiceUser* (*memoserv_suser)(void);

MOD_REQUIRES
  MOD_FUNC(memoserv_suser)
MOD_END

/** Internal functions declaration **/

/* void internal_function(void); */
void ms_save(IRC_User *s, IRC_User *u);

    
/** Local variables **/
/* int my_local_variable; */
ServiceUser* msu;
    
/** load code **/
int mod_load(void)
{
  msu = memoserv_suser();
  suser_add_cmd(msu, "SAVE", ms_save, MS_SAVE_SUMMARY, MS_SAVE_HELP);
  return 0;
}
    
/** unload code **/
void mod_unload(void)
{
  suser_del_mod_cmds(msu, &mod_info);     
}
    
/** internal functions implementation starts here **/
void ms_save(IRC_User *s, IRC_User *u)
{
  u_int32_t source_snid;
  char* sid;
  u_int32_t id = 0;
  
  /* status validation */
  CHECK_IF_IDENTIFIED_NICK
  
  sid = strtok(NULL, " ");
  if(sid)
    id = atoi(sid);

  /* syntax validation */
  if(id == 0)
    send_lang(u, s, MS_SAVE_SYNTAX);
  /* check requirements */
  else if(sql_singlequery("SELECT id FROM memoserv"
    " WHERE owner_snid=%d and id=%d", source_snid, id) == 0)
    send_lang(u, s, NO_SUCH_MEMO_X, id);
  /* execute operation */
  else 
  {  
    sql_execute("UPDATE memoserv SET flags = (flags | %d)"
      " WHERE owner_snid=%d and id=%d", MFL_SAVED,  source_snid, id);
    send_lang(u, s, MS_SAVE_SAVED_X, id);
  }
}
    
/* End of module */
