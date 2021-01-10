
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

/*! \file link/xmllinkreader.h
 *  \brief Deals with parsing XML data for knot/link packets.
 */

#ifndef __XMLLINKREADER_H
#ifndef __DOXYGEN
#define __XMLLINKREADER_H
#endif

#include "regina-core.h"
#include "link/link.h"
#include "packet/xmlpacketreader.h"
#include "utilities/stringutils.h"

namespace regina {

/**
 * \weakgroup link
 * @{
 */

/**
 * An XML packet reader that reads a single knot or link.
 */
class REGINA_API XMLLinkReader : public XMLPacketReader {
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
        XMLLinkReader(XMLTreeResolver& resolver);

        virtual Packet* packet() override;
        virtual XMLElementReader* startContentSubElement(
            const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps) override;
        virtual void endContentSubElement(const std::string& subTagName,
            XMLElementReader* subReader) override;
};

/**
 * Helper class that reads the XML element containing basic information
 * about the crossings of a knot or link.
 */
class REGINA_API XMLLinkCrossingsReader : public XMLElementReader {
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
         * @param link the link being read.
         */
        XMLLinkCrossingsReader(Link* link);

        virtual void startElement(const std::string&,
            const regina::xml::XMLPropertyDict&, XMLElementReader*) override;
        virtual void initialChars(const std::string& chars) override;

        /**
         * Indicates whether the XML element has been found to contain
         * invalid data.
         *
         * @return \c true if and only if invalid data has been found.
         */
        bool broken() const;
};

/**
 * Helper class that reads the XML element containing information on
 * connections between crossings of a knot or link.
 */
class REGINA_API XMLLinkConnectionsReader : public XMLElementReader {
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
         * @param link the link being read.
         */
        XMLLinkConnectionsReader(Link* link);

        virtual void initialChars(const std::string& chars);

        /**
         * Indicates whether the XML element has been found to contain
         * invalid data.
         *
         * @return \c true if and only if invalid data has been found.
         */
        bool broken() const;
};

/**
 * Helper class that reads the XML element containing information
 * about the individual components of a link.
 */
class REGINA_API XMLLinkComponentsReader : public XMLElementReader {
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
         * @param link the link being read.
         */
        XMLLinkComponentsReader(Link* link);

        virtual void startElement(const std::string&,
            const regina::xml::XMLPropertyDict&, XMLElementReader*);
        virtual void initialChars(const std::string& chars);

        /**
         * Indicates whether the XML element has been found to contain
         * invalid data.
         *
         * @return \c true if and only if invalid data has been found.
         */
        bool broken() const;
};

/*@}*/

// Inline functions for XMLLinkReader

inline XMLLinkReader::XMLLinkReader(XMLTreeResolver& resolver) :
        XMLPacketReader(resolver), link_(new Link()) {
}

inline Packet* XMLLinkReader::packet() {
    return link_;
}

inline XMLElementReader* XMLLinkReader::startContentSubElement(
        const std::string& subTagName, const regina::xml::XMLPropertyDict&) {
    if (! link_)
        return new XMLElementReader();

    if (subTagName == "crossings")
        return new XMLLinkCrossingsReader(link_);
    else if (subTagName == "connections")
        return new XMLLinkConnectionsReader(link_);
    else if (subTagName == "components")
        return new XMLLinkComponentsReader(link_);

    return new XMLElementReader();
}

inline void XMLLinkReader::endContentSubElement(const std::string& subTagName,
        XMLElementReader* reader) {
    if (! link_)
        return;

    if (subTagName == "crossings") {
        if (static_cast<XMLLinkCrossingsReader*>(reader)->broken()) {
            delete link_;
            link_ = 0;
        }
    } else if (subTagName == "connections") {
        if (static_cast<XMLLinkConnectionsReader*>(reader)->broken()) {
            delete link_;
            link_ = 0;
        }
    } else if (subTagName == "components") {
        if (static_cast<XMLLinkComponentsReader*>(reader)->broken()) {
            delete link_;
            link_ = 0;
        }
    }
}

// Inline functions for XMLLinkCrossingsReader

XMLLinkCrossingsReader::XMLLinkCrossingsReader(Link* link) :
        link_(link), size_(0) {
}

void XMLLinkCrossingsReader::startElement(const std::string&,
        const regina::xml::XMLPropertyDict& props, XMLElementReader*) {
    if (! valueOf(props.lookup("size"), size_))
        link_ = 0;
}

bool XMLLinkCrossingsReader::broken() const {
    return ! link_;
}

// Inline functions for XMLLinkConnectionsReader

XMLLinkConnectionsReader::XMLLinkConnectionsReader(Link* link) : link_(link) {
}

bool XMLLinkConnectionsReader::broken() const {
    return ! link_;
}

// Inline functions for XMLLinkComponentsReader

XMLLinkComponentsReader::XMLLinkComponentsReader(Link* link) :
        link_(link), size_(0) {
}

void XMLLinkComponentsReader::startElement(const std::string&,
        const regina::xml::XMLPropertyDict& props, XMLElementReader*) {
    if (! valueOf(props.lookup("size"), size_))
        link_ = 0;
}

bool XMLLinkComponentsReader::broken() const {
    return ! link_;
}

} // namespace regina

#endif

