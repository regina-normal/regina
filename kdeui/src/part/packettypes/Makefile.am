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
	ncontainercreator.h \
	ncontainerui.h \
	ntextcreator.h \
	ntextui.h

libpackettypes_la_SOURCES = \
	ncontainercreator.cpp \
	ncontainerui.cpp \
	ntextcreator.cpp \
	ntextui.cpp

METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/regina.pot

