
index = regina.sgml
sgmlfiles = $(wildcard *.sgml)

all : html man

html : index.html
index.html : $(sgmlfiles)
	jade -t sgml -i html -d regina.dsl\#html $(index)

man : regina.1
regina.1 : $(sgmlfiles)
	docbook2man $(index)

rtf : regina.rtf
regina.rtf : $(sgmlfiles)
	jade -t rtf -d regina.dsl\#print $(index)

clean :
	-rm *.html *.1 *.5 *.7 manpage.* *.rtf

.PHONY : all html man clean
