/*****************************************************************
 * libircservice is (C) CopyRight PTlink IRC Software 1999-2004  *
 *                    http://software.pt-link.net                *
 * This program is distributed under GNU Public License          *
 * Please read the file COPYING for copyright information.       *
 *****************************************************************

  Description: init connect routines

 *  $Id: infoserv.c,v 1.2 2005/09/08 18:09:55 jpinto Exp $
*/

#include "setup.h"
#include "ircservice.h"

#define SERVICENAME	"InfoServ"

/* internal functions declaration */
void do_hello(IRC_User* s, IRC_User* t);
void do_help(IRC_User* s, IRC_User* t);
void do_unknown(IRC_User* s, IRC_User* t);

IRC_User* info_user; 	/* info user */

void do_hello(IRC_User* s, IRC_User* t)
{
  irc_SendNotice(t, s, "Hello %s", t->nick);
}

void do_help(IRC_User* s, IRC_User* t)
{
  irc_SendNotice(t, s, "**** Available commands ****");
  irc_SendNotice(t, s, "  HELP - List commands");
  irc_SendNotice(t, s, "  HELLO - Hello command");
  irc_SendNotice(t, s, "****************************");  
}

void do_unknown(IRC_User* s, IRC_User* t)
{
  irc_SendNotice(t, s, "The command %s is not implemented", 
    irc_GetLastMsgCmd());
  irc_SendNotice(t, s, "Try /%s HELP", SERVICENAME);
}
  

/* main program */
int main(void)
{
  int cr;
  
  /* set server service info */
  irc_Init(IRCDTYPE, SERVERNAME, "Sample IRC Service", stderr);
  
  printf("Connecting to "CONNECTO"\n");
  cr = irc_FullConnect(CONNECTO,6667,CONNECTPASS, 0);
  if(cr<0)
    {
      printf("Error connecting to irc server: %s\n", irc_GetLastMsg());
      return 1;
     }
  else
    printf("--- Connected ----\n");
  
  /* Create the sample service user */
  info_user = irc_CreateLocalUser(SERVICENAME,"Services","PTlink.net","PTlink.net",
  	"Sample IRC Service","+r");
  	
  /* Introduce the user */
  irc_IntroduceUser(info_user);
  
  /* Set the new user event */
  irc_AddUMsgEvent(info_user, "help", do_help);
  irc_AddUMsgEvent(info_user, "hello", do_hello);
  irc_AddUMsgEvent(info_user, "*", do_unknown);
  
  /* Loop while connected */
  irc_LoopWhileConnected();
  printf("Connection terminated: %s\n", irc_GetLastMsg());
  return 0;
}
