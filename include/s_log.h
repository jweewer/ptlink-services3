/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************  

  Desc: server log functions

 *  $Id: s_log.h,v 1.2 2005/10/18 16:25:06 jpinto Exp $
*/
#ifndef S_LOG_H
#define S_LOG_H

#define L_CRIT    0
#define L_ERROR   1
#define L_WARN    2
#define L_NOTICE  3
#define L_TRACE   4
#define L_INFO    5
#define L_DEBUG   6

#define USE_LOGFILE

int init_log(const char* filename);
void close_log(void);
void set_log_level(int level);
int get_log_level(void);
void slog(int priority, const char* fmt, ...);
void errlog(const char* fmt, ...);
void stdlog(int priority, const char* fmt, ...);
void log_perror(int priority, const char* fmt, ...);
extern const char *get_log_level_as_string(int level);
int get_log_fd(void);
void set_log_aux(void *func);
#endif /* S_LOG_H */
