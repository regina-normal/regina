##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

## ----------------------- Variables ---------------------------------

sgml_index = regina.sgml
sgml_dsl = regina.dsl

sample_html = index.html
sample_man = regina.1
sample_rtf = regina.rtf
sample_tex = regina.tex
sample_pdf = regina.pdf

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

.PHONY : engine javaui html man tex pdf rtf \
  all-local clean-local

all-local : @REGINA_BUILD_DOCSENGINE@ @REGINA_BUILD_DOCSJAVAUI@ @REGINA_BUILD_DOCSHTML@ @REGINA_BUILD_DOCSMAN@

engine :

javaui :

html : $(sample_html)
$(sample_html) : $(sgml_sources)
	@JADE@ -t sgml -i html -d $(sgml_dsl)\#html $(sgml_index)

man : $(sample_man)
$(sample_man) : $(sgml_sources)
	@DOCBOOK2MAN@ $(sgml_index)

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

clean-local :
	rm -f *.html
	rm -f *.[1-8] manpage.*
	rm -f *.tex *.aux *.dvi *.out *.log
	rm -f *.pdf *.rtf

## ----------------------- Normal Makefile.am stuff ------------------

EXTRA_DIST = LICENSE.txt README.txt $(sgml_sources) \
  basman.jpg basman.png \
  largeicon.jpg largeicon.png \
  normalicon.gif normalicon.png \
  title.jpg title.png \
  regina.hs regina.jhm

