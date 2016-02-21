
DROP TABLE IF EXISTS memoserv;
CREATE TABLE memoserv(
  id INT UNSIGNED NOT NULL,
  owner_snid INT UNSIGNED NOT NULL,
  sender_snid INT UNSIGNED NULL,
  sender_name varchar(32) NULL,
  flags INT UNSIGNED NOT NULL,
  t_send INT NOT NULL,
  message VARCHAR(255) NOT NULL,
  PRIMARY KEY (owner_snid, id),
  INDEX(sender_snid),
  CONSTRAINT FK_MS1 FOREIGN KEY (owner_snid) REFERENCES nickserv (snid)
    ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT FK_MS2 FOREIGN KEY (sender_snid) REFERENCES nickserv (snid)
    ON DELETE SET NULL ON UPDATE CASCADE
) Type = InnoDB;

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

