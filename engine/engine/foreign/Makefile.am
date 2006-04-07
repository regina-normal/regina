##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

KDE_OPTIONS=noautodist

INCLUDES = @ENGINE_INCLUDES@ $(all_includes)

foreignincludedir = $(pkgincludedir)/foreign

noinst_LTLIBRARIES = libforeign.la

libforeign_la_SOURCES = dehydration.cpp orb.cpp snappea.cpp

foreigninclude_HEADERS = casson.h dehydration.h orb.h snappea.h

