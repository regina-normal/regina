## Makefile.am for Regina

SUBDIRS = shell part pics toolbar toolbar-main

INCLUDES = $(all_includes) -I$(top_srcdir)/../engine/engine

noinst_HEADERS = \
	reginaabout.h \
	reginafilter.h

METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/regina.pot

