
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

/*! \file angle/xmlanglestructreader.h
 *  \brief Deals with parsing XML data for angle structure lists.
 */

#ifndef __REGINA_XMLANGLESTRUCTREADER_H
#ifndef __DOXYGEN
#define __REGINA_XMLANGLESTRUCTREADER_H
#endif

#include <optional>
#include "regina-core.h"
#include "file/xml/xmlpacketreader.h"
#include "angle/anglestructures.h"

namespace regina {

/**
 * An XML element reader that reads a single angle structure.
 *
 * \ifacespython Not present.
 */
class XMLAngleStructureReader : public XMLElementReader {
    private:
        std::optional<AngleStructure> angles_;
            /**< The angle structure currently being read. */
        SnapshotRef<Triangulation<3>> tri_;
            /**< The triangulation on which this angle structure is placed. */
        long vecLen;
            /**< The length of corresponding angle structure vector. */

    public:
        /**
         * Creates a new angle structure reader.
         *
         * @param newTri the triangulation on which this angle structure lies.
         */
        XMLAngleStructureReader(const SnapshotRef<Triangulation<3>>& tri);

        /**
         * Returns a reference to the angle structure that has been read.
         *
         * @return the angle structure, or no value if an error occurred.
         */
        std::optional<AngleStructure>& structure();

        void startElement(const std::string& tagName,
            const regina::xml::XMLPropertyDict& tagProps,
            XMLElementReader* parentReader) override;
        void initialChars(const std::string& chars) override;
};

/**
 * An XML packet reader that reads a single angle structure list.
 *
 * \ifacespython Not present.
 */
class XMLAngleStructuresReader : public XMLPacketReader {
    private:
        std::shared_ptr<PacketOf<AngleStructures>> list_;
            /**< The angle structure list currently being read. */
        const Triangulation<3>* tri_;
            /**< The triangulation on which these angle structures
                 are placed. */

    public:
        /**
         * Creates a new angle structure list reader.
         *
         * All parameters not explained here are the same as for the
         * parent class XMLPacketReader.
         *
         * @param props the attributes of the \c angles XML element.
         */
        XMLAngleStructuresReader(XMLTreeResolver& resolver,
            std::shared_ptr<Packet> parent, bool anon, std::string label,
            std::string id, const regina::xml::XMLPropertyDict& props);

        std::shared_ptr<Packet> packetToCommit() override;
        XMLElementReader* startContentSubElement(const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps) override;
        void endContentSubElement(const std::string& subTagName,
            XMLElementReader* subReader) override;
};

/**
 * An XML packet reader that reads a single angle structure list using
 * the older second-generation file format.
 *
 * \ifacespython Not present.
 */
class XMLLegacyAngleStructuresReader : public XMLPacketReader {
    private:
        std::shared_ptr<PacketOf<AngleStructures>> list_;
            /**< The angle structure list currently being read. */
        const Triangulation<3>& tri_;
            /**< The triangulation on which these angle structures
                 are placed. */

    public:
        /**
         * Creates a new angle structure list reader.
         *
         * All parameters not explained here are the same as for the
         * parent class XMLPacketReader.
         *
         * @param tri the triangulation on which these angle
         * structures are placed.
         */
        XMLLegacyAngleStructuresReader(XMLTreeResolver& resolver,
            std::shared_ptr<Packet> parent, bool anon, std::string label,
            std::string id, const Triangulation<3>& tri);

        std::shared_ptr<Packet> packetToCommit() override;
        XMLElementReader* startContentSubElement(const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps) override;
        void endContentSubElement(const std::string& subTagName,
            XMLElementReader* subReader) override;
        void endElement() override;
};

// Inline functions for XMLAngleStructureReader

inline XMLAngleStructureReader::XMLAngleStructureReader(
        const SnapshotRef<Triangulation<3>>& tri) : tri_(tri), vecLen(-1) {
}

inline std::optional<AngleStructure>& XMLAngleStructureReader::structure() {
    return angles_;
}

// Inline functions for XMLAngleStructuresReader

inline std::shared_ptr<Packet> XMLAngleStructuresReader::packetToCommit() {
    return list_;
}

// Inline functions for XMLLegacyAngleStructuresReader

inline XMLLegacyAngleStructuresReader::XMLLegacyAngleStructuresReader(
        XMLTreeResolver& res, std::shared_ptr<Packet> parent, bool anon,
        std::string label, std::string id, const Triangulation<3>& tri) :
        XMLPacketReader(res, std::move(parent), anon, std::move(label),
            std::move(id)),
        list_(nullptr), tri_(tri) {
}

inline std::shared_ptr<Packet> XMLLegacyAngleStructuresReader::packetToCommit() {
    return list_;
}

} // namespace regina

#endif

