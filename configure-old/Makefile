
# This makefile should only ever be called from ../Makefile.
# See ../Makefile for further details.

# ------------------------ Variables ---------------------------------

manifest = Manifest

sources = $(wildcard *.java)
made_classes = $(sources:.java=.class)
runtime_jar = ../$(REG_CONF)

# ------------------------ Rules -------------------------------------

%.class : %.java
	$(JAVAC) $<

# ------------------------ Targets -----------------------------------

.PHONY : jar clean

$(LICENSE) : ../$(LICENSE)
	$(COPY) ../$(LICENSE) $(LICENSE)

jar : $(runtime_jar)
$(runtime_jar) : $(made_classes) $(LICENSE)
	$(JAR) -cmf $(manifest) $(runtime_jar) *.class $(LICENSE)

clean :
	-rm *.class

