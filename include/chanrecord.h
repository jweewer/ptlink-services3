/*****************************************************************
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999-2004 *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License          *
 * Please read the file COPYING for copyright information.       *
 *****************************************************************
    
  File: chanrecord.h
  Desc: chanrecord header file
  
 *  $Id: chanrecord.h,v 1.1.1.1 2005/08/27 15:45:10 jpinto Exp $
*/

#ifndef _CHAN_RECORD_H_
#define _CHAN_RECORD_H_
#include "ircservice.h"		/* we need CHANNELLEN */
#include "array.h"

/* max extra data arrays */
#define MAX_EXTRA       1

/* extra data indexes */
#define ED_AKICKS       0

#define R_INSERT        1
#define R_UPDATE        2

struct ChanRecord_s 
{
  u_int32_t scid;  
  char name[CHANNELLEN];
  char *url;
  char *email;
  u_int32_t founder;
  u_int32_t successor;
  char* last_topic;
  char* last_topic_setter;
  time_t t_ltopic;
  time_t t_reg;
  time_t t_last_use;
  char * mlock;
  char *entrymsg;
  time_t t_maxusers;
  int maxusers;
  u_int32_t status;
  u_int32_t flags;
  char *cdesc;
  int change_type;
  int ref_count;
  darray* extra[MAX_EXTRA];  
};
typedef struct ChanRecord_s ChanRecord;

/* Returns SCID for a chan */
u_int32_t chan2scid(char *name);

/* Returns a chan record */
ChanRecord* OpenCR(char *chan);

/* Closes a nick record */
void CloseCR(ChanRecord *cr);

/* Updates a chan record */
int UpdateCR(ChanRecord *cr);

/* Returns a newly created record */
ChanRecord* CreateCR(char *chan);

/* Insers a chan record on the db */
int InsertCR(ChanRecord *cr);

/* Release cr memory */
void FreeCR(ChanRecord *cr);

int get_last_chan_id(void);

char* chanrecord_stats(void);

/* some usefull macros */
#endif /* _NICK_RECORD_H_ */
