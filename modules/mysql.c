/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2006 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************  

  Description: a mysql module

*/

#include "module.h"
#include "my_sql.h"
#include "path.h"

SVS_Module mod_info =
 /* module, version, description */
{"mysql", "1.0",  "mysql support module" };

#define DB_VERSION	1

/** functionsand events we require **/
/* void (*FunctionPointer)(void);*/


/** functionsa and events we provide **/
/* void my_function(void); */
int mysql_connection; /* for now just a dumb provide to enforce the dependency */

MOD_PROVIDES
  MOD_FUNC(mysql_connection)
MOD_END

/** Internal functions declaration **/
int create_user_db(void);
int open_mysql_log(void);
int try_db_install(void);
/* void internal_function(void); */
    
/** Exteran db settings **/
extern char* DB_Host;
extern char* DB_Name;
extern char* DB_User;
extern char *DB_Pass;


/** Local variables **/
/* int my_local_variable; */
int mysql_log;

/** Local functions */
static int get_mysql_config(void)
{
  char conf_fn[256];
  FILE *confile;
  char line[512];
  
  snprintf(conf_fn, sizeof(conf_fn), "%s/ircsvs.conf", ETCPATH);
  confile = fopen(conf_fn,"rt");
  if(confile==NULL)
  {
    errlog("Unable to open configuration file %s !", conf_fn);
    return -2;
  }
  while (!feof(confile))
  {
    char *value;
    char *setting;
    char *tmp;
    line[0] = '\0';
    if(fgets(line, sizeof(line), confile)==NULL)
      break;
    if((tmp = strchr(line, '#')))
      *tmp = '\0';
    strip_rn(line);      
    setting = line;
    while(isspace(*(setting))) /* skip leading spaces */
      ++setting;
    value = setting;
    while(*value && !isspace(*value))  /* skip setting */
      ++value;
    if(*value)
      *value++ = '\0';
    while(isspace(*(value))) /* skip spaces */
      ++value;
    tmp = value;
    while(*tmp && !isspace(*tmp))
      ++tmp;      
    *tmp = '\0';
    if(!value || (*value == 0) || !setting || (*setting == 0)) /* not a valid line */
      continue;
    if(strcasecmp(setting, "DB_Host") == 0)
      DB_Host = strdup(value);
    if(strcasecmp(setting, "DB_User") == 0)
      DB_User = strdup(value);
    if(strcasecmp(setting, "DB_Name") == 0)
      DB_Name = strdup(value);
    if(strcasecmp(setting, "DB_Pass") == 0)
      DB_Pass = strdup(value);
  }
  return 0;  
}
    
/** load code **/
int mod_load(void)
{

  int r;
  
  if((r = get_mysql_config()) < 0 )
  {
    errlog("Error reading configuration!");
    return r;
  }    
    
  if(open_mysql_log() <0)
    return -2;
  
  if(db_mysql_open()<0)
    {
      if(sql_errno() == ER_ACCESS_DENIED_ERROR ||
         sql_errno() == ER_DBACCESS_DENIED_ERROR ||
         sql_errno() == ER_BAD_DB_ERROR)
         return try_db_install();
         
      errlog("Unable to connect to MYSQL: %s", sql_error());
      return -3;   
    }
    
  if(sql_check_inst_upgrade(mod_info.name, DB_VERSION, NULL) < 0)
    return -4;
        
  log_log(mysql_log,"mysql", "Mysql module %s was sucefully loaded", 
    mod_info.version);
    
  return 0;
}

/** unload code **/ 
void mod_unload(void) 
{
  return;
}
    
/** internal functions implementation starts here **/

/* try to install db */
int try_db_install(void)
{
  char ans[10];
  printf("Unable to connect to the DB: %s\n", sql_error());

  printf("Do you want to try to connect as mysql admin to create the user and db ?");
  ans[0]='\0';
  fgets(ans, sizeof(ans), stdin);
  if(ans[0]=='Y' || ans[0]=='y')
    {
      if(create_user_db() == 0)
        return -3;
      if(db_mysql_open() < 0)
      {
        return -4;
      }
    }
  log_log(mysql_log, "Connection", "Connection succesfully established");
  if(sql_check_inst_upgrade(mod_info.name, DB_VERSION, NULL) < 0)
    return -4;  
  return 1;
}

/* ask for the admin user pass and try to connect */
int create_user_db(void)
{
  int res;
  MYSQL my_connection;
  
  char dbhost[128];
  char dbuser[128];
  char dbpass[128];
  char sql[1024];  
  mysql_init(&my_connection);  
  
  printf("MySQL host [localhost]: ");
  fgets(dbhost, sizeof(dbhost), stdin);
  strip_rn(dbhost);
  if(dbhost[0] == '\0' )
    strncpy(dbhost, "localhost", sizeof(dbhost));
  printf("MySQL admin user [root]: ");
  fgets(dbuser, sizeof(dbuser), stdin);
  strip_rn(dbuser);
  if(dbuser[0] == '\0')
    strncpy(dbuser,"root", sizeof(dbuser));        
  printf("MySQL admin pass []: ");
  fflush(stdout);
  get_pass(dbpass,sizeof(dbpass));
  strip_rn(dbpass);
  printf("\n");
  printf("MySQL connect to %s as %s\n", dbhost, dbuser);
  if (!mysql_real_connect(&my_connection, dbhost,
    dbuser, dbpass, "mysql", 0, NULL, 0))
    {
      fprintf(stderr,"Could not connect: %s\n",
       	mysql_error(&my_connection));
      return 0;
    }
  printf("Creating database %s\n", DB_Name);
  snprintf(sql, sizeof(sql)-1, "CREATE DATABASE %s", DB_Name);
  res = mysql_query(&my_connection, sql);
  if(res<0)
  {
    fprintf(stderr,"MySQL Error: %s\n", mysql_error(&my_connection));
    fprintf(stderr,"SQL was: %s\n", sql);
    mysql_close(&my_connection);      
    return 0;
  }        
  printf("Granting privileges to %s@%s\n", DB_User, DB_Host);
  snprintf(sql, sizeof(sql)-1, "GRANT ALL ON %s.* TO %s@%s IDENTIFIED BY '%s'",
    DB_Name, DB_User, DB_Host, DB_Pass);
  res = mysql_query(&my_connection, sql);
  if(res<0)
    {
      fprintf(stderr,"MySQL Error: %s\n", mysql_error(&my_connection));
      fprintf(stderr,"SQL was: %s\n", sql);
      mysql_close(&my_connection);      
      return 0;
    }                
  mysql_close(&my_connection);      

  printf("MySQL connect to %s as %s, database %s\n", 
    DB_Host, DB_User, DB_Name);      
  if (!mysql_real_connect(&my_connection, DB_Host,
    DB_User, DB_Pass, DB_Name, 0, NULL, 0))
    {
      fprintf(stderr,"Could not connect: %s\n",
        mysql_error(&my_connection));
        return 0;
    } 
    
  mysql_close(&my_connection);
  printf("User and DB where succesfully created\n");
  return -1;
}

int open_mysql_log(void)
{
  mysql_log = log_open("mysql","mysql");
  if(mysql_log < 0)
    {
      errlog("Unable to create mysql log file");
      return -1;
    }
  return 0;
}   
