#include "stdinc.h"
#include "ircservice.h"
#include "event.h"


/* event list */
static dlnode* event_elist[100];
static char* irc_last_cmd;

/* just an internal flag to mark a event as disabled */
static int was_aborted;

/*
  Call all functions associated to an event
  returns: 0 if the event called all handlers
           1 if the event was aborted
 */
int irc_CallEvent(int evt, void* edata1, void *edata2)
{
  dlnode* nnode = event_elist[evt];  
  IRC_Event* ev;
  was_aborted = 0;
  while(nnode && !was_aborted)
  { 
#ifdef HAVE_GETTIMEOFDAY
    struct timeval tv1;
#endif  
    ev=nnode->value;
#ifdef HAVE_GETTIMEOFDAY     
    gettimeofday(&tv1, NULL);
#endif    
    ev->func(edata1, edata2);     
#ifdef HAVE_GETTIMEOFDAY     
/*    addsince(&tv1, &ev->tv); */
    (ev->hits)++;   
#endif
    nnode=nnode->next;
   }
 return was_aborted;
}

/*
   This function should be used from event handlers to abort
   processing of further events on the current event type loop.
   It is usefull if we destroy the data target of the event.
   E.G. If we kill a user during a ET_NEW_USER event
 */
void irc_AbortThisEvent(void)
{
  was_aborted = 1;
}

int irc_AddEvent(int evt, void* func)
{
  dlnode* nnode;
  IRC_Event* ev;
  nnode = malloc(sizeof(dlnode));
  ev = malloc(sizeof(IRC_Event));
  ev->func=(EventHandler) func;
  ev->hits=0;
  nnode->value = ev;
  nnode->next=event_elist[evt];
  event_elist[evt]=nnode;
  return 1;
}

void irc_DelEvent(int evt, void* func)
{
  dlnode *nnode, *lnode;
  IRC_Event* ev = NULL;
  nnode = event_elist[evt];
  lnode = NULL;
  while(nnode && (ev = nnode->value)->func != func)
    {
      lnode = nnode;
      nnode = nnode->next;
    }
  assert(nnode != NULL);	/* We didn't find the function to delete !!! */
  if(lnode)
    lnode->next=nnode->next;
  else
    event_elist[evt] = nnode->next;
  free(ev);
  free(nnode);
}

/* Add a message handler for a local user */
int irc_AddUMsgEvent(IRC_User *u, char* msg, void* func)
{
  IrcMsg *nmsg = malloc(sizeof(IrcMsg)); /* create new msg */
  IrcMsg *pmsg;
  nmsg->msg = strdup(msg);
  nmsg->func = (EventHandler) func; 
  pmsg = u->msglist; 
  if(pmsg && (msg[0]=='*') && (msg[1]=='\0')) /* any msg handlers should go to the end of match*/
   {          
     while(pmsg->next) /* go to the end of the list */
      pmsg = pmsg->next;
     nmsg->next=NULL;      
     pmsg->next=nmsg;
     return 1;
   } 
  nmsg->next = u->msglist;
  u->msglist = nmsg;
  return 1;
}

/* Delete a message handler for a local user */
void irc_DelUMsgEvent(IRC_User *u, char* msg, void* func)
{
  IrcMsg *cmdmsg, *prev, *next;
  cmdmsg = u->msglist;
  prev = NULL;
  while(cmdmsg)
    {
      if((cmdmsg->func == func) && strcasecmp(msg, cmdmsg->msg)==0)
        {
          next = cmdmsg->next;
          if(prev)
            prev->next = next;
          else
            u->msglist = next;
          free(cmdmsg->msg);
          free(cmdmsg);
          return;
        }
      prev = cmdmsg;
      cmdmsg = cmdmsg->next;        
    }
  return;
}

/* check any events for user and messages starting with msg */
void  check_user_msg_events(IRC_User* dest, IRC_User* src, char *msg)
{
  char *firstword;
  IrcMsg *umsg, *dmsg = NULL;
  size_t msgsiz;
  int found = 0;
	
  firstword = strtok(msg, " ");
  umsg = dest->msglist;
  msgsiz = strlen(firstword);
  while(umsg)
    {
      if(umsg->msg[0]=='*') /* check if it is the any msg handler */
        dmsg = umsg;
      else
        {
          msgsiz = strlen(umsg->msg);
          if(strncasecmp(umsg->msg, firstword, msgsiz) == 0)
            {
              irc_last_cmd = umsg->msg;
              (void) umsg->func(dest , src);
              found = 1;    	     
              break;
          }
        }
      umsg=umsg->next;
    }
  if((found == 0) && (dmsg != NULL))
    {
      irc_last_cmd = msg;
      (void) dmsg->func(dest , src);	
    }
}

/* delete all events for the user */
void  delete_user_msg_events(IRC_User* u)
{
	IrcMsg *umsg=  u->msglist, *nmsg;
	while(umsg)
	  {
	    nmsg = umsg->next;
	    free(umsg->msg);
	    free(umsg);
    	    umsg = nmsg;
	  }	  
	 u->msglist = NULL;
}

char *irc_GetLastMsgCmd(void)
{
  return irc_last_cmd;
}
