
# This makefile should only ever be called from ../Makefile.
# See ../Makefile for further details.

# ------------------------ Variables ---------------------------------

INCLUDES = -Iengine $(EXTRA_INCLUDES)

JNI_DIR = jni/engine
JNI_PACKAGE = normal.engine.implementation.jni

PWD = $(shell pwd)

DOC_DIR = ../$(BASE_DOC_DIR)/engine
DOC_SAMPLE = $(DOC_DIR)/index.html
DOC_CONF = doc-files/docs.conf

PD_DEFS = $(PD_MACROS:%=-D%)

# ------------------------ Rules -------------------------------------

# Begin with rules requiring extra include directories.

jni/%.o : jni/%.cpp
	$(COMPILE) $(PD_DEFS) -Ijni $(INCLUDES) $(JNI_INCLUDES) -o $@ $<

jni/%.d : jni/%.cpp
	$(SHELL) -ec '$(MAKEDEP) -Ijni $(INCLUDES) $(JNI_INCLUDES) $< \
		| $(SED) '\''s/\($(notdir $*)\)\.o[ :]*/$(subst /,\/,jni/$*).o \
		$(subst /,\/,$@) : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'

# Follow with generic rules.

%.o : %.cpp
	$(COMPILE) $(PD_DEFS) $(INCLUDES) -o $@ $<

%.d : %.cpp
	$(SHELL) -ec '$(MAKEDEP) $(INCLUDES) $< \
		| $(SED) '\''s/\($(notdir $*)\)\.o[ :]*/$(subst /,\/,$*).o \
		$(subst /,\/,$@) : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'

# Only include the JNI dependencies if prepjni appears on the
# Makefile command line.

INCLUDE_JNI_DEPENDS := yes
ifeq (,$(findstring prepjni,$(MAKECMDGOALS)))
INCLUDE_JNI_DEPENDS := no
endif

ifeq ($(INCLUDE_JNI_DEPENDS),yes)
$(JNI_DIR)/%.h : ../$(JAVA_JNI)
	$(JAVAH) -classpath \
		'../$(JAVA_UI)$(cs)../$(JAVA_JNI)$(cs)$(other_classes)' -o $@ \
		$(subst /,.,$(patsubst $(JNI_DIR)/%.h,$(JNI_PACKAGE).%,$@))
	touch $@
endif

# ----------------------- Sources ------------------------------------

include Makefile.sources

dirs_engine_all = $(dirs_engine_engine) $(dirs_engine_jni)

src_engine = $(wildcard $(dirs_engine_engine:=/*.cpp))
src_jni = $(wildcard $(dirs_engine_jni:=/*.cpp))
src_all = $(wildcard $(dirs_engine_all:=/*.cpp))

hdr_engine = $(wildcard $(dirs_engine_engine:=/*.h))
hdr_jni = $(wildcard $(dirs_engine_jni:=/*.h))
hdr_all = $(wildcard $(dirs_engine_all:=/*.h))

JNI_PACKAGE_DIR = ../javaui/$(subst .,/,$(JNI_PACKAGE))

jni_engine_dirs = $(filter $(JNI_DIR)%,$(dirs_engine_jni))
jni_java_mask = $(jni_engine_dirs:$(JNI_DIR)%=$(JNI_PACKAGE_DIR)%/*.java)
jni_java_files = $(wildcard $(jni_java_mask))
hdr_jni_autogen = $(jni_java_files:$(JNI_PACKAGE_DIR)/%.java=$(JNI_DIR)/%.h)

obj_engine = $(patsubst %.cpp,%.o,$(src_engine))
obj_jni = $(patsubst %.cpp,%.o,$(src_jni))
obj_all = $(patsubst %.cpp,%.o,$(src_all))

dep_engine = $(patsubst %.cpp,%.d,$(src_engine))
dep_jni = $(patsubst %.cpp,%.d,$(src_jni))
dep_all = $(patsubst %.cpp,%.d,$(src_all))

engine_misc_src_files = $(wildcard $(engine_misc_src_mask))

output_jni = ../$(ENGINE_JNI)
output_lib = ../$(ENGINE_LIB)

# ------------------------- Targets ----------------------------------

.PHONY : binjni binlib prepjni prepcleanjni docs clean

binjni : $(output_jni)
$(output_jni) : $(output_lib) $(obj_jni)
	$(LINKLIB) -o $(output_jni) $(obj_jni) $(output_lib) \
		$(EXTRA_LIBRARIES)

binlib : $(output_lib)
$(output_lib) : $(obj_engine)
	-rm $(output_lib)
	$(LINKSTATIC) $(output_lib) $(obj_engine)

prepjni : $(hdr_jni_autogen)

prepcleanjni :
	-rm $(hdr_jni_autogen)

docs : $(DOC_SAMPLE)
$(DOC_SAMPLE) : $(hdr_engine) $(DOC_CONF)
	$(DOXYGEN) $(DOC_CONF)

clean :
	-rm $(dirs_engine_all:=/*.o) $(dirs_engine_all:=/*.d)
	-rm exports.def

# ------------------------- Dependencies ----------------------------------

# Only include the appropriate dependencies if binxxx appears on the
# Makefile command line.

INCLUDE_DEPENDS_ENGINE := yes
INCLUDE_DEPENDS_JNI := yes

ifeq (,$(findstring bin,$(MAKECMDGOALS)))
INCLUDE_DEPENDS_ENGINE := no
endif
ifeq (,$(findstring binjni,$(MAKECMDGOALS)))
INCLUDE_DEPENDS_JNI := no
endif

ifeq ($(INCLUDE_DEPENDS_ENGINE),yes)
-include $(dep_engine)
endif
ifeq ($(INCLUDE_DEPENDS_JNI),yes)
-include $(dep_jni)
endif

