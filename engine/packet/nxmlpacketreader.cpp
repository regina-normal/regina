
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

#include "packet/npacket.h"
#include "packet/nxmlpacketreader.h"
#include "packet/nxmltreeresolver.h"
#include "packet/packetregistry.h"
#include "utilities/stringutils.h"

namespace regina {

namespace {
    struct XMLReaderFunction : public Returns<NXMLElementReader*> {
        template <typename Packet>
        inline NXMLElementReader* operator() (Packet,
                NPacket* me, NXMLTreeResolver& resolver) {
            return Packet::Class::getXMLReader(me, resolver);
        }
    };
}

NXMLElementReader* NXMLPacketReader::startSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict& subTagProps) {
    if (subTagName == "packet") {
        NPacket* me = getPacket();
        if (! me)
            return new NXMLPacketReader(resolver_);

        regina::xml::XMLPropertyDict::const_iterator it =
            subTagProps.find("label");
        if (it == subTagProps.end())
            childLabel = "";
        else
            childLabel = it->second;

        it = subTagProps.find("id");
        if (it == subTagProps.end())
            childID = "";
        else
            childID = it->second;

        it = subTagProps.find("typeid");
        if (it == subTagProps.end())
            return new NXMLPacketReader(resolver_);

        long typeID;
        if (! valueOf((*it).second, typeID))
            return new NXMLPacketReader(resolver_);
        if (typeID <= 0)
            return new NXMLPacketReader(resolver_);

        NXMLElementReader* ans = forPacket(static_cast<PacketType>(typeID),
            XMLReaderFunction(), 0, me, std::ref(resolver_));
        if (ans)
            return ans;
        else
            return new NXMLPacketReader(resolver_);
    } else if (subTagName == "tag") {
        if (NPacket* me = getPacket()) {
            std::string packetTag = subTagProps.lookup("name");
            if (! packetTag.empty())
                me->addTag(packetTag);
        }
        return new NXMLElementReader();
    } else
        return startContentSubElement(subTagName, subTagProps);
}

void NXMLPacketReader::endSubElement(const std::string& subTagName,
        NXMLElementReader* subReader) {
    if (subTagName == "packet") {
        NPacket* child =
            dynamic_cast<NXMLPacketReader*>(subReader)->getPacket();
        if (child) {
            NPacket* me = getPacket();
            if (me) {
                child->setPacketLabel(childLabel);
                if (! childID.empty())
                    resolver_.storeID(childID, child);
                if (! child->getTreeParent())
                    me->insertChildLast(child);
            } else
                delete child;
        }
    } else if (subTagName == "tag")
        return;
    else
        endContentSubElement(subTagName, subReader);
}

void NXMLPacketReader::abort(NXMLElementReader* /* subReader */) {
    NPacket* me = getPacket();
    if (me)
        if (! me->getTreeParent())
            delete me;
}

} // namespace regina

