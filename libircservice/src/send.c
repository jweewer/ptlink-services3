/*****************************************************************
 * libircservice is (C) CopyRight PTlink IRC Software 1999-2003   *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License          *
 * Please read the file COPYING for copyright information.       *
 *****************************************************************
 
  File: send.c
  Description: send routines
  Author: Lamego@PTlink.net
*/

#include "stdinc.h"
#include "send.h"
#include "sockutil.h"
#include "ircservice.h"
#include "common.h"

#include <stdarg.h>
#include <stdio.h>

/* internal functions */
void vsendto_ircd(char *source, const char *fmt, va_list args);

void vsendto_ircd(char *source, const char *fmt, va_list args)
{
    char buf[BUFSIZE];

    vsnprintf(buf, sizeof(buf), fmt, args);
    if (source!=NULL) {
        sockprintf(ircd_fd, ":%s %s\r\n", source, buf);    
        if(ircs_debug)
          fprintf(ircslogf,"Sent: :%s %s\n", source, buf);
    } else {
        sockprintf(ircd_fd, "%s\r\n", buf);
        if(ircs_debug)
          fprintf(ircslogf,"Sent: %s\n", buf);
    }
}

void sendto_ircd(char *source, const char *fmt, ...)
{
  va_list args;

  va_start(args, fmt);
  vsendto_ircd(source, fmt, args);
  va_end(args);
}

/* I need to remove the sendto_ircd above */
void irc_SendRaw(char *source, const char *fmt, ...)
{
  va_list args;

  va_start(args, fmt);
  vsendto_ircd(source, fmt, args);
  va_end(args);
}

void send_notice(char *source, const char *target, const char *fmt, ...)
{
  char buf[512];
  va_list args;
  
  va_start(args, fmt);    
  vsnprintf(buf, sizeof(buf), fmt, args);
  sendto_ircd(source, "NOTICE %s :%s", target, buf);
  va_end(args);
}

void send_msg(char *source, const char *target, const char *fmt, ...)
{
  char buf[512];
  va_list args;
  
  va_start(args, fmt);    
  vsnprintf(buf, sizeof(buf), fmt, args);
  sendto_ircd(source, "PRIVMSG %s :%s", target, buf);
  va_end(args);
}
