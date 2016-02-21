/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************  

  Description: memoserv header file

 *  $Id: memoserv.h,v 1.1.1.1 2005/08/27 15:44:26 jpinto Exp $
*/

/* memoserv flags */
#define MFL_UNREAD	0x000000001
#define MFL_SAVED	0x000000002

/* memoserv option flags */
#define MOFL_AUTOSAVE	0x000000001
#define MOFL_FORWARD	0x000000002
#define MOFL_NOMEMOS	0x000000004

OptionMask memoserv_options[] = 
{
  {"autosave", MOFL_AUTOSAVE, NULL },
  {"forward", MOFL_FORWARD, NULL },
  {"nomemos", MOFL_NOMEMOS, NULL },
  { NULL }
};

/* how many chars should be on the preview message */
#define MEMOPREVMAX 20

#ifdef MEMOSERV
int memoserv_get_options(u_int32_t snid, int* maxmemos, int* bquota, u_int32_t* flags);
int memos_count(u_int32_t snid);
int unread_memos_count(u_int32_t snid);
#else
int (*memoserv_get_options)(u_int32_t snid, int* maxmemos, int* bquota, u_int32_t* flags);
int (*memos_count)(u_int32_t snid);
int (*unread_memos_count)(u_int32_t snid);
#endif

#define MEMOSERV_FUNCTIONS \
  MOD_FUNC(memoserv_suser) \
  MOD_FUNC(memoserv_get_options) \
  MOD_FUNC(memos_count) \
  MOD_FUNC(unread_memos_count)  
