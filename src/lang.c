/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************
   
  Description: language related routines
 
 *  $Id: lang.c,v 1.5 2005/10/18 16:25:06 jpinto Exp $
*/
#include "stdinc.h"
#include "ircservice.h"
#include "s_log.h"
#include "strhand.h"
#include "lang.h"

/* this flag is from nickserv.h but needs to be here also */
#define NFL_USEMSG    0x00000100      /* nick wants msg instead of notices */
#define WantsMsg(x)             ((x)->flags & NFL_USEMSG)

/* array for lang associations */
static char* tld_table[40];
static int tld_i_table[40];
static int ass_count = 0;

/* delete lang associations */
void lang_delete_assoc(void)
{
  while(ass_count)  
    {
       free(tld_table[--ass_count]);
    }
}

char* lang_str_l(int lang, const char* message[], ...)
{
  static char buf[4096];  /* to get the final message */     
  int i;
  va_list args;  
      
  va_start(args, message);  
  vsnprintf(buf, sizeof(buf), message[lang], args);
  va_end(args);  
  i = strlen(buf)-1;  
  if(i>-1 && buf[i]=='\n') /* remove trailing end of line */
    buf[i] = '\0';
    
  return buf;
}

char* lang_str(IRC_User *u, const char* message[], ...)
{
  static char buf[4096];  /* to get the final message */     
  int i;
  va_list args;  
      
  va_start(args, message);  
  vsnprintf(buf, sizeof(buf), message[u->lang], args);
  va_end(args);
  i = strlen(buf)-1;  
  if(i>-1 && buf[i]=='\n') /* remove trailing end of line */
    buf[i] = '\0';
    
  return buf;
}

char* format_str(IRC_User *u, const char* message[])
{
  static char buf[4096];
  int i;      
  strncpy(buf, message[u->lang], sizeof(buf));  
  
  i = strlen(buf)-1;  
  if(i>-1 && buf[i]=='\n') /* remove trailing end of line */
    buf[i] = '\0';  
    
  return buf;
}

/* sends a message to an user */
void send_lang(IRC_User *u, IRC_User *s, const char* message[], ...)
{
  char *c, *lastc;
  va_list args;
  char *fmt;
  char buf[4096];  /* to get the final message */     
  char buf2[4096]; /* to do the "//" replacement */
  const char *cp;  
  int i = 0;
  
  cp = message[u->lang];
  bzero(buf2, sizeof(buf2));
  /* lets replace "//" with the sender name */
  while(*cp)
  {
    if((*cp=='/') && (*(cp+1)=='/') && (*(cp-1)!=':'))
    {
      cp+=2;
      strcat(buf2,"/");
      strcat(buf2,s->nick);
      i = strlen(buf2);
      continue;
    }
    buf2[i++] = *cp;
    cp++;
  }
  fmt = buf2;
  va_start(args, message);  
  vsnprintf(buf, sizeof(buf), fmt, args);
  va_end(args);  
  lastc=buf;
  c = strchr(lastc,'\n');
  while(c && *(lastc))
  {      
    *c='\0';
    if(WantsMsg(u))
      irc_SendMsg(u, s, "%s", lastc);
    else
      irc_SendNotice(u, s, "%s", lastc);
    *c='\n';
    lastc=c+1;
    c = strchr(lastc,'\n');
  }
}

/* Associates a list of TLDs with a language
  The lang string should be: "lang:tld,tld"
 */
int AssociateLang(char *lstring)
{
  char* langstr, *tldstr;
  int li; 			/* lang index */  
  li = -1;
  
  langstr = strtok(lstring,":");

  if(!IsNull(langstr))
    {
      lang2index(langstr, li)
    }
    
  if(li == -1)
    {
      errlog("Unknown language %s on AssociateLang", langstr);
      return -1;
    }
  tldstr = strtok(NULL, ",");
  while(tldstr)
    {
      tld_table[ass_count] = strdup(tldstr);
      tld_i_table[ass_count] = li;
      ++ass_count;
      tldstr = strtok(NULL, ",");
    }
  return 0;
}

/* returns the index of the language assigned to a given user 
 -1 if no association was found */
int lang_for_host(char *host)
{
   int i = 0;
   char *p;
   p = host+strlen(host)-1; /* go for the tail */
   while(*p != '.')
     --p;
     
   while(i < ass_count)
     {
       if(tld_table[i][0]=='*') /* We have hit the default language */
         return tld_i_table[i];
         
       if(strcmp(tld_table[i], p) == 0)  /* we have a match */
         return tld_i_table[i];
       ++i;
     }
  return 0;
}
