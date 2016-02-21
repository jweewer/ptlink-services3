# $Id: nickserv.4.sql,v 1.1.1.1 2005/08/27 15:44:48 jpinto Exp $

# We need this create table here because of the nickserv upgrade
DROP TABLE IF EXISTS nickserv_security;
CREATE TABLE nickserv_security (
  snid INT UNSIGNED NOT NULL,
  pass varchar(32) default NULL,
  securitycode varchar(32) default NULL,
  question varchar(128) default NULL,
  answer varchar(128) default NULL,
  t_lset_pass INT UNSIGNED NOT NULL default '0',
  t_lset_answer INT UNSIGNED NOT NULL default '0',
  t_lauth INT UNSIGNED NOT NULL default '0',
  PRIMARY KEY  (snid)
);

# Move the data from one table to another
INSERT INTO nickserv_security 
  SELECT snid, pass, securitycode, NULL , NULL, 0, 0, 0 FROM nickserv;

# Delete old columns
ALTER TABLE nickserv
  DROP COLUMN pass,
  DROP COLUMN securitycode;
