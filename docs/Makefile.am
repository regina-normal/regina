##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

SUBDIRS = @REGINA_BUILD_DOCSENGINE@ @REGINA_BUILD_DOCSJAVAUI@ \
  @REGINA_BUILD_DOCSMAN@ dataformat images

## ----------------------- Variables ---------------------------------

sgml_index = regina.sgml
sgml_dsl = regina.dsl

sample_html = index.html
sample_rtf = regina.rtf
sample_tex = regina.tex
sample_pdf = regina.pdf

sample_engine = engine/index.html
sample_javaui = javaui/index.html

sample_all_refs = $(sample_html) $(sample_engine) $(sample_javaui)
ext_all_refs = css dot gif hs html jhm jpg png txt

doc_jar = @PACKAGE@-docs-@PACKAGE_VERSION@.jar
doc_link = @PACKAGE@-docs.jar

doc_build_dir = doc-jar-tmp

pkgdocdir = $(pkgdatadir)/doc
pkgjardir = $(pkgdatadir)/jar

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

.PHONY : html tex pdf rtf jar

all-local : @REGINA_BUILD_DOCSHTML@ @REGINA_BUILD_DOCSJAR@

$(sample_engine) :
	cd engine && $(MAKE)
$(sample_javaui) :
	cd javaui && $(MAKE)

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

jar : $(doc_jar) $(doc_link)
$(doc_jar) : $(sample_all_refs)
	rm -rf $(doc_build_dir)
	mkdir $(doc_build_dir)
	for ext in $(ext_all_refs); do \
		for i in `find . -name "*.$$ext"`; do \
			_destdir="$(doc_build_dir)/normal/docs/`dirname "$$i"`"; \
			$(mkinstalldirs) "$$_destdir"; \
			$(INSTALL_DATA) $$i "$$_destdir"; \
		done; \
	done
	cd $(doc_build_dir) && @JAR@ -cf ../$(doc_jar) normal/docs
	rm -rf $(doc_build_dir)
$(doc_link) : $(doc_jar)
	rm -f $(doc_link)
	@LN_S@ $(doc_jar) $(doc_link)

mostlyclean-local :
	rm -f *.html
	rm -f *.tex *.aux *.dvi *.out *.log
	rm -f *.pdf *.rtf
	rm -f *.jar

# Install the individual documentation files.
# Install the documentation jar if it's been built.
install-data-local :
	for ext in $(ext_all_refs); do \
		for i in `find . -name "*.$$ext"`; do \
			_destdir="$(DESTDIR)$(pkgdocdir)/`dirname "$$i"`"; \
			$(mkinstalldirs) "$$_destdir"; \
			$(INSTALL_DATA) $$i "$$_destdir"; \
		done; \
	done
	if [ -e "$(doc_jar)" ]; then \
		$(mkinstalldirs) $(DESTDIR)$(pkgjardir); \
		$(INSTALL_DATA) $(doc_jar) $(DESTDIR)$(pkgjardir); \
		rm -f $(DESTDIR)$(pkgjardir)/$(doc_link); \
		cd $(DESTDIR)$(pkgjardir) && @LN_S@ $(doc_jar) $(doc_link); \
	fi

uninstall-local :
	rm -rf $(DESTDIR)$(pkgdocdir)
	rm -f $(DESTDIR)$(pkgjardir)/$(doc_jar) $(DESTDIR)$(pkgjardir)/$(doc_link)

## ----------------------- Normal Makefile.am stuff ------------------

EXTRA_DIST = LICENSE.txt README.txt $(sgml_sources) \
  basman.jpg basman.png \
  largeicon.jpg largeicon.png \
  normalicon.gif normalicon.png \
  title.jpg title.png \
  regina.hs regina.jhm

