/*****************************************************************
 * libircservice is (C) CopyRight PTlink IRC Software 1999-2004  *
 *                    http://software.pt-link.net                *
 * This program is distributed under GNU Public License          *
 * Please read the file COPYING for copyright information.       *
 *****************************************************************

  Description: init/connect routines

 *  $Id: listchans.c,v 1.2 2005/09/08 18:09:55 jpinto Exp $
*/

#include "setup.h"
#include "ircservice.h"

int main(void)
{
  int cr;
  int count = 0;
  IRC_Chan* chan;
  irc_Init(IRCDTYPE,SERVERNAME,"Sample IRC Service", stderr);
  cr = irc_FullConnect(CONNECTO,6667,CONNECTPASS, 0);
  if(cr<0)
    {
      printf("Error connecting to irc server: %s\n", irc_GetLastMsg());
      return 1;
     }
  else
    printf("--- Connected ----\n");
  chan = irc_NextChan(1);
  while(chan)
    {
      ++count;
      printf("%s :%s\n",
        chan->name, chan->last_topic ? chan->last_topic : "");
      chan = irc_NextChan(0);
    }
  printf("Listed %i chans\n", count);
  return 0;
}
