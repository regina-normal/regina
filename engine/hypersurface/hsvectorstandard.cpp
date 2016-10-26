
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
#include "hypersurface/hsvectorstandard.h"
#include "maths/matrix.h"
#include "maths/rational.h"
#include "surfaces/normalsurface.h" // for quadDefn[][].
#include "triangulation/dim4.h"

namespace regina {

LargeInteger HSVectorStandard::edgeWeight(
        size_t edgeIndex, const Triangulation<4>* triang) const {
    // Find a pentachoron next to the edge in question.
    const EdgeEmbedding<4>& emb = triang->edge(edgeIndex)->front();
    long pentIndex = emb.pentachoron()->index();
    int start = emb.vertices()[0];
    int end = emb.vertices()[1];

    // Add up the tetrahedra and prisms meeting that edge.
    // Tetrahedra:
    LargeInteger ans(coords_[15 * pentIndex + start]);
    ans += coords_[15 * pentIndex + end];
    // Prisms:
    unsigned e = Edge<4>::edgeNumber[start][end];
    for (unsigned i = 0; i < 3; ++i) {
        ans += coords_[15 * pentIndex + 5 +
            Edge<4>::edgeNumber[start][Triangle<4>::triangleVertex[e][i]]];
        ans += coords_[15 * pentIndex + 5 +
            Edge<4>::edgeNumber[end][Triangle<4>::triangleVertex[e][i]]];
    }
    return ans;
}

NormalHypersurfaceVector* HSVectorStandard::makeZeroVector(
        const Triangulation<4>* triangulation) {
    return new HSVectorStandard(15 * triangulation->size());
}

MatrixInt* HSVectorStandard::makeMatchingEquations(
        const Triangulation<4>* triangulation) {
    size_t nCoords = 15 * triangulation->size();
    // Seven equations per non-boundary facet.
    // T_boundary + 2 T_internal = 5 P
    long nEquations = 7 * (5 * long(triangulation->size()) -
        long(triangulation->countTetrahedra()));
    MatrixInt* ans = new MatrixInt(nEquations, nCoords);

    // Run through each internal facet and add the corresponding seven
    // equations.
    size_t row = 0;
    int i;
    size_t pent0, pent1;
    Perm<5> perm0, perm1;
    for (Triangulation<4>::TetrahedronIterator tit =
            triangulation->tetrahedra().begin();
            tit != triangulation->tetrahedra().end(); tit++) {
        if (! (*tit)->isBoundary()) {
            pent0 = (*tit)->embedding(0).pentachoron()->index();
            pent1 = (*tit)->embedding(1).pentachoron()->index();
            perm0 = (*tit)->embedding(0).vertices();
            perm1 = (*tit)->embedding(1).vertices();

            // Triangles:
            for (i=0; i<4; i++) {
                // Tetrahedra that meet this triangle:
                ans->entry(row, 15 * pent0 + perm0[i]) += 1;
                ans->entry(row, 15 * pent1 + perm1[i]) -= 1;

                // Prisms that meet this triangle:
                ans->entry(row, 15 * pent0 + 5 +
                    Edge<4>::edgeNumber[perm0[i]][perm0[4]]) += 1;
                ans->entry(row, 15 * pent1 + 5 +
                    Edge<4>::edgeNumber[perm1[i]][perm1[4]]) -= 1;
                row++;
            }

            // Quads:
            for (i=0; i<3; i++) {
                // Prisms that meet this quad:
                ans->entry(row, 15 * pent0 + 5 +
                    Edge<4>::edgeNumber[perm0[quadDefn[i][0]]]
                                        [perm0[quadDefn[i][1]]]) += 1;
                ans->entry(row, 15 * pent0 + 5 +
                    Edge<4>::edgeNumber[perm0[quadDefn[i][2]]]
                                        [perm0[quadDefn[i][3]]]) += 1;
                ans->entry(row, 15 * pent1 + 5 +
                    Edge<4>::edgeNumber[perm1[quadDefn[i][0]]]
                                        [perm1[quadDefn[i][1]]]) -= 1;
                ans->entry(row, 15 * pent1 + 5 +
                    Edge<4>::edgeNumber[perm1[quadDefn[i][2]]]
                                        [perm1[quadDefn[i][3]]]) -= 1;
                row++;
            }
        }
    }
    return ans;
}

EnumConstraints* HSVectorStandard::makeEmbeddedConstraints(
        const Triangulation<4>* triangulation) {
    EnumConstraints* ans = new EnumConstraints(30 * triangulation->size());

    size_t base = 5;
    unsigned edge1, edge2;
    size_t c = 0;
    size_t pent;
    unsigned i;
    for (pent = 0; pent < triangulation->size(); ++pent) {
        for (edge1 = 0; edge1 < 10; ++edge1)
            for (i = 0; i < 3; ++i) {
                edge2 = Edge<4>::edgeNumber[
                    Edge<4>::edgeVertex[edge1][0]][
                    Triangle<4>::triangleVertex[edge1][i]];
                if (edge1 < edge2) {
                    (*ans)[c].insert((*ans)[c].end(), base + edge1);
                    (*ans)[c].insert((*ans)[c].end(), base + edge2);
                    ++c;
                }
                edge2 = Edge<4>::edgeNumber[
                    Edge<4>::edgeVertex[edge1][1]][
                    Triangle<4>::triangleVertex[edge1][i]];
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

