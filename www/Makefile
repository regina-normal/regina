
# This Makefile is used in updating the website http://regina.sourceforge.net/.

# Only the original software author(s) should need to use this Makefile.

www:
	-rm -rf staging
	mkdir staging
	cp -p *.html *.jpg *.css staging
	cp -pr shots staging
	cp -pr ../examples staging/census
	cp -pr ../docs/engine staging/engine-docs
	cp -pr ../kdeui/doc/en staging/docs
	cp -pr /usr/share/doc/kde/HTML/en/common staging/common
	rm -rf `find staging -type d -name CVS`
	rm -rf `find staging -type d -name .svn`
	rm -f `find staging -type f -name .cvsignore`
	rm -f `find staging -type f -name Makefile`
	rm -f `find staging -type f -name Makefile.in`
	rm -f `find staging -type f -name Makefile.am`
	cd staging/docs && meinproc \
		--stylesheet /usr/share/apps/ksgmltools2/customization/kde-web.xsl \
		index.docbook

clean:
	-rm -rf staging

.PHONY: www clean
