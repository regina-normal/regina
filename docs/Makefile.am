##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

SUBDIRS = engine javaui images

## ----------------------- Variables ---------------------------------

USE_CLASSPATH = @JAVAUI_CLASSPATH@

sgml_index = regina.sgml
sgml_dsl = regina.dsl

engine_index = engine/index.html
engine_conf = ../engine/doc-files/docs.conf

javaui_index = javaui/index.html
javaui_overview  = overview.html
javaui_title = 'Regina - Java User Interface'
javaui_bottom =  'Copyright &copy; 1999-2002, Ben Burton<br>This software is released under the GNU Public License.<br>For further information, or to submit a bug or other problem, please contact Ben Burton (<a href="mailto:benb@acm.org">benb@acm.org</a>).'

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

engine : $(engine_index)
$(engine_index) : $(engine_conf)
	cd ../engine && @DOXYGEN@ $(engine_conf)

javaui : $(javaui_index)
$(javaui_index) :
	cd ../javaui && CLASSPATH="$(USE_CLASSPATH)" @JAVADOC@ -d ../docs/javaui \
		-overview $(javaui_overview) -windowtitle $(javaui_title) \
		-doctitle $(javaui_title) -bottom $(javaui_bottom) -package -use \
		`find normal -name package.html | sed -e 's#/package.html$$##'`

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
	rm -f engine/*.html engine/*.png engine/*.gif engine/*.css engine/*.dot
	rm -f javaui/*.html javaui/*.css javaui/package-list
	rm -rf javaui/normal

## ----------------------- Normal Makefile.am stuff ------------------

EXTRA_DIST = LICENSE.txt README.txt $(sgml_sources) \
  basman.jpg basman.png \
  largeicon.jpg largeicon.png \
  normalicon.gif normalicon.png \
  title.jpg title.png \
  regina.hs regina.jhm

