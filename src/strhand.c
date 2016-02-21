#include "stdinc.h"
#include "strhand.h"

time_t CurrentTime;

char* smalldate(time_t tclock)
{
  static  char    buf[64];
  struct  tm *lt, *gm;
  struct  tm      gmbuf;
 
  if (!tclock)
    time(&tclock);
  gm = gmtime(&tclock);
  memcpy((void *)&gmbuf, (void *)gm, sizeof(gmbuf));
  gm = &gmbuf;
  lt = localtime(&tclock);
 
  snprintf(buf, 64, "%d/%02d/%02d %02d:%02d.%02d",
             lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday,
             lt->tm_hour, lt->tm_min, lt->tm_sec);
 
  return buf;
}

char* smalltime(time_t tclock)
{
  static  char    buf[64];
  struct  tm *lt, *gm;
  struct  tm      gmbuf;
 
  if (!tclock)
    time(&tclock);
  gm = gmtime(&tclock);
  memcpy((void *)&gmbuf, (void *)gm, sizeof(gmbuf));
  gm = &gmbuf;
  lt = localtime(&tclock);
 
  snprintf(buf, 64, "%02d:%02d.%02d",
             lt->tm_hour, lt->tm_min, lt->tm_sec);
 
  return buf;
}

/*************************************************************************
 Returns 0 if email is invalid
 *************************************************************************/
int is_email(char *email) 
{
    char *i=NULL, *j=NULL;
    char *validation;
    char *invalid = ";:|/,&()<>[]!*?`'";
    if (strlen(email)>51)
    	return 0;
    	
    i = strchr(email ,'@');
    
    if(i)
    	j = strchr(i ,'.');    

    if(!i || !j || (j-i<3))
    	return 0;
    
    validation = email;
    while(*validation)
      {
        if(strchr(invalid, *validation))
          return 0;
        ++validation;
      }

    		
    while( (i=strchr(j ,'.')) )
    {
    	j = i;
    	++j;	
    }

    if(strlen(email)- ((int)(j-email)) <2)
    	return 0;    
    
   return 1;	
}    

/* check if password is at least 8 chars long and uses some non-alfabetic  */
int is_weak_passwd(char *passwd)
{
  char *c;
  if(strlen(passwd)<7)
    return -1;
  c = passwd;
  while(*c)
    {
      if(!isalpha(*c))   
        return 0;
       ++c;
    }
  return -1;
}

/* generates a random string */
void rand_string(char *target, int minlen, int maxlen)
  {
        int i;
    int len=minlen+(random()%(maxlen-minlen+1));
    for(i=0;i<len;++i)
          {
                target[i]= 'a'+(random()%('z'-'a'+1));
        if(random()%2) target[i]=toupper(target[i]);
          }
    target[i]='\0';
  }


/*
 * Returns hexadecimal representation of a sequence of bytes *
 */
char* hex_str(unsigned char *str, int len)
{
  static char hexTab[] = "0123456789abcdef";
  static char buffer[200];
  int i;
  if(str == NULL)
    return NULL;
  if(len*2>(sizeof(buffer)-1))
    abort();
  for(i=0;i<len;++i)
    {
      buffer[i*2]=hexTab[(str[i] >> 4) & 0x0F];
      buffer[(i*2)+1]=hexTab[str[i] & 0x0F];
    }
  return buffer;
} 

char* hex_bin(char* source)
{
  static char dest[200];
  int i,h1,h2;  
  char *s;
  i = 0;
  s = source;
  while(*s && *(s+1))
    {
      *s = toupper(*s);
      *(s+1) = toupper(*(s+1));
      if(*s>'9')
        h1 = 10 + (*s-'A');
      else 
        h1 = *s - '0'; 
      if(*(s+1)>'9')
        h2 = 10 + (*(s+1)-'A');
      else 
        h2 = *(s+1) - '0';         
      dest[i++] = (h1 << 4) + h2;
      s += 2;
    }
    dest[i]='\0';
    return dest;
} 

void clean_conf_str(char *str)
{
  char *c;
  c = strchr(str, '#');
  if(c)
   *c = '\0';
  c = strchr(str, '\n');
  if(c)
   *c = '\0';   
  c = strchr(str, '\r');
  if(c)
   *c = '\0';   
}

/*
 * Returns time (in seconds) from time string (ending with s/m/h/d) *
 */
int time_str(char *s)
{
    int amount;
                                                                                
    amount = strtol(s, (char**) &s, 10);
    if (*s) {
        switch (*s) {
            case 's': return amount;
            case 'm': return amount*60;
            case 'h': return amount*3600;
            case 'd': return amount*86400;
            case 'M': return amount*30*86400;
            case 'Y' : return amount*365*86400;
            default : return -1;
        }
    } else {
        return amount*3600;
    }
}

/*
 * Returns time (in seconds) from time string (ending with s/m/h/d) *
 */
int ftime_str(char *s)
{
    int amount;
                                                                                
    amount = strtol(s, (char**) &s, 10);
    if (*s) {
        switch (*s) {
            case 's': return amount;
            case 'm': return amount*60;
            case 'h': return amount*3600;
            case 'd': return amount*86400;
            case 'M': return amount*30*86400;
            case 'Y' : return amount*365*86400;
            default : return -1;
        }
    }
  return -1;
}

/*
 * Returns a string representing a date *
 */
const char *str_time(int t)
{
  static char tmpbuf[64];
  if(t > 86400)
    snprintf (tmpbuf, 64, "%dd", t / 86400);
  else if(t > 3600)
    snprintf (tmpbuf, 64, "%dh", t / 3600);
  else if (t > 59)
    snprintf (tmpbuf, 64, "%dm", t / 60);
  else
    snprintf (tmpbuf, 64, "%ds", t);
  return tmpbuf;
}
/* 
 * Stripes \R and \N from string
 */
void strip_rn(char *txt)
{
  char *c;
  while((c = strchr(txt, '\r'))) *c = '\0';
  while((c = strchr(txt, '\n'))) *c = '\0';
}

/* returns a string converted to irc lowercase 
  (according to the irc conversion table) */
char* irc_lower(char *str)
{ 

  char *c;
  static char buf[128];  
  int i = 0;
  
  if(str == NULL)
    return NULL;
    
  c = str;
  while(*c)
    {
      buf[i++] = ToLower(*c);
      ++c;
    }
  buf[i] = '\0';
  return buf;
}

/*
 * get password (for the dbinit) *
 */
int get_pass(char *dest, size_t maxlen)
{
    unsigned int i;
    char c;
    int old_status, ttyfd;
    struct termios orig, new;
    
    ttyfd = fileno(stdin);
    
    old_status = tcgetattr(ttyfd, &orig);
    new = orig;
    
    new.c_lflag &= ~ECHO; /* disable echo */
    new.c_lflag &= ~ISIG; /* ignore signals */
    
    tcsetattr(ttyfd, TCSAFLUSH, &new);
    
    /* get the input */
    i = 0;
    
    while(i < (maxlen -1))
    {
	if(read(ttyfd, &c, 1) <= 0)
	    break;
	if(c == '\n')
	    break;
	dest[i] = c;
	i++;
    }
    dest[i] = '\0';
    
    tcsetattr(ttyfd, TCSAFLUSH, &orig);
    
    return i;
}

/* returns the value of a given mask option */
u_int32_t mask_value(OptionMask* opt, char *name)
{
  OptionMask* op;
  
  op = opt;
  
  while(op->name)
    {
      if(strcasecmp(op->name, name) == 0)
        return op->value;
      ++op;
    }
   return 0;
}

/* returns a string listing the value used inside a mask */
char *mask_string(OptionMask* opt, u_int32_t mask)
{
  static char buf[128];
  OptionMask* op;
  
  op = opt;
  
  buf[0] = '\0';
  while(op->name)
    {
      if(mask & op->value)
        {
          if(buf[0] != '\0')
            strcat(buf, ",");
          strncat(buf, op->name, 128);
        }
      ++op;
    }
   
   return buf; 
}

char* validate_options(char* data, OptionMask* opt, u_int32_t *mask)
{
  char *ac;
  u_int32_t m;
  u_int32_t i;

  m = 0;
  ac = strtok(data, ",");
  while(ac && ac[0]!='\0')
    {
      i = mask_value(opt, ac);
      if(i == 0)
        return ac; /* had an error on this */
      m |= i;
      ac = strtok(NULL, ",");
    }
  *mask = m;
  return NULL;
}

/* check if a reason is prefixed with a duration (+time) 
  return the time and adjust the reason pointer */
int strip_reason(char **reason)
{  
  int t = 0;
  char *r = *reason;
  
  if(r[0] == '+') /* we have a time */
  {
    ++r;
    t = time_str(r);
    while(*r && (*r != ' '))
      ++r;
    if(*r)
      ++r;
    *reason = r;
    return t;
  }  
  return t;
}

int is_posint(char *s)
{
  u_int32_t v = strtol(s, (char**) &s, 10);
  if(v < 0)
    return 1;
  return ((s == NULL) || (*s == '\0'));
}

/**
   returns a string from a int value
*/
char* itoa(u_int32_t value)
{
  static char bufs[20][16]; /* we can keep 20 strings */
  static int stri = 0; /* current string index */
  char* cbuf; /* current buffer */
   
  cbuf = bufs[stri++];
  
  if(stri>19) /* reached end of array */
    stri = 0;
  
  snprintf(cbuf, 16, "%d", value);  
  return cbuf;
}


