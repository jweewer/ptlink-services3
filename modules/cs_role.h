/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2004 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************

  Description: chanserv role header file

 *  $Id: cs_role.h,v 1.3 2005/10/14 18:37:55 jpinto Exp $
*/ 

/* role permissions */
#define P_SET		0x00000001	/* can use the set command */
#define P_KICK		0x00000002	/* can use kick command */
#define P_OPDEOP	0x00000004	/* can user op/deop command */
#define P_LIST		0x00000008	/* can use list command */
#define P_VIEW		0x00000010	/* can use view command */
#define P_VOICEDEVOICE	0x00000020      /* can use voice/devoice command */
#define P_INVITE	0x00000040	/* can use the invite command */
#define P_UNBAN		0x00000080	/* can use the unban command */
#define P_CLEAR		0x00000100	/* can use the clear command */
#define P_AKICK		0x00000200	/* can use the akick command */
#define P_HOPDEHOP	0x00000400	/* can user hop/dehop command */

/* flags for role users */
#define CRF_PENDING     0x00000001      /* pending for approval */
/* if not pending and not rejected it was accepted */
#define CRF_REJECTED    0x00000002      /* role was rejected */
#define CRF_SUSPENDED   0x00000004      /* role was suspended */

#ifdef CS_ROLE
int role_with_permission(u_int32_t scid, u_int32_t snid, int permission);
#else
int (*role_with_permission)(u_int32_t scid, u_int32_t snid, int permission);
#endif

