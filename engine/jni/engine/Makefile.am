##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

INCLUDES = @ENGINE_INCLUDES@ @JNI_INCLUDES@

SUBDIRS = \
  algebra \
  angle \
  file \
  maths \
  packet \
  progress \
  split \
  subcomplex \
  surfaces \
  triangulation

noinst_LTLIBRARIES = libengine.la

libengine_la_SOURCES = jniengine.cpp jnishareableobject.cpp 

libengine_la_LIBADD = \
  algebra/libalgebra.la \
  angle/libangle.la \
  file/libfile.la \
  maths/libmaths.la \
  packet/libpacket.la \
  progress/libprogress.la \
  split/libsplit.la \
  subcomplex/libsubcomplex.la \
  surfaces/libsurfaces.la \
  triangulation/libtriangulation.la

