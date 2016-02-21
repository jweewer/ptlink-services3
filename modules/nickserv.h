/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************  

  Description: nickserv module header file

 *  $Id: nickserv.h,v 1.3 2005/10/18 16:25:06 jpinto Exp $
*/

#ifndef _NICKSERV_H_
#define _NICKSERV_H_
#include "strhand.h" /* we need for the options mask */

/* nickserv flags */
#define NFL_PRIVATE     0x00000001  	/* nick info is private */
#define NFL_SUSPENDED	0x00000002	/* nick is suspended */
#define NFL_NOEXPIRE    0x00000004  	/* nick will not expire */
#define NFL_NONEWS	0x00000008  	/* nick should received  newsletter */
#define NFL_HIDEEMAIL	0x00000010	/* nick wants to hide email from ns info */
#define NFL_AUTHENTIC	0x00000020	/* nick email was authenticated */
#define NFL_PROTECTED	0x00000040	/* nick is protected */
#define NFL_USEMSG	0x00000080	/* nick wants msg instead of notices */
/* NOTE: NFL_USEMSG is also defined on src/lang.c ! */ 

/* nickserv status */
#define NST_ONLINE	0x00000001	/* nick is online */
#define NST_FULLREG	0x00000002	/* nick is fully registered, will get +r */

/* misc macros */
#define IsPrivateNick(x)        ((x)->flags & NFL_PRIVATE)
#define IsHideEmail(x)		((x)->flags & NFL_HIDEEMAIL)

#define IsOnline(x)		((x)->status & NST_ONLINE)
#define SetOnline(x)		((x)->status |= NST_ONLINE)
#define ClearOnline(x)		((x)->status &= ~NST_ONLINE)

#define IsAuthenticated(x)	((x)->flags & NFL_AUTHENTIC)
#define SetAuthenticated(x)	((x)->flags |= NFL_AUTHENTIC)
#define ClearAuthenticated(x)	((x)->flags &= ~NFL_AUTHENTIC)

#define IsProtected(x)		((x)->flags & NFL_PROTECTED)
#define SetProtected(x)		((x)->flags |= NFL_PROTECTED)
#define ClearProtected(x)	((x)->flags &= ~NFL_PROTECTED)

#define WantsMsg(x)		((x)->flags & NFL_USEMSG)

#define IsFullRegistered(x)	((x)->status & NST_FULLREGISTERED)


OptionMask nick_options_mask[] =
  {
    { "private", NFL_PRIVATE, NULL },
    { "nonews", NFL_NONEWS, NULL },
    { "noexpire", NFL_NOEXPIRE, NULL },
    { "hideemail", NFL_HIDEEMAIL, NULL },
    { "protected", NFL_PROTECTED, NULL },
    { "suspended", NFL_SUSPENDED, NULL },
    { "usemsg", NFL_USEMSG, NULL },
    { NULL }
};

/* extra data constants defined here */
#define ED_GROUPS 0

#ifdef NICKSERV
#else
#endif

#endif
