/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************

  Description: bl managment
  
 *  $Id: ns_blist.c,v 1.4 2005/10/11 16:13:06 jpinto Exp $
*/

#include "module.h"
#include "my_sql.h"
#include "path.h"
#include "ns_group.h"
#include "lang/ns_blist.lh"

SVS_Module mod_info =
 /* module, version, description */
{"ns_blist", "1.0",  "nickserv blist module" };

#define DB_VERSION      1

/* Change Log 
  1.0 - initial version
*/

/** functions and events we require **/
ServiceUser* (*nickserv_suser)(void);
u_int32_t (*find_group)(char *name);


Module_Function mod_requires[] = 
{
  MOD_FUNC(nickserv_suser)  
  MOD_FUNC(is_sadmin)
  MOD_FUNC(find_group) 
  {NULL}
};

/** functions and events we provide **/
/* void my_function(void); */
int forbidden_email(char *email);

Module_Function mod_provides[] = 
{
  MOD_FUNC(forbidden_email)
  /* MOD_FUNC(FunctionName) 
  ...
  */
  {NULL}
};

/** Internal functions declaration **/
void ns_blist(IRC_User *s, IRC_User *u);

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
/*
  suser_add_cmd_g(nsu, "LIST", ns_blist, BLIST_SUMMARY, BLIST_HELP,
    find_group("Admin"));  
*/ 
  return 0;
}
    
/** unload code **/
void mod_unload(void)
{
  return;
}

/* checks if a given email is forbidden
   Returns:
        0 not forbidden
        !=0 is forbidden
 */
int forbidden_email(char *email)
{
  MYSQL_RES *res;
  MYSQL_ROW row;
  char *domain;
  int i;
  
  domain = strchr(email, '@');
  if(domain == NULL)
    return 0;
  
  res = sql_query("SELECT count(*) from ns_blist WHERE data=%s OR data=%s", 
    sql_str(email), sql_str(domain));
  if(res == NULL || ((row = sql_next_row(res)) == NULL))
    return 0;
  i = atoi(row[0]);
  sql_free(res);  
  return i;
}

/* End of module */
