##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

KDE_OPTIONS=noautodist

CXX = @MPICXX@
INCLUDES = @ENGINE_INCLUDES@
LDADD = @LIB_ENGINE@ @UTILS_LIBS@ @MPILIBS@

noinst_PROGRAMS = build_nbinarya5 build_nbinarys4

build_nbinarya5_SOURCES = build_nbinarya5.cpp
build_nbinarys4_SOURCES = build_nbinarys4.cpp

noinst_HEADERS = nspin4simplex.h nfield25.h nQuaternion25.h
