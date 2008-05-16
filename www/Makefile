
# This Makefile is used in updating the website http://regina.sourceforge.net/.

# Only the original software author(s) should need to use this Makefile.

VERSION = $(shell grep ^AM_INIT_AUTOMAKE ../configure.ac | sed -e 's/^.*regina, .//' -e 's/].$$//')
HANDBOOK_DIR = regina-handbook-$(VERSION)
HANDBOOK_TGZ = $(HANDBOOK_DIR).tgz
HANDBOOK_ZIP = $(HANDBOOK_DIR).zip
TMPFILE = .tmp.html

www: handbook
	if test -d website; then rm -rf website; fi
	if test -e website; then false; fi
	mkdir website

	cp -p *.html *.jpg *.css website
	cp -pr install website
	cp -pr shots website
	cp -pr ../examples website/census

	cd ../docs/engine && $(MAKE)
	cp -pr ../docs/engine website/engine-docs

	cd website && tar -zxf "../$(HANDBOOK_TGZ)"
	mv "website/$(HANDBOOK_DIR)" website/docs

	rm -rf `find website -type d -name .svn`
	rm -f `find website -type f -name Makefile`
	rm -f `find website -type f -name Makefile.in`
	rm -f `find website -type f -name Makefile.am`

	tar -zcf website.tgz website
	rm -rf website

handbook:
	if test -d "$(HANDBOOK_DIR)"; then rm -rf "$(HANDBOOK_DIR)"; fi
	if test -e "$(HANDBOOK_DIR)"; then false; fi
	cp -pr ../kdeui/doc/en "$(HANDBOOK_DIR)"

	cd "$(HANDBOOK_DIR)" && meinproc \
		--stylesheet /usr/share/apps/ksgmltools2/customization/kde-web.xsl \
		index.docbook
	cp -pr /usr/share/doc/kde/HTML/en/common "$(HANDBOOK_DIR)/common"

	cd "$(HANDBOOK_DIR)" && rm -f *.docbook Makefile*
	cd "$(HANDBOOK_DIR)" && rm -rf .svn
	cd "$(HANDBOOK_DIR)" && for i in *.html; do \
		cat "$$i" | sed -e 's/\.\.\/common\//common\//g' > "$(TMPFILE)"; \
		cp "$(TMPFILE)" "$$i"; \
		rm -f "$(TMPFILE)" ; \
	done

	tar -zcf "$(HANDBOOK_TGZ)" "$(HANDBOOK_DIR)"
	zip -r "$(HANDBOOK_ZIP)" "$(HANDBOOK_DIR)"
	rm -rf "$(HANDBOOK_DIR)"

clean:
	if test -d website; then rm -rf website; fi
	if test -d "$(HANDBOOK_DIR)"; then rm -rf "$(HANDBOOK_DIR)"; fi

.PHONY: www handbook clean
