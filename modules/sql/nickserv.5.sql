# $Id: nickserv.5.sql,v 1.1.1.1 2005/08/27 15:44:48 jpinto Exp $

# Convert table type
ALTER TABLE nickserv_security Type=InnoDB;
# Add foreign key
ALTER TABLE nickserv_security ADD
  CONSTRAINT FK_NS1 FOREIGN KEY (snid) REFERENCES nickserv (snid)
    ON DELETE CASCADE ON UPDATE CASCADE;
