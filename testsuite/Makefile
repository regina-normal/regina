
# This makefile should only ever be called from ../Makefile.
# See ../Makefile for further details.

# ------------------------ Variables -------------------------------------

INCLUDES = -I../engine/engine $(CPPUNIT_INCLUDES) $(EXTRA_INCLUDES)
PD_DEFS = $(PD_MACROS:%=-D%)
BIN_DIR = ../bin

# ------------------------ Rules -------------------------------------

%.o : %.cpp
	$(COMPILE) $(PD_DEFS) $(INCLUDES) -o $@ $<

# ------------------------ Sources -------------------------------------

include Makefile.sources

src_testsuite = $(wildcard $(dirs_testsuite:=/*.cpp))
obj_testsuite = $(patsubst %.cpp,%.o,$(src_testsuite))

output_testsuite = $(BIN_DIR)/regtestsuite

# ------------------------ Targets -------------------------------------

.PHONY : all clean purge

all : $(output_testsuite)

$(output_testsuite) : $(obj_testsuite)
	$(LINKEXEC) -o $(output_testsuite) $(obj_testsuite) ../$(ENGINE_LIB) \
		$(CPPUNIT_LIBRARIES) $(EXTRA_LIBRARIES)

clean :
	-rm $(dirs_testsuite:=/*.o)
purge : clean
	-rm $(output_testsuite)

