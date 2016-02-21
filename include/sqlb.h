/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************

  Desc: sql builder

 *  $Id: sqlb.h,v 1.2 2005/09/14 06:13:53 jpinto Exp $
*/

void sqlb_init(char *table);
void sqlb_add_str(char *name, char *value);
void sqlb_add_int(char *name, u_int32_t value);
void sqlb_add_func(char *name, char* value);
char* sqlb_insert(void);

