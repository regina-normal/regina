##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

## ----------------------- Variables ---------------------------------

engine_index = index.html

## ----------------------- Custom targets ----------------------------

.PHONY : all-local clean-local

all-local : $(engine_index)
$(engine_index) :
	cd ../../engine && @DOXYGEN@ doc-files/docs.conf

mostlyclean-local :
	rm -f *.html *.png *.gif *.css *.dot

