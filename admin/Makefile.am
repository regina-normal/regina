##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

SUBDIRS = stubs

EXTRA_DIST = \
  README.txt RELEASE.txt \
  acinclude.m4.top cppunit.m4.ext libtool.m4.ext pthread.m4.ext \
  configure.ac.bot configure.ac.top \
  config.guess config.sub install-sh ltmain.sh missing mkinstalldirs \
  ac_gen am_update insertstub mksums replacetabs

## ----------------------- Custom targets ----------------------------

.PHONY : stubs tabs

stubs :
	./insertstub stubs/engine \
		`find ../engine -name "*.cpp" -o -name "*.h" -o -name "*.tcc"`
	./insertstub stubs/javaui `find ../javaui -name "*.java"`

tabs :
	./replacetabs `find .. -name "*.cpp" -o -name "*.h" -o -name "*.tcc"`
	./replacetabs `find .. -name "*.java"`
	./replacetabs `find .. -name "*.txt"`
	./replacetabs ../docs/*.sgml

