##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

SUBDIRS = packettypes

INCLUDES = $(all_includes)

noinst_HEADERS = \
	flatbutton.h \
	packetmanager.h \
	packettreeview.h \
	packetui.h \
	packetwindow.h \
	reginapart.h

kde_module_LTLIBRARIES = libreginapart.la

libreginapart_la_LDFLAGS = -module $(KDE_PLUGIN) $(all_libraries)
libreginapart_la_LIBADD = $(LIB_ENGINE) $(LIB_KPARTS) \
	packettypes/libpackettypes.la
libreginapart_la_SOURCES = \
	actionspart.cpp \
	flatbutton.cpp \
	packetmanager.cpp \
	packettreeview.cpp \
	packetui.cpp \
	packetwindow.cpp \
	reginapart.cpp

partdesktopdir   = $(kde_servicesdir)
partdesktop_DATA = reginapart.desktop

partrcdir = $(kde_datadir)/reginapart
partrc_DATA = packetwindow.rc reginapart.rc

METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/regina.pot

