# $Id: ns_photo.sql,v 1.1.1.1 2005/08/27 15:44:49 jpinto Exp $

DROP TABLE IF EXISTS ns_photo;
CREATE TABLE ns_photo(
  id INT UNSIGNED NOT NULL auto_increment,
  snid INT UNSIGNED NOT NULL,
  t_update datetime NOT NULL,
  status char NOT NULL default 'P',
  photo MEDIUMBLOB,
  thumb BLOB,
  hits INT NOT NULL,
  PRIMARY KEY (id),
  UNIQUE KEY(snid),
  CONSTRAINT FK_NSP1 FOREIGN KEY (snid) REFERENCES nickserv (snid)
    ON DELETE CASCADE ON UPDATE CASCADE
) Type = InnoDB;
