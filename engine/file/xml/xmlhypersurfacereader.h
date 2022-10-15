
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

/*! \file hypersurface/xmlhypersurfacereader.h
 *  \brief Deals with parsing XML data for normal hypersurface lists.
 */

#ifndef __REGINA_XMLHYPERSURFACEREADER_H
#ifndef __DOXYGEN
#define __REGINA_XMLHYPERSURFACEREADER_H
#endif

#include <optional>
#include "regina-core.h"
#include "hypersurface/normalhypersurfaces.h"
#include "file/xml/xmlpacketreader.h"

namespace regina {

/**
 * An XML element reader that reads a single normal hypersurface in a
 * 4-manifold triangulation.
 *
 * \nopython
 */
class XMLNormalHypersurfaceReader : public XMLElementReader {
    private:
        std::optional<NormalHypersurface> surface_;
            /**< The normal hypersurface currently being read. */
        SnapshotRef<Triangulation<4>> tri_;
            /**< The triangulation in which this hypersurface lives. */
        HyperCoords coords_;
            /**< The coordinate system used by this hypersurface. */
        int vecEnc_;
            /**< The integer encoding used for the normal hypersurface
                 vector, or 0 if this is unknown. */
        long vecLen_;
            /**< The length of the normal hypersurface vector, or -1 if
                 this is unknown (since 0 is a valid vector length). */
        std::string name_;
            /**< The optional name associated with this normal hypersurface. */

    public:
        /**
         * Creates a new normal hypersurface reader.
         *
         * @param tri the triangulation in which this normal hypersurface lives.
         * @param coords the coordinate system used by this normal hypersurface.
         */
        XMLNormalHypersurfaceReader(const SnapshotRef<Triangulation<4>>& tri,
            HyperCoords coords);

        /**
         * Returns a reference to the normal hypersurface that has been read.
         *
         * @return the normal hypersurface, or no value if an error occurred.
         */
        std::optional<NormalHypersurface>& hypersurface();

        void startElement(const std::string& tagName,
            const regina::xml::XMLPropertyDict& tagProps,
            XMLElementReader* parentReader) override;
        void initialChars(const std::string& chars) override;
        XMLElementReader* startSubElement(const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps) override;
};

/**
 * An XML packet reader that reads a single normal hypersurface list.
 *
 * \nopython
 */
class XMLNormalHypersurfacesReader : public XMLPacketReader {
    private:
        std::shared_ptr<PacketOf<NormalHypersurfaces>> list_;
            /**< The normal hypersurface list currently being read. */
        const Triangulation<4>* tri_;
            /**< The triangulation in which these normal hypersurfaces live. */

    public:
        /**
         * Creates a new normal hypersurface list reader.
         *
         * All parameters not explained here are the same as for the
         * parent class XMLPacketReader.
         *
         * @param props the attributes of the \c hypersurfaces XML element.
         */
        XMLNormalHypersurfacesReader(XMLTreeResolver& resolver,
            std::shared_ptr<Packet> parent, bool anon, std::string label,
            std::string id, const regina::xml::XMLPropertyDict& props);

        std::shared_ptr<Packet> packetToCommit() override;
        XMLElementReader* startContentSubElement(const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps) override;
        void endContentSubElement(const std::string& subTagName,
            XMLElementReader* subReader) override;
};

/**
 * An XML packet reader that reads a single normal hypersurface list
 * using the older second-generation file format.
 *
 * \nopython
 */
class XMLLegacyNormalHypersurfacesReader : public XMLPacketReader {
    private:
        std::shared_ptr<PacketOf<NormalHypersurfaces>> list_;
            /**< The normal hypersurface list currently being read. */
        const Triangulation<4>& tri_;
            /**< The triangulation in which these normal hypersurfaces live. */

    public:
        /**
         * Creates a new normal hypersurface list reader.
         *
         * All parameters not explained here are the same as for the
         * parent class XMLPacketReader.
         *
         * @param tri the triangulation in which these normal hypersurfaces
         * live.
         */
        XMLLegacyNormalHypersurfacesReader(XMLTreeResolver& resolver,
            std::shared_ptr<Packet> parent, bool anon, std::string label,
            std::string id, const Triangulation<4>& tri);

        std::shared_ptr<Packet> packetToCommit() override;
        XMLElementReader* startContentSubElement(const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps) override;
        void endContentSubElement(const std::string& subTagName,
            XMLElementReader* subReader) override;
};

// Inline functions for XMLNormalHypersurfaceReader

inline XMLNormalHypersurfaceReader::XMLNormalHypersurfaceReader(
        const SnapshotRef<Triangulation<4>>& tri, HyperCoords coords) :
        tri_(tri), coords_(coords), vecLen_(-1), vecEnc_(0) {
}

inline std::optional<NormalHypersurface>&
        XMLNormalHypersurfaceReader::hypersurface() {
    return surface_;
}

// Inline functions for XMLNormalHypersurfacesReader

inline std::shared_ptr<Packet> XMLNormalHypersurfacesReader::packetToCommit() {
    return list_;
}

// Inline functions for XMLLegacyNormalHypersurfacesReader

inline XMLLegacyNormalHypersurfacesReader::XMLLegacyNormalHypersurfacesReader(
        XMLTreeResolver& res, std::shared_ptr<Packet> parent, bool anon,
        std::string label, std::string id, const Triangulation<4>& tri) :
        XMLPacketReader(res, std::move(parent), anon, std::move(label),
            std::move(id)),
        list_(nullptr), tri_(tri) {
}

inline std::shared_ptr<Packet>
        XMLLegacyNormalHypersurfacesReader::packetToCommit() {
    return list_;
}

} // namespace regina

#endif

