/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************

  Description: system statistics gathering module

 *  $Id: os_sysstats.c,v 1.6 2005/11/05 10:56:04 jpinto Exp $
*/

#include "module.h"
#include "my_sql.h"

SVS_Module mod_info =
 /* module, version, description */
{"os_sysstats", "1.0",  "system statistics module" };

#define	DB_VERSION 1.0

/* Change Log
  1.0	-  0000299: os_sysstats to log system resources usage
*/



/* interval between statistics collecting */
#define SYSSTATS_INTERVAL 3600

/** functions/events we require **/
/* void (*FunctionPointer)(void);*/
ServiceUser* (*operserv_suser)(void);

MOD_REQUIRES
  MOD_FUNC(operserv_suser)
MOD_END

/** Internal functions declaration **/
void timer_os_sysstats(IRC_User* u, int tag);
/* void internal_function(void); */
    
/** Local variables **/
/* int my_local_variable; */
ServiceUser *osu;
static u_int32_t session_id = 0;

/** load code **/
int mod_load(void)
{
  if(sql_check_inst_upgrade(mod_info.name, DB_VERSION, NULL) < 0)
    return -1;
    
  osu = operserv_suser();
  
  /* get the session id */
  if( sql_singlequery("SELECT session_id FROM os_sysstats"
      " ORDER BY 1 DESC LIMIT 1") > 0  && sql_field_i(0))
    session_id = sql_field_i(0)+1;
  else 
    session_id = 1;

  stdlog(L_INFO, "Logging system statistics with session id %d", session_id);
  
  timer_os_sysstats(NULL, 0); /* running the first time will set the triger */
  
  return 0;
}
    
/** unload code **/
void mod_unload(void)
{
  return;
}
    
/** internal functions implementation starts here **/

#define RU_FIELD(x) sqlb_add_int(#x, my_ru.x)

/* just insert the stats */
void timer_os_sysstats(IRC_User* u, int tag)
{
  struct rusage my_ru;
  int r;
  r = getrusage(RUSAGE_SELF, &my_ru);  
  if(r < 0)
  {
    errlog("Error on rusage");
    return;
  }
  sqlb_init("os_sysstats");
  sqlb_add_int("session_id", session_id);
  sqlb_add_int("t_when", irc_CurrentTime);
  RU_FIELD(ru_maxrss);
  RU_FIELD(ru_ixrss);
  RU_FIELD(ru_idrss);
  RU_FIELD(ru_isrss);
  RU_FIELD(ru_minflt);
  RU_FIELD(ru_majflt);
  RU_FIELD(ru_nswap);
  RU_FIELD(ru_inblock);
  RU_FIELD(ru_oublock);
  RU_FIELD(ru_msgsnd);
  RU_FIELD(ru_msgrcv);
  RU_FIELD(ru_nsignals);
  RU_FIELD(ru_nvcsw);
  RU_FIELD(ru_nivcsw);
  sql_execute("%s", sqlb_insert());  

  /* setup next timer */
  irc_AddUTimerEvent(osu->u, SYSSTATS_INTERVAL , timer_os_sysstats, 0);
}
    
/* End of module */
