/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2004 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************  

  Description: dynamic configuration support module

 *  $Id: dbconf.c,v 1.8 2005/10/18 16:25:06 jpinto Exp $
*/

#include "module.h"
#include "my_sql.h"
#include "my_sql.h"
#define DBCONF
#include "dbconf.h"

SVS_Module mod_info =
 /* module, version, description */
{"dbconf", "1.0",  "dbconf support module" };

#define DB_VERSION	1

/** functionsand events we require **/
/* void (*FunctionPointer)(void);*/
int mysql_connection;

MOD_REQUIRES
  MOD_FUNC(mysql_connection)
MOD_END

/** function and events we provide **/
/* void my_function(void); */
int dbconf_cmd_line(int argc, char **argv);

MOD_PROVIDES
  DBCONF_FUNCTIONS
  MOD_FUNC(dbconf_cmd_line)
MOD_END

/** Internal functions declaration **/
/* void internal_function(void); */
    
/** Local variables **/
/* int my_local_variable; */
int dc_log;
    
/** load code **/
int mod_load(void)
{
  dc_log = log_open(mod_info.name, mod_info.name);

  if(dc_log<0)
  {
    errlog("Could not open dbconf log file!");
    return -1;
  }
  
  if(sql_check_inst_upgrade(mod_info.name, DB_VERSION, NULL) < 0)
    return -1;

  return 0;
}

/** unload code **/ 
void mod_unload(void) 
{
  return;
}

/** internal functions implementation starts here **/
/*
 * dbconf_get_or_build
 * For each item of the dbitems array
 *   If the value exists on the db
 *		update its description
 *		set the value pointer if found
 * else
 *		try to add item, returns -1 if fails
 * returns:
 *		-1 , error adding item
 *		>=0	number of items added to the db
 */
int dbconf_get_or_build(char *module, dbConfItem* dbitems)
{  
  int new_item = 0;  
  dbConfItem* item = dbitems;

  while(item && item->name)
  {
    if(sql_singlequery("SELECT value FROM dbconf WHERE module=%s AND name=%s"
      " ORDER BY module, name",
        sql_str(module), sql_str(item->name)) > 0)
    {
      /* read the value */
      if(!strcmp(item->type, "str") || !strcmp(item->type, "word"))
      {
        FREE(*(char **)item->vptr);
        *(char **)item->vptr = sql_field(0) ? strdup(sql_field(0)) : NULL;
      } 
      else 
      if(!strcmp(item->type,"int") && sql_field_i(0))
        *(int*) item->vptr = sql_field_i(0);
      else 
      if(!strcmp(item->type,"time") && sql_field(0))
      {
        if(ftime_str(sql_field(0)) == -1)
        {
          errlog("Invalid time value on  %s.%s",
            module, item->name);
          return -1;
        } 
        *(int*) item->vptr = ftime_str(sql_field(0));
      }
      else
      if(!strcmp(item->type,"switch") && sql_field(0))
        *(int*) item->vptr = (!strcasecmp(sql_field(0),"on")); 
        
      /* update type and description */
      sql_execute("UPDATE dbconf SET stype=%s, ddesc=%s"
        " WHERE module=%s AND name=%s",
        sql_str(item->type),
        sql_str(item->desc), sql_str(module), sql_str(item->name));    
    }
    else
    {
      sqlb_init("dbconf");
      sqlb_add_str("module", module);
      sqlb_add_str("name", item->name);      
      sqlb_add_str("stype", item->type);
      sqlb_add_str("ddesc",	item->desc);
      sqlb_add_str("optional", item->optional);
      sqlb_add_str("configured", "n");
      sqlb_add_str("value", item->def);      
      if(sql_execute(sqlb_insert()) < 0)
      {
        errlog("Error adding dbconf item %s!", item->name);
        return -1;
      }
      if(!strcmp(item->type, "str") || !strcmp(item->type, "word"))
      {
        FREE(*(char **)item->vptr);
        *(char **)item->vptr = item->def ? strdup(item->def): NULL;
      } 
      else 
      if(!strcmp(item->type, "int") && item->def)
        *(int*) item->vptr = atoi(item->def);
      else
      if(!strcmp(item->type, "switch"))
        *(int*) item->vptr = (!strcasecmp(item->def,"on"));
      else
      if(!strcmp(item->type,"time") && item->def)
      {
        if(ftime_str(item->def) == -1)
        {
          errlog("Invalid default time value on  %s.%s",
            module, item->name);
          return -1;
        } 
        *(int*) item->vptr = ftime_str(item->def);
      } 
      ++new_item;
    }
    item++;
  }
  if(new_item)
    stdlog(L_INFO, "Installed %d new configuration item(s)", new_item);
  return new_item;
}

/* change_item
 * Change a dbconf item, the value is validated accoring to the item type
 * Returns:
 * 	0  - Change was successfull
 *	-1 - Item was not found
 *	-2 - Item of type SWITCH but value is not ON/OFF
 *	-3 - Item of type TIME but value is not a time
 *	-4 - Item of type WORD but value is not a word
 *	-5 - Item of type INT but value is not an positive integer
 *	-6 - Unable to unset item, is not optional
 */ 
static int change_item(char *item, char *value)
{
  MYSQL_RES *res;
  MYSQL_ROW row;
  int error = 0;
  res = sql_query("SELECT module,name,stype,optional FROM dbconf WHERE "
    "CONCAT(module,'.', name) = %s", sql_str(item));
  if(!res || !(row = sql_next_row(res)))
    error = -1;
  else
  if((value == NULL) && (*row[3]=='n')) /* this item is not optional */
    error = -6;
  else
  if(strcasecmp(row[2],"switch") == 0) /* "SWITCH" item */
  {
    if(strcasecmp(value,"on") && strcasecmp(value,"off"))
     error = -2;
  } else
  if(strcasecmp(row[2], "time") == 0) /* "TIME" item */
  {
    if(ftime_str(value) == -1)
     error = -3;
  } else
  if((strcasecmp(row[2], "word") == 0) && value) /* "WORD" item */
  {
    if(strchr(value, ' '))
      error = -4;
  } else
  if(strcasecmp(row[2], "int") == 0) /* "INT" item */
  {
    if(!is_posint(value))
     error = -5;
  }

  sql_free(res);
  if(error)
    return error;
  if(sql_execute("UPDATE dbconf SET value=%s "
    "WHERE CONCAT(module,'.',name)=%s", sql_str(value), sql_str(item)) < 0)
      return -6;
      
  return 0;
}

int dbconf_cmd_line(int argc, char **argv)
{
  const char* usage = "Usage:\n"
    "ircsvs conf list [pattern]\n"
    "ircsvs conf export [pattern]\n"
    "ircsvs conf set module.setting value\n"
    "ircsvs conf unset module.setting\n";
  char* cmd;
  char buf[128];
  
  if(argc<1)
  {
    printf("%s", usage);
    return -1;
  }
  cmd = argv[0];  
  if((strcasecmp(cmd, "list") == 0) || (strcasecmp(cmd, "export") == 0))
  {
    MYSQL_RES *res;
    MYSQL_ROW row;
    char *where;
    int is_export = (strcasecmp(cmd, "export") == 0);
    
    if(argc>1)
    {
      char buf2[128];
      snprintf(buf2, sizeof(buf2), "%%%s%%", argv[1]);
      snprintf(buf, sizeof(buf), " WHERE CONCAT(module,'.',name) LIKE %s", 
        sql_str(buf2));
      where = buf;
    }
    else 
      where = "";
    res = sql_query("SELECT module, name, value, ddesc, stype, optional"
      " FROM dbconf %s", where); 
      
    printf("####### Configuration list #######\n");
    while((row = sql_next_row(res)))
    {
      char *line;
      line = row[3];
      /* show each line from the ddesc field prefixed with # */
      while(line)
      {
        char *p = line;
        char *c = strchr(line,'\n');
        if(c)
        {
          *c = '\0';
          line = c+1;
        } else line = NULL;
        printf("# %s\n", p);
      }
      if(strcmp(row[4],"switch") == 0)
        printf("# This is a switch option, possible values are On or Off\n");
      else
      if(strcmp(row[4],"time") == 0)
        printf("# Time value [m=minutes;h=hours;d=days;M=months,Y=years]\n");
      else
      if(*row[5] == 'y')
        printf("# This setting is optional, you can unset to disable\n");
      if(is_export)
      {
        if(strcmp(row[4],"word") && strcmp(row[4],"str"))
          printf("./ircsvs conf set %s.%s %s", row[0], row[1], row[2] ? row[2] : "NULL");
        else
        if(row[2])
          printf("./ircsvs conf set %s.%s \"%s\"", row[0], row[1], row[2]);
        else
          printf("./ircsvs conf unset %s.%s", row[0], row[1]);
      }
      else
      {
        if(strcmp(row[4],"word") && strcmp(row[4],"str"))
          printf("%s.%s = %s", row[0], row[1], row[2] ? row[2] : "NULL");
          else
        if(row[2])
          printf("%s.%s = \"%s\"", row[0], row[1], row[2]);
        else
          printf("%s.%s = *NOT SET*", row[0], row[1]);
      }
      printf("\n\n");
    }
    printf("##################################\n");
    sql_free(res);
  } else
  if((strcasecmp(cmd, "set") == 0) || (strcasecmp(cmd, "unset") == 0))  
  {
    int r;
    const char *msg = NULL;
    int unset = (strcasecmp(cmd, "unset") == 0);
    if((!unset && (argc < 3)) || !strchr(argv[1], '.'))
    {
      printf("%s", usage);
      return -1;
    }
    r =  change_item(argv[1], unset ? NULL : argv[2]);
    switch(r) 
    {
      case 0:
        msg = NULL;
        break;
      case -1: 
        msg = "There is no item %s !\n";
         break;
      case -2:
        msg = "Value for %s must be On/Off !\n";
        break;
      case -3:
        msg = "Value for %s must be a time value !\n";
        break;
      case -4:
        msg = "Value for %s must be a word !\n";
        break;
      case -5:
        msg = "Value for %s must be a positive integer !\n";
        break;
      case -6:
        msg = "Value for %s can't be unset, is not an optional setting!\n";
        break;
      default:
        msg = "Unknown error changing %s !\n";
        break;
    }
    /* check for errors */
    if(msg)
    {
      printf(msg, argv[1]);
      return r;
    }
    if(unset)
      printf("%s successfully unset\n", argv[1]);
    else
      printf("%s successfully changed to: %s\n", argv[1], argv[2]);
  }
  return 0;
};

/* dbconf_get
 * For each item of the dbitems array
 *	Get the value from the dbconf table
 *	If the item is missing, report error and return -1
 *	If the value is null and the item is mandatory, report error
 *		and return -2
 * return 0 on success
 */
int dbconf_get(dbConfGet *dbitems)
{
  dbConfGet* item = dbitems;
  char *stype;
  while(item && item->name)
  {
    if(sql_singlequery("SELECT value, stype, optional FROM dbconf WHERE module=%s AND name=%s"
      " ORDER BY module, name",
        sql_str(item->module), sql_str(item->name)) > 0)
    {
      stype = sql_field(1);
      /* read the value */
      if(!strcmp(stype, "str") || !strcmp(stype, "word"))
      {
        FREE(*(char **)item->vptr);
        *(char **)item->vptr = sql_field(0) ? strdup(sql_field(0)) : NULL;
      } 
      else 
      if(!strcmp(stype,"int") && sql_field_i(0))
        *(int*) item->vptr = sql_field_i(0);
      else
      if(!strcmp(stype,"time") && sql_field(0))
      {
        if(ftime_str(sql_field(0)) == -1)
        {
          errlog("Invalid time value on  %s.%s",
            item->module, item->name);
          return -1;
        }
        *(int*) item->vptr = ftime_str(sql_field(0));
      }
      if(!strcmp(stype,"switch") && sql_field(0))
        *(int*) item->vptr = (!strcasecmp(sql_field(0),"on"));   
    } else
    {
      errlog("Unable to find configuratiom item %s.%s", 
        item->module, item->name);
      return -1;
    }
    if((*(sql_field(2)) == 'n') && (item->vptr == NULL))
    {
      errlog("Mandatory item %s.%s is not set!",
        item->module, item->name);
      return -2;
    }
    ++item;
  }
  return 0;
}

/* End of module */
