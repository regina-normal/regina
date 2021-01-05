
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

#include <deque>
#include <set>
#include "enumerate/enumconstraints.h"
#include "surfaces/nsvectorquadclosed.h"
#include "surfaces/nsvectorstandard.h"
#include "maths/matrix.h"
#include "maths/rational.h"
#include "snappea/snappeatriangulation.h"

namespace regina {

NormalSurfaceVector* NSVectorQuadClosed::makeZeroVector(
        const Triangulation<3>* triangulation) {
    return new NSVectorQuadClosed(3 * triangulation->size());
}

MatrixInt* NSVectorQuadClosed::makeMatchingEquations(
        const Triangulation<3>* triangulation) {
    // Enforce our basic preconditions.
    if (! (triangulation->isOriented() && triangulation->isIdeal() &&
            triangulation->countBoundaryComponents() == 1 &&
            triangulation->countVertices() == 1 &&
            triangulation->vertex(0)->link() == Vertex<3>::TORUS))
        return 0;

    // We will use SnapPea to build the additional constraint that
    // enforce closed surfaces.  Before doing anything else, see whether
    // SnapPea is going to play along.
    SnapPeaTriangulation snapPea(*triangulation, false);
    MatrixInt* coeffs = snapPea.slopeEquations();
    if (! (coeffs && snapPea.isIdenticalTo(*triangulation))) {
        // SnapPea either couldn't handle it, or else it retriangulated on us.
        delete coeffs;
        return 0;
    }

    unsigned long nCoords = 3 * triangulation->size();
    // One equation per edge, plus two per ideal vertex.
    // (This code is written a little more generically, in order to
    // support multiple ideal vertices at some later date.)
    long nEquations = long(triangulation->countEdges() +
        2 * triangulation->countBoundaryComponents());

    MatrixInt* ans = new MatrixInt(nEquations, nCoords);
    unsigned long row = 0;

    // Run through each internal edge and add the corresponding
    // equation.
    Perm<4> perm;
    unsigned long tetIndex;
    for (auto e : triangulation->edges()) {
        for (auto& emb : *e) {
            tetIndex = emb.tetrahedron()->index();
            perm = emb.vertices();
            ans->entry(row, 3 * tetIndex + quadSeparating[perm[0]][perm[2]])
                += 1;
            ans->entry(row, 3 * tetIndex + quadSeparating[perm[0]][perm[3]])
                -= 1;
        }
        ++row;
    }

    // Run through each ideal vertex and add the corresponding meridian
    // and longitude equations.
    int i, j;
    for (i = 0; i < triangulation->countBoundaryComponents(); ++i) {
        for (j = 0; j < 3 * triangulation->size(); ++j) {
            ans->entry(row, j) = coeffs->entry(2 * i, j);
            ans->entry(row + 1, j) = coeffs->entry(2 * i + 1, j);
        }

        row += 2;
    }

    delete coeffs;
    return ans;
}

} // namespace regina

