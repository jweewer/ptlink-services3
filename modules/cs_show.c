/******************************************************************
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License           *
 * Please read the file COPYING for copyright information.        *
 ******************************************************************
                                                                                
  File: cs_show.c
  Description: chanserv list command
                                                                                
 *  $Id: cs_show.c,v 1.6 2005/10/18 16:25:06 jpinto Exp $
*/

#include "module.h"
#include "chanserv.h"
#include "chanrecord.h"
#include "cs_role.h"
#include "my_sql.h"
#include "nsmacros.h"
#include "nickserv.h"
#include "dbconf.h"
/* laang files */
#include "lang/cs_role.lh"
#include "lang/cs_show.lh"

SVS_Module mod_info =
/* module, version, description */
{"cs_show", "2.1", "chanserv list command" };

/* Change Log
  2.1 - #17: CS SHOW shoud display the pending status
  2.0 - 0000265: remove nickserv cache system
      - 0000281: no auth nick can't use chanserv
*/

/* external functions we need */
ServiceUser* (*chanserv_suser)(void);

MOD_REQUIRES
  MOD_FUNC(dbconf_get)
  MOD_FUNC(chanserv_suser)
MOD_END

/* internal functions */

/* available commands from module */
void cs_show(IRC_User *s, IRC_User *u);

/* Remote config */
static int NeedsAuth;
DBCONF_REQUIRES
  DBCONF_GET("chanserv", NeedsAuth)
DBCONF_END

/* this is called before load and at services rehash */
int mod_rehash(void)
{
  if(dbconf_get(dbconf_requires) < 0 )
  {
    errlog("Error reading dbconf!");
    return -1;
  }
  return 0;
}

/* Local variables */
ServiceUser* csu;

int mod_load(void)
{
  csu = chanserv_suser();  
  suser_add_cmd(csu, "SHOW", cs_show, SHOW_SUMMARY, SHOW_HELP);
  return 0;
}

void mod_unload(void)
{
  suser_del_mod_cmds(csu, &mod_info);
}

/* s = service the command was sent to
   u = user the command was sent from */
void cs_show(IRC_User *s, IRC_User *u)
{
  MYSQL_RES* res;
  MYSQL_ROW row;
  u_int32_t source_snid;
  int rowc = 0;
  
  CHECK_IF_IDENTIFIED_NICK
  if(NeedsAuth && !IsAuthenticated(u))
    send_lang(u, s, NEEDS_AUTH_NICK);
  else
  {
  /* List channels you are founder at */
    res = sql_query("SELECT name FROM chanserv WHERE founder=%d", 
	source_snid);
     row = sql_next_row(res);
     if(row) 
     {
      send_lang(u, s, SHOW_FOUNDER_HEADER);
      do 
      {
        ++rowc;
        send_lang(u, s, SHOW_ITEM_X, row[0]);
      } while((row = sql_next_row(res)));
      send_lang(u, s, SHOW_TAIL_X, rowc);
    }
    else
	send_lang(u, s, SHOW_FOUNDER_EMPTY);
    sql_free(res);  
    rowc = 0;
    /* List channels you are successor at */
    res = sql_query("SELECT name FROM chanserv WHERE successor=%d", 
	source_snid);
    row = sql_next_row(res);
    if(row) 
    {
      send_lang(u, s, SHOW_SUCCESSOR_HEADER);
      do 
      {
        ++rowc;
        send_lang(u, s, SHOW_ITEM_X, row[0]);
      } while((row = sql_next_row(res)));
      send_lang(u, s, SHOW_TAIL_X, rowc);
    }
    sql_free(res);

    rowc = 0;

    /* List channels you are successor at */
    res = sql_query("SELECT r.name, c.name, cr.message, cr.flags "
	"FROM cs_role r, chanserv c, cs_role_users cr "
	"WHERE cr.snid=%d AND r.rid=cr.rid AND c.scid=cr.scid", 
	source_snid);
    
    row = sql_next_row(res);
     if(row) 
     {
      send_lang(u, s, SHOW_ROLES_HEADER);
      do 
      {
        char flagstr[64];
        u_int32_t flags = atoi(row[3]);
        if(flags & CRF_REJECTED)
          snprintf(flagstr, sizeof(flagstr),
            " %s", lang_str(u, REJECTED_ROLE));
        else if(flags & CRF_PENDING)
          snprintf(flagstr, sizeof(flagstr),
            " %s", lang_str(u, PENDING_ROLE));        
        else
          flagstr[0] = '\0';
        ++rowc;
        if(row[2])
          send_lang(u, s, SHOW_ROLES_ITEM_X_X_X_X, row[0], row[1], flagstr, row[2]);
        else
          send_lang(u, s, SHOW_ROLES_ITEM_X_X_X, row[0], row[1], flagstr);
      } while((row = sql_next_row(res)));
      send_lang(u, s, SHOW_ROLES_TAIL_X, rowc);
     }
     sql_free(res);  
  }
}
/* End of Module */
