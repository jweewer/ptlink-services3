/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2004 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************

  Description: service log functions

 *  $Id: s_log.c,v 1.2 2005/10/18 16:25:06 jpinto Exp $
*/ 
 
#include "s_log.h"
#include "stdinc.h"
#include "irc_string.h"
#include "strhand.h"
#include "ircsvs.h"
 
#define LOG_BUFSIZE 2048 


static void* logFunc = NULL;
static int logFile = -1;
static int logLevel = L_INFO;

static const char *logLevelToString[] =
{ "L_CRIT",
  "L_ERROR",
  "L_WARN",
  "L_NOTICE",
  "L_TRACE",
  "L_INFO",
  "L_DEBUG"
};

/*
 * open_log - open ircd logging file
 * returns true (1) if successful, false (0) otherwise
 */
static int open_log(const char* filename)
{
  logFile = open(filename, 
                 O_WRONLY | O_APPEND | O_CREAT | O_NONBLOCK, 0644);                 
  if (-1 == logFile) {
    return 0;
  }
  return 1;
}


void close_log(void)
{

  if (-1 < logFile) {
    close(logFile);
    logFile = -1;
  }
}


/* writes a string to the logfile prefixing with the current time */
static void write_log(const char* message)
{
  char buf[LOG_BUFSIZE];
  snprintf(buf, LOG_BUFSIZE, "[%s] %s\n", smalldate(CurrentTime), message);
  if(logFile == -1)
    printf("%s", buf);
  else
    write(logFile, buf, strlen(buf));
}


/** logs a message, if the priority is lower than the minimum to be logged
  the message will be discarted */  
void slog(int priority, const char* fmt, ...)
{
  char    buf[LOG_BUFSIZE];
  va_list args;
  assert(0 != fmt);

  if (priority > logLevel)
    return;

  va_start(args, fmt);
  vsnprintf(buf, LOG_BUFSIZE, fmt, args);
  va_end(args);


  write_log(buf);

}

/** same as slog but also prints the message to stderr */
void errlog(const char* fmt, ...)
{
  char    buf[LOG_BUFSIZE];
  va_list args;
  assert(0 != fmt);

  va_start(args, fmt);
  vsnprintf(buf, LOG_BUFSIZE, fmt, args);
  va_end(args);

  write_log(buf);
    
  if(logFunc)
    ((void (*)(char* message)) logFunc)(buf);  
  else
    fprintf(stderr, "%s\n", buf);
}

/** same as slog but also prints the message to stdout */
void stdlog(int priority, const char* fmt, ...)
{
  char    buf[LOG_BUFSIZE];
  va_list args;
  assert(-1 < priority);
  assert(0 != fmt);

  va_start(args, fmt);
  vsnprintf(buf, LOG_BUFSIZE, fmt, args);
  va_end(args);

  if (priority <= logLevel)
    write_log(buf);

  if(logFunc)
    ((void (*)(char* message)) logFunc)(buf);  
  else
    fprintf(stdout, "%s\n", buf);    
}

void log_perror(int priority, const char* fmt, ...)
{
  char    buf[LOG_BUFSIZE];
  va_list args;
  int errno_save = errno;
  assert(-1 < priority);
  assert(0 != fmt);

  if (priority > logLevel)
    return;

  va_start(args, fmt);
  vsnprintf(buf, LOG_BUFSIZE, fmt, args);  
  va_end(args);
  strcat(buf,strerror(errno_save));
  write_log(buf);
  errno = errno_save;
}

  
int init_log(const char* filename)
{
  return open_log(filename);
}

void set_log_level(int level)
{
  if (L_ERROR < level && level <= L_DEBUG)
    logLevel = level;
}

int get_log_level(void)
{
  return( logLevel );
}

const char *get_log_level_as_string(int level)
{
  if(level > L_DEBUG)
    level = L_DEBUG;
  else if(level < L_ERROR)
    level = L_ERROR;

  return(logLevelToString[level]);
}

/* returns the log file descriptior */
int get_log_fd(void)
{
  return logFile;
}

/* sets an auxiliary log function */
void set_log_aux(void *func)
{
  logFunc = func;
}
