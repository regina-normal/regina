##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

KDE_OPTIONS=noautodist

INCLUDES = @ENGINE_INCLUDES@

foreignincludedir = $(pkgincludedir)/foreign

noinst_LTLIBRARIES = libforeign.la

libforeign_la_SOURCES = csvsurfacelist.cpp dehydration.cpp orb.cpp snappea.cpp

foreigninclude_HEADERS = csvsurfacelist.h dehydration.h orb.h snappea.h
noinst_HEADERS = casson.h

