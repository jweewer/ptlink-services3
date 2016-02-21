#include "stdinc.h"
#include "raw.h"

IrcRaw* RawList = NULL;
int 
irc_AddRawHandler(char *rawmsg, void   (* func)(int parc, char *argv[]))
{  
  IrcRaw* nraw = malloc(sizeof(IrcRaw));
  nraw->func = func;
  nraw->cmd = rawmsg;
  nraw->next = RawList;
  RawList = nraw;
  return 1;
}

/* Process all raw handlers for command cm */
void DoRawHandlers(char *cm, int argc, char * argv[])
{
  IrcRaw *craw = RawList;
  while(craw)
  {
    if(strcasecmp(craw->cmd,cm)==0)
    	(void) craw->func(argc, argv);
    craw=craw->next;
  }
}

