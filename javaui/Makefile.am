##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

SUBDIRS = normal

USE_CLASSPATH = @JAVAUI_CLASSPATH@

jar_extra_files = normal/images/buttons/*.gif \
  normal/images/packet/*.gif \
  normal/images/main/*.jpg \
  normal/images/main/*.gif \
  LICENSE.txt

ui_jar = @PACKAGE_TARNAME@-@PACKAGE_VERSION@.jar
ui_link = @PACKAGE_TARNAME@.jar

.PHONY : all-local clean-local jni jni-clean

all-local :
	@JAR@ -cf $(ui_jar) `find normal -name "*.class"` $(jar_extra_files)
	rm -f $(ui_link)
	@LN_S@ $(ui_jar) $(ui_link)

clean-local :
	rm -f $(ui_jar) $(ui_link)

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

EXTRA_DIST = JNI.txt LICENSE.txt README.txt REQUIRES.txt TODO.txt \
  findtext overview.html

