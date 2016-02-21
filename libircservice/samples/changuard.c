/*****************************************************************
 * libircservice is (C) CopyRight PTlink IRC Software 1999-2004  *
 *                    http://software.pt-link.net                *
 * This program is distributed under GNU Public License          *
 * Please read the file COPYING for copyright information.       *
 *****************************************************************

  Description: sample service that will join every channel when its created
        and leave it when it gets empty

 *  $Id: changuard.c,v 1.2 2005/09/08 18:09:55 jpinto Exp $
*/

#include "setup.h"
#include "ircservice.h"

#define SERVICENAME	"ChanGuard"

/* internal functions declaration */
void ev_chan_join(IRC_Chan* chan, IRC_ChanNode* cn);
void ev_chan_part(IRC_Chan* chan, IRC_ChanNode* cn);

IRC_User* changuard_user; 	/* info user */

/* this is called after join */
void ev_chan_join(IRC_Chan* chan, IRC_ChanNode* cn)
{
  int remote_users = chan->users_count - chan->lusers_count;
  
  if(remote_users == 1 )  /* first user joining the channel */
    {
      irc_ChanJoin(changuard_user, chan->name, CU_MODE_OP);
    }
}

/* this is called before the part */
void ev_chan_part(IRC_Chan* chan, IRC_ChanNode* cn)
{
  int remote_users = chan->users_count - chan->lusers_count;
  
  /* we must count 1 for our user */
  if(remote_users == 1)
    irc_ChanPart(changuard_user, chan);
}


/* main program */
int main(void)
{
  int cr;
  
  /* set server service info */
  irc_Init(IRCDTYPE, SERVERNAME, "Sample IRC Service", stderr);

  /* Create the service user */
  changuard_user = irc_CreateLocalUser(SERVICENAME,"Services","PTlink.net","PTlink.net",
  	"Sample IRC Service","+r");

  /* Add user events */
  irc_AddEvent(ET_CHAN_JOIN, ev_chan_join);
  irc_AddEvent(ET_CHAN_PART, ev_chan_part);
  
  printf("Connecting to "CONNECTO"\n");
  cr = irc_FullConnect(CONNECTO,6667, CONNECTPASS, 0);
  if(cr<0)
    {
      printf("Error connecting to irc server: %s\n", irc_GetLastMsg());
      return 1;
     }
  else
    printf("--- Connected ----\n");
    

  
  /* Loop while connected */
  irc_LoopWhileConnected();
  printf("Connection terminated: %s\n", irc_GetLastMsg());
  return 0;
}
