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

noinst_PROGRAMS = distinguish-mpi equiv-mpi normal-mpi normal-compare-mpi tricensus-freq-mpi tricensus-normal-mpi
noinst_SCRIPTS = normal-mpi-sanity tricensus-sanity-pairs-coverage tricensus-sanity-pairs-done tricensus-sanity-tricount

distinguish_mpi_SOURCES = distinguish-mpi.cpp
equiv_mpi_SOURCES = equiv-mpi.cpp
normal_compare_mpi_SOURCES = normal-mpi.cpp
normal_mpi_SOURCES = normal-mpi.cpp
tricensus_freq_mpi_SOURCES = tricensus-freq-mpi.cpp
tricensus_normal_mpi_SOURCES = tricensus-freq-mpi.cpp

EXTRA_DIST = README.txt \
	normal-mpi-sanity \
	tricensus-sanity-pairs-coverage \
	tricensus-sanity-pairs-done \
	tricensus-sanity-tricount

