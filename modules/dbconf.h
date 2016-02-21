/************************************************************************
 * PTlink IRC Services 3 - (C) CopyRight PTlink IRC Software 1999-2005  *
 *                 http://software.pt-link.net                          *
 * This program is distributed under GNU Public License                 *
 * Please read the file COPYING for copyright information.              *
 ************************************************************************

  Description: dbconf header

*  $Id: dbconf.h,v 1.3 2005/10/01 08:34:28 jpinto Exp $
*/

typedef struct {
  char *name;
  char *type;
  void *vptr;
  char *def;
  char *desc;
  char *optional;
} dbConfItem;

typedef struct {
  char *module;
  char *name;
  void *vptr;
} dbConfGet;

#define DBCONF_WORD(x, y, z) { #x, "word", &(x), (y) , (z), "n" },
#define DBCONF_WORD_OPT(x, y, z) { #x, "word", &(x), (y) , (z), "y" },
#define DBCONF_STR(x, y, z) { #x, "str", &(x), (y) , (z), "n" },
#define DBCONF_STR_OPT(x, y, z) { #x, "str", &(x), (y) , (z), "y" },
#define DBCONF_INT(x, y, z) { #x, "int", &(x), (y) , (z), "n" },
#define DBCONF_SWITCH(x, y, z) { #x, "switch", &(x), (y) , (z), "n" },
#define DBCONF_TIME(x, y, z) { #x, "time", &(x), (y) , (z), "n" },
#define DBCONF_GET(x, y) { (x) , #y , &(y)},

#define DBCONF_REQUIRES  dbConfGet dbconf_requires[] = \
{

#define DBCONF_PROVIDES  dbConfItem dbconf_provides[] = \
{

#define DBCONF_END	{NULL} \
};

#ifdef DBCONF
int dbconf_get_or_build(char *module, dbConfItem* items);
int dbconf_get(dbConfGet* items);
#else
int (*dbconf_get_or_build)(char *module, dbConfItem* items);
int (*dbconf_get)(dbConfGet* items);
#endif

#define DBCONF_FUNCTIONS \
  MOD_FUNC(dbconf_get)\
  MOD_FUNC(dbconf_get_or_build)
