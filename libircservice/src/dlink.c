/*
 *  ircd-hybrid: an advanced Internet Relay Chat Daemon(ircd).
 *  tools.c: Various functions needed here and there.
 *
 *  Copyright (C) 2002 by the past and present ircd coders, and others.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
 *  USA
 *
 *  $Id: dlink.c,v 1.1.1.1 2005/08/27 15:44:12 jpinto Exp $
 *
 * When you update these functions make sure you update the ones in tools.h
 * as well!!!
 */

#include "stdinc.h"
#include "dlink.h"

/*
  From Hybrid7 - Lamego
 */
/* 
 * dlink_ routines are stolen from squid, except for dlinkAddBefore,
 * which is mine.
 *   -- adrian
 */
void
dlinkAdd(void *data, dlink_node * m, dlink_list * list)
{
 m->data = data;
 m->next = list->head;

 /* Assumption: If list->tail != NULL, list->head != NULL */
 if (list->head != NULL)
   list->head->prev = m;
 else /* if (list->tail == NULL) */
   list->tail = m;

 list->head = m;
 list->length++;
}

void
dlinkAddBefore(dlink_node *b, void *data, dlink_node *m, dlink_list *list)
{
    /* Shortcut - if its the first one, call dlinkAdd only */
    if (b == list->head)
    {
        dlinkAdd(data, m, list);
    }
    else
    {
      m->data = data;
      b->prev->next = m;
      m->prev = b->prev;
      b->prev = m; 
      m->next = b;
      list->length++;
    }
}

void
dlinkAddTail(void *data, dlink_node *m, dlink_list *list)
{
  m->data = data;
  m->next = NULL;
  m->prev = list->tail;
  /* Assumption: If list->tail != NULL, list->head != NULL */
  if (list->tail != NULL)
    list->tail->next = m;
  else /* if (list->head == NULL) */
    list->head = m;
 
  list->tail = m;
  list->length++;
}

/* Execution profiles show that this function is called the most
 * often of all non-spontaneous functions. So it had better be
 * efficient. */
void
dlinkDelete(dlink_node *m, dlink_list *list)
{
 /* Assumption: If m->next == NULL, then list->tail == m
  *      and:   If m->prev == NULL, then list->head == m
  */
  if (m->next)
    m->next->prev = m->prev;
  else
    list->tail = m->prev;
  if (m->prev)
    m->prev->next = m->next;
  else
    list->head = m->next;

  /* Set this to NULL does matter */
  m->next = m->prev = NULL;
  list->length--;
}


/*
 * dlinkFind
 * inputs	- list to search 
 *		- data
 * output	- pointer to link or NULL if not found
 * side effects	- Look for ptr in the linked listed pointed to by link.
 */
dlink_node *
dlinkFind(dlink_list *list, void * data )
{
  dlink_node *ptr;

  DLINK_FOREACH(ptr, list->head)
  {
    if (ptr->data == data)
      return (ptr);
  }
  return (NULL);
}

void
dlinkMoveList(dlink_list *from, dlink_list *to)
{
  /* There are three cases */
  /* case one, nothing in from list */
  if(from->head == NULL)
    return;

  /* case two, nothing in to list */
  /* actually if to->head is NULL and to->tail isn't, thats a bug */

   if(to->head == NULL)
   {
      to->head = from->head;
      to->tail = from->tail;
      from->head = from->tail = NULL;
      to->length = from->length;
      from->length = 0;
      return;
    }

  /* third case play with the links */

   from->tail->next = to->head;
   from->head->prev = to->head->prev;
   to->head->prev = from->tail;
   to->head = from->head;
   from->head = from->tail = NULL;
   to->length += from->length;
   from->length = 0;
  /* I think I got that right */
}

dlink_node *
dlinkFindDelete(dlink_list *list, void *data)
{
  dlink_node *m;

  m = dlinkFind(list, data);
  if (m)
    dlinkDelete(m, list);
  return(m);
}
