
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *                                                                        *
 *  Documentation tools                                                   *
 *  Written by Ben Burton                                                 *
 *  Copyright (c) 1999-2001                                               *
 *                                                                        *
 *  See http://www.computop.org/ for more details or                      *
 *  contact Ben Burton (benb@acm.org).                                    *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 **************************************************************************/

#include <iostream.h>
#include <qdom.h>
#include <qfile.h>
#include <qdict.h>

/**
 * Parses the JavaHelp contents and map files for the Regina
 * documentation and writes a corresponding HTML table of contents to
 * standard output.
 */
int main() {
    QDomDocument toc("contents");
    QDomDocument jhm("help map");
    QFile tocFile("reginaTOC.xml");
    QFile jhmFile("regina.jhm");

    if (! tocFile.open(IO_ReadOnly)) {
        cerr << "Could not open reginaTOC.xml.\n";
        return 1;
    }
    if (! jhmFile.open(IO_ReadOnly)) {
        cerr << "Could not open regina.jhm.\n";
        return 1;
    }

    if (! toc.setContent(&tocFile)) {
        cerr << "Could not parse reginaTOC.xml.\n";
        return 1;
    }
    if (! jhm.setContent(&jhmFile)) {
        cerr << "Could not parse regina.jhm.\n";
        return 1;
    }

    // Read in the help map.
    QDict<QString> helpMap;
    helpMap.setAutoDelete(true);

    QDomElement jhmElem = jhm.documentElement();
    QDomNode n = jhmElem.firstChild();
    while (! n.isNull()) {
        QDomElement e = n.toElement();
        if ((! e.isNull()) && e.tagName() == "mapID")
            helpMap.replace(e.attribute("target"),
                new QString(e.attribute("url")));
        n = n.nextSibling();
    }

    // Write out the table of contents.
    cout << "<html>\n";
    cout << "<head><title>Regina - Table of Contents</title></head>\n";
    cout << "<body>\n";
    cout << "<center><b>Regina<br>Table of Contents</b></center><p>\n";
    cout << "<ul>\n";

    QDomElement tocElem = toc.documentElement();
    n = tocElem.firstChild();

    // Run through the tree.
    bool finishedNode = false;
    QString target;
    QString* page;
    while (n != tocElem) {
        QDomElement e = n.toElement();
        if ((! finishedNode) && (! e.isNull()) && e.tagName() == "tocitem") {
            // Look into this contents item.
            target = e.attribute("target");
            if (target.isNull() || ! (page = helpMap[target]))
                cout << "<li>" << e.attribute("text");
            else
                cout << "<li><a href=\"" << *page <<
                    "\" target=\"reginadocs\">" << e.attribute("text")
                    << "</a>";
        }

        // Move to the next node.
        if ((! finishedNode) && (! n.firstChild().isNull())) {
            n = n.firstChild();
            cout << "\n<ul>\n";
            finishedNode = false;
        } else if (! n.nextSibling().isNull()) {
            n = n.nextSibling();
			cout << "</li>\n";
            finishedNode = false;
        } else {
            n = n.parentNode();
            finishedNode = true;
            cout << "</li>\n</ul></li>\n";
        }
    }

    // Finish off.
    cout << "</body>\n</html>\n";
    return 0;
}

