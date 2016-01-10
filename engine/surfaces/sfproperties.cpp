
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

#include "surfaces/sfproperties.h"
#include "surfaces/nnormalsurface.h"
#include "utilities/xmlutils.h"

#define PROPSF_EULER 1001
#define PROPSF_ORIENT 1002
#define PROPSF_COMPACT 1003
#define PROPSF_REALBDRY 1004

namespace regina {

NLargeInteger NSurfaceFilterProperties::getEC(size_t index) const {
    std::set<NLargeInteger>::const_iterator it = eulerChar.begin();
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
        if (! orientability.contains(surface.isOrientable()))
            return false;

        if (eulerChar.size() > 0)
            if (! eulerChar.count(surface.getEulerChar()))
                return false;
    }

    // All tests passed.
    return true;
}

void NSurfaceFilterProperties::writeTextLong(std::ostream& o) const {
    o << "Filter normal surfaces with restrictions:\n";

    if (eulerChar.size() > 0) {
        o << "    Euler characteristic:";
        std::set<NLargeInteger>::const_reverse_iterator it;
        for (it = eulerChar.rbegin();
                it != eulerChar.rend(); it++)
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

    if (eulerChar.size() > 0) {
        out << "    <euler> ";
        for (std::set<NLargeInteger>::const_iterator it =
                eulerChar.begin(); it != eulerChar.end();
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

} // namespace regina

