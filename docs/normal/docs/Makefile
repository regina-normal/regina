
contents: buildcontents
	./buildcontents > contents.html

buildcontents: buildcontents.cpp
	g++ -I/usr/include/qt -o buildcontents -lqt buildcontents.cpp

.PHONY: contents clean

clean:
	-rm buildcontents
