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

noinst_PROGRAMS = censuscat distinguish equiv identify manifolds volume

censuscat_SOURCES = censuscat.cpp
distinguish_SOURCES = distinguish.cpp
equiv_SOURCES = equiv.cpp
identify_SOURCES = identify.cpp
manifolds_SOURCES = manifolds.cpp
volume_SOURCES = volume.cpp

EXTRA_DIST = README.txt

