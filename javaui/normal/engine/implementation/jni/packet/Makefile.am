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

java_sources = NJNIContainer.java NJNIPacket.java NJNIPacketTagEnumerator.java NJNIScript.java NJNIText.java

# Note that $(java_classes) does _not_ include inner classes.
java_classes = NJNIContainer.class NJNIPacket.class NJNIPacketTagEnumerator.class NJNIScript.class NJNIText.class

all-local : $(java_classes)

mostlyclean-local :
	rm -f *.class

EXTRA_DIST = $(java_sources) package.html

