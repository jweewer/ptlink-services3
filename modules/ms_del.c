/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            
 **********************************************************************

  Description: Memoserv del command

 *  $Id: ms_del.c,v 1.5 2005/10/11 16:13:06 jpinto Exp $
*/

#include "module.h"
#include "memoserv.h"
#include "my_sql.h"
#include "nsmacros.h"
/* lang files */
#include "lang/common.lh"
#include "lang/ms_del.lh"

SVS_Module mod_info =
 /* module, version, description */
{"ms_del", "2.1",  "memoserv del command" };
/* Change Log
  2.1 - 0000316: MemoServ DEL ALL to delete all memos
  2.0 - 0000265: remove nickserv cache system
*/

/** functions and events we require **/
/* void (*FunctionPointer)(void);*/
ServiceUser* (*memoserv_suser)(void);

MOD_REQUIRES
  MOD_FUNC(memoserv_suser)
MOD_END

/** functions and events we provide **/
/* void my_function(void); */

/** Internal functions declaration **/

/* void internal_function(void); */
void ms_del(IRC_User *s, IRC_User *u);
    
/** Local variables **/
/* int my_local_variable; */
ServiceUser* msu;
    
/** load code **/
int mod_load(void)
{
  msu = memoserv_suser();
  suser_add_cmd(msu, "DEL", ms_del, DEL_SUMMARY, DEL_HELP);
  return 0;
}
    
/** unload code **/
void mod_unload(void)
{
  suser_del_mod_cmds(msu, &mod_info);
}

/** internal functions implementation starts here **/
void ms_del(IRC_User *s, IRC_User *u)
{
  u_int32_t source_snid;
  char* memolist;
  u_int32_t id;
  char where[32];
  int is_all = 0;
  
  /* status validation */
  CHECK_IF_IDENTIFIED_NICK
  
  memolist = strtok(NULL, " ");
    
  if(memolist)
  { 
    if(strcasecmp(memolist, "ALL"))
      snprintf(where, sizeof(where),
        "owner_snid=%d AND id=%d", source_snid, atoi(memolist));
    else
    {
      snprintf(where, sizeof(where),
        "owner_snid=%d", source_snid);
      is_all = 1;
    }
  }
  
  /* syntax validation */
  if(!is_all && (IsNull(memolist) || ((id = atoi(memolist)) == 0)))
    send_lang(u, s, DEL_SYNTAX);
  /* check requirements */
  else if(sql_singlequery("SELECT id FROM memoserv"
    " WHERE %s", where) == 0)
    send_lang(u, s, NO_SUCH_MEMO_X, id);
  /* execute operation */
  else if(sql_execute("DELETE FROM memoserv WHERE %s", where))
  {
    if(is_all)
      send_lang(u, s, ALL_MEMOS_DELETED);
    else
      send_lang(u, s, MEMO_X_WAS_DELETED, id);
  }
  else
    send_lang(u, s, UPDATE_FAIL); 
}
    
/* End of module */
