
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
#include "link/link.h"
#include "packet/packet-impl.h"

namespace regina {

template <>
void XMLWriter<Link>::openPre() {
    if (format_ == FileFormat::XmlGen2) {
        out_ << R"(<packet type="Link" typeid=")"
            << static_cast<int>(PacketType::Link) << '"';
    } else {
        out_ << "<link";
    }
}

template <>
void XMLWriter<Link>::writeContent() {
    out_ << "  <crossings size=\"" << data_.crossings_.size()
        << "\">\n ";
    for (const Crossing* c : data_.crossings_)
        out_ << ' ' << (c->sign() == 1 ? '+' : '-');
    out_ << "\n  </crossings>\n";
    out_ << "  <connections>\n";
    for (const Crossing* c : data_.crossings_)
        out_ << "  " << c->next(1) << ' ' << c->next(0) << '\n';
    out_ << "  </connections>\n";
    out_ << "  <components size=\"" << data_.components_.size()
        << "\">\n ";
    for (const StrandRef& s : data_.components_)
        out_ << ' ' << s;
    out_ << "\n  </components>\n";
}

template <>
void XMLWriter<Link>::close() {
    if (format_ == FileFormat::XmlGen2)
        out_ << "</packet> <!-- Link -->\n";
    else
        out_ << "</link>\n";
}

template void PacketOf<Link>::addPacketRefs(PacketRefs&) const;
template void PacketOf<Link>::writeXMLPacketData(std::ostream&, FileFormat,
    bool, PacketRefs&) const;

} // namespace regina

