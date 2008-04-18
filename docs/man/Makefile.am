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
  tricensus-mpi-status.1 \
  trisetcmp.1 \
  regina-kde.1 \
  regina-python.1

EXTRA_DIST = $(man_MANS)

# Only regenerate manpages if you're the maintainer and you have a
# working docbook2man.
if COND_RELEASE_MODE
if HAVE_DOCBOOK2MAN
man_docbook = \
  $(top_srcdir)/kdeui/doc/en/manonly.docbook \
  $(top_srcdir)/kdeui/doc/en/reginaman.docbook \
  $(top_srcdir)/kdeui/doc/en/submani18n.docbook \
  $(top_srcdir)/kdeui/doc/en/utils.docbook

# This might not be terribly portable (it uses features specific to GNU make),
# but then again it only ever gets called by the software author.
man_output = $(addprefix $(top_srcdir)/docs/man/,$(man_MANS))

# Update the man pages directly in the source tree, so they can be
# checked in to the repository and bundled up into the source tarball.
$(man_output) : $(man_docbook)
	cd $(top_srcdir)/kdeui/doc/en && \
		@DOCBOOK2MAN@ -o ../../../docs/man manonly.docbook
endif
endif

mostlyclean-local :
	rm -f $(top_srcdir)/docs/man/manpage.links \
		$(top_srcdir)/docs/man/manpage.refs

