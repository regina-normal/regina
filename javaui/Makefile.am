##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

SUBDIRS = normal

## ----------------------- Variables ---------------------------------

USE_CLASSPATH = @JAVAUI_CLASSPATH@

jar_extra_files = normal/images/buttons/*.gif \
  normal/images/packet/*.gif \
  normal/images/main/*.jpg \
  normal/images/main/*.gif \
  LICENSE.txt

ui_jar = @PACKAGE@-@PACKAGE_VERSION@.jar
ui_link = @PACKAGE@.jar

pkgjardir = $(pkgdatadir)/jar

## ----------------------- Custom targets ----------------------------

.PHONY : jni jni-clean \
  all-local clean-local install-data-local uninstall-local

all-local :
	@JAR@ -cf $(ui_jar) `find normal -name "*.class"` $(jar_extra_files)
	rm -f $(ui_link)
	@LN_S@ $(ui_jar) $(ui_link)

mostlyclean-local :
	rm -f *.jar

install-data-local :
	# Install jars if they're present.
	if [ -e "$(ui_jar)" ]; then \
		$(mkinstalldirs) $(DESTDIR)$(pkgjardir); \
		$(INSTALL_DATA) $(ui_jar) $(DESTDIR)$(pkgjardir); \
		rm -f $(DESTDIR)$(pkgjardir)/$(ui_link); \
		cd $(DESTDIR)$(pkgjardir) && @LN_S@ $(ui_jar) $(ui_link); \
	fi

uninstall-local :
	rm -f $(DESTDIR)$(pkgjardir)/$(ui_jar) $(DESTDIR)$(pkgjardir)/$(ui_link)

jni : all
	for i in `find normal/engine/implementation/jni -name "*.java"`; do \
		_class="`echo "$$i" | sed -e 's:/:.:g;s:\.java$$::'`"; \
		_header="`echo "$$i" | sed -e 's:^normal/engine/implementation/jni:../engine/jni/engine:;s:\.java$$:.h:'`"; \
		echo "Generating $$_header..."; \
		CLASSPATH="$(USE_CLASSPATH)" @JAVAH@ -jni -o "$$_header" "$$_class"; \
	done

jni-clean :
	for i in `find normal/engine/implementation/jni -name "*.java"`; do \
		_header="`echo "$$i" | sed -e 's:^normal/engine/implementation/jni:../engine/jni/engine:;s:\.java$$:.h:'`"; \
		echo "Removing $$_header..."; \
		rm -f "$$_header"; \
	done

## ----------------------- Normal Makefile.am stuff ------------------

EXTRA_DIST = JNI.txt LICENSE.txt README.txt REQUIRES.txt TODO.txt \
  findtext overview.html

