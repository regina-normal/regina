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

java_sources = NAugTriSolidTorus.java NLayeredChain.java NLayeredChainPair.java NLayeredLensSpace.java NLayeredLoop.java NLayeredSolidTorus.java NLensSpace.java NPillowTwoSphere.java NPlugTriSolidTorus.java NSFS.java NSnappedBall.java NSnappedTwoSphere.java NSpiralSolidTorus.java NTriSolidTorus.java

# Note that $(java_classes) does _not_ include inner classes.
java_classes = NAugTriSolidTorus.class NLayeredChain.class NLayeredChainPair.class NLayeredLensSpace.class NLayeredLoop.class NLayeredSolidTorus.class NLensSpace.class NPillowTwoSphere.class NPlugTriSolidTorus.class NSFS.class NSnappedBall.class NSnappedTwoSphere.class NSpiralSolidTorus.class NTriSolidTorus.class

all-local : $(java_classes)

mostlyclean-local :
	rm -f *.class

EXTRA_DIST = $(java_sources) package.html

