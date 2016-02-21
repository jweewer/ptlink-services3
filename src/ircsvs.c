/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************  

  Description: main file

 *  $Id: ircsvs.c,v 1.8 2005/10/31 21:29:16 jpinto Exp $
*/
#include "stdinc.h"
#include "path.h"
#include "ircservice.h"
#include "modules.h"
#include "ircsvs.h"
#include "s_log.h"
#include "patchlevel.h"
#include "modevents.h"
#include "my_sql.h"

/* local vars */
const char* DLOGPATH = LOGPATH;
int nofork = 0;	/* we should not fork */
int debug = 0; /* debug mode */
int sql_debug = 0; /* debug sql queries */
int e_expire = 0; /* data expire event */
int e_complete = 0; /* modules load complete */
  
  
/* internal functions declaration */

void ev_expire(IRC_User* u, char* reason);
static void setup_corefile(void);
void setup_signals();
void fork_process(void);
void write_pidfile(void);
int cmd_conf(int argc, char** argv);

static void parse_command_line(int argc, char* argv[])
{
  const char* options = "dnqv";
  int         opt;
  while ((opt = getopt(argc, argv, options)) != EOF) 
    {
      switch (opt) 
        {
          case 'n': nofork = -1; break;        
          case 'd': nofork = -1; debug = -1; break;
          case 'q': nofork = -1; sql_debug = -1; break;
          case 'v': printf("%s\n", svs_version); exit(0); break;
 	}
    }
}	

void fork_process(void)
{
  int pid;
  
  if((pid = fork()) < 0)
    {
      errlog("Couldn't fork: %s\n", strerror(errno));
      exit(0);
    }
  else if (pid > 0) /* this is the parent process */
    {
      stdlog(L_INFO, "Running in the background with pid=%d", pid);
      exit(0);
    }
    
  setsid();
}


static void check_pidfile(void)
{
  int fd;
  char buff[20];
  pid_t pidfromfile;
  char ppath[256];

#ifndef PIDFILE  
  snprintf(ppath, 256, "%s/run/ircsvs.pid", VARPATH);
#else
  strncpy(ppath, PIDFILE, 256);
#endif  
  if ((fd = open(ppath, O_RDONLY)) >= 0 )
  {
    if (read(fd, buff, sizeof(buff)) == -1)
    {
      /* ignore error on read */
    }
    else
    {
      pidfromfile = atoi(buff);
      if (pidfromfile != (int)getpid() && !kill(pidfromfile, 0))
      {
        printf("ERROR: ircsvs is already running with pid=%i\n", pidfromfile);
        exit(-1);
      }
    }
    close(fd);
  }
  else if(errno != ENOENT)
  {
    printf("WARNING: problem opening %s: %s\n", ppath, strerror(errno));
  }

}

void write_pidfile(void)
{
  int fd;
  char buff[20];
  char ppath[256];

#ifndef PIDFILE  
  snprintf(ppath, 256, "%s/run/ircsvs.pid", VARPATH);
#else
  strncpy(ppath, PIDFILE, 256);
#endif
  
  if ((fd = open(ppath, O_CREAT|O_WRONLY, 0600))>=0)
    {
      snprintf(buff,sizeof(buff), "%d\n", (int)getpid());
      if (write(fd, buff, strlen(buff)) == -1)
        errlog("Error writing to pid file %s", ppath);
      close(fd);
      return;
    }
  else
    errlog("Error opening pid file %s", ppath);
}

#if 0
/* 
   this function will launch the expire events
   according to the ExpireInterval setting
*/
void ev_expire(IRC_User* u, char* reason)
{
  static time_t last_expire_run;  
  if(irc_CurrentTime - last_expire_run < ExpireInterval)
    return;
  mod_do_event(e_expire, NULL, NULL); /* do the expire */
  last_expire_run = irc_CurrentTime;
}
#endif

/* conf was used on the command line 
 * mysql and dbconf modules must be loaded
 * the command line will be processed with dbconf_cmd_line()
 */
int cmd_conf(int argc, char** argv)
{
/*  int* (*dbconf_cmd_line)(int argc, char **argv); */
  int* (*dbconf_cmd_line)(int, char **);

  if(module_load("mysql", 1) <0 )
  {
    errlog("Error loading mysql module !");
    return -1;
  }
  if(module_load("dbconf", 1) < 0)
  {
    errlog("Error loading dbconf module !");
    return -2;
  }

  dbconf_cmd_line = attach_to_function("dbconf_cmd_line", NULL);
  if(dbconf_cmd_line == NULL)
  {
    errlog("Missing dbconf_cmd_line() function !");
    return -2;
  }
  
  return (int)dbconf_cmd_line(argc, argv);
}

/* check_logsize
 *
 * inputs       - nothing
 * output       - nothing
 * side effects - checks logfile if >100k log rotation will occur 
 *                
 * - Lamego
 */
static void check_logsize(void)
{
  struct stat finfo;
  char lpath[256], lpath1[256], lpath2[256], lpath3[256];
  snprintf(lpath, 256, "%s/ircsvs.log", LOGPATH);
  snprintf(lpath1, 256, "%s/ircsvs.log.1", LOGPATH);  
  snprintf(lpath2, 256, "%s/ircsvs.log.2", LOGPATH);
  snprintf(lpath3, 256, "%s/ircsvs.log.3", LOGPATH);    
  
  if(stat(lpath, &finfo)==0) /* successful */
	{
	  if (finfo.st_size > 100000) /* > 100k ? */
		{
		  printf("Rotating %s...\n", lpath);
		  rename(lpath2, lpath3);
		  rename(lpath1, lpath2);
		  rename(lpath, lpath1);		  
		}
	}
}  

int main(int argc, char* argv[])
{
  
  setup_corefile(); /* we wan't core files for debug */
  setup_signals();
  srandom(time(NULL)); /* random seed */  
  /* check if we are just doing a configuration change */
  if((argc > 1) && (strcasecmp(argv[1], "conf")==0))
    return cmd_conf(argc-2, &argv[2]);

  parse_command_line(argc, argv); /* parse command line options */
  printf("Starting %s, CopyRight PTlink IRC Software 1999-2005\n", svs_version);
  printf("Setting ircsvs log file to "LOGPATH "/ircsvs.log\n");

  /* rotate the logsize if required */
  check_logsize();  
  if(init_log(LOGPATH "/ircsvs.log") == 0)
  {
    fprintf(stderr,"ERROR: Could not create log file\n");
    return 3;
  }
  slog(L_INFO, "Starting %s", svs_version);
#if 0  
  if(TimeZone)
  {  
#ifdef HAVE_SETENV
    setenv("TZ",TimeZone,1);
    tzset();
#else
    fprintf(stderr, "TimeZone defined but setenv() is not supported on this OS!");
#endif
    }
#endif    
  /* Let's init the irc functions first, we may use them on modules */

  e_expire = mod_register_event("e_expire");
  e_complete = mod_register_event("e_complete");
  if(load_modules_file("ircsvs") < 0)
    {
      fprintf(stderr,"ERROR: Error loading modules\n");
      return -3;
    }
 
  stdlog(L_INFO, "All modules succesfully loaded");   
  /* check it here to avoid "Server already exists" uppon connection */
  if( nofork == 0 )
      check_pidfile();

#if 0
#warning need to move ExpireInterval to the module
  if(ExpireInterval)
    {
      stdlog(L_INFO, "Running expire routines...");
      ev_expire(NULL, NULL);    
      stdlog(L_INFO,"Expire interval set to %d minute(s)", ExpireInterval / 60);
      irc_AddEvent(ET_LOOP, ev_expire); /* set the expire routines */
    }
  else
    stdlog(L_WARN, "Data expiration is disabled");
#endif    

  stdlog(L_INFO, "Services startup completed.");
  mod_do_event(e_complete, NULL, NULL);  
  
  return 0;
}

static void setup_corefile(void)
{
  struct rlimit rlim; /* resource limits */

  /* Set corefilesize to maximum */
 if (!getrlimit(RLIMIT_CORE, &rlim))
  {
    rlim.rlim_cur = rlim.rlim_max;
    setrlimit(RLIMIT_CORE, &rlim);
  }
}

/*
* sigterm_handler - exit the server
*/
static void sigterm_handler(int sig)
{
  stdlog(L_CRIT, "Service killed By SIGTERM");
  exit(-1);
}

static void sighup_handler(int sig)
{
  stdlog(L_CRIT, "Received SIGHUP, rehashing");
  modules_rehash();
}
        
void setup_signals()
{
  struct sigaction act;
  
  act.sa_flags = 0;
  act.sa_handler = SIG_IGN;
  sigemptyset(&act.sa_mask);
  sigaddset(&act.sa_mask, SIGPIPE);
  sigaddset(&act.sa_mask, SIGALRM);
  sigaddset(&act.sa_mask, SIGTRAP);
  act.sa_handler = sigterm_handler;
  sigaction(SIGTERM, &act, 0);  
  sigaddset(&act.sa_mask, SIGHUP);
  act.sa_handler = sighup_handler;  
  sigaction(SIGHUP, &act, 0); 
}


