##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

KDE_OPTIONS=noautodist

## ----------------------- Variables ---------------------------------

engine_index = index.html

extensions = css dot gif html jpg png txt

pkgdocdir = $(pkgdatadir)/engine-docs

## ----------------------- Custom targets ----------------------------

all-local : $(engine_index)
$(engine_index) :
	cd ../../engine && @DOXYGEN@ doc-files/docs.conf

mostlyclean-local :
	rm -f *.html *.png *.gif *.css *.dot

install-data-local :
	$(mkinstalldirs) "$(DESTDIR)$(pkgdocdir)"
	for i in $(extensions); do \
		for j in `find . -name "*.$$i"`; do \
			$(INSTALL_DATA) $$j "$(DESTDIR)$(pkgdocdir)"; \
		done; \
	done

uninstall-local :
	rm -rf $(DESTDIR)$(pkgdocdir)

