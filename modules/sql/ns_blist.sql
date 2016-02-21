# $Id: ns_blist.sql,v 1.1.1.1 2005/08/27 15:44:48 jpinto Exp $

DROP TABLE IF EXISTS ns_blist;
CREATE TABLE ns_blist (
  data varchar(32) NOT NULL,
  PRIMARY KEY  (data)
) Type = InnoDB;