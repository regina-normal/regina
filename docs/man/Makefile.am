##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

KDE_OPTIONS=noautodist

man_MANS = \
  regconcat.1 \
  regconvert.1 \
  regfiledump.1 \
  regfiletype.1 \
  sigcensus.1 \
  tricensus.1 \
  tricensus-manager.1 \
  tricensus-mpi.1 \
  trisetcmp.1 \
  regina-kde.1 \
  regina-python.1

EXTRA_DIST = $(man_MANS)

# Only regenerate manpages if you're the maintainer and you have a
# working docbook2man.
if MAINTAINER_MODE
if HAVE_DOCBOOK2MAN
man_docbook = \
  ../../kdeui/doc/en/manonly.docbook \
  ../../kdeui/doc/en/reginaman.docbook \
  ../../kdeui/doc/en/utils.docbook

$(man_MANS) : $(man_docbook)
	cd ../../kdeui/doc/en && @DOCBOOK2MAN@ -o ../../../docs/man manonly.docbook
endif
endif

mostlyclean-local :
	rm -f manpage.links manpage.refs

