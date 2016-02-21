/******************************************************************
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License           *
 * Please read the file COPYING for copyright information.        *
 ******************************************************************

  Desc: generic data array

 *  $Id: array.c,v 1.1.1.1 2005/08/27 15:43:46 jpinto Exp $
*/
#include "stdinc.h"
#include "array.h"
#include "strhand.h" /* we need match() */

/* initalize the array with an initial size */
void array_init(darray* a, int size, int type)
{
  int msize;
  
  if(type == DA_STRING)
    msize = sizeof(char*) * size;
  else
    msize = sizeof(u_int32_t) * size;
  a->data = malloc(msize);
  bzero(a->data, msize);
  a->size = size;
  a->type = type;
  a->count = 0;
}

/* free allocated space */
void array_free(darray* a)
{  
  if(a == NULL)
    return;
    
  if(a->type == DA_STRING)
    {
      int i;
      char **data = a->data;
      for(i = 0; i < a->count; ++i)
        free(data[i]);
    }
  if(a->data)
    free(a->data);
  a->data = NULL;
  free(a);
}

/* adds an int value to the array, 
   the function will increase the array size if required */
void array_add_int(darray* a, u_int32_t value)
{
  u_int32_t *data = a->data;  
  if(a->count == a->size) /* no free space, reallocate  */
    {
      a->size++;
      a->data = realloc(a->data, sizeof(u_int32_t) * (++a->size));
      data = a->data;
    }
  data[a->count++] = value;
}

/* adds a string value to the array, 
   the function will increase the array size if required */
void array_add_str(darray* a, char* value)
{
  char **data = a->data;  
  if(a->count == a->size) /* no free space, reallocate  */
    {
      a->size++;
      a->data = realloc(a->data, sizeof(char*) * (++a->size));
      data = a->data;
    }
  data[a->count++] = strdup(value);
}

/* deletes an int from the array */
void array_del_int(darray* a, u_int32_t value)
{
  int i;
  int *data = a->data;

  for(i = 0; i < a->count; ++i)
    if(data[i] == value) /* found value to delete */
      {
        data[i] = data[a->count-1];  
        --a->count;
        break;
      }
  return;
}

/* deletes a str from the array */
void array_del_str(darray* a, char* value)
{
  int i;
  char **data = a->data;

  for(i = 0; i < a->count; ++i)
    if(strcasecmp(data[i], value) == 0) /* found value to delete */
      {
        free(data[i]);
        data[i] = data[a->count-1];  
        --a->count;
        break;
      }
  return;
}

/* deletes all str from the array */
void array_delall_str(darray* a)
{
  int i;
  char **data = a->data;
  for(i = 0; i < a->count; ++i)
    free(data[i]);
  a->count = 0;
  return;
}

/* returns number of elements on the array*/
int array_count(darray* a)
{
  return a->count;
}

/* returns pointer to data (array of ints) */
u_int32_t* array_data_int(darray* a)
{
  return (u_int32_t*) a->data;
};

/* returns pointer to data (array of char*) */
char** array_data_str(darray* a)
{
  return (char**) a->data;
};

/* returns the position for a given value,
  -1 if the value is not found */
int array_find_int(darray* a, u_int32_t value)
{
  int i;
  int *data;
  if(a == NULL)
    return -1;
    
  data = a->data;

  for(i = 0; i < a->count; ++i)
    if(data[i] == value)
      return i;
      
  return -1;
}

/* checks if a given string matchs a mask on the array
   returns:
     the mask if there is a match, NULL if no match */
char* array_match_str(darray* a, char* value)
{
  int i;
  char **data;
  if(a == NULL)
    return NULL;
    
  data = a->data;

  for(i = 0; i < a->count; ++i)
    if(match(data[i], value) == 1)
      return data[i];
      
  return NULL;
}

/* returns the position for a given value,
  -1 if the value is not found */
int array_find_str(darray* a, char* value)
{
  int i;
  char **data;
  
  if(a == NULL)
    return -1;
    
  data = a->data;

  for(i = 0; i < a->count; ++i)
    if(strcasecmp(data[i], value) == 0)
      return i;
      
  return -1;
}
