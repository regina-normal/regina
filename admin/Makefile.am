##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

KDE_OPTIONS=noautodist

SUBDIRS = stubs

EXTRA_DIST = \
  README.txt RELEASE.txt \
  acinclude.m4.top as-ac-expand.m4.ext cppunit.m4.ext kde.m4.ext \
  libtool.m4.ext configure.ac.bot configure.ac.top \
  Doxyfile.am Doxyfile.global Makefile.common am_edit conf.change.pl \
  config.guess config.pl config.sub cvs-clean.pl cvs.sh debianrules \
  detect-autoconf.sh install-sh ltmain.sh missing mkinstalldirs \
  ac_gen am_update distcheck insertstub mksums replacetabs

## ----------------------- Custom targets ----------------------------

.PHONY : stubs tabs

stubs :
	./insertstub stubs/engine \
		`find ../engine -name "*.cpp" -o -name "*.h" -o -name "*.tcc"`
	./insertstub stubs/kdeui \
		`find ../kdeui -name "*.cpp" -o -name "*.h" -o -name "*.tcc"`
	./insertstub stubs/python \
		`find ../python -name "*.cpp" -o -name "*.h" -o -name "*.tcc"`
	./insertstub stubs/testsuite \
		`find ../testsuite -name "*.cpp" -o -name "*.h" -o -name "*.tcc"`

tabs :
	./replacetabs `find .. -name "*.cpp" -o -name "*.h" -o -name "*.tcc"`
	./replacetabs `find .. -name "*.txt"`
	./replacetabs ../kdeui/doc/en/*.docbook
	./replacetabs ../pylib/*.py
	./replacetabs ../scripts/*.py
	./replacetabs ../utils/tricensus-manager

