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

PYTHON_TESTS = euler groups i18n misc surfaces trigeneral

EXTRA_DIST = README.txt testall \
	$(PYTHON_TESTS:=.test) \
	$(PYTHON_TESTS:=.out) \
	i18n-latin1.rga i18n-utf8.rga O2_1.rga O2_1.tri
