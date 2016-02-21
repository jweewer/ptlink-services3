/*
 * send.h
 *
 */

#ifndef INCLUDED_send_h
#define INCLUDED_send_h
#define BUFSIZE	512
extern  void sendto_ircd(char *, const char *, ...);
extern  void send_notice(char *source, const char *target, const char *fmt, ...);
extern void send_msg(char *source, const char *target, const char *fmt, ...);
#endif
