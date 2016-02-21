/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************

  Description: photo managment module

 *  $Id: ns_photo.c,v 1.6 2005/10/18 16:25:06 jpinto Exp $
*/

#include "module.h"
#include "dbconf.h"
#include "my_sql.h"
#include "path.h"
#include "lang/ns_photo.lh"

SVS_Module mod_info =
 /* module, version, description */
{"ns_photo", "3.0",  "user photo module" };

#define DB_VERSION      2

/* Change Log 
  3.0 - 0000305: foreign keys for data integrity
          Added ns_photo.2.sql changes
  2.0 - 0000265: remove nickserv cache system
*/

/** functions and events we require **/
ServiceUser* (*nickserv_suser)(void);
int e_nick_info;

MOD_REQUIRES
  MOD_FUNC(dbconf_get_or_build)
  MOD_FUNC(e_nick_info)
  MOD_FUNC(nickserv_suser)  
MOD_END

/** Internal functions declaration **/
int ev_ns_photo_nick_info(IRC_User* user, u_int32_t *snid);

    
/** Local dconf settings **/
static char *BaseURL;

/*
 * List of dbconf items we provide
 */
dbConfItem dbconf_provides[] =
{
  DBCONF_WORD(BaseURL, "http://www.pt-link.net/services/", 
    "Base URL for the web services")
  {NULL}
};

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

/** Local variables **/
ServiceUser* nsu;
    
/** load code **/
int mod_load(void)
{
  int r;
  r = sql_check_inst_upgrade(mod_info.name, DB_VERSION, NULL);
  if(r < 0)
    return -2;
        
  nsu = nickserv_suser();
  mod_add_event_action(e_nick_info, (ActionHandler) ev_ns_photo_nick_info);
  
  return 0;
}
    
/** unload code **/
void mod_unload(void)
{
}
    
/** internal functions implementation starts here **/
int ev_ns_photo_nick_info(IRC_User* user, u_int32_t* snid)
{
  MYSQL_RES* res;
  MYSQL_ROW row;
  char buf[128];
  
  res = sql_query("SELECT id FROM ns_photo WHERE snid=%d AND status='P'", *snid);
  if((row = sql_next_row(res)))
    {
      snprintf(buf, sizeof(buf), "%sview_photo.php?id=%d", 
        BaseURL, atoi(row[0]));
      send_lang(user, nsu->u, PHOTO_URL_X, buf);
    }
  sql_free(res);

  return 0;
}

/* End of module */
