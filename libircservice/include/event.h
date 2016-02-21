
typedef struct
{
  int hits;
  EventHandler func;
#ifdef HAVE_GETTIMEOFDAY  
  struct timeval tv;
#endif  
} IRC_Event;

void  check_user_msg_events(IRC_User* dest, IRC_User* src, char *msg);
void  delete_user_msg_events(IRC_User* u);
