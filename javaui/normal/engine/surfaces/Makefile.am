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

java_sources = NNormalSurface.java NNormalSurfaceList.java NSurfaceFilter.java NSurfaceFilterCombination.java NSurfaceFilterProperties.java NSurfaceSet.java NSurfaceSubset.java

# Note that $(java_classes) does _not_ include inner classes.
java_classes = NNormalSurface.class NNormalSurfaceList.class NSurfaceFilter.class NSurfaceFilterCombination.class NSurfaceFilterProperties.class NSurfaceSet.class NSurfaceSubset.class

all-local : $(java_classes)

mostlyclean-local :
	rm -f *.class

EXTRA_DIST = $(java_sources) package.html

