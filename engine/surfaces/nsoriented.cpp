
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
#include "surfaces/nsoriented.h"
#include "maths/nmatrixint.h"
#include "maths/nrational.h"
#include "triangulation/ntriangulation.h"

namespace regina {

NLargeInteger NNormalSurfaceVectorOriented::getEdgeWeight(
        unsigned long edgeIndex, NTriangulation* triang) const {
    // Find a tetrahedron next to the edge in question.
    const NEdgeEmbedding& emb = triang->getEdges()[edgeIndex]->
        getEmbeddings().front();
    long tetIndex = triang->tetrahedronIndex(emb.getTetrahedron());
    int start = emb.getVertices()[0];
    int end = emb.getVertices()[1];

    // Add up the triangles and quads meeting that edge.
    // Triangles:
    NLargeInteger ans(getTriangleCoord(tetIndex,start,triang));
    ans += getTriangleCoord(tetIndex,end,triang);
    // Quads:
    ans += getQuadCoord(tetIndex,vertexSplitMeeting[start][end][0],triang);
    ans += getQuadCoord(tetIndex,vertexSplitMeeting[start][end][1],triang);
    return ans;
}

NLargeInteger NNormalSurfaceVectorOriented::getFaceArcs(
        unsigned long faceIndex, int faceVertex, NTriangulation* triang) const {
    // Find a tetrahedron next to the face in question.
    const NFaceEmbedding& emb = triang->getFaces()[faceIndex]->
        getEmbedding(0);
    long tetIndex = triang->tetrahedronIndex(emb.getTetrahedron());
    int vertex = emb.getVertices()[faceVertex];
    int backOfFace = emb.getVertices()[3];

    // Add up the triangles and quads meeting that face in the required arc.
    // Triangles:
    NLargeInteger ans(getTriangleCoord(tetIndex,vertex,triang));
    // Quads:
    ans += getQuadCoord(tetIndex,vertexSplit[vertex][backOfFace],triang);
    return ans;
}

NNormalSurfaceVector* NNormalSurfaceVectorOriented::makeZeroVector(
        const NTriangulation* triangulation) {
    return new NNormalSurfaceVectorOriented(
        14 * triangulation->getNumberOfTetrahedra());
}

NMatrixInt* NNormalSurfaceVectorOriented::makeMatchingEquations(
        NTriangulation* triangulation) {
    unsigned long nCoords = 14 * triangulation->getNumberOfTetrahedra();
    // Six equations per non-boundary face.
    // F_boundary + 2 F_internal = 4 T
    long nEquations = 6 * (4 * long(triangulation->getNumberOfTetrahedra()) -
        long(triangulation->getNumberOfFaces()));
    NMatrixInt* ans = new NMatrixInt(nEquations, nCoords);

    // Run through each internal face and add the corresponding three
    // equations.
    unsigned row = 0;
    int i;
    unsigned long tet0, tet1;
    NPerm4 perm0, perm1;
    bool natural;
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
                    2*vertexSplit[perm0[i]][perm0[3]] +
                        (natural ? 0 : 1)) += 1;
                ans->entry(row+1, 14*tet0 + 8 +
                    2*vertexSplit[perm0[i]][perm0[3]] +
                        (natural ? 1 : 0)) += 1;

                natural = (perm1[i] == 0 || perm1[3] == 0);
                ans->entry(row, 14*tet1 + 8 +
                    2*vertexSplit[perm1[i]][perm1[3]] +
                        (natural ? 0 : 1)) -= 1;
                ans->entry(row+1, 14*tet1 + 8 +
                    2*vertexSplit[perm1[i]][perm1[3]] +
                        (natural ? 1 : 0)) -= 1;

                row+=2;
            }
        }
    }
    return ans;
}

NEnumConstraintList* NNormalSurfaceVectorOriented::makeEmbeddedConstraints(
        NTriangulation* triangulation) {
    // TODO: Must be a neater way of doing this, but might mean re-working
    // bitmasks.
    NEnumConstraintList* ans = new NEnumConstraintList(
        8 * triangulation->getNumberOfTetrahedra());

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

