##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

KDE_OPTIONS=noautodist

AM_CXXFLAGS = '-DREGINA_DATADIR=$(pkgdatadir)'
INCLUDES = $(all_includes)

noinst_HEADERS = \
	reginaprefset.h

lib_LTLIBRARIES = libregina-kdecommon.la

libregina_kdecommon_la_LDFLAGS = -release @PACKAGE_VERSION@ $(all_libraries)
libregina_kdecommon_la_LIBADD = $(LIB_ENGINE)
libregina_kdecommon_la_SOURCES = \
	reginaprefset.cpp

METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/regina.pot

