##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake to produce Makefile.in.
##

KDE_OPTIONS=noautodist

AUTOMAKE_OPTIONS = foreign dist-zip 1.4

SUBDIRS = \
  @REGINA_BUILD_ENGINE@ \
  @REGINA_BUILD_UTILS@ \
  @REGINA_BUILD_KDEUI@ \
  @REGINA_BUILD_PYTHON@ \
  @REGINA_BUILD_TESTSUITE@ \
  admin \
  docs \
  examples \
  icons \
  pylib \
  scripts

EXTRA_DIST = CHANGES.txt HIGHLIGHTS.txt \
  AUTHORS.txt INSTALL.txt LICENSE.txt README.txt \
  Makefile-help Makefile.cvs

## ----------------------- Custom targets ----------------------------

.PHONY : engine kdeui python utils \
  enginedocs man \
  valgrind help

mostlyclean-local :
	rm -f hs_err_pid*.log

engine :
	cd engine/engine && $(MAKE)
kdeui : engine
	cd kdeui && $(MAKE)
python : engine
	cd python && $(MAKE)
utils : engine
	cd utils && $(MAKE)

enginedocs :
	cd docs/engine && $(MAKE)
man :
	cd docs && $(MAKE) man

valgrind:
	## Run the test suite so that libtool regenerates lt-regtestsuite.
	cd testsuite && $(MAKE) check
	valgrind --skin=memcheck --leak-check=yes testsuite/.libs/lt-regtestsuite

help :
	@cat Makefile-help

