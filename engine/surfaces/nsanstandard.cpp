
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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

#include "enumerate/ncompconstraint.h"
#include "surfaces/nsanstandard.h"
#include "utilities/nrational.h"
#include "maths/nmatrixint.h"
#include "maths/nvectorunit.h"
#include "triangulation/ntriangulation.h"

namespace regina {

NLargeInteger NNormalSurfaceVectorANStandard::getEdgeWeight(
        unsigned long edgeIndex, NTriangulation* triang) const {
    // Find a tetrahedron next to the edge in question.
    const NEdgeEmbedding& emb = triang->getEdges()[edgeIndex]->
        getEmbeddings().front();
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

NLargeInteger NNormalSurfaceVectorANStandard::getFaceArcs(
        unsigned long faceIndex, int faceVertex, NTriangulation* triang) const {
    // Find a tetrahedron next to the face in question.
    const NFaceEmbedding& emb = triang->getFaces()[faceIndex]->
        getEmbedding(0);
    long tetIndex = triang->tetrahedronIndex(emb.getTetrahedron());
    int vertex = emb.getVertices()[faceVertex];
    int backOfFace = emb.getVertices()[3];

    // Add up the discs meeting that face in that required arc.
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

NMatrixInt* NNormalSurfaceVectorANStandard::makeMatchingEquations(
        NTriangulation* triangulation) {
    unsigned long nCoords = 10 * triangulation->getNumberOfTetrahedra();
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
            tet0 = triangulation->tetrahedronIndex(
                (*fit)->getEmbedding(0).getTetrahedron());
            tet1 = triangulation->tetrahedronIndex(
                (*fit)->getEmbedding(1).getTetrahedron());
            perm0 = (*fit)->getEmbedding(0).getVertices();
            perm1 = (*fit)->getEmbedding(1).getVertices();
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

NCompConstraintSet* NNormalSurfaceVectorANStandard::makeEmbeddedConstraints(
        NTriangulation* triangulation) {
    // At most one quad/oct per tetrahedron.
    // Also at most one oct type overall.
    NCompConstraintSet* ans = new NCompConstraintSet();
    NCompConstraint* globalOctConstraint = new NCompConstraint();
    std::set<unsigned>& globalOctCoords(globalOctConstraint->getCoordinates());
    NCompConstraint* constraint;

    unsigned i;
    unsigned long base = 0;
    for (unsigned long tet = 0; tet < triangulation->getNumberOfTetrahedra();
            tet++) {
        constraint = new NCompConstraint();
        for (i = 4; i < 10; i++)
            constraint->getCoordinates().insert(
                constraint->getCoordinates().end(), base + i);
        for (i = 7; i < 10; i++)
            globalOctCoords.insert(globalOctCoords.end(), base + i);
        base += 10;

        ans->push_back(constraint);
    }

    ans->push_back(globalOctConstraint);
    return ans;
}

} // namespace regina

