# $Id: botserv.sql,v 1.6 2005/10/05 11:12:39 jpinto Exp $
SET FOREIGN_KEY_CHECKS = 0;

DROP TABLE IF EXISTS botserv;
CREATE TABLE botserv (
  bid INT UNSIGNED NOT NULL auto_increment,
  owner_snid INT UNSIGNED NOT NULL,
  nick varchar(32) NOT NULL default '',
  username varchar(32) NOT NULL,
  publichost varchar(64) NOT NULL,
  realname varchar(64) NOT NULL,
  t_create int(11) NOT NULL default '0',
  t_expire int(11) NOT NULL default '0',
  PRIMARY KEY (bid),
  INDEX (owner_snid),
  CONSTRAINT FK_BS  FOREIGN KEY (owner_snid) REFERENCES nickserv (snid)
	ON DELETE CASCADE ON UPDATE CASCADE
) Type = InnoDB;

DROP TABLE IF EXISTS botserv_chans;
CREATE TABLE botserv_chans(
  scid INT UNSIGNED NOT NULL,
  bid INT UNSIGNED NOT NULL,
  PRIMARY KEY(scid, bid),
  INDEX(bid),
  CONSTRAINT FK_BSC1 FOREIGN KEY (scid) REFERENCES chanserv (scid)
    ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT FK_BSC2  FOREIGN KEY (bid) REFERENCES botserv (bid)
    ON DELETE CASCADE ON UPDATE CASCADE
) Type = InnoDB;

SET FOREIGN_KEY_CHECKS = 1;
