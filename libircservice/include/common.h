/* common externs */
#include <stdio.h>
extern int ircs_debug;
extern FILE* ircslogf;
extern int ircd_fd;
extern int dkmask;
extern char myservername[HOSTLEN];
extern char myserverinfo[HOSTLEN];
void add_me(void);
extern int chan_modes_from_c_to_bitmask[];
#define FREE(x)               if((x) != NULL) free((x)); x = NULL

typedef struct
{
  int flag;
  char letter;
} FLAG_ITEM;

