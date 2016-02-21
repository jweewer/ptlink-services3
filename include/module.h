/******************************************************************
 * PTlink Services is (C) CopyRight PTlink IRC Software 1999-2005 *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License           *
 * Please read the file COPYING for copyright information.        *
 ******************************************************************

  File: module.h
  Description: just the standard includes for modules

 *  $Id: module.h,v 1.2 2005/10/11 16:13:06 jpinto Exp $
*/

#include "modules.h"
#include "stdinc.h"
#include "ircservice.h"
#include "suser.h"
#include "s_log.h"
#include "log.h"
#include "lang.h"
#include "modules.h"
#include "strhand.h"
#include "modevents.h"
#include "ircsvs.h"
#include "array.h"
#include "sqlb.h"

/* default module functions */
int mod_rehash(void);
int mod_load(void);
void mod_unload(void);

/* this is the module version to be included one module */
const int MVERSION = MODULES_VERSION;
