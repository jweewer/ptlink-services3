#include "stdinc.h"
#include "modules.h" /* for CurrentModule */
#include "modevents.h"

/* event table */
static event_table_entry *events_table[512];

/* event count */
static int ev_count = 0;

static int event_was_aborted = 0;

/* returns an event handler for a given event name */
int mod_event_handle(char *name)
{
  int i;
  for(i=0; i<ev_count; ++i)
    if(events_table[i] && strcmp(events_table[i]->name, name) == 0)
      return i;
  return -1;  
}

/* register an event name (for event generators) */
int mod_register_event(char *name)
{
  event_table_entry *et;
  int i;
  if(ev_count>=512)
    return -1;
  i = mod_event_handle(name);
  if(mod_event_handle(name) >-1 )
    return i;
  et = malloc(sizeof(event_table_entry));
  et->name = strdup(name);
  et->ac_count = 0;
  et->ac_list = NULL;
  et->owner = CurrentModule;
  events_table[ev_count] = et;
  return ev_count++;
}    

/*
 * checks if a module owns any events which have associated actions
 * returns the name of the first event found
 * or NULL if the module owns no events.
 */
int mod_check_events(SVS_Module *module, char** evname, char** modname)
{
  int i;
  event_table_entry *et;
  for(i = 0; i < ev_count; ++i)
    {
      et = events_table[i];
      if((et->owner == module) && (et->ac_list != NULL))
        {
          *evname = et->name;
          *modname = et->ac_list->owner->name;
          return -1;
        }
    }
  return 0;
}
                                                                       
/* unregister an event */
int mod_unregister_event(int evhandle)
{
  event_table_entry *et;
  event_entry *ee;
  et = events_table[evhandle];
  ee = et->ac_list;
  
  /* first lets clear the event action list */
  while(et->ac_list)
    { 
      ee = et->ac_list;
      et->ac_list = ee->next;
      free(ee);
    }
    
  /* now delete the event entry */
  free(et->name);
  free(et);
  events_table[evhandle] = NULL;
  
  return 1;
}
                                                                                
/*  do event (call event actions for event handle) */
int mod_do_event(int evhandle, void *data1, void *data2)
{
  event_entry *ee = events_table[evhandle]->ac_list;
  int i =0;
  event_was_aborted = 0;
  while(ee && (event_was_aborted==0))
    {
      ee->action(data1, data2);
      ee=ee->next;
      ++i;
    }
  return i;
}
                                                                                
/* adds an action associated with an event */
int mod_add_event_action(int evhandle, ActionHandler action)
{
  event_entry *ee;
  ee = malloc(sizeof(event_entry));
  ee->action = action;
  ee->next = events_table[evhandle]->ac_list;
  ee->owner = CurrentModule;
  events_table[evhandle]->ac_list = ee;
  return 1;
}

void mod_del_all_mod_events(SVS_Module *module)
{
  int i;
  event_entry *ee, *prev, *next;
  
  /* Loop all event types */
  for(i = 0 ; i < ev_count; ++i)
    {
      ee = events_table[i]->ac_list;
      prev = NULL;
      while(ee)
        {
          if(ee->owner == module)
            {
              next = ee->next;
              if(prev)
                prev->next = ee->next;
              else
                events_table[i]->ac_list = next;
              free(ee);
              ee = next;
            }
          else
            {
              prev = ee;
              ee = ee->next;
            }
        }
    }
}

/* dels an action associated with an event */
int mod_del_event_action(int evhandle, ActionHandler action) 
{
  event_entry *prev, *ee;
  
  ee = events_table[evhandle]->ac_list;  
  prev = ee;
  while(ee)
  {
    if(ee->action == action)
      {
        if(ee == events_table[evhandle]->ac_list)
          events_table[evhandle]->ac_list = ee->next;
        else
          prev->next = ee->next;
        free(ee);
        break;
      }
    prev = ee;
    ee = ee->next;
  }
    
  return 1;
}

void mod_abort_event(void)
{
  event_was_aborted = 1;
}
