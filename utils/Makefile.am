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
bin_PROGRAMS = regconvert regfiledump regfiletype sigcensus tricensus

regconvert_SOURCES = regconvert.cpp
regconvert_LDFLAGS = @FINAL_LDFLAGS@

regfiledump_SOURCES = regfiledump.cpp
regfiledump_LDFLAGS = @FINAL_LDFLAGS@

regfiletype_SOURCES = regfiletype.cpp
regfiletype_LDFLAGS = @FINAL_LDFLAGS@

sigcensus_SOURCES = sigcensus.cpp
sigcensus_LDFLAGS = @FINAL_LDFLAGS@

tricensus_SOURCES = tricensus.cpp
tricensus_LDFLAGS = @FINAL_LDFLAGS@

EXTRA_DIST = README.txt tricensus-manager

