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

java_sources = NJNINormalSurface.java NJNINormalSurfaceList.java NJNISurfaceFilter.java NJNISurfaceFilterCombination.java NJNISurfaceFilterProperties.java NJNISurfaceSubset.java

# Note that $(java_classes) does _not_ include inner classes.
java_classes = NJNINormalSurface.class NJNINormalSurfaceList.class NJNISurfaceFilter.class NJNISurfaceFilterCombination.class NJNISurfaceFilterProperties.class NJNISurfaceSubset.class

all-local : $(java_classes)

mostlyclean-local :
	rm -f *.class

EXTRA_DIST = $(java_sources) package.html

