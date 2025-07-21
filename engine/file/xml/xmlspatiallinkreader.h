
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

/*! \file link/xmlspatiallinkreader.h
 *  \brief Deals with parsing XML data for spatial link packets.
 */

#ifndef __REGINA_XMLSPATIALLINKREADER_H
#ifndef __DOXYGEN
#define __REGINA_XMLSPATIALLINKREADER_H
#endif

#include "regina-core.h"
#include "file/xml/xmlpacketreader.h"
#include "link/spatiallink.h"
#include "utilities/stringutils.h"

namespace regina {

class SpatialLink;

/**
 * An XML packet reader that reads a single spatial link.
 */
class XMLSpatialLinkReader : public XMLPacketReader {
    private:
        std::shared_ptr<PacketOf<SpatialLink>> link_;
            /**< The link currently being read. */

    public:
        /**
         * Creates a new spatial link reader.
         *
         * All parameters not explained here are the same as for the
         * parent class XMLPacketReader.
         *
         * \param props the attributes of the \c spatiallink XML element.
         */
        XMLSpatialLinkReader(XMLTreeResolver& resolver,
            std::shared_ptr<Packet> parent, bool anon, std::string label,
            std::string id, const regina::xml::XMLPropertyDict& props);

        std::shared_ptr<Packet> packetToCommit() override;
        XMLElementReader* startContentSubElement(const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps) override;
        void endContentSubElement(const std::string& subTagName,
            XMLElementReader* subReader) override;
};

/**
 * Helper class that reads the XML element containing a single component of a
 * spatial link.
 */
class XMLSpatialLinkComponentReader : public XMLElementReader {
    private:
        SpatialLink::Component* component_;
            /**< The component currently being read. */

    public:
        /**
         * Creates a new component reader.
         *
         * \param link the link being read.
         */
        XMLSpatialLinkComponentReader(SpatialLink::Component* component);

        XMLElementReader* startSubElement(const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps) override;
        void endSubElement(const std::string& subTagName,
            XMLElementReader* subReader) override;

        /**
         * Indicates whether the XML element has been found to contain
         * invalid data.
         *
         * \return \c true if and only if invalid data has been found.
         */
        bool broken() const;
};

/**
 * Helper class that reads the XML element containing a single node in a
 * component of a spatial link.
 */
class XMLSpatialLinkNodeReader : public XMLElementReader {
    private:
        SpatialLink::Node* node_;
            /**< The node currently being read. */

    public:
        /**
         * Creates a new node reader.
         *
         * \param node the node being read.
         */
        XMLSpatialLinkNodeReader(SpatialLink::Node* node);

        void initialChars(const std::string& chars) override;

        /**
         * Indicates whether the XML element has been found to contain
         * invalid data.
         *
         * \return \c true if and only if invalid data has been found.
         */
        bool broken() const;
};

// Inline functions for XMLSpatialLinkComponentReader

inline XMLSpatialLinkComponentReader::XMLSpatialLinkComponentReader(
        SpatialLink::Component* component) : component_(component) {
}

inline bool XMLSpatialLinkComponentReader::broken() const {
    return ! component_;
}

// Inline functions for XMLSpatialLinkNodeReader

inline XMLSpatialLinkNodeReader::XMLSpatialLinkNodeReader(
        SpatialLink::Node* node) : node_(node) {
}

inline bool XMLSpatialLinkNodeReader::broken() const {
    return ! node_;
}

} // namespace regina

#endif

