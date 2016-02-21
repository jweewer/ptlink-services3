# $Id: dbconf.sql,v 1.3 2005/10/01 08:34:28 jpinto Exp $

DROP TABLE IF EXISTS dbconf;
CREATE TABLE dbconf(
  module varchar(32) NOT NULL,
  name varchar(64) NOT NULL,
  stype enum('int', 'switch', 'time', 'word', 'str') NOT NULL,
  ddesc varchar(255) NOT NULL,
  optional enum ('y', 'n') NOT NULL default 'y',
  configured enum ('y','n') NOT NULL default 'n',  
  value varchar(255),
  PRIMARY KEY (module, name)
) Type = InnoDB;
