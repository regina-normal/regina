##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

KDE_OPTIONS=noautodist

INCLUDES = @ENGINE_INCLUDES@

utilitiesincludedir = $(pkgincludedir)/utilities

noinst_LTLIBRARIES = libutilities.la

libutilities_la_LIBADD = @LTLIBICONV@
libutilities_la_SOURCES = base64.cpp hashutils.cpp i18nutils.cpp nbooleans.cpp nmatrix2.cpp nmpi.cpp nrational.cpp nthread.cpp osutils.cpp stringutils.cpp xmlutils.cpp zstream.cpp

utilitiesinclude_HEADERS = base64.h bitmanip.h boostutils.h hashmap.h hashset.h hashutils.h i18nutils.h memutils.h nbitmask.h nbooleans.h nindexedarray.h nlistoncall.h nmarkedvector.h nmatrix2.h nmpi.h nproperty.h nrational.h nthread.h osutils.h ptrutils.h stlutils.h stringutils.h stringutils.tcc xmlutils.h zstream.h

