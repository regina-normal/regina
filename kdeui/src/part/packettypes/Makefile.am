##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

INCLUDES = $(all_includes)

noinst_LTLIBRARIES = libpackettypes.la

noinst_HEADERS = \
	nanglestructurecreator.h \
	nanglestructureui.h \
	ncontainerui.h \
	nscriptui.h \
	nscriptvaritems.h \
	ntextui.h

libpackettypes_la_SOURCES = \
	nanglestructurecreator.cpp \
	nanglestructureui.cpp \
	ncontainerui.cpp \
	nscriptui.cpp \
	nscriptvaritems.cpp \
	ntextui.cpp

METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/regina.pot

