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
  admin.patch admin.patch.README \
  acinclude.m4.top acx_mpi.m4.ext as-ac-expand.m4.ext cppunit.m4.ext \
  iconv.m4.ext kde.m4.ext lib-ld.m4.ext lib-link.m4.ext lib-prefix.m4.ext \
  libtool.m4.ext configure.ac.bot configure.ac.top \
  Doxyfile.am Doxyfile.global Makefile.common am_edit \
  bcheck.pl conf.change.pl config.guess config.pl config.sub \
  cvs-clean.pl cvs.sh debianrules deps.am detect-autoconf.sh index2gloss \
  install-sh ltmain.sh missing mkinstalldirs nmcheck oldinclude.m4.in \
  ac_gen am_update distcheck insertstub mksums replacetabs

## ----------------------- Custom targets ----------------------------

.PHONY : stubs tabs

stubs :
	cd $(srcdir); ./insertstub stubs/engine \
		`find ../engine -name "*.cpp" -o -name "*.h" -o -name "*.tcc"`
	cd $(srcdir); ./insertstub stubs/kdeui \
		`find ../kdeui -name "*.cpp" -o -name "*.h" -o -name "*.tcc"`
	cd $(srcdir); ./insertstub stubs/python \
		`find ../python -name "*.cpp" -o -name "*.h" -o -name "*.tcc"`
	cd $(srcdir); ./insertstub stubs/testsuite \
		`find ../testsuite -name "*.cpp" -o -name "*.h" -o -name "*.tcc"`
	cd $(srcdir); ./insertstub stubs/general \
		`find ../icons/povray -name "*.pov"`

tabs :
	cd $(srcdir); ./replacetabs `find .. -name "*.cpp" -o -name "*.h" -o -name "*.tcc"`
	cd $(srcdir); ./replacetabs `find .. -name "*.txt"`
	cd $(srcdir); ./replacetabs ../kdeui/doc/en/*.docbook
	cd $(srcdir); ./replacetabs ../pylib/*.py
	cd $(srcdir); ./replacetabs ../scripts/*.py
	cd $(srcdir); ./replacetabs ../utils/tricensus-manager

