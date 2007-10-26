##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

KDE_OPTIONS=noautodist

TESTS = testall

check_SCRIPTS = testall

PYTHON_TESTS = euler groups misc surfaces trigeneral

EXTRA_DIST = README.txt testall \
	$(PYTHON_TESTS:=.test) \
	$(PYTHON_TESTS:=.out)
