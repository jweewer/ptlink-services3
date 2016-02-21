# $Id: ns_group.sql,v 1.5 2005/10/04 16:31:48 jpinto Exp $
SET FOREIGN_KEY_CHECKS = 0;
DROP TABLE IF EXISTS ns_group;
CREATE TABLE ns_group(
  sgid int(3) unsigned NOT NULL auto_increment,
  name varchar(128) NOT NULL,
  master_sgid int(3) unsigned NULL,
  gdesc varchar(128) NULL,
  autoumodes VARCHAR(64) NULL,
  maxusers int(4) NOT NULL,
  PRIMARY KEY (sgid),
  UNIQUE KEY name (name)
) Type = InnoDB;

DROP TABLE IF EXISTS ns_group_users;
CREATE TABLE ns_group_users(
  sgid int(3) unsigned NOT NULL,
  snid INT UNSIGNED NOT NULL,
  t_expire INT NOT NULL DEFAULT '0',
  PRIMARY KEY(snid, sgid),
  INDEX(sgid),
  CONSTRAINT FK_GRU1 FOREIGN KEY (sgid) REFERENCES ns_group (sgid)
    ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT FK_GRU2 FOREIGN KEY (snid) REFERENCES nickserv (snid)
    ON DELETE CASCADE ON UPDATE CASCADE
) Type = InnoDB;

SET FOREIGN_KEY_CHECKS = 1;
