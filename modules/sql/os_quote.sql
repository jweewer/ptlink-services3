# $Id: os_quote.sql,v 1.1.1.1 2005/08/27 15:44:48 jpinto Exp $

DROP TABLE IF EXISTS os_quote;
CREATE TABLE os_quote(
  id INT UNSIGNED NOT NULL auto_increment,
  who VARCHAR(32) NOT NULL,
  quote TEXT NOT NULL,
  PRIMARY KEY  (id)
) Type = InnoDB;
