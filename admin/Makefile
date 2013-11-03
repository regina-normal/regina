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

tabs :
	cd $(srcdir); ./replacetabs `find .. -name "*.cpp" -o -name "*.h" -o -name "*.tcc"`
	cd $(srcdir); ./replacetabs `find .. -name "*.txt"`
	cd $(srcdir); ./replacetabs ../qtui/doc/regina/*.docbook
	cd $(srcdir); ./replacetabs ../qtui/doc/regina-xml/*.docbook
	cd $(srcdir); ./replacetabs ../pylib/*.py

