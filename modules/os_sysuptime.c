/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************

  Description: module o_sysuptime
  Idea and code from: http://www.anope.org/modules/os_sysuptime.c

 *  $Id: os_sysuptime.c,v 1.3 2005/11/03 21:46:01 jpinto Exp $
*/
#include "module.h"
#include "ns_group.h"
#include "lang/common.lh"
#include "lang/os_sysuptime.lh"

#ifdef __OpenBSD__
#include <sys/param.h>
#include <sys/sysctl.h>
#endif /* __OpenBSD__ */



SVS_Module mod_info =
 /* module, version, description */
{"o_sysuptime", "1.1",  "just a o_sysuptime module" };

/* Change Log
  1.1 - security fix 
  1.0 - #25: os_sysuptime.c based on the anope module
*/
/** functions/events we require **/
/* void (*FunctionPointer)(void);*/
ServiceUser* (*operserv_suser)(void);

MOD_REQUIRES
  MOD_FUNC(operserv_suser)
  MOD_FUNC(is_soper)
MOD_END

/* internal functions */
void os_sysuptime(IRC_User *s, IRC_User *u);
char *my_sysuptime(void);

/* Local variables */
static ServiceUser *osu;

/** load code **/
int mod_load(void)
{
  osu = operserv_suser();
  suser_add_cmd(osu, "SYSUPTIME", os_sysuptime,
  	OS_SYSUPTIME_SUMMARY, OS_SYSUPTIME_SUMMARY);
  return 0;
}

void os_sysuptime(IRC_User *s, IRC_User *u)
{
  char *tmp =  my_sysuptime();

  if (!is_soper(u->snid))
  {
    send_lang(u, s, PERMISSION_DENIED);
    return;
  }
  
  send_lang(u, s, OS_SYSUPTIME_X, tmp);
/*   irc_SendNotice(u, s, "Host OS uptime: %s", tmp); */
  free(tmp);
}
    
/** internal functions implementation starts here **/    
char *my_sysuptime(void) {
	FILE *fp;
	int seconds;
	int days = 0, hours = 0, minutes = 0;
	char tmp[40],*ptr;
#ifdef __OpenBSD__ 
	int mib[2];
	size_t size;
	time_t now;
	struct timeval boottime;

	mib[0] = CTL_KERN;
	mib[1] = KERN_BOOTTIME;
	size = sizeof(boottime);
	if (sysctl(mib, 2, &boottime, &size, NULL, 0) < 0)
		return (NULL);
	time(&now);
	seconds = now - boottime.tv_sec;
	seconds += 30;
#else	
	fp = fopen("/proc/uptime","r");
	if( fp == NULL ) {
		return NULL;
	}
	fscanf(fp,"%d",&seconds);
	fclose(fp);
#endif /* __OpenBSD__ */
	days = seconds / 86400;
	seconds -= (days * 86400);
	hours = seconds / 3600;
	seconds -= (hours * 3600);
	minutes = seconds / 60;

	sprintf(tmp,"\2%d\2 days \2%d\2 hours \2%d\2 minutes",days,hours,minutes);
	ptr = (char *)calloc(strlen(tmp)+1,sizeof(char));
	if( ptr != NULL) {
		strcpy(ptr,tmp);
	} else {
		return NULL;
	}
	return ptr;
}
