
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

#include "regina-config.h" // for REGINA_HIGHDIM
#include "packet/packet.h"
#include "file/xml/xmlanglestructreader.h"
#include "file/xml/xmlfilterreader.h"
#include "file/xml/xmlhypersurfacereader.h"
#include "file/xml/xmllinkreader.h"
#include "file/xml/xmlsurfacereader.h"
#include "file/xml/xmlpacketreader.h"
#include "file/xml/xmlpacketreaders.h"
#include "file/xml/xmlsnappeareader.h"
#include "file/xml/xmlspatiallinkreader.h"
#include "file/xml/xmltreeresolver.h"
#include "file/xml/xmltrireader.h"
#include "utilities/stringutils.h"

namespace regina {

// These XML tags can appear beneath all packet types.
// We map them to integers here to allow for real compiler-optimised
// switch statements instead of long if/else lists.
//
// The integer constants that we allow include:
//
// - All of the (positive) integer PacketType constants that were used with
//   the older second-generation XML file format, which were stored numerically
//   in second-generation XML files via <packet typeid=...>.  These are
//   extracted at runtime from the typeid attribute, and so are not all
//   included in the packetXMLTags map below.
//
// - Constants for packets in third-generation XML files, which use different
//   XML tags for different packet types.  In cases where the same reader can
//   be used for second-generation and third-generation formats, we use the
//   same integer constants as above (i.e., the numerical PacketType values);
//   in cases where different readers are required, we introduce separate
//   XML_V7_... constants to denote this (which are negative to avoid clashing
//   with the numerical PacketType constants).
//
// - Some additional (negative) integer constants that do not refer to named
//   packets (e.g., constants for anonymous packets and packet tags).
//
constexpr int XML_LEGACY_CHILD = -1;
constexpr int XML_TAG = -2;
constexpr int XML_ANON = -3;
constexpr int XML_ANONREF = -4;
constexpr int XML_V7_TRIANGULATION = -5;
constexpr int XML_V7_TEXT = -16;
constexpr int XML_V7_ATTACHMENT = -17;
constexpr int XML_V7_SNAPPEA = -18;
constexpr int XML_V7_SURFACES = -19;
constexpr int XML_V7_HYPERSURFACES = -20;
constexpr int XML_V7_ANGLES = -21;
constexpr int XML_V7_FILTER_PROPERTIES = -32;
constexpr int XML_V7_FILTER_COMBINATION = -33;
constexpr int XML_V7_FILTER_PLAIN = -34;

const std::map<std::string, int> packetXMLTags = {
    { "angles", XML_V7_ANGLES },
    { "attachment", XML_V7_ATTACHMENT },
    { "container", static_cast<int>(PacketType::Container) },
    { "filtercomb", XML_V7_FILTER_COMBINATION },
    { "filterplain", XML_V7_FILTER_PLAIN },
    { "filterprop", XML_V7_FILTER_PROPERTIES },
    { "hypersurfaces", XML_V7_HYPERSURFACES },
    { "link", static_cast<int>(PacketType::Link) },
    { "script", static_cast<int>(PacketType::Script) },
    { "snappeadata", XML_V7_SNAPPEA },
    { "spatiallink", static_cast<int>(PacketType::SpatialLink) },
    { "surfaces", XML_V7_SURFACES },
    { "textdata", XML_V7_TEXT },
    { "tri", XML_V7_TRIANGULATION },

    { "anon", XML_ANON },
    { "anonref", XML_ANONREF },
    { "packet", XML_LEGACY_CHILD },
    { "tag", XML_TAG }
};

XMLElementReader* XMLPacketReader::startSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict& subTagProps) {
    auto xmlTag = packetXMLTags.find(subTagName);
    if (xmlTag == packetXMLTags.end()) {
        // This is part of the "real" content specific to the type of
        // packet we are currently reading.
        // Only process it if we have not yet committed the packet.
        if (! packet_) {
            readingContent_ = true;
            return startContentSubElement(subTagName, subTagProps);
        } else
            return new XMLElementReader();
    } else {
        // This is something generic that can appear in all packets.
        // If we have not yet finalised the packet content, do it now.
        commit();

        int xmlTagType = xmlTag->second;
        if (xmlTagType == XML_TAG) {
            // We have <tag name="..."/>.
            std::string packetTag = subTagProps.lookup("name");
            if (packet_ && ! packetTag.empty())
                packet_->addTag(packetTag);
            return new XMLElementReader();
        }
        // All remaining cases are genuine child packets.
        // Ensure that we have somewhere to *put* the child packets.
        if (! (packet_ || anon_))
            return new XMLElementReader();

        // Confirm exactly what kind of child packet we are reading.

        // We will need to fetch and store the following two properties
        // for triangulations.
        long size = 0;
        bool permIndex = false;

        if (xmlTagType == XML_LEGACY_CHILD) {
            // This is a <packet typeid=...>...</packet> element from the
            // older second-generation file format.
            auto prop = subTagProps.find("typeid");
            if (prop == subTagProps.end())
                return new XMLElementReader();
            // Reset xmlTagType to the value of typeid, which is the integer
            // value of the corresponding PacketType constant.
            if (! valueOf(prop->second, xmlTagType))
                return new XMLElementReader();
            if (xmlTagType == static_cast<int>(PacketType::Triangulation2))
                permIndex = true;
        } else if (xmlTagType == XML_V7_TRIANGULATION) {
            // This is a new <tri dim="...">...</tri> element from the
            // newer third-generation file format.
            int dim;
            auto prop = subTagProps.find("dim");
            if (prop == subTagProps.end())
                return new XMLElementReader();
            if (! valueOf(prop->second, dim))
                return new XMLElementReader();
            if (dim < 2 || dim > 15)
                return new XMLElementReader();

            // Reset xmlTagType to the integer value of the PacketType constant
            // for the specific dimension of triangulation that we have.
            switch (dim) {
                case 2:
                    xmlTagType = static_cast<int>(PacketType::Triangulation2);
                    break;
                case 3:
                    xmlTagType = static_cast<int>(PacketType::Triangulation3);
                    break;
                case 4:
                    xmlTagType = static_cast<int>(PacketType::Triangulation4);
                    break;
                default:
                    xmlTagType = 100 + dim; /* 105 .. 115 */
                    break;
            }

            // Fetch the number of top-dimensional simplices.
            prop = subTagProps.find("size");
            if (! (prop != subTagProps.end() && valueOf(prop->second, size) &&
                    size >= 0))
                return new XMLElementReader();

            // Identify how permutations are stored.
            prop = subTagProps.find("perm");
            if (prop == subTagProps.end())
                return new XMLElementReader();
            if (prop->second == "index")
                permIndex = true;
            else if (prop->second == "imagepack")
                permIndex = false;
            else
                return new XMLElementReader();
        }

        // Fetch some properties that we will need once the child reader
        // has actually created the packet.
        std::string childLabel = subTagProps.lookup("label");
        std::string childID = subTagProps.lookup("id");

        // Run through all the packet types that our file format understands.
        switch (xmlTagType) {
            case XML_ANON:
                return new XMLPacketReader(resolver_, packet_, true,
                    std::move(childLabel), std::move(childID));
            case XML_ANONREF:
                return new XMLAnonRefReader(resolver_, packet_, anon_,
                    std::move(childLabel), std::move(childID));
            case static_cast<int>(PacketType::Container):
                return new XMLContainerReader(resolver_, packet_, anon_,
                    std::move(childLabel), std::move(childID));
            case static_cast<int>(PacketType::Triangulation2):
                return new XMLTriangulationReader<2>(resolver_, packet_, anon_,
                    std::move(childLabel), std::move(childID), size, permIndex);
            case static_cast<int>(PacketType::Triangulation3):
                return new XMLTriangulationReader<3>(resolver_, packet_, anon_,
                    std::move(childLabel), std::move(childID), size, permIndex);
            case static_cast<int>(PacketType::Triangulation4):
                return new XMLTriangulationReader<4>(resolver_, packet_, anon_,
                    std::move(childLabel), std::move(childID), size, permIndex);
            case static_cast<int>(PacketType::SnapPea):
                return new XMLLegacySnapPeaReader(resolver_, packet_, anon_,
                    std::move(childLabel), std::move(childID));
            case XML_V7_SNAPPEA:
                return new XMLSnapPeaReader(resolver_, packet_, anon_,
                    std::move(childLabel), std::move(childID));
            case static_cast<int>(PacketType::Link):
                return new XMLLinkReader(resolver_, packet_, anon_,
                    std::move(childLabel), std::move(childID));
            case static_cast<int>(PacketType::SpatialLink):
                return new XMLSpatialLinkReader(resolver_, packet_, anon_,
                    std::move(childLabel), std::move(childID), subTagProps);
            case static_cast<int>(PacketType::Text):
                return new XMLLegacyTextReader(resolver_, packet_, anon_,
                    std::move(childLabel), std::move(childID));
            case XML_V7_TEXT:
                return new XMLTextReader(resolver_, packet_, anon_,
                    std::move(childLabel), std::move(childID));
            case static_cast<int>(PacketType::Script):
                return new XMLScriptReader(resolver_, packet_, anon_,
                    std::move(childLabel), std::move(childID));
            case static_cast<int>(PacketType::SurfaceFilter):
                return new XMLLegacyFilterReader(resolver_, packet_, anon_,
                    std::move(childLabel), std::move(childID));
            case XML_V7_FILTER_PROPERTIES:
                return new XMLPropertiesFilterReader(resolver_, packet_, anon_,
                    std::move(childLabel), std::move(childID), subTagProps);
            case XML_V7_FILTER_COMBINATION:
                return new XMLCombinationFilterReader(resolver_, packet_, anon_,
                    std::move(childLabel), std::move(childID), subTagProps);
            case XML_V7_FILTER_PLAIN:
                return new XMLPlainFilterReader(resolver_, packet_, anon_,
                    std::move(childLabel), std::move(childID));
            case static_cast<int>(PacketType::Attachment):
                return new XMLLegacyPDFReader(resolver_, packet_, anon_,
                    std::move(childLabel), std::move(childID));
            case XML_V7_ATTACHMENT:
                return new XMLAttachmentReader(resolver_, packet_, anon_,
                    std::move(childLabel), std::move(childID), subTagProps);
            case XML_V7_SURFACES:
                return new XMLNormalSurfacesReader(resolver_, packet_, anon_,
                    std::move(childLabel), std::move(childID), subTagProps);
            case XML_V7_HYPERSURFACES:
                return new XMLNormalHypersurfacesReader(resolver_, packet_,
                    anon_, std::move(childLabel), std::move(childID),
                    subTagProps);
            case XML_V7_ANGLES:
                return new XMLAngleStructuresReader(resolver_, packet_, anon_,
                    std::move(childLabel), std::move(childID), subTagProps);
            case static_cast<int>(PacketType::NormalSurfaces):
                if (auto tri = std::dynamic_pointer_cast<Triangulation<3>>(
                        packet_))
                    return new XMLLegacyNormalSurfacesReader(resolver_, packet_,
                        anon_, std::move(childLabel), std::move(childID), *tri);
                else
                    return new XMLElementReader();
            case static_cast<int>(PacketType::NormalHypersurfaces):
                if (auto tri = std::dynamic_pointer_cast<Triangulation<4>>(
                        packet_))
                    return new XMLLegacyNormalHypersurfacesReader(resolver_,
                        packet_, anon_, std::move(childLabel),
                        std::move(childID), *tri);
                else
                    return new XMLElementReader();
            case static_cast<int>(PacketType::AngleStructures):
                if (auto tri = std::dynamic_pointer_cast<Triangulation<3>>(
                        packet_))
                    return new XMLLegacyAngleStructuresReader(resolver_,
                        packet_, anon_, std::move(childLabel),
                        std::move(childID), *tri);
                else
                    return new XMLElementReader();
            case static_cast<int>(PacketType::Triangulation5):
                return new XMLTriangulationReader<5>(resolver_, packet_, anon_,
                    std::move(childLabel), std::move(childID), size, permIndex);
            case static_cast<int>(PacketType::Triangulation6):
                return new XMLTriangulationReader<6>(resolver_, packet_, anon_,
                    std::move(childLabel), std::move(childID), size, permIndex);
            case static_cast<int>(PacketType::Triangulation7):
                return new XMLTriangulationReader<7>(resolver_, packet_, anon_,
                    std::move(childLabel), std::move(childID), size, permIndex);
            case static_cast<int>(PacketType::Triangulation8):
                return new XMLTriangulationReader<8>(resolver_, packet_, anon_,
                    std::move(childLabel), std::move(childID), size, permIndex);
#ifdef REGINA_HIGHDIM
            case static_cast<int>(PacketType::Triangulation9):
                return new XMLTriangulationReader<9>(resolver_, packet_, anon_,
                    std::move(childLabel), std::move(childID), size, permIndex);
            case static_cast<int>(PacketType::Triangulation10):
                return new XMLTriangulationReader<10>(resolver_, packet_, anon_,
                    std::move(childLabel), std::move(childID), size, permIndex);
            case static_cast<int>(PacketType::Triangulation11):
                return new XMLTriangulationReader<11>(resolver_, packet_, anon_,
                    std::move(childLabel), std::move(childID), size, permIndex);
            case static_cast<int>(PacketType::Triangulation12):
                return new XMLTriangulationReader<12>(resolver_, packet_, anon_,
                    std::move(childLabel), std::move(childID), size, permIndex);
            case static_cast<int>(PacketType::Triangulation13):
                return new XMLTriangulationReader<13>(resolver_, packet_, anon_,
                    std::move(childLabel), std::move(childID), size, permIndex);
            case static_cast<int>(PacketType::Triangulation14):
                return new XMLTriangulationReader<14>(resolver_, packet_, anon_,
                    std::move(childLabel), std::move(childID), size, permIndex);
            case static_cast<int>(PacketType::Triangulation15):
                return new XMLTriangulationReader<15>(resolver_, packet_, anon_,
                    std::move(childLabel), std::move(childID), size, permIndex);
#endif /* REGINA_HIGHDIM */
            default:
                return new XMLElementReader();
        }
    }
}

void XMLPacketReader::endSubElement(const std::string& subTagName,
        XMLElementReader* subReader) {
    if (readingContent_) {
        // This sub-element was part of the packet's "real" content.
        endContentSubElement(subTagName, subReader);
        readingContent_ = false;
    }
}

void XMLPacketReader::endElement() {
    commit();
}

void XMLPacketReader::abort(XMLElementReader* /* subReader */) {
    // Fetch the packet under construction if we don't have it already,
    // since we promised to do this.
    if (! packet_)
        packet_ = packetToCommit();

    // Since we are using shared_ptr to store our packets, someone at
    // some point should take care of destroying packet_.  We won't
    // worry about this here.
}

void XMLPacketReader::commit() {
    if (! packet_) {
        if ((packet_ = packetToCommit())) {
            if (! label_.empty())
                packet_->setLabel(label_);
            if (! id_.empty())
                resolver_.storeID(id_, packet_);
            if ((! anon_) && parent_ && ! packet_->parent())
                parent_->append(packet_);
        }
    }
}

} // namespace regina

