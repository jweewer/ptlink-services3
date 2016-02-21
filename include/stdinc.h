/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************

  Description: standard includes

 *  $Id: stdinc.h,v 1.1.1.1 2005/08/27 15:45:10 jpinto Exp $
*/

#define _GNU_SOURCE     
#ifndef _STDINC_H_
#define _STDINC_H_ 
#include "setup.h"

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef STRING_WITH_STRINGS
# include <string.h>
# include <strings.h>
#else
# ifdef HAVE_STRING_H
#  include <string.h>
# else
#  ifdef HAVE_STRINGS_H
#   include <strings.h>
#  endif
# endif 
#endif  


#ifdef HAVE_STDDEF_H
#include <stddef.h>
#endif



#include <stdio.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdarg.h>
#include <signal.h>
#ifdef USE_GETTEXT
#include <libintl.h>
#endif
#include <dirent.h>
#include <ctype.h>

#ifdef VMS
#define _XOPEN_SOURCE 1
#endif
#include <limits.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <sys/time.h>
#include <sys/types.h>
#include <sys/file.h>
#ifdef HAVE_SYS_RESOURCE_H
#include <sys/resource.h>
#endif


#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>

#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif
#include <netinet/in.h>
#include <arpa/inet.h>

#ifdef HAVE_ERRNO_H
#include <errno.h>
#else
extern int errno;
#endif

#ifdef VMS
#include <sys/ioctl.h>
#endif

#ifdef HAVE_TERMIOS_H
#include <termios.h>
#endif

#endif
