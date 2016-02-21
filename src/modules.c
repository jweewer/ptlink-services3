/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************

  Description: modules management routines
 
 *  $Id: modules.c,v 1.5 2005/10/18 16:25:06 jpinto Exp $
*/

#include "stdinc.h"
#ifdef HAVE_DLFCN_H
#include <dlfcn.h>
#endif 
#include "ircservice.h"
#include "modules.h"
#include "path.h"
#include "s_log.h"
#include "strhand.h"
#include "modevents.h"
#include "ircsvs.h"
#include "s_log.h"

SVS_Module* CurrentModule = NULL;

int modules_count = 0;
SVS_Module* svs_modules[MAX_MODULES];
void* svs_modules_h[MAX_MODULES];
FunctionList* provides_list = NULL;

/* internal functions declaration */
int write_version(char *fname, char *version);

/* 
  This function will load a module, module's dconf and .modules if found  
*/
int module_load(char *modname, int silent)
{
  void *handle;
  char mod_fn[256];
  SVS_Module *mod_info;
  int *mversion;
  void* mod_load;
  void* mod_unload;
  void* mod_rehash;
  void* mod_expire;
  char* dlerr;
  Module_Function* mod_provides;
  Module_Function* mod_requires;
  Module_Function* mod_options;
  int result;

  if(modules_count >= MAX_MODULES)
    {
	/* Why abort everything if we can just return? -blackfile */
	errlog("Attempt to load too many modules MAX=%i", MAX_MODULES);
	return -2;
    }

  snprintf(mod_fn, sizeof(mod_fn), "%s/%s.so", MODPATH, modname);     
  
  /* Try to open module */
  handle = dlopen(mod_fn , RTLD_NOW);
  if(IsNull(handle))
   { 
      dlerr = strdup(dlerror());
      errlog("Unable to load module %s: %s", modname, dlerr);
      free(dlerr);
      return -2;
   }
  
  /* Look for required module symbols */
  mversion =  dlsym (handle, "MVERSION");
  mod_info = (SVS_Module*) dlsym (handle, "mod_info"); 
  mod_load = dlsym (handle, "mod_load");
  mod_unload  = dlsym (handle, "mod_unload");
  mod_expire  = dlsym (handle, "mod_expire");  
  mod_rehash = dlsym (handle, "mod_rehash");
  mod_provides =  dlsym (handle, "mod_provides");
  mod_requires = dlsym (handle, "mod_requires");
  mod_options = dlsym (handle, "mod_options");
    
  if(IsNull(mversion))
    {
      errlog("No version on module %s", modname);
      return -2;  
    }
  if(*mversion != MODULES_VERSION)
    {
      errlog("Incompatible version on module %s, mod_vers= %i, we support %i",
      	modname, *mversion, MODULES_VERSION);
      return -3;
    }
  if(IsNull(mod_info))
    {
      errlog("No info on module %s", modname);
      return -2;
    }
    
  if(module_find(mod_info->name))
  {
    errlog("The module %s is already loaded", modname);
    return  -4;
  }

  CurrentModule = mod_info;
  
  if(!silent)
    stdlog(L_INFO,"Loading %s, ver %s",
      modname, mod_info->version);

  /* check for required action */
  if(IsNull(mod_load))    
    {
      errlog("ERROR: No load function on module");
      return -2;
    }       

 /* Check if options exist */
  if(mod_options)
    register_requires(mod_options, mod_info, 0);
  
  /* Check if all required functions exist */
  if(mod_requires)
    {
      if(register_requires(mod_requires, mod_info, 1)==-1)
        {
          return -3;
        }
    }
   
  if(mod_rehash)
  {
    result = ((int (*)(void)) mod_rehash)();
    if(result < 0)
    {
      stdlog(L_ERROR,"ERROR: Failed to rehash module (error=%d)", result);
      return -7;
    }
  }
    
  result = ((int (*)(void)) mod_load)();
  if(result < 0)
    {
      stdlog(L_ERROR,"ERROR: Failed to load module (error=%d)", result);
      return -8;
    } 

  /* This Module was succesfully loaded */
  svs_modules_h[modules_count] = handle;
  svs_modules[modules_count++] =  mod_info;


  /* register functions the module provide */
  if(mod_provides)
    register_provides(mod_provides, mod_info);

  /* load the .modules file if its found */
  return load_modules_file(modname);

}

/* Parse and load modules from .modules file */
int load_modules_file(char* fn)
{
  FILE *f;
  char line[128];
  char ffn[128];
  char *mod_name;
  
  snprintf(ffn, sizeof(ffn), ETCPATH "/%s.modules", fn);  
  f = fopen(ffn, "rt");  
  
  if(f == NULL)
    return 0;

  while(fgets(line, sizeof(line), f))
    {
      clean_conf_str(line);
      mod_name = line;
      if(mod_name && *mod_name)
        {
          int res = module_load(mod_name, 0);
          if(res < 0)
            return res;
        }    
    }
    
  return 0;
}

#if 0
/* add a command to a service module */
void module_add_cmd(SVS_Module *pmodule, char *cmd, void* func, char **summary, char **help)
{
  Module_cmd *modcmd = malloc(sizeof(Module_cmd));
  Module_cmd* m;  
  modcmd->cmd = strdup(cmd);
  modcmd->func = func; 
  modcmd->summary = summary;
  modcmd->help = help;
  modcmd->next = NULL;    

  if(IsNull(pmodule->cmds))
    {
      pmodule->cmds =  modcmd;
      return;
    }
  m = pmodule->cmds;
  
  while(m->next != NULL) /* lookup tail */
      m=m->next;
      
  m->next = modcmd;
}

void module_del_cmd(SVS_Module *pmodule, void* func)
{ 
}



/* add msg event handlers for all module commands */
void module_add_cmd_events(SVS_Module *pmodule, IRC_User *u)
{
  Module_cmd *modcmd = pmodule->cmds;
  while(modcmd)
    {
      irc_AddUMsgEvent(u, modcmd->cmd, modcmd->func);
      modcmd = modcmd->next;
    }
}

#endif

SVS_Module*  module_find(char *modname)
{
  int i;
  for(i=0; i < modules_count; ++i)
    if(strcasecmp(svs_modules[i]->name, modname) == 0)
      return svs_modules[i];
  return NULL;
}
/* register list of functions provided by the current module (last) 
   for functions with ev* we will call the register event instead
*/
void register_provides(Module_Function funclist[], SVS_Module *modinfo)
{
  int i = 0;
  Module_Function *mf;
  FunctionList *fl = NULL;
 
  mf = &funclist[i];
  while(mf->name)
    {
      if(strncmp(mf->name,"e_",2) == 0) /* its an event */
        {        
          *((int*)mf->ptr) = mod_register_event(mf->name);
          mf = &funclist[++i];
          continue;
        }
      fl = malloc(sizeof(FunctionList)); 
      bzero(fl,sizeof(FunctionList));        
      fl->name = mf->name;
      fl->ptr = mf->ptr;
      fl->modinfo = modinfo;
      fl->modattach = NULL; /* init list of attached modules */
      fl->next = provides_list;
      mf = &funclist[++i];   
      provides_list = fl;     
    }  
}

/* attaches all functions from dependencies table
   Returns:
   	-1 Some dependency failed
   	>0 count of resolved dependencies
 */
int register_requires(Module_Function moddeps[], SVS_Module *modinfo, int stop)
{
  int i;
  Module_Function* mf;
  void *func;
  
  i = 0;
  mf = &moddeps[0];
  while(mf->name)
  {
    if(strncmp(mf->name,"e_",2) == 0) /* its an event*/
    {
      int r = mod_event_handle(mf->name);
      if(r==-1)
      {
        if(stop)
        {
          errlog("ERROR: Event %s was not found", mf->name);
          return -1;              
        }
      } 
      *((int*)mf->ptr) = r;
    } 
    else
    {
      /* try to attach to function */
      func = attach_to_function(mf->name, modinfo);
      if(IsNull(func))
      {
        if(stop)
        {
          errlog("ERROR: Function %s was not found", mf->name);
          return -1;
        }
      }
      *(void**) mf->ptr = func;
    }
    mf=&moddeps[++i];      
  }
  
  return i;
}



/* attach to a function from functions list
  Returns: Function pointer
 	  -NULL - Function not found
 On success will add call module to attached list
 and add modinfo to the function's attached modules list
*/ 	 
void* attach_to_function(char *name, SVS_Module *modinfo)
{
  FunctionList *fl;
  fl = provides_list;

  while(fl && strcasecmp(fl->name, name)) /* search on the list */
      fl = fl->next;
  if(fl) /* the function name matches, we found the function */
    {
      fl->modattach = realloc(fl->modattach, (fl->attcount+1)*sizeof(SVS_Module*));
      fl->modattach[fl->attcount] = modinfo;
      fl->attcount++;
      return fl->ptr;
    }
  return NULL;
}

/* delete all actions attached from the module */
void module_delall_attach(SVS_Module *module)
{
  FunctionList *fl, *prev, *next;
  int i;
  
  fl = provides_list;
  prev = NULL;
  while(fl)
    {
      /* first delete all attachs */
      for(i = 0; i< fl->attcount; ++i)
        {
          if(fl->modattach[i] == module)
            {
              fl->modattach[i]=fl->modattach[fl->attcount-1];
              fl->attcount--;
              fl->modattach = realloc(fl->modattach, fl->attcount*sizeof(SVS_Module*));
            }
        }          
      if(fl->modinfo == module) /* its a function we provide */
        {
          if(fl->attcount > 0) /* this should never happen ! */
            abort();
          next = fl->next;
          if(prev)
            prev->next = next;
          else
            provides_list = next;
          free(fl);
          fl = next;          
        }
      else
        {
          prev = fl;
          fl = fl->next;
        }
    }
}


int check_for_functions(SVS_Module *modinfo, char ** fname, char **modname)
{
  FunctionList *fl;
  fl = provides_list;
  while(fl)
    {
      if(fl->modinfo == modinfo && fl->attcount>0)
      {
        *fname = fl->name;
        *modname = fl->modattach[0]->name;
        return -1;
      }
      fl=fl->next;      
    }
  return 0;
}

/* Unload a module
 *   Check will first if module has other functions attached to it
 * 
 */
int module_unload(SVS_Module* module)
{
  int i, i2;
  void* mod_unload;
  void* handle;
  int result;
  
  /* first search the module index */
  for(i = 0; i < modules_count; ++i)
  {
    if(svs_modules[i] == module)
      break;
  }
    
  if(i == modules_count) /* module not found ?? This should never happen */
    {
      errlog("Unload module not found");
      return 0;
    }
  handle = svs_modules_h[i];
  mod_unload = dlsym (handle, "mod_unload");
  
  stdlog(L_WARN, "Unloading module %s", module->name);
  result = ((int (*)(void)) mod_unload)();
  
  if(result < 0)
    {
      stdlog(L_ERROR,"ERROR: There was an error unloading module (error=%d)", result);
    }

  /* delete all actions attached from this module */
  module_delall_attach(module);
  /* delete all event actions attached from this module */  
  mod_del_all_mod_events(module);
  
  dlclose(handle);
  /* just swap the deleted module with the last one */
  for(i2 = i; i2 < modules_count-1; ++i2)
    { 
      svs_modules[i2] = svs_modules[i2+1];  
      svs_modules_h[i2] = svs_modules_h[i2+1];
    }
  --modules_count;

  if (modules_count < 0) /* this should never happen ! -blackfile */
      modules_count = 0;

  stdlog(L_INFO, "Module unloaded.");
    
  return 0;
}

/* modules_rehash
 * call the mod_rehash function on every loaded module
 */
void modules_rehash(void)
{
  int i;
  int result;
  void *mod_rehash;
  
  for(i = 0; i < modules_count; ++i)
  {
  	mod_rehash = dlsym (svs_modules_h[i], "mod_rehash");
  	if(mod_rehash)
  	  result = ((int (*)(void)) mod_rehash)();
	}
  return;
}


