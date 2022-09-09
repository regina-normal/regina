
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

#include <algorithm>
#include "algebra/abeliangroup.h"
#include "manifold/handlebody.h"
#include "manifold/lensspace.h"
#include "manifold/simplesurfacebundle.h"
#include "subcomplex/trivialtri.h"
#include "triangulation/dim3.h"

namespace regina {

std::unique_ptr<TrivialTri> TrivialTri::recognise(const Component<3>* comp) {
    // Since the triangulations are so small we can use census results
    // to recognise the triangulations by properties alone.

    // Are there any boundary components?
    if (! comp->isClosed()) {
        if (comp->countBoundaryComponents() == 1) {
            // We have precisely one boundary component.
            BoundaryComponent<3>* bc = comp->boundaryComponent(0);

            if (! bc->isIdeal()) {
                // The boundary component includes boundary triangles.
                // Look for a one-tetrahedron ball.
                if (comp->size() == 1) {
                    if (bc->countTriangles() == 4)
                        return std::unique_ptr<TrivialTri>(
                            new TrivialTri(BALL_4_VERTEX));
                    if (bc->countTriangles() == 2 && comp->countVertices() == 3)
                        return std::unique_ptr<TrivialTri>(
                            new TrivialTri(BALL_3_VERTEX));
                }
            }
        }

        // Not recognised.
        return nullptr;
    }

    // Otherwise we are dealing with a closed component.

    // Before we do our validity check, make sure the number of
    // tetrahedra is in the supported range.
    if (comp->size() > 3)
        return nullptr;

    // Is the triangulation valid?
    // Since the triangulations is closed we know that the vertices are
    // valid; all that remains is to check the edges.
    size_t nEdges = comp->countEdges();
    for (size_t i = 0; i < nEdges; i++)
        if (! comp->edge(i)->isValid())
            return nullptr;

    // Test for the specific triangulations that we know about.

    if (comp->size() == 2) {
        if (comp->isOrientable()) {
            if (comp->countVertices() == 4) {
                // There's only one closed valid two-tetrahedron
                // four-vertex orientable triangulation.
                return std::unique_ptr<TrivialTri>(
                    new TrivialTri(SPHERE_4_VERTEX));
            } else if (comp->countVertices() == 2) {
                // The census says we have one of three triangulations:
                // - cMcabbgig : S^3, edge degrees 6 4 1 1
                // - cPcbbbaai : L(3,1), edge degrees 6 2 2 2
                // - cPcbbbahh : RP^2, edge degrees 4 4 2 2
                //
                // The only one of these that *this* class is interested
                // in detecting is the L(3,1).
                for (const Edge<3>* e : comp->edges())
                    if (e->degree() == 4)
                        return nullptr;
                return std::unique_ptr<TrivialTri>(new TrivialTri(L31_PILLOW));
            }
        } else {
            // There's only one closed valid two-tetrahedron non-orientable
            // triangulation.
            return std::unique_ptr<TrivialTri>(new TrivialTri(N2));
        }
        return nullptr;
    }

    if (comp->size() == 3) {
        if (! comp->isOrientable()) {
            // If the triangulation is valid and the edge degrees
            // are 2,4,4,6 then we have N(3,1) or N(3,2).
            // All of the vertices are valid since there are no boundary
            // triangles; we thus only need to check the edges.
            if (comp->countEdges() != 4)
                return nullptr;

            size_t degree[4];
            for (int i = 0; i < 4; i++)
                degree[i] = comp->edge(i)->degree();
            std::sort(degree, degree + 4);

            if (degree[0] == 2 && degree[1] == 4 && degree[2] == 6 &&
                    degree[3] == 6) {
                // We have N(3,1) or N(3,2)!
                // Search for Mobius band triangles.
                size_t nTriangles = comp->countTriangles();
                for (size_t i = 0; i < nTriangles; i++)
                    if (comp->triangle(i)->isMobiusBand())
                        return std::unique_ptr<TrivialTri>(
                            new TrivialTri(N3_2));
                return std::unique_ptr<TrivialTri>(new TrivialTri(N3_1));
            }
        }
    }

    return nullptr;
}

std::unique_ptr<Manifold> TrivialTri::manifold() const {
    switch (type_) {
        case SPHERE_4_VERTEX:
            return std::make_unique<LensSpace>(1, 0);
        case BALL_3_VERTEX:
        case BALL_4_VERTEX:
            return std::make_unique<Handlebody>(0);
        case L31_PILLOW:
            return std::make_unique<LensSpace>(3, 1);
        case N2:
            return std::make_unique<SimpleSurfaceBundle>(
                SimpleSurfaceBundle::S2xS1_TWISTED);
        case N3_1:
        case N3_2:
            return std::make_unique<SimpleSurfaceBundle>(
                SimpleSurfaceBundle::RP2xS1);
        default:
            return nullptr;
    }
}

AbelianGroup TrivialTri::homology() const {
    switch (type_) {
        case L31_PILLOW:
            return AbelianGroup(0, {3});
        case N2:
            return AbelianGroup(1);
        case N3_1:
        case N3_2:
            return AbelianGroup(1, {2});
        default:
            return AbelianGroup();
    }
}

std::ostream& TrivialTri::writeName(std::ostream& out) const {
    switch (type_) {
        case SPHERE_4_VERTEX:
            out << "S3 (4-vtx)"; break;
        case BALL_3_VERTEX:
            out << "B3 (3-vtx)"; break;
        case BALL_4_VERTEX:
            out << "B3 (4-vtx)"; break;
        case L31_PILLOW:
            out << "L'(3,1)"; break;
        case N2:
            out << "N(2)"; break;
        case N3_1:
            out << "N(3,1)"; break;
        case N3_2:
            out << "N(3,2)"; break;
    }
    return out;
}
std::ostream& TrivialTri::writeTeXName(std::ostream& out) const {
    switch (type_) {
        case SPHERE_4_VERTEX:
            out << "S^3_{v=4}"; break;
        case BALL_3_VERTEX:
            out << "B^3_{v=3}"; break;
        case BALL_4_VERTEX:
            out << "B^3_{v=4}"; break;
        case L31_PILLOW:
            out << "L'_{3,1}"; break;
        case N2:
            out << "N_{2}"; break;
        case N3_1:
            out << "N_{3,1}"; break;
        case N3_2:
            out << "N_{3,2}"; break;
    }
    return out;
}
void TrivialTri::writeTextLong(std::ostream& out) const {
    switch (type_) {
        case SPHERE_4_VERTEX:
            out << "Two-tetrahedron four-vertex 3-sphere"; break;
        case BALL_3_VERTEX:
            out << "One-tetrahedron three-vertex ball"; break;
        case BALL_4_VERTEX:
            out << "One-tetrahedron four-vertex ball"; break;
        case L31_PILLOW:
            out << "Triangular pillow lens space L(3,1)"; break;
        case N2:
            out << "Non-orientable triangulation N(2)"; break;
        case N3_1:
            out << "Non-orientable triangulation N(3,1)"; break;
        case N3_2:
            out << "Non-orientable triangulation N(3,2)"; break;
    }
}

} // namespace regina

