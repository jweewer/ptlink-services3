/*
 * CHANGES  - Change log for the current release
 *  $Id: 3.5.x,v 1.2 2005/10/16 17:31:32 jpinto Exp $
 */

(.1) 16 Oct 2005

  What has been changed/fixed ?
  -----------------------------
  #30 : users can drop the admin role
  #29 : possible sql injection on cs_role
  #23 : e_nick_identify is not called on ns_login when the nick is online
  #18 : bots are not getting chanmode +o on join
  #16 : autoop isn't working when joing a channel
  #14 : setting OperChan is not optional as it should
  #12 : sset vhost should check for a valid hostname

  PTlink IRC Services 3.5.0 (8 Oct 2005)
==================================================================

  What is new ?
  -------------
  0000340: WelcomeChan option to make new users join a channel
  0000337: Option to set maxusers on groups 
  0000336: ability to insert references to prior strings on lang items
  0000331: recordstats module to save daly nick/chan records balance
  0000330: nickserv links exchange option
  0000329: option to set a favorite link (to be used for links exchange)
  0000328: group members can be added with a given expire time
  0000327: sadmins SSET nick vhost to set a virtual hostname
  0000315: BotServ module created, basic functions available
  Added top links display on the web interface

  What has been changed/fixed ?
  -----------------------------
  0000352: nickserv login is not informing the user with "Password accepted" 
  0000351: cs_clear not enforcing mlock
  0000350: ns_group subcommands detailed help
  0000349: os_sline should support list by line type
  0000344: move the dconf items to databased managed configuration
  0000342: mlock should be enforced by chanserv
  0000338: when the targer nick is used, login should behave like ns_identify
  0000334: review code to use local bot when possible
  0000332: ns_register missing call for the e_nick_register event 
  0000288: Ignore messages with $* as target
