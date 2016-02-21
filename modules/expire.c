/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************

  Description: data expiration trigger routine

 *  $Id: expire.c,v 1.4 2005/10/18 16:25:06 jpinto Exp $
*/

#include "module.h"
#include "dbconf.h"

SVS_Module mod_info =
 /* module, version, description */
{"expire", "1.0",  "data expiration module" };

/** functions/events we require **/
/* void (*FunctionPointer)(void);*/
static int e_expire;

MOD_REQUIRES
  MOD_FUNC(dbconf_get_or_build)
  MOD_FUNC(e_expire)
MOD_END


/** Internal functions declaration **/
void ev_expire(IRC_User* u, char* reason);
    
/*
 * List of dbconf items we provide
 */
static int Interval; 
 
DBCONF_PROVIDES
  DBCONF_TIME(Interval, "1h", "Time between data expiration runs")
DBCONF_END

/** Local variables **/
/* int my_local_variable; */

/** rehash code **/
/* this is called before load and at services rehash */
int mod_rehash(void)
{
  if(dbconf_get_or_build(mod_info.name, dbconf_provides) < 0 )
  {
    errlog("Error reading dbconf!");
    return -1;
  }
  return 0;
}

/** load code **/
int mod_load(void)
{
  if(Interval)
  {
    stdlog(L_INFO, "Running expire routines...");
    ev_expire(NULL, NULL);    
    stdlog(L_INFO,"Expire interval set to %d minute(s)", Interval / 60);
    irc_AddEvent(ET_LOOP, ev_expire); /* set the expire routines */
  }
  else
    stdlog(L_WARN, "Data expiration is disabled");
  return 0;
}

/** unload code **/
void mod_unload(void)
{
   irc_DelEvent(ET_LOOP, ev_expire);
}
    
/** internal functions implementation starts here **/
/*
   this function will launch the expire events
   according to the Interval setting
*/
void ev_expire(IRC_User* u, char* reason)
{
  static time_t last_expire_run;
  if(irc_CurrentTime - last_expire_run < Interval)
    return;
  mod_do_event(e_expire, NULL, NULL); /* do the expire */
  last_expire_run = irc_CurrentTime;
}
    
/* End of module */
