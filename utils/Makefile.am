##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

INCLUDES = @ENGINE_INCLUDES@
LDADD = @LIB_ENGINE@

bin_PROGRAMS = regconvert regfiledump regfiletype sigcensus tricensus

regconvert_SOURCES = regconvert.cpp
regconvert_LDFLAGS = @UTILS_LIBS@

regfiledump_SOURCES = regfiledump.cpp
regfiledump_LDFLAGS = @UTILS_LIBS@

regfiletype_SOURCES = regfiletype.cpp
regfiletype_LDFLAGS = @UTILS_LIBS@

sigcensus_SOURCES = sigcensus.cpp
sigcensus_LDFLAGS = @UTILS_LIBS@

tricensus_SOURCES = tricensus.cpp
tricensus_LDFLAGS = @UTILS_LIBS@

EXTRA_DIST = README.txt

