
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

/*! \file packet/xmlpacketreader.h
 *  \brief Deals with parsing XML data for individual packets.
 */

#ifndef __XMLPACKETREADER_H
#ifndef __DOXYGEN
#define __XMLPACKETREADER_H
#endif

#include "regina-core.h"
#include "utilities/xmlelementreader.h"

namespace regina {

class Packet;
class XMLTreeResolver;

/**
 * \weakgroup packet
 * @{
 */

/**
 * An XML element reader that reads the data for an individual packet.
 *
 * Generally a subclass of XMLPacketReader will be used to receive and
 * store packets that you care about.  However, if you simply wish to
 * ignore a particular packet (and all of its descendants), you can use
 * class XMLPacketReader itself for the packet(s) you wish to ignore.
 *
 * Routine packet() is used to return the packet that was read; see
 * its documentation for further notes on how the packet should be
 * constructed.
 *
 * Routines startSubElement() and endSubElement() should \e not be
 * overridden by derived classes.  They determine whether the subelement
 * is another packet element or a packet tag; if so then they deal with
 * the subelement themselves (packet elements will be read using a new
 * XMLPacketReader of the correct type), and if not then they call
 * startContentSubElement() and endContentSubElement() which \e should
 * be overridden for processing of non-packet XML subelements.
 *
 * If routine abort() is overridden, it \e must at some point call
 * XMLPacketReader::abort() which will destroy whatever new packets
 * have already been created.
 *
 * \ifacespython Not present.
 */
class REGINA_API XMLPacketReader : public XMLElementReader {
    private:
        std::string childLabel;
            /**< The packet label to give the child packet currently
                 being read. */
        std::string childID;
            /**< The internal ID stored in the data file for the child packet
                 currently being read. */

    protected:
        XMLTreeResolver& resolver_;
            /**< The master resolver that will be used to fix dangling packet
                 references after the entire XML file has been read. */

    public:
        /**
         * Creates a new packet element reader.
         *
         * @param resolver the master resolver that will be used to fix
         * dangling packet references after the entire XML file has been read.
         */
        XMLPacketReader(XMLTreeResolver& resolver);

        /**
         * Returns the newly allocated packet that has been read by
         * this element reader.
         *
         * Deallocation of this new packet is not the responsibility of
         * this class.  Once this routine gives a non-zero return value,
         * it should continue to give the same non-zero return value
         * from this point onwards.
         *
         * If this routine is ever to give a non-zero return value, it
         * \e must be giving that non-zero return value by the time the
         * first child packet or packet tag is encountered; otherwise
         * child packets will not be inserted into the packet tree and/or
         * packet tags will not be added.
         *
         * The newly allocated packet should not be given a packet
         * label.  This will be done by XMLPacketReader::endSubElement().
         *
         * The newly allocated packet may or may not be inserted in the
         * packet tree structure; this does not matter (although if it
         * is inserted it must be inserted in the correct place).
         *
         * The newly allocated packet should not be given any associated
         * packet tags.  This will be done by
         * XMLPacketReader::startSubElement().
         *
         * The default implementation returns 0.
         *
         * @return the packet that has been read, or 0 if packet reading
         * is incomplete, the packet should be ignored or an error
         * occurred.
         */
        virtual Packet* packet();

        /**
         * Used instead of startSubElement() for XML subelements that
         * are not child packets or packet tags.
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
        virtual XMLElementReader* startContentSubElement(
            const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps);
        /**
         * Used instead of endSubElement() for XML subelements that are
         * not child packets or packet tags.
         *
         * The default implementation does nothing.
         *
         * @param subTagName the name of the subelement closing tag.
         * @param subReader the child reader that was used to parse the
         * subelement (this is the reader that was returned by the
         * corresponding startContentSubElement() call).  It is guaranteed
         * that endElement() has already been called upon this child reader
         * and that the child reader has not yet been destroyed.
         */
        virtual void endContentSubElement(const std::string& subTagName,
            XMLElementReader* subReader);

        virtual XMLElementReader* startSubElement(
            const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps);
        virtual void endSubElement(const std::string& subTagName,
            XMLElementReader* subReader);
        virtual void abort(XMLElementReader *subReader);
};

/*@}*/

// Inline functions for XMLPacketReader

inline XMLPacketReader::XMLPacketReader(XMLTreeResolver& resolver) :
        resolver_(resolver) {
}

inline Packet* XMLPacketReader::packet() {
    return 0;
}

inline XMLElementReader* XMLPacketReader::startContentSubElement(
        const std::string&, const regina::xml::XMLPropertyDict&) {
    return new XMLElementReader();
}

inline void XMLPacketReader::endContentSubElement(const std::string&,
        XMLElementReader*) {
}

} // namespace regina

#endif

