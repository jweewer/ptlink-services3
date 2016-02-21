/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2006 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************  

  Description: mysql functions

*/

#include "stdinc.h"
#include "path.h"
#include "ircservice.h"
#include "s_log.h"
#include "log.h"
#include "chanrecord.h"
#include "ircsvs.h"
#include "my_sql.h"
#include "sqlb.h"
#include "hash.h"
#include "strhand.h"

extern int sql_debug;

/* DB_* are external, should be loaded from dconf */
char* DB_Host = NULL;
char* DB_Name = NULL;
char* DB_User = NULL;
char* DB_Pass = NULL;

int mysql_log = 0;

int sql_close(void);
int db_mysql_query(char *sql);

MYSQL *mysql;                   /* MySQL Handler */
MYSQL_ROW my_row;

#define CR_SELECT_FIELDS \
      "scid, name, url, email, founder, successor," \
      "last_topic, last_topic_setter, t_ltopic, t_reg,  t_last_use," \
      "mlock, status, flags, entrymsg, cdesc, t_maxusers," \
      "maxusers"

#define CR_UPDATE_PARAM \
      "name=%s, url=%s, email=%s, founder=%s, successor=%s," \
      "last_topic=%s, last_topic_setter=%s, t_ltopic=%d, t_reg=%d, t_last_use=%d," \
      "mlock=%s, status=%d, flags=%d, entrymsg=%s, cdesc=%s, t_maxusers=%d, maxusers=%d " \
      "WHERE scid=%d", \
      sql_str(irc_lower(cr->name)), sql_str(cr->url), sql_str(cr->email), founder, successor, \
      sql_str(cr->last_topic), sql_str(cr->last_topic_setter), (int) cr->t_ltopic, (int) cr->t_reg, (int) cr->t_last_use,\
      sql_str(cr->mlock), cr->status, cr->flags, sql_str(cr->entrymsg), sql_str(cr->cdesc), (int) cr->t_maxusers, cr->maxusers, \
      cr->scid \

void db_mysql_error(int severity, char *msg)
{
    static char buf[512];

    if (mysql_error(mysql)) {
        snprintf(buf, sizeof(buf), "MySQL %s %s: %s", msg,
                 severity == MYSQL_WARNING ? "warning" : "error",
                 mysql_error(mysql));
    } else {
        snprintf(buf, sizeof(buf), "MySQL %s %s", msg,
                 severity == MYSQL_WARNING ? "warning" : "error");
    }

    log_log(mysql_log, "ERROR", "%s", buf);
    fprintf(stderr, "%s\n", buf);

    if (severity == MYSQL_ERROR) {
        log_log(mysql_log, "FATAL", "MySQL FATAL error... aborting.");
        slog(L_ERROR, "MySQL FATAL error... aborting.");
        exit(0);
    }
}


const char *sql_error(void)
{
  return mysql_error(mysql);
}

/*************************************************************************/


int db_mysql_open()
{
  mysql = mysql_init(NULL);
  
  if ((!mysql_real_connect
      (mysql, DB_Host, DB_User, DB_Pass, DB_Name, 0, NULL, 0))) 
      {
        printf("MySQL ErrNo: %d\n", mysql_errno(mysql)); 
        log_log(mysql_log, "ERROR", "%s", mysql_error(mysql));            
        slog(L_ERROR,"Cant connect to MySQL: %s\n", mysql_error(mysql));
        return -1;
      }
  /* slog(L_INFO,"MySQL connected to %s", DB_Host); */
  log_log(mysql_log, "Connect", "MySQL connected to %s", DB_Host);

  return 1;  
}

/*************************************************************************/

int db_mysql_query(char *sql)
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

/**
   returns a sql safe string (between "'") removing "\" and "'"
   it will also remove those chars from the parameter string;
*/
char* sql_str(const char *str)
{
  static char bufs[20][512]; /* we can keep 20 strings */
  static int stri = 0; /* current string index */
  char* cbuf; /* current buffer */
  const char* c; /* position on str */
  int i;
  
  if(IsNull(str))
    return "NULL";
  cbuf = bufs[stri++];
  
  if(stri>19) /* reached end of array */
    stri = 0;
  
  cbuf[0] = '\''; /* start the string */
  i = 1;
  c = str;
  while(*c && (i<507)) /* 511 - (2 for "\" + char), 2 for string end */
    {
      /* prefix with "\" */
      if((*c == '\'') || (*c == '\\'))      
        cbuf[i++] = '\\';
        
      cbuf[i++] = *(c++);
    }  
  cbuf[i] = '\''; /* close the string */
  cbuf[i+1] = '\0';  
  
  return cbuf;
}



/*************************************************************************/

int sql_close()
{
    mysql_close(mysql);
    return 1;
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
u_int32_t sql_execute(char *fmt, ...)
{
  static char buf[4096];
  va_list args;
  
  va_start(args, fmt);
  vsnprintf(buf, sizeof(buf), fmt, args);
  va_end(args);

  if(sql_debug)
    stdlog(L_INFO, "%s", buf);    

  if (db_mysql_query(buf)) 
    {
      /* slog(L_ERROR, "Can't create sql query: %s", buf);*/
      log_log(mysql_log, "ERROR", "sql_execute() %s", buf);
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
MYSQL_RES* sql_query(char *fmt, ...)
{
  static char buf[4096];
  va_list args;
  
  va_start(args, fmt);
  vsnprintf(buf, sizeof(buf), fmt, args);
  va_end(args);

  if(sql_debug)
    stdlog(L_INFO, "%s", buf);  
  if (db_mysql_query(buf)) 
    {
      log_log(mysql_log, "ERROR", "sql_query() %s", buf);
      /* slog(L_ERROR, "Can't create sql query: %s", buf); */
      db_mysql_error(MYSQL_WARNING, "query");
      return NULL;
    }
    
  return mysql_store_result(mysql);
}

/* */
int sql_singlequery(char *fmt, ...)
{
  static char buf[4096];
  va_list args;
  static MYSQL_RES* res = NULL;

  if(res)
    {
      sql_free(res);
      res = NULL;
    }
    
  va_start(args, fmt);
  vsnprintf(buf, sizeof(buf), fmt, args);
  va_end(args);

  if(sql_debug)
    stdlog(L_INFO, "%s", buf);  
    
  if (db_mysql_query(buf)) 
    {
      log_log(mysql_log, "ERROR:", "sql_singlequery() %s", buf);
      /* slog(L_ERROR, "Can't create sql query: %s", buf); */
      db_mysql_error(MYSQL_WARNING, "query");
      return 0;
    }
    
  res = mysql_store_result(mysql);
  if(res)
    {
      my_row = mysql_fetch_row(res);
      if(my_row)
        return 1;
    }
    
  return 0;
}

/* Moves to the next row */
MYSQL_ROW sql_next_row(MYSQL_RES* res)
{
  if(res == NULL)
    return NULL;
    
  my_row = mysql_fetch_row(res);
  return my_row;
}

void sql_free(MYSQL_RES* res)
{
  mysql_free_result(res);
}

char *sql_field(int i)
{
  if(my_row)
    return my_row[i];
  else
    return NULL;
}

u_int32_t sql_field_i(int i)
{
  if(my_row && my_row[i])
    return atoi(my_row[i]);
  else
    return 0;
}

MYSQL_RES* sql_use_result(void)
{
  return mysql_use_result(mysql);
}


u_int32_t sql_last_id(void)
{
  return mysql_insert_id(mysql);
}

/*************************************************************************/

/*
 * NickRecord functions *
 */

u_int32_t nick2snid(char *name)
{
  MYSQL_RES* mysql_res;    
  MYSQL_ROW row;
  char sqlcmd[MAX_SQL_BUF]; 
  u_int32_t snid;
  snprintf(sqlcmd, MAX_SQL_BUF,"SELECT snid FROM nickserv WHERE nick=%s", 
    sql_str(irc_lower_nick(name)));
  if (db_mysql_query(sqlcmd))
    {
      log_log(mysql_log, "ERROR:", "nick2snid() %s", sqlcmd);
      /*      
      slog(L_ERROR, "Can't create sql query: %s", sqlcmd);
      */
        db_mysql_error(MYSQL_WARNING, "query");
        
        return 0;
    }
  mysql_res = mysql_store_result(mysql);
  
  if(mysql_res == NULL)
    return 0;
  
  row = mysql_fetch_row(mysql_res);

  if(row == NULL)
    snid = 0;
  else 
    snid = atoi(row[0]);

  mysql_free_result(mysql_res);
  return snid;
}

/* returns count of nicks with given email */
int reg_count_for_email(char *email)
{
  MYSQL_RES* mysql_res;    
  MYSQL_ROW row;
  char sqlcmd[MAX_SQL_BUF]; 
  int count = 0;

  snprintf(sqlcmd, MAX_SQL_BUF,
    "SELECT count(*) FROM nickserv WHERE email=%s", sql_str(email));
  if (db_mysql_query(sqlcmd))
    {
      log_log(mysql_log, "ERROR:", "email_count() %s", sqlcmd);
        db_mysql_error(MYSQL_WARNING, "query");
        return 0;
    }
  mysql_res = mysql_store_result(mysql);
  row = mysql_fetch_row(mysql_res);
  if(row)
    count = atoi(row[0]);
  mysql_free_result(mysql_res);            
  return count;
}

/*
 * ChanRecord functions *
 */ 

/* Insert a new chan record */
int db_mysql_insert_cr(ChanRecord* cr)
{
    char successor[16];
    char founder[16];
    sqlb_init("chanserv");
    sqlb_add_int("scid", cr->scid);
    sqlb_add_str("name", irc_lower(cr->name));
    sqlb_add_str("email", cr->email);
    if(cr->founder)
    {
      snprintf(founder, sizeof(founder), "%d", cr->founder);
      sqlb_add_str("founder", founder);
    }
    else
      sqlb_add_str("founder", NULL);
    if(cr->successor)
    {    
      snprintf(successor, sizeof(successor), "%d", cr->successor);
      sqlb_add_str("successor", successor);
    }
    else
      sqlb_add_str("successor", NULL);
    sqlb_add_str("last_topic", cr->last_topic);
    sqlb_add_str("last_topic_setter", cr->last_topic_setter);
    sqlb_add_int("t_ltopic", cr->t_ltopic);
    sqlb_add_int("t_reg", cr->t_reg);
    sqlb_add_int("t_last_use", cr->t_last_use);
    sqlb_add_str("mlock", cr->mlock);
    sqlb_add_int("status", cr->status);
    sqlb_add_int("flags", cr->flags);
    sqlb_add_str("entrymsg", cr->entrymsg);
    sqlb_add_str("cdesc", cr->cdesc);
    sqlb_add_int("t_maxusers", cr->t_maxusers);
    sqlb_add_int("maxusers", cr->maxusers);

    if (db_mysql_query(sqlb_insert())) {
        log_log(mysql_log, "ERROR:", "insert_cr() %s", sqlb_insert());
        db_mysql_error(MYSQL_WARNING, "query");
        return -1;
    }
    cr->scid = mysql_insert_id(mysql);
    return cr->scid;
}

ChanRecord* db_mysql_get_cr(u_int32_t scid)
{
  MYSQL_RES* mysql_res;    
  MYSQL_ROW row;
  ChanRecord *cr;  
  char sqlcmd[MAX_SQL_BUF]; 
  
  snprintf(sqlcmd, MAX_SQL_BUF,
    "SELECT "
    CR_SELECT_FIELDS
    " FROM chanserv WHERE scid=%d", scid);
          
  if (db_mysql_query(sqlcmd)) 
    {
      log_log(mysql_log, "ERROR:", "get_cr() %s", sqlcmd);    
/*      
      slog(L_ERROR, "Can't create sql query: %s", sqlcmd);
*/      
        db_mysql_error(MYSQL_WARNING, "query");
      return NULL;
    }
  mysql_res = mysql_store_result(mysql);
  row = mysql_fetch_row(mysql_res);  
  if(IsNull(row))
    {
      mysql_free_result(mysql_res);     
      return NULL;
    }
        
 /* we have data, lets process it */
  cr = CreateCR(row[1]);
  cr->scid = atoi(row[0]);   
  SDUP(cr->url, row[2]); 
  SDUP(cr->email, row[3]);     
  if(row[4])
    cr->founder = atoi(row[4]);
  else
    cr->founder = 0;
  if(row[5])
    cr->successor = atoi(row[5]);
  else
    cr->successor = 0;
  SDUP(cr->last_topic, row[6]);
  SDUP(cr->last_topic_setter, row[7]);
  cr->t_ltopic = atoi(row[8]);
  cr->t_reg = atoi(row[9]);
  cr->t_last_use = atoi(row[10]);
  SDUP(cr->mlock, row[11]);
  cr->status = atoi(row[12]);
  cr->flags = atoi(row[13]);
  SDUP(cr->entrymsg, row[14]);
  SDUP(cr->cdesc, row[15]);    
  cr->t_maxusers = atoi(row[16]);
  cr->maxusers = atoi(row[17]);
  
  mysql_free_result(mysql_res); 
  return cr;
}

/* update chan record */
int db_mysql_update_cr(ChanRecord* cr)
{
    char sqlcmd[MAX_SQL_BUF];
    char successor[16];
    char founder[16];
    
    if(cr->successor)
      snprintf(successor, sizeof(successor), "'%d'", cr->successor);
    else
      strcpy(successor, "NULL");

    if(cr->founder)
      snprintf(founder, sizeof(founder), "'%d'", cr->founder);
    else
      strcpy(founder, "NULL");      
    
    snprintf(sqlcmd, MAX_SQL_BUF,
      "UPDATE chanserv SET "
      CR_UPDATE_PARAM
      );

    if (db_mysql_query(sqlcmd)) {
        log_log(mysql_log, "ERROR:", "update_cr() %s", sqlcmd);    
/*        
        slog(L_ERROR, "Can't create sql query: %s", sqlcmd);
*/        
        db_mysql_error(MYSQL_WARNING, "query");
        return 0;
    }
    return 1;
}

u_int32_t db_mysql_chan2scid(char *name)
{
  MYSQL_RES* mysql_res;
  MYSQL_ROW row;
  char sqlcmd[MAX_SQL_BUF]; 
  u_int32_t scid;
  snprintf(sqlcmd, MAX_SQL_BUF,"SELECT scid, name FROM chanserv WHERE name=%s", 
    sql_str(irc_lower(name)));
  if (db_mysql_query(sqlcmd))
    {
      log_log(mysql_log, "ERROR:", "chan2scid() %s", sqlcmd);    
      /*      
      slog(L_ERROR, "Can't create sql query: %s", sqlcmd);
      */      
        db_mysql_error(MYSQL_WARNING, "query");
        return 0;
    }
  mysql_res = mysql_store_result(mysql);
  
  if(mysql_res == NULL)
    return 0;
    
  while((row = mysql_fetch_row(mysql_res)) && irccmp(row[1], name)) ;
  
  if(row == NULL)
    scid = 0;
  else
    scid = atoi(row[0]);
    
  mysql_free_result(mysql_res);
  return scid;
}

/*
  Execute a sql command from file 
*/
int mysql_from_file(char *fn)
{
  FILE *f;
  char buf[32000];

  char *sql;
  int res;
  
  bzero(buf, 32000);  
  f = fopen(fn,"rt");  
  if(IsNull(f))
    {
      stdlog(L_ERROR, "Could not open %s \n", fn); 
      return -1;
    }
  fread(buf, sizeof(buf), 1, f);
  sql = strtok(buf,";");
  while(sql && strlen(sql)>10)
    {    
      res = mysql_query(mysql, sql);
      if(res)
        {
          errlog("MySQL Error: %s", mysql_error(mysql));
          errlog("SQL: %s", sql);
          fclose(f);
          return -2;
        }
      sql = strtok(NULL, ";");            
    }
  fclose(f);
  return 0;
}

int sql_errno(void)
{
  return mysql_errno(mysql);
}

/* sql_fields_count

 *  returns:
 *	count of fields from last sql query
 */
int sql_field_count(void)
{
  return mysql_field_count(mysql);
}

/**
 * check, install or upgrade tables for a module
 * returns:
 *	<0 : there was some error
 *	0  : tables are ok
 *	1  : tables were installed
 *	2  : tables were upgraded
 */
int sql_check_inst_upgrade(char *name, int version, void* upgrade_func) 
{
  char fn[256];
  char sql[128];
  int current_version = 0;
  MYSQL_RES* res;
  MYSQL_ROW row;
  int i;
  snprintf(sql, sizeof(sql),
    "SELECT MAX(version) FROM ircsvs_tables WHERE name='%s'", name);
  if(mysql_query(mysql, sql) == 0)
  {
    res = mysql_store_result(mysql);
    if(res)
      row = mysql_fetch_row(res);  
    if(row  && row[0])
      current_version = atoi(row[0]);
    if(res)    
      mysql_free_result(res);
  }
  
  if(current_version == 0)
    {
      stdlog(L_WARN, "Installing %s table(s)", name);
      log_log(mysql_log, "INSTALL", "Installing %s table(s)", name);
      snprintf(fn, sizeof(fn), "%s/%s.sql", SQLPATH, name);
      if(mysql_from_file(fn) < 0)
        {
          errlog("Error executing SQL from %s", fn);
          return -2;      
        }
      if(sql_execute("INSERT INTO ircsvs_tables "
          " VALUES('%s',%d, NOW())", name, version) == 0)
        {
          errlog("Unable to insert table version info!");
          return -3;
        }
      return 1;
    }
  else if(current_version < version)
    {
      stdlog(L_WARN, "Upgrading %s tables from version %d to %d",
        name, current_version, version);        
      for(i = current_version+1; i <= version; ++i)
        {
          int r;
          stdlog(L_WARN, "SQL Upgrade %s.%d", name, i);
          if(upgrade_func) /* we have an upgrade function to call */
            {
              r = ((int (*)(int, int)) upgrade_func)(i, 0);
              if(r < 0)
                {
                  errlog("Error %i from pre upgrade routine %i", r, i);
                  return -3;
                }
            }          
          snprintf(fn, sizeof(fn), "%s/%s.%d.sql", SQLPATH, name, i);
          if(mysql_from_file(fn) < 0)
            {
              errlog("Error executing SQL from %s", fn);
              return -2;      
            }
          if(upgrade_func) /* we have an upgrade function to call */
            {
              r = ((int (*)(int, int)) upgrade_func)(i, 1);
              if(r < 0)
                {
                  errlog("Error %i from post upgrade routine %i", r, i);
                  return -3;
                }
            }
          if(sql_execute("INSERT INTO ircsvs_tables "
            " VALUES('%s',%d, NOW())", name, i) == 0)
            {
              errlog("Unable to insert table version info!");
              return -4;
            }
        }
      return 2;
    }
  else if(current_version > version)
    {
      errlog("Module %s expects version %d, current version: %d",
        name, version, current_version); 
        return -4;
    }
  return 0;
}

/* sql_find_module - check if a given module tables are installed
 *  accepts:
 *	module name
 *  returns:
 *	number of version installed, 0 if not found
 */
int sql_find_module(char *module_name)
{
  MYSQL_RES* res;
  MYSQL_ROW row;
  int version = 0;
  res = sql_query("SELECT version FROM ircsvs_tables WHERE name=%s",
    sql_str(module_name));
  if((row = sql_next_row(res)))
    version = atoi(row[0]);
  sql_free(res);
  return version;
}

/**************************************************************************
 DBMem functions
 **************************************************************************
 This functions should be used whenever you need to keep a set of data both 
 on the db and memory synchronized. This maybe required when you have data 
 with frequent processing (like pattern searchs) which will get a major 
 performance boost over the sql queries. 
 **************************************************************************/

/* dbmem_init
 *  accepts:
 *	table_name of the table containing the data
 *	alloc_units number of rows for pre-allocation
 *  returns:
 *	DBMem structure
 */
DBMem* dbmem_init(char *table_name, int alloc_units)
{
  DBMem* new_dbmem = malloc(sizeof(DBMem));
  new_dbmem->table_name = strdup(table_name);
  new_dbmem->alloc_units = alloc_units;
  new_dbmem->data = malloc(sizeof(char**) * alloc_units);
  new_dbmem->row_max = alloc_units;
  new_dbmem->row_count = 0;
  return new_dbmem;
};

/* dbmem_increase - increase memory allocation to add rows data
 *  accepts:
 *	DBMem structure
 *  returns:
 *	 <0 if there was an error (out of memory ?)
 */
int dbmem_increase(DBMem* dbm)
{
  char ***data;
  int new_max = dbm->row_max + dbm->alloc_units;
  data = realloc(dbm->data, sizeof(char**)*new_max);
  if(data)
  {
    dbm->data = data;
    dbm->row_max = new_max;
  }
  else
    return -1;
  return 0;
}

/* dbmem_load - load data from the db to memory
 *  accepts:
 *	DBMem structure
 *  returns:
 *	Number of retrieved rows, <0 for errors
 *  errors:
 *	-1: SQL error (table doest not exist ?)
 *	-2: Unable to allocate memory
 */
int dbmem_load(DBMem* dbm)
{
  MYSQL_FIELD *key_field;
  MYSQL_RES *res;
  MYSQL_ROW row;
  int new_count = 0;
  int count = 0;
  int max = 0;
  int field_count = 0;

  res = sql_query("SELECT * FROM %s", dbm->table_name);
  if(res == NULL)
    return -1;
  field_count = sql_field_count();
  dbm->field_count = field_count;
  count = dbm->row_count;
  max = dbm->row_max;
  while((row = sql_next_row(res)))
  {
    char ** curr_row;
    int i;
    if(count >= max)
    {
      if(dbmem_increase(dbm)<0)
	return -2;
      count = dbm->row_count;
      max = dbm->row_max;
    }
    curr_row = malloc(sizeof(char*) * field_count);
    dbm->data[count] = curr_row;
    for(i=0; i < field_count; ++i) /* copy data to memory */
       curr_row[i] = row[i] ? strdup(row[i]) : NULL; 
    ++count;
    ++new_count;
  }
  /* get first field for the key */
  key_field = mysql_fetch_field(res);
  assert(key_field != NULL);
  dbm->key_field = strdup(key_field->name);
  sql_free(res);
  dbm->row_count = count;
  
  return new_count;
}

/* dbmem_insert - inserts a new row into db/mem
 *  accepts:
 *	DBMem structure
 *	data row
 *  returns:
 *	1 if row was added, <0 for errors
 *  errors:
 *	-1: SQL error (table doest not exist ?)
 *	-2: Unable to allocate memory
 */
int dbmem_insert(DBMem* dbm, char** row)
{
  char buf[1024];
  int i;
  int len;
  int field_count;
  char **curr_row;
  u_int32_t id;
  
  if(dbm->row_count >= dbm->row_max)
  {
    if(dbmem_increase(dbm)<0)
      return -2;
  }
  field_count = dbm->field_count;
  len = snprintf(buf, sizeof(buf), "INSERT INTO %s VALUES(", dbm->table_name);
  for(i = 0; i < field_count; ++i)
  {
    len += snprintf(buf+len, sizeof(buf)-len, "%s", sql_str(row[i]));
    if((i < field_count-1) &&  (sizeof(buf)-len-1>0))
      buf[len++] = ',';
  }
  snprintf(buf+len, sizeof(buf)-len, ")");
  if(sql_execute(buf) == 0)
    return -1;
  /* data was inserted, lets keep it on mem */
  curr_row = malloc(sizeof(char*) * field_count);
  dbm->data[dbm->row_count] = curr_row;  
  for(i = 0; i < field_count; ++i)
     curr_row[i] = row[i] ? strdup(row[i]) : NULL;
  id = mysql_insert_id(mysql);
  if(id)
  {
    if(curr_row[0])
      free(curr_row[0]);
    curr_row[0] = strdup(itoa(id));
  }
  dbm->row_curr = dbm->row_count;
  ++(dbm->row_count);
  return 1;
}

/* dbmem_replace_key - updates the current row key
 *  accepts:
 *	DBMem structure
 *	data row
 *	new key
 *  returns:
 *	1 if row was added, <0 for errors
 *  errors:
 *	-1: SQL error (table doest not exist ?)
 *	-2: Unable to allocate memory
 */
int dbmem_replace_key(DBMem* dbm, char* new_key)
{
  char **row = dbm->data[dbm->row_curr];
  if(sql_execute("UPDATE %s SET %s=%s WHERE %s=%s", dbm->table_name,
    dbm->key_field, sql_str(new_key), dbm->key_field, sql_str(row[0])) == 0)
      return -1;
  FREE(row[0]);
  row[0] = strdup(new_key);
  return 1;
}

/* dbmem_first_row - move internal pointer to the first row
 *  accepts:
 *	DBMem structure
 *  returns:
 *	first row of data , NULL if no data
 */
char** dbmem_first_row(DBMem* dbm)
{
  if(dbm->row_count > 0)
  {
    dbm->row_curr = 0;
    return dbm->data[0];
  }
  else
    return NULL;
}

/* dbmem_next_row - move internal pointer to the next row
 *  accepts:
 *	DBMem structure
 *  returns:
 *	next row of data
 */
char** dbmem_next_row(DBMem* dbm)
{
  
  dbm->row_curr++;
  
  /* check if we reached end of data */
  if(dbm->row_curr >= dbm->row_count)
    return NULL;
  
  return dbm->data[dbm->row_curr];
}

/* dbmem_row_at - returns row at a given position
 *  accepts:
 *      DBMem structure
 *	index of the row to be returned
 *  returns:
 *      row at position i
 */
char **dbmem_row_at(DBMem* dbm, int i)
{
  return dbm->data[i];
}

/* dbmem_current_row - returns row at the current position
 *  accepts:
 *      DBMem structure
 *  returns:
 *      row at position i
 */
char **dbmem_current_row(DBMem* dbm)
{
  return dbm->data[dbm->row_curr];
}
     
/* dbmem_find_exact - return row with value matching a given value
 *  accepts:
 *	DBMem structure
 *	key_column index, column index to compare with
 *	value to be found
 *  returns:
 *	row of data matching the value, NULL not found
 */
char** dbmem_find_exact(DBMem* dbm, char* value, int key_column)
{
  char** row;
  int i;
  for(i= 0; i < dbm->row_count; ++i)
  {
    row = dbm->data[i];
    if(row[key_column] && (strcasecmp(row[key_column], value) == 0))
    {
      dbm->row_curr = i;
      return row;
    }
  }
  return NULL;
}

/* dbmem_find_match - return row with value matching a given value
 *  accepts:
 *	DBMem structure
 *	key_column index, column index to compare with
 *	value to be found
 *  returns:
 *	row of data matching the value, NULL not found
 */
char** dbmem_find_match(DBMem* dbm, char* value, int key_column)
{
  char** row;
  int i;
  for(i= 0; i < dbm->row_count; ++i)
  {
    row = dbm->data[i];
    if(row[key_column] && (match(row[key_column], value) == 0))
    {
      dbm->row_curr = i;
      return row;
    }
  }
  return NULL;
}

/* dbmem_delete_current - returns the current row
 *  accepts:
 *	DBMem structure
 *  returns:
 *	1 on success
 :	<0 on error
 */
int dbmem_delete_current(DBMem* dbm)
{
  int field_count;
  char** curr_row;
  int i;
  assert(dbm->row_curr != -1);
  assert(dbm->row_count > 0);
  curr_row = dbm->data[dbm->row_curr];
  field_count = dbm->field_count;
  if(sql_execute("DELETE FROM %s WHERE %s=%s", dbm->table_name, 
    dbm->key_field, sql_str(curr_row[0])) == 0)
      return -1;
  /* firs lets release the data from the current row */
  for(i = 0; i < field_count; ++i)
    if(curr_row[i])
      free(curr_row[i]);
  free(curr_row);
  /* now swap with the last element */
  --(dbm->row_count);  
  dbm->data[dbm->row_curr] = dbm->data[dbm->row_count];
  return 1;
}

/* dbmem_expire - delete items which havexpired
 *  accepts:
 *	DBMem structure
 *	column of expire time and duration time
 *  returns:
 *	1 on success
 :	<0 on error
 */
int dbmem_expire(DBMem* dbm, int c_when, int c_duration)
{
  char** row;
  int field_count;
  int i, i2;
  time_t t_expire;
  int duration;
  for(i= 0; i < dbm->row_count; ++i)
  {
    row = dbm->data[i];
    duration = atoi(row[c_duration]);
    if(duration == 0) /* never expires, skip */
      continue;
    t_expire = atoi(row[c_when])+duration;
    if(t_expire < irc_CurrentTime)
    {
      if(sql_execute("DELETE FROM %s WHERE %s=%s", dbm->table_name, 
    	dbm->key_field, sql_str(row[0])) == 0)
      	  return -1;
      field_count = dbm->field_count;
      for(i2 = 0; i2 < field_count; ++i2)
      	if(row[i2])
          free(row[i2]);
      free(row);
      if(--dbm->row_count > i)
        dbm->data[i] = dbm->data[dbm->row_count];      
    }
  }
  return 1;
}

/* dbmem_free - free all memory allocated to a dbmem unit
 *  accepts:
 *	DBMem structure
 */
void dbmem_free(DBMem* dbm) 
{ 
  char **curr_row;
  int i;
  int i2;
  for(i = 0; i < dbm->row_count; ++i)
  {
    curr_row = dbm->data[i];
    for(i2 = 0; i2 < dbm->field_count; ++i2)
      if(curr_row[i2])
        free(curr_row[i2]);
    free(curr_row);
  }
  free(dbm->table_name);
  free(dbm->key_field);
  free(dbm->data);
  free(dbm);
}
