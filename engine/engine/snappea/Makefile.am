##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

KDE_OPTIONS=noautodist

INCLUDES = @ENGINE_INCLUDES@

SUBDIRS = kernel

snappeaincludedir = $(pkgincludedir)/snappea

noinst_LTLIBRARIES = libsnappea.la

libsnappea_la_SOURCES = nsnappeatriangulation.cpp uimessages.cpp
libsnappea_la_LIBADD = kernel/libsnappeakernel.la

snappeainclude_HEADERS = nsnappeatriangulation.h

