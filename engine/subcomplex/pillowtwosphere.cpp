
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#include "subcomplex/pillowtwosphere.h"
#include "triangulation/dim3.h"

namespace regina {

std::unique_ptr<PillowTwoSphere> PillowTwoSphere::recognise(
        Triangle<3>* tri1, Triangle<3>* tri2) {
    if (tri1 == tri2 || tri1->isBoundary() || tri2->isBoundary())
        return nullptr;
    Edge<3>* edge[2][3];
    int i;
    for (i = 0; i < 3; i++) {
        edge[0][i] = tri1->edge(i);
        edge[1][i] = tri2->edge(i);
    }
    if (edge[0][0] == edge[0][1] || edge[0][0] == edge[0][2] ||
            edge[0][1] == edge[0][2])
        return nullptr;

    // The first triangle has three distinct edges.  See if it matches to the
    // second triangle.
    int joinTo0 = -1;
    for (i = 0; i < 3; i++)
        if (edge[0][0] == edge[1][i]) {
            joinTo0 = i;
            break;
        }
    if (joinTo0 == -1)
        return nullptr;

    // Now make sure the edges all match up and with the correct
    // permutations.
    Perm<4> perm = tri2->edgeMapping(joinTo0) * tri1->edgeMapping(0).inverse();
    for (i = 1; i < 3; i++) {
        if (edge[0][i] != edge[1][perm[i]])
            return nullptr;
        if (! (tri2->edgeMapping(perm[i]) == perm * tri1->edgeMapping(i)))
            return nullptr;
    }

    // We have an answer.
    //
    // Note: we cannot use make_unique here, since the class
    // constructor is private.
    return std::unique_ptr<PillowTwoSphere>(new PillowTwoSphere(
        tri1, tri2, perm));
}

void PillowTwoSphere::writeTextShort(std::ostream& out) const {
    out << "Pillow 2-sphere, triangles "
        << triangle_[0]->index() << ", " << triangle_[1]->index();
}

} // namespace regina

