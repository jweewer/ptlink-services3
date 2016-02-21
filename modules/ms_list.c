/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            
 **********************************************************************

  Description: Memoserv list command

 *  $Id: ms_list.c,v 1.4 2005/10/11 16:13:06 jpinto Exp $
*/

#include "module.h"
#include "memoserv.h"
#include "my_sql.h"
#include "nsmacros.h"
/* lang files */
#include "lang/common.lh"
#include "lang/ms_list.lh"

SVS_Module mod_info =
 /* module, version, description */
{"ms_list", "2.0",  "memoserv list command" };

/* Change Log
  2.0 - 0000265: remove nickserv cache system
*/
  
/** functions and events we require **/
/* void (*FunctionPointer)(void);*/
ServiceUser* (*memoserv_suser)(void);

Module_Function mod_requires[] = 
{
  /* MOD_FUNC(FunctionName) 
  ...
  */
  MOD_FUNC(memoserv_suser)
  {NULL}
};

/** functions and events we provide **/
/* void my_function(void); */

Module_Function mod_provides[] = 
{
  /* MOD_FUNC(FunctionName) 
  ...
  */
  {NULL}
};

/** Internal functions declaration **/
/* void internal_function(void); */
void ms_list(IRC_User *s, IRC_User *u);

    
/** Local variables **/
/* int my_local_variable; */
ServiceUser* msu;
    
/** load code **/
int mod_load(void)
{
  msu = memoserv_suser();
  suser_add_cmd(msu, "LIST", ms_list, LIST_SUMMARY, LIST_HELP);
  return 0;
}
    
/** unload code **/
void mod_unload(void)
{
  suser_del_mod_cmds(msu, &mod_info);
  return;
}
    
/** internal functions implementation starts here **/
void ms_list(IRC_User *s, IRC_User *u)
{
  MYSQL_RES* res;
  MYSQL_ROW row;
  u_int32_t source_snid;
  int rowc = 0;
  char memoprev[MEMOPREVMAX+1];
  
  /* status validation */
  CHECK_IF_IDENTIFIED_NICK
  
  res = sql_query("SELECT id, flags, t_send, sender_name, message FROM memoserv"
    " WHERE owner_snid=%d ORDER BY id", source_snid);
  if(res)
    rowc = mysql_num_rows(res);
  if(rowc == 0)
    send_lang(u, s, NO_MEMOS);
  else
    {
      send_lang(u, s, LIST_HEADER_X, rowc);
      while((row = sql_next_row(res)))
        {
          char buf[64];
          struct tm *tm;
          time_t t_send = atoi(row[2]);
          
          tm = localtime(&t_send);
          strftime(buf, sizeof(buf), format_str(u, DATE_FORMAT), tm);
          snprintf(memoprev, MEMOPREVMAX, "%s", row[4]);          
          if(atoi(row[1]) & MFL_SAVED)
            send_lang(u, s, LIST_FORMAT, 
              atoi(row[0]), (atoi(row[1]) & MFL_SAVED) ? "\002(S)\002" :" ", buf, row[3], memoprev);
          else
            send_lang(u, s, LIST_FORMAT, 
              atoi(row[0]), (atoi(row[1]) & MFL_UNREAD) ? "\002*\002" :" ", buf, row[3], memoprev);
        }
      send_lang(u, s, LIST_TAIL);
    }
  sql_free(res);
}
    
/* End of module */
