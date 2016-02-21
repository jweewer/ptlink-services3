/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************

  Desc: sql builder

 *  $Id: sqlb.c,v 1.3 2005/10/27 22:27:02 jpinto Exp $
*/
#include "stdinc.h"
#include "my_sql.h"	/* we need sql_str( */
#include "strhand.h"	/* we need FREE( */
#include "sqlb.h"

static char my_table[64];
static char* f_names[128]; /* field names */
static int fcount = 0; /* fields count */
static char* f_values[128]; /* field values */
static int f_types[128]; /* filed types */

#ifndef SQL_STRING
#define SQL_STRING 1
#endif
#ifndef SQL_INT
#define SQL_INT 2
#endif


void sqlb_init(char *table)
{
  int i;
  /* first free any existing items */
  for(i = 0; i < fcount; ++i)
  {
    FREE(f_names[i]);
    FREE(f_values[i]);
  }
  strncpy(my_table, table, sizeof(my_table));
  fcount = 0;
}

void sqlb_add_str(char *name, char *value)
{ 
  f_names[fcount] = strdup(name);
  f_values[fcount] = value ? strdup(value) : NULL;
  f_types[fcount] = SQL_STRING;
  ++fcount;
}

void sqlb_add_int(char *name, u_int32_t value)
{ 
  char buf[12];
  f_names[fcount] = strdup(name);
  snprintf(buf, 12, "%d", value);
  f_values[fcount] = strdup(buf);
  f_types[fcount] = SQL_INT;
  ++fcount;
}

void sqlb_add_func(char *name, char* value)
{ 
  f_names[fcount] = strdup(name);
  f_values[fcount] = strdup(value);
  f_types[fcount] = SQL_INT;
  ++fcount;
}

char *sqlb_insert(void)
{
  static char buf[4096];
  int i;
  int len;
  
  len = snprintf(buf, sizeof(buf), "INSERT INTO %s(", my_table); 
  for(i = 0; i<fcount; ++i)
  {    
    len += snprintf(buf+len, sizeof(buf)-len, "%s", f_names[i]);
    if(i < fcount-1)
      buf[len++] = ',';
  }
  len += snprintf(buf+len, sizeof(buf)-len, ") VALUES (");
  for(i = 0; i<fcount; ++i)
  {
    if(f_types[i] == SQL_STRING)
      len += snprintf(buf+len, sizeof(buf)-len, "%s", sql_str(f_values[i]));
    else
      len += snprintf(buf+len, sizeof(buf)-len, "%s", f_values[i]);
      
    if((i < fcount-1) &&  (sizeof(buf)-len-1>0))
      buf[len++] = ',';
  }
  snprintf(buf+len, sizeof(buf)-len, ")");
  return buf;
}
