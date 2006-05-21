##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

KDE_OPTIONS=noautodist

INCLUDES = @ENGINE_INCLUDES@

manifoldincludedir = $(pkgincludedir)/manifold

noinst_LTLIBRARIES = libmanifold.la

libmanifold_la_SOURCES = ngraphloop.cpp ngraphpair.cpp ngraphtriple.cpp nhandlebody.cpp nlensspace.cpp nmanifold.cpp nsfs.cpp nsfsaltset.cpp nsimplesurfacebundle.cpp nsnappeacensusmfd.cpp ntorusbundle.cpp order.cpp

manifoldinclude_HEADERS = ngraphloop.h ngraphpair.h ngraphtriple.h nhandlebody.h nlensspace.h nmanifold.h notation.h nsfs.h nsfsaltset.h nsimplesurfacebundle.h nsnappeacensusmfd.h ntorusbundle.h

EXTRA_DIST = sfsnotation.eps sfsnotation.fig sfsnotation.png
