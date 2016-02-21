/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************

  Description: string handling functions

 *  $Id: strhand.h,v 1.3 2005/10/27 22:27:02 jpinto Exp $
*/

#ifndef _STRHAND_H_
#define _STRHAND_H_
#include "stdinc.h" /* we need this for time_t */

extern int ircsprintf(char *str, const char
*format, ...);
int is_email(char *email);
int is_weak_passwd(char *passwd);
void rand_string(char *target, int minlen, int maxlen);
char* hex_str(unsigned char *str, int len);
char* hex_bin(char* source);
char* smalldate(time_t clockt);
char* smalltime(time_t clockt);
void clean_conf_str(char *str);
int match(const char *mask, const char *name);
char* collapse(char *pattern);
int time_str(char *s);
int ftime_str(char *s);
const char *str_time(int t);
void strip_rn(char *txt);
char* irc_lower(char *str);
char* irc_lower_nick(char *str);
int get_pass(char *dest, size_t maxlen);

/* usefull macros */
#define IsNull(x) 	((x) == NULL)
#define FREE(x)		if((x) != NULL) free((x)); x = NULL
#define SDUP(y,x)	if((x) && *(x)!='\0') y=strdup(x); else y=NULL

/*
 * character macros
 */
extern const unsigned char ToLowerTab[];
#define ToLower(c) (ToLowerTab[(unsigned char)(c)])
                                                                                
extern const unsigned char ToUpperTab[];
#define ToUpper(c) (ToUpperTab[(unsigned char)(c)])

/* Mask options come here */
typedef struct {
    char *name;
    int  value;
    void *func;
} OptionMask;

/* mask option functions */
u_int32_t mask_value(OptionMask* opt, char *name);
char *mask_string(OptionMask* opt, u_int32_t mask);
char* validate_options(char* data, OptionMask* opt, u_int32_t *mask);


int irccmp(const char *s1, const char *s2);
int ircncmp(const char *s1, const char *s2, int n);
int strip_reason(char **reason);
int is_posint(char *s);
char* itoa(u_int32_t value);

#endif
