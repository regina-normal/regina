##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

KDE_OPTIONS=noautodist

AM_CXXFLAGS = '-DREGINA_DATADIR="$(pkgdatadir)"'
INCLUDES = $(all_includes) -I../common

noinst_HEADERS = \
	examplesaction.h \
	reginaiface.h \
	reginamain.h \
	reginapref.h

bin_PROGRAMS = regina-kde

regina_kde_LDFLAGS = $(KDE_RPATH) $(all_libraries)
regina_kde_LDADD   = ../common/libregina-kdecommon.la \
	$(LIB_ENGINE) $(LIB_KPARTS) -lktexteditor
regina_kde_SOURCES = \
	examplesaction.cpp \
	main.cpp \
	reginamain.cpp \
	reginapref.cpp \
	reginaiface.skel

xdg_apps_DATA = regina.desktop

shelldatadir = $(kde_datadir)/regina
shelldata_DATA = reginamain.rc tips

mimedir = $(kde_mimedir)/application
mime_DATA = x-regina.desktop

METASOURCES = AUTO

EXTRA_DIST = reginamain.rc regina.desktop tips x-regina.desktop

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/regina.pot

