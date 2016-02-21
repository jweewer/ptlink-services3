# $Id: nickserv.3.sql,v 1.1.1.1 2005/08/27 15:44:48 jpinto Exp $

# Add t_expire and publichost
ALTER TABLE nickserv
  CHANGE COLUMN t_sign t_expire INT NOT NULL,
  ADD COLUMN publichost varchar(64) default NULL AFTER realhost;
# Now update publichost from realhost
UPDATE nickserv SET publichost=realhost;
