## Makefile.am for Regina

SUBDIRS = pics toolbar

# Flags for compilation:
INCLUDES = $(all_includes) -I$(top_srcdir)/../engine/engine

# Project headers:
noinst_HEADERS = \
	packetmanager.h \
	packettreeview.h \
	reginaabout.h \
	reginafilter.h \
	reginamain.h \
	reginapart.h \
	reginapref.h

# Main program(s):
bin_PROGRAMS = regina

regina_LDFLAGS = $(KDE_RPATH) $(all_libraries)
regina_LDADD   = $(top_srcdir)/../engine/engine/libregina-engine.la \
	$(LIB_KPARTS)
regina_SOURCES = \
	main.cpp \
	reginamain.cpp \
	reginapref.cpp \
	reginaiface.skel

shelldesktopdir   = $(kde_appsdir)/Applications
shelldesktop_DATA = regina.desktop

shellrcdir = $(kde_datadir)/regina
shellrc_DATA = reginamain.rc

# KPart:
kde_module_LTLIBRARIES = libreginapart.la

libreginapart_la_LDFLAGS = -module $(KDE_PLUGIN) $(all_libraries)
libreginapart_la_LIBADD = $(top_srcdir)/../engine/engine/libregina-engine.la \
	$(LIB_KPARTS)
libreginapart_la_SOURCES = \
	packetmanager.cpp \
	packettreeview.cpp \
	reginapart.cpp

partdesktopdir   = $(kde_servicesdir)
partdesktop_DATA = reginapart.desktop

partrcdir = $(kde_datadir)/reginapart
partrc_DATA = reginapart.rc

# Other KDE stuff:
METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/regina.pot

KDE_ICON = regina

