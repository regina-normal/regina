##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

KDE_OPTIONS=noautodist

SUBDIRS = common shell part pics toolbar toolbar-main

INCLUDES = $(all_includes)

METASOURCES = AUTO

EXTRA_DIST = purgeconfig

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/regina.pot

