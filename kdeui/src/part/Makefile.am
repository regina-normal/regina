##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

SUBDIRS = foreign packettypes

INCLUDES = $(all_includes)

noinst_HEADERS = \
	flatbutton.h \
	newpacketdialog.h \
	packetchooser.h \
	packetcreator.h \
	packetfilter.h \
	packetmanager.h \
	packettreeview.h \
	packetui.h \
	packetwindow.h \
	reginapart.h

kde_module_LTLIBRARIES = libreginapart.la

libreginapart_la_LDFLAGS = -module $(KDE_PLUGIN) $(all_libraries)
libreginapart_la_LIBADD = $(LIB_ENGINE) $(LIB_KPARTS) \
	foreign/libforeign.la \
	packettypes/libpackettypes.la
libreginapart_la_SOURCES = \
	actionspart.cpp \
	exports.cpp \
	flatbutton.cpp \
	imports.cpp \
	newpacketdialog.cpp \
	newpackets.cpp \
	packetchooser.cpp \
	packetmanager.cpp \
	packettreeview.cpp \
	packetui.cpp \
	packetwindow.cpp \
	reginapart.cpp \
	treenav.cpp

partdesktopdir   = $(kde_servicesdir)
partdesktop_DATA = reginapart.desktop

partrcdir = $(kde_datadir)/reginapart
partrc_DATA = packetwindow.rc reginapart.rc

METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/regina.pot

