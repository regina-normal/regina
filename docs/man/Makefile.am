##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

man_MANS = \
  regconvert.1 \
  regfiledump.1 \
  regfiletype.1 \
  @PACKAGE@.1 \
  sigcensus.1 \
  tricensus.1 \
  regina.conf.5

SUFFIXES = .1 .2 .3 .4 .5 .6 .7 .8

%.1 %.2 %.3 %.4 %.5 %.6 %.7 %.8 :
	cd .. && @DOCBOOK2MAN@ -o man regina.sgml
	if [ "x@PACKAGE@" != "xregina" ]; then \
		mv regina.1 @PACKAGE@.1; \
	fi

mostlyclean-local :
	rm -f *.[1-8] manpage.*

