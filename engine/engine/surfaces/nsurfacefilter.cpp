
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

#include "surfaces/nsurfacefilter.h"
#include "surfaces/nnormalsurfacelist.h"
#include "surfaces/filterregistry.h"
#include "file/nfile.h"
#include "utilities/xmlutils.h"

namespace regina {

#define __FILTER_REGISTRY_BODY

NSurfaceFilter* NSurfaceFilter::readFilter(NFile&, NPacket*) {
    return new NSurfaceFilter();
}

void NSurfaceFilter::writePacket(NFile& out) const {
    out.writeInt(getFilterID());
    writeFilter(out);
    writeProperties(out);
    writeAllPropertiesFooter(out);
}

#define REGISTER_FILTER(id, c, name) \
    case id: out << regina::xml::xmlEncodeSpecialChars(name); break;

void NSurfaceFilter::writeXMLPacketData(std::ostream& out) const {
    int id = getFilterID();

    out << "  <filter type=\"";
    switch(id) {
        // Import cases from the filter registry.
        #include "surfaces/filterregistry.h"
        default: out << "Unknown"; break;
    }
    out << "\" typeid=\"" << id << "\">\n";

    writeXMLFilterData(out);

    out << "  </filter>\n";
}

void NSurfaceFilter::readIndividualProperty(NFile& in, unsigned propType) {
}

void NSurfaceFilter::writeProperties(NFile& out) const {
}

#undef REGISTER_FILTER
#define REGISTER_FILTER(id, class, n) \
    case id: ans = class::readFilter(in, parent); break;

NSurfaceFilter* NSurfaceFilter::readPacket(NFile& in, NPacket* parent) {
    NSurfaceFilter* ans = 0;
    int id = in.readInt();
    switch (id) {
        // Import cases from the filter registry.
        #include "surfaces/filterregistry.h"
        default: ans = new NSurfaceFilter();
    }

    ans->readProperties(in);
    return ans;
}

#undef REGISTER_FILTER
#define REGISTER_FILTER(id, class, n) \
    case id: return new class((const class&)(*this));

NPacket* NSurfaceFilter::internalClonePacket(NPacket* parent) const {
    switch (getFilterID()) {
        // Import cases from the filter registry.
        #include "surfaces/filterregistry.h"
        default: return new NSurfaceFilter();
    }
}

} // namespace regina

