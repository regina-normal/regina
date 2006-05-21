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

noinst_PROGRAMS = equiv-mpi

equiv_mpi_SOURCES = equiv-mpi.cpp

EXTRA_DIST = README.txt

