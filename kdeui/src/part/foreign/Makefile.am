##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

KDE_OPTIONS=noautodist

INCLUDES = @ENGINE_INCLUDES@ $(all_includes) -I$(top_srcdir)/kdeui/src/common

noinst_LTLIBRARIES = libforeign.la

noinst_HEADERS = \
	dehydrationhandler.h \
	exportdialog.h \
	importdialog.h \
	orbhandler.h \
	packetexporter.h \
	packetimporter.h \
	pythonhandler.h \
	reginahandler.h \
	snappeahandler.h \
	sourcehandler.h

libforeign_la_SOURCES = \
	dehydrationhandler.cpp \
	exportdialog.cpp \
	importdialog.cpp \
	orbhandler.cpp \
	pythonhandler.cpp \
	reginahandler.cpp \
	snappeahandler.cpp \
	sourcehandler.cpp

METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/regina.pot

