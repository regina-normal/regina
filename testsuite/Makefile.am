##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

INCLUDES = @ENGINE_INCLUDES@
LDFLAGS = @TESTSUITE_LIBS@

SUBDIRS = \
  maths

TESTS = regtestsuite

check_PROGRAMS = regtestsuite

regtestsuite_SOURCES = runalltests.cpp

regtestsuite_LDADD = \
  maths/libmaths.a \
  @LIB_ENGINE@

EXTRA_DIST = README.txt findtext

