##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

INCLUDES = @ENGINE_INCLUDES@ @BOOST_PYTHON_INCLUDES@

SUBDIRS = algebra file maths packet progress utilities

pkgpythonlibdir = $(pkglibdir)/python

pkgpythonlib_LTLIBRARIES = regina.la

regina_la_SOURCES = modulemain.cpp
regina_la_LDFLAGS = -module -avoid-version @ADD_STDCALL_ALIAS@
regina_la_LIBADD = @LIB_ENGINE@ @BOOST_PYTHON_LIBS@ \
  algebra/libalgebra.la \
  file/libfile.la \
  maths/libmaths.la \
  packet/libpacket.la \
  progress/libprogress.la \
  utilities/libutilities.la

EXTRA_DIST = regina-python.in

install-exec-local :
	$(mkinstalldirs) $(DESTDIR)$(bindir)
	$(INSTALL_SCRIPT) regina-python $(DESTDIR)$(bindir)

uninstall-local :
	rm -f $(DESTDIR)$(bindir)/regina-python
