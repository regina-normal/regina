##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

KDE_OPTIONS=noautodist

INCLUDES = @ENGINE_INCLUDES@ -I/usr/include/snappea
LDADD = @LIB_ENGINE@ @UTILS_LIBS@ -lsnappea

bin_PROGRAMS = dehn1

dehn1_SOURCES = dehn1.cpp

EXTRA_DIST = README.txt

