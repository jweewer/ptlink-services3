/******************************************************************
 * ircservices lib is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License           *
 * Please read the file COPYING for copyright information.        * 
 ******************************************************************
 
  File: sockutil.c
  Description: socket handling routines
  Author: Lamego@PTlink.net
*/

#include "setup.h"
#include "ircservice.h"
#include "common.h"
#include "sockutil.h"

#include <sys/poll.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h> 
#include <stdio.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <sys/wait.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>

/* local state variables */ 
static int is_connected = 0;
static u_int32_t in_byte_count = 0;
static u_int32_t out_byte_count = 0;

/* establish socket connection */
int sock_conn(char *hostname, unsigned short portnum, char *LocalAddress, FILE* logfd) 
  { 
	struct sockaddr_in sa;  
	struct hostent     *hp;   
	int s;   
        /* For local bind() */
        struct sockaddr_in SCAN_LOCAL;

        is_connected = 0; /* just to be safe */
        memset(&SCAN_LOCAL, 0, sizeof(struct sockaddr_in));

        /* Setup SCAN_LOCAL for local bind() */
        if (LocalAddress) {
                if (!inet_aton(LocalAddress, &(SCAN_LOCAL.sin_addr))) {
                        fprintf(logfd,"SCAN -> bind(): %s is an invalid address",
                            LocalAddress);
                        exit(EXIT_FAILURE);
                }

                SCAN_LOCAL.sin_family = AF_INET;
                SCAN_LOCAL.sin_port = 0;
        }

	if(ircs_debug)
	  fprintf(logfd,"Trying connection to %s, port %i...\n", hostname, portnum);
	
	if ((hp= gethostbyname(hostname)) == NULL) 
	  { /* do we know the host's */     
		errno= ECONNREFUSED;                       /* address? */     
		return(-1);                                /* no */   
	  }   
	memset(&sa,0,sizeof(sa));   
	memcpy((char *)&sa.sin_addr,hp->h_addr,hp->h_length);     
	
	/* set address */   
	sa.sin_family= hp->h_addrtype;   
	sa.sin_port= htons((u_short)portnum);   
	
	if ((s= socket(hp->h_addrtype,SOCK_STREAM,0)) < 0)     /* get socket */     
	  return(-1);   

      /* Bind to specific interface designated in conf file. */
        if (LocalAddress) {
                if (bind(s, (struct sockaddr *)&SCAN_LOCAL,
                    sizeof(struct sockaddr_in)) == -1) {
                        switch (errno) {
                        case EACCES:
                                fprintf(logfd, "SCAN -> bind(): No access to bind to %s\n",
                                    LocalAddress);
                                break;
                        default:
                                fprintf(logfd, "SCAN -> bind(): Error binding to %s\n",
                                    LocalAddress);
                                break;

                        }
                        exit(EXIT_FAILURE);
                }
        }
	  
	if (connect(s,(struct sockaddr *)&sa,sizeof sa) < 0) 
	  { /* connect */     
		close(s);     
		return(-1);   
	  }   
	set_non_blocking(s);
	is_connected = 1;
	return(s); 
}

int sockprintf(int s, char *fmt, ...)
{
  va_list args;
  char buf[16384];	/* Really huge, to try and avoid truncation */
  int ret;
  int len;
  char *bufp = buf;

  if(is_connected == 0)
    return 0;
      
  va_start(args, fmt);
  vsnprintf(buf, sizeof(buf), fmt, args);
  len = strlen(buf);
  if(len == 0) /* just to  be safe */
    return 0;
  /* we should check for errors here sure */
  do 
  {
    ret = write(s, bufp, len);
    if(ret>0)
    {
      bufp += len;
      len -= ret; 
      out_byte_count += ret;
    }
  } while (((ret==-1) && (errno==EAGAIN)) || ((ret>=0) && len>0));
  return ret;
}


int sockbuf_init(SockBuffer *sbuf, int bsize) {
    sbuf->data = malloc(bsize);
    sbuf->data[0]='\0';
    sbuf->seekpos = sbuf->data;
    sbuf->size = bsize;
    return 1;
}


int sockbuf_read(int sock, SockBuffer *sbuf) {
    int retval;
    int toread;
    
#if 0
    static struct pollfd ufds[1];
    
    ufds[0].events = POLLIN;
    ufds[0].revents = 0;
    ufds[0].fd = sock;

    if(poll(ufds, 1, 100)<1 || !(ufds[0].revents & POLLIN))
      return 0;
#endif
    toread =  &(sbuf->data[sbuf->size-1]) - sbuf->seekpos-1;
    
      
    if(toread<1)
      {
/*        log(L_ERROR,"Read buffer is FULL!!!"); */
        return 0;
      }
   
    retval = read(sock,sbuf->seekpos, toread);

    if((retval==-1) && (errno==EAGAIN))
      return 0;
    
    if(retval<1) {
	  return -1;
    }
    *(sbuf->seekpos+retval)='\0';    

    sbuf->seekpos+=retval;
    if(retval>0)
      in_byte_count += retval;
    return retval;
}

void sockbuf_reset(SockBuffer *sbuf, int bsize) 
{
  sbuf->data[0]='\0';
  sbuf->seekpos = sbuf->data;
}

/*
 * set_non_blocking - Set the client connection into non-blocking mode. 
 * If your system doesn't support this, you're screwed, ircd will run like
 * crap.
 * returns true (1) if successful, false (0) otherwise
 */
/* from ircd -Lamego */
int set_non_blocking(int fd)
{
  /*
   * NOTE: consult ALL your relevant manual pages *BEFORE* changing
   * these ioctl's.  There are quite a few variations on them,
   * as can be seen by the PCS one.  They are *NOT* all the same.
   * Heed this well. - Avalon.
   */
  /* This portion of code might also apply to NeXT.  -LynX */
#ifdef NBLOCK_SYSV
  int res = 1;

  if (ioctl(fd, FIONBIO, &res) == -1)
    return 0;

#else /* !NBLOCK_SYSV */
  int nonb = 0;
  int res;

#ifdef NBLOCK_POSIX
  nonb |= O_NONBLOCK;
#endif
#ifdef NBLOCK_BSD
  nonb |= O_NDELAY;
#endif

  res = fcntl(fd, F_GETFL, 0);
  if (-1 == res || fcntl(fd, F_SETFL, res | nonb) == -1)
    return 0;
#endif /* !NBLOCK_SYSV */
  return 1;
}

u_int32_t irc_InByteCount(void)
{
  return in_byte_count;
}

u_int32_t irc_OutByteCount(void)
{
  return out_byte_count;
}
