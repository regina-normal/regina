
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

#include <iomanip>
#include "utilities/nxmlcallback.h"

namespace regina {

const int NXMLCallback::WAITING = 1;
const int NXMLCallback::WORKING = 2;
const int NXMLCallback::DONE = 3;
const int NXMLCallback::ABORTED = 4;
    
NXMLCallback::~NXMLCallback() {
    if (! readers.empty())
        abort();
}
    
void NXMLCallback::start_document(regina::xml::XMLParser* parser) {
    topReader.usingParser(parser);
}

void NXMLCallback::end_document() {
    if (state_ == WAITING) {
        errStream << "XML Fatal Error: File contains no tags." << std::endl;
        abort();
    } else if (state_ == WORKING || ! readers.empty()) {
        errStream << "XML Fatal Error: Unfinished file." << std::endl;
        abort();
    }
}

void NXMLCallback::start_element(const std::string& n,
        const regina::xml::XMLPropertyDict& p) {
    if (state_ == DONE) {
        errStream << "XML Fatal Error: File contains multiple top-level tags."
            << std::endl;
        abort();
    } else if (state_ == WAITING) {
        currentReader()->startElement(n, p, 0);
        currChars = "";
        charsAreInitial = true;
        state_ = WORKING;
    } else if (state_ == WORKING) {
        NXMLElementReader* current = currentReader();
        if (charsAreInitial)
            current->initialChars(currChars);

        NXMLElementReader* child = current->startSubElement(n, p);
        readers.push(child);
        child->startElement(n, p, current);
        currChars = "";
        charsAreInitial = true;
    }
}

void NXMLCallback::end_element(const std::string& n) {
    if (state_ == WORKING) {
        NXMLElementReader* current = currentReader();

        if (charsAreInitial) {
            charsAreInitial = false;
            current->initialChars(currChars);
        }
        current->endElement();

        if (readers.empty()) {
            // In this case, current is the top-level reader.
            state_ = DONE;
        } else {
            // In this case, current is at the top of the stack.
            readers.pop();
            currentReader()->endSubElement(n, current);
            delete current;
        }
    }
}

void NXMLCallback::characters(const std::string& s) {
    if (state_ == WORKING)
        if (charsAreInitial)
            currChars += s;
}

void NXMLCallback::warning(const std::string& s) {
    errStream << "XML Warning: " << s << std::endl;
}

void NXMLCallback::error(const std::string& s) {
    errStream << "XML Error: " << s << std::endl;
    abort();
}

void NXMLCallback::fatal_error(const std::string& s) {
    errStream << "XML Fatal Error: " << s << std::endl;
    abort();
}

void NXMLCallback::abort() {
    if (state_ == ABORTED)
        return;
    state_ = ABORTED;

    // Make sure we don't delete a child reader until we've called
    // abortElement() on its parent.
    NXMLElementReader* child = 0;
    while (! readers.empty()) {
        readers.top()->abort(child);
        if (child)
            delete child;

        child = readers.top();
        readers.pop();
    }

    topReader.abort(child);
    if (child)
        delete child;
}

} // namespace regina

