
# This makefile should only ever be called from ../Makefile.
# See ../Makefile for further details.

# ------------------------ Variables -------------------------------------

INCLUDES = -I../engine/engine $(EXTRA_INCLUDES)
PD_DEFS = $(PD_MACROS:%=-D%)
BIN_DIR = ../bin

# ------------------------ Rules -------------------------------------

%.o : %.cpp
	$(COMPILE) $(PD_DEFS) $(INCLUDES) -o $@ $<
$(BIN_DIR)/%$(EXE) : %.o ../$(ENGINE_LIB)
	$(LINKEXEC) -o $@ $< ../$(ENGINE_LIB) $(EXTRA_LIBRARIES)

# ------------------------ Sources -------------------------------------

include Makefile.sources

allobjs = $(utils:=.o)
allexes = $(utils:%=$(BIN_DIR)/%$(EXE))

# We shouldn't have *all* executables depend on *all* sources, but this
# avoids the alternative nasty of having to specify each exe-from-obj
# dependency by hand. 

$(allexes) : $(allobjs)

# ------------------------ Targets -------------------------------------

.PHONY : all clean purge

# All utilities:

all : $(allexes)

# Other targets:

clean :
	-rm *.o *.d
purge :
	-rm $(allexes)

