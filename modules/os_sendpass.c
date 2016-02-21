/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2006 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            
 **********************************************************************

  Description: operserv sendpass command

*/

#include "module.h"
#include "my_sql.h"
#include "nsmacros.h"
#include "ns_group.h"   /* is_sadmin */
#include "encrypt.h"
#include "email.h"
#include "lang/common.lh"
#include "lang/os_sendpass.lh"

#define PASSLEN	10	/* reset password length */

SVS_Module mod_info =
 /* module, version, description */
{"os_sendpass", "2.2",  "operserv sendpass module" };

/* Change Log
  2.2 - #77, OS SENDPASS is using the wrong field name
  2.1 - security fix
  2.0 - 0000265: remove nickserv cache system
*/

/** functions/events we require **/
/* void (*FunctionPointer)(void);*/
ServiceUser* (*operserv_suser)(void);

MOD_REQUIRES
  MOD_FUNC(operserv_suser)
  MOD_FUNC(is_sadmin)
  MOD_FUNC(is_sroot)
  MOD_FUNC(is_soper)
  EMAIL_FUNCTIONS
MOD_END


/** Internal functions declaration **/
/* void internal_function(void); */
void os_sendpass(IRC_User *s, IRC_User *u);


/** Local variables **/
/* int my_local_variable; */
ServiceUser *osu = NULL;
int os_log = 0;
    
/** load code **/
int mod_load(void)
{

#ifndef SENDMAIL
  errlog("sendmail binary path was not found, this module will not work");
#else
  os_log = log_handle("operserv");  
  osu = operserv_suser();  
  suser_add_cmd(osu, "SENDPASS", (void*) os_sendpass, SENDPASS_SUMMARY, SENDPASS_HELP);    
#endif  
  return 0;
}
    
/** unload code **/
void mod_unload(void)
{
  suser_del_mod_cmds(osu, &mod_info);
  return;
}
    
/** internal functions implementation starts here **/
void os_sendpass(IRC_User *s, IRC_User *u)
{
  u_int32_t source_snid;
  u_int32_t snid;
  char *target;
  char *email;
  int lang;
  
  /* status validation */  
  CHECK_IF_IDENTIFIED_NICK
  /* syntax validation */
  target = strtok(NULL, " ");

  if (!is_soper(u->snid))
  {
    send_lang(u, s, PERMISSION_DENIED);
    return;
  }
  
  if(!irc_IsUMode(u, UMODE_OPER)) /* extra security */
    return;
  else if(IsNull(target))
    send_lang(u, s, SENDPASS_SYNTAX);
  else if( (snid = nick2snid(target)) == 0 )
    send_lang(u, s, NICK_X_NOT_REGISTERED, target);
  /* sub-command */
  else if(is_sadmin(snid) || is_sroot(snid))
  {
    log_log(os_log, mod_info.name, "Nick %s trying SENDPASS on sadmin/soper %s",
      s->nick, target);
    irc_SendSanotice(s, "Nick %s trying SENDPASS on sadmin/soper %s",
      s->nick, target);
  }
  else if((sql_singlequery("SELECT email, lang FROM nickserv WHERE snid=%d", 
    snid) < 1) || ((email = sql_field(0)) == NULL))
     send_lang(u, s, OS_SENDPASS_NO_EMAIL_X, target);
  else
  {
    char buf[512];
    char pbuf[PASSLEN+1];
    lang = sql_field_i(1);
    rand_string(pbuf, PASSLEN, PASSLEN);
    pbuf[2] = '0'+ (random() % 10);
    sql_execute("UPDATE nickserv_security SET pass=%s WHERE snid=%d",
      sql_str(hex_str(encrypted_password(pbuf), 16)), snid);
    snprintf(buf, sizeof(buf), 
    "From: \"%%from_name%%\" <%%from%%>\r\nTo:\"%s\" <%s>\r\nSubject:%s\r\n\r\n%s",
      target, email,
      "Nick Password", 
      lang_str_l(lang, SENDPASS_X_X, target, pbuf)
      );
    email_init_symbols();
    email_add_symbol("email", email);
    email_send(buf);
    memset(pbuf, 0, PASSLEN);
    send_lang(u, s, SENDPASS_X_SENT_X, target, email);
    log_log(os_log, mod_info.name, "SENDPASS for %s requested by %s", 
      target, u->nick);
    irc_SendSanotice(s, "SENDPASS for %s requested by %s",
      target, u->nick);
  }
}

/* End of module */
