##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake to produce Makefile.in.
##

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
  regina.in regina.conf.sample \
  Makefile-help Makefile.cvs

## ----------------------- Custom targets ----------------------------

.PHONY : engine utils bin \
  enginedocs html man docs \
  valgrind help

mostlyclean-local :
	rm -f hs_err_pid*.log

engine :
	cd engine/engine && $(MAKE)
utils : engine
	cd utils && $(MAKE)
bin : engine utils

enginedocs :
	cd docs/engine && $(MAKE)
html :
	cd docs && $(MAKE) html
man :
	cd docs && $(MAKE) man
docs : enginedocs html man

valgrind:
	## Run the test suite so that libtool regenerates lt-regtestsuite.
	cd testsuite && $(MAKE) check
	valgrind --skin=memcheck --leak-check=yes testsuite/.libs/lt-regtestsuite

help :
	@cat Makefile-help

