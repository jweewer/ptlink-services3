/*****************************************************************
 * PTlink OPM is (C) CopyRight PTlink IRC Software 1999-2002      *
 * http://www.ptlink.net/Coders/ - coders@PTlink.net             *
 * This program is distributed under GNU Public License          *
 * Please read the file COPYING for copyright information.       *
 *****************************************************************
 
  File: IRC I/O
  Description: IRCD I/O functions
  Author: Lamego@PTlink.net
*/
#include "sockutil.h"
#include "ircdio.h"
#include "irc_parse.h"
#include "send.h"
#include "ircservice.h"
#include "common.h"
#include "timer.h"

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

SockBuffer ircd_buffer_in;  /* incoming data buffer */
SockBuffer ircd_buffer_out; /* outgoing data buffer */
time_t last_irc_read;
time_t irc_CurrentTime;
void ircd_buff_init(void)
{  
  sockbuf_init(&ircd_buffer_in, INBUFFER_SIZE);
  last_irc_read = time(NULL);
}

/*
 * Check ircd buffer and parse data if available
 * Returns:
 * -1 = Sucess
 *  0 = Connection to ircd terminated
 */
int check_ircd_buffer(void)
{
#if 0
  fd_set fds_r;
  fd_set fds_w;
  struct timeval tv = {1,0}; /* 1 s timeout */
  int   retval;  
#endif  
  int   res;
  char *bufpos , *bufend;
  int	rc = 0;
  int fd = ircd_fd;
  if (fd < 0)
    {
	  fprintf(ircslogf,"invalid fd on check_ircd_buffer()");
	  exit(1);
    }
    
  if( time(NULL)-last_irc_read > IRC_READ_TIMEOUT)
    {
      fprintf(ircslogf,"Closed on IRC_READ_TIMEOUT");
      return 0;
    }
    
#if 0              
  FD_ZERO(&fds_r);
  FD_ZERO(&fds_w);
  FD_SET(fd, &fds_r);

  retval = select(fd+1, &fds_r, &fds_w, 0, &tv);

  if(retval>0 && FD_ISSET(fd, &fds_r))
    {
#endif    
      rc = sockbuf_read(fd, &ircd_buffer_in);

	  if(rc>0)
	    {
	      last_irc_read = time(NULL);
          bufpos = ircd_buffer_in.data;
          bufend = strchr(bufpos,'\r');	          

	      while(bufend && *bufpos)
		    {		
              (*bufend--)='\0';
         
		      while(*bufpos && ((*bufpos=='\r') || (*bufpos=='\n')))
                ++bufpos;                    

              irc_CurrentTime = time(NULL);
#if 0              
              log(L_DEBUG,"%d %d %d", bufpos, bufend, ircd_buffer_in.seekpos);
#endif              
			  irc_parse(bufpos, bufend);

			  bufpos = bufend+1;
			  if(bufpos<ircd_buffer_in.seekpos)  
                {                  
                  ++bufpos;
	              bufend = strchr(bufpos,'\r');
                }
		    }
          res = (ircd_buffer_in.seekpos-bufpos)+1;
		  if(res>0)
            {
		      memmove(ircd_buffer_in.data, bufpos, res);
              ircd_buffer_in.seekpos = ircd_buffer_in.data+(res-1);
            }                      
	    }
      else if (rc==-1)
		{
		  return 0; /* going to retry */
		}
	  else
	    {
#if 0        
        /* read buffer is full */
		}
#endif        
    }
        
  return -1;
} 

/* Check input buffer and process any raw events 
RETURNS:
  -1 = Connected
  0 = Disconnected
*/
int irc_DoEvents(void)
{
  check_u_timer_events();
  check_c_timer_events();
  return check_ircd_buffer();
}
