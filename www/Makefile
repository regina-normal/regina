
# This Makefile is used in updating the website http://regina.sourceforge.net/.

# Only the original software author(s) should need to use this Makefile.

docs: clean
	cd .. && $(MAKE) docs
	cp -r ../docs .

clean:
	-rm -rf docs

.PHONY: docs clean
