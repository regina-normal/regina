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

java_sources = AboutBox.java FilePane.java LibraryPane.java NormalFrame.java PacketPane.java PacketTreeCellRenderer.java PacketTreeNode.java TagEditor.java TopologyPane.java

# Note that $(java_classes) does _not_ include inner classes.
java_classes = AboutBox.class FilePane.class LibraryPane.class NormalFrame.class PacketPane.class PacketTreeCellRenderer.class PacketTreeNode.class TagEditor.class TopologyPane.class

all-local : $(java_classes)

mostlyclean-local :
	rm -f *.class

EXTRA_DIST = $(java_sources) package.html

