# $Id: chanserv.sql,v 1.3 2005/10/04 16:31:48 jpinto Exp $

SET FOREIGN_KEY_CHECKS = 0;
DROP TABLE IF EXISTS chanserv;
CREATE TABLE chanserv (
  scid INT UNSIGNED NOT NULL auto_increment,
  name varchar(32) BINARY NOT NULL,
  url varchar(64) default NULL,
  email varchar(64) default NULL,
  founder INT UNSIGNED NULL,
  successor INT UNSIGNED NULL,
  last_topic TEXT,
  last_topic_setter varchar(32) default NULL,
  t_ltopic INT NOT NULL,
  t_reg INT NOT NULL,
  t_last_use INT NOT NULL,
  mlock varchar(64) default NULL,
  status int(2) NOT NULL default '0',
  flags int(2) NOT NULL default '0',
  entrymsg varchar(255) default NULL,
  cdesc varchar(255) default NULL,
  t_maxusers INT NOT NULL,
  maxusers int(5) NOT NULL default '0',
  PRIMARY KEY (scid),
  UNIQUE KEY name (name),
  INDEX(founder),
  INDEX(successor),
  CONSTRAINT FK_CS1 FOREIGN KEY (founder) REFERENCES nickserv (snid)
    ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT FK_CS2 FOREIGN KEY (successor) REFERENCES nickserv (snid)
    ON DELETE SET NULL ON UPDATE CASCADE
) Type = InnoDB;

DROP TABLE IF EXISTS chanserv_suspensions;
CREATE TABLE chanserv_suspensions
(
  scid INT UNSIGNED NOT NULL,
  who varchar(32) NOT NULL,
  t_when INT UNSIGNED NOT NULL,
  duration INT UNSIGNED NOT NULL,
  reason VARCHAR(128) NOT NULL,
  PRIMARY KEY  (scid),
  CONSTRAINT FK_CSS1 FOREIGN KEY (scid) REFERENCES chanserv (scid)
    ON DELETE CASCADE ON UPDATE CASCADE
) Type = InnoDB;

SET FOREIGN_KEY_CHECKS = 1;
