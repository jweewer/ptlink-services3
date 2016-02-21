# $Id: nickserv.8.sql,v 1.4 2005/10/29 11:22:29 jpinto Exp $

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
) Type = InnoDB;
