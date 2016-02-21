# $Id: memoserv.2.sql,v 1.1.1.1 2005/08/27 15:44:49 jpinto Exp $

# Add temporary columns for conversion
ALTER TABLE memoserv
	DROP INDEX smid,
	CHANGE smid id INT UNSIGNED NOT NULL,
	DROP PRIMARY KEY,
	ADD PRIMARY KEY(owner_snid, id),
	ADD INDEX(sender_snid),
	ADD COLUMN t_send_tmp INT AFTER t_send;

# Now lets convert the date to unix timestamp
UPDATE memoserv SET t_send_tmp=UNIX_TIMESTAMP(t_send);
ALTER TABLE memoserv 
	DROP COLUMN t_send,
	CHANGE t_send_tmp t_send INT NOT NULL;

