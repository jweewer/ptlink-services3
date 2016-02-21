/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2006 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************

  Description: export users to joomla

*/

#include "module.h"
#include "my_sql.h"
#include "dbconf.h"

#define REG_GROUP 18

SVS_Module mod_info =
 /* module, version, description */
{"joomla_export", "1.0",  "export users to joomla" };

/* Change Log
  1.0 - initial release
*/

/** functions/events we require **/
/* void (*FunctionPointer)(void);*/
int e_nick_register = -1;
int e_nick_delete = -1;

MOD_REQUIRES
  MOD_FUNC(dbconf_get_or_build)
  MOD_FUNC(e_nick_register)
  MOD_FUNC(e_nick_delete)
MOD_END

/** functions/events we provide **/

/** Internal functions declaration **/
/* void internal_function(void); */
int ev_joomla_export_nick_register(IRC_User *u, u_int32_t *snid);
int ev_joomla_export_nick_delete(u_int32_t *snid, void *dummy);
int joomla_db_mysql_open();
int joomla_db_mysql_query(char *sql);
u_int32_t joomla_sql_execute(char *fmt, ...);
MYSQL_RES* joomla_sql_query(char *fmt, ...);
int export_users(void);
int export_groups(void);

extern int sql_debug;

/** Local variables **/
static MYSQL *mysql;                   /* MySQL Handler */

/** Local config **/
static char* DB_Host;
static char* DB_User;
static char* DB_Password;
static char* DB_Name;
static char* DB_Prefix;

DBCONF_PROVIDES
  DBCONF_WORD(DB_Host,"Host", "Joomla MySQL server")
  DBCONF_WORD(DB_User, "User", "Joomla MySQL user")  
  DBCONF_WORD_OPT(DB_Password, "Password", "Joomla MySQL user password")
  DBCONF_WORD(DB_Name, 	"Database", "Joomla MySQL database name")
  DBCONF_WORD(DB_Prefix,"jos_",
    "Set to the $mosConfig_dbprefix from your Joomla configuration.php")
DBCONF_END


int mod_rehash(void)
{
  if(dbconf_get_or_build(mod_info.name, dbconf_provides) < 0 )
  {
    errlog("Error reading dbconf!");
    return -1;
  }
  return 0;
}

static int jo_log;
/** load code **/
int mod_load(void)
{
  MYSQL_RES* res;
  MYSQL_ROW row;
  u_int32_t nickserv_total = 0;
  u_int32_t joomla_total = 0;
  
  jo_log = log_open("joomla","joomla");
    
  /* first lets connect to the joomla db */  
  if(joomla_db_mysql_open() < 0)
  {
    errlog("Unable to connect to the Joomla database !");
    return -1;
  }  
  
  /* now lets check if the dbs are sinchronized */
  res = sql_query("SELECT count(*) FROM nickserv");
  if(res && (row = sql_next_row(res)))
    nickserv_total = atoi(row[0]);
  sql_free(res);

  res = joomla_sql_query("SELECT count(*) FROM %susers", DB_Prefix);
  if(res && (row = sql_next_row(res)))
    joomla_total = atoi(row[0]);    
  sql_free(res);
  
  if(nickserv_total != joomla_total)
  {
    stdlog(L_WARN, "Joomla database is out of sync, exporting full user database");
    if(export_users() < 0)
    {
      errlog("Error exporting to joomla db");
      return -2;
    }
    if(export_groups() < 0)
    {
      errlog("Error exporting groups");
      return -2;
    }    
  }
  
  /* setup the actions for the updates */
  mod_add_event_action(e_nick_register, 
    (ActionHandler) ev_joomla_export_nick_register);
  mod_add_event_action(e_nick_delete, 
    (ActionHandler) ev_joomla_export_nick_delete);
  
  return 0;
}
    
/** unload code **/
void mod_unload(void)
{
  return;
}
    
int ev_joomla_export_nick_register(IRC_User *u, u_int32_t *snid)
{
  MYSQL_RES *res;
  MYSQL_ROW row;    
  res = sql_query("SELECT n.snid, n.nick, n.email, ns.pass"
    " FROM nickserv n, nickserv_security ns"
    " WHERE ns.snid=n.snid AND n.snid=%d", *snid);
  if(res && (row = sql_next_row(res)))
  {
    u_int32_t aro_id = 0;
    if(joomla_sql_execute("INSERT INTO %susers "
      "VALUES (%s, %s, %s, %s, %s, 'Registered',"
      "0, 1, %d, 0, 0, '', '')", DB_Prefix,
      sql_str(row[0]), sql_str(row[1]), sql_str(row[1])
      , sql_str(row[2]), sql_str(row[3]), REG_GROUP) == 0)
        return 0;
    aro_id = joomla_sql_execute("INSERT INTO %score_acl_aro "
      "VALUES (%s, 'users', %s, 0, %s, 0)", 
      DB_Prefix, sql_str(row[0]), sql_str(row[0]), sql_str(row[1]));
    if(aro_id == 0)
        return 0;
    if(joomla_sql_execute("INSERT INTO %score_acl_groups_aro_map"
      " VALUES(%d, '', %d)", DB_Prefix, REG_GROUP, aro_id) == 0)
        return 0;  
  }
  return 0;
}

int ev_joomla_export_nick_delete(u_int32_t *snid, void *dummy)
{  
  joomla_sql_execute("DELETE FROM %susers WHERE id=%d", DB_Prefix, *snid); 
  joomla_sql_execute("DELETE FROM %score_acl_aro WHERE aro_id=%d", 
    DB_Prefix, *snid);
  joomla_sql_execute("DELETE FROM %score_acl_groups_aro_map"
    " WHERE aro_id=%d", DB_Prefix, *snid);    
  return 0;
}

int export_users(void)
{
  MYSQL_RES *res;
  MYSQL_ROW row;

  /* delete the existing users and acls */
  joomla_sql_execute("DELETE FROM %susers", DB_Prefix); 
  joomla_sql_execute("DELETE FROM %score_acl_aro", DB_Prefix);
  joomla_sql_execute("DELETE FROM %score_acl_groups_aro_map", DB_Prefix);  
  
  /* insert the new users */
  res = sql_query("SELECT n.snid, n.nick, n.email, ns.pass"
    " FROM nickserv n, nickserv_security ns"
    " WHERE ns.snid=n.snid");      
  while((row = sql_next_row(res)))
  {
    u_int32_t aro_id;

    /* joomla does not accept null emails or fields */
    if(row[2] == NULL)
      row[2] = "";
    if(row[3] == NULL)
      row[3] = "";
    if(joomla_sql_execute("INSERT INTO %susers "
      "VALUES (%s, %s, %s, %s, %s, 'Registered',"
      "0, 1, %d, 0, 0, '', '')", DB_Prefix,
      sql_str(row[0]), sql_str(row[1]), sql_str(row[1])
      , sql_str(row[2]), sql_str(row[3]), REG_GROUP) == 0)
        return -1;
    
    aro_id = joomla_sql_execute("INSERT INTO %score_acl_aro "
      "VALUES (%s, 'users', %s, 0, %s, 0)", 
      DB_Prefix, sql_str(row[0]), sql_str(row[0]), sql_str(row[1]));
    if(aro_id == 0)
        return -1;
    if(joomla_sql_execute("INSERT INTO %score_acl_groups_aro_map"
      " VALUES(%d, '', %d)", DB_Prefix, REG_GROUP, aro_id) == 0)
        return -1;
  }
  sql_free(res);
  return 0;
}

int export_groups(void)
{
  MYSQL_RES *res;
  MYSQL_ROW row;

  /* inser the new users */
  res = sql_query("SELECT ng.name, ngu.snid"
    " FROM ns_group ng, ns_group_users ngu"
    " WHERE ng.sgid=ngu.sgid");      
  while((row = sql_next_row(res)))
  {
    int new_gid = 0;
    if(strcasecmp(row[0], "Root") == 0)
      new_gid = 25; /* Super Admin */
    else
    if(strcasecmp(row[0], "Admin") == 0)    
      new_gid = 24; /* Admin */
    else
    if(strcasecmp(row[0], "Admin") == 0)    
      new_gid = 23; /* Manager */
    else
      continue;
    if(joomla_sql_execute("UPDATE %susers SET gid=%d WHERE id=%s"
      , DB_Prefix, new_gid, row[1]) == 0)
      return -1;
    joomla_sql_execute("UPDATE %score_acl_groups_aro_map "
      "SET group_id=%d WHERE aro_id=%s", DB_Prefix, new_gid, row[1]);
  }
  sql_free(res);
  return 0;
}

int joomla_db_mysql_open()
{
  mysql = mysql_init(NULL);
  
  if ((!mysql_real_connect
      (mysql, DB_Host, DB_User, DB_Password, DB_Name, 0, NULL, 0)))
      {
        printf("MySQL ErrNo: %d\n", mysql_errno(mysql));
        errlog("Cant connect to MySQL: %s\n", mysql_error(mysql));
        return -1;
      }
      
  log_log(jo_log, "Connect", "MySQL connected to %s", DB_Host);

  return 1;
}

/*************************************************************************/

int joomla_db_mysql_query(char *sql)
{

    int result, lcv;
    /* slog(L_INFO,"%s", sql); */
    result = mysql_query(mysql, sql);

    if (result) {
        switch (mysql_errno(mysql)) {
        case CR_SERVER_GONE_ERROR:
        case CR_SERVER_LOST:
            /* Reconnect -> 5 tries (need to move to config file) */
            for (lcv = 0; lcv < 5; lcv++) {
                if (db_mysql_open()>0) {
                    result = mysql_query(mysql, sql);
                    return (result);
                }
                sleep(1);
            }

            /* If we get here, we could not connect. */
            slog(L_ERROR, "Unable to reconnect to database: %s\n",
                       mysql_error(mysql));
            db_mysql_error(MYSQL_ERROR, "connect");
            exit(-5);
            /* Never reached. */
            break;

        default:
            /* Unhandled error. */
            return (result);
        }
    }

    return (0);

}    

/* execute an sql query (for insert/deletes/updates
  returns
  ANY ACTION
    0 means ERROR
  INSERT
    mysql_insert_id if there is some autoincrement field
    or affected rows otherwise
  DELETE/UPDATES
    number of affected rows
*/
u_int32_t joomla_sql_execute(char *fmt, ...)
{
  static char buf[4096];
  va_list args;
  
  va_start(args, fmt);
  vsnprintf(buf, sizeof(buf), fmt, args);
  va_end(args);

  if(sql_debug)
    stdlog(L_INFO, "%s", buf);    

  if (joomla_db_mysql_query(buf)) 
    {
      /* slog(L_ERROR, "Can't create sql query: %s", buf);*/
      log_log(jo_log, "ERROR", "sql_execute() %s", buf);
      db_mysql_error(MYSQL_WARNING, "query");
      return 0;
    }    
    
  if(strncasecmp(buf,"INSERT",6) == 0)
    {
      u_int32_t id;
      id = mysql_insert_id(mysql);
      if(id == 0) /* only tables without id return 0 */
        return mysql_affected_rows(mysql);
      return id;
    }

  return  mysql_affected_rows(mysql);  
}

/* */
MYSQL_RES* joomla_sql_query(char *fmt, ...)
{
  static char buf[4096];
  va_list args;
  
  va_start(args, fmt);
  vsnprintf(buf, sizeof(buf), fmt, args);
  va_end(args);

  if(sql_debug)
    stdlog(L_INFO, "%s", buf);  
  if (joomla_db_mysql_query(buf)) 
    {
      log_log(jo_log, "ERROR", "sql_query() %s", buf);
      /* slog(L_ERROR, "Can't create sql query: %s", buf); */
      db_mysql_error(MYSQL_WARNING, "query");
      return NULL;
    }
    
  return mysql_store_result(mysql);
}


/* End of module */
