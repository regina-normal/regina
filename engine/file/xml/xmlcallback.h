
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

/*! \file utilities/xmlcallback.h
 *  \brief Deals with parsing XML program data at the file level.
 */

#ifndef __REGINA_XMLCALLBACK_H
#ifndef __DOXYGEN
#define __REGINA_XMLCALLBACK_H
#endif

#include <iostream>
#include <stack>
#include "regina-core.h"
#include "file/xml/xmlelementreader.h"

namespace regina {

/**
 * Provides the callbacks for an XMLParser required to parse an entire
 * file using a series of XMLElementReader objects.
 * See the XMLElementReader class notes for details of precisely how
 * processing will take place.
 *
 * \ifacespython Not present.
 */
class XMLCallback : public regina::xml::XMLParserCallback {
    public:
        /**
         * Used to indicate the state of a callback object.
         */
        enum State {
            WAITING = 1,
                /**< The top-level XML element has not yet been seen. */
            WORKING = 2,
                /**< XML elements are currently being processed. */
            DONE = 3,
                /**< Processing of all XML elements has finished. */
            ABORTED = 4
                /**< XML processing was aborted. */
        };

    private:
        XMLElementReader& topReader;
            /**< The top-level element reader. */
        std::stack<XMLElementReader*> readers;
            /**< A stack of all currently active element readers. */
        std::ostream& errStream;
            /**< The output stream to use for warning or error messages. */
        std::string currChars;
            /**< The initial characters that have currently been received
                 for the current deepest-level XML element. */
        bool charsAreInitial;
            /**< \c true if and only if we have not yet finished
                 receiving initial characters for the current deepest-level
                 XML element. */
        State state_;
            /**< The current state of this callback object; this will be
                 one of the state constants defined in this class. */

    public:
        /**
         * Creates a new callback object.
         *
         * @param newTopReader the element reader to use for the
         * top-level XML element.  This is the only element reader that
         * will not be destroyed once parsing has finished.
         * @param newErrStream the output stream to which any warning or
         * error messages should be sent.
         */
        XMLCallback(XMLElementReader& newTopReader,
            std::ostream& newErrStream);
        /**
         * Destroys this callback object.  Any element reader (aside from
         * the top-level reader) that has not yet been destroyed will
         * have abort() called upon it and will be destroyed at this point.
         */
        ~XMLCallback() override;

        /**
         * Returns the state that this callback object is currently in.
         * The returned value will be one of the state constants defined
         * in this class.
         *
         * @return the current state of this callback object.
         */
        State state() const;

        /**
         * Aborts processing of the XML file completely.  The XMLParser
         * may continue sending information but it will be completely
         * ignored by this XMLCallback object from this point onwards.
         *
         * All currently active readers will have
         * XMLElementReader::abort() called upon them and all except for
         * the top-level reader will be destroyed.
         */
        void abort();

        void start_document(regina::xml::XMLParser* parser) override;
        void end_document() override;
        void start_element(const std::string& n,
            const regina::xml::XMLPropertyDict& p) override;
        void end_element(const std::string& n) override;
        void characters(const std::string& s) override;
        void warning(const std::string& s) override;
        void error(const std::string& s) override;
        void fatal_error(const std::string& s) override;

        // Make this class non-copyable.
        XMLCallback(const XMLCallback&) = delete;
        XMLCallback& operator = (const XMLCallback&) = delete;

    private:
        /**
         * Returns the element reader processing the deepest-level
         * XML element that is currently being parsed.
         *
         * @return the current deepest element reader.
         */
        XMLElementReader* currentReader();
};

// Inline functions for XMLCallback

inline XMLCallback::XMLCallback(XMLElementReader& newTopReader,
        std::ostream& newErrStream) : topReader(newTopReader),
        errStream(newErrStream), charsAreInitial(true), state_(WAITING) {
}

inline XMLElementReader* XMLCallback::currentReader() {
    return (readers.empty() ? &topReader : readers.top());
}

inline XMLCallback::State XMLCallback::state() const {
    return state_;
}

} // namespace regina

#endif

