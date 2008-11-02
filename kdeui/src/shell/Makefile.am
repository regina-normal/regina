##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

KDE_OPTIONS=noautodist

INCLUDES = @ENGINE_INCLUDES@ $(all_includes) -I$(top_srcdir)/kdeui/src/common

noinst_HEADERS = \
	examplesaction.h \
	reginaiface.h \
	reginamain.h \
	reginapref.h

bin_PROGRAMS = regina-kde

regina_kde_LDFLAGS = $(KDE_RPATH)
regina_kde_LDADD   = ../common/libregina-kdecommon.la \
	$(LIB_ENGINE) $(GUI_KDE_LIBS) $(all_libraries)
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

