##
##  Regina - A Normal Surface Theory Calculator
##  Makefile Template
##
##  Process this file with automake in the top-level directory
##  to produce Makefile.in.
##

## ----------------------- Variables ---------------------------------

USE_CLASSPATH = .:@JAVAUI_CLASSPATH@

javaui_index = index.html
javaui_overview = overview.html
javaui_title = 'Regina - Java User Interface'
javaui_bottom =  'Copyright &copy; 1999-2002, Ben Burton<br>This software is released under the GNU Public License.<br>For further information, or to submit a bug or other problem, please contact Ben Burton (<a href="mailto:benb@acm.org">benb@acm.org</a>).'

## ----------------------- Custom targets ----------------------------

all-local : $(javaui_index)
$(javaui_index) :
	cd ../../javaui && CLASSPATH="$(USE_CLASSPATH)" @JAVADOC@ -d ../docs/javaui \
		-overview $(javaui_overview) -windowtitle $(javaui_title) \
		-doctitle $(javaui_title) -bottom $(javaui_bottom) -package -use \
		`find normal -name package.html | sed -e 's#/package.html$$##;s#/#.#g'`

mostlyclean-local :
	rm -f *.html *.css package-list
	rm -rf normal

