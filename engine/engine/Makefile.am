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
  enumerate \
  file \
  foreign \
  knot \
  maths \
  packet \
  progress \
  property \
  split \
  subcomplex \
  surfaces \
  triangulation \
  utilities

lib_LTLIBRARIES = libregina-engine.la

libregina_engine_la_SOURCES = shareableobject.cpp
libregina_engine_la_LDFLAGS = -release @PACKAGE_VERSION@

pkginclude_HEADERS = docs.h engine.h howto.h regina.h shareableobject.h

libregina_engine_la_LIBADD = \
  algebra/libalgebra.la \
  angle/libangle.la \
  census/libcensus.la \
  file/libfile.la \
  foreign/libforeign.la \
  knot/libknot.la \
  maths/libmaths.la \
  packet/libpacket.la \
  progress/libprogress.la \
  property/libproperty.la \
  split/libsplit.la \
  subcomplex/libsubcomplex.la \
  surfaces/libsurfaces.la \
  triangulation/libtriangulation.la \
  utilities/libutilities.la \
  @ENGINE_LIBS@

