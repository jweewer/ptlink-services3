/******************************************************************
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License           *
 * Please read the file COPYING for copyright information.        *
 ******************************************************************
    
  Description: chanrecord source file
  
 *  $Id: chanrecord.c,v 1.1.1.1 2005/08/27 15:43:42 jpinto Exp $
*/
#include "stdinc.h"
#include "chanrecord.h"
#include "hash.h"
#include "s_log.h"
#include "ircservice.h"
#include "my_sql.h"
#include "strhand.h"


/* just stats numbers */
static u_int32_t st_open_cr = 0;
static u_int32_t st_open_cr_cache = 0;
static u_int32_t st_create_cr = 0;
static u_int32_t st_close_cr = 0;
static u_int32_t st_update_cr = 0;

/* Returns the CR for a given name (be sure to close it)*/
ChanRecord* OpenCR(char *chan)
{
  ChanRecord* cr;
  IRC_Chan* c;
  u_int32_t scid;
  
  c = irc_FindChan(chan);
  if(c && c->sdata)
    {
      st_open_cr_cache++;
      cr = c->sdata;
      cr->ref_count++;
    }
  else
    {
      scid = chan2scid(chan);
      cr = db_mysql_get_cr(scid);  
      if(cr)
        st_create_cr--;
    }
  if(cr)
    {
      st_open_cr++;
      cr->change_type = R_UPDATE;
      return cr;
    }
  
  return NULL;
}

/* Updates a channel record, insert if its a new record */
int UpdateCR(ChanRecord *cr)
{
  st_update_cr++;
  if(cr->change_type == R_INSERT)
    {
      cr->change_type = R_UPDATE;
      return db_mysql_insert_cr(cr);
    }
  else
    return db_mysql_update_cr(cr);
}

void FreeCR(ChanRecord *cr)
{
  int i;
  FREE(cr->url);
  FREE(cr->email);
  FREE(cr->last_topic);
  FREE(cr->last_topic_setter);
  FREE(cr->mlock);
  FREE(cr->entrymsg);
  FREE(cr->cdesc);   
  for(i=0; i < MAX_EXTRA; ++i)
    array_free(cr->extra[i]);  
  free(cr);
}

void CloseCR(ChanRecord *cr)
{
  if(cr)
    { 
      st_close_cr++;
      if(--(cr->ref_count) == 0)
        FreeCR(cr);
    }
}

/* Creates a new NC */
ChanRecord* CreateCR(char *name)
{
  ChanRecord *cr = malloc(sizeof(ChanRecord));
  bzero(cr, sizeof(ChanRecord));
  strncpy(cr->name, name, CHANNELLEN);
  cr->change_type = R_INSERT;
  cr->ref_count = 1;  
  st_create_cr++;
  bzero(cr->extra, sizeof(cr->extra));
  return cr;
}


/* returns the scid for a given name */
u_int32_t chan2scid(char* name)
{
  
 /* maybe this will need an in memory map
    for now let's relay on the db */
  return db_mysql_chan2scid(name);
}

char* chanrecord_stats(void)
{
  static char buf[128];
  
  snprintf(buf, sizeof(buf), 
  "ChanRecord: Create() %d, Open() %d [%d from cache], Update() %d, Close() %d, Memory: %d",
  st_create_cr, st_open_cr, st_open_cr_cache, st_update_cr, st_close_cr,
    st_create_cr+st_open_cr-st_close_cr);
    
  return buf;
}
