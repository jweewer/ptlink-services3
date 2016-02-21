/*****************************************************************
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999-2000 *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License          *
 * Please read the file COPYING for copyright information.       *
 *****************************************************************
 
  File: sockutil.h
  Description: socket handling routines header file
  Author: Lamego@PTlink.net
*/

#include <stdio.h>
typedef struct {
    char* data;
    char* seekpos;
    int size;
} SockBuffer;

int sockprintf(int , char *, ...);
int sock_conn(char *hostname, unsigned short portnum, char* LocalAddress, FILE* logfd);
int sockbuf_read(int sock, SockBuffer *sbuf);
int sockbuf_init(SockBuffer *sbuf, int bsize);
void sockbuf_reset(SockBuffer *sbuf, int bsize);
int set_non_blocking(int fd);
