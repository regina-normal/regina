
# Type "make" to see a list of available targets and their descriptions.

# ------------------------------ Options --------------------------------

# Variables in this section should be changed to reflect the current
# system and release.

# Specify here the version of Regina being released:

REGINA_VERSION = 2.2

# ------------------------------ Variables ------------------------------

include ../Makefile.options

include ../Makefile.sources
include ../engine/Makefile.sources
include ../javaui/Makefile.sources

REGINA_DIR = regina-$(REGINA_VERSION)

SRC_ARCHIVE = regina-$(REGINA_VERSION)_src.tgz
BIN_ARCHIVE = regina-$(REGINA_VERSION)_bin.tgz
DOC_ARCHIVE = regina-$(REGINA_VERSION)_doc.tgz
DOC_JAR_FINAL = regina-docs-$(REGINA_VERSION).jar

DOC_OVERVIEW = overview.html
LICENSE = LICENSE.txt
DOC_HTML_DIR = docs

empty_src_dirs = bin $(DOC_HTML_DIR)/engine $(DOC_HTML_DIR)/javaui
dist_mask = Makefile* *.txt *.cpp insertstubreplace replacetabs stub*

engine_dirs = $(dirs_engine_engine) $(dirs_engine_jni) $(dirs_engine_corba)
engine_src = $(engine_dirs:=/*.cpp)
engine_hdr = $(engine_dirs:=/*.h)

java_package_dirs_ui = $(subst .,/,$(packages_ui))
java_package_dirs_all = $(subst .,/,$(packages_ui)) \
	$(subst .,/,$(packages_jni)) $(subst .,/,$(packages_corba))
java_mask = $(java_package_dirs_all:=/*.java)
java_doc_sources = $(java_package_dirs_ui:=/package.html) $(DOC_OVERVIEW)

# ------------------------------ Targets --------------------------------

help :
	@cat Makefile-help

.PHONY : distsrc distbin distdocs dist stubs tabs clean purge help

distsrc : $(SRC_ARCHIVE)
$(SRC_ARCHIVE):
	-rm $(REGINA_DIR)
	ln -s .. $(REGINA_DIR)
	$(ARCHIVE) $(SRC_ARCHIVE) \
		$(patsubst %,$(REGINA_DIR)/engine/%, \
			$(engine_src) $(engine_hdr) $(engine_misc_src_mask)) \
		$(patsubst %,$(REGINA_DIR)/javaui/%, \
			$(java_mask) $(java_doc_sources) $(java_aux_mask) \
			$(java_misc_src_mask)) \
		$(patsubst %,$(REGINA_DIR)/$(DOC_HTML_DIR)/%, $(misc_doc_mask)) \
		$(patsubst %,$(REGINA_DIR)/%, \
			$(scripts) $(misc_src_mask) idl \
			$(empty_src_dirs:=/.keepme) $(dist_mask:%=dist/%))
	-rm $(REGINA_DIR)

distbin : $(BIN_ARCHIVE)
$(BIN_ARCHIVE) :
	cd .. && $(MAKE) bin
	-rm $(REGINA_DIR)
	ln -s .. $(REGINA_DIR)
	$(ARCHIVE) $(BIN_ARCHIVE) $(patsubst %,$(REGINA_DIR)/%, \
		$(ENGINE_JNI) $(ENGINE_CORBA) \
		$(JAVA_UI) $(JAVA_JNI) $(JAVA_CORBA) \
		$(misc_bin_mask) $(scripts))
	-rm $(REGINA_DIR)

distdocs : $(DOC_ARCHIVE) $(DOC_JAR_FINAL)
$(DOC_ARCHIVE):
	cd .. && $(MAKE) docs
	-rm $(REGINA_DIR)
	ln -s .. $(REGINA_DIR)
	$(ARCHIVE) $(DOC_ARCHIVE) $(REGINA_DIR)/docs $(REGINA_DIR)/$(LICENSE)
	-rm $(REGINA_DIR)
$(DOC_JAR_FINAL):
	cd .. && $(MAKE) bindocs
	$(COPY) ../$(DOC_JAR) $(DOC_JAR_FINAL)

dist : distsrc distbin distdocs

stubs : insertstub
	./insertstubreplace "/* end stub */" stub.engine \
		`find ../engine -name "*.cpp"`
	./insertstubreplace "/* end stub */" stub.engine \
		`find ../engine -name "*.h"`
	./insertstubreplace "/* end stub */" stub.javaui \
		`find ../javaui -name "*.java"`
	./insertstubreplace "/* end stub */" stub.idl \
		`find ../idl -name "*.idl"`
tabs :
	./replacetabs `find ../engine -name "*.cpp"`
	./replacetabs `find ../engine -name "*.h"`
	./replacetabs `find ../javaui -name "*.java"`
	./replacetabs `find ../idl -name "*.idl"`
	./replacetabs ../CHANGES.txt ../README.txt
	./replacetabs *.txt *.cpp

clean :
	cd .. && $(MAKE) clean
	-rm insertstub
	-rm $(REGINA_DIR)

purge : clean
	cd .. && $(MAKE) purge
	-rm $(SRC_ARCHIVE) $(BIN_ARCHIVE) $(DOC_ARCHIVE) $(DOC_JAR_FINAL)
