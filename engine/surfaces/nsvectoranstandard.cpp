
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
#include "surfaces/nsvectoranstandard.h"
#include "maths/matrix.h"
#include "maths/rational.h"
#include "triangulation/dim3.h"

namespace regina {

LargeInteger NSVectorANStandard::edgeWeight(
        size_t edgeIndex, const Triangulation<3>* triang) const {
    // Find a tetrahedron next to the edge in question.
    const EdgeEmbedding<3>& emb = triang->edge(edgeIndex)->front();
    long tetIndex = emb.tetrahedron()->index();
    int start = emb.vertices()[0];
    int end = emb.vertices()[1];

    // Add up the triangles, quads and octagons meeting that edge.
    // Triangles:
    LargeInteger ans(coords_[10 * tetIndex + start]);
    ans += coords_[10 * tetIndex + end];
    // Quads:
    ans += coords_[10 * tetIndex + 4 + quadMeeting[start][end][0]];
    ans += coords_[10 * tetIndex + 4 + quadMeeting[start][end][1]];
    // Octagons:
    ans += coords_[10 * tetIndex + 7];
    ans += coords_[10 * tetIndex + 8];
    ans += coords_[10 * tetIndex + 9];
    ans += coords_[10 * tetIndex + 7 + quadSeparating[start][end]];
    return ans;
}

LargeInteger NSVectorANStandard::arcs(size_t triIndex,
        int triVertex, const Triangulation<3>* triang) const {
    // Find a tetrahedron next to the triangle in question.
    const TriangleEmbedding<3>& emb = triang->triangles()[triIndex]->front();
    long tetIndex = emb.tetrahedron()->index();
    int vertex = emb.vertices()[triVertex];
    int backOfFace = emb.vertices()[3];

    // Add up the discs meeting that triangle in that required arc.
    // Triangles:
    LargeInteger ans(coords_[10 * tetIndex + vertex]);
    // Quads:
    ans += coords_[10 * tetIndex + 4 + quadSeparating[vertex][backOfFace]];
    // Octagons:
    ans += coords_[10 * tetIndex + 7 + quadMeeting[vertex][backOfFace][0]];
    ans += coords_[10 * tetIndex + 7 + quadMeeting[vertex][backOfFace][1]];
    return ans;
}

NormalSurfaceVector* NSVectorANStandard::makeZeroVector(
        const Triangulation<3>* triangulation) {
    return new NSVectorANStandard(10 * triangulation->size());
}

MatrixInt* NSVectorANStandard::makeMatchingEquations(
        const Triangulation<3>* triangulation) {
    size_t nCoords = 10 * triangulation->size();
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
                ans->entry(row, 10*tet0 + perm0[i]) += 1;
                ans->entry(row, 10*tet1 + perm1[i]) -= 1;
                // Quads:
                ans->entry(row, 10*tet0 + 4 +
                    quadSeparating[perm0[i]][perm0[3]]) += 1;
                ans->entry(row, 10*tet1 + 4 +
                    quadSeparating[perm1[i]][perm1[3]]) -= 1;
                // Octagons:
                ans->entry(row, 10*tet0 + 7 +
                    quadMeeting[perm0[i]][perm0[3]][0]) += 1;
                ans->entry(row, 10*tet1 + 7 +
                    quadMeeting[perm1[i]][perm1[3]][0]) -= 1;
                ans->entry(row, 10*tet0 + 7 +
                    quadMeeting[perm0[i]][perm0[3]][1]) += 1;
                ans->entry(row, 10*tet1 + 7 +
                    quadMeeting[perm1[i]][perm1[3]][1]) -= 1;
                row++;
            }
        }
    }
    return ans;
}

EnumConstraints* NSVectorANStandard::makeEmbeddedConstraints(
        const Triangulation<3>* triangulation) {
    // At most one quad/oct per tetrahedron.
    // Also at most one oct type overall.
    EnumConstraints* ans = new EnumConstraints(triangulation->size() + 1);

    unsigned base = 0;
    for (unsigned c = 1; c < ans->size(); ++c) {
        (*ans)[c].insert((*ans)[c].end(), base + 4);
        (*ans)[c].insert((*ans)[c].end(), base + 5);
        (*ans)[c].insert((*ans)[c].end(), base + 6);
        (*ans)[c].insert((*ans)[c].end(), base + 7);
        (*ans)[c].insert((*ans)[c].end(), base + 8);
        (*ans)[c].insert((*ans)[c].end(), base + 9);

        (*ans)[0].insert((*ans)[0].end(), base + 7);
        (*ans)[0].insert((*ans)[0].end(), base + 8);
        (*ans)[0].insert((*ans)[0].end(), base + 9);

        base += 10;
    }

    return ans;
}

} // namespace regina

