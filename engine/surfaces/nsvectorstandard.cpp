
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

#include "enumerate/enumconstraints.h"
#include "surfaces/nsvectorstandard.h"
#include "maths/matrix.h"
#include "maths/rational.h"
#include "triangulation/dim3.h"

namespace regina {

LargeInteger NSVectorStandard::edgeWeight(
        size_t edgeIndex, const Triangulation<3>* triang) const {
    // Find a tetrahedron next to the edge in question.
    const EdgeEmbedding<3>& emb = triang->edge(edgeIndex)->front();
    long tetIndex = emb.tetrahedron()->index();
    int start = emb.vertices()[0];
    int end = emb.vertices()[1];

    // Add up the triangles and quads meeting that edge.
    // Triangles:
    LargeInteger ans(coords_[7 * tetIndex + start]);
    ans += coords_[7 * tetIndex + end];
    // Quads:
    ans += coords_[7 * tetIndex + 4 + quadMeeting[start][end][0]];
    ans += coords_[7 * tetIndex + 4 + quadMeeting[start][end][1]];
    return ans;
}

LargeInteger NSVectorStandard::arcs(size_t triIndex,
        int triVertex, const Triangulation<3>* triang) const {
    // Find a tetrahedron next to the triangle in question.
    const TriangleEmbedding<3>& emb = triang->triangles()[triIndex]->front();
    long tetIndex = emb.tetrahedron()->index();
    int vertex = emb.vertices()[triVertex];
    int backOfFace = emb.vertices()[3];

    // Add up the triangles and quads meeting that triangle in the required arc.
    // Triangles:
    LargeInteger ans(coords_[7 * tetIndex + vertex]);
    // Quads:
    ans += coords_[7 * tetIndex + 4 + quadSeparating[vertex][backOfFace]];
    return ans;
}

NormalSurfaceVector* NSVectorStandard::makeZeroVector(
        const Triangulation<3>* triangulation) {
    return new NSVectorStandard(7 * triangulation->size());
}

MatrixInt* NSVectorStandard::makeMatchingEquations(
        const Triangulation<3>* triangulation) {
    size_t nCoords = 7 * triangulation->size();
    // Three equations per non-boundary triangle.
    // F_boundary + 2 F_internal = 4 T
    long nEquations = 3 * (4 * long(triangulation->size()) -
        long(triangulation->countTriangles()));
    MatrixInt* ans = new MatrixInt(nEquations, nCoords);

    // Run through each internal triangle and add the corresponding three
    // equations.
    unsigned row = 0;
    int i;
    size_t tet0, tet1;
    Perm<4> perm0, perm1;
    for (auto fit = triangulation->triangles().begin();
            fit != triangulation->triangles().end(); fit++) {
        if (! (*fit)->isBoundary()) {
            tet0 = (*fit)->embedding(0).tetrahedron()->index();
            tet1 = (*fit)->embedding(1).tetrahedron()->index();
            perm0 = (*fit)->embedding(0).vertices();
            perm1 = (*fit)->embedding(1).vertices();
            for (i=0; i<3; i++) {
                // Triangles:
                ans->entry(row, 7*tet0 + perm0[i]) += 1;
                ans->entry(row, 7*tet1 + perm1[i]) -= 1;
                // Quads:
                ans->entry(row, 7*tet0 + 4 +
                    quadSeparating[perm0[i]][perm0[3]]) += 1;
                ans->entry(row, 7*tet1 + 4 +
                    quadSeparating[perm1[i]][perm1[3]]) -= 1;
                row++;
            }
        }
    }
    return ans;
}

EnumConstraints* NSVectorStandard::makeEmbeddedConstraints(
        const Triangulation<3>* triangulation) {
    EnumConstraints* ans = new EnumConstraints(triangulation->size());

    unsigned base = 0;
    for (unsigned c = 0; c < ans->size(); ++c) {
        (*ans)[c].insert((*ans)[c].end(), base + 4);
        (*ans)[c].insert((*ans)[c].end(), base + 5);
        (*ans)[c].insert((*ans)[c].end(), base + 6);
        base += 7;
    }

    return ans;
}

} // namespace regina

