
index = regina.sgml

all: html man

html: index.html
index.html: $(index)
	jw $(index)

man: regina.1
regina.1: $(index)
	jw -b man $(index)

clean:
	-rm *.html *.1 *.5 *.7 manpage.*

.PHONY: all html man clean
