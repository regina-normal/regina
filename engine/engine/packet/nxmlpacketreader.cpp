
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "packet/npacket.h"
#include "packet/nxmlpacketreader.h"
#include "packet/packetregistry.h"
#include "utilities/stringutils.h"

namespace regina {

#define __PACKET_REGISTRY_BODY

#define REGISTER_PACKET(class, type, name) \
    if (typeID == class::packetType) \
        return class::getXMLReader(me);

NXMLElementReader* NXMLPacketReader::startSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict& subTagProps) {
    if (subTagName == "packet") {
        NPacket* me = getPacket();
        if (! me)
            return new NXMLPacketReader();

        regina::xml::XMLPropertyDict::const_iterator it =
            subTagProps.find("label");
        if (it == subTagProps.end())
            childLabel = "";
        else
            childLabel = (*it).second;

        it = subTagProps.find("typeid");
        if (it == subTagProps.end())
            return new NXMLPacketReader();

        long typeID;
        if (! valueOf((*it).second, typeID))
            return new NXMLPacketReader();
        if (typeID <= 0)
            return new NXMLPacketReader();

        // Pull in cases from the packet registry.
        #include "packet/packetregistry.h"
        return new NXMLPacketReader();
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

void NXMLPacketReader::abort(NXMLElementReader *subReader) {
    NPacket* me = getPacket();
    if (me)
        if (! me->getTreeParent())
            delete me;
}

} // namespace regina

