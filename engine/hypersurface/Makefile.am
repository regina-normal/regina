##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

KDE_OPTIONS=noautodist

INCLUDES = @ENGINE_INCLUDES@

hypersurfaceincludedir = $(pkgincludedir)/hypersurface

noinst_LTLIBRARIES = libhypersurface.la

libhypersurface_la_SOURCES = hsflavours.cpp links.cpp nhsstandard.cpp nnormalhypersurface.cpp nnormalhypersurfacelist.cpp nxmlhypersurfacereader.cpp triangulate.cpp

hypersurfaceinclude_HEADERS = hsflavourregistry.h nhsstandard.h nhsmirrored.h nnormalhypersurface.h nnormalhypersurfacelist.h nxmlhypersurfacereader.h

