/*
 *  $Id: channel.h,v 1.2 2005/10/16 17:31:32 jpinto Exp $
 */ 
IRC_Chan* irc_CreateChan(char *name);
IRC_ChanNode* add_user_to_chan(IRC_User* user, IRC_Chan *chan, int cumodes);
IRC_UserNode* add_chan_to_user(IRC_Chan* chan, IRC_User *user, int cumodes);
void del_user_from_chan(IRC_User *user, IRC_Chan *chan);
void del_chan_from_user(IRC_Chan* chan, IRC_User *user);
void channel_mode(int parc,char *parv[], int check_mlock);
void send_user_njoins(IRC_User* u);
char *cmode_string(IRC_Chan *chan);

/* m_commands */
void m_sjoin(int parc,char *parv[]);
void m_part(int parc,char *parv[]);
void m_topic(int parc,char *parv[]);
void m_kick(int parc,char *parv[]);

