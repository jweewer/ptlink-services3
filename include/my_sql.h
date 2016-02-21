/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************  

  Desc: mysql routines header file
                                                                                
 *  $Id: my_sql.h,v 1.5 2005/10/31 13:01:14 jpinto Exp $
*/

#ifndef _MYSQL_H_
#define _MYSQL_H_
#include <errmsg.h>
#include <mysql.h>
#include <mysqld_error.h>
#include "chanrecord.h"
#define	MAX_SQL_BUF	2048
#define MYSQL_ERROR 1
#define MYSQL_WARNING 2

int db_mysql_open(void);
int sql_close(void);
u_int32_t nick2snid(char *name);
u_int32_t db_mysql_chan2scid(char *name);
int db_mysql_build_snid_hash(void);
int reg_count_for_email(char *email);
int db_mysql_init(void);
int db_mysql_set_builtin(void);
int db_mysql_delete_hosrtule(int id);

/* chanrecord*/
int db_mysql_insert_cr(ChanRecord* cr);
int db_mysql_update_cr(ChanRecord* cr);
ChanRecord* db_mysql_get_cr(u_int32_t scid);

/* raw sql functions */
int db_mysql_query(char *sql);
void db_mysql_error(int severity, char *msg);

/* aux sql functions */
MYSQL_RES* sql_query(char *fmt, ...);
u_int32_t sql_execute(char *fmt, ...);
MYSQL_ROW sql_next_row(MYSQL_RES *res);
MYSQL_RES* sql_query_get(MYSQL_ROW *row);
void sql_free(MYSQL_RES *res);
int sql_singlequery(char *fmt, ...);
char*  sql_field(int i);
u_int32_t sql_field_i(int i);
MYSQL_RES* sql_use_result(void);
u_int32_t sql_last_id(void);
int sql_field_count(void);
int sql_find_module(char *module_name);

/* aux sql string functions */
char* sql_string(char *str);
char* sql_str(const char *str);

int mysql_from_file(char *fn);
const char *sql_error(void);
int sql_errno(void);
MYSQL_RES* sql_result(void);
int sql_check_inst_upgrade(char *name, int version, void* update_func);

/**************************************************************************
 DBMem functions
 **************************************************************************/
struct DBMem_s
{
  char *table_name;
  int alloc_units;
  int row_count;
  int row_max;
  int row_curr;
  int field_count;
  char ***data;
  char* key_field;
};
typedef struct DBMem_s DBMem;

DBMem* dbmem_init(char *table_name, int alloc_units);
int dbmem_increase(DBMem* dbm);
int dbmem_load(DBMem* dbm);
int dbmem_insert(DBMem* dbm, char** row);
char** dbmem_first_row(DBMem* dbm);
char** dbmem_next_row(DBMem* dbm);
char** dbmem_find_exact(DBMem* dbm, char* value, int key_column);
char** dbmem_find_match(DBMem* dbm, char* value, int key_column);
int dbmem_delete_current(DBMem* dbm);
int dbmem_expire(DBMem* dbm, int c_when, int c_duration);
char **dbmem_row_at(DBMem* dbm, int i);
void dbmem_free(DBMem* dbm);
char **dbmem_current_row(DBMem* dbm);
int dbmem_replace_key(DBMem* dbm, char* new_key);
#endif /* _MYSQL_H_ */

