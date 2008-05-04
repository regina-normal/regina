##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

KDE_OPTIONS=noautodist

SUBDIRS = @REGINA_BUILD_PYTHON@

INCLUDES = @ENGINE_INCLUDES@ $(all_includes)

noinst_HEADERS = \
	coordinatechooser.h \
	coordinates.h \
	pythonmanager.h \
	qtcompat.h \
	reginaabout.h \
	reginafilter.h \
	reginaprefset.h \
	shortrunner.h

lib_LTLIBRARIES = libregina-kdecommon.la

if COND_BUILD_PYTHON
libregina_kdecommon_la_LDFLAGS = -release @PACKAGE_VERSION@ $(all_libraries) \
	@BOOST_PYTHON_LDFLAGS@
libregina_kdecommon_la_LIBADD = $(LIB_ENGINE) python/libregina-kdepython.la
else
libregina_kdecommon_la_LDFLAGS = -release @PACKAGE_VERSION@ $(all_libraries)
libregina_kdecommon_la_LIBADD = $(LIB_ENGINE)
endif

libregina_kdecommon_la_SOURCES = \
	coordinatechooser.cpp \
	coordinates.cpp \
	pythonmanager.cpp \
	qtcompat.cpp \
	reginaabout.cpp \
	reginaprefset.cpp \
	shortrunner.cpp

METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/regina.pot

