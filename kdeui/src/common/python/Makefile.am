##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

KDE_OPTIONS=noautodist

INCLUDES = $(all_includes) @BOOST_PYTHON_INCLUDES@

noinst_LTLIBRARIES = libregina-kdepython.la

noinst_HEADERS = \
	pythonconsole.h

libregina_kdepython_la_LIBADD = $(LIB_KHTML)

libregina_kdepython_la_SOURCES = \
	pythonconsole.cpp

METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/regina.pot

