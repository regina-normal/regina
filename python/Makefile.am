##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

KDE_OPTIONS=noautodist

INCLUDES = @ENGINE_INCLUDES@ @BOOST_PYTHON_INCLUDES@
CXXFLAGS = @CXXFLAGS_NODEBUG@

SUBDIRS = srcutils \
  algebra \
  angle \
  census \
  file \
  foreign \
  manifold \
  maths \
  packet \
  progress \
  split \
  subcomplex \
  surfaces \
  triangulation \
  utilities

pkgpythonlibdir = $(pkglibdir)/python

pkgpythonlib_LTLIBRARIES = regina.la

regina_la_SOURCES = globalarray.cpp modulemain.cpp
regina_la_LDFLAGS = -module -avoid-version @ADD_STDCALL_ALIAS@
regina_la_LIBADD = @LIB_ENGINE@ @BOOST_PYTHON_LIBS@ \
  algebra/libalgebra.la \
  angle/libangle.la \
  census/libcensus.la \
  file/libfile.la \
  foreign/libforeign.la \
  manifold/libmanifold.la \
  maths/libmaths.la \
  packet/libpacket.la \
  progress/libprogress.la \
  split/libsplit.la \
  subcomplex/libsubcomplex.la \
  surfaces/libsurfaces.la \
  triangulation/libtriangulation.la \
  utilities/libutilities.la

noinst_HEADERS = globalarray.h

EXTRA_DIST = README.txt TODO.txt regina-python.in

install-exec-local :
	$(mkinstalldirs) $(DESTDIR)$(bindir)
	$(INSTALL_SCRIPT) regina-python $(DESTDIR)$(bindir)

uninstall-local :
	rm -f $(DESTDIR)$(bindir)/regina-python
