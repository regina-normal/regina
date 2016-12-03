
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
#include "surfaces/nsvectororiented.h"
#include "maths/matrix.h"
#include "maths/rational.h"
#include "triangulation/dim3.h"

namespace regina {

LargeInteger NSVectorOriented::edgeWeight(
        size_t edgeIndex, const Triangulation<3>* triang) const {
    // Find a tetrahedron next to the edge in question.
    const EdgeEmbedding<3>& emb = triang->edge(edgeIndex)->front();
    long tetIndex = emb.tetrahedron()->index();
    int start = emb.vertices()[0];
    int end = emb.vertices()[1];

    // Add up the triangles and quads meeting that edge.
    // Triangles:
    LargeInteger ans(triangles(tetIndex,start,triang));
    ans += triangles(tetIndex,end,triang);
    // Quads:
    ans += quads(tetIndex, quadMeeting[start][end][0],triang);
    ans += quads(tetIndex, quadMeeting[start][end][1],triang);
    return ans;
}

LargeInteger NSVectorOriented::arcs(size_t triIndex,
        int triVertex, const Triangulation<3>* triang) const {
    // Find a tetrahedron next to the triangle in question.
    const TriangleEmbedding<3>& emb = triang->triangles()[triIndex]->front();
    long tetIndex = emb.tetrahedron()->index();
    int vertex = emb.vertices()[triVertex];
    int backOfFace = emb.vertices()[3];

    // Add up the triangles and quads meeting that triangle in the required arc.
    // Triangles:
    LargeInteger ans(triangles(tetIndex,vertex,triang));
    // Quads:
    ans += quads(tetIndex, quadSeparating[vertex][backOfFace],triang);
    return ans;
}

NormalSurfaceVector* NSVectorOriented::makeZeroVector(
        const Triangulation<3>* triangulation) {
    return new NSVectorOriented(14 * triangulation->size());
}

MatrixInt* NSVectorOriented::makeMatchingEquations(
        const Triangulation<3>* triangulation) {
    size_t nCoords = 14 * triangulation->size();
    // Six equations per non-boundary triangle.
    // F_boundary + 2 F_internal = 4 T
    long nEquations = 6 * (4 * long(triangulation->size()) -
        long(triangulation->countTriangles()));
    MatrixInt* ans = new MatrixInt(nEquations, nCoords);

    // Run through each internal triangle and add the corresponding three
    // equations.
    unsigned row = 0;
    int i;
    size_t tet0, tet1;
    Perm<4> perm0, perm1;
    bool natural;
    for (auto fit = triangulation->triangles().begin();
            fit != triangulation->triangles().end(); fit++) {
        if (! (*fit)->isBoundary()) {
            tet0 = (*fit)->embedding(0).tetrahedron()->index();
            tet1 = (*fit)->embedding(1).tetrahedron()->index();
            perm0 = (*fit)->embedding(0).vertices();
            perm1 = (*fit)->embedding(1).vertices();
            for (i=0; i<3; i++) {
                // row: oriented towards the vertex of the face
                // row+1: oriented towards the opposite face

                // Triangles:
                ans->entry(row, 14*tet0 + 2*perm0[i]) += 1;
                ans->entry(row+1, 14*tet0 + 2*perm0[i] + 1) += 1;

                ans->entry(row, 14*tet1 + 2*perm1[i]) -= 1;
                ans->entry(row+1, 14*tet1 + 2*perm1[i] + 1) -= 1;

                // Quads:
                natural = (perm0[i] == 0 || perm0[3] == 0);
                ans->entry(row, 14*tet0 + 8 +
                    2*quadSeparating[perm0[i]][perm0[3]] +
                        (natural ? 0 : 1)) += 1;
                ans->entry(row+1, 14*tet0 + 8 +
                    2*quadSeparating[perm0[i]][perm0[3]] +
                        (natural ? 1 : 0)) += 1;

                natural = (perm1[i] == 0 || perm1[3] == 0);
                ans->entry(row, 14*tet1 + 8 +
                    2*quadSeparating[perm1[i]][perm1[3]] +
                        (natural ? 0 : 1)) -= 1;
                ans->entry(row+1, 14*tet1 + 8 +
                    2*quadSeparating[perm1[i]][perm1[3]] +
                        (natural ? 1 : 0)) -= 1;

                row+=2;
            }
        }
    }
    return ans;
}

EnumConstraints* NSVectorOriented::makeEmbeddedConstraints(
        const Triangulation<3>* triangulation) {
    // TODO: Must be a neater way of doing this, but might mean re-working
    // bitmasks.
    EnumConstraints* ans = new EnumConstraints(8 * triangulation->size());

    unsigned base = 0;
    unsigned c = 0;
    while (c < ans->size()) {
        for (unsigned d = 0; d < 2; d++) {
            for (unsigned e = 0; e < 2; e++) {
                for (unsigned f = 0; f < 2; f++) {
                    (*ans)[c].insert((*ans)[c].end(), base + 8 + d);
                    (*ans)[c].insert((*ans)[c].end(), base + 10 + e);
                    (*ans)[c].insert((*ans)[c].end(), base + 12 + f);
                    ++c;
                }
            }
        }
        base += 14;
    }

    return ans;
}

} // namespace regina

