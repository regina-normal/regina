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

java_sources = CoordinateViewer.java Coordinates.java MatchingEquationsViewer.java NNormalSurfaceListCreator.java NNormalSurfaceListViewer.java NSFCombinationEditor.java NSFPropertiesEditor.java NSurfaceFilterCreator.java NSurfaceFilterEditor.java

# Note that $(java_classes) does _not_ include inner classes.
java_classes = CoordinateViewer.class Coordinates.class MatchingEquationsViewer.class NNormalSurfaceListCreator.class NNormalSurfaceListViewer.class NSFCombinationEditor.class NSFPropertiesEditor.class NSurfaceFilterCreator.class NSurfaceFilterEditor.class

all-local : $(java_classes)

mostlyclean-local :
	rm -f *.class

EXTRA_DIST = $(java_sources) package.html

