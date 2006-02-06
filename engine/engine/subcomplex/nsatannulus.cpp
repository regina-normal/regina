
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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

#include "subcomplex/nsatannulus.h"
#include "triangulation/nisomorphism.h"
#include "triangulation/ntriangulation.h"

namespace regina {

unsigned NSatAnnulus::meetsBoundary() const {
    unsigned ans = 0;
    if (! tet[0]->getAdjacentTetrahedron(roles[0][3]))
        ans++;
    if (! tet[1]->getAdjacentTetrahedron(roles[1][3]))
        ans++;
    return ans;
}

void NSatAnnulus::switchSides() {
    unsigned which, face;
    for (which = 0; which < 2; which++) {
        face = roles[which][3];
        roles[which] = tet[which]->getAdjacentTetrahedronGluing(face) *
            roles[which];
        tet[which] = tet[which]->getAdjacentTetrahedron(face);
    }
}

bool NSatAnnulus::isAdjacent(const NSatAnnulus& other, bool* refVert,
        bool* refHoriz) const {
    if (other.meetsBoundary())
        return false;

    // See what is actually attached to the given annulus.
    NSatAnnulus opposite(other);
    opposite.switchSides();

    if (opposite.tet[0] == tet[0] && opposite.tet[1] == tet[1]) {
        // Could be a match without horizontal reflection.

        if (opposite.roles[0] == roles[0] && opposite.roles[1] == roles[1]) {
            // Perfect match.
            if (refVert) *refVert = false;
            if (refHoriz) *refHoriz = false;
            return true;
        }

        if (opposite.roles[0] == roles[0] * NPerm(0, 1) &&
                opposite.roles[1] == roles[1] * NPerm(0, 1)) {
            // Match with vertical reflection.
            if (refVert) *refVert = true;
            if (refHoriz) *refHoriz = false;
            return true;
        }
    }

    if (opposite.tet[0] == tet[1] && opposite.tet[1] == tet[0]) {
        // Could be a match with horizontal reflection.

        if (opposite.roles[0] == roles[1] * NPerm(0, 1) &&
                opposite.roles[1] == roles[0] * NPerm(0, 1)) {
            // Match with horizontal reflection.
            if (refVert) *refVert = false;
            if (refHoriz) *refHoriz = true;
            return true;
        }

        if (opposite.roles[0] == roles[1] && opposite.roles[1] == roles[0]) {
            // Match with both reflections.
            if (refVert) *refVert = true;
            if (refHoriz) *refHoriz = true;
            return true;
        }
    }

    // No match.
    return false;
}

void NSatAnnulus::transform(const NTriangulation* originalTri,
        const NIsomorphism* iso, NTriangulation* newTri) {
    unsigned which;
    unsigned long tetID;
    for (which = 0; which < 2; which++) {
        tetID = originalTri->getTetrahedronIndex(tet[which]);
        tet[which] = newTri->getTetrahedron(iso->tetImage(tetID));
        roles[which] = iso->facePerm(tetID) * roles[which];
    }
}

} // namespace regina

