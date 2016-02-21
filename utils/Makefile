#**********************************************************************
#* PTlink IRC Services is (C) CopyRight PTlink IRC Software 1999-2005 *
#*                  http://software.pt-link.net                       *
#* This program is distributed under GNU Public License               *
#* Please read the file COPYING for copyright information.            *
#**********************************************************************
#
#  Description: PTlink IRC Services utils Makefile.in
#
#  $Id: Makefile.in,v 1.1.1.1 2005/08/27 15:43:52 jpinto Exp $
#

prefix          = /home/lamego/ircsvs
utilsdir	= /home/lamego/ircsvs/utils
sqldir		= /home/lamego/ircsvs/utils/sql
INSTALL         = /usr/bin/install -c
INSTALL_DATA    = ${INSTALL} -m 644
RM		= /bin/rm

UTILS		= \
		svsquery.sh \
		expire_email.sh \
		news_email.sh
SQLS		= \
		clear_db.sql \
		news_emails.sql \
		expire_emails.sql 

build:
	@echo Nothing to build on utils
install-mkdirs:
	-@if test ! -d $(prefix); then \
		echo "mkdir $(prefix)"; \
		mkdir $(prefix); \
	fi
	-@if test ! -d $(utilsdir); then \
		echo "mkdir $(utilsdir)"; \
		mkdir $(utilsdir); \
	fi
	-@if test ! -d $(sqldir); then \
		echo "mkdir $(sqldir)"; \
		mkdir $(sqldir); \
	fi
install: install-mkdirs
	@for i in $(UTILS); do \
		if ! test -f $(utilsdir)/$$i; then \
			$(INSTALL) $$i $(utilsdir); \
		fi; \
        done
	@for i in $(SQLS); do \
		if ! test -f $(sqldir)/$$i; then \
			$(INSTALL_DATA) sql/$$i $(sqldir); \
		fi; \
        done

clean:
	${RM} -f *.o *.exe *~ core.* ${PROG}

distclean: clean
	${RM} -f Makefile version.c.last
 