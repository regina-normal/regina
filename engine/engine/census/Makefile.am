##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

INCLUDES = @ENGINE_INCLUDES@

censusincludedir = $(pkgincludedir)/census

noinst_LTLIBRARIES = libcensus.la

libcensus_la_SOURCES = \
  ncensus.cpp \
  nfacepairing.cpp \
  ngluingperms.cpp

censusinclude_HEADERS = \
  ncensus.h \
  nfacepairing.h \
  ngluingperms.h

EXTRA_DIST = results.txt

