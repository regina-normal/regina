##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

KDE_OPTIONS=noautodist

AM_CXXFLAGS = '-DREGINA_PYLIBDIR="$(pkglibdir)/python"'
INCLUDES = $(all_includes) @BOOST_PYTHON_INCLUDES@

noinst_LTLIBRARIES = libregina-kdepython.la

noinst_HEADERS = \
	commandedit.h \
	pythonconsole.h \
	pythoninterpreter.h \
	pythonoutputstream.h

libregina_kdepython_la_LIBADD = @BOOST_PYTHON_LIBS@ @PYTHON_LIBS@

libregina_kdepython_la_SOURCES = \
	commandedit.cpp \
	pythonconsole.cpp \
	pythoninterpreter.cpp \
	pythonoutputstream.cpp

METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/regina.pot

