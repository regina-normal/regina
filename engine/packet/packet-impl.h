
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

/*! \file packet/packet-impl.h
 *  \brief Contains some implementation details for packet wrapper classes.
 *
 *  This file is _not_ included from packet.h.  The routines it contains are
 *  explicitly instantiated in Regina's calculation engine for all necessary
 *  packet types, and so end users should not need to include this file
 *  themselves.
 */

#ifndef __REGINA_PACKET_IMPL_H
#ifndef __DOXYGEN
#define __REGINA_PACKET_IMPL_H
#endif

#include "packet/packet.h"
#include "utilities/xmlutils.h"

namespace regina {

template <typename Held>
void PacketOf<Held>::writeXMLPacketData(std::ostream& out, FileFormat format,
        bool anon, PacketRefs& refs) const {
    XMLWriter<Held> writer(*this, out, format);

    if constexpr (XMLWriter<Held>::requiresTriangulation) {
        // This is one of Regina's surface/structure list types, that for
        // the second-generation format required the triangulation to be
        // the parent packet, and for the third-generation format requires
        // the triangulation to have been written to file earlier than the list.
        std::shared_ptr<const Packet> triPacket;
        if constexpr (XMLWriter<Held>::dimension == 3)
            triPacket = this->triangulation().inAnyPacket();
        else
            triPacket = this->triangulation().packet();

        if (format == REGINA_XML_GEN_2)
            if (! (triPacket && triPacket == parent())) {
                // The second-generation format required tri == parent(), and
                // Regina <= 6.0.1 cannot handle lists *without* this property.
                // Do not write this list (or its descendants) at all.
                return;
            }

        if (triPacket) {
            // We know from addPacketRefs() that refs contains the
            // triangulation.
            if (! refs.find(triPacket.get())->second) {
                // The triangulation has not yet been written to file.
                // Do it now.
                writeXMLAnon(out, format, refs, *triPacket);
            }

            writer.wroteTriangulationID(triPacket->internalID());
        } else {
            // The triangulation is not a packet at all.
            // Write it anonymously now, with an ID that is guaranteed not to
            // match a packet ID.
            std::string triID = this->triangulation().anonID();

            out << "<anon>\n";
            XMLWriter<Triangulation<XMLWriter<Held>::dimension>> triWriter(
                this->triangulation(), out, format);
            triWriter.openPre();
            out << " id=\"" << triID << "\"";
            triWriter.openPost();
            triWriter.writeContent();
            triWriter.close();
            out << "</anon>\n";

            writer.wroteTriangulationID(std::move(triID));
        }
    }

    writer.openPre();
    out << " label=\"" << regina::xml::xmlEncodeSpecialChars(label())
        << "\"";

    auto pos = refs.find(this);
    if (pos != refs.end()) {
        // Someone has asked for this packet to store its ID.
        out << " id=\"" << internalID() << "\"";
        pos->second = true; // Indicate this packet is now being written↪.
    } else if (anon) {
        // Nobody *asked* for this packet to be referred to, but it is
        // still being written as anonymous block.  It's not clear how
        // such a situation could arise in practice, but regardless,
        // we should note that the packet has been "written ahead".
        out << " id=\"" << internalID() << "\"";
        refs.insert({ this, true });
    }

    writer.openPost();
    writer.writeContent();
    if (! anon)
        writeXMLTreeData(out, format, refs);
    writer.close();
}

template <typename Held>
void PacketOf<Held>::addPacketRefs(PacketRefs& refs) const {
    if constexpr (XMLWriter<Held>::requiresTriangulation) {
        if constexpr (XMLWriter<Held>::dimension == 3) {
            if (auto p = Held::triangulation().inAnyPacket())
                refs.insert({ p.get(), false });
        } else {
            if (auto p = Held::triangulation().packet())
                refs.insert({ p.get(), false });
        }
    }
}

} // namespace regina

#endif
