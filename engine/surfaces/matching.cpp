
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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
#include "surfaces/normalsurface.h"
#include "maths/matrix.h"
#include "snappea/snappeatriangulation.h"
#include "triangulation/dim3.h"

namespace regina {

EnumConstraints makeEmbeddedConstraints(const Triangulation<3>& triangulation,
        NormalCoords coords) {
    const NormalEncoding enc(coords);
    if (enc.storesOctagons()) {
        // At most one quad/oct per tetrahedron.
        // Also at most one oct type overall.
        EnumConstraints ans(triangulation.size() + 1);

        unsigned base = (enc.storesTriangles() ? 4 : 0);
        for (unsigned c = 1; c < ans.size(); ++c) {
            ans[c].insert(ans[c].end(), base);
            ans[c].insert(ans[c].end(), base + 1);
            ans[c].insert(ans[c].end(), base + 2);
            ans[c].insert(ans[c].end(), base + 3);
            ans[c].insert(ans[c].end(), base + 4);
            ans[c].insert(ans[c].end(), base + 5);

            ans[0].insert(ans[0].end(), base + 3);
            ans[0].insert(ans[0].end(), base + 4);
            ans[0].insert(ans[0].end(), base + 5);

            base += enc.block();
        }
        return ans;
    } else {
        // No octagon constraints.
        EnumConstraints ans(triangulation.size());

        unsigned base = (enc.storesTriangles() ? 4 : 0);
        for (unsigned c = 0; c < ans.size(); ++c) {
            ans[c].insert(ans[c].end(), base);
            ans[c].insert(ans[c].end(), base + 1);
            ans[c].insert(ans[c].end(), base + 2);

            base += enc.block();
        }
        return ans;
    }
}

std::optional<MatrixInt> makeMatchingEquations(
        const Triangulation<3>& triangulation, NormalCoords coords) {
    switch (coords) {
        case NS_STANDARD:
        case NS_AN_STANDARD:
        {
            const size_t block = (coords == NS_STANDARD ? 7 : 10);
            const size_t nCoords = block * triangulation.size();
            // Three equations per non-boundary triangle.
            // F_boundary + 2 F_internal = 4 T
            const size_t nEquations = 3 * (4 * triangulation.size() -
                triangulation.countTriangles());
            MatrixInt ans(nEquations, nCoords);

            // Run through each internal triangle and add the corresponding
            // three equations.
            size_t row = 0;
            for (Triangle<3>* t : triangulation.triangles()) {
                if (! t->isBoundary()) {
                    size_t pos0 = block * t->embedding(0).tetrahedron()->index();
                    size_t pos1 = block * t->embedding(1).tetrahedron()->index();
                    Perm<4> perm0 = t->embedding(0).vertices();
                    Perm<4> perm1 = t->embedding(1).vertices();
                    for (int i=0; i<3; i++) {
                        // Triangles:
                        ++ans.entry(row, pos0 + perm0[i]);
                        --ans.entry(row, pos1 + perm1[i]);
                        // Quads:
                        ++ans.entry(row, pos0 + 4 +
                            quadSeparating[perm0[i]][perm0[3]]);
                        --ans.entry(row, pos1 + 4 +
                            quadSeparating[perm1[i]][perm1[3]]);
                        // Octagons:
                        if (coords == NS_AN_STANDARD) {
                            ++ans.entry(row, pos0 + 7 +
                                quadMeeting[perm0[i]][perm0[3]][0]);
                            --ans.entry(row, pos1 + 7 +
                                quadMeeting[perm1[i]][perm1[3]][0]);
                            ++ans.entry(row, pos0 + 7 +
                                quadMeeting[perm0[i]][perm0[3]][1]);
                            --ans.entry(row, pos1 + 7 +
                                quadMeeting[perm1[i]][perm1[3]][1]);
                        }
                        ++row;
                    }
                }
            }
            return ans;
        }
        case NS_QUAD:
        case NS_AN_QUAD_OCT:
        {
            const size_t block = (coords == NS_QUAD ? 3 : 6);
            const size_t nCoords = block * triangulation.size();
            // One equation per non-boundary edge.
            size_t nEquations = triangulation.countEdges();
            for (BoundaryComponent<3>* bc : triangulation.boundaryComponents())
                nEquations -= bc->countEdges();

            MatrixInt ans(nEquations, nCoords);

            // Run through each internal edge and add the corresponding
            // equation.
            size_t row = 0;
            for (Edge<3>* e : triangulation.edges()) {
                if (! e->isBoundary()) {
                    for (const auto& emb : *e) {
                        size_t pos = block * emb.tetrahedron()->index();
                        Perm<4> perm = emb.vertices();
                        ++ans.entry(row, pos +
                            quadSeparating[perm[0]][perm[2]]);
                        --ans.entry(row, pos +
                            quadSeparating[perm[0]][perm[3]]);
                        if (coords == NS_AN_QUAD_OCT) {
                            --ans.entry(row, pos + 3 +
                                quadSeparating[perm[0]][perm[2]]);
                            ++ans.entry(row, pos + 3 +
                                quadSeparating[perm[0]][perm[3]]);
                        }
                    }
                    ++row;
                }
            }
            return ans;
        }
        case NS_QUAD_CLOSED:
        case NS_AN_QUAD_OCT_CLOSED:
        {
            // Enforce our basic preconditions.
            if (! (triangulation.isOriented() && triangulation.isIdeal() &&
                    triangulation.countBoundaryComponents() == 1 &&
                    triangulation.countVertices() == 1 &&
                    triangulation.vertex(0)->linkType() == Vertex<3>::TORUS))
                return std::nullopt;

            // We will use SnapPea to build the additional constraint that
            // enforce closed surfaces.  Before doing anything else, see whether
            // SnapPea is going to play along.
            SnapPeaTriangulation snapPea(triangulation, false);
            MatrixInt coeffs;
            try {
                coeffs = snapPea.slopeEquations();
            } catch (const regina::FailedPrecondition&) {
                // SnapPea couldn't handle it.
                return std::nullopt;
            }
            if (! snapPea.isIdenticalTo(triangulation)) {
                // SnapPea retriangulated.
                return std::nullopt;
            }

            const size_t block = (coords == NS_QUAD_CLOSED ? 3 : 6);
            const size_t nCoords = block * triangulation.size();
            // One equation per edge, plus two per ideal vertex.
            // (This code is written a little more generically, in order to
            // support multiple ideal vertices at some later date.)
            const size_t nEquations = triangulation.countEdges() +
                2 * triangulation.countBoundaryComponents();

            MatrixInt ans(nEquations, nCoords);
            size_t row = 0;

            // Run through each internal edge and add the corresponding
            // equation.
            for (Edge<3>* e : triangulation.edges()) {
                for (auto& emb : *e) {
                    size_t pos = block * emb.tetrahedron()->index();
                    Perm<4> perm = emb.vertices();
                    ++ans.entry(row, pos +
                        quadSeparating[perm[0]][perm[2]]);
                    --ans.entry(row, pos +
                        quadSeparating[perm[0]][perm[3]]);
                    if (coords == NS_AN_QUAD_OCT_CLOSED) {
                        --ans.entry(row, pos + 3 +
                            quadSeparating[perm[0]][perm[2]]);
                        ++ans.entry(row, pos + 3 +
                            quadSeparating[perm[0]][perm[3]]);
                    }
                }
                ++row;
            }
            // Run through each ideal vertex and add the corresponding meridian
            // and longitude equations.
            //
            // The coefficients here are differences of terms from
            // SnapPy's get_cusp_equation(), which works in native
            // integers; therefore we will happily convert them back
            // to native integers now.
            //
            // Note: from preconditions, #vertices == #boundaries.
            for (size_t i = 0; i < triangulation.countVertices(); ++i) {
                // These two branches could be merged a bit better.
                // Note: the cusp equations are always expressed in terms of
                // quad coordinates.
                if (coords == NS_QUAD_CLOSED) {
                    for (int j = 0; j < 3 * triangulation.size(); ++j) {
                        ans.entry(row, j) = coeffs.entry(2 * i, j);
                        ans.entry(row + 1, j) = coeffs.entry(2 * i + 1, j);
                    }
                } else {
                    for (int j = 0; j < triangulation.size(); ++j) {
                        for (int k = 0; k < 3; ++k){
                            // Quad contributions
                            ans.entry(row, 6*j + k) =
                                coeffs.entry(2 * i, 3*j + k);
                            ans.entry(row + 1, 6*j + k) =
                                coeffs.entry(2 * i + 1, 3*j + k);
                            // Oct contributions; signs are opposite of those
                            // for the quads as with the edge equations.
                            ans.entry(row, 6*j + 3 + k) =
                                -coeffs.entry(2 * i, 3*j + k);
                            ans.entry(row + 1, 6*j + 3 + k) =
                                -coeffs.entry(2 * i + 1, 3*j + k);
                        }
                    }
                }
                row += 2;
            }
            return ans;
        }
        default:
            return std::nullopt;
    }
}

} // namespace regina

