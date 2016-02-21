# Add the new time
DROP TABLE IF EXISTS memoserv_options;
CREATE TABLE memoserv_options(
  snid INT UNSIGNED NOT NULL,
  maxmemos INT UNSIGNED NOT NULL,
  bquota INT UNSIGNED NOT NULL,
  flags INT UNSIGNED NOT NULL,
  PRIMARY KEY (snid),
  CONSTRAINT FK_MS3 FOREIGN KEY(snid) REFERENCES nickserv (snid)
        ON DELETE CASCADE ON UPDATE CASCADE
) Type = InnoDB;

# Set all previous memos to saved
UPDATE memoserv SET flags = flags | 2;
