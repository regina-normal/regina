
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

std::unique_ptr<SnappedBall> SnappedBall::recognise(Tetrahedron<3>* tet) {
    int inFace1, inFace2;
    Perm<4> perm;
    for (inFace1 = 0; inFace1 < 3; inFace1++)
        if (tet->adjacentTetrahedron(inFace1) == tet) {
            perm = tet->adjacentGluing(inFace1);
            inFace2 = perm[inFace1];
            if (perm == Perm<4>(inFace1, inFace2)) {
                // This is it!
                return std::unique_ptr<SnappedBall>(new SnappedBall(
                    tet, Edge<3>::edgeNumber[inFace1][inFace2]));
            }
        }

    return nullptr;
}

std::unique_ptr<Manifold> SnappedBall::manifold() const {
    return std::make_unique<Handlebody>(0);
}

AbelianGroup SnappedBall::homology() const {
    return AbelianGroup();
}

} // namespace regina

