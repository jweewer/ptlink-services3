/*
 * CHANGES  - Change log for the current release
 *  $Id: 3.3.x,v 1.1.1.1 2005/08/27 15:45:09 jpinto Exp $
 */


  PTlink IRC Services 3.3.0 (7 Aug 2005)
==================================================================

  What is new ?
  -------------
  0000287: ns_getpass and ns_getsec to recover password and security code
  0000285: os_globalmsg to send global with private message
  0000281: No authenticated nicks can't use chanserv commands
  0000279: nickserv list support for flags (auth, noexpire, forbidden ...)
  0000278: secureops option on chanserv
  0000277: ns_blacklist to support email blacklists
  0000276: nick password expire option
  0000261: mlock option


  What has been changed/fixed ?
  -----------------------------
  0000294: bot hostrule parameter incorrectly converted to time
  0000293: invalid sql string on cs_akick list with mask
  0000290: send_lang using invalid parameters
  0000284: irc opers should not be kicked from restricted/forbidden chans
  0000283: chanserv forbidden has no effect
  0000282: akick mask is not on the standard nick!user@host format
  0000274: empty options field on ns info for authenticated nicks
  0000273: +a only set on sadmins if OperChan is defined
  0000272: move nickserv security info to a specific table
  0000270: topiclock flag is not correct for the cs info
  0000265: remove nickserv cache system
  0000280: akick expire date now displayed on list
