##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

KDE_OPTIONS=noautodist

INCLUDES = @ENGINE_INCLUDES@
AM_CXXFLAGS = '-DREGINA_DATADIR="$(pkgdatadir)"' '-DREGINA_PYLIBDIR="$(pkglibdir)/python"'

fileincludedir = $(pkgincludedir)/file

noinst_LTLIBRARIES = libfile.la

libfile_la_SOURCES = nfile.cpp nfileinfo.cpp nglobaldirs.cpp nresources.cpp nxmlcallback.cpp nxmlfile.cpp

fileinclude_HEADERS = nfile.h nfileinfo.h nfilepropertyreader.h nglobaldirs.h nresources.h nxmlcallback.h nxmlelementreader.h nxmlfile.h

