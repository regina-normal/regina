##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

man_MANS = \
  regconcat.1 \
  regconvert.1 \
  regfiledump.1 \
  regfiletype.1 \
  sigcensus.1 \
  tricensus.1 \
  tricensus-manager.1 \
  regina-kde.1 \
  regina-python.1

SUFFIXES = .1 .2 .3 .4 .5 .6 .7 .8

%.1 %.2 %.3 %.4 %.5 %.6 %.7 %.8 :
	cd ../../kdeui/doc/en && @DOCBOOK2MAN@ -o ../../../docs/man manonly.docbook

mostlyclean-local :
	rm -f *.[1-8] manpage.*

