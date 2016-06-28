
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

/*! \file link/nxmllinkreader.h
 *  \brief Deals with parsing XML data for knot/link packets.
 */

#ifndef __NXMLLINKREADER_H
#ifndef __DOXYGEN
#define __NXMLLINKREADER_H
#endif

#include "regina-core.h"
#include "link/link.h"
#include "packet/nxmlpacketreader.h"

namespace regina {

/**
 * \weakgroup link
 * @{
 */

/**
 * An XML packet reader that reads a single knot or link.
 */
class REGINA_API XMLLinkReader : public NXMLPacketReader {
    private:
        Link* link_;
            /**< The link currently being read. */

    public:
        /**
         * Creates a new knot/link reader.
         *
         * @param resolver the master resolver that will be used to fix
         * dangling packet references after the entire XML file has been read.
         */
        XMLLinkReader(NXMLTreeResolver& resolver);

        virtual NPacket* packet() override;
        virtual NXMLElementReader* startContentSubElement(
            const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps) override;
        virtual void endContentSubElement(const std::string& subTagName,
            NXMLElementReader* subReader) override;
};

/*@}*/

// Inline functions for XMLLinkReader<2>

inline XMLLinkReader::XMLLinkReader(NXMLTreeResolver& resolver) :
        NXMLPacketReader(resolver), link_(new Link()) {
}

inline NPacket* XMLLinkReader::packet() {
    return link_;
}

inline NXMLElementReader* XMLLinkReader::startContentSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict& subTagProps) {
}

inline void XMLLinkReader::endContentSubElement(const std::string& subTagName,
        NXMLElementReader* subReader) {
}

} // namespace regina

#endif

