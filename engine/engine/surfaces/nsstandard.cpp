
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "enumerate/ncompconstraint.h"
#include "surfaces/nsstandard.h"
#include "utilities/nrational.h"
#include "maths/nmatrixint.h"
#include "maths/nvectorunit.h"
#include "triangulation/ntriangulation.h"

namespace regina {

NLargeInteger NNormalSurfaceVectorStandard::getEdgeWeight(
        unsigned long edgeIndex, NTriangulation* triang) const {
    // Find a tetrahedron next to the edge in question.
    const NEdgeEmbedding& emb = triang->getEdges()[edgeIndex]->
        getEmbeddings().front();
    long tetIndex = triang->getTetrahedronIndex(
        emb.getTetrahedron());
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

NLargeInteger NNormalSurfaceVectorStandard::getFaceArcs(
        unsigned long faceIndex, int faceVertex, NTriangulation* triang) const {
    // Find a tetrahedron next to the face in question.
    const NFaceEmbedding& emb = triang->getFaces()[faceIndex]->
        getEmbedding(0);
    long tetIndex = triang->getTetrahedronIndex(
        emb.getTetrahedron());
    int vertex = emb.getVertices()[faceVertex];
    int backOfFace = emb.getVertices()[3];

    // Add up the triangles and quads meeting that face in the required arc.
    // Triangles:
    NLargeInteger ans((*this)[7 * tetIndex + vertex]);
    // Quads:
    ans += (*this)[7 * tetIndex + 4 + vertexSplit[vertex][backOfFace]];
    return ans;
}

NMatrixInt* NNormalSurfaceVectorStandard::makeMatchingEquations(
        NTriangulation* triangulation) {
    unsigned long nCoords = 7 * triangulation->getNumberOfTetrahedra();
    // Three equations per non-boundary face.
    // F_boundary + 2 F_internal = 4 T
    long nEquations = 3 * (4 * long(triangulation->getNumberOfTetrahedra()) -
        long(triangulation->getNumberOfFaces()));
    NMatrixInt* ans = new NMatrixInt(nEquations, nCoords);

    // Run through each internal face and add the corresponding three
    // equations.
    unsigned row = 0;
    int i;
    unsigned long tet0, tet1;
    NPerm perm0, perm1;
    for (NTriangulation::FaceIterator fit = triangulation->getFaces().begin();
            fit != triangulation->getFaces().end(); fit++) {
        if (! (*fit)->isBoundary()) {
            tet0 = triangulation->getTetrahedronIndex(
                (*fit)->getEmbedding(0).getTetrahedron());
            tet1 = triangulation->getTetrahedronIndex(
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

NCompConstraintSet* NNormalSurfaceVectorStandard::makeEmbeddedConstraints(
        NTriangulation* triangulation) {
    NCompConstraintSet* ans = new NCompConstraintSet();
    NCompConstraint* constraint;

    unsigned i;
    unsigned long base = 0;
    for (unsigned long tet = 0; tet < triangulation->getNumberOfTetrahedra();
            tet++) {
        constraint = new NCompConstraint(1);
        for (i = 4; i < 7; i++)
            constraint->getCoordinates().insert(
                constraint->getCoordinates().end(), base + i);
        base += 7;

        ans->push_back(constraint);
    }

    return ans;
}

} // namespace regina

