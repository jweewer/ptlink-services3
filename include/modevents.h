typedef void (*ActionHandler)(void *data1, void *data2);

/* returns an event handler for a given event name */
int mod_event_handle(char *name);

/* register an event name (for event generators) */
int mod_register_event(char *name);

/* unregister an event */
int mod_unregister_event(int evhandle);

/*  do event (call event actions for event handle) */
int mod_do_event(int evhandle, void *data1, void *data2);

/* adds an action associated with an event */
int mod_add_event_action(int evhandle, ActionHandler action);

/* dels an action associated with an event */
int mod_del_event_action(int evhandle, ActionHandler action);

/* cheks if a given module owns any events */
int mod_check_events(SVS_Module *module, char** evname, char** modname);

/* dels an action associated with an event */
void mod_del_all_mod_events(SVS_Module *module);

/* aborts the current event beeing "done" */
void mod_abort_event(void);

struct event_entry_s
{
  ActionHandler action;
  struct event_entry_s *next;
  SVS_Module *owner;
};

typedef struct event_entry_s event_entry;

struct event_table_entry_s
{
  char *name;
  int	ac_count;
  event_entry *ac_list;  
  void *owner;
};
typedef struct event_table_entry_s event_table_entry;
