##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

INCLUDES = @ENGINE_INCLUDES@
LDADD = @LIB_ENGINE@ @UTILS_LIBS@

bin_SCRIPTS = tricensus-manager
bin_PROGRAMS = regconcat regconvert regfiledump regfiletype sigcensus tricensus

regconcat_SOURCES = regconcat.cpp

regconvert_SOURCES = regconvert.cpp

regfiledump_SOURCES = regfiledump.cpp

regfiletype_SOURCES = regfiletype.cpp

sigcensus_SOURCES = sigcensus.cpp

tricensus_SOURCES = tricensus.cpp

EXTRA_DIST = README.txt tricensus-manager

