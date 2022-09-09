
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

#include <sstream>
#include "regina-core.h"
#include "file/fileformat.h"
#include "file/xml/xmlwriter.h"
#include "packet/packet-impl.h"
#include "snappea/snappeatriangulation.h"

namespace regina {

template <>
void XMLWriter<SnapPeaTriangulation>::openPre() {
    if (format_ == REGINA_XML_GEN_2) {
        out_ << R"(<packet type="SnapPea Triangulation" typeid=")"
            << PACKET_SNAPPEATRIANGULATION << '"';
    } else {
        out_ << "<snappeadata";
    }
}

template <>
void XMLWriter<SnapPeaTriangulation>::openPost() {
    out_ << '>';
    if (format_ == REGINA_XML_GEN_2)
        out_ << '\n';
}

template <>
void XMLWriter<SnapPeaTriangulation>::writeContent() {
    if (! data_.isNull()) {
        if (format_ == REGINA_XML_GEN_2)
            out_ << "  <snappea>";
        out_ << regina::xml::xmlEncodeSpecialChars(data_.snapPea());
        if (format_ == REGINA_XML_GEN_2)
            out_ << "</snappea>\n";
    }
}

template <>
void XMLWriter<SnapPeaTriangulation>::close() {
    if (format_ == REGINA_XML_GEN_2)
        out_ << "</packet> <!-- SnapPea Triangulation -->\n";
    else
        out_ << "</snappeadata>\n";
}

template void PacketOf<SnapPeaTriangulation>::addPacketRefs(PacketRefs&) const;
template void PacketOf<SnapPeaTriangulation>::writeXMLPacketData(
    std::ostream&, FileFormat, bool, PacketRefs&) const;

} // namespace regina

