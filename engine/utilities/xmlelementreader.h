
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

/*! \file utilities/xmlelementreader.h
 *  \brief Deals with parsing XML program data at the tag level.
 */

#ifndef __XMLELEMENTREADER_H
#ifndef __DOXYGEN
#define __XMLELEMENTREADER_H
#endif

#include "regina-core.h"
#include "utilities/xmlutils.h"

namespace regina {

/**
 * \weakgroup file
 * @{
 */

/**
 * Used to read the contents of a single XML element.  Specifically, this
 * class concerns itself with reading and storing the contents between a
 * single opening tag and the corresponding closing tag.  It is not concerned
 * with reading subelements of the element in question, although the contents
 * of subelements will be made available.
 *
 * Generally a subclass of XMLElementReader will be used to receive and
 * store information that you care about.  However, if you simply wish to
 * ignore the contents of a particular XML element (and all of its
 * subelements), you can use class XMLElementReader itself for the
 * element(s) you wish to ignore.
 *
 * When the parser runs through a particular XML element, the routines of
 * the corresponding XMLElementReader will be called as follows.  First
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
 * which may be of different subclasses of XMLElementReader), create a
 * new regina::xml::XMLParser with an XMLCallback as its corresponding
 * callback object.
 *
 * When parsing begins on an entire XML file, an additional call is made:
 * usingParser() will be called on the top-level element reader, so that
 * it can gain direct access to the parser if required.
 *
 * \ifacespython Not present.
 */
class REGINA_API XMLElementReader {
    public:
        /**
         * Creates a new element reader.
         */
        XMLElementReader();
        /**
         * Destroys this element reader.
         *
         * The default implementation does nothing.
         */
        virtual ~XMLElementReader();

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
            XMLElementReader* parentReader);
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
         * The default implementation returns a new XMLElementReader
         * which can be used to ignore the subelement completely.
         *
         * @param subTagName the name of the subelement opening tag.
         * @param subTagProps the properties associated with the
         * subelement opening tag.
         * @return a newly created element reader that will be used to
         * parse the subelement.  This class should \e not take care of
         * the new reader's destruction; that will be done by the parser.
         */
        virtual XMLElementReader* startSubElement(
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
            XMLElementReader* subReader);
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
         * Called for the top-level element in an XML file when parsing
         * begins.  This allows direct access to the parser if needed
         * (for instance, to change the character encoding).
         *
         * The default implementation does nothing.
         *
         * @param parser the current XML parser.
         */
        virtual void usingParser(regina::xml::XMLParser* parser);

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
        virtual void abort(XMLElementReader* subReader);
};

/**
 * A reader for an XML element that contains only characters.
 * Any XML subelements will be ignored (as will any characters occurring
 * after any subelements).
 *
 * \ifacespython Not present.
 */
class REGINA_API XMLCharsReader : public XMLElementReader {
    private:
        std::string readChars;
            /**< The characters stored in this XML element. */

    public:
        /**
         * Creates a new XML element reader.
         */
        XMLCharsReader();

        /**
         * Returns the characters stored in the XML element that has
         * been read.
         *
         * @return the characters stored in the XML element.
         */
        const std::string& chars();

        virtual void initialChars(const std::string& chars);
};

/*@}*/

// Inline functions for XMLElementReader

inline XMLElementReader::XMLElementReader() {
}

inline XMLElementReader::~XMLElementReader() {
}

inline void XMLElementReader::startElement(const std::string&,
        const regina::xml::XMLPropertyDict&, XMLElementReader*) {
}

inline void XMLElementReader::initialChars(const std::string&) {
}

inline XMLElementReader* XMLElementReader::startSubElement(
        const std::string&, const regina::xml::XMLPropertyDict&) {
    return new XMLElementReader();
}

inline void XMLElementReader::endSubElement(const std::string&,
        XMLElementReader*) {
}

inline void XMLElementReader::endElement() {
}

inline void XMLElementReader::usingParser(regina::xml::XMLParser*) {
}

inline void XMLElementReader::abort(XMLElementReader*) {
}

// Inline functions for XMLCharsReader

inline XMLCharsReader::XMLCharsReader() {
}

inline const std::string& XMLCharsReader::chars() {
    return readChars;
}

inline void XMLCharsReader::initialChars(const std::string& chars) {
    readChars = chars;
}

} // namespace regina

#endif

