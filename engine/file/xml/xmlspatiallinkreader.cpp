
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

#include "link/spatiallink.h"
#include "file/xml/xmlspatiallinkreader.h"
#include <sstream>

namespace regina {

XMLSpatialLinkReader::XMLSpatialLinkReader(XMLTreeResolver& res,
        std::shared_ptr<Packet> parent, bool anon, std::string label,
        std::string id) :
        XMLPacketReader(res, std::move(parent), anon, std::move(label),
            std::move(id)),
        link_(make_packet<SpatialLink>()) {
}

std::shared_ptr<Packet> XMLSpatialLinkReader::packetToCommit() {
    return link_;
}

XMLElementReader* XMLSpatialLinkReader::startContentSubElement(
        const std::string& subTagName, const regina::xml::XMLPropertyDict&) {
    if (! link_)
        return new XMLElementReader();

    if (subTagName == "component") {
        link_->components_.emplace_back();
        return new XMLSpatialLinkComponentReader(std::addressof(
            link_->components_.back()));
    }

    return new XMLElementReader();
}

void XMLSpatialLinkReader::endContentSubElement(const std::string& subTagName,
        XMLElementReader* reader) {
    if (! link_)
        return;

    if (subTagName == "component") {
        if (static_cast<XMLSpatialLinkComponentReader*>(reader)->broken()) {
            link_.reset();
            return;
        }

        // Some basic sanity checking: to be embedded, each component must
        // have at least three nodes.
        if (link_->components_.back().size() < 3) {
            link_.reset();
            return;
        }
    }
}

XMLElementReader* XMLSpatialLinkComponentReader::startSubElement(
        const std::string& subTagName, const regina::xml::XMLPropertyDict&) {
    if (! component_)
        return new XMLElementReader();

    if (subTagName == "node") {
        component_->emplace_back();
        return new XMLSpatialLinkNodeReader(std::addressof(component_->back()));
    }

    return new XMLElementReader();
}

void XMLSpatialLinkComponentReader::endSubElement(
        const std::string& subTagName, XMLElementReader* reader) {
    if (! component_)
        return;

    if (subTagName == "node")
        if (static_cast<XMLSpatialLinkNodeReader*>(reader)->broken())
            component_ = nullptr;
}

void XMLSpatialLinkNodeReader::initialChars(const std::string& chars) {
    if (! node_)
        return;

    std::istringstream in(chars);

    in >> node_->x >> node_->y >> node_->z;
    if (! in)
        node_ = nullptr;
}

} // namespace regina

