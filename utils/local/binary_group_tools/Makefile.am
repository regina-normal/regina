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

noinst_PROGRAMS = build_nbinarya5 build_nbinarys4

build_nbinarya5_SOURCES = build_nbinarya5.cpp nfield25.cpp nQuaternion25.cpp
build_nbinarys4_SOURCES = build_nbinarys4.cpp nfield25.cpp nQuaternion25.cpp

noinst_HEADERS = nspin4simplex.h nfield25.h nQuaternion25.h

EXTRA_DIST = README.txt
