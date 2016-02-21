/**********************************************************************
 * PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                     http://software.pt-link.net                    *
 * This program is distributed under GNU Public License               *
 * Please read the file COPYING for copyright information.            *
 **********************************************************************

  Description: module template

 *  $Id: template.c,v 1.5 2005/10/11 16:13:06 jpinto Exp $
*/

#include "module.h"

SVS_Module mod_info =
 /* module, version, description */
{"template", "1.0",  "just a template module" };

/** functions/events we require **/
/* void (*FunctionPointer)(void);*/

MOD_REQUIRES
  /* MOD_FUNC(FunctionName) 
  ...
  */
MOD_END

/** functions/events we provide **/
/* void my_function(void); */

MOD_PROVIDES
  /* MOD_FUNC(FunctionName) 
  ...
  */
MOD_END

/** Internal functions declaration **/
/* void internal_function(void); */

/** Local variables **/
/* int my_local_variable; */
   
/** rehash code **/
/* this function is called before mod_load and when services are rehashed  */ 
int mod_rehash(void)
{
  return 0;
}

/** load code **/
int mod_load(void)
{
  return 0;
}
    
/** unload code **/
void mod_unload(void)
{
}
    
/** internal functions implementation starts here **/
    
/* End of module */
