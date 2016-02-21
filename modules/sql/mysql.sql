# $Id: mysql.sql,v 1.1.1.1 2005/08/27 15:44:49 jpinto Exp $

DROP TABLE IF EXISTS ircsvs_tables;
CREATE TABLE ircsvs_tables(
  name varchar(32) NOT NULL,
  version INT UNSIGNED NOT NULL,
  inst_time datetime NOT NULL
) Type = InnoDB;
