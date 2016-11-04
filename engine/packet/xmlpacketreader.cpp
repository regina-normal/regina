
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include "packet/packet.h"
#include "packet/xmlpacketreader.h"
#include "packet/xmltreeresolver.h"
#include "packet/packetregistry.h"
#include "triangulation/xmltrireader.h"
#include "utilities/stringutils.h"

namespace regina {

namespace {
    struct XMLReaderFunction : public Returns<XMLElementReader*> {
        template <typename PacketInfo>
        inline XMLElementReader* operator() (Packet* me,
                XMLTreeResolver& resolver) {
            return PacketInfo::Class::xmlReader(me, resolver);
        }
    };
}

XMLElementReader* XMLPacketReader::startSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict& subTagProps) {
    if (subTagName == "packet") {
        Packet* me = packet();
        if (! me)
            return new XMLPacketReader(resolver_);

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
            return new XMLPacketReader(resolver_);

        long typeID;
        if (! valueOf((*it).second, typeID))
            return new XMLPacketReader(resolver_);
        if (typeID <= 0)
            return new XMLPacketReader(resolver_);

        XMLElementReader* ans = forPacket(static_cast<PacketType>(typeID),
            XMLReaderFunction(), 0, me, resolver_);
        if (ans)
            return ans;
        else
            return new XMLPacketReader(resolver_);
    } else if (subTagName == "tag") {
        if (Packet* me = packet()) {
            std::string packetTag = subTagProps.lookup("name");
            if (! packetTag.empty())
                me->addTag(packetTag);
        }
        return new XMLElementReader();
    } else
        return startContentSubElement(subTagName, subTagProps);
}

void XMLPacketReader::endSubElement(const std::string& subTagName,
        XMLElementReader* subReader) {
    if (subTagName == "packet") {
        Packet* child =
            dynamic_cast<XMLPacketReader*>(subReader)->packet();
        if (child) {
            Packet* me = packet();
            if (me) {
                child->setLabel(childLabel);
                if (! childID.empty())
                    resolver_.storeID(childID, child);
                if (! child->parent())
                    me->insertChildLast(child);
            } else
                delete child;
        }
    } else if (subTagName == "tag")
        return;
    else
        endContentSubElement(subTagName, subReader);
}

void XMLPacketReader::abort(XMLElementReader* /* subReader */) {
    Packet* me = packet();
    if (me)
        if (! me->parent())
            delete me;
}

} // namespace regina

