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

java_sources = NJNIBoundaryComponent.java NJNIComponent.java NJNIEdge.java NJNIFace.java NJNITetrahedron.java NJNITriangulation.java NJNIVertex.java

# Note that $(java_classes) does _not_ include inner classes.
java_classes = NJNIBoundaryComponent.class NJNIComponent.class NJNIEdge.class NJNIFace.class NJNITetrahedron.class NJNITriangulation.class NJNIVertex.class

all-local : $(java_classes)

mostlyclean-local :
	rm -f *.class

EXTRA_DIST = $(java_sources) package.html

