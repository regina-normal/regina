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

java_sources = NBoundaryComponent.java NComponent.java NEdge.java NEdgeEmbedding.java NFace.java NFaceEmbedding.java NPerm.java NTetrahedron.java NTriangulation.java NVertex.java NVertexEmbedding.java

# Note that $(java_classes) does _not_ include inner classes.
java_classes = NBoundaryComponent.class NComponent.class NEdge.class NEdgeEmbedding.class NFace.class NFaceEmbedding.class NPerm.class NTetrahedron.class NTriangulation.class NVertex.class NVertexEmbedding.class

all-local : $(java_classes)

mostlyclean-local :
	rm -f *.class

EXTRA_DIST = $(java_sources) package.html

