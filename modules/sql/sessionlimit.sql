# $Id: sessionlimit.sql,v 1.1 2005/10/27 22:27:02 jpinto Exp $

SET FOREIGN_KEY_CHECKS = 0;

DROP TABLE IF EXISTS session_exceptions;
CREATE TABLE session_exceptions
(
  hostmask VARCHAR(32) NOT NULL,
  who_snid INT UNSIGNED NULL,
  owner_snid INT UNSIGNED NULL,
  t_when INT NOT NULL DEFAULT '0',
  duration INT NOT NULL DEFAULT '0',
  slimit INT NOT NULL,
  reason VARCHAR(128) NOT NULL,
  PRIMARY KEY(hostmask),
  INDEX(who_snid),
  INDEX(owner_snid),
  CONSTRAINT FK_SE1 FOREIGN KEY (who_snid) REFERENCES nickserv (snid)
    ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT FK_SE2 FOREIGN KEY (owner_snid) REFERENCES nickserv (snid)
    ON DELETE CASCADE ON UPDATE CASCADE
) Type = InnoDB;

SET FOREIGN_KEY_CHECKS = 1;
