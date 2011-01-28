##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

KDE_OPTIONS=noautodist

SUBDIRS = @REGINA_BUILD_MPI@ binary_group_tools

INCLUDES = @ENGINE_INCLUDES@
LDADD = @LIB_ENGINE@ @UTILS_LIBS@

noinst_PROGRAMS = censuscat dehydrate distinguish edgelinks equiv equiv-dim4 identify manifolds normal normal-compare pluggedibundles reduce sig-freq trisort volume

censuscat_SOURCES = censuscat.cpp
dehydrate_SOURCES = dehydrate.cpp
distinguish_SOURCES = distinguish.cpp
edgelinks_SOURCES = edgelinks.cpp
equiv_SOURCES = equiv.cpp
equiv_dim4_SOURCES = equiv-dim4.cpp
identify_SOURCES = identify.cpp
manifolds_SOURCES = manifolds.cpp
normal_SOURCES = normal.cpp
normal_compare_SOURCES = normal-compare.cpp
pluggedibundles_SOURCES = pluggedibundles.cpp
reduce_SOURCES = reduce.cpp
sig_freq_SOURCES = sig-freq.cpp
trisort_SOURCES = trisort.cpp
volume_SOURCES = volume.cpp

EXTRA_DIST = README.txt

