##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

SUBDIRS = normal

jar_extra_files = normal/images/buttons/*.gif \
  normal/images/packet/*.gif \
  normal/images/main/*.jpg \
  normal/images/main/*.gif \
  LICENSE.txt

ui_jar = @PACKAGE_TARNAME@-@PACKAGE_VERSION@.jar

.PHONY : all-local clean-local

all-local :
	@JAR@ -cf $(ui_jar) `find normal -name "*.class"` $(jar_extra_files)

clean-local :
	rm -f $(ui_jar)

EXTRA_DIST = JNI.txt LICENSE.txt README.txt REQUIRES.txt TODO.txt \
  findtext overview.html

