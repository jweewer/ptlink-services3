/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************

  Description: irc interface module

 *  $Id: irc.c,v 1.7 2005/10/18 16:25:06 jpinto Exp $
*/

#include "module.h"
#include "patchlevel.h"
#include "dbconf.h"

SVS_Module mod_info =
 /* module, version, description */
{"irc", "1.0",  "just a irc module" };

/** functions/events we require **/
/* void (*FunctionPointer)(void);*/

extern int debug;
extern int nofork;
extern void write_pidfile(void);
extern void fork_process(void);
static int e_complete;

MOD_REQUIRES
  MOD_FUNC(dbconf_get_or_build)
  MOD_FUNC(e_complete)
MOD_END

/** functions/events we provide **/
/* void my_function(void); */
static char* irc = "irc module"; /* just some dumb function for dependency */

MOD_PROVIDES
  MOD_FUNC(irc)
MOD_END

/** Internal functions declaration **/
/* void internal_function(void); */
int ev_irc_connect(void* dummy1, void* dummy2);
    
/** Local dbconf settings **/

static char *LocalAddress;
static char *ServerName;
static char *ServerDesc;
static char *RemoteServer;
static int RemotePort;
static char *ServerPass;

/*
 * List of dbconf items we provide
 */
dbConfItem dbconf_provides[] =
{
  DBCONF_WORD_OPT(LocalAddress, NULL,
    "IP Address services should bind to before connecting to the IRC Server")
  DBCONF_WORD(ServerName, "services.ptlink.net",
    "Services Server Name")
  DBCONF_STR(ServerDesc, "PTlink IRC Services 3",
    "Services Server Description")
  DBCONF_WORD(RemoteServer, "localhost",
    "Hostname/IP of the IRC server for the services connection")
  DBCONF_INT(RemotePort, "6667",
    "Port number of the IRC server for the services connection")
  DBCONF_WORD(ServerPass, "servpass",
    "Password for the server connection, must match the C/N password")
  { NULL }
};


/** Local variables **/
/* int my_local_variable; */
  
/** rehash code **/
/* this is called before load and at services rehash */
int mod_rehash(void)
{
  if(dbconf_get_or_build(mod_info.name, dbconf_provides) < 0 )
  {
    errlog("Error reading dbconf!");
    return -1;
  }
  return 0;
}  

/** load code **/
int mod_load(void)
{

  /* Let's init the irc functions first, we may use them on modules */
  irc_SetLocalAddress(LocalAddress);
  irc_Init(PTLINK6, ServerName, ServerDesc, stderr);
  irc_SetVersion(svs_version);
  irc_SetDebug(debug); /* set irc level debug for -d */

  mod_add_event_action(e_complete, (ActionHandler) ev_irc_connect);
          
  return 0;
}
    
/** unload code **/
void mod_unload(void)
{
  return;
}
    
/** internal functions implementation starts here **/
int ev_irc_connect(void* dummy1, void* dummy2)
{
  int cr;  
  stdlog(L_INFO, "Connecting to IRC server %s:%i", RemoteServer, RemotePort);  
  cr = irc_FullConnect(RemoteServer, RemotePort, ServerPass, 0);
  if(cr < 0)
    {
      errlog("Could not connect to IRC server: %s", irc_GetLastMsg());
      exit(1);
    }        
  stdlog(L_INFO, "Netjoin complete, %.1d Kbs received", irc_InByteCount()/1024);    

  /* not sure if this fork should be on the irc module
     for now lets leave it like this to make sure we only fork after the
     connection is fully established
  */
  if( nofork == 0 )
  {
    fork_process();
    write_pidfile();
  }  
    
  irc_LoopWhileConnected();
  errlog("Disconnected:%s\n", irc_GetLastMsg());  

  /* stdlog(L_INFO, "PTlink IRC Services Terminated"); */
  
  return 0;
}
  
/* End of module */
