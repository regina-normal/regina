
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "surfaces/sfproperties.h"
#include "surfaces/nnormalsurface.h"
#include "file/nfile.h"
#include "utilities/xmlutils.h"

#define PROPSF_EULER 1001
#define PROPSF_ORIENT 1002
#define PROPSF_COMPACT 1003
#define PROPSF_REALBDRY 1004

namespace regina {

NLargeInteger NSurfaceFilterProperties::getEC(unsigned long index) const {
    std::set<NLargeInteger>::const_iterator it = eulerCharacteristic.begin();
    advance(it, index);
    return *it;
}

bool NSurfaceFilterProperties::accept(const NNormalSurface& surface) const {
    if (! realBoundary.contains(surface.hasRealBoundary()))
        return false;
    if (! compactness.contains(surface.isCompact()))
        return false;

    // Some properties may only be calculated for compact surfaces.
    if (surface.isCompact()) {
        NTriBool orientable = surface.isOrientable();
        if (orientable.isKnown())
            if (! orientability.contains(orientable.isTrue()))
                return false;

        if (eulerCharacteristic.size() > 0)
            if (! eulerCharacteristic.count(surface.getEulerCharacteristic()))
                return false;
    }

    // All tests passed.
    return true;
}

void NSurfaceFilterProperties::writeTextLong(std::ostream& o) const {
    o << "Filter normal surfaces with restrictions:\n";

    if (eulerCharacteristic.size() > 0) {
        o << "    Euler characteristic:";
        std::set<NLargeInteger>::const_reverse_iterator it;
        for (it = eulerCharacteristic.rbegin();
                it != eulerCharacteristic.rend(); it++)
            o << ' ' << *it;
        o << '\n';
    }
    if (orientability != NBoolSet::sBoth)
        o << "    Orientability: " << orientability << '\n';
    if (compactness != NBoolSet::sBoth)
        o << "    Compactness: " << compactness << '\n';
    if (realBoundary != NBoolSet::sBoth)
        o << "    Has real boundary: " << realBoundary << '\n';
}

void NSurfaceFilterProperties::writeXMLFilterData(std::ostream& out) const {
    using regina::xml::xmlValueTag;

    if (eulerCharacteristic.size() > 0) {
        out << "    <euler> ";
        for (std::set<NLargeInteger>::const_iterator it =
                eulerCharacteristic.begin(); it != eulerCharacteristic.end();
                it++)
            out << (*it) << ' ';
        out << "</euler>\n";
    }

    if (orientability != NBoolSet::sBoth)
        out << "    " << xmlValueTag("orbl", orientability) << '\n';
    if (compactness != NBoolSet::sBoth)
        out << "    " << xmlValueTag("compact", compactness) << '\n';
    if (realBoundary != NBoolSet::sBoth)
        out << "    " << xmlValueTag("realbdry", realBoundary) << '\n';
}

void NSurfaceFilterProperties::writeProperties(NFile& out) const {
    std::streampos bookmark(0);

    if (eulerCharacteristic.size() > 0) {
        bookmark = out.writePropertyHeader(PROPSF_EULER);
        out.writeULong(eulerCharacteristic.size());
        for (std::set<NLargeInteger>::const_iterator it =
                eulerCharacteristic.begin(); it != eulerCharacteristic.end();
                it++)
            out.writeLarge(*it);
        out.writePropertyFooter(bookmark);
    }

    if (orientability != NBoolSet::sBoth) {
        bookmark = out.writePropertyHeader(PROPSF_ORIENT);
        out.writeBoolSet(orientability);
        out.writePropertyFooter(bookmark);
    }

    if (compactness != NBoolSet::sBoth) {
        bookmark = out.writePropertyHeader(PROPSF_COMPACT);
        out.writeBoolSet(compactness);
        out.writePropertyFooter(bookmark);
    }

    if (realBoundary != NBoolSet::sBoth) {
        bookmark = out.writePropertyHeader(PROPSF_REALBDRY);
        out.writeBoolSet(realBoundary);
        out.writePropertyFooter(bookmark);
    }
}

NSurfaceFilter* NSurfaceFilterProperties::readFilter(NFile&, NPacket*) {
    return new NSurfaceFilterProperties();
}

void NSurfaceFilterProperties::readIndividualProperty(NFile& in,
        unsigned propType) {
    NSurfaceFilter::readIndividualProperty(in, propType);

    switch (propType) {
        case PROPSF_EULER:
            eulerCharacteristic.clear();
            for (unsigned long size = in.readULong(); size != 0; size--)
                eulerCharacteristic.insert(in.readLarge());
            break;
        case PROPSF_ORIENT:
            orientability = in.readBoolSet(); break;
        case PROPSF_COMPACT:
            compactness = in.readBoolSet(); break;
        case PROPSF_REALBDRY:
            realBoundary = in.readBoolSet(); break;
    }
}

} // namespace regina

