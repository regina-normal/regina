
# This makefile should only ever be called from ../Makefile.
# See ../Makefile for further details.

# ------------------------ Variables ---------------------------------

INCLUDES = -I. $(EXTRA_INCLUDES)

JNI_DIR = jni/engine
JNI_PACKAGE = normal.engine.implementation.jni

IDL_DIR = ../idl
CORBA_DIR = corba/Regina

PWD = $(shell pwd)

DOC_DIR = ../$(BASE_DOC_DIR)/engine
DOC_SAMPLE = $(DOC_DIR)/index.html
DOC_CONF = doc-files/docs.conf

# ------------------------ Rules -------------------------------------

# Begin with rules requiring extra include directories.

jni/%.o : jni/%.cpp
	$(COMPILE) $(INCLUDES) $(JNI_INCLUDES) -o $@ $<

jni/%.d : jni/%.cpp
	$(SHELL) -ec '$(MAKEDEP) $(INCLUDES) $(JNI_INCLUDES) $< \
		| $(SED) '\''s/\($(notdir $*)\)\.o[ :]*/$(subst /,\/,jni/$*).o \
		$(subst /,\/,$@) : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'

corba/%.o : corba/%.cpp
	$(COMPILE) $(INCLUDES) $(CORBA_INCLUDES) \
		$(patsubst idl%,-I$(CORBA_DIR)%,$(dirs_idl)) -o $@ $<

corba/%.d : corba/%.cpp
	$(SHELL) -ec '$(MAKEDEP) $(INCLUDES) $(CORBA_INCLUDES) \
		$(patsubst idl%,-I$(CORBA_DIR)%,$(dirs_idl)) $< \
		| $(SED) '\''s/\($(notdir $*)\)\.o[ :]*/$(subst /,\/,corba/$*).o \
		$(subst /,\/,$@) : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'

# Follow with generic rules.

%.o : %.cpp
	$(COMPILE) $(INCLUDES) -o $@ $<

%.d : %.cpp
	$(SHELL) -ec '$(MAKEDEP) $(INCLUDES) $< \
		| $(SED) '\''s/\($(notdir $*)\)\.o[ :]*/$(subst /,\/,$*).o \
		$(subst /,\/,$@) : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'

$(CORBA_DIR)/%SK.cpp : $(IDL_DIR)/%.idl
	cd $(dir $@) && $(IDLTOCPPSERVER) -I$(PWD)/$(IDL_DIR) $(PWD)/$<

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

dirs_engine_all = $(dirs_engine_engine) $(dirs_engine_jni) $(dirs_engine_corba)

src_engine = $(wildcard $(dirs_engine_engine:=/*.cpp))
src_jni = $(wildcard $(dirs_engine_jni:=/*.cpp))
src_corba = $(wildcard $(dirs_engine_corba:=/*.cpp))
src_all = $(wildcard $(dirs_engine_all:=/*.cpp))

src_corba_idl = $(patsubst idl/%.idl,$(CORBA_DIR)/%SK.cpp,$(src_idl))

hdr_engine = $(wildcard $(dirs_engine_engine:=/*.h))
hdr_jni = $(wildcard $(dirs_engine_jni:=/*.h))
hdr_corba = $(wildcard $(dirs_engine_corba:=/*.h))
hdr_all = $(wildcard $(dirs_engine_all:=/*.h))

JNI_PACKAGE_DIR = ../javaui/$(subst .,/,$(JNI_PACKAGE))

jni_engine_dirs = $(filter $(JNI_DIR)%,$(dirs_engine_jni))
jni_java_mask = $(jni_engine_dirs:$(JNI_DIR)%=$(JNI_PACKAGE_DIR)%/*.java)
jni_java_files = $(wildcard $(jni_java_mask))
hdr_jni_autogen = $(jni_java_files:$(JNI_PACKAGE_DIR)/%.java=$(JNI_DIR)/%.h)

obj_engine = $(patsubst %.cpp,%.o,$(src_engine))
obj_jni = $(patsubst %.cpp,%.o,$(src_jni))
obj_corba = $(patsubst %.cpp,%.o,$(src_corba))
obj_all = $(patsubst %.cpp,%.o,$(src_all))

dep_engine = $(patsubst %.cpp,%.d,$(src_engine))
dep_jni = $(patsubst %.cpp,%.d,$(src_jni))
dep_corba = $(patsubst %.cpp,%.d,$(src_corba))
dep_all = $(patsubst %.cpp,%.d,$(src_all))

engine_misc_src_files = $(wildcard $(engine_misc_src_mask))

output_jni = ../$(ENGINE_JNI)
output_corba = ../$(ENGINE_CORBA)

# ------------------------- Targets ----------------------------------

.PHONY : binjni bincorba \
	prepjni prepcorba \
	prepcleanjni prepcleancorba \
	docs clean

binjni : $(output_jni)
$(output_jni) : $(obj_engine) $(obj_jni)
	$(LINKLIB) -o $(output_jni) $(obj_engine) $(obj_jni) \
		$(EXTRA_LIBRARIES)

bincorba : $(output_corba)
$(output_corba) : $(obj_engine) $(obj_corba)
	$(LINKEXEC) -o $(output_corba) $(obj_engine) $(obj_corba) \
		$(CORBA_LIBRARIES) $(EXTRA_LIBRARIES)

prepjni : $(hdr_jni_autogen)

prepcorba : $(src_corba_idl)

prepcleanjni :
	-rm $(hdr_jni_autogen)

prepcleancorba :
	-rm $(src_idl:idl/%.idl=corba/Regina/%IDL.h)
	-rm $(src_idl:idl/%.idl=corba/Regina/%SK.*)

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
INCLUDE_DEPENDS_CORBA := yes

ifeq (,$(findstring bin,$(MAKECMDGOALS)))
INCLUDE_DEPENDS_ENGINE := no
endif
ifeq (,$(findstring binjni,$(MAKECMDGOALS)))
INCLUDE_DEPENDS_JNI := no
endif
ifeq (,$(findstring bincorba,$(MAKECMDGOALS)))
INCLUDE_DEPENDS_CORBA := no
endif

ifeq ($(INCLUDE_DEPENDS_ENGINE),yes)
-include $(dep_engine)
endif
ifeq ($(INCLUDE_DEPENDS_JNI),yes)
-include $(dep_jni)
endif
ifeq ($(INCLUDE_DEPENDS_CORBA),yes)
-include $(dep_corba)
endif

