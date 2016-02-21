/*
 * CHANGES  - Change log for the current release
 *  $Id: 3.2.x,v 1.1.1.1 2005/08/27 15:45:09 jpinto Exp $
 */

(.1)  21 Jul 2005

  What has been fixed ?
  ---------------------
  0000269: potential crash on ns set email
  0000267: chan options are not displayed on the info for founder
  0000266: ms_cancel deletes all unread memos of all users
  completed and fixed some typos on the pt langs
  fixed some buffer size length validations

  PTlink IRC Services 3.2.0 (19 Jul 2005)
==================================================================

  What is new ?
  ---------------------
  0000258: chanserv topiclock option
  0000256: ago time split in larger time units
  0000249: add nickserv REGAIN alias to LOGIN
  0000247: cache nick groups info
  0000246: group filter on help display 
  0000214: akick module

  What has been fixed ?
  ---------------------
  0000262: os_hostrule crashing on module unload
  0000254: rename cs_list to cs_show
  0000253: hostrule not being deleted after expire
  Added the missing code for issue 0000245
  0000250: change irc_lower_nick( and irc_lower_chan( to functions
  0000248: nick login log message using wrong string pointer
  0000242: finished the help set language file as far as possible
