
/**************************************************************************
 *                                                                        *
 *  Regina - A normal surface theory calculator                           *
 *  Computational engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "sfproperties.h"
    #include "nnormalsurface.h"
    #include "nfile.h"
#else
    #include "engine/surfaces/sfproperties.h"
    #include "engine/surfaces/nnormalsurface.h"
    #include "engine/file/nfile.h"
#endif

#define PROPID_EULER 1001
#define PROPID_ORIENT 1002
#define PROPID_COMPACT 1003
#define PROPID_REALBDRY 1004

NSurfaceFilterProperties::NSurfaceFilterProperties(
        const NSurfaceFilterProperties& cloneMe) :
        orientability(cloneMe.orientability),
        compactness(cloneMe.compactness),
        realBoundary(cloneMe.realBoundary) {
    for (NDynamicArrayIterator<NLargeInteger> it(cloneMe.eulerCharacteristic);
            ! it.done(); it++)
        eulerCharacteristic.addLast(*it);
}

void NSurfaceFilterProperties::initialiseAllProperties() {
    NSurfaceFilter::initialiseAllProperties();

    eulerCharacteristic.flush();
    orientability.fill();
    compactness.fill();
    realBoundary.fill();
}

bool NSurfaceFilterProperties::accept(NNormalSurface& surface) const {
    if (! realBoundary.contains(surface.hasRealBoundary()))
        return false;
    if (! compactness.contains(surface.isCompact()))
        return false;
    if (! orientability.contains(surface.isOrientable()))
        return false;

    if (eulerCharacteristic.size() > 0)
        if (eulerCharacteristic.position(surface.getEulerCharacteristic()) < 0)
            return false;
    return true;
}

void NSurfaceFilterProperties::writeTextLong(ostream& o) const {
    o << "Filter normal surfaces with restrictions:\n";

    if (eulerCharacteristic.size() > 0) {
        o << "    Euler characteristic:";
        NDynamicArrayIterator<NLargeInteger> it;
        for (it.initEnd(eulerCharacteristic); ! it.done(); it--)
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

void NSurfaceFilterProperties::writeProperties(NFile& out) const {
    streampos bookmark(0);

    if (eulerCharacteristic.size() > 0) {
        bookmark = writePropertyHeader(out, PROPID_EULER);
        out.writeULong(eulerCharacteristic.size());
        for (NDynamicArrayIterator<NLargeInteger> it(eulerCharacteristic);
                ! it.done(); it++)
            out.writeLarge(*it);
        writePropertyFooter(out, bookmark);
    }

    if (orientability != NBoolSet::sBoth) {
        bookmark = writePropertyHeader(out, PROPID_ORIENT);
        out.writeBoolSet(orientability);
        writePropertyFooter(out, bookmark);
    }

    if (compactness != NBoolSet::sBoth) {
        bookmark = writePropertyHeader(out, PROPID_COMPACT);
        out.writeBoolSet(compactness);
        writePropertyFooter(out, bookmark);
    }

    if (realBoundary != NBoolSet::sBoth) {
        bookmark = writePropertyHeader(out, PROPID_REALBDRY);
        out.writeBoolSet(realBoundary);
        writePropertyFooter(out, bookmark);
    }
}

NSurfaceFilter* NSurfaceFilterProperties::readFilter(NFile& in, NPacket*) {
    return new NSurfaceFilterProperties();
}

void NSurfaceFilterProperties::readIndividualProperty(NFile& in,
        unsigned propType) {
    NSurfaceFilter::readIndividualProperty(in, propType);

    switch (propType) {
        case PROPID_EULER:
            eulerCharacteristic.flush();
            for (unsigned long size = in.readULong(); size != 0; size--)
                eulerCharacteristic.addLast(in.readLarge());
            break;
        case PROPID_ORIENT:
            orientability = in.readBoolSet(); break;
        case PROPID_COMPACT:
            compactness = in.readBoolSet(); break;
        case PROPID_REALBDRY:
            realBoundary = in.readBoolSet(); break;
    }
}

