# $Id: nickserv.sql,v 1.12 2005/10/29 11:22:29 jpinto Exp $
SET FOREIGN_KEY_CHECKS = 0;

DROP TABLE IF EXISTS user_events;
CREATE TABLE user_events (
  id INT UNSIGNED NOT NULL auto_increment,
  t_when INT NOT NULL DEFAULT '0',
  duration INT NOT NULL DEFAULT '0',
  ev_type INT UNSIGNED NOT NULL,
  ev_param VARCHAR(128) NULL,
  action_type INT NOT NULL,
  action_param TEXT NOT NULL,
  PRIMARY KEY(id)
) Type = InnoDB;

SET FOREIGN_KEY_CHECKS = 1;
