
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2004, Ben Burton                                   *
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

#include "surfaces/nxmlfilterreader.h"
#include "surfaces/filterregistry.h"
#include "utilities/stringutils.h"

#define __FILTER_REGISTRY_BODY

namespace regina {

#define REGISTER_FILTER(id, class, n) \
    if (type == id) \
        return class::getXMLFilterReader(parent); \

NXMLElementReader* NXMLFilterPacketReader::startContentSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict& props) {
    if (! filter)
        if (subTagName == "filter") {
            int type;
            if (valueOf(props.lookup("typeid"), type)) {
                // Import cases from the filter registry.
                #include "surfaces/filterregistry.h"
                return new NXMLFilterReader();
            }
        }
    return new NXMLElementReader();
}

void NXMLFilterPacketReader::endContentSubElement(
        const std::string& subTagName,
        NXMLElementReader* subReader) {
    if (! filter)
        if (subTagName == "filter")
            filter = dynamic_cast<NXMLFilterReader*>(subReader)->getFilter();
}

NXMLPacketReader* NSurfaceFilter::getXMLReader(NPacket* parent) {
    return new NXMLFilterPacketReader(parent);
}

// Tidy up.
#undef REGISTER_FILTER
#undef __FILTER_REGISTRY_BODY

} // namespace regina

