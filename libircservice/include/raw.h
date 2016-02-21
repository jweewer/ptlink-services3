#include "ircservice.h"

typedef struct IrcRaw_s IrcRaw;
struct IrcRaw_s
{
  char *cmd;
  RawHandler func;
  struct IrcRaw_s* next;
} ;

void DoRawHandlers(char *cm, int argc, char * arg[]);
