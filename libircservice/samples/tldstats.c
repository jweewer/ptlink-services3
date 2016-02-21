/*****************************************************************
 * libircservice is (C) CopyRight PTlink IRC Software 1999-2004  *
 *                    http://software.pt-link.net                *
 * This program is distributed under GNU Public License          *
 * Please read the file COPYING for copyright information.       *
 *****************************************************************

  Description: init connect routines

 *  $Id: tldstats.c,v 1.2 2005/09/08 18:09:55 jpinto Exp $
*/

#include "setup.h"
#include "ircservice.h"

char* tldlist[1024];
int tldhits[1024];
int tldcount = 0;

void stats_count(char *host);
void stats_count(char *host)
{
 int i = 0;
 char* c = host+strlen(host)-1;
 
 if(isdigit(*host)) /* dont count ips */
   c="unresolved";
 else      
   while(*c!='.' && c>host) /* look for tld */
     --c;

 /* first look if it is on some list */
 while(tldlist[i] && strcasecmp(c,tldlist[i]))
   ++i;   
 if(tldlist[i])
   tldhits[i]++; /* increase hit count */
 else
   {
     tldlist[i] = strdup(c);
     tldhits[i] = 1;
   }
}

void show_stats(int total);
void show_stats(int total)
{
  int i=0, i2=0;
  int max, maxi;
  int tmp;
  char *stmp;
  
  /* sort the list first - selection sort */
  while(tldlist[i])
    {
      i2=i;maxi=i;
      max=tldhits[i];
      while(tldlist[i2])
        {
          if(tldhits[i2]>max)
            {
              max = tldhits[i2];
              maxi = i2;
            }
	  ++i2;
        }
      tmp=tldhits[i];
      tldhits[i]=max;
      tldhits[maxi]=tmp;
      stmp=tldlist[i];
      tldlist[i]=tldlist[maxi];
      tldlist[maxi]=stmp;
      ++i;
    }
  
  i=0;  
  while(tldlist[i])
    {
      printf("%s (%i) [%2.1f%%]\n",
      	tldlist[i], tldhits[i], ((float)tldhits[i]/total)*100);
      ++i;
    }
}

int main(void)
{
  int cr;
  int total = 0;
  IRC_UserList gl;
  IRC_User* new;
  irc_Init(IRCDTYPE,SERVERNAME,"Sample IRC Service", stderr);
  cr = irc_FullConnect(CONNECTO,6667,CONNECTPASS, 0);
  if(cr<0)
    {
      printf("Error connecting to irc server: %s\n", irc_GetLastMsg());
      return 1;
     }
  else
    printf("--- Connected ----\n");
  new = irc_GetGlobalList(&gl);
  while(new)
    {    
      stats_count(new->publichost);
      new = irc_GetNextUser(&gl);
      ++total;
    }
  show_stats(total);
  return 0;
}
