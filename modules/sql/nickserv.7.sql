# $Id: nickserv.7.sql,v 1.3 2005/10/14 18:50:00 jpinto Exp $

# Drops columns we don't need anymore
ALTER TABLE nickserv
  DROP COLUMN username,
  DROP COLUMN realhost,
  DROP COLUMN publichost,
  DROP COLUMN info,
  DROP COLUMN nmask,
  DROP COLUMN ajoin;
