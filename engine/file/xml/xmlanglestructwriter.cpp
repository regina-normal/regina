
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
#include "angle/anglestructures.h"
#include "triangulation/dim3.h"
#include "packet/packet-impl.h"

namespace regina {

template <>
void XMLWriter<AngleStructures>::openPre() {
    if (format_ == FileFormat::XmlGen2) {
        out_ << R"(<packet type="Angle Structure List" typeid=")"
            << static_cast<int>(PacketType::AngleStructures) << '"';
    } else {
        out_ << R"(<angles tri=")" << triID_
            << R"(" tautonly=")" << (data_.tautOnly_ ? 'T' : 'F')
            << R"(" algorithm=")" << data_.algorithm_.intValue() << '"';
    }
}

template <>
void XMLWriter<AngleStructures>::writeContent() {
    using regina::xml::xmlValueTag;

    if (format_ == FileFormat::XmlGen2) {
        // Write the enumeration parameters in a separate angleparams element.
        out_ << "  <angleparams "
            "tautonly=\"" << (data_.tautOnly_ ? 'T' : 'F') << "\" "
            "algorithm=\"" << data_.algorithm_.intValue() << "\"/>\n";
    }

    // Write the individual structures.
    for (const AngleStructure& a : data_.structures_)
        a.writeXMLData(out_);

    // Write the properties.
    if (data_.doesSpanStrict_.has_value())
        out_ << "  " << xmlValueTag("spanstrict", *data_.doesSpanStrict_)
            << '\n';
    if (data_.doesSpanTaut_.has_value())
        out_ << "  " << xmlValueTag("spantaut", *data_.doesSpanTaut_) << '\n';
}

template <>
void XMLWriter<AngleStructures>::close() {
    if (format_ == FileFormat::XmlGen2)
        out_ << "</packet> <!-- Angle Structure List -->\n";
    else
        out_ << "</angles>\n";
}

template void PacketOf<AngleStructures>::addPacketRefs(PacketRefs&) const;
template void PacketOf<AngleStructures>::writeXMLPacketData(std::ostream&,
    FileFormat, bool, PacketRefs&) const;

} // namespace regina

