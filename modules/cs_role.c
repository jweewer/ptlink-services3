/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2006 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************
                                                                                
  Description: chanserv role command
                                                                                
*/

#include "module.h"

#include "chanrecord.h"
#include "chanserv.h"
#include "my_sql.h"
#define CS_ROLE
#include "cs_role.h"
#include "nsmacros.h"
#include "nickserv.h"
#include "dbconf.h"
/* lang files */
#include "lang/common.lh"
#include "lang/cscommon.lh"
#include "lang/cs_role.lh"

SVS_Module mod_info =
/* module, version, description */
{"cs_role", "4.7", "chanserv role functions" };

/* Change Log
  4.7 - #64: Fixed SecureOps & BotServ
  4.6 - #63: validate_options() provided by strhand.c
  4.5 - #54: CS ROLE DEL ALL
        #26: Added chanserv suspensions
  4.4 - #27 : half-op support
  4.3 - #30 : users can drop the admin role
        #29 : possible sql injection on cs_role
        #16 : autoop isn't working when joing a channel  
  4.2 - 0000339: attempt to change the admin role crashes services
        0000334: review code to use local bot when possible
        0000321: HelpChan option to set +h when ops join the help chan
  4.1 - 0000322: cs_role crash when upgrading from ptsvs2 database
  4.0 - 0000305: foreign keys for data integrity
          Added cs_role.3.sql changes          
  3.0 - 0000284: irc opers should not be kicked from restricted/forbidden chans
      - 0000283: chanserv forbidden has no effect
      - 0000281: No auth nicks can't use chanserv
      - 0000265: remove nickserv cache system
  2.1 - we don't need irc_lower()
      - call mod_abort_event() when the user is kicked
      - added P_AKICK
  2.0 - added support for accept/reject roles
  1.1 - fixed the message for MaxChanUsers, was using the wrong value
*/
  

#define DB_VERSION	3

#define DEF_ADMIN_ACTIONS     A_AADM | A_AOP | A_MSG
#define DEF_ADMIN_PERMS       P_SET | P_KICK | P_OPDEOP | P_LIST | P_VIEW | P_VOICEDEVOICE | P_INVITE | P_UNBAN | P_CLEAR | P_AKICK | P_HOPDEHOP

#define DEF_OPERATOR_ACTIONS	A_AOP | A_MSG
#ifdef HALFOPS
#define DEF_OPERATOR_PERMS	P_KICK | P_OPDEOP | P_LIST | P_VOICEDEVOICE | P_INVITE | P_UNBAN | P_HOPDEHOP
#else
#define DEF_OPERATOR_PERMS	P_KICK | P_OPDEOP | P_LIST | P_VOICEDEVOICE | P_INVITE | P_UNBAN
#endif

#ifdef HALFOPS
#define DEF_HALFOPERATOR_ACTIONS	A_AHOP | A_MSG
#define DEF_HALFOPERATOR_PERMS		P_KICK | P_VOICEDEVOICE | P_INVITE | P_UNBAN | P_HOPDEHOP
#endif

#define DEF_VOICE_ACTIONS	A_AVOICE | A_MSG
#define DEF_VOICE_PERMS		P_VOICEDEVOICE

/* external functions we need */
ServiceUser* (*chanserv_suser)(void);
int e_chan_register;
int e_regchan_join;
int e_nick_identify;

MOD_REQUIRES
  MOD_FUNC(dbconf_get)
  MOD_FUNC(dbconf_get_or_build)
  MOD_FUNC(chanserv_suser)
  MOD_FUNC(e_chan_register)
  MOD_FUNC(e_regchan_join)
  MOD_FUNC(e_nick_identify)
MOD_END

/** functions/events we provide **/
/* int role_with_permission(u_int32_t scid, u_int32_t snid, int permission); */

MOD_PROVIDES
  MOD_FUNC(role_with_permission)
MOD_END

/* Internal functions declaration */
u_int32_t find_role(u_int32_t scid, char *rname);
u_int32_t create_role(u_int32_t scid, char *rname, u_int32_t mroleid, u_int32_t actions, u_int32_t permissions);
int drop_role(u_int32_t roleid, u_int32_t scid);
int add_to_role(u_int32_t roleid, u_int32_t scid, u_int32_t snid, u_int32_t who, char* msg, int flags);
int del_from_role(u_int32_t scid, u_int32_t snid);
int del_roles_from(u_int32_t scid);
int roles_count(u_int32_t scid);
int users_count(u_int32_t scid);
int is_member_or_master(u_int32_t snid, u_int32_t rid);
int role_is_master(u_int32_t roleid, u_int32_t masterid);
int is_master(u_int32_t snid, u_int32_t rid);
void set_role_master(u_int32_t roleid, u_int32_t masterid);
void set_role_prop(u_int32_t roleid, u_int32_t actions, u_int32_t perms);
void fix_channels_roles(void);
void ev_cs_role_nick_identify(IRC_User* u, u_int32_t* snid);
void ev_cs_role_timer_part(IRC_Chan* chan, int tag);
void ev_cs_role_op(IRC_Chan *chan, IRC_User *user);
int sql_upgrade(int version, int post);

/* core event handlers */
int ev_cs_chan_register(IRC_User *u, ChanRecord* cr);
int ev_cs_role_chan_join(ChanRecord *cr, IRC_ChanNode* cn);

/* available commands from module */
void cs_role(IRC_User *s, IRC_User *u);

/* Local config */
static int MaxRolesPerChan;
static int MaxUsersPerChan;
static int RoleAcceptance;
static char *HelpChan;

DBCONF_PROVIDES
  DBCONF_INT(MaxRolesPerChan, "10", 
    "Max. number of roles for each channel")
  DBCONF_INT(MaxUsersPerChan, "100",
    "Max. number of users with assigned roles for each channel")
  DBCONF_SWITCH(RoleAcceptance, "on",
    "Users need to accept the roles they get assigned to")
  DBCONF_STR_OPT(HelpChan, NULL,
    "Users will get user mode +h when joining this chan")
DBCONF_END

/* Remote config */
static int NeedsAuth;

DBCONF_REQUIRES
  DBCONF_GET("chanserv", NeedsAuth)
DBCONF_END

int mod_rehash(void)
{
  if(dbconf_get(dbconf_requires) < 0 )
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

/* role actions */
#define A_AADM		0x00000001
#define A_AOP			0x00000002
#define A_AVOICE	0x00000004
/* #define A_AKICK		0x00000008 reuse this later */
#define A_NOTICE	0x00000010
#define A_MSG		0x00000020
#define A_AHOP		0x00000040

/* actions declaration here */
int action_aadm(IRC_Chan *chan, IRC_ChanNode* cn, char* msg);
int action_aop(IRC_Chan *chan, IRC_ChanNode* cn, char* msg);
int action_ahop(IRC_Chan *chan, IRC_ChanNode* cn, char* msg);
int action_avoice(IRC_Chan *chan, IRC_ChanNode* cn, char* msg);
int action_notice(IRC_Chan *chan, IRC_ChanNode* cn, char* msg);
int action_msg(IRC_Chan *chan, IRC_ChanNode* cn, char* msg);

OptionMask actions_mask[] = 
  {
    { "aadm", A_AADM, action_aadm },
    { "aop", A_AOP, action_aop },
    { "ahop", A_AHOP, action_ahop },
    { "avoice", A_AVOICE, action_avoice },
    { "notice", A_NOTICE, action_notice },
    { "msg", A_MSG, action_msg },
    { NULL }
};

OptionMask permissions_mask[] =
  {
    { "set", P_SET, NULL },
    { "kick", P_KICK, NULL },
    { "opdeop", P_OPDEOP, NULL },
    { "hopdehop", P_HOPDEHOP, NULL },
    { "voice", P_VOICEDEVOICE, NULL },    
    { "list", P_LIST, NULL },
    { "view", P_VIEW, NULL },
    { "invite", P_INVITE, NULL  },    
    { "unban", P_UNBAN, NULL  },
    { "clear", P_CLEAR, NULL },        
    { "akick", P_AKICK, NULL },
    { NULL }
};
        

/* Local variables */
ServiceUser* csu;
int cs_log;


int mod_load(void)
{
  int r;
  cs_log = log_handle("chanserv");
  
  r = sql_check_inst_upgrade(mod_info.name, DB_VERSION, sql_upgrade);

  if( r < 0 )
    return -4;
  else if(r == 1) /* table was installed */
    fix_channels_roles(); /* this is needed for svs2 tables */
  
  /* */
  csu = chanserv_suser();  
  
  suser_add_cmd(csu, "ROLE", cs_role, ROLE_SUMMARY, ROLE_HELP);
  suser_add_help(csu, "ACTIONLIST", CHAN_ROLE_ACTIONLIST);
  suser_add_help(csu, "PERMLIST", CHAN_ROLE_PERMLIST);
  suser_add_help(csu, "ROLE CREATE", CHAN_ROLE_CREATE_HELP);
  suser_add_help(csu, "ROLE DROP", CHAN_ROLE_DROP_HELP);
  suser_add_help(csu, "ROLE VIEW", CHAN_ROLE_VIEW_HELP);
  suser_add_help(csu, "ROLE LIST", CHAN_ROLE_LIST_HELP);
  suser_add_help(csu, "ROLE ADD", CHAN_ROLE_ADD_HELP);
  suser_add_help(csu, "ROLE DEL", CHAN_ROLE_DEL_HELP);  
  suser_add_help(csu, "ROLE SETMSG", CHAN_ROLE_SETMSG_HELP);
  suser_add_help(csu, "ROLE SET", CHAN_ROLE_SET_HELP);
  suser_add_help(csu, "ROLE ACCEPT", CHAN_ROLE_ACCEPT_HELP);
  suser_add_help(csu, "ROLE REJECT", CHAN_ROLE_REJECT_HELP);

  /* Add actions  */    
  mod_add_event_action(e_chan_register, (ActionHandler) ev_cs_chan_register);
  mod_add_event_action(e_regchan_join, (ActionHandler) ev_cs_role_chan_join);
  mod_add_event_action(e_nick_identify, (ActionHandler) ev_cs_role_nick_identify);
  
  /* we take care of secureops */
  irc_AddCmodeChange("+o", ev_cs_role_op);
  
  return 0;
}

void
mod_unload(void)
{
  suser_del_mod_cmds(csu, &mod_info);
}

/* core events implementation */
int ev_cs_chan_register(IRC_User *u, ChanRecord* cr)
{
  u_int32_t founder_rid;
  u_int32_t operator_rid;
#ifdef HALFOPS  
  u_int32_t halfoperator_rid;
#endif  
  u_int32_t  voice_rid;
  int r;
  
  founder_rid = create_role(cr->scid, "admin", 0, DEF_ADMIN_ACTIONS, DEF_ADMIN_PERMS);
  if(founder_rid == 0)
    {
      send_lang(u, csu->u, CHAN_ROLE_CREATE_ERROR_X_X, "admin", cr->name);
      return 0;
    }
   send_lang(u, csu->u, CHAN_ROLE_X_X_CREATED, "admin", cr->name);
  
  r = add_to_role(founder_rid, cr->scid, u->snid, u->snid, NULL, 0);
  if(r>0)
    send_lang(u, csu->u, NICK_X_ADDED_TO_ROLE_X_ON_X,
      u->nick, "admin", cr->name);

  operator_rid = create_role(cr->scid, "operator", founder_rid, DEF_OPERATOR_ACTIONS, DEF_OPERATOR_PERMS);
  if(operator_rid == 0)
    {
      send_lang(u, csu->u, CHAN_ROLE_CREATE_ERROR_X_X, "operator", cr->name);
      return 0;
    }
  /* send_lang(u, csu->u, CHAN_ROLE_X_X_CREATED, "operator", cr->name);  */
#ifdef HALFOPS  
  halfoperator_rid = create_role(cr->scid, "halfoperator", operator_rid, DEF_HALFOPERATOR_ACTIONS, DEF_HALFOPERATOR_PERMS);  
  if(halfoperator_rid == 0)
    {
      send_lang(u, csu->u, CHAN_ROLE_CREATE_ERROR_X_X, "halfoperator", cr->name);
      return 0;
    }
  /* send_lang(u, csu->u, CHAN_ROLE_X_X_CREATED, "halfoperator", cr->name);  */
  voice_rid = create_role(cr->scid, "voice", halfoperator_rid, DEF_VOICE_ACTIONS, DEF_VOICE_PERMS);
#else
  voice_rid = create_role(cr->scid, "voice", operator_rid, DEF_VOICE_ACTIONS, DEF_VOICE_PERMS);
#endif  
  if(voice_rid == 0)
    {
      send_lang(u, csu->u, CHAN_ROLE_CREATE_ERROR_X_X, "voice", cr->name);
      return 0;
    }
  /* send_lang(u, csu->u, CHAN_ROLE_X_X_CREATED, "voice", cr->name); */
  
  return 0;
}

/* TO DO */
/* internal functions implementation */
u_int32_t create_role(u_int32_t scid, char *rname, u_int32_t mroleid, u_int32_t act, u_int32_t priv)
{  

  if(mroleid)
    return sql_execute("INSERT INTO cs_role VALUES(0, %d, %s, %d, %d ,%d)",
	scid, sql_str(rname), mroleid, act, priv);
  else
    return sql_execute("INSERT INTO cs_role VALUES(0, %d, %s, NULL, %d ,%d)",
  	scid, sql_str(rname), act, priv);
}

int drop_role(u_int32_t roleid, u_int32_t scid)
{
  MYSQL_RES* res;
  MYSQL_ROW row;
  u_int32_t master_rid = 0;
  
  res = sql_query("SELECT rid FROM cs_role WHERE scid=%d and master_rid IS NULL", scid);
  if((row = sql_next_row(res)) && row[0])
    master_rid = atoi(row[0]);
  sql_free(res);
  if(master_rid == 0)
    {
      slog(L_ERROR, "Attempt to drop masterless role %d", roleid);
      return 0;
    }
 
  /* First remaster any roles for which the dropped role is master 
  to the founder role */ 
  sql_execute("UPDATE cs_role SET master_rid=%d WHERE master_rid = %d",
    master_rid, roleid);
    
  /* Now delete the data */
  return sql_execute("DELETE FROM cs_role WHERE rid=%d", roleid);
}


/* s = service the command was sent to
   u = user the command was sent from */
void cs_role(IRC_User *s, IRC_User *u)
{
  u_int32_t source_snid;
  ChanRecord* cr = NULL;
  char *valres;
  u_int32_t snid;
  char *chname,*cmd;
  
  chname = strtok(NULL, " ");  
  cmd = strtok(NULL, " ");

  /* status validation */
  CHECK_IF_IDENTIFIED_NICK  
  
  /* base syntax validation */
  if(NeedsAuth && !IsAuthenticated(u))
    send_lang(u, s, NEEDS_AUTH_NICK);
  else
  if(IsNull(chname) || IsNull(cmd))
    send_lang(u, s, CHAN_ROLE_SYNTAX);    
  else if((cr = OpenCR(chname)) == NULL)
    send_lang(u, s, CHAN_X_NOT_REGISTERED, chname);
  else if(strcasecmp(cmd, "CREATE") == 0) /* */
    {
      char *rname; /* role name */
      char *mrole; /* master role */        
      u_int32_t mroleid;
      u_int32_t iactions = 0;
      u_int32_t iperms = 0;
      char *actions;
      char *perms;
          
      rname = strtok(NULL, " ");
      mrole = strtok(NULL, " ");
      actions = strtok(NULL,":");
      perms = strtok(NULL," ");
      
      /* syntax validation */
      if( IsNull(rname) || IsNull(mrole) )
        send_lang(u, s, CHAN_ROLE_CREATE_SYNTAX);
      /* permissions validation */
      else if((source_snid != cr->founder)) /* it's not founder */
        send_lang(u,s, ONLY_FOUNDER_X, chname);
      /* check requirements */
      else if((mroleid = find_role(cr->scid, mrole)) == 0)
        send_lang(u, s, CHAN_ROLE_X_X_NOT_FOUND, mrole, chname);
      else if(actions && ((valres = validate_options(actions, actions_mask, &iactions)) != NULL))
        send_lang(u, s, INVALID_ACTION_X, valres);
      else if(perms && ((valres = validate_options(perms, permissions_mask, &iperms)) != NULL))
        send_lang(u, s, INVALID_PRIVILEGE_X, valres);
      else if(MaxRolesPerChan && roles_count(cr->scid) >= MaxRolesPerChan)
        send_lang(u, s, REACHED_MAX_ROLES_X, MaxRolesPerChan);
      /* avoid duplicates */        
      else if(find_role(cr->scid, rname) != 0)
        send_lang(u, s, CHAN_ROLE_X_X_ALREADY_EXISTS, rname, chname);        
      /* execute operation */
      else if(create_role(cr->scid, rname, mroleid, iactions, iperms) > 0)
      /* report operation status */
        send_lang(u, s, CHAN_ROLE_X_X_CREATED, rname, chname);
      else
        send_lang(u, s, CHAN_ROLE_CREATE_ERROR_X_X, rname, chname);
    }
  else if(strcasecmp(cmd, "DROP") == 0) /* drop role */
    {
      char *rname;
      u_int32_t roleid;
          
      rname = strtok(NULL, " ");
      /* syntax validation */
      if(IsNull(rname))
        send_lang(u, s, CHAN_ROLE_DROP_SYNTAX);
      /* privileges validation */
      if((source_snid != cr->founder)) /* it's not founder */
        send_lang(u,s, ONLY_FOUNDER_X, chname);        
      /* check requirements */
      else 
      if((roleid = find_role(cr->scid, rname)) == 0)
        send_lang(u, s, CHAN_ROLE_X_X_NOT_FOUND, rname, chname);            
      else 
      /* NOTE !!! Here we assume find_role gets the master on field 1 */
      if(sql_field(1) == NULL)
        send_lang(u, s, CANT_DROP_ADMIN_ROLE_ON_X, chname);
      /* execute operation */
      else if(drop_role(roleid, cr->scid)>0)
      /* report operation status */
        send_lang(u, s, CHAN_ROLE_X_X_DROP, rname, chname);
      else      
        send_lang(u, s, UPDATE_FAIL);
    }
  else if(strcasecmp(cmd, "ADD") == 0) /* add role nick message */
    {
      char *rname;
      char *rnick;
      char *msg;
      u_int32_t roleid;

      rname = strtok(NULL, " ");
      rnick = strtok(NULL, " "); 
      msg = strtok(NULL,"");
      /* syntax validation */
      if(IsNull(rname) || IsNull(rnick))
        send_lang(u, s, CHAN_ROLE_ADD_SYNTAX);
      /* check requirements */
      else if((snid = nick2snid(rnick)) == 0)
        send_lang(u, s, NICK_X_NOT_REGISTERED, rnick);
      else if((roleid = find_role(cr->scid, rname)) == 0 )
        send_lang(u, s, CHAN_ROLE_X_X_NOT_FOUND, rname, chname);
      /* privileges validation */
      else if((source_snid != cr->founder) /* its not founder */
            && !is_master(source_snid, roleid)) /* its not master */
        send_lang(u,s, NOT_MASTER_OF_X_ON_X, rname, chname);
      else if(sql_singlequery("SELECT r.rid, r.name FROM cs_role r, cs_role_users u"
            " WHERE u.scid=%d AND u.snid=%d AND r.rid=u.rid", 
            cr->scid, snid) != 0)
        send_lang(u, s, NICK_X_IS_X_ON_X, rnick, sql_field(1), cr->name);
      else if(MaxUsersPerChan && users_count(cr->scid) >= MaxUsersPerChan)
        send_lang(u, s, REACHED_MAX_USERS_X, MaxUsersPerChan);
      else 
        {
          int flags;
          if(RoleAcceptance && (source_snid != snid))
            flags = CRF_PENDING;
          else
            flags = 0;
          if(add_to_role(roleid, cr->scid, snid, source_snid, msg, flags) != 0)
            {
              send_lang(u, s, NICK_X_ADDED_TO_ROLE_X_ON_X,
                rnick, rname, cr->name);
              if(flags & CRF_PENDING)
                {
                  IRC_User* target;
                  target = irc_FindUser(rnick);
                  send_lang(u, s, ROLE_PENDING);                  
                  if(target && target->snid) /* online and identified */
                    send_lang(target, s, ROLE_X_X_X_X_PENDING,
                      rname, cr->name, cr->name, cr->name);
                }
            
            }
        }
    }
  else if(strcasecmp(cmd, "DEL") == 0) /* add role nick message */
    {
      char *rnick;      
      int is_all = 0;
      rnick = strtok(NULL, " ");
      if(rnick && (strcasecmp(rnick, "all") == 0))
        is_all = 1;
      if(IsNull(rnick))
        send_lang(u, s, CHAN_ROLE_DEL_SYNTAX);
      else if(!is_all && (snid = nick2snid(rnick)) == 0)
        send_lang(u, s, NICK_X_NOT_REGISTERED, rnick);
      else if(!is_all && sql_singlequery("SELECT r.rid, r.name FROM cs_role r, cs_role_users u"
            " WHERE u.scid=%d AND u.snid=%d AND r.rid=u.rid",
            cr->scid, snid) == 0)
        send_lang(u, s, NICK_X_HAS_NO_ROLE_ON_X, rnick, chname);
      else
        {
          if(is_all)
          {
            if(source_snid != cr->founder) /* its not founder */
              send_lang(u,s, CHAN_NOT_FOUNDER_X, chname);
            else
            {
              send_lang(u,s, DELETED_ALL_ROLES_X, chname);
              del_roles_from(cr->scid);
            }
          }
          else
          {            
            u_int32_t roleid = atoi(sql_field(0));
            char *is_rname = strdup(sql_field(1));

            if((source_snid != cr->founder) /* its not founder */
              && !is_master(source_snid, roleid) /* its not master */
              && (source_snid != snid)) /* is not the role owner */
                send_lang(u,s, NOT_MASTER_OF_NICK_X_ON_X, rnick, cr->name);
            else if(del_from_role(cr->scid, snid) != 0)
              send_lang(u, s, NICK_X_DELETED_FROM_ROLE_X_ON_X,
                rnick, is_rname, cr->name);
            free(is_rname);
          }
        }        
    }
  else if(strcasecmp(cmd,"LIST") == 0) /* List all users with assigned roles */
    {
      if(source_snid != cr->founder && 
        !irc_IsUMode(u, UMODE_OPER) &&
        role_with_permission(cr->scid, source_snid, P_LIST) == 0)
        send_lang(u, s, NO_LIST_PERM_ON_X, chname);
      else
        {          
          u_int32_t roleid;
          char *rname;
          char sqlcmd[256];
          char sqlcmd2[64];
          
          rname = strtok(NULL, " ");
          if(rname && ((roleid = find_role(cr->scid, rname)) == 0 ))
            send_lang(u, s, CHAN_ROLE_X_X_NOT_FOUND, rname, chname);
          else        
            {           
              MYSQL_RES* res; 
              snprintf(sqlcmd, sizeof(sqlcmd), 
                "SELECT n.nick, cr.name, cu.message, cu.flags "
                "FROM cs_role_users cu,nickserv n, cs_role cr "
                "WHERE cu.scid=%d AND cr.rid=cu.rid AND n.snid=cu.snid", 
                  cr->scid); 
              if(rname)
                {
                  snprintf(sqlcmd2,sizeof(sqlcmd2), " AND cr.rid=%d", roleid);
                  strcat(sqlcmd, sqlcmd2);
                }                
              send_lang(u, s, ROLE_LIST_HEADER_X, cr->name);
              res = sql_query("%s", sqlcmd);
              while(sql_next_row(res))
                {
                  char flagstr[64];
                  char* msg;
                  int flags;
                  msg = sql_field(2);
                  flags = atoi(sql_field(3));                  
                  flagstr[0] = '\0';                  
                  if(flags & CRF_REJECTED)
                    strncat(flagstr, lang_str(u, REJECTED_ROLE), 63);
                  else if(flags & CRF_PENDING)
                    strncat(flagstr, lang_str(u, PENDING_ROLE), 63);
                  if(msg)
                    send_lang(u, s, ROLE_LIST_X_X_X_X,
                      sql_field(0), sql_field(1), flagstr, msg);
                  else
                    send_lang(u, s, ROLE_LIST_X_X_X,
                     sql_field(0), sql_field(1), flagstr, msg);
                }
              send_lang(u, s, ROLE_LIST_TAIL);                
              sql_free(res);
            }
        }
    }    
  else if(strcasecmp(cmd,"VIEW") == 0) /* List all roles */
    {
      if(source_snid != cr->founder && 
      !irc_IsUMode(u, UMODE_OPER) &&
      role_with_permission(cr->scid, source_snid, P_VIEW) == 0)
        send_lang(u, s, NO_VIEW_PERM_ON_X, chname);
      else
        { 
          MYSQL_RES *res;         
          send_lang(u, s, ROLE_VIEW_HEADER_X, cr->name);          
          
          /* firs list all masterless roles on a different query
           this should catch at least founder*/
          res = sql_query("SELECT cr.name, cr.master_rid, cr.actions, cr.perms FROM cs_role cr"
            " WHERE cr.scid=%d AND cr.master_rid IS NULL", cr->scid);
          while(sql_next_row(res))
            {
              char *actionstr;
              char *permstr;

              actionstr = strdup(mask_string(actions_mask, atoi(sql_field(2))));
              permstr = strdup(mask_string(permissions_mask, atoi(sql_field(3))));              
              send_lang(u, s, ROLE_VIEW_X_X_X_X, sql_field(0), "", actionstr, permstr);
              free(actionstr);free(permstr);            
            }
          sql_free(res);
          /* now get all the other roles */
          res = sql_query("SELECT cr.name, cr.master_rid, cr.actions, cr.perms, mr.name FROM cs_role cr, cs_role mr"
            " WHERE cr.scid=%d AND mr.rid=cr.master_rid", cr->scid);
          while(sql_next_row(res))
            {
              char *actionstr;
              char *permstr;
              
              actionstr = strdup(mask_string(actions_mask, atoi(sql_field(2))));
              permstr = strdup(mask_string(permissions_mask, atoi(sql_field(3))));
              send_lang(u, s, ROLE_VIEW_X_X_X_X, sql_field(0), sql_field(4), actionstr, permstr);
              free(actionstr);free(permstr);
            }
          send_lang(u, s, ROLE_VIEW_TAIL);
          sql_free(res);
        }
    }
  else if(strcasecmp(cmd,"SETMSG") == 0) /* Sets the role message */
    {
      char *rnick, *rmsg;
      
      rnick = strtok(NULL, " ");
      rmsg = strtok(NULL, "");
      if(IsNull(rnick))
        send_lang(u, s, CHAN_ROLE_SETMSG_SYNTAX);
      else if((snid = nick2snid(rnick)) == 0)
        send_lang(u, s, NICK_X_NOT_REGISTERED, rnick);
      else if(sql_singlequery("SELECT r.rid, r.name FROM cs_role r, cs_role_users u"
              " WHERE u.scid=%d AND u.snid=%d AND r.rid=u.rid",
              cr->scid, snid) == 0)
        send_lang(u, s, NICK_X_HAS_NO_ROLE_ON_X, rnick, chname);
      else 
        {
          u_int32_t roleid = atoi(sql_field(0));
          if((source_snid != cr->founder) /* its not founder */
              && !is_master(source_snid, roleid)) /* its not master */
            send_lang(u,s, NOT_MASTER_OF_NICK_X_ON_X, rnick, cr->name);
          else if(sql_execute("UPDATE cs_role_users SET message=%s"
                  " WHERE snid=%d AND rid=%d", sql_str(rmsg), snid, roleid)>0)
            {
              if(rmsg)
              send_lang(u, s, MESSAGE_FOR_X_ON_X_CHANGED_TO_X, 
                rnick, chname, rmsg);
              else
                send_lang(u, s, MESSAGE_FOR_X_ON_X_REMOVED, 
                  rnick, chname);
            }
        }
    }
  else if(strcasecmp(cmd, "SET") == 0) /* SET role MASTER PROP VALUE */
    {
      char *rname;
      char *option;
      char *value;
      u_int32_t roleid;
      
      rname = strtok(NULL, " ");
      option = strtok(NULL, " ");
      value = strtok(NULL, " ");
          
      if(IsNull(rname) || IsNull(option) || IsNull(value)||
          (strcasecmp(option,"MASTER") &&
          strcasecmp(option,"PROP")))
        send_lang(u, s, CHAN_ROLE_SET_SYNTAX);
      else if((source_snid != cr->founder)) /* it's not founder */
        send_lang(u,s, ONLY_FOUNDER_X, chname);          
      else if((roleid = find_role(cr->scid, rname)) == 0 )              
        send_lang(u, s, CHAN_ROLE_X_X_NOT_FOUND, rname, chname);
      else
        { 
          if(strcasecmp(option, "MASTER") == 0) /* master option */
            {
              u_int32_t masterid;
              
              if(sql_field(1) == NULL) /* this depends on last find_role() call !!! */
                send_lang(u, s, CANT_SET_ADMIN_MASTER);
              else if((masterid = find_role(cr->scid, value)) == 0 )
                send_lang(u, s, CHAN_ROLE_X_X_NOT_FOUND, value, chname);
              else if(masterid == roleid)
                send_lang(u, s, CHAN_ROLE_NO_SELF_MASTER);
              else if(role_is_master(roleid, masterid) != 0) 
                send_lang(u, s, CHAN_ROLE_X_ON_X_IS_MASTER_OF_X, 
                  rname, chname, value);
              else
                {
                  set_role_master(roleid, masterid);
                  send_lang(u, s, CHAN_ROLE_MASTER_X_ON_X_TO_X,
                    rname, chname, value);
                }
            }
          else if(strcasecmp(option, "PROP") == 0) /* master option */
            {
              u_int32_t iactions = 0;
              u_int32_t iperms = 0;
              char *actions = 0;
              char *perms = 0;
              char *valuecp;

              valuecp = strdup(value);
              actions = strtok(valuecp,":");
              perms = strtok(NULL," ");
              if(actions && ((valres = validate_options(actions, actions_mask, &iactions)) != NULL))
                send_lang(u, s, INVALID_ACTION_X, valres);
              else if(perms && ((valres = validate_options(perms, permissions_mask, &iperms)) != NULL))
                send_lang(u, s, INVALID_PRIVILEGE_X, valres);
              else
                {
                  set_role_prop(roleid, iactions, iperms);
                  send_lang(u, s, ROLE_X_PROP_SET_X, rname, value);
                }
              free(valuecp);
            }
        }
    }
  else if(strcasecmp(cmd,"ACCEPT") == 0) /* List all roles */
    {
      int r;
      r = sql_execute("UPDATE cs_role_users SET flags=flags & ~%d"
        " WHERE scid=%d and snid=%d and (flags & %d)", 
        CRF_PENDING, cr->scid, source_snid, CRF_PENDING);
      if(r==1)
        send_lang(u, s, ROLE_ON_X_ACCEPTED, cr->name);
      else
        send_lang(u, s, NO_ROLE_PENDING_ON_X, cr->name);
    }
  else if(strcasecmp(cmd,"REJECT") == 0) /* List all roles */
    {
      int r;
      r = sql_execute("UPDATE cs_role_users SET flags=%d"
        " WHERE scid=%d and snid=%d and (flags & %d)", 
        CRF_REJECTED, cr->scid, source_snid, CRF_PENDING);
      if(r==1)
        send_lang(u, s, ROLE_ON_X_REJECTED, cr->name);
      else
        send_lang(u, s, NO_ROLE_PENDING_ON_X, cr->name);
    }    
  else
    send_lang(u, s, CHAN_ROLE_SYNTAX);
  
  CloseCR(cr);
}    

/* this is called on event ch_reg_chan */
int ev_cs_role_chan_join(ChanRecord* cr, IRC_ChanNode* cn)
{
  u_int32_t acmask = 0; /* actions mask */
  IRC_Chan* chan;
  int has_role = 0;
  
  if((chan = irc_FindChan(cr->name)) == NULL)
    abort(); /* this should never happen */  
    
  /* just don't do nothing for stealtch users */
  if(irc_IsUMode(cn->user, UMODE_STEALTH))
    return 0;
  
  if(cn->user && cn->user->snid) /* it's a registered nick */
  {
    MYSQL_RES* res;
    res = sql_query("SELECT r.rid, r.actions, u.message  FROM cs_role r, cs_role_users u"
      " WHERE u.scid=%d AND u.snid=%d AND r.rid=u.rid AND (u.flags & %d)=0", 
      cr->scid, cn->user->snid, CRF_PENDING|CRF_REJECTED);
        
    /* There is one role for this user, lets call the actions on it */
    if(sql_next_row(res))
    {
      int ret;
      char *msg;
      OptionMask* op = actions_mask;
          
      has_role = 1;
      SDUP(msg,sql_field(2));
           
      acmask = atoi(sql_field(1)); /* get the action mask */                    
      /* Let's call actions that match mask */
      while(op->name)
      {
        if(acmask & op->value)
        {
          ret = ((int (*)(IRC_Chan*, IRC_ChanNode*, char*)) op->func)(chan, cn, msg);
          if(ret == -1)
          {
            FREE(msg);
            sql_free(res);
            return -1;
          }
        }
        ++op;
      }
      FREE(msg);
      cr->t_last_use = irc_CurrentTime;
      UpdateCR(cr);
    }      
    sql_free(res);
  } 
      
  /* channel is for forbidden, or it is suspended, or it is
     restricted and user has no role, take action */
  if(!irc_IsUMode(cn->user, UMODE_OPER) &&
    (IsSuspendedChan(cr) || (IsRestrictedChan(cr) && (has_role == 0))))
  {
    char mask[HOSTLEN+10]; 
    char *reason = ""; 
    /* check if we need to join cs to keep the ban */ 
    if(chan->users_count == 1)
    {
      irc_ChanJoin(csu->u, cr->name, CU_MODE_ADMIN|CU_MODE_OP);
      irc_AddCTimerEvent(chan, 30 , ev_cs_role_timer_part, 0);
    }
    /* Suspended */
    if(IsSuspendedChan(cr))
    {
      strcpy(mask, "*!*@*");
      reason = "Suspended";
    }
    else 
    if(IsRestrictedChan(cr) && has_role == 0)
    { 
      snprintf(mask, sizeof(mask), "*!*@%s", cn->user->publichost);
      reason = "Restricted";
    }
    /* Apply ban */
    irc_ChanMode(chan->local_user ? chan->local_user : csu->u, chan,
      "+b %s", mask);
    /* And kick user */
    irc_Kick(chan->local_user ? chan->local_user : csu->u, chan, cn->user,
      reason);
    mod_abort_event();
    return 0;
  }

  /* roles were checked, now remove op if it was not aop */
  if(irc_IsCNOp(cn) && !(acmask & A_AOP))
    irc_ChanUMode(chan->local_user ? chan->local_user : csu->u, chan, "-o" , cn->user);
    
  /* set +h if op on HelpChan */
  if((acmask & A_AOP) && HelpChan && (irccmp(chan->name, HelpChan) == 0))
    irc_SvsMode(cn->user, csu->u, "+h");
  return 0;
}

/** actions code goes here
  if an action returns -1 then the other actions should noe be checked
*/
int action_aadm(IRC_Chan *chan, IRC_ChanNode* cn, char* msg)
{
  if(!irc_IsCNAdmin(cn)) 
    irc_ChanUMode(chan->local_user ? chan->local_user : csu->u, chan, "+a" , cn->user);
  return 0;
};

int action_aop(IRC_Chan *chan, IRC_ChanNode* cn, char* msg)
{
  if(!irc_IsCNOp(cn))
    irc_ChanUMode(chan->local_user ? chan->local_user : csu->u, chan, "+o" , cn->user);
  return 0;
}

int action_ahop(IRC_Chan *chan, IRC_ChanNode* cn, char* msg)
{
  if(!irc_IsCNHOp(cn))
    irc_ChanUMode(chan->local_user ? chan->local_user : csu->u, chan, "+h" , cn->user);
  return 0;
}

int action_avoice(IRC_Chan *chan, IRC_ChanNode* cn, char* msg)
{
  if(!irc_IsCNVoice(cn))
    irc_ChanUMode(chan->local_user ? chan->local_user : csu->u, chan, "+v" , cn->user);
  return 0;
}

int action_notice(IRC_Chan *chan, IRC_ChanNode* cn, char* msg)
{
  if(msg && (irc_ConnectionStatus() == 2))
    irc_SendCNotice(chan, chan->local_user ? chan->local_user : csu->u, "%s", msg);
  return 0;
}

int action_msg(IRC_Chan *chan, IRC_ChanNode* cn, char* msg)
{
  if(msg && (irc_ConnectionStatus() == 2 ))
    irc_SendCMsg(chan, chan->local_user ? chan->local_user : csu->u, "%s", msg);
  return 0;
}

u_int32_t find_role(u_int32_t scid, char *rname)
{

  if(sql_singlequery("SELECT rid, master_rid FROM cs_role WHERE scid=%d and name=%s",
    scid, sql_str(rname)) == 0)
      return 0;
  	
  return atoi(sql_field(0));
}

int add_to_role(u_int32_t roleid, u_int32_t scid, u_int32_t snid, u_int32_t who, char* msg, int flags)
{
  return sql_execute("INSERT INTO cs_role_users VALUES(%d, %d, %d, %d, %s, %d)",
    roleid, snid, who, scid, sql_str(msg), flags);
}

int del_from_role(u_int32_t scid, u_int32_t snid)
{
  return sql_execute("DELETE FROM cs_role_users WHERE scid=%d AND snid=%d",
    scid, snid);
}

int del_roles_from(u_int32_t scid)
{
  return sql_execute("DELETE FROM cs_role_users WHERE scid=%d",
    scid);
}

/**
  returns the number of roles that exist for a given channel
*/
int roles_count(u_int32_t scid)
{
  int count;
  count = sql_singlequery("SELECT count(*) FROM cs_role WHERE scid=%d", scid);
  if(count > 0)
    return atoi(sql_field(0));    
  return 0;
}

/**
  returns the number of users with roles assigned on a given channel
*/
int users_count(u_int32_t scid)
{
  int count;
  count = sql_singlequery("SELECT count(*) FROM cs_role_users WHERE scid=%d", scid);
  if(count > 0)
    return atoi(sql_field(0));
  return 0;
}

/** checks if a given snid is master of a given rid
  Returns: 0 if not, >0 if it is
 */
int is_master(u_int32_t snid, u_int32_t rid)
{
  int r;

  while(rid != 0)
    {
      r = sql_singlequery("SELECT master_rid FROM cs_role WHERE rid=%d", rid);

      if((r == 0) || (sql_field(0) == NULL)) /* reached a masterless role */
        return 0;
      else
        rid = atoi(sql_field(0));
        
      /* check if snid is member of master */
      r = sql_singlequery("SELECT snid FROM cs_role_users WHERE rid=%d AND snid=%d", 
        rid, snid);
      if( r > 0)
        return 1;
    }
  return 0;
}

/** checks if a given snid is member or master of a given rid
  Returns: 0 if not
           >0 if yes
 */
int is_member_or_master(u_int32_t snid, u_int32_t rid)
{
  if(sql_singlequery("SELECT snid FROM cs_role_users WHERE rid=%d AND snid=%d", 
    rid, snid) > 0)
    return 1;
  else    
    return is_master(snid, rid); /* now check if it is master */
    
}

/** checks if roleid1 is master or above of roleid2
 */
int role_is_master(u_int32_t roleid1, u_int32_t roleid2)
{
  int r;
  u_int32_t rid;
  
  rid = roleid2;  
  while(rid != 0)
    {
      r = sql_singlequery("SELECT master_rid FROM cs_role WHERE rid=%d", rid);
      if((r == 0) || !sql_field(0)) /* reached a masterless role */
        return 0;
      else
        rid = atoi(sql_field(0));
      
      if(roleid1 == rid)
        return 1;
    } 
  return 0;
}

/** Sets role master */
void set_role_master(u_int32_t roleid, u_int32_t masterid)
{
  sql_execute("UPDATE cs_role SET master_rid=%d WHERE rid=%d",
    masterid, roleid);
}

/** Sets role properties */
void set_role_prop(u_int32_t roleid, u_int32_t actions, u_int32_t perms)
{
  sql_execute("UPDATE cs_role SET actions=%d, perms=%d WHERE rid=%d",
    actions, perms, roleid);
}
                      
/** Check if a given snid has a given permission
  0 = no
  >0 = yes
  */
int role_with_permission(u_int32_t scid, u_int32_t snid, int permission)
{
  int r;
  r = sql_singlequery("SELECT r.rid, r.perms  FROM cs_role r, cs_role_users u "
    "WHERE u.scid=%d AND u.snid=%d AND r.rid=u.rid AND (u.flags & %d)=0", 
    scid, snid, CRF_PENDING|CRF_REJECTED);    
  if(r>0)
    {
      int i;
      i = atoi(sql_field(1));
      if(i & permission)
        return 1;
    }
  return 0;
}

/* will add the default roles for channel without roles (from services2) */
void fix_channels_roles(void)
{
  MYSQL_RES* res;
  MYSQL_ROW row;
  MYSQL_ROW row2;
  MYSQL_RES* res2;
  u_int32_t admin_rid;
  u_int32_t operator_rid;
#ifdef HALFOPS  
  u_int32_t halfoperator_rid;
#endif  
  u_int32_t  voice_rid;
  
  /* expire code goes here */
  res = sql_query("SELECT scid, founder, name FROM chanserv");

  while((row = sql_next_row(res)))
    {
      u_int32_t scid = atoi(row[0]);
      u_int32_t founder_snid = 0;
      if(row[1] == NULL)
      {
        log_log(cs_log, mod_info.name, "Skipping impot on forbidden channel %s",
          row[2]);
        continue;
      }
      founder_snid = atoi(row[1]);
      if(sql_singlequery("SELECT COUNT(*) FROM cs_role WHERE scid=%d", scid) == 0)
        return;
      if(atoi(sql_field(0)) == 0)
        {
          /* create default roles */
          admin_rid = create_role(scid, "admin", 0, DEF_ADMIN_ACTIONS, DEF_ADMIN_PERMS);
          if(admin_rid == 0)
            return;
          add_to_role(admin_rid, scid, founder_snid, founder_snid, NULL, 0);

          operator_rid = create_role(scid, "operator", admin_rid, DEF_OPERATOR_ACTIONS, DEF_OPERATOR_PERMS);
          if(operator_rid == 0)
            return;  
#ifdef HALFOPS        
          halfoperator_rid = create_role(scid, "halfoperator", operator_rid, DEF_HALFOPERATOR_ACTIONS, DEF_HALFOPERATOR_PERMS);
          if(halfoperator_rid == 0)
            return;  
          voice_rid = create_role(scid, "voice", halfoperator_rid, DEF_VOICE_ACTIONS, DEF_VOICE_PERMS);
#else
          voice_rid = create_role(scid, "voice", operator_rid, DEF_VOICE_ACTIONS, DEF_VOICE_PERMS);
#endif          
          if(voice_rid == 0)
            return;
          /* import role users from role temporary map */
          res2 = sql_query("SELECT snid, who, rtype FROM cs_role_temp" 
            " WHERE scid=%d", scid);
          while((row2 = sql_next_row(res2)))
            {   
              u_int32_t rid;          
              switch(atoi(row2[2]))
                {
                  case 1: rid = admin_rid; break;
                  case 2: rid = operator_rid; break;
#ifdef HALFOPS                  
                  case 4: rid = halfoperator_rid; break;
#endif                  
                  case 3:
                  default: rid = voice_rid; break;
                }
              add_to_role(rid, scid, atoi(row2[0]), atoi(row2[1]), NULL, 0);
            }
          sql_free(res2);
        }
    }
    
  sql_free(res);
  return;
}

/* notify users about pending roles */
void ev_cs_role_nick_identify(IRC_User* u, u_int32_t* snid)
{
  MYSQL_RES *res;
  MYSQL_ROW row;
  int rowc = 0;
  res = sql_query("SELECT cr.name, c.name FROM chanserv c, cs_role cr, cs_role_users cru"
          " WHERE cru.snid=%d and (cru.flags & %d)"
          " AND cr.rid=cru.rid AND c.scid=cr.scid",
          *snid, CRF_PENDING);
  while((row = sql_next_row(res)))
    {
      ++rowc;
      send_lang(u, csu->u, ROLE_X_X_X_X_PENDING, row[0], row[1], row[1], row[1]);
    }  
  sql_free(res);
}

void ev_cs_role_timer_part(IRC_Chan* chan, int tag)
{
  /* check if we are still on chan */
  if(chan->local_user == csu->u)
    irc_ChanPart(csu->u, chan);
}
/* Check users on +o for secureops */
void ev_cs_role_op(IRC_Chan *chan, IRC_User *user)
{
  int r = 0;
  ChanRecord *cr = chan->sdata;
  
  /* we dont deop local users */
  if(irc_IsLocalUser(user))
    return ;
  
  if(cr && user->snid)
    r = sql_singlequery("SELECT r.rid, r.perms  FROM cs_role r, cs_role_users u "
      "WHERE u.scid=%d AND u.snid=%d AND r.rid=u.rid AND (u.flags & %d)=0",
          cr->scid, user->snid, CRF_PENDING|CRF_REJECTED);
  if(cr && IsSecureOps(cr) && ((user->snid == 0) || (r == 0)))
    irc_ChanUMode(chan->local_user ? chan->local_user : csu->u, chan, "-o" , user);
}

int sql_upgrade(int version, int post)
{ 
  MYSQL_RES *res;
  MYSQL_ROW row;
  switch(version)
  {
    case 3:
    if(!post) /* we need to check for "lost" roles */
    {
      int rowc = 0;
      res = sql_query("SELECT"
        " cs_role.rid, cs_role.scid FROM cs_role"
        " LEFT JOIN chanserv ON (cs_role.scid = chanserv.scid)"
        " WHERE cs_role.scid IS NOT NULL AND chanserv.scid IS NULL");
      while((row = sql_next_row(res)))
      {
        log_log(cs_log, mod_info.name, "Removing lost role %s for scid %s",
          row[0], row[1]);
        sql_execute("DELETE FROM cs_role_users WHERE rid=%s",
          row[0]);          
        sql_execute("DELETE FROM cs_role WHERE rid=%s",
          row[0]);
        ++rowc;
      }
      if(rowc)
        log_log(cs_log, mod_info.name, "Removed %d lost roles(s)", rowc);
      sql_free(res);    
    }
    break;
  }
  return 1;
}

/* End of Module */
