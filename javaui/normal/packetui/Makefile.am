##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

SUBDIRS = census packet surfaces triangulation angle

USE_CLASSPATH = @JAVAUI_CLASSPATH@

SUFFIXES = .java .class

%.class : %.java
	CLASSPATH="$(USE_CLASSPATH)" @JAVAC@ $<

java_sources = DefaultPacketEditor.java DefaultPacketViewer.java NewPacketDialog.java PacketComboBox.java PacketCreator.java PacketInfoTabbedPane.java PacketInfoUI.java PacketListCellRenderer.java PacketTabbedEditor.java PacketUI.java PacketUIManager.java UIHasChangesEvent.java UIHasChangesListener.java UnknownPacketViewer.java

# Note that $(java_classes) does _not_ include inner classes.
java_classes = DefaultPacketEditor.class DefaultPacketViewer.class NewPacketDialog.class PacketComboBox.class PacketCreator.class PacketInfoTabbedPane.class PacketInfoUI.class PacketListCellRenderer.class PacketTabbedEditor.class PacketUI.class PacketUIManager.class UIHasChangesEvent.class UIHasChangesListener.class UnknownPacketViewer.class

all-local : $(java_classes)

mostlyclean-local :
	rm -f *.class

EXTRA_DIST = $(java_sources) package.html

