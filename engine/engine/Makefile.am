##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

INCLUDES = @ENGINE_INCLUDES@

SUBDIRS = \
  algebra \
  angle \
  census \
  file \
  foreign \
  maths \
  packet \
  progress \
  property \
  split \
  subcomplex \
  surfaces \
  triangulation \
  utilities

noinst_LTLIBRARIES = libregina-engine.la

libregina_engine_la_SOURCES = \
  shareableobject.cpp

pkginclude_HEADERS = \
  docs.h \
  engine.h \
  howto.h \
  regina.h \
  shareableobject.h

libregina_engine_la_LIBADD = \
  algebra/libalgebra.la \
  angle/libangle.la \
  census/libcensus.la \
  file/libfile.la \
  foreign/libforeign.la \
  maths/libmaths.la \
  packet/libpacket.la \
  progress/libprogress.la \
  property/libproperty.la \
  split/libsplit.la \
  subcomplex/libsubcomplex.la \
  surfaces/libsurfaces.la \
  triangulation/libtriangulation.la \
  utilities/libutilities.la

