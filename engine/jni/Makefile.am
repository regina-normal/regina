##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

INCLUDES = @ENGINE_INCLUDES@ @JNI_INCLUDES@

SUBDIRS = engine registry

pkglib_LTLIBRARIES = libregina-engine-jni.la

libregina_engine_jni_la_SOURCES = javacast.cpp jnitools.cpp
libregina_engine_jni_la_LDFLAGS = -module -avoid-version -no-undefined \
  @FINAL_LDFLAGS@
libregina_engine_jni_la_LIBADD = @LIB_ENGINE@ \
  engine/libengine.la \
  registry/libregistry.la

noinst_HEADERS = javacast.h jnisafe.h jnitools.h njnienumeration.h

