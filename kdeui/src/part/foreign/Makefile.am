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
	pythonhandler.h \
	reginahandler.h \
	snappeahandler.h

libforeign_la_SOURCES = \
	exportdialog.cpp \
	importdialog.cpp \
	pythonhandler.cpp \
	reginahandler.cpp \
	snappeahandler.cpp

METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/regina.pot

