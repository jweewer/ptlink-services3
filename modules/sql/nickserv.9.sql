DROP TABLE IF EXISTS nickserv_suspensions;
CREATE TABLE nickserv_suspensions
(
  snid INT UNSIGNED NOT NULL,
  who varchar(32) NOT NULL,
  t_when INT UNSIGNED NOT NULL,
  duration INT UNSIGNED NOT NULL,
  reason VARCHAR(128) NOT NULL,
  PRIMARY KEY  (snid),
  CONSTRAINT FK_NSS1 FOREIGN KEY (snid) REFERENCES nickserv (snid)
    ON DELETE CASCADE ON UPDATE CASCADE
) Type = InnoDB;

# Import old forbiddens
INSERT INTO 
  nickserv_suspensions(snid, who, t_when,duration, reason)
SELECT 
  snid, 'import', UNIX_TIMESTAMP(), 0, 'Was forbidden' 
FROM 
  nickserv WHERE flags & 2;

# Update previous forbidden flag
UPDATE nickserv SET flags = flags | 2 WHERE flags & 0x80;
UPDATE nickserv SET flags = flags & ~0x80 WHERE flags & 0x80;
