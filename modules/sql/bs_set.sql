# $Id: bs_set.sql,v 1.2 2005/10/04 16:31:48 jpinto Exp $
SET FOREIGN_KEY_CHECKS = 0;

DROP TABLE IF EXISTS bs_set;
CREATE TABLE bs_set (
  scid int(10) unsigned NOT NULL,
  bid int(10) unsigned default NULL,
  flags int(11) NOT NULL default '0',
  kick int(11) NOT NULL default '0',
  ttb int(11) NOT NULL default '0',
  capsmin int(11) NOT NULL default '0',
  capspercent int(11) NOT NULL default '0',
  floodlines int(11) NOT NULL default '0',
  floodsecs int(11) NOT NULL default '0',
  repeattimes int(11) NOT NULL default '0',
  bantype int(2) NOT NULL default '0',
  banlast int(11) NOT NULL default '0',
  INDEX (bid),
  INDEX (scid),
  FOREIGN KEY (scid) REFERENCES chanserv (scid) ON DELETE CASCADE ON UPDATE CAS
  FOREIGN KEY (bid) REFERENCES botserv (bid) ON DELETE CASCADE ON UPDATE CASCAD
) Type = InnoDB;

SET FOREIGN_KEY_CHECKS = 1;
