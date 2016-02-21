#include "stdinc.h"
#include "irc_parse.h"
#include "irc_string.h"
#include "send.h"
#include "ircservice.h"
#include "common.h"
#include "raw.h"

extern FILE* ircslogf;
/*
 * NOTE: irc_parse() should not be called recursively by other functions!
 */
static  char    *para[MAXPARA+1];
static  char    sender[HOSTLEN+1];
static  char    bckbuffer[1024];

/*
 * parse a buffer.
 *
 * NOTE: irc_parse() should not be called recursively by any other functions!
 */
int irc_parse(char *buffer, char *bufend)
{
  char  *ch;
  char  *s;
  int   i;
  char* numeric = 0;
  int   paramcount;


  
  strncpy(bckbuffer, buffer, sizeof(bckbuffer));  /* save buffer for dump on debug  */

  if(ircs_debug)
    fprintf(ircslogf, "Parsing: %s\n", buffer);

  s = sender;
  *s = '\0';

  for (ch = buffer; *ch == ' '; ch++)   /* skip spaces */
    /* null statement */ ;

  para[0] = NULL;

  if (*ch == ':')
    {
      ch++;

      /*
      ** Copy the prefix to 'sender' assuming it terminates
      ** with SPACE (or NULL, which is an error, though).
      */
      for (i = 0; *ch && *ch != ' '; i++ )
	    {
	      if ((unsigned int)i < (sizeof(sender)-1))
	        *s++ = *ch; /* leave room for NULL */
	      ch++;
	    }
      *s = '\0';
      i = 0;

      if (*sender)
          para[0] = sender;

      while (*ch == ' ')
        ch++;
    }

  if (*ch == '\0')
    {	
      fprintf(ircslogf, "Empty message from host %s",
              para[0]);
      return(-1);
    }

  /*
  ** Extract the command code from the packet.  Point s to the end
  ** of the command code and calculate the length using pointer
  ** arithmetic.  Note: only need length for numerics and *all*
  ** numerics must have parameters and thus a space after the command
  ** code. -avalon
  *
  * ummm???? - Dianora
  */

  if( *(ch + 3) == ' ' && /* ok, lets see if its a possible numeric.. */
      IsDigit(*ch) && IsDigit(*(ch + 1)) && IsDigit(*(ch + 2)) )
    {
      numeric = ch;
      paramcount = MAXPARA;
      s = ch + 3;       /* I know this is ' ' from above if */
      *s++ = '\0';      /* blow away the ' ', and point s to next part */
    }
  else
    {
      s = strchr(ch, ' ');      /* moved from above,now need it here */
      if (s)
        *s++ = '\0';

      paramcount = MAXPARA;
      i = bufend - ((s) ? s : ch);

    }
  /*
  ** Must the following loop really be so devious? On
  ** surface it splits the message to parameters from
  ** blank spaces. But, if paramcount has been reached,
  ** the rest of the message goes into this last parameter
  ** (about same effect as ":" has...) --msa
  */

  /* Note initially true: s==NULL || *(s-1) == '\0' !! */

  /* ZZZ hmmmmmmmm whats this then? */

  i = 1;

  if (s)
    {
      if (paramcount > MAXPARA)
        paramcount = MAXPARA;

      for (;;)
        {
	  while(*s == ' ')	/* tabs are not considered space */
	    *s++ = '\0';

          if(!*s)
            break;

          if (*s == ':')
            {
              /*
              ** The rest is single parameter--can
              ** include blanks also.
              */
              para[i++] = s + 1;
              break;
            }
	  else
	    {
	      para[i++] = s;
              if (i > paramcount)
                {
                  break;
                }
              /* scan for end of string, either ' ' or '\0' */
              while (IsNonEOS(*s))
                s++;
	    }
        }
    }

  para[i] = NULL;
  
  
  DoRawHandlers(ch,i, para);
  return 1;
}

char* irc_GetLastMsg(void)
{
  return bckbuffer;
}
