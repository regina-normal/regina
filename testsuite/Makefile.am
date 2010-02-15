##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

KDE_OPTIONS=noautodist

INCLUDES = -I$(top_srcdir) @ENGINE_INCLUDES@

SUBDIRS = \
  algebra \
  angle \
  census \
  dim4 \
  hypersurface \
  maths \
  snappea \
  subcomplex \
  surfaces \
  triangulation \
  utilities

TESTS = regtestsuite

check_PROGRAMS = regtestsuite

regtestsuite_SOURCES = runalltests.cpp
regtestsuite_LDADD = \
  algebra/libalgebra.a \
  angle/libangle.a \
  census/libcensus.a \
  dim4/libdim4.a \
  maths/libmaths.a \
  snappea/libsnappea.a \
  subcomplex/libsubcomplex.a \
  surfaces/libsurfaces.a \
  triangulation/libtriangulation.a \
  utilities/libutilities.a \
  @LIB_ENGINE@ \
  @TESTSUITE_LIBS@

EXTRA_DIST = README.txt

