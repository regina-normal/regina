
# This Makefile is used in updating the website http://regina.sourceforge.net/.

# Only the original software author(s) should need to use this Makefile.

docs: clean
	cd docs && meinproc \
		--stylesheet /usr/share/apps/ksgmltools2/customization/kde-web.xsl \
		index.docbook

clean:
	-rm -f docs/*.html

.PHONY: docs clean
