#include "ircservice.h"

#define lang2index(x,y) \
	if(strcmp((x),"en_us") ==0) \
	  (y) = 0; \
	else if(strcmp((x),"pt") == 0) \
	  (y) = 1; \
	else if(strcmp((x),"nl") == 0) \
	  (y) = 2; \
	else if(strcmp((x),"pt_br") ==0) \
	  (y) = 3; \
	else if(strcmp((x),"de") == 0 ) \
	  (y) = 4; \
	else \
	  (y) = -1;
	   
#define index2lang(x,y) \
	if((x) == 0) \
	  (y) = "en_us"; \
	else if((x) == 1) \
	  (y) = "pt"; \
	else if((x) == 2) \
	  (y) = "nl"; \
	else if((x) == 3) \
	  (y) = "pt_br"; \
	else if((x) == 4) \
	  (y) = "de";

#define MAX_LANGS 5


/* functions */
void send_lang(IRC_User *u, IRC_User *s, const char* message[], ...);
char* lang_str_l(int lang, const char* message[], ...);
char* lang_str(IRC_User *u, const char* message[], ...);
int AssociateLang(char *lstring);
char* format_str(IRC_User *u, const char* message[]);
int lang_for_host(char *host);
void lang_delete_assoc(void);

