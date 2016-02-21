/*
 * CHANGES  - Change log for the current release
 *  $Id: CHANGES,v 1.47 2005/12/11 16:51:17 jpinto Exp $
 */

(.1) 23 Dec 2005

  What has been changed/fixed ?
  -----------------------------
  Added missing is_soper() import on the sessionlimit module
  Set +B on users connecting with exception

  PTlink IRC Services 3.7.0 (11 Dec 2005)
==================================================================

  What is new ?
  -------------
  #50: nickserv login option to override nick language
  #8: os_session with better session handling

  What has been changed/fixed ?
  -----------------------------
  #47: all core directories creation/setup must be configurable
  #46: missing foreign key relations on channels and nicks
  #40: bs_create should use nick2snid to get the nick snids
  #36: CS AJOIN will INVITE users if there's enough permission
  #35: ability to set order on ajoins
