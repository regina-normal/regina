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
  dim4 \
  enumerate \
  file \
  foreign \
  hypersurface \
  manifold \
  maths \
  packet \
  progress \
  snappea \
  split \
  subcomplex \
  surfaces \
  triangulation \
  utilities \
  data \
  doxygen

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
  dim4/libdim4.la \
  file/libfile.la \
  foreign/libforeign.la \
  hypersurface/libhypersurface.la \
  manifold/libmanifold.la \
  maths/libmaths.la \
  packet/libpacket.la \
  progress/libprogress.la \
  snappea/libsnappea.la \
  split/libsplit.la \
  subcomplex/libsubcomplex.la \
  surfaces/libsurfaces.la \
  triangulation/libtriangulation.la \
  utilities/libutilities.la \
  @ENGINE_LIBS@

libregina_engine_noinst_la_SOURCES = engine.cpp shareableobject.cpp
libregina_engine_noinst_la_LIBADD = $(libregina_engine_la_LIBADD)

man_MANS = regina-engine-config.1

EXTRA_DIST = README.txt checkdocs regina-engine-config.in $(man_MANS)

## ----------------------- Custom targets ----------------------------

.PHONY : install-data-local install-exec-local uninstall-local

install-data-local :
	$(mkinstalldirs) $(DESTDIR)$(pkgincludedir)
	$(INSTALL_DATA) regina-config.h $(DESTDIR)$(pkgincludedir)

install-exec-local :
	$(mkinstalldirs) $(DESTDIR)$(bindir)
	$(INSTALL_SCRIPT) regina-engine-config $(DESTDIR)$(bindir)

uninstall-local :
	rm -f $(DESTDIR)$(pkgincludedir)/regina-config.h
	rm -f $(DESTDIR)$(bindir)/regina-engine-config

