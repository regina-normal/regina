
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2004, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file nxmlcallback.h
 *  \brief Deals with parsing XML program data at the file level.
 */

#ifndef __NXMLCALLBACK_H
#ifndef __DOXYGEN
#define __NXMLCALLBACK_H
#endif

#include <iostream>
#include <stack>
#include "file/nxmlelementreader.h"

namespace regina {

/**
 * \weakgroup file
 * @{
 */

/**
 * Provides the callbacks for an XMLParser required to parse an entire
 * file using a series of NXMLElementReader objects.
 * See the NXMLElementReader class notes for details of precisely how
 * processing will take place.
 *
 * \ifacespython Not present.
 */
class NXMLCallback : public regina::xml::XMLParserCallback {
    public:
        static const int WAITING;
            /**< Signifies that the top-level XML element has not yet been
                 seen. */
        static const int WORKING;
            /**< Signifies that XML elements are currently being processed. */
        static const int DONE;
            /**< Signifies that processing of all XML elements has finished. */
        static const int ABORTED;
            /**< Signifies that XML processing was aborted. */

    private:
        NXMLElementReader& topReader;
            /**< The top-level element reader. */
        std::stack<NXMLElementReader*> readers;
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
        int state;
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
        NXMLCallback(NXMLElementReader& newTopReader,
            std::ostream& newErrStream);
        /**
         * Destroys this callback object.  Any element reader (aside from
         * the top-level reader) that has not yet been destroyed will
         * have abort() called upon it and will be destroyed at this point.
         */
        virtual ~NXMLCallback();

        /**
         * Returns the state that this callback object is currently in.
         * The returned value will be one of the state constants defined
         * in this class.
         *
         * @return the current state of this callback object.
         */
        int getState() const;

        /**
         * Aborts processing of the XML file completely.  The XMLParser
         * may continue sending information but it will be completely
         * ignored by this NXMLCallback object from this point onwards.
         *
         * All currently active readers will have
         * NXMLElementReader::abort() called upon them and all except for
         * the top-level reader will be destroyed.
         */
        void abort();

        virtual void end_document();
        virtual void start_element(const std::string& n,
            const regina::xml::XMLPropertyDict& p);
        virtual void end_element(const std::string& n);
        virtual void characters(const std::string& s);
        virtual void warning(const std::string& s);
        virtual void error(const std::string& s);
        virtual void fatal_error(const std::string& s);

    private:
        /**
         * Returns the element reader processing the deepest-level
         * XML element that is currently being parsed.
         *
         * @return the current deepest element reader.
         */
        NXMLElementReader* currentReader();
};

/*@}*/

// Inline functions for NXMLCallback

inline NXMLCallback::NXMLCallback(NXMLElementReader& newTopReader,
        std::ostream& newErrStream) : topReader(newTopReader),
        errStream(newErrStream), charsAreInitial(true), state(WAITING) {
}

inline NXMLElementReader* NXMLCallback::currentReader() {
    return (readers.empty() ? &topReader : readers.top());
}

inline int NXMLCallback::getState() const {
    return state;
}

} // namespace regina

#endif

