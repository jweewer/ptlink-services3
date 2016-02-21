# $Id: recordstats.sql,v 1.2 2005/10/01 08:13:10 waxweazle Exp $

DROP TABLE IF EXISTS recordstats;
CREATE TABLE recordstats (
  day DATE NOT NULL,
  ns_total INT UNSIGNED NOT NULL,
  ns_new_irc INT NOT NULL,
  ns_new_web INT NOT NULL,
  ns_lost INT NOT NULL,
  cs_total INT UNSIGNED NOT NULL,
  cs_new_irc INT NOT NULL,
  cs_new_web INT NOT NULL,
  cs_lost INT NOT NULL,
  PRIMARY KEY(day)
) Type = InnoDB;
