
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

/*! \file packet/packet-impl.h
 *  \brief Contains some implementation details for packet wrapper classes.
 *
 *  This file is \e not included from packet.h.  The routines it contains are
 *  explicitly instantiated in Regina's calculation engine for all necessary
 *  packet types, and so end users should not need to include this file
 *  themselves.
 */

#ifndef __REGINA_PACKET_IMPL_H
#ifndef __DOXYGEN
#define __REGINA_PACKET_IMPL_H
#endif

#include "packet/packet.h"
#include "utilities/base64.h"
#include "utilities/xmlutils.h"

namespace regina {

template <typename Held>
void PacketOf<Held>::writeXMLPacketData(std::ostream& out, FileFormat format,
        bool anon, PacketRefs& refs) const {
    XMLWriter<Held> writer(*this, out, format);
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
    writeXMLTreeData(out, format, anon, refs);
    writer.close();
}

template <typename Held>
std::string PacketData<Held>::anonID() const {
    char ptrAsBytes[sizeof(PacketData<Held>*)];
    *(reinterpret_cast<const PacketData<Held>**>(&ptrAsBytes)) = this;

    char* id = 0;
    base64Encode(ptrAsBytes, sizeof(Packet*), &id);

    std::string ans;
    ans += regina::base64Spare[0];
    ans += id;
    delete[] id;
    return ans;
}

} // namespace regina

#endif
