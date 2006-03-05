##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

KDE_OPTIONS=noautodist

SUBDIRS = sigs

examplesdir = $(pkgdatadir)/examples

examples_DATA = README.txt \
  closed-hyp-census.rga closed-or-census.rga \
  closed-or-census-large.rga closed-nor-census.rga \
  knot-link-census.rga sample-misc.rga \
  sig-3mfd-census.rga sig-prime-min-census.rga snappea-census.rga \
  progress.py progress.session \
  tree.py tree.session \
  triangulation.py triangulation.session

EXTRA_DIST = $(examples_DATA) gap-tests.rga

