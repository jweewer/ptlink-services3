/*****************************************************************
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License          *
 * Please read the file COPYING for copyright information.       *
 *****************************************************************
                                                                                
  File: log.c
  Description: log handling functions

 *  $Id: log.c,v 1.2 2005/10/18 16:25:06 jpinto Exp $
*/

#include "stdinc.h"
#include "path.h"
#include "ircsvs.h"
#include "strhand.h"
#include "s_log.h"
#include "log.h"
#include "ircservice.h" /* we use irc_SendRaw */

static int log_count = 0;
static log_entry log_list[MAX_LOG_FILES];
static time_t t_now;

static void write_log(int logFile, char* logevent, char *message)
{
  char buf[LOG_BUFSIZE];
  if(logevent)
    snprintf(buf, LOG_BUFSIZE, "%s [%s] %s\n", smalltime(t_now), logevent, message);
  else
    snprintf(buf, LOG_BUFSIZE, "%s %s\n", smalltime(t_now), message);
  write(logFile, buf, strlen(buf));
}

/* opens a new log file */
int log_open(char *logid, char *filename)
{
  log_entry le;
  char fullfilename[512];
  int logFile;
  struct tm tm;
  char buf[256];
  
  assert(log_count<MAX_LOG_FILES);
  t_now = time(NULL);
  tm = *localtime(&t_now);   
  strftime(buf, sizeof(buf), "%Y%m%d", &tm);
  
  snprintf(fullfilename, 512, "%s/%s_%s.log", LOGPATH, filename, buf);
  logFile = open(fullfilename,
            O_WRONLY | O_APPEND | O_CREAT | O_NONBLOCK, 0644);
            
  if (-1 == logFile)
     return -1; 
                          
  le.logid = strdup(logid);
  le.filename = strdup(filename);
  le.fd = logFile;
  le.last_log_tm = tm;
  log_list[log_count] = le;
  log_list[log_count].logchan = NULL;
  log_list[log_count].lognick = NULL;
  /* mark end of list */
  log_list[log_count+1].logid = NULL;  
  return log_count++;
}

/* returns the log handle for a given logid */
int log_handle(char *logid)
{
 int i;
 for(i = 0; i< log_count; ++i)
   {
     if(log_list[i].logid && (strcmp(log_list[i].logid, logid)==0))
       return i;
   }
  return -1;  
}

int log_log(int loghandle, char *logevent, const char* fmt, ...)
{
  char  buf[LOG_BUFSIZE];
  char 	buf2[32];
  va_list args;
  struct  tm *lt;
  char fullfilename[512];
  log_entry* loge;

  /* write the log file */
  va_start(args, fmt);
  vsnprintf(buf, LOG_BUFSIZE, fmt, args);
  va_end(args);
  
  if(loghandle < 0)
    {
      errlog("Attempt to log %d: %s", loghandle, buf);
      return -11;
    }
  
  loge = &log_list[loghandle];
  /* Check if we have a day change for log filename rotation */
  t_now = time(NULL);
  lt = localtime(&t_now);
  if(lt->tm_mday != loge->last_log_tm.tm_mday)
   {
     close(loge->fd);
     strftime(buf2, sizeof(buf2), "%Y%m%d", lt);                                                                                
     snprintf(fullfilename, 512, "%s/%s_%s.log", 
       LOGPATH, loge->filename, buf2);
     loge->fd = open(fullfilename,
       O_WRONLY | O_APPEND | O_CREAT | O_NONBLOCK, 0644);     
     loge->last_log_tm = *lt;
   }

  if(loge->logchan)
    irc_SendRaw(loge->lognick, "PRIVMSG %s :%s", loge->logchan, buf);
    
  write_log(loge->fd, logevent, buf);
    
  return 1;
}

void log_set_irc(int handle, char *lnick, char *lchan)
{
  if((lnick == NULL) || (lchan == NULL) || (handle < 0))
    return;
  FREE(log_list[handle].logchan);
  log_list[handle].lognick = lnick;    
  log_list[handle].logchan = strdup(lchan);
}

/* closes a given log id */
int log_close(int loghandle)
{
  return 1;  
}
