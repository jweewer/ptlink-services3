/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************

  Description: email sending functions

 *  $Id: email.h,v 1.1 2005/09/30 13:59:18 jpinto Exp $
*/

#ifdef EMAIL
int email_load(char *name, char **emails);
int email_send(char *msg);
void email_free(char **email);
void email_init_symbols(void);
void email_add_symbol(char *name, char* value);
char* email_symbol(char *name);
#else
int (*email_load)(char *name, char **emails);
int (*email_send)(char *msg);
void (*email_free)(char **email);
void (*email_init_symbols)(void);
void (*email_add_symbol)(char *name, char* value);
char* (*email_symbol)(char *name);
#endif

#define EMAIL_FUNCTIONS \
MOD_FUNC(email_load) \
MOD_FUNC(email_send) \
MOD_FUNC(email_init_symbols) \
MOD_FUNC(email_add_symbol) \
MOD_FUNC(email_free)
