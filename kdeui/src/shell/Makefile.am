##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

INCLUDES = $(all_includes)

noinst_HEADERS = \
	reginamain.h \
	reginapref.h

bin_PROGRAMS = regina-kde

regina_kde_LDFLAGS = $(KDE_RPATH) $(all_libraries)
regina_kde_LDADD   = $(LIB_ENGINE) $(LIB_KPARTS)
regina_kde_SOURCES = \
	main.cpp \
	reginamain.cpp \
	reginapref.cpp \
	reginaiface.skel

shelldesktopdir   = $(kde_appsdir)/Applications
shelldesktop_DATA = regina.desktop

shellrcdir = $(kde_datadir)/regina
shellrc_DATA = reginamain.rc

METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/regina.pot

