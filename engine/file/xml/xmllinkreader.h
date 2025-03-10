
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
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/*! \file link/xmllinkreader.h
 *  \brief Deals with parsing XML data for knot/link packets.
 */

#ifndef __REGINA_XMLLINKREADER_H
#ifndef __DOXYGEN
#define __REGINA_XMLLINKREADER_H
#endif

#include "regina-core.h"
#include "file/xml/xmlpacketreader.h"
#include "utilities/stringutils.h"

namespace regina {

class Link;

/**
 * An XML packet reader that reads a single knot or link.
 */
class XMLLinkReader : public XMLPacketReader {
    private:
        std::shared_ptr<PacketOf<Link>> link_;
            /**< The link currently being read. */

    public:
        /**
         * Creates a new knot/link reader.
         *
         * All parameters are the same as for the parent class XMLPacketReader.
         */
        XMLLinkReader(XMLTreeResolver& resolver, std::shared_ptr<Packet> parent,
            bool anon, std::string label, std::string id);

        std::shared_ptr<Packet> packetToCommit() override;
        XMLElementReader* startContentSubElement(const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps) override;
        void endContentSubElement(const std::string& subTagName,
            XMLElementReader* subReader) override;
};

/**
 * Helper class that reads the XML element containing basic information
 * about the crossings of a knot or link.
 */
class XMLLinkCrossingsReader : public XMLElementReader {
    private:
        Link* link_;
            /**< The link currently being read. */
        size_t size_;
            /**< The number of crossings in the link, as defined by the
                 \c size attribute of this tag. */

    public:
        /**
         * Creates a new crossings reader.
         *
         * The given link should be empty; its crossings will be created
         * by this reader.
         *
         * \param link the link being read.
         */
        XMLLinkCrossingsReader(Link* link);

        void startElement(const std::string&,
            const regina::xml::XMLPropertyDict&, XMLElementReader*) override;
        void initialChars(const std::string& chars) override;

        /**
         * Indicates whether the XML element has been found to contain
         * invalid data.
         *
         * \return \c true if and only if invalid data has been found.
         */
        bool broken() const;
};

/**
 * Helper class that reads the XML element containing information on
 * connections between crossings of a knot or link.
 */
class XMLLinkConnectionsReader : public XMLElementReader {
    private:
        Link* link_;
            /**< The link currently being read. */

    public:
        /**
         * Creates a new connections reader.
         *
         * The given link should have its crossings initialised, but
         * with no connections between them.
         *
         * \param link the link being read.
         */
        XMLLinkConnectionsReader(Link* link);

        void initialChars(const std::string& chars) override;

        /**
         * Indicates whether the XML element has been found to contain
         * invalid data.
         *
         * \return \c true if and only if invalid data has been found.
         */
        bool broken() const;
};

/**
 * Helper class that reads the XML element containing information
 * about the individual components of a link.
 */
class XMLLinkComponentsReader : public XMLElementReader {
    private:
        Link* link_;
            /**< The link currently being read. */
        size_t size_;
            /**< The number of components in the link, as defined by the
                 \c size attribute of this tag. */

    public:
        /**
         * Creates a new components reader.
         *
         * The given link should have all its crossings and
         * connections set up, but should have an empty list of components.
         *
         * \param link the link being read.
         */
        XMLLinkComponentsReader(Link* link);

        void startElement(const std::string&,
            const regina::xml::XMLPropertyDict&, XMLElementReader*) override;
        void initialChars(const std::string& chars) override;

        /**
         * Indicates whether the XML element has been found to contain
         * invalid data.
         *
         * \return \c true if and only if invalid data has been found.
         */
        bool broken() const;
};

// Inline functions for XMLLinkCrossingsReader

inline XMLLinkCrossingsReader::XMLLinkCrossingsReader(Link* link) :
        link_(link), size_(0) {
}

inline void XMLLinkCrossingsReader::startElement(const std::string&,
        const regina::xml::XMLPropertyDict& props, XMLElementReader*) {
    if (! valueOf(props.lookup("size"), size_))
        link_ = nullptr;
}

inline bool XMLLinkCrossingsReader::broken() const {
    return ! link_;
}

// Inline functions for XMLLinkConnectionsReader

inline XMLLinkConnectionsReader::XMLLinkConnectionsReader(Link* link) :
        link_(link) {
}

inline bool XMLLinkConnectionsReader::broken() const {
    return ! link_;
}

// Inline functions for XMLLinkComponentsReader

inline XMLLinkComponentsReader::XMLLinkComponentsReader(Link* link) :
        link_(link), size_(0) {
}

inline void XMLLinkComponentsReader::startElement(const std::string&,
        const regina::xml::XMLPropertyDict& props, XMLElementReader*) {
    if (! valueOf(props.lookup("size"), size_))
        link_ = nullptr;
}

inline bool XMLLinkComponentsReader::broken() const {
    return ! link_;
}

} // namespace regina

#endif

