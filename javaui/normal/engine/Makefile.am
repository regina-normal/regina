##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

SUBDIRS = implementation \
  algebra \
  angle \
  file \
  maths \
  packet \
  progress \
  split \
  subcomplex \
  surfaces \
  triangulation \
  utilities

USE_CLASSPATH = @JAVAUI_CLASSPATH@

SUFFIXES = .java .class

%.class : %.java
	CLASSPATH="$(USE_CLASSPATH)" @JAVAC@ $<

java_sources = Engine.java ShareableObject.java

# Note that $(java_classes) does _not_ include inner classes.
java_classes = Engine.class ShareableObject.class

all-local : $(java_classes)

mostlyclean-local :
	rm -f *.class

EXTRA_DIST = $(java_sources) package.html

