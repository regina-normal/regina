##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

SUBDIRS = shell part pics toolbar toolbar-main

INCLUDES = $(all_includes)

noinst_HEADERS = \
	reginaabout.h \
	reginafilter.h \
	reginaprefset.h

METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/regina.pot

