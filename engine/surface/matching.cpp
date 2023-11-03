
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

#include "enumerate/validityconstraints.h"
#include "surface/normalsurface.h"
#include "maths/matrix.h"
#include "snappea/snappeatriangulation.h"
#include "triangulation/dim3.h"

namespace regina {

ValidityConstraints makeEmbeddedConstraints(
        const Triangulation<3>& triangulation, NormalCoords coords) {
    const NormalEncoding enc(coords);
    if (enc.storesOctagons()) {
        // At most one quad/oct per tetrahedron.
        // Also at most one oct type overall.
        ValidityConstraints ans(enc.block(), triangulation.size(), 1, 1);

        if (enc.storesTriangles()) {
            ans.addLocal({ 4, 5, 6, 7, 8, 9 });
            ans.addGlobal({ 7, 8, 9 });
        } else {
            ans.addLocal({ 0, 1, 2, 3, 4, 5 });
            ans.addGlobal({ 3, 4, 5 });
        }

        return ans;
    } else {
        // No octagon constraints.
        ValidityConstraints ans(enc.block(), triangulation.size(), 1);

        if (enc.storesTriangles())
            ans.addLocal({ 4, 5, 6 });
        else
            ans.addLocal({ 0, 1, 2 });

        return ans;
    }
}

MatrixInt makeMatchingEquations(const Triangulation<3>& triangulation,
        NormalCoords coords) {
    switch (coords) {
        case NormalCoords::Standard:
        case NormalCoords::AlmostNormal:
        {
            const size_t block = (coords == NormalCoords::Standard ? 7 : 10);
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
                        if (coords == NormalCoords::AlmostNormal) {
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
        case NormalCoords::Quad:
        case NormalCoords::QuadOct:
        {
            const size_t block = (coords == NormalCoords::Quad ? 3 : 6);
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
                        if (coords == NormalCoords::QuadOct) {
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
        case NormalCoords::QuadClosed:
        case NormalCoords::QuadOctClosed:
        {
            // Enforce our basic preconditions.
            if (! (triangulation.isOriented() && triangulation.isIdeal() &&
                    triangulation.countBoundaryComponents() == 1 &&
                    triangulation.countVertices() == 1 &&
                    triangulation.vertex(0)->linkType() == Vertex<3>::TORUS))
                throw InvalidArgument(
                    "NormalCoords::QuadClosed and NormalCoords::QuadOctClosed "
                    "require an oriented ideal triangulation with "
                    "precisely one torus cusp and no other vertices");

            // We will use SnapPea to build the additional constraint that
            // enforces closed surfaces.
            SnapPeaTriangulation snapPea(triangulation, false);
            if (snapPea.isNull())
                throw UnsolvedCase("SnapPea produced a null triangulation "
                    "when attempting to build the matching equations");

            MatrixInt coeffs = snapPea.slopeEquations();

            if (snapPea != triangulation)
                throw UnsolvedCase("SnapPea retriangulated "
                    "when attempting to build the matching equations");

            const size_t block = (coords == NormalCoords::QuadClosed ? 3 : 6);
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
                    if (coords == NormalCoords::QuadOctClosed) {
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
            // Note: from preconditions, #vertices == #boundaries.
            for (size_t i = 0; i < triangulation.countVertices(); ++i) {
                // These two branches could be merged a bit better.
                // Note: the cusp equations are always expressed in terms of
                // quad coordinates.
                if (coords == NormalCoords::QuadClosed) {
                    for (size_t j = 0; j < 3 * triangulation.size(); ++j) {
                        ans.entry(row, j) = coeffs.entry(2 * i, j);
                        ans.entry(row + 1, j) = coeffs.entry(2 * i + 1, j);
                    }
                } else {
                    for (size_t j = 0; j < triangulation.size(); ++j) {
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
            throw InvalidArgument("makeMatchingEquations() was given "
                "an invalid coordinate system");
    }
}

} // namespace regina

