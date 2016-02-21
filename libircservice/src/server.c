#include "ircservice.h"
#include "irc_string.h"
#include "event.h"
#include "common.h"
#include "hash.h"
#include "user.h" /* we need remove_user() */

IRC_Server *irc_LocalServer;

/* internal functions */
void m_server(int parc, char *parv[]);
void m_squit(int parc, char *parv[]);
void del_all_from_server(IRC_Server *serv);

/*
        parv[1] = server name
        parv[2] = hop count (1 since we are directly connected)
        parv[3] = server version
        parv[4] = server description
*/
void m_server(int parc,char *parv[])
{
  IRC_Server* nserver;
  IRC_Server* from = NULL;
  char *name;
  /* validations */
  if(parc<5)
    return;
  name = parv[1];    
  if(parv[0])
    from = irc_FindServer(parv[0]);
  if(parv[0] && (from == NULL))
    {
      fprintf(ircslogf,"SERVER from non-existent server %s\n", parv[0]);
      return;
    }
  if(irc_FindServer(name) != NULL) 
    {
      fprintf(ircslogf,"Ignoring duplicated server %s from %s", 
        name, from->sname);
      return;
    }
  /* create the new server structure */
  nserver = malloc(sizeof(IRC_Server));
  bzero(nserver, sizeof(IRC_Server));    
  strncpy(nserver->nick, name, NICKLEN);
  nserver->sname = strdup(name);
  nserver->from = from;    
  	
  irc_CallEvent(ET_NEW_SERVER, nserver, nserver);  
  add_to_user_hash_table(name, nserver);
  /* add_user_to_global_list(nserver); */
}

/*
 * m_squit - SQUIT message handler
 *	parv[0] = sender prefix
 *   	parv[1] = server name
 *      parv[2] = comment
 */     
void m_squit(int parc,char *parv[])
{

  IRC_Server *serv = irc_FindServer(parv[1]);

  if(serv == NULL) /* this should never happen*/
    {
      fprintf(ircslogf,"SQUIT for non-existen server %s\n", parv[1]);
      return;
    }
    
  del_all_from_server(serv);
  remove_remote_user(serv);
}

void del_all_from_server(IRC_Server *serv)
{
  IRC_UserList ul;
  IRC_User *user, *next;
  user = irc_GetGlobalList(&ul);
  
  /* delete all users from that server */
  while(user)
    {
      next = irc_GetNextUser(&ul);
      if((user->server == serv) || (user->from == serv))
          remove_remote_user(user);        

      user = next;
    }
}

IRC_Server* irc_FindServer(char *name)
{
  return hash_find_server(name);
}


void add_me(void)
{
  IRC_Server* nserver;
  
  /* create the new server structure */
  nserver = malloc(sizeof(IRC_Server));
  bzero(nserver, sizeof(IRC_Server));    
  strncpy(nserver->nick, myservername, NICKLEN);
  nserver->sname = strdup(myservername);
  nserver->from = NULL;
  add_to_user_hash_table(myservername, nserver);
  /* add_user_to_global_list(nserver); */
  irc_LocalServer = nserver;
}
