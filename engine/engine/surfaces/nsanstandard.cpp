
/**************************************************************************
 *                                                                        *
 *  Regina - A normal surface theory calculator                           *
 *  Computational engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "nsanstandard.h"
    #include "nrational.h"
    #include "nmatrixint.h"
    #include "ntriangulation.h"
#else
    #include "engine/surfaces/nsanstandard.h"
    #include "engine/utilities/nrational.h"
    #include "engine/maths/nmatrixint.h"
    #include "engine/triangulation/ntriangulation.h"
#endif

bool NNormalSurfaceVectorANStandard::isCompatibleWith(
        const NNormalSurfaceVector& other) const {
    unsigned base = 4;
    int quad, oct;
    bool foundQuads;
        // Have we already found a quad type in this tetrahedron?
    bool foundOcts = false;
        // Have we already found an oct type anywhere?
    while (base < size()) {
        // Check that each tetrahedron has at most one quad type.
        foundQuads = false;
        for (quad = 0; quad < 3; quad++) {
            if ((*this)[base + quad] != 0 || other[base + quad] != 0) {
                if (foundQuads)
                    return false;
                else
                    foundQuads = true;
            }
        }
        // Look for octahedra.
        // An octahedral type is incompatible with any other octahedral
        // type, as well as any quad type in this tetrahedron.
        for (oct = 0; oct < 3; oct++) {
            if ((*this)[base + 3 + oct] != 0 || other[base + 3 + oct] != 0) {
                if (foundOcts || foundQuads)
                    return false;
                else
                    foundOcts = true;
            }
        }
        base += 10;
    }
    return true;
}

NLargeInteger NNormalSurfaceVectorANStandard::getEdgeWeight(
        unsigned long edgeIndex, NTriangulation* triang) const {
    // Find a tetrahedron next to the edge in question.
    const NEdgeEmbedding& emb = triang->getEdges()[edgeIndex]->
        getEmbeddings()[0];
    long tetIndex = triang->getTetrahedronIndex(
        emb.getTetrahedron());
    int start = emb.getVertices()[0];
    int end = emb.getVertices()[1];

    // Add up the triangles, quads and octahedra meeting that edge.
    // Triangles:
    NLargeInteger ans((*this)[10 * tetIndex + start]);
    ans += (*this)[10 * tetIndex + end];
    // Quads:
    ans += (*this)[10 * tetIndex + 4 + vertexSplitMeeting[start][end][0]];
    ans += (*this)[10 * tetIndex + 4 + vertexSplitMeeting[start][end][1]];
    // Octahedra:
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
    long tetIndex = triang->getTetrahedronIndex(
        emb.getTetrahedron());
    int vertex = emb.getVertices()[faceVertex];
    int backOfFace = emb.getVertices()[3];

    // Add up the discs meeting that face in that required arc.
    // Triangles:
    NLargeInteger ans((*this)[10 * tetIndex + vertex]);
    // Quads:
    ans += (*this)[10 * tetIndex + 4 + vertexSplit[vertex][backOfFace]];
    // Octahedra:
    ans += (*this)[10 * tetIndex + 7 +
        vertexSplitMeeting[vertex][backOfFace][0]];
    ans += (*this)[10 * tetIndex + 7 +
        vertexSplitMeeting[vertex][backOfFace][1]];
    return ans;
}

NDoubleList<NNormalSurfaceVector*>* NNormalSurfaceVectorANStandard::
        createNonNegativeCone(NTriangulation* triangulation) {
    unsigned long nCoords = 10 * triangulation->getNumberOfTetrahedra();
    NDoubleList<NNormalSurfaceVector*>* ans =
        new NDoubleList<NNormalSurfaceVector*>;
    NNormalSurfaceVector* vector;
    for (unsigned long i=0; i<nCoords; i++) {
        vector = new NNormalSurfaceVectorANStandard(nCoords);
        vector->setElement(i, NLargeInteger::one);
        ans->addLast(vector);
    }
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
    NTriangulation::FaceIterator fit(triangulation->getFaces());
    unsigned row = 0;
    int i;
    unsigned long tet0, tet1;
    NPerm perm0, perm1;
    while (! fit.done()) {
        if (! (*fit)->isBoundary()) {
            tet0 = triangulation->getTetrahedronIndex(
                (*fit)->getEmbedding(0).getTetrahedron());
            tet1 = triangulation->getTetrahedronIndex(
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
                // Octahedra:
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
        fit++;
    }
    return ans;
}

