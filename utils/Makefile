
# This makefile should only ever be called from ../Makefile.
#
# To make target "foo" in this directory, move to the parent directory
# and type "make utils/foo".
#
# See ../Makefile for further details.

# ------------------------ Variables -------------------------------------

INCLUDES = -I../engine/engine $(EXTRA_INCLUDES)
PD_DEFS = $(PD_MACROS:%=-D%)

ifeq ($(OSTYPE),cygwin)
  EXE = .exe
else
  EXE =
endif

# ------------------------ Rules -------------------------------------

%.o : %.cpp
	$(COMPILE) $(PD_DEFS) $(INCLUDES) -o $@ $<
%$(EXE) : %.o
	$(LINKEXEC) -o $@ $< ../$(ENGINE_LIB) $(EXTRA_LIBRARIES)

# ------------------------ Targets -------------------------------------

.PHONY : all clean

# All utilities:

all : regfiledump$(EXE)

# Individual utilities:

regfiledump$(EXE) : regfiledump.o

# Other targets:

clean :
	-rm *.o *.d
	-rm regfiledump$(EXE)

