## Makefile.am for Regina

INCLUDES = $(all_includes) -I$(top_srcdir)/../engine/engine

noinst_HEADERS = \
	packetmanager.h \
	packettreeview.h \
	reginapart.h

kde_module_LTLIBRARIES = libreginapart.la

libreginapart_la_LDFLAGS = -module $(KDE_PLUGIN) $(all_libraries)
libreginapart_la_LIBADD = $(LIB_ENGINE) $(LIB_KPARTS)
libreginapart_la_SOURCES = \
	actionspart.cpp \
	packetmanager.cpp \
	packettreeview.cpp \
	reginapart.cpp

partdesktopdir   = $(kde_servicesdir)
partdesktop_DATA = reginapart.desktop

partrcdir = $(kde_datadir)/reginapart
partrc_DATA = reginapart.rc

METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/regina.pot

