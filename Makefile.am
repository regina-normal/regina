##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake to produce Makefile.in.
##

AUTOMAKE_OPTIONS = foreign dist-zip 1.4

SUBDIRS = \
  @REGINA_BUILD_ENGINE@ \
  @REGINA_BUILD_JAVAUI@ \
  @REGINA_BUILD_UTILS@ \
  @REGINA_BUILD_TESTSUITE@ \
  docs \
  examples \
  icons \
  lib \
  pylib \
  scripts

EXTRA_DIST = CHANGES.txt HIGHLIGHTS.txt \
  AUTHORS.txt INSTALL.txt LICENSE.txt README.txt \
  regina regina.bat regina.conf.sample runfromsrc

## ----------------------- Custom targets ----------------------------

.PHONY : engine jni java utils bin \
  enginedocs javadocs html man docjar docs \
  jnihdr jnihdr-clean \
  help

engine :
	cd engine/engine && $(MAKE)
jni : engine
	cd engine/jni && $(MAKE)
java :
	cd javaui && $(MAKE)
utils : engine
	cd utils && $(MAKE)
bin : engine jni java utils

enginedocs :
	cd docs/engine && $(MAKE)
javadocs : java
	cd docs/javaui && $(MAKE)
html :
	cd docs && $(MAKE) html
man :
	cd docs && $(MAKE) man
docjar :
	cd docs && $(MAKE) jar
docs : enginedocs javadocs html man docjar

jnihdr :
	cd javaui && $(MAKE) jni
jnihdr-clean :
	cd javaui && $(MAKE) jni-clean

help :
	@cat Makefile-help

