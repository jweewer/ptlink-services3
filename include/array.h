/******************************************************************
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License           *
 * Please read the file COPYING for copyright information.        *
 ******************************************************************

  Desc: generic data array

 *  $Id: array.h,v 1.1.1.1 2005/08/27 15:45:10 jpinto Exp $
*/

#ifndef _ARRAY_H_
#define _ARRAY_H_

/* array data types */
#define DA_INT		1
#define DA_STRING	2

struct darray_s
{
  int size;	/* allocated space */
  int type;	/* array type, int or string */
  int count;	/* count of used positions */  
  void *data;
};
typedef struct darray_s darray;

void array_init(darray* a, int size, int type);
void array_free(darray* a);
/* int arrays */
void array_add_int(darray* a, u_int32_t value);
void array_del_int(darray* a, u_int32_t value);
u_int32_t* array_data_int(darray* a);
int array_find_int(darray* a, u_int32_t value);
/* str arrays */
void array_add_str(darray* a, char* value);
void array_del_str(darray* a, char* value);
void array_delall_str(darray* a);
char** array_data_str(darray* a);
int array_find_str(darray* a, char* value);
int array_count(darray* a);
char* array_match_str(darray* a, char* value);
#endif
