##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

USE_CLASSPATH = @JAVAUI_CLASSPATH@

SUFFIXES = .java .class

%.class : %.java
	CLASSPATH="$(USE_CLASSPATH)" @JAVAC@ $<

java_sources = Exporter.java ReginaBinaryExporter.java ReginaExporter.java ScriptExporter.java SnapPeaExporter.java

# Note that $(java_classes) does _not_ include inner classes.
java_classes = Exporter.class ReginaBinaryExporter.class ReginaExporter.class ScriptExporter.class SnapPeaExporter.class

all-local : $(java_classes)

mostlyclean-local :
	rm -f *.class

EXTRA_DIST = $(java_sources) package.html

