# $Id: nickserv.2.sql,v 1.1.1.1 2005/08/27 15:44:48 jpinto Exp $

# Add temporary columns for conversion
ALTER TABLE nickserv
	ADD COLUMN t_ident_tmp INT AFTER t_ident,
	ADD COLUMN t_reg_tmp INT AFTER t_reg,
	ADD COLUMN t_seen_tmp INT AFTER t_seen,
	ADD COLUMN t_sign_tmp INT AFTER t_sign;

# Now lets convert the dates to unix timestamp
UPDATE nickserv SET
	t_ident_tmp = UNIX_TIMESTAMP(t_ident),
	t_reg_tmp = UNIX_TIMESTAMP(t_reg),
	t_seen_tmp  = UNIX_TIMESTAMP(t_seen),
	t_sign_tmp = UNIX_TIMESTAMP(t_sign);

ALTER TABLE nickserv
	DROP COLUMN t_ident,
	CHANGE t_ident_tmp t_ident INT NOT NULL,
	DROP COLUMN t_reg,
	CHANGE t_reg_tmp t_reg INT NOT NULL,
	DROP COLUMN t_seen,
	CHANGE t_seen_tmp t_seen INT NOT NULL,
	DROP COLUMN t_sign,
	CHANGE t_sign_tmp t_sign INT NOT NULL;

# Finally set the BINARY option where it should be
ALTER TABLE nickserv
	CHANGE nick nick varchar(32) BINARY NOT NULL;
