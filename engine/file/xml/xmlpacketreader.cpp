
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

XMLElementReader* XMLPacketReader::startSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict& subTagProps) {
    auto it = subTagProps.find("label");
    if (it != subTagProps.end())
        childLabel_ = it->second;

    it = subTagProps.find("id");
    if (it != subTagProps.end())
        childID_ = it->second;

    if (subTagName == "packet") {
        auto it = subTagProps.find("typeid");
        if (it == subTagProps.end())
            return new XMLPacketReader(resolver_);

        long typeID;
        if (! valueOf((*it).second, typeID))
            return new XMLPacketReader(resolver_);

        // Run through all the packet types that the file format supports.
        switch (static_cast<PacketType>(typeID)) {
            case PACKET_CONTAINER:
                return new XMLContainerReader(resolver_);
            case PACKET_TEXT:
                return new XMLTextReader(resolver_);
            case PACKET_TRIANGULATION3:
                return new XMLTriangulationReader<3>(resolver_);
            case PACKET_NORMALSURFACES:
                return new XMLNormalSurfacesReader(
                    dynamic_cast<Triangulation<3>*>(packet()), resolver_);
            case PACKET_SCRIPT:
                return new XMLScriptReader(resolver_);
            case PACKET_SURFACEFILTER:
                return new XMLFilterPacketReader(resolver_);
            case PACKET_ANGLESTRUCTURES:
                return new XMLAngleStructuresReader(
                    dynamic_cast<Triangulation<3>*>(packet()), resolver_);
            case PACKET_PDF:
                return new XMLPDFReader(resolver_);
            case PACKET_TRIANGULATION2:
                return new XMLTriangulationReader<2>(resolver_);
            case PACKET_TRIANGULATION4:
                return new XMLTriangulationReader<4>(resolver_);
            case PACKET_NORMALHYPERSURFACES:
                return new XMLNormalHypersurfacesReader(
                    dynamic_cast<Triangulation<4>*>(packet()), resolver_);
            case PACKET_SNAPPEATRIANGULATION:
                return new XMLSnapPeaReader(resolver_);
            case PACKET_LINK:
                return new XMLLinkReader(resolver_);
#ifndef REGINA_LOWDIMONLY
            case PACKET_TRIANGULATION5:
                return new XMLTriangulationReader<5>(resolver_);
            case PACKET_TRIANGULATION6:
                return new XMLTriangulationReader<6>(resolver_);
            case PACKET_TRIANGULATION7:
                return new XMLTriangulationReader<7>(resolver_);
            case PACKET_TRIANGULATION8:
                return new XMLTriangulationReader<8>(resolver_);
            case PACKET_TRIANGULATION9:
                return new XMLTriangulationReader<9>(resolver_);
            case PACKET_TRIANGULATION10:
                return new XMLTriangulationReader<10>(resolver_);
            case PACKET_TRIANGULATION11:
                return new XMLTriangulationReader<11>(resolver_);
            case PACKET_TRIANGULATION12:
                return new XMLTriangulationReader<12>(resolver_);
            case PACKET_TRIANGULATION13:
                return new XMLTriangulationReader<13>(resolver_);
            case PACKET_TRIANGULATION14:
                return new XMLTriangulationReader<14>(resolver_);
            case PACKET_TRIANGULATION15:
                return new XMLTriangulationReader<15>(resolver_);
#endif /* ! REGINA_LOWDIMONLY */
            default:
                return new XMLPacketReader(resolver_);
        }
    } else if (subTagName == "tag") {
        if (Packet* me = packet()) {
            std::string packetTag = subTagProps.lookup("name");
            if (! packetTag.empty())
                me->addTag(packetTag);
        }
        return new XMLElementReader();
    } else
        return startContentSubElement(subTagName, subTagProps);
}

void XMLPacketReader::endSubElement(const std::string& subTagName,
        XMLElementReader* subReader) {
    auto childReader = dynamic_cast<XMLPacketReader*>(subReader);
    if (childReader) {
        if (Packet* child = childReader->packet()) {
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
    } else if (subTagName != "tag") {
        endContentSubElement(subTagName, subReader);
    }

    childLabel_.clear();
    childID_.clear();
}

void XMLPacketReader::abort(XMLElementReader* /* subReader */) {
    if (Packet* me = packet())
        if (! me->parent())
            delete me;
}

} // namespace regina

