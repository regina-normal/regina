##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

INCLUDES = @ENGINE_INCLUDES@

SUBDIRS = \
  census \
  maths \
  utilities

TESTS = regtestsuite

check_PROGRAMS = regtestsuite

regtestsuite_SOURCES = runalltests.cpp
regtestsuite_LDADD = \
  census/libcensus.a \
  maths/libmaths.a \
  utilities/libutilities.a \
  @LIB_ENGINE@ \
  @TESTSUITE_LIBS@

EXTRA_DIST = README.txt findtext

