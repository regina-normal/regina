
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include "algebra/abeliangroup.h"
#include "manifold/handlebody.h"
#include "triangulation/dim3.h"
#include "subcomplex/snappedball.h"

namespace regina {

SnappedBall* SnappedBall::clone() const {
    SnappedBall* ans = new SnappedBall();
    ans->tet = tet;
    ans->equator = equator;
    return ans;
}

SnappedBall* SnappedBall::formsSnappedBall(Tetrahedron<3>* tet) {
    int inFace1, inFace2;
    Perm<4> perm;
    for (inFace1 = 0; inFace1 < 3; inFace1++)
        if (tet->adjacentTetrahedron(inFace1) == tet) {
            perm = tet->adjacentGluing(inFace1);
            inFace2 = perm[inFace1];
            if (perm == Perm<4>(inFace1, inFace2)) {
                // This is it!
                SnappedBall* ans = new SnappedBall();
                ans->tet = tet;
                ans->equator = Edge<3>::edgeNumber[inFace1][inFace2];
                return ans;
            }
        }

    return 0;
}

Manifold* SnappedBall::manifold() const {
    return new Handlebody(0, true);
}

AbelianGroup* SnappedBall::homology() const {
    return new AbelianGroup();
}

} // namespace regina

