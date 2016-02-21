#include "stdinc.h"
#include "ircservice.h"
#include "event.h"
#include "umode.h"

/* user mode handler list [+/-][umode char] */
static dlnode* umode_elist[2][256];

static int user_modes_from_c_to_bitmask[] =
{ 
  /* 0x00 */ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0x0F */
  /* 0x10 */ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0x1F */
  /* 0x20 */ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0x2F */
  /* 0x30 */ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0x3F */
  0,            /* @ */
  UMODE_ADMIN,  /* A */
  UMODE_BOT,    /* B */
  0,            /* C */
  0,            /* D */
  0,            /* E */
  0,            /* F */
  0,            /* G */
  UMODE_HIDEOPER,/* H */
  0,            /* I */
  0,            /* J */
  0,            /* K */
  0,            /* L */
  0,            /* M */
  UMODE_NETADMIN,/* N */
  UMODE_LOCOP,  /* O */
  0,            /* P */
  0,            /* Q */
  UMODE_REGMSG, /* R */
  UMODE_STEALTH,/* S */
  UMODE_TECHADMIN,/* T */
  0,            /* U */
  0,            /* V */
  0,            /* W */
  0,            /* X */
  0,            /* Y */
  0,            /* Z 0x5A */
  0, 0, 0, 0, 0, /* 0x5F */ 
  /* 0x60 */       0,
  UMODE_SADMIN, /* a */
  0,		/* b */
  0,  		/* c */
  0,  		/* d */
  0,		/* e */
  UMODE_FLOODEX,/* f */
  0,            /* g */
  UMODE_HELPER, /* h */
  UMODE_INVISIBLE, /* i */
  0,    	/* j */
  0,  		/* k */
  0,            /* l */
  0,            /* m */
  0, 		/* n */
  UMODE_OPER,   /* o */
  UMODE_PRIVATE,/* p */
  0,            /* q */
  UMODE_IDENTIFIED, /* r */
  UMODE_SSL, 	/* s */
  0,            /* t */
  0,            /* u */
  UMODE_NODCC,  /* v */
  UMODE_WALLOP, /* w */
  0				, /* x */
  UMODE_SPY,    /* y */
  UMODE_ZOMBIE, /* z 0x7A */
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

/*
 In:
 mchange - mode change +/-umode
 efunc - function to be executed
 */
void irc_AddUmodeChange(char *mchange, void* efunc)
{
  dlnode* nnode;
  IRC_Event* ev;
  int hashi;
  int hashc;
  hashi = (mchange[0]=='+') ? 1 : 0;
  hashc = mchange[1];
  nnode = malloc(sizeof(dlnode));
  ev = malloc(sizeof(IRC_Event));
  ev->func=(EventHandler) efunc;
  ev->hits=0;
  nnode->value = ev;
  nnode->next=umode_elist[hashi][hashc];
  umode_elist[hashi][hashc]=nnode;
}

/*
 In:
 mchange - mode change string
 u - user 
 */
void umode_change(char *mchange, IRC_User* u)
{
  char *c=mchange;
  int hashi;
  int flag;
  
  hashi = 1;
  while(*c)
    {
      if(*c=='+')
        hashi = 1 ;
      else if(*c=='-')
        hashi = 0;
      else /* do list of functions for the umode char */
        {
          dlnode* nnode = umode_elist[hashi][(int)*c];
          IRC_Event* ev;
  	  while(nnode)
   	    {
              ev=nnode->value;
              (ev->hits)++;
              ev->func(u, c);
              nnode=nnode->next;
       	    }

	  flag = user_modes_from_c_to_bitmask[(unsigned char)*c];
       	  if(hashi)
       	    u->umodes |= flag;
       	  else
       	    u->umodes  &= ~flag;
        }
      ++c; /* look next char */
    }
}

/*
 In:
 mchange - mode change string
 u - user 
 */
void umode_update(char *mchange, IRC_User* u)
{
  char *c=mchange;
  int hashi;
  int flag;
  
  hashi = 1;
  while(*c)
    {
      if(*c=='+')
        hashi = 1 ;
      else if(*c=='-')
        hashi = 0;
      else /* do list of functions for the umode char */
        {
	  flag = user_modes_from_c_to_bitmask[(unsigned char)*c];
       	  if(hashi)
       	    u->umodes |= flag;
       	  else
       	    u->umodes  &= ~flag;
        }
      ++c; /* look next char */
    }
}

/* 
 * returns the strings with the umodes from a umodes bitmap 
 *
 * This needs to be optimized in case we call it frequently
 * -
*/
char *umode_string(u_int32_t umodes)
{
  static char umode_str[64];
  int c = 0;
  int i;
  for(i = 0; i < 255; ++i)
    {
      if(umodes & user_modes_from_c_to_bitmask[i])
        umode_str[c++] = i;
    };
  umode_str[c] = 0;
  return umode_str;
}
