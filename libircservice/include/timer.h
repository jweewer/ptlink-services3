struct u_timer_event_s
{
  IRC_User *user;
  int trigger_time;
  EventHandler func;
  struct u_timer_event_s* lnext;
  int tag;
  int deleted;
};
typedef struct u_timer_event_s u_timer_event;

struct c_timer_event_s
{
  IRC_Chan *chan;
  int trigger_time;
  EventHandler func;
  struct c_timer_event_s* lnext;
  int tag;
  int deleted;
};
typedef struct c_timer_event_s c_timer_event;

void check_u_timer_events(void);
void check_c_timer_events(void);
