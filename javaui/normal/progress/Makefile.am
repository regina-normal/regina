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

java_sources = ProgressComponent.java ProgressDialog.java ProgressMessage.java ProgressPercent.java

# Note that $(java_classes) does _not_ include inner classes.
java_classes = ProgressComponent.class ProgressDialog.class ProgressMessage.class ProgressPercent.class

all-local : $(java_classes)

mostlyclean-local :
	rm -f *.class

EXTRA_DIST = $(java_sources) package.html

