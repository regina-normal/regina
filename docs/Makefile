
index = regina.sgml
sgmlfiles = $(wildcard *.sgml)

all : html man

html : index.html
index.html : $(sgmlfiles)
	jw $(index)

man : regina.1
regina.1 : $(sgmlfiles)
	jw -b man $(index)

clean :
	-rm *.html *.1 *.5 *.7 manpage.*

.PHONY : all html man clean
