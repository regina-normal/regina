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
	reginaiface.h \
	reginamain.h \
	reginapref.h

bin_PROGRAMS = regina-kde

regina_kde_LDFLAGS = $(KDE_RPATH) $(all_libraries)
regina_kde_LDADD   = $(LIB_ENGINE) $(LIB_KPARTS) -lktexteditor
regina_kde_SOURCES = \
	main.cpp \
	reginamain.cpp \
	reginapref.cpp \
	reginaiface.skel

shelldesktopdir   = $(kde_appsdir)/Applications
shelldesktop_DATA = regina.desktop

shellrcdir = $(kde_datadir)/regina
shellrc_DATA = reginamain.rc

mimedir = $(kde_mimedir)/application
mime_DATA = x-regina.desktop

METASOURCES = AUTO

EXTRA_DIST = reginamain.rc regina.desktop x-regina.desktop

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/regina.pot

