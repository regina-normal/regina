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

java_sources = AllowAll.java PacketFilter.java SingleClass.java Standalone.java TwoClasses.java

# Note that $(java_classes) does _not_ include inner classes.
java_classes = AllowAll.class PacketFilter.class SingleClass.class Standalone.class TwoClasses.class

all-local : $(java_classes)

mostlyclean-local :
	rm -f *.class

EXTRA_DIST = $(java_sources) package.html

