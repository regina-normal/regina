##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

INCLUDES = @ENGINE_INCLUDES@ @BOOST_PYTHON_INCLUDES@

SUBDIRS = utilities

pkgpythonlibdir = $(pkglibdir)/python

pkgpythonlib_LTLIBRARIES = regina.la

regina_la_SOURCES = modulemain.cpp
regina_la_LDFLAGS = -module -avoid-version @ADD_STDCALL_ALIAS@
regina_la_LIBADD = @LIB_ENGINE@ @BOOST_PYTHON_LIBS@ \
  utilities/libutilities.la

