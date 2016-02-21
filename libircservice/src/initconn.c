/*****************************************************************
 * libircservice is (C) CopyRight PTlink IRC Software 1999-2005  *
 *                    http://software.pt-link.net                *
 * This program is distributed under GNU Public License          *
 * Please read the file COPYING for copyright information.       *
 *****************************************************************
  
  Description: init/connect routines

 *  $Id: initconn.c,v 1.4 2005/10/16 17:31:32 jpinto Exp $
*/
#include "stdinc.h"
#include "setup.h"
#include "sockutil.h"
#include "ircdio.h"
#include "ircservice.h"
#include "m_commands.h"
#include "common.h"
#include "channel.h"
#include "hash.h"
#include "event.h"
#include "send.h"
#include "umode.h"
#include "patchlevel.h"

char myservername[HOSTLEN];
char myserverinfo[HOSTLEN];
char myserverversion[HOSTLEN];
int ircd_fd;
int ircfullconnect = 0;
int ircs_debug;
int ircd_type = PTLINK6;
FILE *ircslogf;
int dkmask = 0;
static int server_sent = 0; /* SERVER message was sent */
static char *LocalAddress;

extern void cmodes_build(void);
/**
  * Set local address to bind before the connection
  */
void irc_SetLocalAddress(char *la)
{
  if(la)
    LocalAddress = strdup(la);
}

/**
  * init local server
  */
int irc_Init(int stype,char *sname, char *sinfo, FILE *logfd)
{
  cmodes_build();
  ircd_type = stype;
  ircslogf = logfd;
  strncpy(myservername, sname, HOSTLEN);
  strncpy(myserverinfo, sinfo, HOSTLEN);
  irc_SetVersion(PATCHLEVEL);
  server_sent = 0;
  ircfullconnect = 0;
  irc_CurrentTime = time(NULL);
  add_me();
  irc_ChanMlocker = irc_LocalServer;
  return 1;
}

/**
  * set server version
  */
void irc_SetVersion(char *version)
{
  strncpy(myserverversion, version, HOSTLEN);
}
/*
 * Lets attempt the connection
 */
int irc_StartConnect(char* ircserver,int port, char* connectpass,
         int options)
{
  ircd_buff_init();
  ircd_fd = sock_conn(ircserver,port, LocalAddress, ircslogf);
  if(ircd_fd<0)
    return ircd_fd;
  sendto_ircd(NULL, "PASS %s :TS",connectpass);
  sendto_ircd(NULL, "SERVER %s 1 %s :%s",
    myservername, myserverversion, myserverinfo);
  sendto_ircd(NULL, "SVINFO %d %d %d", TS_CURRENT, TS_MIN, irc_CurrentTime);
  sendto_ircd(NULL, "SVSINFO %d %d",
    irc_CurrentTime, 0);
  sendto_ircd(NULL,"PING :%s", myservername); 
  server_sent = 1;
  return ircd_fd;
}

                  
/*
 * Lets do a full connection (return after gettin all netjoin data)
 */
int irc_FullConnect(char* ircserver,int port, char* connectpass,
         int options)
{
  int rc;
  IRC_User *u;
  
  rc = irc_StartConnect(ircserver, port, connectpass, options);
  if(rc < 0) /* we got an error */
    return rc;
    
/* we need to add the core handlers here */
  irc_AddRawHandler("SERVER", m_server);
  irc_AddRawHandler("SQUIT", m_squit);
  irc_AddRawHandler("NICK", m_nick);
  irc_AddRawHandler("NNICK", m_nick);  
  irc_AddRawHandler("PONG", m_pong);
  irc_AddRawHandler("PING", m_ping);  
  irc_AddRawHandler("QUIT", m_quit);
  irc_AddRawHandler("MODE", m_mode);
  irc_AddRawHandler("KILL", m_kill);
  irc_AddRawHandler("PRIVMSG", m_privmsg);
  irc_AddRawHandler("SJOIN", m_sjoin);
  irc_AddRawHandler("NJOIN", m_sjoin);      
  irc_AddRawHandler("PART", m_part);
  irc_AddRawHandler("KICK", m_kick);  
  irc_AddRawHandler("TOPIC", m_topic);
  
  /* lets introduce our local clients */
  u = hash_next_localuser(1);
  while(u)
   {
     irc_IntroduceUser(u);
     u = hash_next_localuser(0);
   }
    
  while(irc_DoEvents() && !ircfullconnect)
    usleep(10000);
    
  if(ircfullconnect == 0)
    return -1;
    
  return 1;
}


/* enable/disable debug mode */
void irc_SetDebug(int dval)
{
  ircs_debug=dval;
}

/* Event handling loop, returns when connection is lost */
void irc_LoopWhileConnected(void) 
{
  while(irc_DoEvents())
    {
      irc_CallEvent(ET_LOOP, NULL, NULL);
      usleep(100);      
    }
}


void m_pong(int parc,char *parv[])
{
  if(parc<2)
    return ;
  ircfullconnect = 1;
}

void m_ping(int parc,char *parv[])
{
  if(parc<2)
    return;  
  sendto_ircd(myservername,"PONG :%s", parv[1]);    
}


void m_mode(int parc,char *parv[])
{
  IRC_User* u;
  if(parc<3)
    return;        
  if(parv[1][0] == '#')
  	{
    	  channel_mode(parc, parv, 1);
  	}
  else
  	{
  		u=irc_FindUser(parv[1]);
  		if(!u)
  		  {
  		      fprintf(ircslogf,"Mode for non-existent user %s\n",parv[1]);
  		      return ;
  		  }
    	umode_change(parv[2], u);
  	}
}

void m_privmsg(int parc,char *parv[])
{
	char *ch;
	IRC_User *dest;
	IRC_User *src = irc_FindUser(parv[0]);
	
	if(parc < 3)
	  return;
	  
	if(src == NULL)
	{
	  fprintf(ircslogf,"Received message from non existent user %s\n", parv[0]);
    return;		
	}
	
	if(parv[1][0] == '#')
	{
 		/* c = irc_FindChan(parv[1]); */
	}
	else if(parv[1][0] != '$') /* we simply ignore this */
	{
    if((ch = strchr(parv[1],'@'))) /* it can be nick@server */
      *ch = '\0';
    dest = irc_FindLocalUser(parv[1]);
    if(!dest)
    {
      fprintf(ircslogf,"Received message for non local user %s\n", parv[1]);
  	 	return ;
    }
    check_user_msg_events(dest, src, parv[2]);
  }
}

/* Returns the current connection status
 */
int irc_ConnectionStatus(void)
{
  if(ircfullconnect)
    return 2;
  if(server_sent)
    return 1;  
    
  return 0;
}
