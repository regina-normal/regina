
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

#include "algebra/nabeliangroup.h"
#include "manifold/nhandlebody.h"
#include "triangulation/ntetrahedron.h"
#include "subcomplex/nsnappedball.h"

namespace regina {

NSnappedBall* NSnappedBall::clone() const {
    NSnappedBall* ans = new NSnappedBall();
    ans->tet = tet;
    ans->equator = equator;
    return ans;
}

NSnappedBall* NSnappedBall::formsSnappedBall(NTetrahedron* tet) {
    int inFace1, inFace2;
    NPerm perm;
    for (inFace1 = 0; inFace1 < 3; inFace1++)
        if (tet->adjacentTetrahedron(inFace1) == tet) {
            perm = tet->adjacentGluing(inFace1);
            inFace2 = perm[inFace1];
            if (perm == NPerm(inFace1, inFace2)) {
                // This is it!
                NSnappedBall* ans = new NSnappedBall();
                ans->tet = tet;
                ans->equator = NEdge::edgeNumber[inFace1][inFace2];
                return ans;
            }
        }

    return 0;
}

NManifold* NSnappedBall::getManifold() const {
    return new NHandlebody(0, true);
}

NAbelianGroup* NSnappedBall::getHomologyH1() const {
    return new NAbelianGroup();
}

} // namespace regina

