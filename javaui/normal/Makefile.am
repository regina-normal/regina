##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

SUBDIRS = \
  algorithm \
  console \
  engine \
  exports \
  images \
  imports \
  mainui \
  options \
  packetfilter \
  packetui \
  progress

USE_CLASSPATH = @JAVAUI_CLASSPATH@

SUFFIXES = .java .class

%.class : %.java
	CLASSPATH="$(USE_CLASSPATH)" @JAVAC@ $<

java_sources = AppletShell.java Application.java ApplicationShell.java EngineLoader.java Images.java Shell.java

# Note that $(java_classes) does _not_ include inner classes.
java_classes = AppletShell.class Application.class ApplicationShell.class EngineLoader.class Images.class Shell.class

all-local : $(java_classes)

mostlyclean-local :
	rm -f *.class

EXTRA_DIST = $(java_sources) package.html

