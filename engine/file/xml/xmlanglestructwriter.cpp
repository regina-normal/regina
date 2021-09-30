
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

#include <sstream>
#include "regina-core.h"
#include "file/fileformat.h"
#include "file/xml/xmlwriter.h"
#include "angle/anglestructures.h"
#include "packet/packet-impl.h"

namespace regina {

template <>
bool XMLWriter<AngleStructures>::openPre() {
    const Packet* triPacket = triangulation_->inAnyPacket();

    if (format == REGINA_XML_GEN_2 && ! (triPacket && triPacket == parent())) {
        // The second-generation format required tri == parent(), and
        // Regina <= 6.0.1 cannot handle lists *without* this property at all.
        // Do not write this list (or its descendants) at all.
        return false;
    }

    if (triPacket) {
        // We know from addPacketRefs() that refs contains the triangulation.
        if (! refs.find(triPacket)->second) {
            // The triangulation has not yet been written to file.  Do it now.
            writeXMLAnon(out, format, refs, triPacket);
        }

        if (format_ == REGINA_XML_GEN_2) {
            out_ << "<packet type=\"Angle Structures\" typeid=\""
                << PACKET_ANGLESTRUCTURES << '\"';
        } else {
            out_ << "<angles "
                "tri=\"" << triPacket->internalID() << "\" "
                "tautonly=\"" << (tautOnly_ ? 'T' : 'F') << "\" "
                "algorithm=\"" << algorithm_.intValue() << '\"';
        }
    } else {
        // The triangulation is not a packet at all.
        // Write it anonymously now, with an ID that is guaranteed not to
        // match a packet ID.
        std::string id = triangulation_->anonID();

        out << "<anon>\n";
        XMLWriter<Triangulation<3>> writer(*triangulation_, out, format);
        writer.openPre();
        out << " id=\"" << id << "\"";
        writer.openPost();
        writer.writeContent();
        writer.close();
        out << "</anon>\n";

        if (format_ == REGINA_XML_GEN_2) {
            out_ << "<packet type=\"Angle Structures\" typeid=\""
                << PACKET_ANGLESTRUCTURES << '\"';
        } else {
            out_ << "<angles "
                "tri=\"" << id << "\" "
                "tautonly=\"" << (tautOnly_ ? 'T' : 'F') << "\" "
                "algorithm=\"" << algorithm_.intValue() << '\"';
        }
    }
}

template <>
void XMLWriter<AngleStructures>::writeContent() {
    using regina::xml::xmlValueTag;

    if (format_ == REGINA_XML_GEN_2) {
        // Write the enumeration parameters in a separate angleparams element.
        out << "  <angleparams "
            "tautonly=\"" << (data_.tautOnly_ ? 'T' : 'F') << "\" "
            "algorithm=\"" << data_.algorithm_.intValue() << "\"/>\n";
    }

    // Write the individual structures.
    for (const AngleStructure& a : data_.structures_)
        a.writeXMLData(out_);

    // Write the properties.
    if (doesSpanStrict_.has_value())
        out_ << "  " << xmlValueTag("spanstrict", *data_.doesSpanStrict_)
            << '\n';
    if (doesSpanTaut_.has_value())
        out_ << "  " << xmlValueTag("spantaut", *data_.doesSpanTaut_) << '\n';
}

template <>
void XMLWriter<AngleStructures>::close() {
    if (format_ == REGINA_XML_GEN_2)
        out_ << "</angles> <!-- Angle Structures -->\n";
    else
        out_ << "</angles>\n";
}

template void PacketOf<AngleStructures>::writeXMLPacketData(std::ostream&,
    FileFormat, bool, PacketRefs&) const;

} // namespace regina

