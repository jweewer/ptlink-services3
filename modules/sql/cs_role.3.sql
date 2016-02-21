# $Id: cs_role.3.sql,v 1.1.1.1 2005/08/27 15:44:48 jpinto Exp $

# Change column to allow null values and add the required indexes
ALTER TABLE cs_role
  CHANGE master_rid master_rid INT UNSIGNED NULL,
  ADD INDEX(master_rid);
UPDATE cs_role
  SET master_rid = NULL WHERE master_rid = 0;

# Add the foreign keys
ALTER TABLE cs_role
  ADD CONSTRAINT FK_CSR1 FOREIGN KEY (scid) REFERENCES chanserv (scid) 
    ON DELETE CASCADE ON UPDATE CASCADE;
ALTER TABLE cs_role
  ADD CONSTRAINT FK_CSR2 FOREIGN KEY (master_rid) REFERENCES cs_role (rid)
    ON DELETE SET NULL ON UPDATE CASCADE;

# Change column to allow null values and add the required indexes
ALTER TABLE cs_role_users
  CHANGE who who  INT UNSIGNED NULL,
  ADD INDEX(who),
  ADD INDEX(rid);

UPDATE cs_role_users
  SET who=NULL WHERE who = 0;

# There was some bug which caused this
DELETE FROM cs_role_users WHERE snid = 0;

# Add the foreign keys
ALTER TABLE cs_role_users
  ADD CONSTRAINT FK_CSRU1 FOREIGN KEY (rid) REFERENCES cs_role (rid)
    ON DELETE CASCADE ON UPDATE CASCADE;
ALTER TABLE cs_role_users
  ADD CONSTRAINT FK_CSRU2 FOREIGN KEY (snid) REFERENCES nickserv (snid)
    ON DELETE CASCADE ON UPDATE CASCADE;
ALTER TABLE cs_role_users
  ADD CONSTRAINT FK_CSRU3 FOREIGN KEY (who) REFERENCES nickserv (snid)
    ON DELETE SET NULL ON UPDATE CASCADE;
ALTER TABLE cs_role_users
  ADD CONSTRAINT FK_CSRU4 FOREIGN KEY (scid) REFERENCES chanserv (scid)
    ON DELETE CASCADE ON UPDATE CASCADE;
