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

java_sources = ConsoleUtils.java JPythonConsole.java JPythonConsoleFrame.java JPythonPacketConsole.java JPythonUtils.java

# Note that $(java_classes) does _not_ include inner classes.
java_classes = ConsoleUtils.class JPythonConsole.class JPythonConsoleFrame.class JPythonPacketConsole.class JPythonUtils.class

all-local : $(java_classes)

mostlyclean-local :
	rm -f *.class

EXTRA_DIST = $(java_sources) package.html

