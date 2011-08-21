##
##  Regina - A Normal Surface Theory Calculator
##  Auxiliary Makefile
##
##  These tools should be used only by the core software maintainers.
##  In fact, there are good arguments that they should not be used at all.
##  Keeping them around though just in case.
##

srcdir = ..

all :

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

