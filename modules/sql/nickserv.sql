# $Id: nickserv.sql,v 1.12 2005/10/29 11:22:29 jpinto Exp $
SET FOREIGN_KEY_CHECKS = 0;

DROP TABLE IF EXISTS nickserv;
CREATE TABLE nickserv (
  snid INT UNSIGNED NOT NULL auto_increment,
  nick varchar(32) BINARY NOT NULL,
  t_reg INT NOT NULL,
  t_ident INT NOT NULL,
  t_seen INT NOT NULL,
  t_expire INT NOT NULL,
  email varchar(64) default NULL,
  url varchar(64) default NULL,
  favlink varchar(64) default NULL,
  imid varchar(64) default NULL,
  location varchar(64) default NULL,
  ontime INT UNSIGNED NOT NULL default '0',
  vhost varchar(64) default NULL,
  status int NOT NULL default '0',
  flags int NOT NULL default '0',
  lang int NOT NULL default '0',
  master_snid INT UNSIGNED NOT NULL default '0',  
  PRIMARY KEY  (snid),
  UNIQUE KEY nick (nick)
) Type = InnoDB;

DROP TABLE IF EXISTS nickserv_security;
CREATE TABLE nickserv_security (
  snid INT UNSIGNED NOT NULL,
  pass varchar(32) default NULL,
  securitycode varchar(32) default NULL,
  question varchar(128) default NULL,
  answer varchar(128) default NULL,
  t_lset_pass INT UNSIGNED NOT NULL default '0',
  t_lset_answer INT UNSIGNED NOT NULL default '0',
  t_lauth INT UNSIGNED NOT NULL default '0',
  PRIMARY KEY  (snid),
  CONSTRAINT FK_NS1 FOREIGN KEY (snid) REFERENCES nickserv (snid)
    ON DELETE CASCADE ON UPDATE CASCADE
) Type = InnoDB;

DROP TABLE IF EXISTS nickserv_suspensions;
CREATE TABLE nickserv_suspensions
(
  snid INT UNSIGNED NOT NULL,
  who varchar(32) NOT NULL,
  t_when INT UNSIGNED NOT NULL,
  duration INT UNSIGNED NOT NULL,
  reason VARCHAR(128) NOT NULL,
  PRIMARY KEY  (snid),
  CONSTRAINT FK_NSS1 FOREIGN KEY (snid) REFERENCES nickserv (snid)
    ON DELETE CASCADE ON UPDATE CASCADE
) Type = InnoDB;

SET FOREIGN_KEY_CHECKS = 1;
