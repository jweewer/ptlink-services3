#include "ircservice.h"
#include "timer.h"

/* keep it lower to save cpu */
#define TIMER_INTERVAL 5

static u_timer_event *u_timers_list = NULL;

/* Add an event to the list */
void irc_AddUTimerEvent(IRC_User *u, int ttime, void* func, int tag)
{
  u_timer_event *t = malloc(sizeof(u_timer_event));
  t->user = u;
  t->tag = tag;
  t->trigger_time = irc_CurrentTime+ttime;
  t->func = (EventHandler) func;
  t->lnext = u_timers_list;
  u_timers_list = t;
  u->timer_count++;
  t->deleted = 0;
}

void irc_CancelUserTimerEvents(IRC_User *u)
{
  u_timer_event *t;
  
  if(u->timer_count == 0) /* no timers set to this user */
    return ;
    
  t = u_timers_list;
  /* stop before the event */
  while(t)
    {
      if(t->user == u)
        {
          t->deleted = 1;
          t->user->timer_count--;
        }
      t = t->lnext;
    }
}

void check_u_timer_events(void)
{
  static time_t last_run = 0;
  u_timer_event *t, *prev, *next;
  
  if(irc_CurrentTime-last_run > TIMER_INTERVAL)
    last_run = irc_CurrentTime-last_run;
  else
    return;
    
  t = u_timers_list;
  prev = NULL;
  
  /* stop before the event */
  while(t)
  {
    if(t->deleted)
    {
      next = t->lnext;
      if(prev) /* and delete */
        prev->lnext = next;
      else
        u_timers_list = next ;
      free(t);
      t = next;
      continue; 	  
    }
    if(irc_CurrentTime > t->trigger_time) 
    {
      t->func(t->user, &t->tag); /* execute */      
      t->deleted = 1; /* deleted on next run */
    }
    prev = t;
    t = t->lnext;
  }
}

static c_timer_event *c_timers_list = NULL;

/* Add an event to the list */
void irc_AddCTimerEvent(IRC_Chan *c, int ttime, void* func, int tag)
{
  c_timer_event *t = malloc(sizeof(c_timer_event));
  t->chan = c;
  t->tag = tag;
  t->trigger_time = irc_CurrentTime+ttime;
  t->func = (EventHandler) func;
  t->lnext = c_timers_list;
  c_timers_list = t;
  c->timer_count++;
  t->deleted = 0;
}

void irc_CancelChanTimerEvents(IRC_Chan *c)
{
  c_timer_event *t;
  
  if(c->timer_count == 0) /* no timers set to this user */
    return ;
    
  t = c_timers_list;
  /* stop before the event */
  while(t)
    {
      if(t->chan == c)
        {
          t->deleted = 1;
          t->chan->timer_count--;
        }
      t = t->lnext;
    }
}

void check_c_timer_events(void)
{
  static time_t last_run = 0;
  c_timer_event *t, *prev, *next;
  
  if(irc_CurrentTime-last_run > TIMER_INTERVAL)
    last_run = irc_CurrentTime-last_run;
  else
    return;
    
  t = c_timers_list;
  prev = NULL;
  
  /* stop before the event */
  while(t)
  {
    if(t->deleted)
    {
      next = t->lnext;
      if(prev) /* and delete */
        prev->lnext = next;
      else
        c_timers_list = next ;
      free(t);
      t = next; 	  
      continue;
    }
    if(irc_CurrentTime > t->trigger_time)
    {
      t->func(t->chan, &t->tag); /* execute */        
      t->deleted = 1; /* deleted on next run */
    }
    prev = t;
    t = t->lnext;    
  } 

}

void irc_TimerStats(IRC_User *to, IRC_User* from)
{
  u_int32_t count = 0;
  u_int32_t del_count = 0;
  u_timer_event* ut = u_timers_list;
  c_timer_event* ct = c_timers_list;
  while(ut)
  {
    ++count;    
    if(ut->deleted)
      ++del_count;
    ut = ut->lnext;      
  }
  irc_SendNotice(to, from, 
    "     User timer events: %d (%d deleted) [%d bytes]",
    count, del_count, count*sizeof(u_timer_event));
  count = 0; del_count = 0;
  while(ct)
  {
    ++count;
    if(ct->deleted)
      ++del_count;
    ct = ct->lnext;      
  }
  irc_SendNotice(to, from, 
    "  Channel timer events: %d (%d deleted) [%d bytes]",
    count, del_count, count*sizeof(c_timer_event));    
}
