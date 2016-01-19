
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

#include "enumerate/nenumconstraint.h"
#include "surfaces/nsanstandard.h"
#include "maths/nmatrixint.h"
#include "maths/nrational.h"
#include "triangulation/ntriangulation.h"

namespace regina {

NLargeInteger NNormalSurfaceVectorANStandard::edgeWeight(
        size_t edgeIndex, const NTriangulation* triang) const {
    // Find a tetrahedron next to the edge in question.
    const NEdgeEmbedding& emb = triang->edge(edgeIndex)->front();
    long tetIndex = triang->tetrahedronIndex(emb.getTetrahedron());
    int start = emb.getVertices()[0];
    int end = emb.getVertices()[1];

    // Add up the triangles, quads and octagons meeting that edge.
    // Triangles:
    NLargeInteger ans((*this)[10 * tetIndex + start]);
    ans += (*this)[10 * tetIndex + end];
    // Quads:
    ans += (*this)[10 * tetIndex + 4 + vertexSplitMeeting[start][end][0]];
    ans += (*this)[10 * tetIndex + 4 + vertexSplitMeeting[start][end][1]];
    // Octagons:
    ans += (*this)[10 * tetIndex + 7];
    ans += (*this)[10 * tetIndex + 8];
    ans += (*this)[10 * tetIndex + 9];
    ans += (*this)[10 * tetIndex + 7 + vertexSplit[start][end]];
    return ans;
}

NLargeInteger NNormalSurfaceVectorANStandard::arcs(size_t triIndex,
        int triVertex, const NTriangulation* triang) const {
    // Find a tetrahedron next to the triangle in question.
    const NTriangleEmbedding& emb = triang->triangles()[triIndex]->front();
    long tetIndex = triang->tetrahedronIndex(emb.getTetrahedron());
    int vertex = emb.getVertices()[triVertex];
    int backOfFace = emb.getVertices()[3];

    // Add up the discs meeting that triangle in that required arc.
    // Triangles:
    NLargeInteger ans((*this)[10 * tetIndex + vertex]);
    // Quads:
    ans += (*this)[10 * tetIndex + 4 + vertexSplit[vertex][backOfFace]];
    // Octagons:
    ans += (*this)[10 * tetIndex + 7 +
        vertexSplitMeeting[vertex][backOfFace][0]];
    ans += (*this)[10 * tetIndex + 7 +
        vertexSplitMeeting[vertex][backOfFace][1]];
    return ans;
}

NNormalSurfaceVector* NNormalSurfaceVectorANStandard::makeZeroVector(
        const NTriangulation* triangulation) {
    return new NNormalSurfaceVectorANStandard(10 * triangulation->size());
}

NMatrixInt* NNormalSurfaceVectorANStandard::makeMatchingEquations(
        const NTriangulation* triangulation) {
    size_t nCoords = 10 * triangulation->size();
    // Three equations per non-boundary triangle.
    // F_boundary + 2 F_internal = 4 T
    long nEquations = 3 * (4 * long(triangulation->size()) -
        long(triangulation->countTriangles()));
    NMatrixInt* ans = new NMatrixInt(nEquations, nCoords);

    // Run through each internal triangle and add the corresponding three
    // equations.
    unsigned row = 0;
    int i;
    size_t tet0, tet1;
    NPerm4 perm0, perm1;
    for (auto fit = triangulation->triangles().begin();
            fit != triangulation->triangles().end(); fit++) {
        if (! (*fit)->isBoundary()) {
            tet0 = triangulation->tetrahedronIndex(
                (*fit)->embedding(0).getTetrahedron());
            tet1 = triangulation->tetrahedronIndex(
                (*fit)->embedding(1).getTetrahedron());
            perm0 = (*fit)->embedding(0).getVertices();
            perm1 = (*fit)->embedding(1).getVertices();
            for (i=0; i<3; i++) {
                // Triangles:
                ans->entry(row, 10*tet0 + perm0[i]) += 1;
                ans->entry(row, 10*tet1 + perm1[i]) -= 1;
                // Quads:
                ans->entry(row, 10*tet0 + 4 +
                    vertexSplit[perm0[i]][perm0[3]]) += 1;
                ans->entry(row, 10*tet1 + 4 +
                    vertexSplit[perm1[i]][perm1[3]]) -= 1;
                // Octagons:
                ans->entry(row, 10*tet0 + 7 +
                    vertexSplitMeeting[perm0[i]][perm0[3]][0]) += 1;
                ans->entry(row, 10*tet1 + 7 +
                    vertexSplitMeeting[perm1[i]][perm1[3]][0]) -= 1;
                ans->entry(row, 10*tet0 + 7 +
                    vertexSplitMeeting[perm0[i]][perm0[3]][1]) += 1;
                ans->entry(row, 10*tet1 + 7 +
                    vertexSplitMeeting[perm1[i]][perm1[3]][1]) -= 1;
                row++;
            }
        }
    }
    return ans;
}

NEnumConstraintList* NNormalSurfaceVectorANStandard::makeEmbeddedConstraints(
        const NTriangulation* triangulation) {
    // At most one quad/oct per tetrahedron.
    // Also at most one oct type overall.
    NEnumConstraintList* ans = new NEnumConstraintList(
        triangulation->size() + 1);

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

