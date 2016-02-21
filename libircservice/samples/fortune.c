/*****************************************************************
 * libircservice is (C) CopyRight PTlink IRC Software 1999-2004  *
 *                    http://software.pt-link.net                *
 * This program is distributed under GNU Public License          *
 * Please read the file COPYING for copyright information.       *
 *****************************************************************

  Description: init connect routines

 *  $Id: fortune.c,v 1.1.1.1 2005/08/27 15:44:19 jpinto Exp $
*/

#include "setup.h"
#include "ircservice.h"

#define SERVICENAME	"Fortune"
#define FORTUNEFN	"fortune.txt"

/* fortune msgs list */
static char fortune_list[256][256];
static int fcount = 0;
IRC_User* fortune_u; 	/* fortune user */

void read_fortune(void);

/* read strings from fortune.txt */
void read_fortune(void)
{
  FILE *ff;
  ff = fopen(FORTUNEFN, "r");
  if(ff == NULL)
    {
      printf("Coul not open %s\n", FORTUNEFN);
      exit(2);
    }
  while(!feof(ff))
   fgets(fortune_list[fcount++],255,ff);
}

void do_fortune(IRC_User *u);

/* send a random fortune string */
void do_fortune(IRC_User *u)
{
  char *msg;
	
  if(fcount == 0)
    return;
  msg = fortune_list[rand() % fcount];
  irc_SendNotice(u, fortune_u, "%s",msg);
}

/* main program */
int main(void)
{
  int cr;  
  
  read_fortune(); /* read lines from fortune file */  
  /* set server service info */
  irc_Init(IRCDTYPE, SERVERNAME, "Fortune IRC Service", stderr);
  
  printf("Connecting to "SERVERNAME"\n");
  cr = irc_FullConnect(CONNECTO,6667,CONNECTPASS, 0);
  if(cr<0)
    {
      printf("Error connecting to irc server: %s\n", irc_GetLastMsg());
      return 1;
     }
  else
    printf("--- Connected ----\n");
  
  /* Create the fortune service user */
  fortune_u = irc_CreateLocalUser(SERVICENAME,"Services","PTlink.net","PTlink.net",
  	"Fortune IRC Service","+r");
  /* Introduce the user */
  irc_IntroduceUser(fortune_u);
  /* Set the new user event */
  irc_AddEvent(ET_NEW_USER, do_fortune);
  /* Loop while connected */
  irc_LoopWhileConnected();
  printf("Connection terminated: %s\n", irc_GetLastMsg());
  return 0;
}
