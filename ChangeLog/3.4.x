/*
 * CHANGES  - Change log for the current release
 *  $Id: 3.4.x,v 1.2 2005/09/24 19:23:00 jpinto Exp $
 */
(.2) 24 Sep 2004

  What is new ?
  -------------
  0000321: HelpChan option to set +h when ops join the help chan

  What has been changed/fixed ?
  -----------------------------
  0000335: --with-pidfile option to specify pidfile path
  0000339: attempt to change the admin role crashes services


(.1) 7 Sept 2005

  What has been changed/fixed ?
  -----------------------------
  0000323: os_hostrule not working with some rule types
  0000322: cs_role crash when upgrading from ptsvs2 database

  PTlink IRC Services 3.4.0 ( 4 Sep 2005 )
==================================================================

  What is new ?
  ------------- 
  0000316: MemoServ DEL ALL to delete all memos
  0000308: group names can have "@server" for "from server" restriction
  0000307: support for auto user modes on groups
  0000305: foreign keys for data integrity
  0000299: os_sysstats to log system resources usage
  0000291: LocalAddress for connection binding
  0000298: userhost is now displayed on nickserv info
  0000302: default mlock modes can be set on config for register
  0000304: last seen time only on info when nick is offline
  0000303: noexpire nicks don't have an expire time on the nickserv info
  0000312: chanserv list command for listing channels by given mask

  What has been changed/fixed ?
  -----------------------------
  0000320: chanserv is not updating memory records during nick delete
  0000318: array_find_str must be case insensitive
  0000317: ChanServ doesn't keep on the logchan when RestrictedChans is enabled
  0000311: non registered nicks can bypass topiclock
  0000309: HOSTULE ADD syntax changed to ADD TYPE HOST [+param] Message
  0000306: nickserv info shows nick as private everytime it is online
  0000300: aglines not properly set
  0000297: userlog crash on module unload
  0000295: mlock support for +k,+l,+f
