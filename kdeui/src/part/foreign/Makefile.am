##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

INCLUDES = $(all_includes)

noinst_LTLIBRARIES = libforeign.la

noinst_HEADERS = \
	exportdialog.h \
	importdialog.h \
	packetexporter.h \
	packetimporter.h \
	reginahandler.h \
	snappea.h

libforeign_la_SOURCES = \
	exportdialog.cpp \
	importdialog.cpp \
	reginahandler.cpp \
	snappea.cpp

METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/regina.pot

