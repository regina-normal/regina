
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

#include <sstream>
#include "regina-core.h"
#include "file/fileformat.h"
#include "file/xml/xmlwriter.h"
#include "surface/normalsurfaces.h"
#include "triangulation/dim3.h"
#include "packet/packet-impl.h"

namespace regina {

template <>
void XMLWriter<NormalSurfaces>::openPre() {
    if (format_ == FileFormat::XmlGen2) {
        out_ << R"(<packet type="Normal Surface List" typeid=")"
            << static_cast<int>(PacketType::NormalSurfaces) << '"';
    } else {
        out_ << R"(<surfaces tri=")" << triID_
            << R"(" type=")" << data_.which_.intValue()
            << R"(" algorithm=")" << data_.algorithm_.intValue()
            << R"(" coords=")" << static_cast<int>(data_.coords_) << '"';
    }
}

template <>
void XMLWriter<NormalSurfaces>::writeContent() {
    using regina::xml::xmlValueTag;

    if (format_ == FileFormat::XmlGen2) {
        // Write the enumeration parameters.
        out_ << "  <params "
            "type=\"" << data_.which_.intValue() << "\" "
            "algorithm=\"" << data_.algorithm_.intValue() << "\" "
            "flavourid=\"" << static_cast<int>(data_.coords_) << "\"\n";
        out_ << "\tflavour=\""
            << regina::xml::xmlEncodeSpecialChars(NormalInfo::name(
                data_.coords_))
            << "\"/>\n";
    }

    // Write the individual surfaces.
    for (const auto& s : data_.surfaces_)
        s.writeXMLData(out_, format_, std::addressof(data_));
}

template <>
void XMLWriter<NormalSurfaces>::close() {
    if (format_ == FileFormat::XmlGen2)
        out_ << "</packet> <!-- Normal Surface List -->\n";
    else
        out_ << "</surfaces>\n";
}

template void PacketOf<NormalSurfaces>::addPacketRefs(PacketRefs&) const;
template void PacketOf<NormalSurfaces>::writeXMLPacketData(std::ostream&,
    FileFormat, bool, PacketRefs&) const;

} // namespace regina

