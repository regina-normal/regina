
# Type "make" to see a list of available targets and their descriptions.

# All system-specific variables are in Makefile.options and should be
# edited there.  Nothing in this file should need editing.

# ------------------------- Variables --------------------------------

include Makefile.options

# Global variables:

include Makefile.sources
include idl/Makefile.sources

export src_idl = $(wildcard $(dirs_idl:=/*.idl))

export BIN_DIR = bin
export BASE_DOC_DIR = docs
export CONF_DIR = configure

export cs = $(CLASSPATH_SEP)
export other_classes = \
	$(BTOOLS)$(cs)$(JPYTHON)$(cs)$(JAVAHELP)$(cs)$(EXTRA_CLASSES)

export LICENSE = LICENSE.txt

# Local variables:

empty_dirs = $(DIST_DIR) $(BIN_DIR) engine javaui idl
DOC_LICENSE = $(BASE_DOC_DIR)/normal/docs/LICENSE.txt
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
		$(GREP) "^conf" Makefile-help; \
		$(ECHO)

.PHONY : help helpgeneral helpbin helpdocs helpprep helprun \
	binengine binenginejni binenginecorba \
	binjava binjavaui binjavajni binjavacorba \
	binconf bindocs bin \
	docsengine docsjava docs \
	prepengine prepenginejni prepenginecorba prepjava prepjavacorba \
		prepcorba prepjni prepcleancorba prepcleanjni prepclean prep \
	run runconsole runtext runjni runcorba runcorbaengine \
	debug debugjni debugcorba configure \
	clean

binengine : binenginejni binenginecorba
binenginejni : prepenginejni
	cd engine && $(MAKE) binjni
binenginecorba : prepenginecorba
	cd engine && $(MAKE) bincorba
binjava : binjavaui binjavajni binjavacorba
binjavaui :
	cd javaui && $(MAKE) uijar
binjavajni :
	cd javaui && $(MAKE) jnijar
binjavacorba :
	cd javaui && $(MAKE) corbajar
binconf :
	cd $(CONF_DIR) && $(MAKE) jar
bindocs : $(DOC_JAR)
$(DOC_JAR) : docs
	cd docs && $(JAR) -cf ../$(DOC_JAR) *
bin : binengine binjava binconf

docsengine :
	cd engine && $(MAKE) docs
docsjava :
	cd javaui && $(MAKE) docs
docs : docsengine docsjava $(DOC_LICENSE)
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
	$(JAVA) -Djava.library.path=$(BIN_DIR) -cp \
		'$(JAVA_UI)$(cs)$(JAVA_JNI)$(cs)$(DOC_JAR)$(cs)$(other_classes)' \
		$(APP_CLASS) --console
runtext : binenginejni binjavaui binjavajni
	$(JAVA) -Djava.library.path=$(BIN_DIR) -cp \
		'$(JAVA_UI)$(cs)$(JAVA_JNI)$(cs)$(DOC_JAR)$(cs)$(other_classes)' \
		$(APP_CLASS) --text
runjni : binenginejni binjavaui binjavajni
	$(JAVA) -Djava.library.path=$(BIN_DIR) -cp \
		'$(JAVA_UI)$(cs)$(JAVA_JNI)$(cs)$(DOC_JAR)$(cs)$(other_classes)' \
		$(APP_CLASS)
runcorba : binjavaui binjavacorba
	$(JAVA) -cp \
		'$(JAVA_UI)$(cs)$(JAVA_CORBA)$(cs)$(DOC_JAR)$(cs)$(other_classes)' \
		$(APP_CLASS) --corba \
		-ORBInitialHost $(ORBInitialHost) -ORBInitialPort $(ORBInitialPort)
runcorbaengine : binenginecorba
	$(ENGINE_CORBA) \
		-ORBInitialHost $(ORBInitialHost) -ORBInitialPort $(ORBInitialPort)
debug : debugjni
debugjni : binenginejni binjavaui binjavajni
	$(JDB) -Djava.library.path=$(BIN_DIR) -classpath \
		'$(JAVA_UI)$(cs)$(JAVA_JNI)$(cs)$(DOC_JAR)$(cs)$(other_classes)' \
		$(APP_CLASS)
debugcorba : binenginecorba binjavaui binjavacorba
	$(JDB) -classpath \
		'$(JAVA_UI)$(cs)$(JAVA_CORBA)$(cs)$(DOC_JAR)$(cs)$(other_classes)' \
		$(APP_CLASS)
configure : binconf
	$(JAVA) -jar $(REG_CONF)

clean :
	cd engine && $(MAKE) clean
	cd javaui && $(MAKE) clean
	cd $(CONF_DIR) && $(MAKE) clean

