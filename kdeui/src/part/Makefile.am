##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

KDE_OPTIONS=noautodist

SUBDIRS = foreign packettypes

AM_CXXFLAGS = '-DREGINA_DATADIR="$(pkgdatadir)"'
INCLUDES = $(all_includes) -I../common

noinst_HEADERS = \
	exttabctl.h \
	flatbutton.h \
	newpacketdialog.h \
	packetchooser.h \
	packetcreator.h \
	packetfilter.h \
	packetmanager.h \
	packettabui.h \
	packettreeview.h \
	packetui.h \
	packetwindow.h \
	progressdialogs.h \
	reginapart.h

kde_module_LTLIBRARIES = libreginapart.la

libreginapart_la_LDFLAGS = -module $(KDE_PLUGIN) $(all_libraries)
libreginapart_la_LIBADD = $(LIB_ENGINE) $(LIB_KPARTS) -lktexteditor \
	../common/libregina-kdecommon.la \
	foreign/libforeign.la \
	packettypes/libpackettypes.la
libreginapart_la_SOURCES = \
	actionspart.cpp \
	exports.cpp \
	exttabctl.cpp \
	flatbutton.cpp \
	imports.cpp \
	newpacketdialog.cpp \
	newpackets.cpp \
	packetchooser.cpp \
	packetmanager.cpp \
	packettabui.cpp \
	packettreeview.cpp \
	packetui.cpp \
	packetwindow.cpp \
	progressdialogs.cpp \
	reginapart.cpp \
	treenav.cpp

partdesktopdir   = $(kde_servicesdir)
partdesktop_DATA = reginapart.desktop

partrcdir = $(kde_datadir)/reginapart
partrc_DATA = packetwindow.rc reginapart.rc

METASOURCES = AUTO

EXTRA_DIST = reginapart.desktop reginapart.rc packetwindow.rc

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/regina.pot

