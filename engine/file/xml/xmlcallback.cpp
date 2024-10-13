
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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
#include "file/xml/xmlcallback.h"

namespace regina {

XMLCallback::~XMLCallback() {
    if (! readers_.empty())
        abort();
}

void XMLCallback::start_document(regina::xml::XMLParser* parser) {
    topReader_.usingParser(parser);
}

void XMLCallback::end_document() {
    if (state_ == State::Waiting) {
        errStream_ << "XML Fatal Error: File contains no tags." << std::endl;
        abort();
    } else if (state_ == State::Working || ! readers_.empty()) {
        errStream_ << "XML Fatal Error: Unfinished file." << std::endl;
        abort();
    }
}

void XMLCallback::start_element(const std::string& n,
        const regina::xml::XMLPropertyDict& p) {
    if (state_ == State::Done) {
        errStream_ << "XML Fatal Error: File contains multiple top-level tags."
            << std::endl;
        abort();
    } else if (state_ == State::Waiting) {
        currentReader()->startElement(n, p, nullptr);
        currChars_ = "";
        charsAreInitial_ = true;
        state_ = State::Working;
    } else if (state_ == State::Working) {
        XMLElementReader* current = currentReader();
        if (charsAreInitial_)
            current->initialChars(currChars_);

        XMLElementReader* child = current->startSubElement(n, p);
        readers_.push(child);
        child->startElement(n, p, current);
        currChars_ = "";
        charsAreInitial_ = true;
    }
}

void XMLCallback::end_element(const std::string& n) {
    if (state_ == State::Working) {
        XMLElementReader* current = currentReader();

        if (charsAreInitial_) {
            charsAreInitial_ = false;
            current->initialChars(currChars_);
        }
        current->endElement();

        if (readers_.empty()) {
            // In this case, current is the top-level reader.
            state_ = State::Done;
        } else {
            // In this case, current is at the top of the stack.
            readers_.pop();
            currentReader()->endSubElement(n, current);
            delete current;
        }
    }
}

void XMLCallback::characters(const std::string& s) {
    if (state_ == State::Working)
        if (charsAreInitial_)
            currChars_ += s;
}

void XMLCallback::warning(const std::string& s) {
    errStream_ << "XML Warning: " << s << std::endl;
}

void XMLCallback::error(const std::string& s) {
    errStream_ << "XML Error: " << s << std::endl;
    abort();
}

void XMLCallback::fatal_error(const std::string& s) {
    errStream_ << "XML Fatal Error: " << s << std::endl;
    abort();
}

void XMLCallback::abort() {
    if (state_ == State::Aborted)
        return;
    state_ = State::Aborted;

    // Make sure we don't delete a child reader until we've called
    // abortElement() on its parent.
    XMLElementReader* child = nullptr;
    while (! readers_.empty()) {
        readers_.top()->abort(child);
        if (child)
            delete child;

        child = readers_.top();
        readers_.pop();
    }

    topReader_.abort(child);
    if (child)
        delete child;
}

} // namespace regina

