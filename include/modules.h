/******************************************************************
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License           *
 * Please read the file COPYING for copyright information.        *
 ******************************************************************
   
  File: modules.h
  Desc: modules header file
 
 *  $Id: modules.h,v 1.5 2005/10/16 18:32:18 jpinto Exp $
*/
#ifndef _MODULES_H
#define _MODULES_H
#define MODULES_VERSION	1

#define MAX_MODULES 	128


struct SVS_Module_s
{
  char *name;  		/* module name */
  char *version;	/* version */
  char *desc;		/* desription */
};
typedef struct SVS_Module_s SVS_Module;

struct SVS_Module_dep_s
{
  char *name;		/* symbol name */
  void *ptr;		/* data/function pointer */
};
typedef struct SVS_Module_dep_s SVS_Module_dep;

struct Module_Function_s
{
  char *name;		/* symbol name */
  void *ptr;		/* data/function pointer */  
};
typedef struct Module_Function_s Module_Function;

struct FunctionList_s
{
  char *name;
  void *ptr;
  SVS_Module *modinfo;
  SVS_Module **modattach;	/* modules attached to this function */
  int attcount;			/* counf of attached modules */
  struct FunctionList_s *next;
};
typedef struct FunctionList_s FunctionList;


  
/* usefull macro */
#define GET_PARENT_MODULE	\
  if(mod_info.parent) /* we need a parent module, lets search it */ \
    { \
      pmodule = module_find(mod_info.parent); \
    } \
  if(pmodule == NULL) \
    { \
      slog(L_ERROR, "Could not find parent module: %s", mod_info.parent); \
      return -1; \
    }
    
#endif

extern SVS_Module* CurrentModule;

#define MOD_FUNC(x) { #x, &x },

#define MOD_REQUIRES Module_Function mod_requires[] =\
{

#define MOD_PROVIDES Module_Function mod_provides[] =\
{

#define MOD_OPTIONS Module_Function mod_options[] =\
{

#define MOD_END {NULL}\
};

/* functions */
int module_load(char *modfn, int silent);
int module_unload(SVS_Module* module);
SVS_Module*  module_find(char *modname);
int load_modules_file(char* fn);
int get_installed_version(char *fname);    
void register_provides(Module_Function funclist[], SVS_Module* modinfo);
int register_requires(Module_Function moddeps[], SVS_Module *modinfo, int stop);
void *attach_to_function(char *name, SVS_Module* modinfo);
int check_for_functions(SVS_Module *modinfo, char ** fname, char **modname);
int get_local_dconf(void);
void module_delall_attach(SVS_Module *module);
void modules_rehash(void);

/* extern structures */
extern int modules_count;
extern SVS_Module* svs_modules[MAX_MODULES];

#define COMMON_MODULES_HEADERS \
#include "stdinc.h" \
#include "ircservice.h" \
#include "suser.h" \
#include "s_log.h" \
#include "log.h" \
#include "lang.h" \
#include "modules.h" \
#include "strhand.h" \
#include "modevents.h" \
#include "ircsvs.h"
