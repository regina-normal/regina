
# This makefile should only ever be called from ../Makefile.
# See ../Makefile for further details.

# ------------------------ Variables ---------------------------------

IDL_DIR = ../idl
CORBA_PACKAGE = normal.engine.implementation.corba
CORBA_DIR = $(subst .,/,$(CORBA_PACKAGE))
CORBA_MODULE = Regina

java_files_corba_server = \
	$(CORBA_DIR)/$(CORBA_MODULE)/File/_NRandomAccessResourceImplBase.java

DOC_DIR = ../$(BASE_DOC_DIR)/javaui
DOC_SAMPLE = $(DOC_DIR)/index.html
DOC_OVERVIEW = overview.html

DOC_TITLE = 'Regina - Java User Interface'
DOC_BOTTOM = 'Copyright &copy; 1999-2001, Ben Burton<br>This software is released under the GNU Public License.<br>For further information, or to submit a bug or other problem, please contact Ben Burton (<a href="mailto:benb@acm.org">benb@acm.org</a>).'

# --------------------------- Rules ---------------------------------------

%.class : %.java
	$(JAVAC) -classpath '.$(cs)$(other_classes)' $<

$(CORBA_DIR)/$(CORBA_MODULE)/%.java : $(IDL_DIR)/%.idl
	$(IDLTOJAVA) -pkgPrefix $(CORBA_MODULE) $(CORBA_PACKAGE) \
		-i $(IDL_DIR) -td . $<

$(CORBA_DIR)/$(CORBA_MODULE)/File/_%ImplBase.java : $(IDL_DIR)/File/%.idl
	$(IDLTOJAVA) -pkgPrefix $(CORBA_MODULE) $(CORBA_PACKAGE) \
		-i $(IDL_DIR) -td . -fserver $<

# --------------------------- Sources -------------------------------------

include Makefile.sources

raw_package_dirs_ui = $(subst .,/,$(packages_ui))
raw_package_dirs_jni = $(subst .,/,$(packages_jni))
raw_package_dirs_corba = $(subst .,/,$(packages_corba))
raw_package_dirs = $(raw_package_dirs_ui) \
	$(raw_package_dirs_jni) $(raw_package_dirs_corba)

java_mask_ui = $(patsubst %,%/*.java,$(raw_package_dirs_ui))
java_mask_jni = $(patsubst %,%/*.java,$(raw_package_dirs_jni))
java_mask_corba = $(patsubst %,%/*.java,$(raw_package_dirs_corba))
java_mask = $(patsubst %,%/*.java,$(raw_package_dirs))

class_mask_ui = $(patsubst %,%/*.class,$(raw_package_dirs_ui))
class_mask_jni = $(patsubst %,%/*.class,$(raw_package_dirs_jni))
class_mask_corba = $(patsubst %,%/*.class,$(raw_package_dirs_corba))
class_mask = $(patsubst %,%/*.class,$(raw_package_dirs))

java_files_ui = $(wildcard $(java_mask_ui))
java_files_jni = $(wildcard $(java_mask_jni))
java_files_corba = $(wildcard $(java_mask_corba))
java_files = $(wildcard $(java_mask))

java_files_corba_idl = \
	$(patsubst idl/%.idl,$(CORBA_DIR)/$(CORBA_MODULE)/%.java,$(src_idl))

made_class_files_ui = $(java_files_ui:.java=.class)
made_class_files_jni = $(java_files_jni:.java=.class)
made_class_files_corba = $(java_files_corba:.java=.class)
made_class_files = $(java_files:.java=.class)

package_overviews_ui = $(patsubst %,%/package.html,$(raw_package_dirs_ui))

java_aux_files = $(wildcard $(java_aux_mask))

doc_sources = $(package_overviews_ui) $(DOC_OVERVIEW)

jar_ui = ../$(JAVA_UI)
jar_jni = ../$(JAVA_JNI)
jar_corba = ../$(JAVA_CORBA)

# --------------------------- Targets -------------------------------------

.PHONY : uibin jnibin corbabin \
	uijar jnijar corbajar \
	prepcorba prepcleancorba \
	docs clean

uibin : prepcorba $(made_class_files_ui)
jnibin : $(made_class_files_jni)
corbabin : prepcorba $(made_class_files_corba)

uijar : $(jar_ui)
$(jar_ui) : $(java_files_corba_idl) $(made_class_files_ui) $(java_aux_files) \
		$(LICENSE)
	$(JAR) -cf $(jar_ui) $(class_mask_ui) $(java_aux_mask) $(LICENSE)

jnijar : $(jar_jni)
$(jar_jni) : $(made_class_files_jni) $(LICENSE)
	$(JAR) -cf $(jar_jni) $(class_mask_jni) $(LICENSE)

corbajar : $(jar_corba)
$(jar_corba) : $(java_files_corba_idl) $(made_class_files_corba) $(LICENSE)
	$(JAR) -cf $(jar_corba) $(class_mask_corba) $(LICENSE)

prepcorba : $(java_files_corba_idl) $(java_files_corba_server)

prepcleancorba :
	-rm -rf $(CORBA_DIR)/$(CORBA_MODULE)

docs : $(DOC_SAMPLE)
$(DOC_SAMPLE) : $(java_files_ui) $(doc_sources)
	$(JAVADOC) -classpath '.$(cs)$(other_classes)' -d $(DOC_DIR) \
		-overview $(DOC_OVERVIEW) -windowtitle $(DOC_TITLE) \
		-doctitle $(DOC_TITLE) -bottom $(DOC_BOTTOM) \
		-package -use $(packages_ui)

clean :
	-rm $(class_mask)

