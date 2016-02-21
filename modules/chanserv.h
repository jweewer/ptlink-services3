/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************

  Description: chanserv module header file

 *  $Id: chanserv.h,v 1.1.1.1 2005/08/27 15:44:47 jpinto Exp $
*/
#define CFL_PRIVATE     0x00000001      /* chan info is private */
#define CFL_SUSPENDED	0x00000002	/* channel is suspended */
#define CFL_NOEXPIRE    0x00000004      /* chan will not expire */
#define CFL_OPNOTICE	0x00000008	/* send opnotice messages */
#define CFL_RESTRICTED	0x00000010	/* only role users can join */
#define CFL_TOPICLOCK	0x00000020	/* only founder can change the topic */
#define CFL_SECUREOPS	0x00000040	/* only users with role can get ops */


#define IsSuspendedChan(x)          	((x)->flags & CFL_SUSPENDED)
#define SetSuspendedChan(x)         	((x)->flags |= CFL_SUSPENDED)
#define ClearSuspendedChan(x)           ((x)->flags  &= ~CFL_SUSPENDED)

#define IsPrivateChan(x)            	((x)->flags & CFL_PRIVATE)
#define IsOpNotice(x)			((x)->flags & CFL_OPNOTICE)
#define IsRestrictedChan(x)		((x)->flags & CFL_RESTRICTED)
#define IsTopicLock(x)			((x)->flags & CFL_TOPICLOCK)
#define IsSecureOps(x)			((x)->flags & CFL_SECUREOPS)
