/*
 * CHANGES  - Change log for the current release
 *  $Id: 3.1.x,v 1.1.1.1 2005/08/27 15:45:09 jpinto Exp $
 */

  PTlink IRC Services 3.1.0 (26 Jun 2005)
==================================================================

  What is new ?
  ---------------------
  0000243: protected nick set option
  0000237: RestrictedChans option (to match the ircd option) 
  0000218: NickDefaultOptions to set nick default options
  0000210: os_raw module
  0000209: os_mode module
  0000208: ns_list module
  0000206: os_kick module
  0000204: option to allow users to accept/reject when added to roles
  0000203: services admins can sdrop channel without using security code
  0000202: ns_sdrop function to drop nicks without code for sadmins
  0000201: new module for random quotes
  0000200: AgeBonus - An option to give older nicks expire extension  

  What has been fixed ?
  ---------------------
  0000245: use snid to track previous identify
  0000244: login command to work for nick protection and ghost
  0000239: fresh install producing "cs_role_temp" warning
  0000238: Duplicate language field on nickrecord
  0000233: potential buffer overrun at mask_string() function
  0000230: cs_role() potential NULL pointer
  0000220: fixed up the name and version when the versions are wrong
  0000229: The cs_voice is not supporting the OpNotice option
  0000228: Server description is now used from the dconf file
  0000224: SSET on password send to log channel and nickserv notice
  0000221: call the nick_identify event on ghost
  0000213: role help permission options help completed
  0000212: sline mask has been increased from 32 to 128 chars
  0000205: GCC4 warnings fixed
  0000199: Change datetime fields to integers
  0000198: Memo's will have per user memo ids
