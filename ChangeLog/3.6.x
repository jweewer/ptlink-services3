/*
 * CHANGES  - Change log for the current release
 *  $Id: 3.6.x,v 1.2 2005/11/03 22:21:38 jpinto Exp $
 */

(.1) 3 Nov 2005

  What has been changed/fixed ?
  -----------------------------
  Major security fix
  #45: invalid time when using 'Y' time letter
  #44: invalid string on email sender


  PTlink IRC Services 3.6.0 (18 Oct 2005)
==================================================================

  What is new ?
  -------------
  #27 : half-op support
  #25 : os_sysuptime.c based on the anope module
  #19 : DefaultLang configuration item is required
  #11 : Finish the web nick registration form
  #2  : ns_last module to record/display nicks login activity
  #1  : Replace NS AUTOJOIN with CS AJOIN ADD/DEL

  What has been changed/fixed ?
  -----------------------------
  #33 : main ircsvs log rotation
  #34 : libircservice is not sending SVINFO/SVSINFO
  #28 : avoid unknown table warning when creating the database
  #24 : move e_nick_register event registration to nickserv
  #22 : e_nick_recognize to distinguish +r nick recognitions
  #21 : remove unused/moved fields from nickserv table
  #15 : kill -HUP must call mod_rehash() on modules
