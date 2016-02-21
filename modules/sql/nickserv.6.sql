# $Id: nickserv.6.sql,v 1.2 2005/09/10 18:14:34 jpinto Exp $

# Add t_expire and publichost
ALTER TABLE nickserv
  ADD COLUMN vhost varchar(64) default NULL AFTER publichost,
  ADD COLUMN favlink varchar(64) default NULL AFTER url;
