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

java_sources = AlgebraViewer.java CompositionViewer.java GluingEditor.java NTriangulationCreator.java NTriangulationEditor.java SkeletonTableFrame.java SkeletonViewer.java SurfaceViewer.java TetrahedronDescriptionEditor.java TetrahedronDescriptionRenderer.java TetrahedronFaceEditor.java TetrahedronFaceRenderer.java TetrahedronModel.java TriangulationTableModel.java

# Note that $(java_classes) does _not_ include inner classes.
java_classes = AlgebraViewer.class CompositionViewer.class GluingEditor.class NTriangulationCreator.class NTriangulationEditor.class SkeletonTableFrame.class SkeletonViewer.class SurfaceViewer.class TetrahedronDescriptionEditor.class TetrahedronDescriptionRenderer.class TetrahedronFaceEditor.class TetrahedronFaceRenderer.class TetrahedronModel.class TriangulationTableModel.class

all-local : $(java_classes)

mostlyclean-local :
	rm -f *.class

EXTRA_DIST = $(java_sources) package.html

