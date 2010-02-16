
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

#include "dim4/dim4edge.h"
#include "dim4/dim4face.h"
#include "dim4/dim4triangulation.h"
#include "enumerate/nenumconstraint.h"
#include "hypersurface/nhsstandard.h"
#include "maths/nmatrixint.h"
#include "maths/nrational.h"
#include "maths/nvectorunit.h"
#include "surfaces/nnormalsurface.h" // for vertexSplitDefn[][].

namespace regina {

NLargeInteger NNormalHypersurfaceVectorStandard::getEdgeWeight(
        unsigned long edgeIndex, Dim4Triangulation* triang) const {
    // Find a pentachoron next to the edge in question.
    const Dim4EdgeEmbedding& emb = triang->getEdges()[edgeIndex]->
        getEmbeddings().front();
    long pentIndex = triang->pentachoronIndex(emb.getPentachoron());
    int start = emb.getVertices()[0];
    int end = emb.getVertices()[1];

    // Add up the tetrahedra and prisms meeting that edge.
    // Tetrahedra:
    NLargeInteger ans((*this)[15 * pentIndex + start]);
    ans += (*this)[15 * pentIndex + end];
    // Prisms:
    unsigned e = Dim4Edge::edgeNumber[start][end];
    for (unsigned i = 0; i < 3; ++i) {
        ans += (*this)[15 * pentIndex + 5 +
            Dim4Edge::edgeNumber[start][Dim4Face::faceVertex[e][i]]];
        ans += (*this)[15 * pentIndex + 5 +
            Dim4Edge::edgeNumber[end][Dim4Face::faceVertex[e][i]]];
    }
    return ans;
}

NNormalHypersurfaceVector* NNormalHypersurfaceVectorStandard::makeZeroVector(
        const Dim4Triangulation* triangulation) {
    return new NNormalHypersurfaceVectorStandard(
        15 * triangulation->getNumberOfPentachora());
}

NMatrixInt* NNormalHypersurfaceVectorStandard::makeMatchingEquations(
        Dim4Triangulation* triangulation) {
    unsigned long nCoords = 15 * triangulation->getNumberOfPentachora();
    // Seven equations per non-boundary face.
    // T_boundary + 2 T_internal = 5 P
    long nEquations = 7 * (5 * long(triangulation->getNumberOfPentachora()) -
        long(triangulation->getNumberOfTetrahedra()));
    NMatrixInt* ans = new NMatrixInt(nEquations, nCoords);

    // Run through each internal facet and add the corresponding seven
    // equations.
    unsigned row = 0;
    int i;
    unsigned long pent0, pent1;
    NPerm5 perm0, perm1;
    for (Dim4Triangulation::TetrahedronIterator tit =
            triangulation->getTetrahedra().begin();
            tit != triangulation->getTetrahedra().end(); tit++) {
        if (! (*tit)->isBoundary()) {
            pent0 = triangulation->pentachoronIndex(
                (*tit)->getEmbedding(0).getPentachoron());
            pent1 = triangulation->pentachoronIndex(
                (*tit)->getEmbedding(1).getPentachoron());
            perm0 = (*tit)->getEmbedding(0).getVertices();
            perm1 = (*tit)->getEmbedding(1).getVertices();

            // Triangles:
            for (i=0; i<4; i++) {
                // Tetrahedra that meet this triangle:
                ans->entry(row, 15 * pent0 + perm0[i]) += 1;
                ans->entry(row, 15 * pent1 + perm1[i]) -= 1;

                // Prisms that meet this triangle:
                ans->entry(row, 15 * pent0 + 5 +
                    Dim4Edge::edgeNumber[perm0[i]][perm0[4]]) += 1;
                ans->entry(row, 15 * pent1 + 5 +
                    Dim4Edge::edgeNumber[perm1[i]][perm1[4]]) -= 1;
                row++;
            }

            // Quads:
            for (i=0; i<3; i++) {
                // Prisms that meet this quad:
                ans->entry(row, 15 * pent0 + 5 +
                    Dim4Edge::edgeNumber[perm0[vertexSplitDefn[i][0]]]
                                        [perm0[vertexSplitDefn[i][1]]]) += 1;
                ans->entry(row, 15 * pent0 + 5 +
                    Dim4Edge::edgeNumber[perm0[vertexSplitDefn[i][2]]]
                                        [perm0[vertexSplitDefn[i][3]]]) += 1;
                ans->entry(row, 15 * pent1 + 5 +
                    Dim4Edge::edgeNumber[perm1[vertexSplitDefn[i][0]]]
                                        [perm1[vertexSplitDefn[i][1]]]) -= 1;
                ans->entry(row, 15 * pent1 + 5 +
                    Dim4Edge::edgeNumber[perm1[vertexSplitDefn[i][2]]]
                                        [perm1[vertexSplitDefn[i][3]]]) -= 1;
                row++;
            }
        }
    }
    return ans;
}

NEnumConstraintList* NNormalHypersurfaceVectorStandard::makeEmbeddedConstraints(
        Dim4Triangulation* triangulation) {
    NEnumConstraintList* ans = new NEnumConstraintList(
        30 * triangulation->getNumberOfPentachora());

    unsigned base = 5;
    unsigned edge1, edge2;
    unsigned c = 0;
    unsigned pent, i;
    for (pent = 0; pent < triangulation->getNumberOfPentachora(); ++pent) {
        for (edge1 = 0; edge1 < 10; ++edge1)
            for (i = 0; i < 3; ++i) {
                edge2 = Dim4Edge::edgeNumber[
                    Dim4Edge::edgeVertex[edge1][0]][
                    Dim4Face::faceVertex[edge1][i]];
                if (edge1 < edge2) {
                    (*ans)[c].insert((*ans)[c].end(), base + edge1);
                    (*ans)[c].insert((*ans)[c].end(), base + edge2);
                    ++c;
                }
                edge2 = Dim4Edge::edgeNumber[
                    Dim4Edge::edgeVertex[edge1][1]][
                    Dim4Face::faceVertex[edge1][i]];
                if (edge1 < edge2) {
                    (*ans)[c].insert((*ans)[c].end(), base + edge1);
                    (*ans)[c].insert((*ans)[c].end(), base + edge2);
                    ++c;
                }
            }
        base += 15;
    }

    return ans;
}

} // namespace regina

