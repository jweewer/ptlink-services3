# $Id: ns_photo.2.sql,v 1.1.1.1 2005/08/27 15:44:49 jpinto Exp $

ALTER TABLE ns_photo 
  ADD CONSTRAINT FK_NSP1 FOREIGN KEY (snid) REFERENCES nickserv (snid)
    ON DELETE CASCADE ON UPDATE CASCADE;
