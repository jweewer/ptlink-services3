# $Id: cs_role.2.sql,v 1.1.1.1 2005/08/27 15:44:48 jpinto Exp $

# Add an unique key to scid, name
ALTER TABLE cs_role
	ADD UNIQUE KEY(scid, name);

# Add a primary key on scid, snid and a status field
ALTER TABLE cs_role_users
	ADD PRIMARY KEY(scid, snid),
	ADD UNIQUE KEY (snid, rid),
	ADD COLUMN flags INT NOT NULL;
