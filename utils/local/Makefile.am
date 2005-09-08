##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

KDE_OPTIONS=noautodist

INCLUDES = @ENGINE_INCLUDES@
LDADD = @LIB_ENGINE@ @UTILS_LIBS@

noinst_PROGRAMS = censuscat identify

censuscat_SOURCES = censuscat.cpp
identify_SOURCES = identify.cpp

EXTRA_DIST = README.txt

