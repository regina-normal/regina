##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

KDE_OPTIONS=noautodist

INCLUDES = @ENGINE_INCLUDES@

SUBDIRS = \
  algebra \
  angle \
  census \
  enumerate \
  file \
  foreign \
  knot \
  manifold \
  maths \
  packet \
  progress \
  split \
  subcomplex \
  surfaces \
  triangulation \
  utilities

lib_LTLIBRARIES = libregina-engine.la

# With --enable-static-final, we'll need a convenience engine library as
# well as the usual shared library.

if STATIC_FINAL
noinst_LTLIBRARIES = libregina-engine-noinst.la
endif

pkginclude_HEADERS = docs.h engine.h shareableobject.h

libregina_engine_la_SOURCES = engine.cpp shareableobject.cpp
libregina_engine_la_LDFLAGS = -release @PACKAGE_VERSION@
libregina_engine_la_LIBADD = \
  algebra/libalgebra.la \
  angle/libangle.la \
  census/libcensus.la \
  file/libfile.la \
  foreign/libforeign.la \
  knot/libknot.la \
  manifold/libmanifold.la \
  maths/libmaths.la \
  packet/libpacket.la \
  progress/libprogress.la \
  split/libsplit.la \
  subcomplex/libsubcomplex.la \
  surfaces/libsurfaces.la \
  triangulation/libtriangulation.la \
  utilities/libutilities.la \
  @ENGINE_LIBS@

libregina_engine_noinst_la_SOURCES = engine.cpp shareableobject.cpp
libregina_engine_noinst_la_LIBADD = $(libregina_engine_la_LIBADD)

## ----------------------- Custom targets ----------------------------

.PHONY : install-data-local uninstall-local

install-data-local :
	$(mkinstalldirs) $(DESTDIR)$(pkgincludedir)
	$(INSTALL_DATA) regina-config.h $(DESTDIR)$(pkgincludedir)

uninstall-local :
	rm -f $(DESTDIR)$(pkgincludedir)/regina-config.h

