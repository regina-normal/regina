
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

#include "surfaces/sfcombination.h"
#include "file/nfile.h"

#define TYPE_AND 1
#define TYPE_OR 2

namespace regina {

bool NSurfaceFilterCombination::accept(const NNormalSurface& surface) const {
    if (usesAnd) {
        // Combine all child filters using AND.
        for (NPacket* child = getFirstTreeChild(); child;
                child = child->getNextTreeSibling())
            if (child->getPacketType() == NSurfaceFilter::packetType)
                if (! (dynamic_cast<NSurfaceFilter*>(child)->accept(surface)))
                    return false;
        return true;
    } else {
        // Combine all child filters using OR.
        for (NPacket* child = getFirstTreeChild(); child;
                child = child->getNextTreeSibling())
            if (child->getPacketType() == NSurfaceFilter::packetType)
                if (dynamic_cast<NSurfaceFilter*>(child)->accept(surface))
                    return true;
        return false;
    }
}

void NSurfaceFilterCombination::writeXMLFilterData(std::ostream& out) const {
    out << "    <op type=\"" << (usesAnd ? "and" : "or") << "\"/>\n";
}

void NSurfaceFilterCombination::writeFilter(NFile& out) const {
    if (usesAnd)
        out.writeInt(TYPE_AND);
    else
        out.writeInt(TYPE_OR);
}

NSurfaceFilter* NSurfaceFilterCombination::readFilter(NFile& in, NPacket*) {
    NSurfaceFilterCombination* ans = new NSurfaceFilterCombination();
    ans->usesAnd = (in.readInt() == TYPE_AND);
    return ans;
}

} // namespace regina

