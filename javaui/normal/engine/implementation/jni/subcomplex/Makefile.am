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

java_sources = NJNIAugTriSolidTorus.java NJNILayeredChain.java NJNILayeredChainPair.java NJNILayeredLensSpace.java NJNILayeredLoop.java NJNILayeredSolidTorus.java NJNILensSpace.java NJNIPillowTwoSphere.java NJNIPlugTriSolidTorus.java NJNISFS.java NJNISnappedBall.java NJNISnappedTwoSphere.java NJNISpiralSolidTorus.java NJNITriSolidTorus.java

# Note that $(java_classes) does _not_ include inner classes.
java_classes = NJNIAugTriSolidTorus.class NJNILayeredChain.class NJNILayeredChainPair.class NJNILayeredLensSpace.class NJNILayeredLoop.class NJNILayeredSolidTorus.class NJNILensSpace.class NJNIPillowTwoSphere.class NJNIPlugTriSolidTorus.class NJNISFS.class NJNISnappedBall.class NJNISnappedTwoSphere.class NJNISpiralSolidTorus.class NJNITriSolidTorus.class

all-local : $(java_classes)

mostlyclean-local :
	rm -f *.class

EXTRA_DIST = $(java_sources) package.html

