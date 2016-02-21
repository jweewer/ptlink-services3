/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************  

  Description: nickserv common macros

 *  $Id: nsmacros.h,v 1.2 2005/10/10 16:52:07 jpinto Exp $
*/
#ifndef _NSMACROS_H_
#define _NSMACROS_H
#include "lang/nscommon.lh"
/* 
  The follwing macro will check if the nick is registered and identified 
  and open the nick record 
*/
#define CHECK_IF_IDENTIFIED_NICK \
    if(u->snid == 0)\
      {\
        send_lang(u, s, NICK_NOT_IDENTIFIED);\
        return;\
      }\
    else source_snid = u->snid;
#endif

#define CHECK_DURATION(x) \
  duration = 0; \
  if((x) && (x)[0]=='+') \
  {\
    duration = ftime_str((x)); \
    if(duration < 0)\
    {\
       send_lang(u, s, INVALID_TIME_X, (x)); \
       return; \
    } \
    else \
      (x) = strtok(NULL, " "); \
  }
