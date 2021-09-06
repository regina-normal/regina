
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

#include "packet/packet.h"
#include "file/xml/xmlanglestructreader.h"
#include "file/xml/xmlfilterreader.h"
#include "file/xml/xmlhypersurfacereader.h"
#include "file/xml/xmllinkreader.h"
#include "file/xml/xmlsurfacereader.h"
#include "file/xml/xmlpacketreader.h"
#include "file/xml/xmlpacketreaders.h"
#include "file/xml/xmlsnappeareader.h"
#include "file/xml/xmltreeresolver.h"
#include "file/xml/xmltrireader2.h"
#include "file/xml/xmltrireader3.h"
#include "file/xml/xmltrireader4.h"
#include "utilities/stringutils.h"

namespace regina {

// These XML tags can appear beneath all packet types.
// We map them to strings here to allow for real compiler-optimised
// switch statements instead of long if/else lists.
constexpr int PACKET_LEGACY_CHILD = -1;
constexpr int PACKET_TAG = -2;
constexpr int PACKET_TRIANGULATION_ANY = -3;
constexpr int PACKET_V7_TEXT = -16;
constexpr int PACKET_V7_PDF = -17;
constexpr int PACKET_V7_SNAPPEA = -18;
constexpr int PACKET_V7_FILTER_PROPERTIES = -19;
constexpr int PACKET_V7_FILTER_COMBINATION = -20;
constexpr int PACKET_V7_FILTER_PLAIN = -21;
const std::map<std::string, int> packetXMLTags = {
    { "angles", PACKET_ANGLESTRUCTURES },
    { "container", PACKET_CONTAINER },
    { "filtercomb", PACKET_V7_FILTER_COMBINATION },
    { "filterplain", PACKET_V7_FILTER_PLAIN },
    { "filterprop", PACKET_V7_FILTER_PROPERTIES },
    { "hypersurfaces", PACKET_NORMALHYPERSURFACES },
    { "link", PACKET_LINK },
    { "pdfdata", PACKET_V7_PDF },
    { "script", PACKET_SCRIPT },
    { "snappeadata", PACKET_V7_SNAPPEA },
    { "surfaces", PACKET_NORMALSURFACES },
    { "textdata", PACKET_V7_TEXT },

    { "tri", PACKET_TRIANGULATION_ANY },

    { "packet", PACKET_LEGACY_CHILD },
    { "tag", PACKET_TAG }
};

XMLElementReader* XMLPacketReader::startSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict& subTagProps) {
    auto xmlTag = packetXMLTags.find(subTagName);
    if (xmlTag == packetXMLTags.end()) {
        // This is part of the "real" content specific to the type of
        // packet we are currently reading.
        return startContentSubElement(subTagName, subTagProps);
    } else {
        int xmlTagType = xmlTag->second;

        // This is something generic that can appear in all packets.
        if (xmlTagType == PACKET_TAG) {
            // We have <tag name="..."/>.
            if (Packet* me = packet()) {
                std::string packetTag = subTagProps.lookup("name");
                if (! packetTag.empty())
                    me->addTag(packetTag);
            }
            return new XMLElementReader();
        }
        // All remaining cases are genuine child packets.
        // Confirm exactly what kind of child packet we are reading.

        // We will need to fetch and store the following two properties
        // for triangulations.
        long size = 0;
        bool permIndex = (xmlTagType == PACKET_TRIANGULATION2);

        if (xmlTagType == PACKET_LEGACY_CHILD) {
            // This is a <packet typeid=...>...</packet> element from the
            // older Regina 6.x file format.
            auto prop = subTagProps.find("typeid");
            if (prop == subTagProps.end())
                return new XMLPacketReader(resolver_);
            if (! valueOf(prop->second, xmlTagType))
                return new XMLPacketReader(resolver_);
        } else if (xmlTagType == PACKET_TRIANGULATION_ANY) {
            // This is a new <tri dim="...">...</tri> element from the
            // newer Regina 7.0 file format.
            int dim;
            auto prop = subTagProps.find("dim");
            if (prop == subTagProps.end())
                return new XMLPacketReader(resolver_);
            if (! valueOf(prop->second, dim))
                return new XMLPacketReader(resolver_);
            if (dim < 2 || dim > 15)
                return new XMLPacketReader(resolver_);
            switch (dim) {
                case 2: xmlTagType = PACKET_TRIANGULATION2; break;
                case 3: xmlTagType = PACKET_TRIANGULATION3; break;
                case 4: xmlTagType = PACKET_TRIANGULATION4; break;
                default: xmlTagType = 100 + dim; /* 105 .. 115 */ break;
            }

            // Fetch the number of top-dimensional simplices.
            prop = subTagProps.find("size");
            if (! (prop != subTagProps.end() && valueOf(prop->second, size) &&
                    size >= 0))
                return new XMLPacketReader(resolver_);

            // Identify how permutations are stored.
            prop = subTagProps.find("perm");
            if (prop == subTagProps.end())
                return new XMLPacketReader(resolver_);
            if (prop->second == "index")
                permIndex = true;
            else if (prop->second == "imagepack")
                permIndex = false;
            else
                return new XMLPacketReader(resolver_);
        }

        // Fetch some properties that we will need once the child reader
        // has actually created the packet.
        auto prop = subTagProps.find("label");
        if (prop != subTagProps.end())
            childLabel_ = prop->second;

        prop = subTagProps.find("id");
        if (prop != subTagProps.end())
            childID_ = prop->second;

        // Run through all the packet types that our file format understands.
        switch (xmlTagType) {
            case PACKET_CONTAINER:
                return childReader_ = new XMLContainerReader(resolver_);
            case PACKET_TEXT:
                return childReader_ = new XMLLegacyTextReader(resolver_);
            case PACKET_V7_TEXT:
                return childReader_ = new XMLTextReader(resolver_);
            case PACKET_TRIANGULATION3:
                return childReader_ = new XMLTriangulationReader<3>(
                    resolver_, size, permIndex);
            case PACKET_NORMALSURFACES:
                return childReader_ = new XMLNormalSurfacesReader(
                    dynamic_cast<Triangulation<3>*>(packet()), resolver_);
            case PACKET_SCRIPT:
                return childReader_ = new XMLScriptReader(resolver_);
            case PACKET_SURFACEFILTER:
                return childReader_ = new XMLLegacyFilterReader(resolver_);
            case PACKET_V7_FILTER_PROPERTIES:
                return childReader_ = new XMLPropertiesFilterReader(resolver_);
            case PACKET_V7_FILTER_COMBINATION:
                return childReader_ = new XMLCombinationFilterReader(resolver_);
            case PACKET_V7_FILTER_PLAIN:
                return childReader_ = new XMLPlainFilterReader(resolver_);
            case PACKET_ANGLESTRUCTURES:
                return childReader_ = new XMLAngleStructuresReader(
                    dynamic_cast<Triangulation<3>*>(packet()), resolver_);
            case PACKET_PDF:
                return childReader_ = new XMLLegacyPDFReader(resolver_);
            case PACKET_V7_PDF:
                return childReader_ = new XMLPDFReader(resolver_);
            case PACKET_TRIANGULATION2:
                return childReader_ = new XMLTriangulationReader<2>(
                    resolver_, size, permIndex);
            case PACKET_TRIANGULATION4:
                return childReader_ = new XMLTriangulationReader<4>(
                    resolver_, size, permIndex);
            case PACKET_NORMALHYPERSURFACES:
                return childReader_ = new XMLNormalHypersurfacesReader(
                    dynamic_cast<Triangulation<4>*>(packet()), resolver_);
            case PACKET_SNAPPEATRIANGULATION:
                return childReader_ = new XMLLegacySnapPeaReader(resolver_);
            case PACKET_V7_SNAPPEA:
                return childReader_ = new XMLSnapPeaReader(resolver_);
            case PACKET_LINK:
                return childReader_ = new XMLLinkReader(resolver_);
#ifndef REGINA_LOWDIMONLY
            case PACKET_TRIANGULATION5:
                return childReader_ = new XMLTriangulationReader<5>(
                    resolver_, size, permIndex);
            case PACKET_TRIANGULATION6:
                return childReader_ = new XMLTriangulationReader<6>(
                    resolver_, size, permIndex);
            case PACKET_TRIANGULATION7:
                return childReader_ = new XMLTriangulationReader<7>(
                    resolver_, size, permIndex);
            case PACKET_TRIANGULATION8:
                return childReader_ = new XMLTriangulationReader<8>(
                    resolver_, size, permIndex);
            case PACKET_TRIANGULATION9:
                return childReader_ = new XMLTriangulationReader<9>(
                    resolver_, size, permIndex);
            case PACKET_TRIANGULATION10:
                return childReader_ = new XMLTriangulationReader<10>(
                    resolver_, size, permIndex);
            case PACKET_TRIANGULATION11:
                return childReader_ = new XMLTriangulationReader<11>(
                    resolver_, size, permIndex);
            case PACKET_TRIANGULATION12:
                return childReader_ = new XMLTriangulationReader<12>(
                    resolver_, size, permIndex);
            case PACKET_TRIANGULATION13:
                return childReader_ = new XMLTriangulationReader<13>(
                    resolver_, size, permIndex);
            case PACKET_TRIANGULATION14:
                return childReader_ = new XMLTriangulationReader<14>(
                    resolver_, size, permIndex);
            case PACKET_TRIANGULATION15:
                return childReader_ = new XMLTriangulationReader<15>(
                    resolver_, size, permIndex);
#endif /* ! REGINA_LOWDIMONLY */
            default:
                return childReader_ = new XMLPacketReader(resolver_);
        }
    }
}

void XMLPacketReader::endSubElement(const std::string& subTagName,
        XMLElementReader* subReader) {
    if (childReader_) {
        if (Packet* child = childReader_->packet()) {
            if (Packet* me = packet()) {
                if (! childLabel_.empty())
                    child->setLabel(childLabel_);
                if (! childID_.empty())
                    resolver_.storeID(childID_, child);
                if (! child->parent())
                    me->insertChildLast(child);
            } else
                delete child;
        }
        childLabel_.clear();
        childID_.clear();
        childReader_ = nullptr;
    } else if (subTagName != "tag") {
        // This sub-element was part of the packet's "real" content.
        endContentSubElement(subTagName, subReader);
    }
}

void XMLPacketReader::abort(XMLElementReader* /* subReader */) {
    if (Packet* me = packet())
        if (! me->parent())
            delete me;
}

} // namespace regina

