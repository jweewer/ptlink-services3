# $Id: os_sline.sql,v 1.1.1.1 2005/08/27 15:44:48 jpinto Exp $

DROP TABLE IF EXISTS os_sline;
CREATE TABLE os_sline(
  id INT UNSIGNED NOT NULL auto_increment,
  letter CHAR(1) NOT NULL,
  who_nick VARCHAR(32) NOT NULL,
  mask VARCHAR(128) NOT NULL,
  t_create DATETIME NOT NULL,
  message VARCHAR(128) NOT NULL,  
  PRIMARY KEY (id),
  KEY id (id)
) Type = InnoDB;
