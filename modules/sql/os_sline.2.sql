# $Id: os_sline.2.sql,v 1.1.1.1 2005/08/27 15:44:48 jpinto Exp $

ALTER TABLE os_sline
    CHANGE mask mask VARCHAR(128) NOT NULL;

