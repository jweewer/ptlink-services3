# $Id: ns_group.2.sql,v 1.1.1.1 2005/08/27 15:44:48 jpinto Exp $
# The primary key was missing
ALTER TABLE ns_group
  DROP KEY sgid,
  DROP KEY name,
  ADD UNIQUE KEY(name),
  ADD COLUMN autoumodes VARCHAR(64) NULL AFTER gdesc,
  CHANGE master_sgid master_sgid int(3) UNSIGNED NULL,
  CHANGE gdesc gdesc varchar(128) NULL,
  CHANGE name name varchar(128) NOT NULL;

UPDATE ns_group 
  SET master_sgid = NULL WHERE master_sgid = 0;

ALTER TABLE ns_group_users
  CHANGE snid snid INT UNSIGNED NOT NULL,
  ADD PRIMARY KEY(snid, sgid),
  ADD INDEX(sgid);
# Add the foreign keys
ALTER TABLE ns_group_users
ADD CONSTRAINT FK_GRU1 FOREIGN KEY (sgid) REFERENCES ns_group (sgid)
    ON DELETE CASCADE ON UPDATE CASCADE;
ALTER TABLE ns_group_users
ADD  CONSTRAINT FK_GRU2 FOREIGN KEY (snid) REFERENCES nickserv (snid)
    ON DELETE CASCADE ON UPDATE CASCADE;
