
# This makefile should only ever be called from ../Makefile.
# See ../Makefile for further details.

# --------------------------- Variables -----------------------------------

JAVAUI_DIR = ../javaui
ENGINE_DIR = ../engine

# --------------------------- Sources -------------------------------------

sgml_sources = $(wildcard *.sgml)
sgml_index = regina.sgml
sgml_dsl = regina.dsl

sample_html = index.html
sample_man = regina.1
sample_rtf = regina.rtf

output_html = *.html
output_man = *.1 *.5 *.7 manpage.*
output_rtf = *.rtf

# --------------------------- Platform-specific ---------------------------

ifeq ($(OSTYPE),cygwin)
  man_target = noman
else
  man_target = $(sample_man)
endif

# --------------------------- Targets -------------------------------------

.PHONY : html man noman rtf engine javaui clean

html : $(sample_html)
$(sample_html) : $(sgml_sources)
	$(JADE) -t sgml -i html -d $(sgml_dsl)\#html $(sgml_index)

man : $(man_target)
$(sample_man) : $(sgml_sources)
	$(DOCBOOK2MAN) $(sgml_index)

noman :
	$(ECHO) "\n\nMan page generation is not supported on this platform.\n\n"

rtf : $(sample_rtf)
$(sample_rtf) : $(sgml_sources)
	$(JADE) -t rtf -d $(sgml_dsl)\#print $(sgml_index)

engine :
	cd $(ENGINE_DIR) && $(MAKE) docs

javaui :
	cd $(JAVAUI_DIR) && $(MAKE) docs

clean :
	-rm $(output_html) $(output_man) $(output_rtf)
