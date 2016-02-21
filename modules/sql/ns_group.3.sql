# $Id: ns_group.3.sql,v 1.2 2005/09/10 18:14:34 jpinto Exp $

# Add maxusers;
ALTER TABLE ns_group
	ADD COLUMN maxusers int(4) NOT NULL AFTER autoumodes;

# Add t_expire and maxusers
ALTER TABLE ns_group_users
	ADD COLUMN t_expire INT NOT NULL DEFAULT '0' AFTER snid;
