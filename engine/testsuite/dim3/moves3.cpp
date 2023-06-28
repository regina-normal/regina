
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
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

#include "manifold/simplesurfacebundle.h"
#include "triangulation/dim3.h"
#include "triangulation/example3.h"

#include "testhelper.h"

using regina::SimpleSurfaceBundle;
using regina::Triangulation;

static void verifyTwoZeroEdge(Triangulation<3> tri, size_t whichEdge,
        const Triangulation<3>& result, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    EXPECT_TRUE(tri.twoZeroMove(tri.edge(whichEdge)));
    EXPECT_TRUE(tri.isIsomorphicTo(result));
    // Note: in all but one of our tests, the triangulations are not just
    // isomorphic but also identical.  (The exception is internal-flat-lens.)
}

TEST(Moves3Test, twoZeroEdge) {
    // A one-boundary-face case that Regina 4.1 used to crash on.
    verifyTwoZeroEdge(Triangulation<3>::fromGluings(5, {
            { 0, 1, 2, {3,0,1,2} }, { 0, 2, 4, {3,0,2,1} },
            { 0, 3, 4, {3,0,2,1} }, { 1, 2, 2, {0,1,3,2} },
            { 1, 3, 4, {2,1,3,0} }, { 2, 1, 3, {0,2,3,1} },
            { 2, 2, 3, {2,1,0,3} }, { 3, 3, 4, {0,1,2,3} }}),
        0, Triangulation<3>::fromGluings(3, {
            { 0, 2, 1, {0,1,3,2} }, { 0, 3, 1, {1,2,3,0} },
            { 1, 1, 2, {0,2,3,1} }, { 1, 2, 2, {2,1,0,3} }}),
        "one-boundary-face");

    verifyTwoZeroEdge(Triangulation<3>::fromGluings(3, {
            // Two tetrahedra glued along a degree two edge:
            { 0, 0, 1, {} }, { 0, 1, 1, {} }, // tet 0 <-> tet 1 on edge 23
            // Fold the other two faces of tetrahedron 0 together:
            { 0, 2, 0, {2,3} },
            // Attach an extra tetrahedron to one of the two boundary faces:
            { 1, 2, 2, {} }}),
        3, Triangulation<3>::fromGluings(1, {}), "boundary-loop-tet");

    // A degree two edge gadget with two boundary faces, and the others
    // attached to the top of a layered solid torus.
    {
        Triangulation<3> orig = regina::Example<3>::lst(4, 7);
        regina::Tetrahedron<3>* top = orig.newTetrahedron();
        orig.tetrahedron(0)->join(2, top, {2,3,0,1});
        orig.tetrahedron(0)->join(3, top, {2,3,0,1});

        verifyTwoZeroEdge(std::move(orig), 0,
            regina::Example<3>::lst(3, 4), "boundary-layer");
    }

    // A degree two edge gadget wedged between two adjacent internal faces in
    // a LST(3,4,7).
    verifyTwoZeroEdge(Triangulation<3>::fromGluings(5, {
            { 0, 0, 1, {2,1,3,0} }, { 0, 1, 1, {0,3,1,2} },
            { 1, 0, 4, {3,1,2,0} }, { 1, 1, 4, {0,2,1,3} },
            { 2, 0, 2, {1,2,3,0} }, { 2, 2, 3, {0,1,2,3} },
            { 2, 3, 3, {0,1,2,3} }, { 3, 0, 4, {0,1,2,3} },
            { 3, 1, 4, {0,1,2,3} }}),
        5, regina::Example<3>::lst(3, 4), "internal-flat");

    // A degree two edge gadget wedged into the tip of a layered L(10,3),
    // with two faces joined in a loop.  We reuse this triangulation a second
    // time, since we can flatten around two different degree two edges.
    {
        Triangulation<3> orig = Triangulation<3>::fromGluings(5, {
            { 0, 0, 1, {2,1,3,0} }, { 0, 1, 1, {0,3,1,2} },
            { 0, 2, 3, {0,1,2,3} }, { 0, 3, 3, {0,1,2,3} },
            { 1, 0, 2, {3,1,2,0} }, { 1, 1, 2, {0,2,1,3} },
            { 2, 0, 2, {1,2,3,0} }, { 3, 0, 4, {0,1,2,3} },
            { 3, 1, 4, {0,1,2,3} }, { 4, 2, 4, {1,2,3,0} }});
        Triangulation<3> lens = regina::Example<3>::lens(10, 3);

        verifyTwoZeroEdge(orig, 5, lens, "internal-loop-twist");
        verifyTwoZeroEdge(std::move(orig), 0, lens, "internal-flat-lens");
    }

    // Two solid Klein bottles glued along a single face.
    verifyTwoZeroEdge(Triangulation<3>::fromGluings(4, {
            // Two tetrahedra glued along a degree two edge, with two of the
            // outer faces glued together to form a solid Klein bottle:
            { 0, 0, 1, {} }, { 0, 1, 1, {} }, { 0, 2, 1, {1,2,3,0} },
            // A second copy of this solid Klein bottle:
            { 2, 0, 3, {} }, { 2, 1, 3, {} }, { 2, 2, 3, {1,2,3,0} },
            // Attach the two solid Klein bottles along a single face:
            { 0, 3, 2, {} }}),
        3, Triangulation<3>::fromGluings(2, {
            // Just a single solid Klein bottle:
            { 0, 0, 1, {} }, { 0, 1, 1, {} }, { 0, 2, 1, {1,2,3,0} }}),
        "bdry-cross-tet");

    // A hand-constructed example formed by squeezing the same solid Klein
    // bottle from above into a two-sided Mobius band face in K(iii | 1,0 | 0,1)
    // from the 6-tetrahedron non-orientable census.
    verifyTwoZeroEdge(Triangulation<3>::fromGluings(8, {
            { 0, 0, 1, {0,2,1,3} }, { 0, 1, 1, {2,1,3,0} },
            { 0, 2, 2, {1,3,0,2} }, { 0, 3, 2, {3,0,2,1} },
            { 1, 2, 6, {2,0,3,1} }, { 1, 3, 3, {2,3,1,0} },
            { 2, 2, 7, {1,3,2,0} }, { 2, 3, 4, {2,3,1,0} },
            { 3, 1, 4, {0,1,2,3} }, { 3, 2, 5, {3,1,0,2} },
            { 3, 3, 5, {2,0,3,1} }, { 4, 2, 5, {1,3,2,0} },
            { 4, 3, 5, {0,2,1,3} }, { 6, 0, 7, {0,1,2,3} },
            { 6, 1, 7, {0,1,2,3} }, { 6, 2, 7, {1,2,3,0} }}),
        8, Triangulation<3>::fromGluings(6, {
            { 0, 0, 1, {0,2,1,3} }, { 0, 1, 1, {2,1,3,0} },
            { 0, 2, 2, {1,3,0,2} }, { 0, 3, 2, {3,0,2,1} },
            { 1, 2, 2, {0,1,2,3} }, { 1, 3, 3, {2,3,1,0} },
            { 2, 3, 4, {2,3,1,0} }, { 3, 1, 4, {0,1,2,3} },
            { 3, 2, 5, {3,1,0,2} }, { 3, 3, 5, {2,0,3,1} },
            { 4, 2, 5, {1,3,2,0} }, { 4, 3, 5, {0,2,1,3} }}),
        "internal-cross");
}

/**
 * This is for cases that have an internal degree two edge, but where the
 * corresponding 2-0 move is not allowed.
 */
static void verifyTwoZeroEdgeInvalid(const Triangulation<3>& tri,
        const char* name) {
    SCOPED_TRACE_CSTRING(name);

    bool found = false;
    for (auto e : tri.edges()) {
        if (e->degree() == 2 && ! e->isBoundary())
            found = true;
        // Use a const_cast for now: the move should be illegal.
        ASSERT_FALSE(const_cast<Triangulation<3>&>(tri).twoZeroMove(e));
    }
    EXPECT_TRUE(found);
}

TEST(Moves3Test, twoZeroEdgeInvalid) {
    // A degree two edge gadget, with all four outer faces joined together in a
    // simple loop.
    verifyTwoZeroEdgeInvalid(
        SimpleSurfaceBundle(SimpleSurfaceBundle::S2xS1).construct(),
        "round-loop");

    // A degree two edge gadget, with all four outer faces joined together in a
    // crossed loop.
    verifyTwoZeroEdgeInvalid(
        SimpleSurfaceBundle(SimpleSurfaceBundle::S2xS1_TWISTED)
            .construct(),
        "crossed-loop");

    // A degree two edge gadget, with standalone tetrahedra attached to each
    // of the four boundary faces (thus the bigon that would normally be
    // flattened forms a properly embedded disc that separates the manifold
    // into two pieces).
    verifyTwoZeroEdgeInvalid(Triangulation<3>::fromGluings(6, {
            { 0, 0, 1, {} }, { 0, 1, 1, {} },
            { 0, 2, 2, {} }, { 0, 3, 3, {} },
            { 1, 2, 4, {} }, { 1, 3, 5, {} }}),
        "boundary-edges");

    // A degree two edge gadget, with all four outer faces identified so that
    // the bigon that would normally be flattened forms a 2-sphere.
    verifyTwoZeroEdgeInvalid(Triangulation<3>::fromGluings(6, {
            { 0, 0, 1, {} }, { 0, 1, 1, {} },
            { 0, 2, 2, {} }, { 0, 3, 3, {} }, { 1, 2, 4, {} },
            { 1, 3, 5, {} }, { 2, 3, 4, {} }, { 3, 2, 5, {} }}),
        "identified-edges-S2");

    // A degree two edge gadget, with all four outer faces identified so that
    // the bigon that would normally be flattened forms a projective plane.
    verifyTwoZeroEdgeInvalid(Triangulation<3>::fromGluings(6, {
            { 0, 0, 1, {} }, { 0, 1, 1, {} },
            { 0, 2, 2, {} }, { 0, 3, 3, {} }, { 1, 2, 4, {} },
            { 1, 3, 5, {} }, { 2, 3, 4, {0,1} }, { 3, 2, 5, {0,1} }}),
        "identified-edges-RP2");

    // A degree two edge gadget, with two adjacent outer faces glued together
    // in a loop.
    verifyTwoZeroEdgeInvalid(Triangulation<3>::fromGluings(2, {
            { 0, 0, 1, {} }, { 0, 1, 1, {} }, { 0, 2, 0, {2,3} }}),
        "boundary-loop-boundary");

    // A degree two edge gadget, with two diagonally opposite outer faces
    // glued together to form a solid Klein bottle.
    verifyTwoZeroEdgeInvalid(Triangulation<3>::fromGluings(2, {
            { 0, 0, 1, {} }, { 0, 1, 1, {} }, { 0, 2, 1, {1,2,3,0} }}),
        "boundary-cross-boundary");

    // A degree two edge gadget with two opposite outer faces boundary,
    // and the other two wedged inside an LST(3,4,7).
    verifyTwoZeroEdgeInvalid(Triangulation<3>::fromGluings(5, {
            { 0, 0, 1, {2,1,3,0} }, { 0, 1, 1, {0,3,1,2} },
            { 1, 0, 4, {3,1,2,0} }, { 1, 1, 2, {0,2,1,3} },
            { 2, 0, 2, {1,2,3,0} }, { 2, 3, 3, {0,1,2,3} },
            { 3, 0, 4, {0,1,2,3} }, { 3, 1, 4, {0,1,2,3} }}),
        "boundary-opposite-wedge");

    // A degree two edge gadtet with two diagonally opposite outer faces
    // boundary, and the other two glued to an LST boundary.
    verifyTwoZeroEdgeInvalid(Triangulation<3>::fromGluings(5, {
            // LST(3,4,7):
            { 0, 0, 1, {2,1,3,0} }, { 0, 1, 1, {0,3,1,2} },
            { 1, 0, 2, {3,1,2,0} }, { 1, 1, 2, {0,2,1,3} },
            { 2, 0, 2, {1,2,3,0} },
            // A degree two edge gadget:
            { 3, 0, 4, {} }, { 3, 1, 4, {} },
            // Attach the two pieces together:
            { 3, 3, 0, {1,2,0,3} }, { 4, 2, 0, {0,3,2,1} }}),
        "boundary-diag");
}
