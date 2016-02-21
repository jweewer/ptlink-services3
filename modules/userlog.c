/******************************************************************
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License           *
 * Please read the file COPYING for copyright information.        *
 ******************************************************************

  Description: userlog module

 *  $Id: userlog.c,v 1.4 2005/10/11 16:13:06 jpinto Exp $
*/

#include "module.h"

/* module, version, description */
SVS_Module mod_info =
{"userlog", "1.1", "log users connects/quits" };

/* Change Log 
  1.0 	-  0000297: userlog crash on module unload
*/

int ulog;

/* internal functions */
void ev_userlog_new_user(IRC_User* u, void *s);
void ev_userlog_quit(IRC_User* u, char* reason);

int mod_load(void)
{

  ulog = log_open("userlog","userlog");
  if(ulog < 0)
    {
      slog(L_ERROR,"Unable to create userlog log file");
      fprintf(stderr,"Unable to create userlog log file\n");
      return -1;
    }
    
  /* Add user events */
  irc_AddEvent(ET_NEW_USER, ev_userlog_new_user); /* new user */
  irc_AddEvent(ET_QUIT, ev_userlog_quit); /* user quit */
    
  return 0;
}

void
mod_unload(void)
{

  /* remove irc events */
  irc_DelEvent(ET_NEW_USER, ev_userlog_new_user);
  irc_DelEvent(ET_QUIT, ev_userlog_quit);
  
  log_close(ulog);
}
 


void ev_userlog_new_user(IRC_User* u, void *s)
{
  log_log(ulog, "LogOn","%s!%s@%s (%s) :%s",
     u->nick, u->username, u->realhost, u->publichost, u->info);
}

void ev_userlog_quit(IRC_User* u, char *quitreason)
{
  log_log(ulog, "LogOff","%s!%s@%s :%s",
     u->nick, u->username, u->realhost, quitreason);
}

