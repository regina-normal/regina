
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

#include "manifold/nsnappeacensusmfd.h"

namespace regina {

const char NSnapPeaCensusManifold::SEC_5 = 'm';
const char NSnapPeaCensusManifold::SEC_6_OR = 's';
const char NSnapPeaCensusManifold::SEC_6_NOR = 'x';
const char NSnapPeaCensusManifold::SEC_7_OR = 'v';
const char NSnapPeaCensusManifold::SEC_7_NOR = 'y';

std::ostream& NSnapPeaCensusManifold::writeName(std::ostream& out) const {
    // Some manifolds will get special names, and will have their usual
    // SnapPea names written in writeStructure() instead.
    if (section == SEC_5) {
        if (index == 0)
            return out << "Gieseking manifold";
        if (index == 4)
            return out << "Figure eight knot complement";
    }

    // No special names, just the usual SnapPea notation.
    out << "SnapPea " << section;

    // Pad the index with leading zeroes.
    // All sections are written with three-digit indices, except for
    // 7-tetrahedron orientable which uses four-digit indices.
    if (section == SEC_7_OR && index < 1000)
        out << '0';
    if (index < 100)
        out << '0';
    if (index < 10)
        out << '0';
    out << index;

    return out;
}

std::ostream& NSnapPeaCensusManifold::writeTeXName(std::ostream& out) const {
    out << '$' << section << "_{";

    // Pad the index with leading zeroes.
    // All sections are written with three-digit indices, except for
    // 7-tetrahedron orientable which uses four-digit indices.
    if (section == SEC_7_OR && index < 1000)
        out << '0';
    if (index < 100)
        out << '0';
    if (index < 10)
        out << '0';
    out << index << "}$";

    return out;
}

std::ostream& NSnapPeaCensusManifold::writeStructure(std::ostream& out) const {
    // If we didn't give the usual SnapPea name in writeName(), give it here.
    if (section == SEC_5) {
        // Gieseking manifold and figure eight knot complement:
        if (index == 0 || index == 4)
            out << "SnapPea m00" << index;
    }

    return out;
}

} // namespace regina

