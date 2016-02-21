#define LT_GLOBAL 	1	/* global list */
void add_user_to_global_list(IRC_User *user);
void del_user_from_global_list(IRC_User* user);
void del_user_chans(IRC_User* user);
IRC_User
*irc_CreateUser(char *nick, char *username, char *host, char *phost,
        char *info, char* umode, char *servername);
void remove_local_user(IRC_User *user);
void remove_remote_user(IRC_User *user);

        
