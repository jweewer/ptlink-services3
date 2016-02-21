/*****************************************************************
 * libircservice is (C) CopyRight PTlink IRC Software 1999-2004  *
 *                    http://software.pt-link.net                *
 * This program is distributed under GNU Public License          *
 * Please read the file COPYING for copyright information.       *
 *****************************************************************

  Description: init connect routines

 *  $Id: watchdog.c,v 1.2 2005/09/08 18:09:55 jpinto Exp $
*/

#include "ircservice.h"
#include "setup.h"

/* functions declaration */
void do_newserver(IRC_Server* s);
void do_newuser(IRC_User *u);
void do_nickchange(IRC_User *u);
void do_quit(IRC_User *u);
void do_oper(IRC_User *u);
void do_unoper(IRC_User *u);


void do_newserver(IRC_Server* s)
{
  if(s->from)
    printf("New Server %s from %s :%s)\n", 
      s->sname,  s->from->sname, s->info);
  else
    printf("New Server %s :%s\n", s->sname, s->info);
};

void do_newuser(IRC_User *u)
{
  printf("New User: %s!%s@%s[%s]@%s :%s\n",
    u->nick, u->username, u->publichost, u->realhost, 
    u->server->sname, u->info);
}

void do_nickchange(IRC_User *u)
{
  printf("Nick Change: %s to %s\n", u->lastnick, u->nick);
}

void do_quit(IRC_User *u)
{
  printf("Quit: %s\n", u->nick);
}

void do_oper(IRC_User *u)
{
  printf("Operator set on %s\n", u->nick);
}

void do_unoper(IRC_User *u)
{
  printf("Operator removed from %s\n", u->nick);
}

int main(void)
{
  int cr;
  /* irc_SetDebug(1); */
  irc_Init(IRCDTYPE,SERVERNAME,"Sample IRC Service", stderr);
  cr = irc_FullConnect(CONNECTO,6667,CONNECTPASS, 0);
  if(cr<0)
    {
      printf("Error connecting to irc server: %s\n", irc_GetLastMsg());
      return 1;
     }
  else
    printf("--- Connected ----\n");
  /* Add event handlers */
  irc_AddEvent(ET_NEW_SERVER, do_newserver);
  irc_AddEvent(ET_NEW_USER, do_newuser);      
  irc_AddEvent(ET_NICK_CHANGE, do_nickchange);        
  irc_AddEvent(ET_QUIT, do_quit);
  irc_AddUmodeChange("+o", do_oper);
  irc_AddUmodeChange("-o", do_unoper);
  irc_LoopWhileConnected();
  printf("Closed: %s\n",irc_GetLastMsg());
  return 0;
}
