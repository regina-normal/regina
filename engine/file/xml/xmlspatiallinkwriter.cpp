
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

#include <sstream>
#include "regina-core.h"
#include "file/fileformat.h"
#include "file/xml/xmlwriter.h"
#include "link/spatiallink.h"
#include "packet/packet-impl.h"

// Note: the format XmlGen2 does not support SpatialLink at all.
// We will simply output the third-generation format, and a second-generation
// reader will happily ignore it.

namespace regina {

template <>
void XMLWriter<SpatialLink>::openPre() {
    out_ << "<spatiallink";
    if (data_.hasRadius()) {
        // Temporarily enable hexfloat format, which should be lossless.
        auto oldFlags = out_.flags();
        out_ << std::hexfloat;
        out_ << R"( radius=")" << data_.radius() << '"';
        out_.flags(oldFlags);
    }
}

template <>
void XMLWriter<SpatialLink>::writeContent() {
    // Temporarily enable hexfloat format, which should be lossless.
    auto oldFlags = out_.flags();
    out_ << std::hexfloat;
    for (const auto& c : data_.components_) {
        out_ << "  <component>\n";
        for (const auto& node : c) {
            out_ << "  <node> " << node.x << ' ' << node.y << ' ' << node.z
                << " </node>\n";
        }
        out_ << "  </component>\n";
    }
    out_.flags(oldFlags);
}

template <>
void XMLWriter<SpatialLink>::close() {
    out_ << "</spatiallink>\n";
}

template void PacketOf<SpatialLink>::addPacketRefs(PacketRefs&) const;
template void PacketOf<SpatialLink>::writeXMLPacketData(std::ostream&,
    FileFormat, bool, PacketRefs&) const;

} // namespace regina

