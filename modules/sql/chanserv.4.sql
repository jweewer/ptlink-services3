DROP TABLE IF EXISTS chanserv_suspensions;
CREATE TABLE chanserv_suspensions
(
  scid INT UNSIGNED NOT NULL,
  who varchar(32) NOT NULL,
  t_when INT UNSIGNED NOT NULL,
  duration INT UNSIGNED NOT NULL,
  reason VARCHAR(128) NOT NULL,
  PRIMARY KEY  (scid),
  CONSTRAINT FK_CSS1 FOREIGN KEY (scid) REFERENCES chanserv (scid)
    ON DELETE CASCADE ON UPDATE CASCADE
) Type = InnoDB;

# Import old forbiddens
INSERT INTO
  chanserv_suspensions(scid, who, t_when,duration, reason)
SELECT
  scid, 'import', UNIX_TIMESTAMP(), 0, 'Was forbidden'
FROM
  chanserv WHERE flags & 2;
