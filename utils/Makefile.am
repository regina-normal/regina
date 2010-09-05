##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

KDE_OPTIONS=noautodist

SUBDIRS = @REGINA_BUILD_MPI@

INCLUDES = @ENGINE_INCLUDES@
LDADD = @LIB_ENGINE@ @UTILS_LIBS@

bin_SCRIPTS = tricensus-manager
bin_PROGRAMS = regconcat regconvert regfiledump regfiletype regmerge sigcensus tricensus trisetcmp

regconcat_SOURCES = regconcat.cpp

regconvert_SOURCES = regconvert.cpp

regfiledump_SOURCES = regfiledump.cpp

regfiletype_SOURCES = regfiletype.cpp

regmerge_SOURCES = regmerge.cpp

sigcensus_SOURCES = sigcensus.cpp

tricensus_SOURCES = tricensus.cpp

trisetcmp_SOURCES = trisetcmp.cpp

EXTRA_DIST = README.txt tricensus-manager

