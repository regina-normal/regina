
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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

/*! \file nxmlelementreader.h
 *  \brief Deals with parsing XML program data at the tag level.
 */

#ifndef __NXMLELEMENTREADER_H
#ifndef __DOXYGEN
#define __NXMLELEMENTREADER_H
#endif

#include "utilities/xmlutils.h"

namespace regina {

/**
 * Used to read the contents of a single XML element.  Specifically, this
 * class concerns itself with reading and storing the contents between a
 * single opening tag and the corresponding closing tag.  It is not concerned
 * with reading subelements of the element in question, although the contents
 * of subelements will be made available.
 *
 * Generally a subclass of NXMLElementReader will be used to receive and
 * store information that you care about.  However, if you simply wish to
 * ignore the contents of a particular XML element (and all of its
 * subelements), you can use class NXMLElementReader itself for the
 * element(s) you wish to ignore.
 *
 * When the parser runs through a particular XML element, the routines of
 * the corresponding NXMLElementReader will be called as follows.  First
 * startElement() and initialChars() will be called.  Then for each
 * subelement encountered the following processing will take place:
 * startSubElement() will be called to create a new child reader, the entire
 * cycle of parsing routines will be called upon this child reader and
 * then endSubElement() will be called upon the parent reader, \e after
 * which the child reader will be destroyed.  After all subelements have
 * been processed, endElement() will be called.
 *
 * If at any point parsing is aborted, routine abort() will be called
 * upon all active readers and all active readers will be destroyed.
 *
 * To parse an entire XML file using a variety of element readers (all of
 * which may be of different subclasses of NXMLElementReader), create a
 * new regina::xml::XMLParser with an NXMLCallback as its corresponding
 * callback object.
 *
 * \ifaces Not present.
 */
class NXMLElementReader {
    public:
        /**
         * Creates a new element reader.
         */
        NXMLElementReader();
        /**
         * Destroys this element reader.
         *
         * The default implementation does nothing.
         */
        virtual ~NXMLElementReader();

        /**
         * Signifies that parsing of this XML element is beginning.
         *
         * The default implementation does nothing.
         *
         * @param tagName the name of the opening tag for this element.
         * @param tagProps the properties associated with the opening tag.
         * @param parentReader the reader currently parsing the parent XML
         * element, or 0 if this is the top-level element.  If this
         * paraneter is non-zero, it is guaranteed that startSubElement()
         * has already been called upon the parent reader.
         */
        virtual void startElement(const std::string& tagName,
            const regina::xml::XMLPropertyDict& tagProps,
            NXMLElementReader* parentReader);
        /**
         * Signifies that the initial text belonging to this XML element
         * has been read.  The initial text is everything between the
         * opening tag and the first subelement or closing tag.
         *
         * The default implementation does nothing.
         *
         * @param chars the initial text for this element.
         */
        virtual void initialChars(const std::string& chars);
        /**
         * Signifies that a subelement of this XML element is about to be
         * parsed.
         *
         * The default implementation returns a new NXMLElementReader
         * which can be used to ignore the subelement completely.
         *
         * @param subTagName the name of the subelement opening tag.
         * @param subTagProps the properties associated with the
         * subelement opening tag.
         * @return a newly created element reader that will be used to
         * parse the subelement.  This class should \e not take care of
         * the new reader's destruction; that will be done by the parser.
         */
        virtual NXMLElementReader* startSubElement(
            const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps);
        /**
         * Signifies that parsing has finished for a subelement of this
         * XML element.
         *
         * The default implementation does nothing.
         *
         * @param subTagName the name of the subelement closing tag.
         * @param subReader the child reader that was used to parse the
         * subelement (this is the reader that was returned by the
         * corresponding startSubElement() call).  It is guaranteed that
         * endElement() has already been called upon this child reader
         * and that the child reader has not yet been destroyed.
         */
        virtual void endSubElement(const std::string& subTagName,
            NXMLElementReader* subReader);
        /**
         * Signifies that parsing of this XML element is finished.
         *
         * It is guaranteed that endSubElement() has not yet been called
         * upon the parent reader (if one exists).
         *
         * The default implementation does nothing.
         */
        virtual void endElement();

        /**
         * Signifies that XML parsing has been aborted.
         * This element reader will be destroyed shortly after this
         * routine is called.
         *
         * The default implementation does nothing.
         *
         * @param subReader the corresponding child reader if a
         * subelement is currently being parsed, or 0 otherwise.  If this
         * parameter is non-zero, it is guaranteed that abort() has
         * already been called upon the child reader and that the child
         * reader has not yet been destroyed.
         */
        virtual void abort(NXMLElementReader* subReader);
};

/**
 * A reader for an XML element that contains only characters.
 * Any XML subelements will be ignored (as will any characters occurring
 * after any subelements).
 */
class NXMLTextReader : public NXMLElementReader {
    private:
        std::string text;
            /**< The characters stored in this XML element. */

    public:
        /**
         * Creates a new XML element reader.
         */
        NXMLTextReader();

        /**
         * Returns the characters stored in the XML element that has
         * been read.
         *
         * @return the characters stored in the XML element.
         */
        const std::string& getText();

        virtual void initialChars(const std::string& chars);
};

// Inline functions for NXMLElementReader

inline NXMLElementReader::NXMLElementReader() {
}

inline NXMLElementReader::~NXMLElementReader() {
}

inline void NXMLElementReader::startElement(const std::string&,
        const regina::xml::XMLPropertyDict&, NXMLElementReader*) {
}

inline void NXMLElementReader::initialChars(const std::string&) {
}

inline NXMLElementReader* NXMLElementReader::startSubElement(
        const std::string&, const regina::xml::XMLPropertyDict&) {
    return new NXMLElementReader();
}

inline void NXMLElementReader::endSubElement(const std::string&,
        NXMLElementReader*) {
}

inline void NXMLElementReader::endElement() {
}

inline void NXMLElementReader::abort(NXMLElementReader*) {
}

// Inline functions for NXMLTextReader

inline NXMLTextReader::NXMLTextReader() {
}

inline const std::string& NXMLTextReader::getText() {
    return text;
}

inline void NXMLTextReader::initialChars(const std::string& chars) {
    text = chars;
}

} // namespace regina

#endif

