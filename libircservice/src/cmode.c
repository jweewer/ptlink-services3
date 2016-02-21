#include "stdinc.h"
#include "ircservice.h"
#include "common.h"
#include "send.h"
#include "event.h"
#include "channel.h"
#include "irc_string.h" /* we need irccmp() */

#define BUFSIZE 	512

IRC_User* irc_ChanMlocker;

/* user mode handler list [+/-][cmode char] */
static dlnode* cmode_elist[2][256];

/* internal functions */
void channel_mode(int parc, char *parv[], int check_mlock);
void add_ban(IRC_Chan* chan, char* ban);
void del_ban(IRC_Chan* chan, char* ban);
void irc_ChanMLockApply(IRC_User *from, IRC_Chan *chan);
void cmodes_build(void);

static int is_numeric(char *str);

int chan_modes_from_c_to_bitmask[] =
{ 
  /* 0x00 */ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0x0F */
  /* 0x10 */ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0x1F */
  /* 0x20 */ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0x2F */
  /* 0x30 */ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0x3F */
  0,            /* @ */
  CMODE_A,	/* A */
  CMODE_B,    	/* B */
  CMODE_C,            /* C */
  0,            /* D */
  0,            /* E */
  0,            /* F */
  0,            /* G */
  0,            /* H */
  0,            /* I */
  0,            /* J */
  CMODE_K,      /* K */
  0,            /* L */
  0,            /* M */
  CMODE_N,	/* N */
  CMODE_O,	/* O */
  0,            /* P */
  0,            /* Q */
  CMODE_R,	/* R */
  CMODE_S,	/* S */
  0,		/* T */
  0,            /* U */
  0,            /* V */
  0,            /* W */
  0,            /* X */
  0,            /* Y */
  0,            /* Z 0x5A */
  0, 0, 0, 0, 0, /* 0x5F */ 
  /* 0x60 */       0,
  0, 		/* a */
  0,		/* b */
  CMODE_c,	/* c */
  CMODE_d,	/* d */
  0,		/* e */
  CMODE_f,	/* f */
  0,            /* g */
  0, 		/* h */
  CMODE_i, 	/* i */
  0,    	/* j */
  CMODE_k,	/* k */
  CMODE_l,      /* l */
  CMODE_m,      /* m */
  CMODE_n,	/* n */
  0,   		/* o */
  CMODE_p,	/* p */
  CMODE_q,      /* q */
  CMODE_r,	/* r */
  CMODE_s,	/* s */
  CMODE_t,      /* t */
  0,            /* u */
  0,  		/* v */
  0, 		/* w */
  0, 		/* x */
  0,    	/* y */
  0, 		/* z 0x7A */
  0,0,0,0,0,     /* 0x7B - 0x7F */

  /* 0x80 */ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0x9F */
  /* 0x90 */ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0x9F */
  /* 0xA0 */ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0xAF */
  /* 0xB0 */ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0xBF */
  /* 0xC0 */ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0xCF */
  /* 0xD0 */ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0xDF */
  /* 0xE0 */ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0xEF */
  /* 0xF0 */ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0  /* 0xFF */
};

static FLAG_ITEM chan_modes[255];

/*
 In:
 mchange - mode change +/-cmode
 efunc - function to be executed
 */
void irc_AddCmodeChange(char *mchange, void* efunc)
{
  dlnode* nnode;
  IRC_Event* ev;
  int hashi;
  int hashc;
  hashi = (mchange[0]=='+') ? 1 : 0;
  hashc = mchange[1];
  nnode = malloc(sizeof(dlnode));
  ev = malloc(sizeof(IRC_Event));
  ev->func = (EventHandler) efunc;
  ev->hits = 0;
  nnode->value = ev;
  nnode->next=cmode_elist[hashi][hashc];
  cmode_elist[hashi][hashc]=nnode;
}

/* process a channel MODE command */
void channel_mode(int parc, char *parv[], int check_mlock)
{
  static int loop_count = 0;
  char *cname;
  IRC_Chan *chan;
  IRC_ChanNode *cn;
  IRC_User *u;
  char *m;
  int add_mode;
  int i;
  dlnode* nnode;
  IRC_Event* ev;
  int mchange;

  if(loop_count++ > 10) /* bounce loop() ? */
    abort();
  cn = NULL;
  add_mode = 1; /* default should be mode is beeing added */
  cname = parv[1];
  chan = irc_FindChan(cname);
  if(chan==NULL)
    {
      --loop_count;
      fprintf(ircslogf,"MODE for non existent channel %s\n", cname);    
      return;
    }
  m = parv[2];
  i = 2;
  while(*m)
  {       
    switch(*m) 
    {
      case '+': add_mode = 1; break;
      case '-': add_mode = 0; break;
      case 'k': 
        if(add_mode)
        {
          if(++i>=parc)
            {
              --loop_count;
              fprintf(ircslogf,"Not enough parameters on MODE %s +%c",
                cname, *m);
              return;
            }
          FREE(chan->key);
          chan->key= strdup(parv[i]);
        }
        else 
          {
            FREE(chan->key);
          }
      break;
      case 'l':
        if(add_mode)
        {
          if(++i>=parc)
          {
            --loop_count;
            fprintf(ircslogf,"Not enough parameters on MODE %s +%c",
              cname, *m);
            return;
          }
          chan->limit = atoi(parv[i]);
        }
        else 
          chan->limit = 0;
      break;
      case 'f':
        if(add_mode)
        {
          char *c;
          if(++i>=parc)
          {
            --loop_count;
            fprintf(ircslogf,"Not enough parameters on MODE %s +%c\n",
              cname, *m);
            return;
          }
        c = strchr(parv[i], ':');
        if(c)
        {
          *c = '\0';
          chan->max_msgs = atoi(parv[i]);
          chan->max_msgs_time =  atoi(c+1);                  
        }
      }
      else 
        { 
          chan->max_msgs = 0; 
          chan->max_msgs_time = 0; 
        }
          break;
          case 'b':
            if(++i>=parc)
              {
                --loop_count;
                fprintf(ircslogf,"Not enough parameters on MODE %s +%c\n",
                  cname, *m);
                return;
              }
            if(add_mode)
              add_ban(chan, parv[i]);
            else
              del_ban(chan, parv[i]);
          break;
          case 'a':
          case 'o':
          case 'v':  
            if(++i >= parc)
                  {
                    --loop_count;
                    fprintf(ircslogf,"Not enough parameters on MODE %s %c\n",
                      cname, *m);
                    return;
                  }
            u = irc_FindUser(parv[i]);                  
            if(u == NULL)
              {
                fprintf(ircslogf,"MODE %s %c%c for non existent user %s\n", 
                  cname, add_mode ? '+' : '-', *m, parv[i]);
                ++m;
                continue;

              }
              
       /* call events before internal processing of the modes */
         nnode = cmode_elist[add_mode][(int)*m];
         while(nnode)
           {
             ev=nnode->value;
             (ev->hits)++;
             ev->func(chan, u);
             nnode=nnode->next;
           }; 
                        
            cn = chan->userlist;
            while(cn && cn->user != u) /* lets look for the correct node */
              cn = cn->next; 
              
            if(cn==NULL) /* we got a mode but user was not found on chan */
              {
                fprintf(ircslogf,"MODE %s %c%c for user %s not on channel\n", 
                  cname, add_mode ? '+' : '-', *m, parv[i]);
                  ++m;
                continue;              
              }
            /* IMPORTANT NOTE: 
               We are not updating the corresponding usernode cumodes !!! */
            if(*m == 'a')
              mchange = CU_MODE_ADMIN;
            else if(*m == 'o')
              mchange = CU_MODE_OP;
            else if(*m == 'v')
              mchange = CU_MODE_VOICE;
            else
              mchange = 0;
              
            if(add_mode)
              cn->cumodes |= mchange;
            else
              cn->cumodes &= ~mchange;
              
            break;
          default:
            mchange = chan_modes_from_c_to_bitmask[(unsigned char)*m];
            if(mchange)
              {
                if(add_mode)
                {
                  chan->cmodes |= mchange;
                }
                else
                {                
                  chan->cmodes &= ~mchange;
                }
              }            
            break;
        }
      ++m;
    }
  
  if(check_mlock)    
    irc_ChanMLockApply(chan->local_user ? chan->local_user : irc_ChanMlocker, 
      chan);
  --loop_count;
}

void irc_ChanMode(IRC_User* from, IRC_Chan* chan, char *fmt, ...)
{
  char buf[BUFSIZE];
  va_list args;
  char* para[16];  
  char* s;
  int i;
  int paramcount = 16;
  
  va_start(args, fmt); 
  vsnprintf(buf, sizeof(buf), fmt, args);
  va_end(args);
  if(irc_ConnectionStatus()>0)
    sendto_ircd(from->nick, "MODE %s %s", chan->name, buf);      
  para[0] = from->nick;
  para[1] = chan->name;  
  i = 2;
  s = buf;

  /* parse code here is from irc_parse.c */
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
            break;
          /* scan for end of string, either ' ' or '\0' */
          while (IsNonEOS(*s))
            s++;
        }
    }

  para[i] = NULL;

  channel_mode(i, para, 0);
}

void irc_ChanUMode(IRC_User* from, IRC_Chan* chan, char *modestr, IRC_User* to)
{
  char* parv[4];
  sendto_ircd(from->nick, "MODE %s %s %s", chan->name, modestr, to->nick);      
  parv[0] = from->nick;
  parv[1] = chan->name;
  parv[2] = modestr;
  parv[3] = to->nick;
  channel_mode(4, parv, 0);
}

void add_ban(IRC_Chan* chan, char* ban)
{
  IRC_BanList* bl;
  bl = malloc(sizeof(IRC_BanList));
  bl->next = chan->bans;
  bl->value = strdup(ban);
  chan->bans = bl;  
}

void del_ban(IRC_Chan* chan, char* ban)
{
  IRC_BanList* bl = chan->bans;
  IRC_BanList* last_bl = NULL;

  while(bl)
    {      
      if(irccmp(bl->value, ban) == 0)
        {
          if(last_bl) /* we have a previous ban to change */
            last_bl->next = bl->next;
          else /* this is the first ban on the list */
            chan->bans = bl->next;
          
          free(bl->value);
          free(bl);
          return;
        }        
      last_bl = bl;
      bl = bl->next;
    }
}

/* unban user from a channel 
  returns:
    number of bans found
*/
int irc_ChanUnban(IRC_Chan* chan, IRC_User* target, IRC_User* source)
{
  IRC_BanList* bl = chan->bans;
  IRC_BanList* last_bl = NULL;
  int match_count = 0;
  
  
  while(bl)
    {
      char *tmp; 
      char *old_tmp;
      char *sep; /* separator */
      
      tmp = strdup(bl->value);
      old_tmp = tmp;      
      sep = strchr(tmp, '!');
      if(sep)
        {
          *sep = '\0';
          if(match(tmp, target->nick) == 0)
            {
              free(old_tmp);
              last_bl = bl;
              bl = bl->next;
              continue;
            }
          tmp = sep+1;
        }
      sep = strchr(tmp, '@');
      if(*tmp && sep)
        {
          *sep = '\0';
          if(match(tmp, target->username) == 0)
            {
              free(old_tmp);
              last_bl = bl;
              bl = bl->next;
              continue;
            }
          tmp = sep+1;
        }
        
      if((*tmp && match(tmp, target->realhost) == 0) 
          && (match(tmp, target->publichost) == 0))
        {
          free(old_tmp);
          last_bl = bl;
          bl = bl->next;
          continue;
        }
        
      /* send the unban */
      sendto_ircd(source->nick, "MODE %s -b %s", chan->name, bl->value);
      /* count it */
      ++match_count;
      free(old_tmp);
      
      /* and remove it from the ban list */
      if(last_bl) /* we have a previous ban to change */      
        last_bl->next = bl->next;
      else /* this is the first ban on the list */
        chan->bans = bl->next;
           
      free(bl->value);
      free(bl);    
      if(last_bl)
        bl = last_bl->next;
      else 
        bl = chan->bans;
    }
    
  return match_count;  
}

/* builds the chan_modes array */
void cmodes_build(void)
{
  int c = 0;
  int i;
  for(i = 0; i < 255; ++i)
  {
    if(chan_modes_from_c_to_bitmask[i])
    {
      chan_modes[c].flag = chan_modes_from_c_to_bitmask[i];
      chan_modes[c].letter = i;
      c++;
    }
  } 
  chan_modes[c].flag = 0;
  chan_modes[c].letter = 0;
}

/*
 * returns the strings with the cmodes for a given channel
 *
 * This needs to be optimized in case we call it frequently
 * -
*/
char *cmode_string(IRC_Chan *chan)
{
  static char cmode_str[64];
  int c = 0;
  int i;
  for(i = 0; i < 255; ++i)
    {
      if(chan->cmodes & chan_modes_from_c_to_bitmask[i])
        cmode_str[c++] = i;
    };
  cmode_str[c] = 0;
  return cmode_str;
}

/* validates and sets mlock on channel
  Please note that this function doesn't enforce the mlock
  You will need to use irc_ChanMLockApply()
  returns:
    0	ok
    -1 	there was an invalid mode on the string
    -2  missing parameter
    -3  invalid parameter
    -4	extra parameter
    -5	conflict modes
  */
int irc_ChanMLockSet(IRC_User* from, IRC_Chan* chan, char *mlock)
{
  int parc;
  char* para[16];  
  int paramcount = 16;
  char *m;
  char *is = NULL;
  char *s = NULL;  
  int i = 0;
  int add_mode = 1;
  u_int32_t mchange;
  u_int32_t mlock_on = 0;
  u_int32_t mlock_off = 0;
  char mlock_key[64];
  int mlock_limit = 0;
  int mlock_max_msgs = 0;
  int mlock_max_msgs_time = 0;
  
  if(strlen(mlock) > BUFSIZE)
    mlock[BUFSIZE] = '\0';
  
  mlock_key[0] = '\0';

  para[0] = from->nick;
  if(chan)
    para[1] = chan->name;
  else
    para[1] =  NULL;

  i = 2;
  if(mlock)
    {
      is = strdup(mlock);
      s = is;
    }
  /* parse code here is from irc_parse.c */
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
            break;
          /* scan for end of string, either ' ' or '\0' */
          while (IsNonEOS(*s))
            s++;
        }
    }
    
  parc = i;
  para[i] = NULL;
  
  if(para[2])
    m = para[2];  
  else 
    m = "";
  i = 2;  
  while(*m)
  {       
    switch(*m) 
    {
      case '+': add_mode = 1;break;
      case '-': add_mode = 0;break;
      case 'k': 
        if(add_mode)
        {
          if(++i >= parc)
          {
            FREE(is);
            return -2;
          }
          if(strchr(para[i], ':'))
          {
            FREE(is);
            return -3;
          }
          strncpy(mlock_key, para[i], sizeof(mlock_key));
        }
      break;
      case 'l':
        if(add_mode)
        {
          if(++i >= parc)
          {
            FREE(is);
            return -2;
          }
          mlock_limit = atoi(para[i]);          
          if(!is_numeric(para[i]) || (mlock_limit == 0))
          {
            FREE(is);
            return -3;
          }
        }
      break;
      case 'f':
        if(add_mode)
        {
          char *c;
          if(++i >= parc)
          {
            FREE(is);
            return -2;
          }
          c = strchr(para[i], ':');
          if(c)
          {
            *c = '\0';
            mlock_max_msgs = atoi(para[i]);
            mlock_max_msgs_time =  atoi(c+1);                  
            if(!is_numeric(para[i]) || !is_numeric(c+1))
            {
              FREE(is);
              return -3;            
            }
          }
          if((mlock_max_msgs<1) || (mlock_max_msgs_time <1) ||
             (mlock_max_msgs>99) || (mlock_max_msgs_time>99))
          {
            FREE(is);
            return -3;
          }
        }
      break;
      case 'b':
      case 'a':
      case 'o':
      case 'v':
      case 'A':
      case 'O':
      case 'r':
        FREE(is);
        return -1;
      break;
    }
    if(*m != '+' && *m != '-')   
    {
      mchange = chan_modes_from_c_to_bitmask[(unsigned char)*m];
      if(mchange)
      {
        if(add_mode)
          mlock_on |= mchange;
        else
          mlock_off |= mchange;
      }
      else 
      {
        FREE(is);
        return -1; /* invalid char */
      }    
    }
    ++m;
  }
  FREE(is);
  
  if(parc-1>i) /* we don't allow extra parameters to avoid errors*/
    return -4;
    
  
  if(mlock_off & mlock_on) /* we have an double on/off mlock*/
    return -5;
  
  if(chan)
  {
    chan->mlock_on = mlock_on;
    chan->mlock_off = mlock_off;
    FREE(chan->mlock_key);
    if(mlock_key[0])
      chan->mlock_key = strdup(mlock_key);
    chan->mlock_limit = mlock_limit;
    chan->mlock_max_msgs = mlock_max_msgs;
    chan->mlock_max_msgs_time = mlock_max_msgs_time;    
    /* irc_ChanMLockApply(from, chan); */
  }
  return 0;
}

/* Sends the required modes changes to enforce mlock on the channel */
void irc_ChanMLockApply(IRC_User *from, IRC_Chan *chan)
{
  char mlock_on[256];
  int mlock_on_i = 1;
  char mlock_off[256];
  int mlock_off_i = 1;
  char mlock_par[BUFSIZE];
  char buf[16];
  char m_on;
  u_int32_t flag;
  char letter;
  int i = 0;
  mlock_on[0] = '+';
  mlock_off[0] = '-';
  mlock_par[0] = '\0';
  
  if(!chan->mlock_on && !chan->mlock_off)
    return;
  
  while(chan_modes[i].letter)
  {
    flag = chan_modes[i].flag;  
    letter = chan_modes[i].letter;
    m_on = '\0';

    switch(letter)
    {
      case 'k':
        if(chan->key && (chan->mlock_off & flag))
          mlock_off[mlock_off_i++] = letter;
        else if(chan->mlock_key && 
          (!chan->key || strcasecmp(chan->key, chan->mlock_key)))
        {
          mlock_on[mlock_on_i++] = letter;
          strncat(mlock_par, chan->mlock_key,
            sizeof(mlock_par)-strlen(mlock_par));
          strncat(mlock_par, " ",
            sizeof(mlock_par)-strlen(mlock_par));          
        }
      break;
      case 'l':
        if(chan->limit && (chan->mlock_off & flag))
          mlock_off[mlock_off_i++] = chan_modes[i].letter;
        else if(chan->mlock_limit && (chan->limit != chan->mlock_limit))
        {
          mlock_on[mlock_on_i++] = letter;
          snprintf(buf, sizeof(buf), "%d ", chan->mlock_limit);
          strncat(mlock_par, buf,
            sizeof(mlock_par)-strlen(mlock_par));
        }
      break;
      case 'f':
        if(chan->max_msgs && (chan->mlock_off & flag))
          mlock_off[mlock_off_i++] = chan_modes[i].letter;
        else if(chan->mlock_max_msgs && 
          ((chan->max_msgs != chan->mlock_max_msgs) ||
           (chan->max_msgs_time != chan->mlock_max_msgs_time)))
        {
          mlock_on[mlock_on_i++] = letter;
          snprintf(buf, sizeof(buf), "%d:%d ",
            chan->mlock_max_msgs,
            chan->mlock_max_msgs_time);
          strncat(mlock_par, buf,
            sizeof(mlock_par)-strlen(mlock_par));
        }
      break;
      default:    
        if((chan->mlock_on & flag) && !(chan->cmodes & flag)) /* mode locked on */
        {
          m_on = chan_modes[i].letter;
          mlock_on[mlock_on_i++] = m_on;
        }
        else
          if((chan->mlock_off & flag) && (chan->cmodes & flag))
            mlock_off[mlock_off_i++] = chan_modes[i].letter;      
        break;
    }
    ++i;
  }  
  mlock_on[mlock_on_i] = '\0';
  mlock_off[mlock_off_i] = '\0';
  if(mlock_on_i>1 || mlock_off_i>1)
    irc_ChanMode(from, chan, "%s%s %s",
      mlock_off_i>1 ? mlock_off: "", 
      mlock_on_i>1 ? mlock_on: "", 
      mlock_par);
}

/* check if a given string represents anumeric value */
static int is_numeric(char *str)
{
  char *c = str;
  while(*c)
  {
    if(!isdigit(*c))
      return 0;
    ++c;
  }
  return 1;
}

void irc_SetChanMlocker(IRC_User *user)
{
  /* only local users can send mlock related commands */
  if((user->iflags & IFL_LOCAL) == 0)
    return;
  irc_ChanMlocker = user;
}
