/*****************************************************************
 * libircservice is (C) CopyRight PTlink IRC Software 1999-2005  *
 *                    http://software.pt-link.net                *
 * This program is distributed under GNU Public License          *
 * Please read the file COPYING for copyright information.       *
 *****************************************************************

  Desc: hash routines

 *  $Id: hash.h,v 1.1.1.1 2005/08/27 15:44:23 jpinto Exp $
*/

#ifndef INCLUDED_hash_h
#define INCLUDED_hash_h
#include "stdinc.h"
#include "ircservice.h"
#include "irc_string.h"
/* 
 * Client hash table size
 *
 */
#define U_MAX 65536

/* 
 * Local client hash table size
 *
 */
#define LU_MAX 256

/* 
 * Channel hash table size
 *
 * used in hash.c
 */
#define CH_MAX 16384

struct HashEntry {
  int    hits;
  int    links;
  void*  list;
};


extern struct HashEntry hash_get_channel_block(int i);
extern size_t hash_get_user_table_size(void);
extern size_t hash_get_channel_table_size(void);
extern void   init_hash(void);
extern void   del_from_user_hash_table(const char* name, 
                                         IRC_User* user);
extern void   del_from_localuser_hash_table(const char* name, 
                                         IRC_User* user);                                         
extern void   add_to_user_hash_table(const char* name, 
                                        IRC_User* use);                                         
extern void   add_to_localuser_hash_table(const char* name, 
                                        IRC_User* use);
extern void   add_to_channel_hash_table(const char* name, 
                                        IRC_Chan* chan);
extern void   del_from_channel_hash_table(const char* name, 
                                          IRC_Chan* chan);
extern IRC_Chan* hash_find_channel(const char* name); 
extern IRC_User* hash_find_user(const char* name);
extern IRC_User* hash_find_server(const char* name);
extern IRC_User* hash_find_localuser(const char* name);
extern unsigned int hash_nick_name(const char* name);
extern unsigned int hash_channel_name(const char* name);
extern IRC_User* hash_next_localuser(int reset);
extern IRC_Chan* hash_next_channel(int reset);

extern void  clear_user_hash_table();
#endif  /* INCLUDED_hash_h */



