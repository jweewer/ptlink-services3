/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************  

  Desc: log file functions

 *  $Id: log.h,v 1.1.1.1 2005/08/27 15:45:10 jpinto Exp $
*/
#include "stdinc.h"
#define MAX_LOG_FILES 256
#define	LOG_BUFSIZE 512
struct log_entry_t {
  char *logid;
  char *filename;
  int fd;  
  int options;
  char* logchan;
  char* lognick;
  struct tm last_log_tm; /* we need to store last log time */
};
typedef struct log_entry_t log_entry;

/* opens a new log file */
int log_open(char *logid, char *file);

/* returns the log handle for a given logid */
int log_handle(char *logid);

/* logs a message */
int log_log(int loghandle, char *logtype, const char* fmt, ...);

/* sets the irc user/chan associated to a log */
void log_set_irc(int handle, char *lnick, char *lchan);

/* closes a given log id */
int log_close(int loghandle);


