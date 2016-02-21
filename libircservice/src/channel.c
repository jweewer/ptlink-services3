#include "stdinc.h"
#include "ircservice.h"
#include "channel.h"
#include "common.h"
#include "send.h"
#include "hash.h"
#include "event.h"

/*
 *  m_sjoin
 * parv[0] - sender
 * parv[1] - TS
 * parv[2] - channel
 * parv[3] - modes + n arguments (key and/or limit)
 * parv[4+n] - flags+nick list (all in one parameter)
 *
 */
void m_sjoin(int parc,char *parv[])
{
  IRC_User* nuser;	/* new user */
  IRC_Server* server;
  IRC_Chan* chan;
  int skipparms;	/* skipparms */
  char *nicklist;
  char *c;
  int cumodes;		/* channel user modes (op,voice,...) */
  char* nick;
  char* para[16];
  
  if(parc<4)
    {
      fprintf(ircslogf,"Invalid sjoin %s\n", irc_GetLastMsg());
    }

  server = irc_FindServer(parv[0]);

  if(server == NULL)
    {
      fprintf(ircslogf,"SJOIN from non-existent server %s\n", parv[0]);
      return;
    }

  chan = irc_FindChan(parv[2]);
  if( chan == NULL) /* its a new channel, lets create it */
    {
      chan = irc_CreateChan(parv[2]);
      add_to_channel_hash_table(parv[2], chan);
    }
  /* set TS */    
  chan->ts = atoi(parv[1]);
  
  /* parse channel modes */
  c = parv[3];
  skipparms = 0;
  while(*c)
    {
      switch(*c) {
        case 'k': 
        case 'l': 
        case 'f': 
          ++skipparms; para[2+skipparms] = parv[3+skipparms]; break;
        default: 
          chan->cmodes |= chan_modes_from_c_to_bitmask[(unsigned char)*c];
          break;
       }
      c++;
    }
  para[0] = parv[0];
  para[1] = chan->name;
  para[2] = parv[3];
  /* channel_mode(3+skipparms, para); */
  
  nicklist = parv[3+skipparms+1];
  nick = strtok(nicklist, " ");
  while(nick)
    { 
      cumodes = 0;
      while(*nick == '@' || *nick =='+' || *nick == '.' || *nick == '%')
        {
          switch(*nick)
          {
            case '.': cumodes |= CU_MODE_ADMIN; break;
            case '@': cumodes |= CU_MODE_OP; break;
            case '+': cumodes |= CU_MODE_VOICE; break;
            case '%': break; /* no code for half-ops yet */
          }
          ++nick;
        }
      nuser = irc_FindUser(nick);
      if(nuser == NULL)
        {
          fprintf(ircslogf,"SJOIN %s from non-existent user %s\n",
            parv[2], nick);          
        }
      else /* ok to process nick join */
        {        
          add_chan_to_user(chan, nuser, cumodes);
          add_user_to_chan(nuser, chan, cumodes);          
        }                     
      nick = strtok(NULL, " ");
    }
  
  channel_mode(3+skipparms, para, 1);
  
}

/*
 *  m_part
 * parv[0] - sender
 * parv[1] - channel
 *
 */
void m_part(int parc,char *parv[])
{
  IRC_User* user;
  IRC_Chan* chan;
  if(parc<2)
    return;
  user = irc_FindUser(parv[0]);
  if(user == NULL)
    {
      fprintf(ircslogf,"PART %s from non-existent user %s\n",
        parv[1], parv[0]);
      return;
    }
  chan = irc_FindChan(parv[1]);
  if(chan == NULL)
    {
      fprintf(ircslogf,"PART from non-existent channel %s ,  user %s\n",
        parv[1], parv[0]);
      return;
    };
  del_chan_from_user(chan, user); 
  del_user_from_chan(user, chan); /* this will destroy chan for empty channels */
}

/*
 *  m_kick
 * parv[0] - sender
 * parv[1] - channel
 * parv[2] - target
 *
 */
void m_kick(int parc,char *parv[])
{
  IRC_User* source, *target;
  IRC_Chan* chan;
  
  if(parc<3)
    return;
  source = irc_FindUser(parv[0]);    
  target = irc_FindUser(parv[2]);
  
  if(target == NULL) /* it could be a local user */
    target = irc_FindLocalUser(parv[2]);
    
  if(source == NULL)
    {
      fprintf(ircslogf,"KICK %s from non-existent user %s\n",
        parv[1], parv[0]);
      return;
    }
  if(target == NULL)
    {
      fprintf(ircslogf,"KICK %s for non-existent user %s\n",
        parv[1], parv[2]);
      return;
    }    
  
  chan = irc_FindChan(parv[1]);
  if(chan == NULL)
    {
      fprintf(ircslogf,"KICK on non-existent channel %s, from %s,  target %s\n",
        parv[1], parv[0], parv[2]);
      return;
    };
  del_chan_from_user(chan, target); 
  del_user_from_chan(target, chan); /* this will destroy chan for empty channels */
}

/*
 *  m_topic
 **      parv[0] = sender prefix
 **      parv[1] = channel
 **      parv[2] = topic nick
 **      parv[3] = topic time
 **      parv[4] = topic text
 */
void m_topic(int parc,char *parv[])
{
  IRC_Chan* chan;
  IRC_User* user;
  if(parc<4)
    return;

  chan = irc_FindChan(parv[1]);
  user = irc_FindUser(parv[2]);
  if(chan == NULL)
    {
     /* we get this for +O channels
      fprintf(ircslogf,"TOPIC from non-existent channel %s\n",
        parv[1]);
     */
      return;
    };
  chan->t_ltopic = atoi(parv[2]);
  FREE(chan->last_topic_setter);
  chan->last_topic_setter = strdup(parv[2]);
  FREE(chan->last_topic);
  if(parc>4)
    chan->last_topic = strdup(parv[4]);
  irc_CallEvent(ET_CHAN_TOPIC, chan, user);
}

/* creates a new channel structure */
IRC_Chan *irc_CreateChan(char *name)
{
  IRC_Chan* c;

  c = malloc(sizeof(IRC_Chan));
  bzero(c, sizeof(IRC_Chan));
  strncpy(c->name, name, CHANNELLEN);

  return c;
}

IRC_Chan* irc_FindChan(char *name)
{
  return hash_find_channel(name);
}

/* add user to channel's userlist */
IRC_ChanNode* add_user_to_chan(IRC_User* user, IRC_Chan *chan, int cumodes)
{
  IRC_ChanNode* node;
  if(irc_IsLocalUser(user) && (chan->local_user == NULL))
    chan->local_user = user;
  chan->users_count++;
  if(irc_IsLocalUser(user))
   chan->lusers_count++;   
  node = malloc(sizeof(IRC_ChanNode));
  node->user = user;
  node->cumodes = cumodes;
  node->next = chan->userlist;  
  chan->userlist = node;
  irc_CallEvent(ET_CHAN_JOIN, chan, node);
  return node;
}

/* del user from channel's userlist */
void del_user_from_chan(IRC_User *user, IRC_Chan *chan)
{
  IRC_ChanNode* node, *old_node;
       
  node = chan->userlist;
  old_node = NULL;  
  while(node && (node->user != user)) /* search for the user to delete */
    {
      old_node = node;
      node = node->next;
    }
    
  if(node == NULL)
    return;

  irc_CallEvent(ET_CHAN_PART, chan, user);    
  /* first remove user from channel's user list */
  if(old_node) /* fix list chain/list header as needed */
    old_node->next = node->next;
  else
    chan->userlist = node->next;
        
  /* remove node */
  free(node);
  chan->users_count--;
  /* check if we need to decrease the local users count and set another local_user */
  if(irc_IsLocalUser(user))
  {
    chan->lusers_count--;
    if(user == chan->local_user) /* local user is leaving */
    {  /* lets search for other local user */
      chan->local_user = NULL;
      node = chan->userlist;
      while(node)
      {
        if(irc_IsLocalUser(node->user))
        {
          chan->local_user = node->user;
          break;
        }        
        node = node->next;
      }
    }
  }
  assert(chan->users_count >= 0); /* this should always be true */
  if(chan->userlist == NULL)
    {
      IRC_BanList *next, *bl = chan->bans;    
      assert(chan->users_count == 0); /* this should always be true */
      del_from_channel_hash_table(chan->name, chan);
      FREE(chan->last_topic);
      FREE(chan->last_topic_setter);
      FREE(chan->key);
      FREE(chan->mlock_key);
      while(bl)
        {
          next = bl->next;
          free(bl->value);
          free(bl);
          bl = next;
        }
      irc_CancelChanTimerEvents(chan);
      free(chan);
    }
}

/* add channel to user's channel list */
IRC_UserNode* add_chan_to_user(IRC_Chan* chan, IRC_User *user, int cumodes)
{
  IRC_UserNode* un;  
  un = malloc(sizeof(IRC_UserNode));
  un->chan = chan;
  un->next = user->chanlist;  
  un->cumodes = cumodes;
  user->chanlist = un;
  return un;
}

/* del chan from user's chanlist */
void del_chan_from_user(IRC_Chan *chan, IRC_User *user)
{
  IRC_UserNode *node, *old_node;
  
  node = user->chanlist;
  old_node = NULL;  
  while(node && (node->chan != chan)) /* search for the user to delete */
    {
      old_node = node;
      node = node->next;
    }

  if(node == NULL)
    return;
    
  /* first remove user from channel's user list */
  if(old_node) /* fix list chain/list header as needed */
    old_node->next = node->next;
  else
    user->chanlist = node->next;
      
  /* remove node */
  free(node);
}

/* check if the user is op on channel
   returns:
     >0 - yes
     0 - no
*/

int irc_IsChanOp(IRC_User* user, IRC_Chan* chan)
{
  IRC_ChanNode* cn;
  
  cn = chan->userlist;
  while(cn && cn->user != user) /* lets look for the correct node */
    cn = cn->next;
    
  if(cn)
    return (cn->cumodes & CU_MODE_OP);
  else
    return 0;
}

IRC_Chan* irc_NextChan(int start) 
{
  return hash_next_channel(start);  
}

void irc_Kick(IRC_User* from, IRC_Chan* chan, IRC_User *to, char *fmt, ...)
{
  char buf[BUFSIZE];
  va_list args;
  buf[0]='\0';
  if(fmt)
  {
    va_start(args, fmt); 
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
  }
  sendto_ircd(from->nick, "KICK %s %s :%s", 
    chan->name, to->nick, *buf ? buf : "");
  del_chan_from_user(chan, to);
  del_user_from_chan(to, chan); /* this will destroy chan for empty channels */  
}

/* get the user out ot channels */
void irc_ChanPart(IRC_User* user, IRC_Chan* chan)
{
  /* firs lets check the user is a local user */
  if(!irc_IsLocalUser(user))
    {
       fprintf(ircslogf, "irc_ChanJoin(): for remote user %s\n", 
         user->nick);
       return;
    }
  if(irc_ConnectionStatus()>0) /* we are connected, send the SJOIN */
    sendto_ircd(user->nick, "PART %s", chan->name);         
  del_chan_from_user(chan, user);
  del_user_from_chan(user, chan); /* this will destroy chan for empty channels */  

}

/* changes a channel topic */
IRC_Chan* irc_ChanTopic(IRC_Chan* chan, char* who, int t_topic, char *topic)
{
  chan->t_ltopic = t_topic;
  FREE(chan->last_topic_setter);
  chan->last_topic_setter = strdup(who);
  FREE(chan->last_topic);
  chan->last_topic = strdup(topic);
  sendto_ircd(NULL, "TOPIC %s %s %lu :%s", chan->name, who, 
    t_topic, topic);     
  return 0;
}

/* makes a local user join a channel */
IRC_Chan* irc_ChanJoin(IRC_User* user, char* chname, int cumodes)
{
  IRC_Chan* chan;
  
  /* firs lets check the user is a local user */
  if(!irc_IsLocalUser(user))
    {
       fprintf(ircslogf, "irc_ChanJoin(): for remote user %s\n", 
         user->nick);
       return NULL;
    }
    
  chan = irc_FindChan(chname);
  if(chan == NULL) /* Channel does not exist, lets create it */
    {
      if( chan == NULL) /* its a new channel, lets create it */
        {
          chan = irc_CreateChan(chname);
          add_to_channel_hash_table(chname, chan);
        }
        
      /* set TS */    
      chan->ts = time(NULL);
    }

  /* now update the channel's and user's list */
  if(irc_ConnectionStatus()>0) /* we are connected, send the SJOIN */
    {
      char cumodestr[10];
      int i = 0;      
      if(cumodes & CU_MODE_ADMIN)
        cumodestr[i++]='.';
      if(cumodes & CU_MODE_OP)
        cumodestr[i++]='@';
      if(cumodes & CU_MODE_VOICE)
        cumodestr[i++]='+';
      cumodestr[i] = '\0';
      sendto_ircd(myservername, "SJOIN %d %s +nt :%s%s", 
        time(NULL), chan->name, cumodestr, user->nick);    
    }
  add_chan_to_user(chan, user, cumodes);        
  add_user_to_chan(user, chan, cumodes);
  return chan;  
}

#if 0
/* makes a local user part a channel */
void* irc_ChanPart(IRC_User* user, char* chname)
{
  IRC_Chan* chan;
  
  /* firs lets check the user is a local user */
  if(!irc_IsLocalUser(user))
    {
       fprintf(ircslogf, "irc_ChanPart(): for remote user %s\n", 
         user->nick);
       return NULL;
    }
    
  chan = irc_FindChan(chname);
  if(chan == NULL) /* Channel does not exist, just ignore the command */
    return;

  /* now update the channel's and user's list */
  del_chan_from_user(chan, user);  
  del_user_from_chan(user, chan);
  if(irc_ConnectionStatus()>0) /* we are connected, send the SJOIN */
    sendto_ircd(user->nick, "PART %s :%s%s", chname);   
  return;
}
#endif

/* send NJOINS for user's list of channels */
void send_user_njoins(IRC_User* user)
{
  IRC_UserNode *node;
  char cumodes[10];
  int i;
  char* cmodes;
  
  node = user->chanlist;
  while(node) /* search for the user to delete */
    {
      i = 0;
      /* lets build the cmodes string on first join */
      cmodes = cmode_string(node->chan); 

      if(node->cumodes & CU_MODE_ADMIN)
        cumodes[i++]='.';
      if(node->cumodes & CU_MODE_OP)
        cumodes[i++]='@';        
      if(node->cumodes & CU_MODE_VOICE)
        cumodes[i++]='+';        
      cumodes[i] = '\0';
      sendto_ircd(myservername, "NJOIN %d %s +%s :%s%s", 
        time(NULL), node->chan->name, cmodes, cumodes, user->nick);
      node = node->next;
    }
}

/*   Returns a channode of the user if its found on channel chan */
IRC_ChanNode* irc_FindOnChan(IRC_Chan *chan, IRC_User *user)
{
  IRC_ChanNode* node;
  
  node = chan->userlist;

  while(node && (node->user != user)) /* search for the user to delete */
    {
      node = node->next;
    }
  return node;
}

void irc_ChanInvite(IRC_Chan *chan, IRC_User *target, IRC_User *source)
{
  sendto_ircd(source->nick, "INVITE %s %s", target->nick, chan->name);
}

void irc_CNameInvite(char *chname, IRC_User *target, IRC_User *source)
{
  sendto_ircd(source->nick, "INVITE %s %s", target->nick, chname);
}

