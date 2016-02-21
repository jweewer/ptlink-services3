/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2006 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************

  Description: email functions

*/

#include "module.h"
#include "dbconf.h"
#include "path.h" /* ETCPATH */
#define EMAIL
#include "email.h"


SVS_Module mod_info =
 /* module, version, description */
{"email", "1.2",  "just an email module" };
/*  Change Log
  1.1 - #44: invalid string on email sender
*/

/** functions/events we require **/
/* void (*FunctionPointer)(void);*/

MOD_REQUIRES
  MOD_FUNC(dbconf_get_or_build)
MOD_END

/** functions/events we provide **/
/* void my_function(void); */

MOD_PROVIDES
  EMAIL_FUNCTIONS
MOD_END

static char* EmailFrom;
static char* EmailFromName;
DBCONF_PROVIDES
  DBCONF_WORD(EmailFrom,"noreply@pt-link.net", 
    "Email address to be used on the From: field for services emails")
  DBCONF_STR(EmailFromName, "PTlink IRC Services",
    "Name to be used on the From: field for services emails")
DBCONF_END

/** Internal functions declaration **/
/* void internal_function(void); */

/** Local variables **/
static int symcount = 0; /* count of stored symbols */
static char* symbol_names[32];
static char* symbol_data[32];    

/** rehash code **/
int mod_rehash(void)
{
  if(dbconf_get_or_build(mod_info.name, dbconf_provides) < 0 )
  {
    errlog("Error reading dbconf!");
    return -1;
  }
  return 0;
}

/** load code **/
int mod_load(void)
{
#ifndef SENDMAIL
  errlog(
    "sendmail client was not found during configure, email functios are disabled!");
#endif
  return 0;
}
    
/** unload code **/
void mod_unload(void)
{
  return;
}
    
/** internal functions implementation starts here **/


/* free memory allocated for the email templates */
void email_free(char **emails)
{
  int i;
  for(i=0; i<MAX_LANGS;++i)
    free(emails[i]);    
}

/* load an email template from file */
int email_load(char *name, char **emails)
{
  int i;
  char *tld = NULL;
  char buf[2048];
  FILE *f;
  
  for(i=0; i<MAX_LANGS;++i)
  {
    index2lang(i, tld);
    if(tld==NULL)
      continue;
    snprintf(buf, sizeof(buf), "%s/mails/%s.%s", ETCPATH, name, tld);
    if (!(f = fopen(buf, "r")))
      {
        errlog("Failed to load email %s\n", buf);
        return -1;
      }
  
    bzero(buf, sizeof(buf));
    fread(buf, 1, sizeof(buf)-1, f);    
    emails[i] = strdup(buf);    
    fclose(f);
  }
  return 0;
}

/*
 * sens an email, replacing all variables (symbols)
 */
int email_send(char *msg)
{
  char buf[2048];
  char sendmail[PATH_MAX];
  int len;
  char *s, *t;  
  FILE *p;

#ifdef SENDMAIL
  snprintf(sendmail, sizeof(sendmail), "%s -f%s %s",
    SENDMAIL,  EmailFrom, email_symbol("email"));
#else
  slog(L_WARN, "Trying to send auth without SENDMAIL detected");
  return -1;
#endif

  len = strlen(msg);
  if(len > sizeof(buf))
    {
      errlog("Trying to send email too big!");
      return -1;
    }
    
  /* build new buffer applying symbols replacement */
  s = msg;
  t = buf;
  while(*s && ( t-buf < sizeof(buf) ))
  {
    if(*s == '%')
    {
      char *symbol;
      ++s;
      symbol = s;
      while(*s && *s !='%')
        ++s;
      if(*s == '%')
        {
          *s = '\0';
          if(strcasecmp(symbol, "from") == 0)
            t += snprintf(t, sizeof(buf)-(t-buf), "%s", EmailFrom);
          else
          if(strcasecmp(symbol, "from_name") == 0)
            t += snprintf(t, sizeof(buf)-(t-buf), "%s", EmailFromName);
          else
            t += snprintf(t, sizeof(buf)-(t-buf), "%s", email_symbol(symbol));
          *s = '%';
        }
    } 
    else
      *(t++) = *s;
    ++s;
  }
  *t = '\0';

  if (!(p = popen(sendmail, "w")))
    {
      errlog("ERROR, could not popen on email_send()");
      return -1;
    }
  fprintf(p, "%s", buf);
  pclose(p);

  return 0;
}

/*
 * initialize the symbols list
 */
void email_init_symbols(void)
{
  symcount = 0;
}

/*
 * adds a symbol to the symbols list
 */
void email_add_symbol(char *name, char* value)
{
  symbol_names[symcount] = name;
  symbol_data[symcount] = value;
  ++symcount;
}

/* 
 * returns the value of a symbol
 */
char* email_symbol(char *name)
{
  int i;
  for(i = 0; i < symcount; ++i)
  {
    if(strcmp(name, symbol_names[i]) == 0)
      return symbol_data[i];
  }
  return "<undefined>";
}
    
/* End of module */
