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

noinst_PROGRAMS = censuscat dehydrate distinguish equiv identify manifolds pluggedibundles reduce trisort volume

censuscat_SOURCES = censuscat.cpp
dehydrate_SOURCES = dehydrate.cpp
distinguish_SOURCES = distinguish.cpp
equiv_SOURCES = equiv.cpp
identify_SOURCES = identify.cpp
manifolds_SOURCES = manifolds.cpp
pluggedibundles_SOURCES = pluggedibundles.cpp
reduce_SOURCES = reduce.cpp
trisort_SOURCES = trisort.cpp
volume_SOURCES = volume.cpp

EXTRA_DIST = README.txt

