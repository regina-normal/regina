##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

SUBDIRS = @REGINA_BUILD_DOCSENGINE@ @REGINA_BUILD_DOCSMAN@ dataformat images

## ----------------------- Variables ---------------------------------

sgml_index = regina.sgml
sgml_dsl = regina.dsl

sample_html = index.html
sample_rtf = regina.rtf
sample_tex = regina.tex
sample_pdf = regina.pdf

sample_engine = engine/index.html

sample_all_refs = $(sample_html) $(sample_engine)
ext_all_refs = css dot gif hs html jhm jpg png txt

pkgdocdir = $(pkgdatadir)/doc

## ----------------------- Sources -----------------------------------

sgml_sources = regina.dsl \
  angles.sgml \
  bibliography.sgml \
  featureset.sgml \
  jython.sgml \
  license.sgml \
  options.sgml \
  packet.sgml \
  regina.sgml \
  reginaman.sgml \
  surfaces.sgml \
  triangulations.sgml \
  trouble.sgml \
  tutorial.sgml \
  utils.sgml

## ----------------------- Custom targets ----------------------------

.PHONY : html tex pdf rtf

all-local : @REGINA_BUILD_DOCSHTML@

$(sample_engine) :
	cd engine && $(MAKE)

html : $(sample_html)
$(sample_html) : $(sgml_sources)
	@JADE@ -t sgml -i html -d $(sgml_dsl)\#html $(sgml_index)

tex : $(sample_tex)
$(sample_tex) : $(sgml_sources)
	@JADE@ -t tex -d $(sgml_dsl)\#print $(sgml_index)

pdf : $(sample_pdf)
$(sample_pdf) : $(sample_tex)
	-@PDFJADETEX@ $(sample_tex)
	-@PDFJADETEX@ $(sample_tex)
	-@PDFJADETEX@ $(sample_tex)

rtf : $(sample_rtf)
$(sample_rtf) : $(sgml_sources)
	@JADE@ -t rtf -d $(sgml_dsl)\#print $(sgml_index)

mostlyclean-local :
	rm -f *.html
	rm -f *.tex *.aux *.dvi *.out *.log
	rm -f *.pdf *.rtf

# Install the individual documentation files.
install-data-local :
	for ext in $(ext_all_refs); do \
		for i in `find . -name "*.$$ext"`; do \
			_destdir="$(DESTDIR)$(pkgdocdir)/`dirname "$$i"`"; \
			$(mkinstalldirs) "$$_destdir"; \
			$(INSTALL_DATA) $$i "$$_destdir"; \
		done; \
	done

uninstall-local :
	rm -rf $(DESTDIR)$(pkgdocdir)

## ----------------------- Normal Makefile.am stuff ------------------

EXTRA_DIST = LICENSE.txt README.txt $(sgml_sources) \
  basman.jpg basman.png \
  largeicon.jpg largeicon.png \
  normalicon.gif normalicon.png \
  title.jpg title.png \
  regina.hs regina.jhm

