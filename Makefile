
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
include idl/Makefile.sources

export src_idl = $(wildcard $(dirs_idl:=/*.idl))

export BIN_DIR = bin
export BASE_DOC_DIR = docs

export cs = $(CLASSPATH_SEP)
export other_classes = \
	$(BTOOLS)$(cs)$(JYTHON)$(cs)$(EXTRA_CLASSES)

export LICENSE = LICENSE.txt

# Local variables:

empty_dirs = $(DIST_DIR) $(BIN_DIR) engine javaui idl
DOC_LICENSE = $(BASE_DOC_DIR)/LICENSE.txt
DOC_BUILD_DIR = doc-build-tmp
APP_CLASS = normal.Application

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
	binengine binenginejni binenginecorba binenginelib \
	binjava binjavaui binjavajni binjavacorba \
	binjni bincorba bindocs bin \
	docsengine docsjava docshtml docsman docsrtf docstex docsdvi docspdf docs \
	prepengine prepenginejni prepenginecorba prepjava prepjavacorba \
		prepcorba prepjni prepcleancorba prepcleanjni prepclean prep \
	run runconsole runtext runjni runcorba runcorbaengine \
	debug debugjni debugcorba \
	clean purge

binengine : binenginelib binenginejni binenginecorba
binenginejni : prepenginejni
	cd engine && $(MAKE) binjni
binenginecorba : prepenginecorba
	cd engine && $(MAKE) bincorba
binenginelib :
	cd engine && $(MAKE) binlib
binjava : binjavaui binjavajni binjavacorba
binjavaui :
	cd javaui && $(MAKE) uijar
binjavajni :
	cd javaui && $(MAKE) jnijar
binjavacorba :
	cd javaui && $(MAKE) corbajar
binjni : binenginejni binjavaui binjavajni
bincorba : binenginecorba binjavaui binjavacorba
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

prepengine : prepenginejni prepenginecorba
prepenginejni : binjavaui binjavajni
	cd engine && $(MAKE) prepjni
prepenginecorba :
	cd engine && $(MAKE) prepcorba
prepjava : prepjavacorba
prepjavacorba :
	cd javaui && $(MAKE) prepcorba
prepcorba : prepenginecorba prepjavacorba
prepjni : prepenginejni
prepcleancorba :
	cd engine && $(MAKE) prepcleancorba
	cd javaui && $(MAKE) prepcleancorba
prepcleanjni :
	cd engine && $(MAKE) prepcleanjni
prepclean : prepcleancorba prepcleanjni
prep : prepjava prepengine

run : runjni
runconsole : binenginejni binjavaui binjavajni
	./regina --console --jni
runtext : binenginejni binjavaui binjavajni
	./regina --text --jni
runjni : binenginejni binjavaui binjavajni
	./regina --gui --jni
runcorba : binjavaui binjavacorba
	./regina --gui --corba
runcorbaengine : binenginecorba
	$(ENGINE_CORBA) \
		-ORBInitialHost $(ORBInitialHost) -ORBInitialPort $(ORBInitialPort)
debug : debugjni
debugjni : binenginejni binjavaui binjavajni
	export REGINA_JAVA="$(JDB)" && ./regina --gui --jni
debugcorba : binenginecorba binjavaui binjavacorba
	export REGINA_JAVA="$(JDB)" && ./regina --gui --corba

clean :
	cd engine && $(MAKE) clean
	cd javaui && $(MAKE) clean
	cd docs && rm -rf $(gen_doc_mask) manpage.*

purge : clean
	-rm $(ENGINE_JNI) $(ENGINE_CORBA) $(ENGINE_LIB)
	-rm $(JAVA_UI) $(JAVA_JNI) $(JAVA_CORBA)
	-rm $(DOC_JAR)

Makefile.options :
	@echo
	@echo "Makefile.options is missing."
	@echo "Try taking one of the sample files (Makefile.options.*) and"
	@echo "    modifying it to suit your system."
	@echo
	@false
