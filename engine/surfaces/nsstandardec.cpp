
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

#include "enumerate/nenumconstraint.h"
#include "surfaces/nsstandardec.h"
#include "maths/nmatrixint.h"
#include "maths/nrational.h"
#include "maths/nvectorunit.h"
#include "triangulation/ntriangulation.h"

namespace regina {

NLargeInteger NNormalSurfaceVectorStandardEC::getEdgeWeight(
        unsigned long edgeIndex, NTriangulation* triang) const {
    // Find a tetrahedron next to the edge in question.
    const NEdgeEmbedding& emb = triang->getEdges()[edgeIndex]->
        getEmbeddings().front();
    long tetIndex = triang->tetrahedronIndex(emb.getTetrahedron());
    int start = emb.getVertices()[0];
    int end = emb.getVertices()[1];

    // Add up the triangles and quads meeting that edge.
    // Triangles:
    NLargeInteger ans((*this)[7 * tetIndex + start]);
    ans += (*this)[7 * tetIndex + end];
    // Quads:
    ans += (*this)[7 * tetIndex + 4 + vertexSplitMeeting[start][end][0]];
    ans += (*this)[7 * tetIndex + 4 + vertexSplitMeeting[start][end][1]];
    return ans;
}

NLargeInteger NNormalSurfaceVectorStandardEC::getFaceArcs(
        unsigned long faceIndex, int faceVertex, NTriangulation* triang) const {
    // Find a tetrahedron next to the face in question.
    const NFaceEmbedding& emb = triang->getFaces()[faceIndex]->
        getEmbedding(0);
    long tetIndex = triang->tetrahedronIndex(emb.getTetrahedron());
    int vertex = emb.getVertices()[faceVertex];
    int backOfFace = emb.getVertices()[3];

    // Add up the triangles and quads meeting that face in the required arc.
    // Triangles:
    NLargeInteger ans((*this)[7 * tetIndex + vertex]);
    // Quads:
    ans += (*this)[7 * tetIndex + 4 + vertexSplit[vertex][backOfFace]];
    return ans;
}

NNormalSurfaceVector* NNormalSurfaceVectorStandardEC::makeZeroVector(
        const NTriangulation* triangulation) {
    return new NNormalSurfaceVectorStandardEC(
        7 * triangulation->getNumberOfTetrahedra() + 1);
}

NMatrixInt* NNormalSurfaceVectorStandardEC::makeMatchingEquations(
        NTriangulation* triangulation) {
    unsigned long nCoords = 7 * triangulation->getNumberOfTetrahedra() + 1;
    // Three equations per non-boundary face, and one more for Euler char.
    // F_boundary + 2 F_internal = 4 T
    long nEquations = 3 * (4 * long(triangulation->getNumberOfTetrahedra()) -
        long(triangulation->getNumberOfFaces())) + 1;
    NMatrixInt* ans = new NMatrixInt(nEquations, nCoords);

    // Add an initial equation for Euler characteristic.
    // The equation will be of the form 2\chi - 2F + 2E - 2V = 0.
    // We compute (2E - 2F) by iterating through disc types in
    // tetrahedra, and we compute 2V by iterating through edge weights.
    unsigned long tet0, tet1;
    int i,j;
    NTetrahedron* t;
    unsigned pos = 0;
    for (NTriangulation::TetrahedronIterator tit =
            triangulation->getTetrahedra().begin();
            tit != triangulation->getTetrahedra().end(); ++tit) {
        t = *tit;

        ans->entry(0, pos) = 1;
        ans->entry(0, pos + 1) = 1;
        ans->entry(0, pos + 2) = 1;
        ans->entry(0, pos + 3) = 1;
        ans->entry(0, pos + 4) = 2;
        ans->entry(0, pos + 5) = 2;
        ans->entry(0, pos + 6) = 2;

        for (i = 0; i < 4; ++i)
            if (! t->getAdjacentTetrahedron(i)) {
                // Face i is a boundary face.
                for (j = 0; j < 7; ++j)
                    if (j != i)
                        ans->entry(0, pos + i) += 1;
            }

        pos += 7;
    }

    ans->entry(0, pos) = 2;

    for (NTriangulation::EdgeIterator eit = triangulation->getEdges().begin();
            eit != triangulation->getEdges().end(); eit++) {
        // Find a tetrahedron next to the edge in question.
        const NEdgeEmbedding& emb = (*eit)->getEmbeddings().front();
        tet0 = triangulation->tetrahedronIndex(emb.getTetrahedron());
        i = emb.getVertices()[0];
        j = emb.getVertices()[1];

        // Add up the triangles and quads meeting that edge.
        // Triangles:
        ans->entry(0, 7 * tet0 + i) -= 2;
        ans->entry(0, 7 * tet0 + j) -= 2;
        // Quads:
        ans->entry(0, 7 * tet0 + 4 + vertexSplitMeeting[i][j][0]) -= 2;
        ans->entry(0, 7 * tet0 + 4 + vertexSplitMeeting[i][j][1]) -= 2;
    }

    // Run through each internal face and add the corresponding three
    // equations.
    unsigned row = 1;
    NPerm4 perm0, perm1;
    for (NTriangulation::FaceIterator fit = triangulation->getFaces().begin();
            fit != triangulation->getFaces().end(); fit++) {
        if (! (*fit)->isBoundary()) {
            tet0 = triangulation->tetrahedronIndex(
                (*fit)->getEmbedding(0).getTetrahedron());
            tet1 = triangulation->tetrahedronIndex(
                (*fit)->getEmbedding(1).getTetrahedron());
            perm0 = (*fit)->getEmbedding(0).getVertices();
            perm1 = (*fit)->getEmbedding(1).getVertices();
            for (i=0; i<3; i++) {
                // Triangles:
                ans->entry(row, 7*tet0 + perm0[i]) += 1;
                ans->entry(row, 7*tet1 + perm1[i]) -= 1;
                // Quads:
                ans->entry(row, 7*tet0 + 4 +
                    vertexSplit[perm0[i]][perm0[3]]) += 1;
                ans->entry(row, 7*tet1 + 4 +
                    vertexSplit[perm1[i]][perm1[3]]) -= 1;
                row++;
            }
        }
    }
    return ans;
}

NEnumConstraintList* NNormalSurfaceVectorStandardEC::makeEmbeddedConstraints(
        NTriangulation* triangulation) {
    NEnumConstraintList* ans = new NEnumConstraintList(
        triangulation->getNumberOfTetrahedra());

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

