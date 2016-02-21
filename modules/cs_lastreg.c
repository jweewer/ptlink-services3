/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************

  Description: shows last registered channels

 *  $Id: cs_lastreg.c,v 1.6 2005/10/18 16:25:06 jpinto Exp $
*/

#include "module.h"
#include "my_sql.h"
#include "chanserv.h"
#include "dbconf.h"
#include "lang/cs_lastreg.lh"

SVS_Module mod_info =
 /* module, version, description */
{"cs_lastreg", "1.1",  "last registered channels" };
/* Change Log
  1.1 -  0000257: unload module cs_lastreg crashes
*/
  
/** functions and events we require **/
ServiceUser* (*chanserv_suser)(void);
int e_chan_register;

MOD_REQUIRES
  MOD_FUNC(dbconf_get_or_build)
  MOD_FUNC(chanserv_suser)
  MOD_FUNC(e_chan_register)
MOD_END


/** Internal functions declaration **/
void ev_cs_lastreg_new_user(IRC_User* u, void *s);
int ev_cs_lastreg_chan_register(IRC_User *u, ChanRecord* cr);

    
/** Local config **/
static int DisplayCount;
DBCONF_PROVIDES
  DBCONF_INT(DisplayCount, "10",
    "Number of chans to display on the last registered list")
DBCONF_END

/** Local variables **/
static ServiceUser* csu;
static char **last_reg_list;

/* this is called before load and at services rehash */
int mod_rehash(void)
{
  if(dbconf_get_or_build(mod_info.name, dbconf_provides) < 0 )
  {
    errlog("Error reading dbconf!");
    return -1;
  }
  return 0;
}

/** load code **/
int mod_load(void)
{
  int i;
  MYSQL_RES* res;
  MYSQL_ROW row;

  csu = chanserv_suser();
  
  /* alocate memory to keep the chan list*/
  last_reg_list = malloc(DisplayCount * sizeof(char*));  
  res = sql_query("SELECT name FROM chanserv "
    "WHERE (flags & %d)=0 ORDER BY t_reg DESC "
    "LIMIT %d", CFL_PRIVATE, DisplayCount);
  i = 0;
  while((row = sql_next_row(res)) && i < DisplayCount)
    {
      last_reg_list[i++] = strdup(row[0]);
    }         
  sql_free(res);
  
  while(i < DisplayCount) /* just clear the remaining free items */
    last_reg_list[i++] = NULL;     
  
  /* Add event to dump list */ 
  irc_AddEvent(ET_NEW_USER, ev_cs_lastreg_new_user); /* new user */
  
  /* Add action to update list */
  mod_add_event_action(e_chan_register, (ActionHandler) ev_cs_lastreg_chan_register);
    
  return 0;
}
    
/** unload code **/
void mod_unload(void)
{
  int i;
  /* just free the list mem */
  for(i=0; i < DisplayCount; ++i)
    {
      FREE(last_reg_list[i]);
    }
  free(last_reg_list);
  irc_DelEvent(ET_NEW_USER, ev_cs_lastreg_new_user);
  return;
}
    
/** internal functions implementation starts here **/
void ev_cs_lastreg_new_user(IRC_User* u, void *s)
{
  int i;
  if(irc_IsUMode(u, UMODE_IDENTIFIED)) /* new user with +r from a netjoin */
    return;
  
  send_lang(u, csu->u, LAST_X_REGCHANS, DisplayCount);
  for(i=0; i < DisplayCount; ++i)
    if(last_reg_list[i])
      send_lang(u, csu->u, LAST_ITEM_X, last_reg_list[i]);
  send_lang(u, csu->u, LAST_TRAIL);
}

/* we need to update the last reg list here */
int ev_cs_lastreg_chan_register(IRC_User *u, ChanRecord* cr)
{
  int i;
  FREE(last_reg_list[DisplayCount-1]); /* free out item */
    
  for(i = DisplayCount-1; i > 0; --i)
    last_reg_list[i]=last_reg_list[i-1];
  last_reg_list[0] = strdup(cr->name); /* update the first item */  
  return 0;
}
    
/* End of Module */
