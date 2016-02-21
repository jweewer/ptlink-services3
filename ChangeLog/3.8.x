/*
 * CHANGES  - Change log for the current release
 */

(.1) 16 Jan 2006

  What has been changed/fixed ?
  -----------------------------
  #66, fixed cs hop/dehop status check
  #65: Fixed Chan Drop & BotServ
  #64: Fixed SecureOps & BotServ
  #63: validate_options() provided by strhand.c


  PTlink IRC Services 3.8.0 (2 Jan 2006)
==================================================================

  What is new ?
  -------------
  #61: MaxTimeForAuth setting to expire unauthenticated nicks
  #54: CS ROLE DEL ALL
  #53: CS AKICK DEL ALL
  #26: Added chanserv suspensions
  os_uevent module
  #56: USEMSG setting to force users to use messages instead of notices

  What has been changed/fixed ?
  -----------------------------
  #62: suspensions replace the forbid setting
  #60: ns_suspend must check and change the target if it is online
  #17: CS SHOW shoud display the pending status
  Added a notice presenting the autojoin list on login
