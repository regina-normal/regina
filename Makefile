
# Type "make" to see a list of available targets and their descriptions.

# All system-specific variables are in Makefile.options and should be
# edited there.  Nothing in this file should need editing.

# ------------------------- Variables --------------------------------

-include Makefile.options

# Platform-specific flags:

ifeq ($(OSTYPE),)
	ifeq ($(wildcard /usr/bin/cygpath.exe),/usr/bin/cygpath.exe)
		export OSTYPE = cygwin
	else
		export OSTYPE = $(shell uname -s)
	endif
endif

# Global variables:

include Makefile.sources

export BIN_DIR = bin
export BASE_DOC_DIR = docs

export cs = $(CLASSPATH_SEP)
export other_classes = \
	$(BTOOLS)$(cs)$(JYTHON)$(cs)$(EXTRA_CLASSES)

export LICENSE = LICENSE.txt

# Local variables:

empty_dirs = $(DIST_DIR) $(BIN_DIR) engine javaui
DOC_LICENSE = $(BASE_DOC_DIR)/LICENSE.txt
DOC_BUILD_DIR = doc-build-tmp
APP_CLASS = normal.Application

# ------------------------- Rules ----------------------------------

# Allow unrecognised targets in certain subdirectories:

docs/% :
	cd docs && $(MAKE) $*
test/% :
	cd test && $(MAKE) $*
utils/% : binenginelib
	cd utils && $(MAKE) $*

# ------------------------- Targets ----------------------------------

helpgeneral :
	@$(ECHO); $(ECHO) "General targets only:"; $(ECHO); \
		$(CAT) Makefile-help-gen; $(ECHO)
help :
	@$(ECHO); $(ECHO) "All targets:"; $(ECHO); \
		$(CAT) Makefile-help; $(ECHO)
helpbin :
	@$(ECHO); $(ECHO) "Binary targets (made in bin/):"; $(ECHO); \
		$(GREP) "^bin" Makefile-help; $(ECHO)
helpdocs :
	@$(ECHO); $(ECHO) "Documentation targets (made beneath docs/):"; $(ECHO); \
		$(GREP) "^docs" Makefile-help; $(ECHO)
helpprep :
	@$(ECHO); $(ECHO) "Preparation targets:"; $(ECHO); \
		$(GREP) "^prep" Makefile-help; $(ECHO)
helprun :
	@$(ECHO); $(ECHO) "Run targets:"; $(ECHO); \
		$(GREP) "^run" Makefile-help; \
		$(GREP) "^debug" Makefile-help; \
		$(ECHO)

.PHONY : help helpgeneral helpbin helpdocs helpprep helprun \
	binengine binenginejni binenginelib \
	binjava binjavaui binjavajni binjni bindocs bin \
	docsengine docsjava docshtml docsman docsrtf docstex docsdvi docspdf docs \
	prepengine prepjni prepcleanjni prepclean prep \
	run runconsole runtext debug \
	clean purge

binengine : binenginelib binenginejni
binenginejni : prepjni
	cd engine && $(MAKE) binjni
binenginelib :
	cd engine && $(MAKE) binlib
binjava : binjavaui binjavajni
binjavaui :
	cd javaui && $(MAKE) uijar
binjavajni :
	cd javaui && $(MAKE) jnijar
binjni : binenginejni binjavaui binjavajni
bindocs : $(DOC_JAR)
$(DOC_JAR) : docs
	-rm -rf $(DOC_BUILD_DIR)
	$(MKDIR) $(DOC_BUILD_DIR)
	$(MKDIR) $(DOC_BUILD_DIR)/normal
	$(MKDIR) $(DOC_BUILD_DIR)/normal/docs
	$(COPY) -r docs/* $(DOC_BUILD_DIR)/normal/docs
	cd $(DOC_BUILD_DIR) && $(JAR) -cf ../$(DOC_JAR) \
		$(patsubst %,normal/docs/%, $(misc_doc_mask) $(gen_doc_mask))
	rm -rf $(DOC_BUILD_DIR)
bin : binengine binjava

docsengine :
	cd engine && $(MAKE) docs
docsjava :
	cd javaui && $(MAKE) docs
docshtml :
	cd docs && $(MAKE) html
docsrtf:
	cd docs && $(MAKE) rtf
docstex:
	cd docs && $(MAKE) tex
docsdvi:
	cd docs && $(MAKE) dvi
docspdf:
	cd docs && $(MAKE) pdf
docsman :
	cd docs && $(MAKE) man
docs : docsengine docsjava docshtml docsman $(DOC_LICENSE)
$(DOC_LICENSE) : $(LICENSE)
	$(COPY) $(LICENSE) $(DOC_LICENSE)

prepengine : prepjni
prepjni : binjavaui binjavajni
	cd engine && $(MAKE) prepjni
prepcleanjni :
	cd engine && $(MAKE) prepcleanjni
prepclean : prepcleanjni
prep : prepengine

run : binenginejni binjavaui binjavajni
	./regina --gui --jni
runconsole : binenginejni binjavaui binjavajni
	./regina --console --jni
runtext : binenginejni binjavaui binjavajni
	./regina --text --jni
debug : binenginejni binjavaui binjavajni
	export REGINA_JAVA="$(JDB)" && ./regina --gui --jni

clean :
	cd engine && $(MAKE) clean
	cd javaui && $(MAKE) clean
	cd docs && $(MAKE) clean

purge : clean
	-rm $(ENGINE_JNI) $(ENGINE_LIB)
	-rm $(JAVA_UI) $(JAVA_JNI)
	-rm $(DOC_JAR)

Makefile.options :
	@echo
	@echo "Makefile.options is missing."
	@echo "Try taking one of the sample files (Makefile.options.*) and"
	@echo "    modifying it to suit your system."
	@echo
	@false
