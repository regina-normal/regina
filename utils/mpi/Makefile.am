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

bin_PROGRAMS = tricensus-mpi

tricensus_mpi_SOURCES = tricensus-mpi.cpp

EXTRA_DIST = README.txt

