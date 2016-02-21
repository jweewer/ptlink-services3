/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************

  Description: nickserv group command

 *  $Id: ns_group.c,v 1.18 2005/10/18 16:25:06 jpinto Exp $
*/

#include "module.h"
/* modules we depend on */
#include "my_sql.h"
#include "nsmacros.h"
#include "dbconf.h"
/* lang files */
#include "lang/common.lh"
#include "lang/ns_group.lh"

SVS_Module mod_info =
/* module, version, description */
{"ns_group", "3.2", "nickserv group command" };

#define DB_VERSION 	3

/* Change Log
  3.2 - #22 : e_nick_recognize to distinguish +r nick recognitions
  3.1 - 0000350: ns_group subcommands detailed help
        0000337: Option to set maxusers on groups
        0000328: group members can be added with a given expire time
          Added ns_group.3.sql changes
  3.0 - 0000308: group names can have "@server" for "from server" restriction
        0000307: support for auto user modes on groups
        0000305: foreign keys for data integrity
          Added ns_group.2.sql changes
  2.0 - 0000265: remove nickserv cache system
  1.2 - we don't need irc_lower()
  1.1 - 0000247: cache nick groups info
*/

#define ED_GROUPS       0
    
/** functions and events we require **/
ServiceUser* (*nickserv_suser)(void);
static int e_nick_identify;
static int e_nick_recognize;
static int e_expire;


MOD_REQUIRES
  DBCONF_FUNCTIONS
  MOD_FUNC(e_expire) /* we need this to run the expire routines */
  MOD_FUNC(nickserv_suser)
  MOD_FUNC(e_nick_identify)
  MOD_FUNC(e_nick_recognize)
MOD_END

/* functions and events we provide */
int is_sadmin(u_int32_t snid);
int is_sroot(u_int32_t snid);
int is_soper(u_int32_t snid);
u_int32_t find_group(char *name);
int is_member_of(u_int32_t snid, u_int32_t sgid);

MOD_PROVIDES
   MOD_FUNC(is_sadmin)
   MOD_FUNC(is_sroot)
   MOD_FUNC(is_soper)
   MOD_FUNC(find_group)
   MOD_FUNC(is_member_of)
MOD_END

/** Internal functions declaration **/
void create_core_groups(void);
void ns_group(IRC_User *s, IRC_User *u);
int is_member_of_online(IRC_User *user, u_int32_t sgid);
int group_create(char *name, u_int32_t master_sgid, char *gdesc, char *umodes);
int add_to_group(u_int32_t sgid, u_int32_t snid, time_t t_expire);
int del_from_group(u_int32_t sgid, u_int32_t snid);
int drop_group(u_int32_t sgid);
int is_master(u_int32_t snid, u_int32_t sgid);
int ev_ns_group_nick_identify(IRC_User* u, u_int32_t* snid);
u_int32_t find_group(char *name);
int ev_ns_group_expire(void* dummy1, void* dummy2);
int group_is_full(u_int32_t sgid);

/** Local variables **/
ServiceUser *nsu;
int ns_log;

/* Remote config */
static char* Root;
DBCONF_REQUIRES
  DBCONF_GET("nickserv", Root)
DBCONF_END
/* Local config*/
static int ExpireWarningTime;
DBCONF_PROVIDES
  DBCONF_TIME(ExpireWarningTime, "5d",
    "Warn users with group membership exping in less than ExpireWarningTime")
DBCONF_END

/* this is called before load and at services rehash */
int mod_rehash(void)
{
  if(dbconf_get(dbconf_requires) < 0)
  {
    errlog("Error reading dbconf!");
    return -1;
  }

  if(dbconf_get_or_build(mod_info.name, dbconf_provides) < 0 )
  {
    errlog("Error reading dbconf!");
    return -1;
  }  
  return 0;
}

/* module load code */
int mod_load(void)
{
  int r;
  
  ns_log = log_handle("nickserv");
  
  r = sql_check_inst_upgrade(mod_info.name, DB_VERSION, NULL);
  
  if(r < 0)
    return -4;
  else if(r == 1) /* table was installed */
    create_core_groups();
  else
    {
      if(Root)
        stdlog(L_WARN, "Root is defined, please add a nick to the Root group and disable the setting");
    }
      
  nsu = nickserv_suser();
  
  suser_add_cmd(nsu, "GROUP", ns_group, NS_GROUP_SUMMARY, NS_GROUP_HELP);

  /* Add event actions */    
  mod_add_event_action(e_nick_identify, (ActionHandler) ev_ns_group_nick_identify);
  mod_add_event_action(e_nick_recognize, (ActionHandler) ev_ns_group_nick_identify);
  mod_add_event_action(e_expire, (ActionHandler) ev_ns_group_expire);

  /* Add subcommands help */ 
  suser_add_help(nsu, "GROUP ADD", NS_GROUP_ADD_HELP);
  suser_add_help(nsu, "GROUP CREATE", NS_GROUP_CREATE_HELP);
  suser_add_help(nsu, "GROUP DROP", NS_GROUP_DROP_HELP);
  suser_add_help(nsu, "GROUP DEL", NS_GROUP_DEL_HELP);
  suser_add_help(nsu, "GROUP INFO", NS_GROUP_INFO_HELP);
  suser_add_help(nsu, "GROUP LIST", NS_GROUP_LIST_HELP);
  suser_add_help(nsu, "GROUP SET", NS_GROUP_SET_HELP);
  suser_add_help(nsu, "GROUP SHOW", NS_GROUP_SHOW_HELP);  
  
  return 0;
}

void mod_unload(void)
{
  suser_del_mod_cmds(nsu, &mod_info);
}

/** internal functions implementation starts here **/

#define STRING_SET(x,y,z) \
  { \
    if(IsNull(value)) \
      { \
        log_log(ns_log, mod_info.name, "%s GROUP %s UNSET %s", \
          u->nick, gname, option);\
        send_lang(u, s, (y), gname); \
      } \
    else \
      { \
        log_log(ns_log, mod_info.name, "%s GROUP %s SET %s %s", \
          u->nick, gname, option, value);\
        send_lang(u, s, (z), gname, value); \
      } \
     sql_execute("UPDATE ns_group SET %s=%s "\
       "WHERE sgid=%d", (x), sql_str(value), sgid);\
  }

#define INT_SET(x,y) \
  { \
    if(IsNull(value)) \
      send_lang(u, s, REQUIRES_NUMERIC_X, option); \
    else \
      { \
        log_log(ns_log, mod_info.name, "%s GROUP %s SET %s %d", \
          u->nick, gname, option, atoi(value));\
        send_lang(u, s, (y), gname, atoi(value)); \
        sql_execute("UPDATE ns_group SET %s=%d " \
          "WHERE sgid=%d", (x), atoi(value), sgid); \
      } \
  }

/* s = service the command was sent to
   u = user the command was sent from */
void ns_group(IRC_User *s, IRC_User *u)
{
  u_int32_t source_snid;
  u_int32_t snid;
  char *cmd;
  char *gname;
  char *nick;
  int memberc = 0;
  u_int32_t master_sgid;
  u_int32_t sgid;

  CHECK_IF_IDENTIFIED_NICK
  
  cmd = strtok(NULL, " ");
  gname = strtok(NULL, " ");
  
  /* base syntax validation */
  if(IsNull(cmd))
    send_lang(u, s, NS_GROUP_SYNTAX);
  else if(strcasecmp(cmd,"CREATE") == 0)
    {
      char *master;
      char *gdesc;
      char *umodes = NULL;
      master = strtok(NULL, " ");
      gdesc = strtok(NULL, ""); 
      if(gname) /* first check if the name contains umodes */
      {
        char *pumodes;
        char *eumodes;
        pumodes = strchr(gname,'[');
        if(pumodes && pumodes[0])
        {
          *(pumodes++) = '\0';
          eumodes = strchr(pumodes,']');
          if(eumodes)
          {
            *eumodes = '\0';
            umodes = pumodes;
          }
        }
      }
      /* syntax validation */
      if(IsNull(gname) || IsNull(master))
        send_lang(u, s, NS_GROUP_CREATE_SYNTAX);
      /* permissions validation */
      else if(!is_sroot(source_snid))
        send_lang(u, s, NICK_NOT_ROOT);
      /* check requirements */
      else if((master_sgid = find_group(master)) == 0)
        send_lang(u, s, NS_GROUP_MASTER_NOT_FOUND, master);
      /* avoid duplicates */
      else if((sgid = find_group(gname)) != 0)
        send_lang(u, s, NS_GROUP_ALREADY_EXISTS, gname);
      /* execute operation */
      else if(group_create(gname, master_sgid, gdesc, umodes) > 0)
      /* report operation status */
        send_lang(u, s, NS_GROUP_CREATE_OK, gname);
      else 
        send_lang(u, s, UPDATE_FAIL);
    }
  else if(strcasecmp(cmd,"ADD") == 0)
    {
      u_int32_t duration = 0;
      time_t master_expire = 0;              
      u_int32_t is_master_sgid;
      char *duration_str;
      
      nick = strtok(NULL, " ");
      duration_str =  strtok(NULL, " ");
      if(duration_str)
        duration = time_str(duration_str);
        
      /* syntax validation */
      if(IsNull(gname) || IsNull(nick))
        send_lang(u, s, NS_GROUP_ADD_SYNTAX);
      /* check requirements */
      else if((snid = nick2snid(nick)) == 0)
        send_lang(u, s, NO_SUCH_NICK_X, nick);      
      else if((sgid = find_group(gname)) == 0)
        send_lang(u, s, NO_SUCH_GROUP_X, gname);
      /* privileges validation */
      else if(group_is_full(sgid))
        send_lang(u, s, NS_GROUP_IS_FULL_X);
      else if(((is_master_sgid = is_master(source_snid, sgid))== 0) 
        && !is_sroot(source_snid))
          send_lang(u, s, NOT_MASTER_OF_X, gname);
      /* avoid duplicates */
      else if(sql_singlequery("SELECT t_expire FROM ns_group_users "
        " WHERE sgid=%d AND snid=%d", is_master_sgid, source_snid)
        && (master_expire = sql_field_i(0)) && duration)
        send_lang(u, s, NS_GROUP_CANT_DEFINE_TIME_X, gname);
      else if(is_member_of(snid, sgid))
        send_lang(u, s, NICK_X_ALREADY_ON_X, nick, gname);
      /* execute operation */
      else
      {
        time_t t_expire = 0;
        if(master_expire)
          t_expire = master_expire;
        else if(duration)
          t_expire = irc_CurrentTime + duration;
        if(add_to_group(sgid, snid, t_expire) > 0)
        /* report operation status */
        {   
          char *server = strchr(gname, '@');      
          IRC_User *user = irc_FindUser(nick);   
          send_lang(u, s, NICK_ADDED_X_X, nick, gname);
          if(server) /* we have a server rule to be validated */
            ++server;
          if(user && (!server || (strcasecmp(server,u->server->sname) == 0)))
          {
            if(user->extra[ED_GROUPS] == NULL)
            {
              user->extra[ED_GROUPS] = malloc(sizeof(darray));
              array_init(user->extra[ED_GROUPS], 1, DA_INT);
            }
            array_add_int(user->extra[ED_GROUPS], sgid);
          }
        }
      	else
          send_lang(u, s, UPDATE_FAIL);        
      }
    }
  else if(strcasecmp(cmd,"DEL") == 0)
    {
      nick = strtok(NULL, " ");
      /* syntax validation */ 
      if(IsNull(gname) || IsNull(nick))
        send_lang(u, s, NS_GROUP_DEL_SYNTAX);
      /* check requirements */
      else if((sgid = find_group(gname)) == 0)
        send_lang(u, s, NO_SUCH_GROUP_X, gname);
      else if((snid = nick2snid(nick)) == 0)
        send_lang(u, s, NO_SUCH_NICK_X, nick);        
      /* privileges validation */
      else if(!is_sroot(source_snid) && !is_master(source_snid, sgid))
        send_lang(u, s, NOT_MASTER_OF_X, gname);
      else if(!is_member_of(snid, sgid))
        send_lang(u, s, NICK_X_NOT_ON_GROUP_X, nick, gname);
      /* execute operation */
      else if(del_from_group(sgid, snid) > 0)
      /* report operation status */
        {
          IRC_User *user = irc_FindUser(nick);
          send_lang(u, s, NICK_DEL_X_X, nick, gname);
          if(user)
            array_del_int(user->extra[ED_GROUPS], sgid);
        }
      else
        send_lang(u, s, UPDATE_FAIL);
    }
  else if(strcasecmp(cmd,"INFO") == 0)
    {
      /* syntax validation */
      if(IsNull(gname))
        send_lang(u, s, NS_GROUP_INFO_SYNTAX);
      /* check requirements */
      else if((sgid = find_group(gname)) == 0)
        send_lang(u, s, NO_SUCH_GROUP_X, gname);
      /*  check privileges */
      else if(!is_master(source_snid, sgid) && 
              !is_member_of(source_snid, sgid))
        send_lang(u, s, NOT_MASTER_OR_MEMBER_X, gname);      
      else if((sgid = find_group(gname))) /* we need to get the group description */
        {
          /* execute operation */  
          MYSQL_RES* res;
          master_sgid = 0;
          sql_singlequery("SELECT gdesc, master_sgid FROM ns_group WHERE sgid=%d", 
            sgid);  
          send_lang(u, s, NS_GROUP_INFO_X, gname);        
          if(sql_field(0))
            send_lang(u, s, NS_GROUP_INFO_DESC_X, sql_field(0));
          master_sgid = sql_field_i(1);
          if(master_sgid != 0)
          {
            if(sql_singlequery("SELECT name FROM ns_group WHERE sgid=%d", 
                master_sgid) > 0)
            {
              send_lang(u, s, NS_GROUP_INFO_MASTER_X, sql_field(0));              
            }
          }
          res = sql_query("SELECT n.nick, gm.t_expire FROM "
            "nickserv n, ns_group_users gm WHERE gm.sgid=%d AND n.snid=gm.snid", 
            sgid);
          if(sql_next_row(res) == NULL)
            send_lang(u, s, NS_GROUP_EMPTY);
          else
          {
            do
            {
      				char buf[64];
      				struct tm *tm;
      				time_t t_expire = sql_field_i(1);
							buf[0] = '\0';
							if(t_expire)
							{
      					tm = localtime(&t_expire);
      					strftime(buf, sizeof(buf), format_str(u, DATE_FORMAT), tm);
              	send_lang(u,s, NS_GROUP_ITEM_X_X, sql_field(0), buf);
							} else
                send_lang(u,s, NS_GROUP_ITEM_X, sql_field(0));
              ++memberc;
            } while(sql_next_row(res));
            send_lang(u, s, NS_GROUP_MEMBERS_TAIL_X, memberc);
          }
          sql_free(res);
        }  
    }
  else if(strcasecmp(cmd,"DROP") == 0)
    {
      /* syntax validation */
      if(IsNull(gname))
        send_lang(u, s, NS_GROUP_DROP_SYNTAX);
      /* privileges validation */
      else if(!is_sroot(source_snid))
        send_lang(u, s, NICK_NOT_ROOT);
      /* check requirements */
      else if((sgid = find_group(gname)) == 0)
        send_lang(u, s, NO_SUCH_GROUP_X, gname);
      /* NOTE: The following sql_field( depends on previous find_group( */
      else if(!sql_field(2) || (master_sgid = atoi(sql_field(2))) == 0)
        send_lang(u, s, CANT_DROP_ROOT);        
      /* execute operation */
      else if(drop_group(sgid)>0)      
      /* report operation status */
        send_lang(u, s, NS_GROUP_DROPPED_X, gname);
      else
        send_lang(u, s, UPDATE_FAIL);
    }
  else if(strcasecmp(cmd,"LIST") == 0) /* List groups */
    {
      MYSQL_RES* res;
      MYSQL_ROW row;
      /* privileges validation */
      if(!is_sroot(source_snid))
        send_lang(u, s, NICK_NOT_ROOT);
      else 
        {
          res = sql_query("SELECT name, master_sgid, gdesc FROM ns_group");
          send_lang(u, s, NS_GROUP_LIST_HEADER);
          while((row = sql_next_row(res)))
            {
              char* mname = "";
              if(row[1] && sql_singlequery("SELECT name FROM ns_group WHERE sgid=%d", 
                atoi(row[1])) > 0)
                mname = sql_field(0);          
              send_lang(u, s, NS_GROUP_LIST_X_X_X, row[0], mname, 
                row[2] ? row[2] : "");
            }
          send_lang(u, s, NS_GROUP_LIST_TAIL);
          sql_free(res);          
        }
    }
  else if(strcasecmp(cmd,"SHOW") == 0) /* Show groups we belong to */
    {
      /* groups count */
      int gc = array_count(u->extra[ED_GROUPS]);
      if(gc == 0)
        send_lang(u, s, NO_GROUPS);
      else
      {
        MYSQL_RES *res;
        MYSQL_ROW row;
        char buf[64];
        struct tm *tm;
        time_t t_expire;
#if 0        
        int i;        
        u_int32_t* data = array_data_int(u->extra[ED_GROUPS]);
#endif        
        send_lang(u, s, NS_GROUP_SHOW_HEADER);
#if 0        
        for(i = 0; i < gc; ++i)
        {
          if(sql_singlequery("SELECT name,gdesc FROM ns_group WHERE sgid=%d", 
            data[i]) > 0 )              
              send_lang(u, s, NS_GROUP_SHOW_X_X, sql_field(0), 
                sql_field(1) ? sql_field(1) : "");
        }
#endif
        res = sql_query("SELECT g.name, g.gdesc, gu.t_expire FROM ns_group g, ns_group_users gu"
          " WHERE gu.snid=%d AND g.sgid=gu.sgid ORDER BY g.master_sgid",
          source_snid);
        while((row = sql_next_row(res)))
        {
          t_expire = sql_field_i(2);
					buf[0] = '\0';
					if(t_expire)
					{
      		  tm = localtime(&t_expire);
            strftime(buf, sizeof(buf), format_str(u, DATE_FORMAT), tm);
            send_lang(u,s, NS_GROUP_SHOW_X_X_X, row[0], row[1] ? row[1] : "", buf);        
          }
          else
            send_lang(u, s, NS_GROUP_SHOW_X_X, row[0], row[1] ? row[1] : "");
        }
        send_lang(u, s, NS_GROUP_SHOW_TAIL);
        sql_free(res);
      }
    }
  else if(strcasecmp(cmd,"SET") == 0)
  {
    char *option;
    char *value ;
    option = strtok(NULL, " ");
    value = strtok(NULL, " ");
    /* syntax validation */
    if(IsNull(gname) || IsNull(option))
      send_lang(u, s, NS_GROUP_SET_SYNTAX);
    /* privileges validation */
    else if(!is_sroot(source_snid))
      send_lang(u, s, NICK_NOT_ROOT);
    /* check requirements */
    else if((sgid = find_group(gname)) == 0)
      send_lang(u, s, NO_SUCH_GROUP_X, gname);
    else
    {
      if(strcasecmp(option,"AUTOMODES") == 0)
        STRING_SET("autoumodes", AUTOMODES_X_UNSET, AUTOMODES_X_CHANGED_TO_X)
      else if(strcasecmp(option,"DESC") == 0)
        STRING_SET("gdesc", DESC_X_UNSET, DESC_X_CHANGED_TO_X)
      else if(strcasecmp(option, "MAXUSERS") == 0)
        INT_SET("maxusers", NS_GROUP_SET_MAXUSERS_SET_X_X)
      else
        send_lang(u, s, SET_INVALID_OPTION_X, option);
    }
  }
  else
    send_lang(u, s, NS_GROUP_SYNTAX);
}

/**
  Checks if a given nick is services root 
  Ret:
    0 = no
    >0 = yes
*/
int is_sadmin(u_int32_t snid)
{

  if(snid)
    return is_member_of(snid, find_group("Admin"));
    
  return 0;
};

/**
  Checks if a given snid is services root 
  Ret:
    0 = no
    >0 = yes
*/
int is_sroot(u_int32_t snid)
{
  if(snid == 0)
    return 0;

  if(Root)
  { 
    u_int32_t root_snid = nick2snid(Root);
    if(root_snid && (root_snid == snid))
      return 1;
  }
    
  return is_member_of(snid, find_group("Root"));  
}

/**
  Checks if a given snid is oper
  Ret:
    0 = no
    >0 = yes
*/
int is_soper(u_int32_t snid)
{
  if(snid == 0)
    return 0;
    
  if(Root)
  { 
    u_int32_t root_snid = nick2snid(Root);
    if(root_snid && (root_snid == snid))
      return 1;
  }      
  
  return is_member_of(snid, find_group("Oper"));  
}

void create_core_groups(void)
{
  u_int32_t lastg;
  log_log(ns_log, mod_info.name, "Creating core groups");
  lastg = group_create("Root", 0, "Services Root", NULL);
  lastg = group_create("Admin", lastg, "Services Administrators", NULL);
  lastg = group_create("Oper", lastg, "Services Operators", NULL);
}

int is_member_of(u_int32_t snid, u_int32_t sgid)
{
  MYSQL_RES *res;
  MYSQL_ROW row;
  int is = 0;
  res = sql_query("SELECT snid FROM ns_group_users WHERE snid=%d and sgid=%d",
      snid, sgid);
  row = sql_next_row(res);
  if(row)
    is = 1;
  sql_free(res);
  return is;
}

int is_member_of_online(IRC_User* user, u_int32_t sgid)
{
  return (array_find_int(user->extra[ED_GROUPS], sgid) != -1);
}

/* Creates a nick group 
 Returns: 0<= on error
     1 on sucess
*/
int group_create(char *name, u_int32_t master_sgid, char *gdesc, char *umodes)
{

  if(strlen(name) > 128) /* truncate description */
    name[32] = '\0';  
 
  if(gdesc && strlen(gdesc) > 128) /* truncate description */
    gdesc[255] = '\0';    
  
  /* all conditions are met, lets create the group */
  return sql_execute("INSERT INTO ns_group (name, master_sgid, gdesc, autoumodes, maxusers)"
    "VALUES (%s, %d, %s, %s, 9999)", sql_str(name), master_sgid, sql_str(gdesc),
      umodes ? sql_str(umodes) : "NULL");

}

int add_to_group(u_int32_t sgid, u_int32_t snid, time_t t_expire)
{
  
  return sql_execute("INSERT INTO ns_group_users (sgid, snid, t_expire) VALUES (%d, %d, %d)",
    sgid, snid, t_expire);
}

int del_from_group(u_int32_t sgid, u_int32_t snid)
{
  return sql_execute("DELETE FROM ns_group_users WHERE sgid=%d AND snid=%d",
   sgid, snid);
}

int drop_group(u_int32_t sgid)
{
  u_int32_t master_sgid = 0;
  
  /* first retrieve the group master */
  if(sql_singlequery("SELECT master_sgid from ns_group WHERE sgid=%d", sgid) == 0)
    {
      log_log(ns_log, mod_info.name, "Attempt to drop masterless groupd %d", sgid);
      return 0;
    }  
  if(sql_field(0))
    master_sgid = atoi(sql_field(0));
  /* if it is master of some role, point his master to our own master */
  sql_execute("UPDATE ns_group SET master_sgid=%d WHERE master_sgid=%d", 
    master_sgid, sgid);
  return sql_execute("DELETE FROM ns_group WHERE sgid=%d", sgid);
}           


/** checks if a given snid is master of a given sgid
  Returns: 0 if not, sgid of the group which makes it master
 */
int is_master(u_int32_t snid, u_int32_t sgid)
{

  MYSQL_RES *res;
  MYSQL_ROW row;
  
  while(sgid != 0)
    {
      res = sql_query("SELECT master_sgid FROM ns_group WHERE sgid=%d", sgid);
      row = sql_next_row(res);
      if(row == NULL)
      {
        sql_free(res);
        return 0; /* reached a masterless group */
      }
        
      if(row[0] && atoi(row[0]))
        sgid = atoi(row[0]);
      else 
        sgid = 0;
      sql_free(res);
      
      /* check if snid is member of master */
      res = sql_query("SELECT snid FROM ns_group_users WHERE sgid=%d AND snid=%d",
          sgid, snid);
      
      if(sql_next_row(res))
      {
        sql_free(res);
        return sgid; /* found */
      }
      sql_free(res);
    }
    
  return 0;
}

/* this event is called when a nick is identified 
   we will load the groups a nick belongs to
*/
int ev_ns_group_nick_identify(IRC_User* u, u_int32_t* snid)
{

  MYSQL_RES* res;
  MYSQL_ROW row;
  int rowc = 0;
  res = sql_query("SELECT gu.sgid, g.autoumodes, g.name, gu.t_expire FROM ns_group_users gu, ns_group g WHERE gu.snid=%d"
    " AND g.sgid=gu.sgid",
    u->snid);
  if(res)
    rowc = mysql_num_rows(res);
  if(u->extra[ED_GROUPS] != NULL)
    array_free(u->extra[ED_GROUPS]);
  u->extra[ED_GROUPS] = malloc(sizeof(darray));
  array_init(u->extra[ED_GROUPS], rowc, DA_INT);
  while((row = sql_next_row(res)))
  {
    char *gname = row[2];
    char *server = strchr(gname, '@');
    time_t t_expire = atoi(row[3]);
    if(server) /* we have a server rule to be validated */
    {
      ++server;
      if(strcasecmp(server, u->server->sname) && 
      (u->vlink && strcasecmp(server, u->vlink)))
        continue;
    }
    if(t_expire && ExpireWarningTime &&
      ((t_expire - irc_CurrentTime) < ExpireWarningTime))
      send_lang(u, nsu->u, NS_GROUP_X_EXPIRING_X,
        gname, (t_expire - irc_CurrentTime)/(24*3600));
    array_add_int(u->extra[ED_GROUPS], atoi(row[0]));
    if(row[1] && row[1][0])
      irc_SvsMode(u, nsu->u, row[1]);
  }
  sql_free(res);
  
  return 0;
}

/* check if a given group reached maxusers */
int group_is_full(u_int32_t sgid)
{
  int maxusers = 0;
  if(sql_singlequery("SELECT maxusers FROM ns_group WHERE sgid=%d", sgid))
    maxusers = sql_field_i(0);
  else
    return 1; /* this should never happen */
    
  if(maxusers == 0)
    return 0;
    
  sql_singlequery("SELECT count(*) FROM ns_group_users WHERE sgid=%d", sgid);
  
  return  (sql_field_i(0) >= maxusers);
}

u_int32_t find_group(char *name)
{
  MYSQL_RES *res;
  MYSQL_ROW row;
  u_int32_t sgid = 0;

  res = sql_query("SELECT sgid, gdesc, master_sgid "
    "FROM ns_group WHERE UPPER(name)=UPPER(%s)",
    sql_str(name));

  row = sql_next_row(res);
  if(row)
    sgid = atoi(row[0]);
    
  sql_free(res);
  
  return sgid;
}
#undef STRING_SET
#undef INT_SET

int ev_ns_group_expire(void* dummy1, void* dummy2)
{
  return sql_execute("DELETE FROM ns_group_users WHERE t_expire>0 and t_expire<%d",
    irc_CurrentTime);
}


