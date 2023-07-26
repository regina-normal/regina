
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

#include <cmath>
#include "angle/anglestructures.h"
#include "link/link.h"
#include "manifold/simplesurfacebundle.h"
#include "snappea/snappeatriangulation.h"
#include "split/signature.h"
#include "subcomplex/standardtri.h"
#include "surface/normalsurfaces.h"
#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "triangulation/example3.h"

#include "generic/triangulationtest.h"
#include "testexhaustive.h"

// Pi only becomes a standard language constant in C++20.
// For now, use the non-standard M_PI (which requires the following #define).
// This is used in the Turaev-Viro approximation tests.
#define _USE_MATH_DEFINES

using regina::AbelianGroup;
using regina::Example;
using regina::Signature;
using regina::Triangulation;

class Dim3Test : public TriangulationTest<3> {
    protected:
        // Closed orientable triangulations:
        TestCase s3 { Example<3>::lens(1, 0), "Sphere (1 vtx)" };
        TestCase rp3_1 { Example<3>::lens(2, 1), "RP^3 (1 vtx)" };
        TestCase rp3_2 { Example<3>::layeredLoop(2, false), "RP^3 (2 vtx)" };
        TestCase lens3_1 { Triangulation<3>::fromGluings(2, {
                // Triangular pillow, boundary triangles glued via 1/3 rotation:
                { 0, 0, 1, {0,2,3,1} },
                { 0, 1, 1, {} }, { 0, 2, 1, {} }, { 0, 3, 1, {} }
            }), "L(3, 1) (2 vtx)" };
        TestCase lens8_3 { Example<3>::lens(8, 3), "L(8,3)" };
        TestCase lens7_1_loop { Example<3>::layeredLoop(7, false), "L(7,1)" };
        TestCase rp3rp3 { Signature("aabccd.b.d").triangulate(),
            "RP^3 # RP^3" };
        TestCase q32xz3 { Signature("aabcdb.cedfef").triangulate(),
            "S^3 / Q_32 x Z_3" };
        TestCase q28 { Example<3>::layeredLoop(7, true), "S^3 / Q_28" };
        TestCase weberSeifert { Example<3>::weberSeifert(), "Weber-Seifert" };
        TestCase lens100_1 { Example<3>::lens(100, 1), "L(100,1)" };
        TestCase s3_large { Signature("abc.abd.cef.de.fg.g").triangulate(),
            "S^3 (large)" };
        TestCase rp3_large { Signature("aabcdedcfb.fg.e.g").triangulate(),
            "RP^3 (large)" };
        TestCase lens8_3_large { Signature("aabcb.cd.d").triangulate(),
            "L(8,3) (large)" };
        TestCase q20_large { Signature("abcdeabcdef.fg.g").triangulate(),
            "S^3 / Q_20 (large)" };

        // Closed non-orientable triangulations:
        TestCase rp2xs1 { Example<3>::rp2xs1(), "RP^2 x S^1" };

        // Bounded orientable triangulations:
        TestCase lst3_4_7 { Example<3>::lst(3, 4), "LST(3,4,7)" };
        TestCase ball_bary { Triangulation<3>::fromGluings(1, {}),
            // We call subdivide() in the class constructor.
            "B^3 (subdivided)" };
        TestCase ball_large { Triangulation<3>::fromGluings(4, {
                // Long ago, this used to crash the simplification routines.
                { 0, 2, 0, {0,2} }, { 0, 1, 1, {2,0,1,3} },
                { 1, 2, 2, {} }, { 1, 1, 2, {2,0,1,3} },
                { 2, 1, 3, {2,0,1,3} }, { 3, 2, 3, {1,2} }
            }), "B^3 (large)" };
        TestCase ball_large_pillows { Triangulation<3>::fromGluings(4, {
                // Long ago, this used to crash the 2-0 vertex moves.
                { 0, 0, 1, {} }, { 0, 1, 1, {} }, { 0, 2, 1, {} },
                { 2, 0, 3, {} }, { 2, 1, 3, {} }, { 2, 2, 3, {} },
                { 0, 3, 2, {} }
            }), "B^3 (two pillows)" };
        TestCase ball_large_snapped { Triangulation<3>::fromGluings(3, {
                { 0, 2, 0, {2,3} }, { 1, 2, 1, {2,3} }, { 2, 2, 2, {2,1} },
                { 0, 1, 1, {} }, { 1, 0, 2, {} }
            }), "B^3 (three snapped balls)" };

        // Ideal orientable triangulations:
        TestCase figure8 { Example<3>::figureEight(),
            "Figure eight knot complement" };
        TestCase trefoil { Example<3>::trefoil(), "Trefoil complement" };
        TestCase knot18 { regina::Link::fromKnotSig(
                "sabcdeafghidejklmnopqgcbfqhinmjrpolkrlLvnvvNdM9aE").
                complement(),
            "18-crossing knot complement" };
        TestCase idealGenusTwoHandlebody {
            Example<3>::idealGenusTwoHandlebody(),
            "Ideal genus two handlebody" };
        TestCase figure8_bary {
            // We call subdivide() in the class constructor.
            Example<3>::figureEight(), "Figure eight (subdivided)" };

        // Ideal non-orientable triangulations:
        TestCase gieseking { Example<3>::gieseking(), "Gieseking manifold" };
        TestCase idealRP2xI { Triangulation<3>::fromGluings(1, {
                // Like invalidRP2xI but we then call subdivide() in the class
                // constructor, thus replacing invalid edges with RP^2 cusps.
                { 0, 0, 0, {1,0,3,2} }, { 0, 2, 0, {1,0,3,2} }
            }), "RP^2 x I (ideal)" };

        // Invalid triangulations:
        TestCase pinchedSolidTorus { Triangulation<3>::fromGluings(2, {
                // Identify two opposite faces of a square pyramid:
                { 0, 3, 1, {0, 1, 2, 3} },
                { 0, 2, 1, {0, 3, 1, 2} }
            }), "Pinched solid torus" };
        TestCase pinchedSolidKB { Triangulation<3>::fromGluings(2, {
                // Identify two opposite faces of a square pyramid, with a flip:
                { 0, 3, 1, {0, 1, 2, 3} },
                { 0, 2, 1, {0, 2, 1, 3} }
            }), "Pinched solid Klein bottle" };
        TestCase invalidRP2xI { Triangulation<3>::fromGluings(1, {
                // Fold faces of a tetrahedron together in pairs, but with a
                // reflection on each fold (thus forming two invalid edges).
                { 0, 0, 0, {1,0,3,2} }, { 0, 2, 0, {1,0,3,2} }
            }), "RP^2 x I (invalid edges)" };

        // Disconnected triangulations (built in the class constructor):
        TestCase disjoint2 { {}, "Gieseking U (ideal genus two handlebody)" };
        TestCase disjoint3 { {}, "(S^2 x S^1) U B^3 U (figure eight)" };

        Dim3Test() {
            ball_bary.tri.subdivide();
            figure8_bary.tri.subdivide();
            idealRP2xI.tri.subdivide();

            disjoint2.tri.insertTriangulation(gieseking.tri);
            disjoint2.tri.insertTriangulation(idealGenusTwoHandlebody.tri);

            disjoint3.tri.insertTriangulation(sphereBundle.tri);
            disjoint3.tri.insertTriangulation(ball_large_pillows.tri);
            disjoint3.tri.insertTriangulation(figure8.tri);
        }

        /**
         * Run a given test over all of the example triangulations stored in
         * this test fixture.
         */
        void testManualCases(void (*f)(const Triangulation<3>&, const char*),
                bool includeLarge = true) {
            testGenericCases(f);

            f(s3.tri, s3.name);
            f(rp3_1.tri, rp3_1.name);
            f(rp3_2.tri, rp3_2.name);
            f(lens3_1.tri, lens3_1.name);
            f(lens8_3.tri, lens8_3.name);
            f(lens7_1_loop.tri, lens7_1_loop.name);
            f(rp3rp3.tri, rp3rp3.name);
            f(q32xz3.tri, q32xz3.name);
            f(q28.tri, q28.name);
            f(weberSeifert.tri, weberSeifert.name);
            if (includeLarge)
                f(lens100_1.tri, lens100_1.name);
            f(s3_large.tri, s3_large.name);
            f(rp3_large.tri, rp3_large.name);
            f(lens8_3_large.tri, lens8_3_large.name);
            f(q20_large.tri, q20_large.name);

            f(rp2xs1.tri, rp2xs1.name);

            f(lst3_4_7.tri, lst3_4_7.name);
            f(ball_bary.tri, ball_bary.name);
            f(ball_large.tri, ball_large.name);
            f(ball_large_pillows.tri, ball_large_pillows.name);
            f(ball_large_snapped.tri, ball_large_snapped.name);

            f(figure8.tri, figure8.name);
            f(trefoil.tri, trefoil.name);
            if (includeLarge)
                f(knot18.tri, knot18.name);
            f(idealGenusTwoHandlebody.tri, idealGenusTwoHandlebody.name);
            f(figure8_bary.tri, figure8_bary.name);

            f(gieseking.tri, gieseking.name);
            f(idealRP2xI.tri, idealRP2xI.name);

            f(pinchedSolidTorus.tri, pinchedSolidTorus.name);
            f(pinchedSolidKB.tri, pinchedSolidKB.name);
            f(invalidRP2xI.tri, invalidRP2xI.name);

            f(disjoint2.tri, disjoint2.name);
            f(disjoint3.tri, disjoint3.name);
        }
};

TEST_F(Dim3Test, magic) {
    // Verify the "magic" string-based constructor.
    testManualCases([](const Triangulation<3>& t, const char* name) {
        SCOPED_TRACE_CSTRING(name);

        std::string sig = t.isoSig();

        {
            Triangulation<3> recon(sig);
            EXPECT_EQ(recon.isoSig(), sig);
        }

        if (t.isConnected() && t.size() <= 25 && ! t.hasBoundaryFacets()) {
            Triangulation<3> recon(t.dehydrate());
            EXPECT_EQ(recon.isoSig(), sig);
        }

        if ((! t.isEmpty()) && t.isValid() && ! t.hasBoundaryFacets()) {
            Triangulation<3> recon(t.snapPea());
            EXPECT_EQ(recon.isoSig(), sig);
        }
    });

    EXPECT_THROW({
        Triangulation<3> t("INVALID");
    }, regina::InvalidArgument);
}

static void verifyInvalid(const TriangulationTest<3>::TestCase& test,
        size_t invalidVertices, size_t invalidEdges) {
    SCOPED_TRACE_CSTRING(test.name);

    EXPECT_FALSE(test.tri.isValid());

    {
        size_t found = 0;
        for (auto v : test.tri.vertices()) {
            EXPECT_FALSE(v->hasBadIdentification());
            EXPECT_EQ(! v->isValid(), v->hasBadLink());
            if (v->hasBadLink())
                ++found;
        }
        EXPECT_EQ(found, invalidVertices);
    }
    {
        size_t found = 0;
        for (auto e : test.tri.edges()) {
            EXPECT_FALSE(e->hasBadLink());
            EXPECT_EQ(! e->isValid(), e->hasBadIdentification());
            if (e->hasBadIdentification())
                ++found;
        }
        EXPECT_EQ(found, invalidEdges);
    }
}

TEST_F(Dim3Test, validity) {
    TriangulationTest<3>::validityGenericCases();

    verifyValid(s3);
    verifyValid(rp3_1);
    verifyValid(rp3_2);
    verifyValid(lens3_1);
    verifyValid(lens8_3);
    verifyValid(lens7_1_loop);
    verifyValid(rp3rp3);
    verifyValid(q32xz3);
    verifyValid(q28);
    verifyValid(weberSeifert);
    verifyValid(lens100_1);
    verifyValid(s3_large);
    verifyValid(rp3_large);
    verifyValid(lens8_3_large);
    verifyValid(q20_large);

    verifyValid(rp2xs1);

    verifyValid(lst3_4_7);
    verifyValid(ball_bary);
    verifyValid(ball_large);
    verifyValid(ball_large_pillows);
    verifyValid(ball_large_snapped);

    verifyValid(figure8);
    verifyValid(trefoil);
    verifyValid(knot18);
    verifyValid(idealGenusTwoHandlebody);
    verifyValid(figure8_bary);

    verifyValid(gieseking);
    verifyValid(idealRP2xI);

    verifyInvalid(pinchedSolidTorus, 1, 0);
    verifyInvalid(pinchedSolidKB, 1, 0);
    verifyInvalid(invalidRP2xI, 0, 2);

    verifyValid(disjoint2);
    verifyValid(disjoint3);
}

TEST_F(Dim3Test, connectivity) {
    TriangulationTest<3>::connectivityGenericCases();

    EXPECT_TRUE(s3.tri.isConnected());
    EXPECT_TRUE(rp3_1.tri.isConnected());
    EXPECT_TRUE(rp3_2.tri.isConnected());
    EXPECT_TRUE(lens3_1.tri.isConnected());
    EXPECT_TRUE(lens8_3.tri.isConnected());
    EXPECT_TRUE(lens7_1_loop.tri.isConnected());
    EXPECT_TRUE(rp3rp3.tri.isConnected());
    EXPECT_TRUE(q32xz3.tri.isConnected());
    EXPECT_TRUE(q28.tri.isConnected());
    EXPECT_TRUE(weberSeifert.tri.isConnected());
    EXPECT_TRUE(lens100_1.tri.isConnected());
    EXPECT_TRUE(s3_large.tri.isConnected());
    EXPECT_TRUE(rp3_large.tri.isConnected());
    EXPECT_TRUE(lens8_3_large.tri.isConnected());
    EXPECT_TRUE(q20_large.tri.isConnected());

    EXPECT_TRUE(rp2xs1.tri.isConnected());

    EXPECT_TRUE(lst3_4_7.tri.isConnected());
    EXPECT_TRUE(ball_bary.tri.isConnected());
    EXPECT_TRUE(ball_large.tri.isConnected());
    EXPECT_TRUE(ball_large_pillows.tri.isConnected());
    EXPECT_TRUE(ball_large_snapped.tri.isConnected());

    EXPECT_TRUE(figure8.tri.isConnected());
    EXPECT_TRUE(trefoil.tri.isConnected());
    EXPECT_TRUE(knot18.tri.isConnected());
    EXPECT_TRUE(idealGenusTwoHandlebody.tri.isConnected());
    EXPECT_TRUE(figure8_bary.tri.isConnected());

    EXPECT_TRUE(gieseking.tri.isConnected());
    EXPECT_TRUE(idealRP2xI.tri.isConnected());

    EXPECT_TRUE(pinchedSolidTorus.tri.isConnected());
    EXPECT_TRUE(pinchedSolidKB.tri.isConnected());
    EXPECT_TRUE(invalidRP2xI.tri.isConnected());

    EXPECT_FALSE(disjoint2.tri.isConnected());
    EXPECT_FALSE(disjoint3.tri.isConnected());
}

TEST_F(Dim3Test, orientability) {
    TriangulationTest<3>::orientabilityGenericCases();

    EXPECT_TRUE(s3.tri.isOrientable());
    EXPECT_TRUE(rp3_1.tri.isOrientable());
    EXPECT_TRUE(rp3_2.tri.isOrientable());
    EXPECT_TRUE(lens3_1.tri.isOrientable());
    EXPECT_TRUE(lens8_3.tri.isOrientable());
    EXPECT_TRUE(lens7_1_loop.tri.isOrientable());
    EXPECT_TRUE(rp3rp3.tri.isOrientable());
    EXPECT_TRUE(q32xz3.tri.isOrientable());
    EXPECT_TRUE(q28.tri.isOrientable());
    EXPECT_TRUE(weberSeifert.tri.isOrientable());
    EXPECT_TRUE(lens100_1.tri.isOrientable());
    EXPECT_TRUE(s3_large.tri.isOrientable());
    EXPECT_TRUE(rp3_large.tri.isOrientable());
    EXPECT_TRUE(lens8_3_large.tri.isOrientable());
    EXPECT_TRUE(q20_large.tri.isOrientable());

    EXPECT_FALSE(rp2xs1.tri.isOrientable());

    EXPECT_TRUE(lst3_4_7.tri.isOrientable());
    EXPECT_TRUE(ball_bary.tri.isOrientable());
    EXPECT_TRUE(ball_large.tri.isOrientable());
    EXPECT_TRUE(ball_large_pillows.tri.isOrientable());
    EXPECT_TRUE(ball_large_snapped.tri.isOrientable());

    EXPECT_TRUE(figure8.tri.isOrientable());
    EXPECT_TRUE(trefoil.tri.isOrientable());
    EXPECT_TRUE(knot18.tri.isOrientable());
    EXPECT_TRUE(idealGenusTwoHandlebody.tri.isOrientable());
    EXPECT_TRUE(figure8_bary.tri.isOrientable());

    EXPECT_FALSE(gieseking.tri.isOrientable());
    EXPECT_FALSE(idealRP2xI.tri.isOrientable());

    EXPECT_TRUE(pinchedSolidTorus.tri.isOrientable());
    EXPECT_FALSE(pinchedSolidKB.tri.isOrientable());
    EXPECT_FALSE(invalidRP2xI.tri.isOrientable());

    EXPECT_FALSE(disjoint2.tri.isOrientable());
    EXPECT_TRUE(disjoint3.tri.isOrientable());
}

TEST_F(Dim3Test, standardness) {
    EXPECT_TRUE(empty.tri.isStandard());
    EXPECT_TRUE(sphere.tri.isStandard());
    EXPECT_TRUE(simpSphere.tri.isStandard());
    EXPECT_TRUE(sphereBundle.tri.isStandard());
    EXPECT_TRUE(twistedSphereBundle.tri.isStandard());
    EXPECT_TRUE(ball.tri.isStandard());
    EXPECT_TRUE(ballBundle.tri.isStandard());
    EXPECT_TRUE(twistedBallBundle.tri.isStandard());

    EXPECT_TRUE(s3.tri.isStandard());
    EXPECT_TRUE(rp3_1.tri.isStandard());
    EXPECT_TRUE(rp3_2.tri.isStandard());
    EXPECT_TRUE(lens3_1.tri.isStandard());
    EXPECT_TRUE(lens8_3.tri.isStandard());
    EXPECT_TRUE(lens7_1_loop.tri.isStandard());
    EXPECT_TRUE(rp3rp3.tri.isStandard());
    EXPECT_TRUE(q32xz3.tri.isStandard());
    EXPECT_TRUE(q28.tri.isStandard());
    EXPECT_TRUE(weberSeifert.tri.isStandard());
    EXPECT_TRUE(lens100_1.tri.isStandard());
    EXPECT_TRUE(s3_large.tri.isStandard());
    EXPECT_TRUE(rp3_large.tri.isStandard());
    EXPECT_TRUE(lens8_3_large.tri.isStandard());
    EXPECT_TRUE(q20_large.tri.isStandard());

    EXPECT_TRUE(rp2xs1.tri.isStandard());

    EXPECT_TRUE(lst3_4_7.tri.isStandard());
    EXPECT_TRUE(ball_bary.tri.isStandard());
    EXPECT_TRUE(ball_large.tri.isStandard());
    EXPECT_TRUE(ball_large_pillows.tri.isStandard());
    EXPECT_TRUE(ball_large_snapped.tri.isStandard());

    EXPECT_TRUE(figure8.tri.isStandard());
    EXPECT_TRUE(trefoil.tri.isStandard());
    EXPECT_TRUE(knot18.tri.isStandard());
    EXPECT_FALSE(idealGenusTwoHandlebody.tri.isStandard());
    EXPECT_TRUE(figure8_bary.tri.isStandard());

    EXPECT_TRUE(gieseking.tri.isStandard());
    EXPECT_FALSE(idealRP2xI.tri.isStandard());

    EXPECT_FALSE(pinchedSolidTorus.tri.isStandard());
    EXPECT_FALSE(pinchedSolidKB.tri.isStandard());
    // invalidRP2xI has 2-sphere vertex links:
    EXPECT_TRUE(invalidRP2xI.tri.isStandard());

    EXPECT_FALSE(disjoint2.tri.isStandard());
    EXPECT_TRUE(disjoint3.tri.isStandard());
}

TEST_F(Dim3Test, eulerChar) {
    TriangulationTest<3>::eulerCharGenericCases();

    EXPECT_EQ(s3.tri.eulerCharTri(), 0);
    EXPECT_EQ(s3.tri.eulerCharManifold(), 0);
    EXPECT_EQ(rp3_1.tri.eulerCharTri(), 0);
    EXPECT_EQ(rp3_1.tri.eulerCharManifold(), 0);
    EXPECT_EQ(rp3_2.tri.eulerCharTri(), 0);
    EXPECT_EQ(rp3_2.tri.eulerCharManifold(), 0);
    EXPECT_EQ(lens3_1.tri.eulerCharTri(), 0);
    EXPECT_EQ(lens3_1.tri.eulerCharManifold(), 0);
    EXPECT_EQ(lens8_3.tri.eulerCharTri(), 0);
    EXPECT_EQ(lens8_3.tri.eulerCharManifold(), 0);
    EXPECT_EQ(lens7_1_loop.tri.eulerCharTri(), 0);
    EXPECT_EQ(lens7_1_loop.tri.eulerCharManifold(), 0);
    EXPECT_EQ(rp3rp3.tri.eulerCharTri(), 0);
    EXPECT_EQ(rp3rp3.tri.eulerCharManifold(), 0);
    EXPECT_EQ(q32xz3.tri.eulerCharTri(), 0);
    EXPECT_EQ(q32xz3.tri.eulerCharManifold(), 0);
    EXPECT_EQ(q28.tri.eulerCharTri(), 0);
    EXPECT_EQ(q28.tri.eulerCharManifold(), 0);
    EXPECT_EQ(weberSeifert.tri.eulerCharTri(), 0);
    EXPECT_EQ(weberSeifert.tri.eulerCharManifold(), 0);
    EXPECT_EQ(lens100_1.tri.eulerCharTri(), 0);
    EXPECT_EQ(lens100_1.tri.eulerCharManifold(), 0);
    EXPECT_EQ(s3_large.tri.eulerCharTri(), 0);
    EXPECT_EQ(s3_large.tri.eulerCharManifold(), 0);
    EXPECT_EQ(rp3_large.tri.eulerCharTri(), 0);
    EXPECT_EQ(rp3_large.tri.eulerCharManifold(), 0);
    EXPECT_EQ(lens8_3_large.tri.eulerCharTri(), 0);
    EXPECT_EQ(lens8_3_large.tri.eulerCharManifold(), 0);
    EXPECT_EQ(q20_large.tri.eulerCharTri(), 0);
    EXPECT_EQ(q20_large.tri.eulerCharManifold(), 0);

    EXPECT_EQ(rp2xs1.tri.eulerCharTri(), 0);
    EXPECT_EQ(rp2xs1.tri.eulerCharManifold(), 0);

    EXPECT_EQ(lst3_4_7.tri.eulerCharTri(), 0);
    EXPECT_EQ(lst3_4_7.tri.eulerCharManifold(), 0);
    EXPECT_EQ(ball_bary.tri.eulerCharTri(), 1);
    EXPECT_EQ(ball_bary.tri.eulerCharManifold(), 1);
    EXPECT_EQ(ball_large.tri.eulerCharTri(), 1);
    EXPECT_EQ(ball_large.tri.eulerCharManifold(), 1);
    EXPECT_EQ(ball_large_pillows.tri.eulerCharTri(), 1);
    EXPECT_EQ(ball_large_pillows.tri.eulerCharManifold(), 1);
    EXPECT_EQ(ball_large_snapped.tri.eulerCharTri(), 1);
    EXPECT_EQ(ball_large_snapped.tri.eulerCharManifold(), 1);

    EXPECT_EQ(figure8.tri.eulerCharTri(), 1);
    EXPECT_EQ(figure8.tri.eulerCharManifold(), 0);
    EXPECT_EQ(trefoil.tri.eulerCharTri(), 1);
    EXPECT_EQ(trefoil.tri.eulerCharManifold(), 0);
    EXPECT_EQ(knot18.tri.eulerCharTri(), 1);
    EXPECT_EQ(knot18.tri.eulerCharManifold(), 0);
    EXPECT_EQ(idealGenusTwoHandlebody.tri.eulerCharTri(), 2);
    EXPECT_EQ(idealGenusTwoHandlebody.tri.eulerCharManifold(), -1);
    EXPECT_EQ(figure8_bary.tri.eulerCharTri(), 1);
    EXPECT_EQ(figure8_bary.tri.eulerCharManifold(), 0);

    EXPECT_EQ(gieseking.tri.eulerCharTri(), 1);
    EXPECT_EQ(gieseking.tri.eulerCharManifold(), 0);
    EXPECT_EQ(idealRP2xI.tri.eulerCharTri(), 1);
    EXPECT_EQ(idealRP2xI.tri.eulerCharManifold(), 1);

    EXPECT_EQ(invalidRP2xI.tri.eulerCharTri(), -1);
    EXPECT_EQ(invalidRP2xI.tri.eulerCharManifold(), 1);
    EXPECT_EQ(pinchedSolidTorus.tri.eulerCharTri(), 1);
    EXPECT_EQ(pinchedSolidTorus.tri.eulerCharManifold(), 0);
    EXPECT_EQ(pinchedSolidKB.tri.eulerCharTri(), 1);
    EXPECT_EQ(pinchedSolidKB.tri.eulerCharManifold(), 0);

    EXPECT_EQ(disjoint2.tri.eulerCharTri(), 3);
    EXPECT_EQ(disjoint2.tri.eulerCharManifold(), -1);
    EXPECT_EQ(disjoint3.tri.eulerCharTri(), 2);
    EXPECT_EQ(disjoint3.tri.eulerCharManifold(), 1);

    {
        Triangulation<3> t(idealRP2xI.tri);
        t.idealToFinite();
        EXPECT_EQ(t.eulerCharTri(), 1);
        EXPECT_EQ(t.eulerCharManifold(), 1);
    }
    {
        Triangulation<3> t(idealGenusTwoHandlebody.tri);
        t.idealToFinite();
        EXPECT_EQ(t.eulerCharTri(), -1);
        EXPECT_EQ(t.eulerCharManifold(), -1);
    }
    {
        Triangulation<3> t(pinchedSolidTorus.tri);
        t.idealToFinite();
        EXPECT_EQ(t.eulerCharTri(), 0);
        EXPECT_EQ(t.eulerCharManifold(), 0);
    }
    {
        Triangulation<3> t(pinchedSolidKB.tri);
        t.idealToFinite();
        EXPECT_EQ(t.eulerCharTri(), 0);
        EXPECT_EQ(t.eulerCharManifold(), 0);
    }
}

TEST_F(Dim3Test, boundaryBasic) {
    TriangulationTest<3>::boundaryBasicGenericCases();

    verifyBoundaryBasic(s3, {}, {}, {});
    verifyBoundaryBasic(rp3_1, {}, {}, {});
    verifyBoundaryBasic(rp3_2, {}, {}, {});
    verifyBoundaryBasic(lens3_1, {}, {}, {});
    verifyBoundaryBasic(lens8_3, {}, {}, {});
    verifyBoundaryBasic(lens7_1_loop, {}, {}, {});
    verifyBoundaryBasic(rp3rp3, {}, {}, {});
    verifyBoundaryBasic(q32xz3, {}, {}, {});
    verifyBoundaryBasic(q28, {}, {}, {});
    verifyBoundaryBasic(weberSeifert, {}, {}, {});
    verifyBoundaryBasic(lens100_1, {}, {}, {});
    verifyBoundaryBasic(s3_large, {}, {}, {});
    verifyBoundaryBasic(rp3_large, {}, {}, {});
    verifyBoundaryBasic(lens8_3_large, {}, {}, {});
    verifyBoundaryBasic(q20_large, {}, {}, {});

    verifyBoundaryBasic(rp2xs1, {}, {}, {});

    verifyBoundaryBasic(lst3_4_7, {0}, {}, {});
    verifyBoundaryBasic(ball_bary, {2}, {}, {});
    verifyBoundaryBasic(ball_large, {2}, {}, {});
    verifyBoundaryBasic(ball_large_pillows, {2}, {}, {});
    verifyBoundaryBasic(ball_large_snapped, {2}, {}, {});

    verifyBoundaryBasic(figure8, {}, {0}, {});
    verifyBoundaryBasic(trefoil, {}, {0}, {});
    verifyBoundaryBasic(knot18, {}, {0}, {});
    verifyBoundaryBasic(idealGenusTwoHandlebody, {}, {-2}, {});
    verifyBoundaryBasic(figure8_bary, {}, {0}, {});

    verifyBoundaryBasic(gieseking, {}, {0}, {});
    verifyBoundaryBasic(idealRP2xI, {}, {1,1}, {});

    verifyBoundaryBasic(pinchedSolidTorus, {1 /* S^2 with pinch */}, {}, {});
    verifyBoundaryBasic(pinchedSolidKB, {1}, {}, {});
    verifyBoundaryBasic(invalidRP2xI, {}, {}, {});

    verifyBoundaryBasic(disjoint2, {}, {0,-2}, {});
    verifyBoundaryBasic(disjoint3, {2}, {0}, {});
}

TEST_F(Dim3Test, boundaryPinching) {
    testManualCases(verifyBoundaryPinching);
}

TEST_F(Dim3Test, vertexLinksBasic) {
    using regina::Vertex;
    TriangulationTest<3>::vertexLinksBasicGenericCases();

    verifyVertexLinksBasic(s3, 1, 0);
    verifyVertexLinksBasic(rp3_1, 1, 0);
    verifyVertexLinksBasic(rp3_2, 2, 0);
    verifyVertexLinksBasic(lens3_1, 2, 0);
    verifyVertexLinksBasic(lens8_3, 1, 0);
    verifyVertexLinksBasic(lens7_1_loop, 2, 0);
    verifyVertexLinksBasic(rp3rp3, 1, 0);
    verifyVertexLinksBasic(q32xz3, 1, 0);
    verifyVertexLinksBasic(q28, 1, 0);
    verifyVertexLinksBasic(weberSeifert, 1, 0);
    verifyVertexLinksBasic(lens100_1, 1, 0);
    verifyVertexLinksBasic(s3_large, 2, 0);
    verifyVertexLinksBasic(rp3_large, 1, 0);
    verifyVertexLinksBasic(lens8_3_large, 1, 0);
    verifyVertexLinksBasic(q20_large, 1, 0);

    verifyVertexLinksBasic(rp2xs1, 1, 0);

    verifyVertexLinksBasic(lst3_4_7, 0, 1);
    verifyVertexLinksBasic(ball_bary, 1, 14);
    verifyVertexLinksBasic(ball_large, 1, 4);
    verifyVertexLinksBasic(ball_large_pillows, 2, 3);
    verifyVertexLinksBasic(ball_large_snapped, 1, 3);

    verifyVertexLinksBasic(figure8, 0, 0, 1);
    verifyVertexLinksBasic(trefoil, 0, 0, 1);
    verifyVertexLinksBasic(knot18, 0, 0, 1);
    verifyVertexLinksBasic(idealGenusTwoHandlebody, 1, 0, 1);
    verifyVertexLinksBasic(figure8_bary, 8, 0, 1);

    verifyVertexLinksBasic(gieseking, 0, 0, 1);
    verifyVertexLinksBasic(idealRP2xI, 7, 0, 2);

    verifyVertexLinksBasic(pinchedSolidTorus, 0, 2, 0, 1);
    verifyVertexLinksBasic(pinchedSolidKB, 0, 2, 0, 1);
    verifyVertexLinksBasic(invalidRP2xI, 2, 0);

    verifyVertexLinksBasic(disjoint2, 1, 0, 2);
    verifyVertexLinksBasic(disjoint3, 3, 3, 1);

    // Verify the specific surfaces for non-spheres/balls:
    EXPECT_EQ(figure8.tri.vertex(0)->linkType(), Vertex<3>::TORUS);
    EXPECT_EQ(trefoil.tri.vertex(0)->linkType(), Vertex<3>::TORUS);
    EXPECT_EQ(knot18.tri.vertex(0)->linkType(), Vertex<3>::TORUS);
    {
        auto v = idealGenusTwoHandlebody.tri.vertex(1);
        EXPECT_EQ(v->linkType(), Vertex<3>::NON_STANDARD_CUSP);
        EXPECT_EQ(v->linkEulerChar(), -2);
        EXPECT_TRUE(v->isLinkOrientable());
    }
    EXPECT_EQ(figure8_bary.tri.vertex(0)->linkType(), Vertex<3>::TORUS);

    EXPECT_EQ(gieseking.tri.vertex(0)->linkType(), Vertex<3>::KLEIN_BOTTLE);
    {
        auto v = idealRP2xI.tri.vertex(1);
        EXPECT_EQ(v->linkType(), Vertex<3>::NON_STANDARD_CUSP);
        EXPECT_EQ(v->linkEulerChar(), 1);
        EXPECT_FALSE(v->isLinkOrientable());
    }
    {
        auto v = idealRP2xI.tri.vertex(8);
        EXPECT_EQ(v->linkType(), Vertex<3>::NON_STANDARD_CUSP);
        EXPECT_EQ(v->linkEulerChar(), 1);
        EXPECT_FALSE(v->isLinkOrientable());
    }

    {
        auto v = pinchedSolidTorus.tri.vertex(0);
        EXPECT_EQ(v->linkType(), Vertex<3>::INVALID);
        EXPECT_EQ(v->linkEulerChar(), 0);
        EXPECT_TRUE(v->isLinkOrientable());
    }
    {
        auto v = pinchedSolidKB.tri.vertex(0);
        EXPECT_EQ(v->linkType(), Vertex<3>::INVALID);
        EXPECT_EQ(v->linkEulerChar(), 0);
        EXPECT_FALSE(v->isLinkOrientable());
    }

    EXPECT_EQ(disjoint2.tri.vertex(0)->linkType(), Vertex<3>::KLEIN_BOTTLE);
    {
        auto v = disjoint2.tri.vertex(2);
        EXPECT_EQ(v->linkType(), Vertex<3>::NON_STANDARD_CUSP);
        EXPECT_EQ(v->linkEulerChar(), -2);
        EXPECT_TRUE(v->isLinkOrientable());
    }
    EXPECT_EQ(disjoint3.tri.vertex(6)->linkType(), Vertex<3>::TORUS);
}

static void verifyVertexLinks(const Triangulation<3>& tri, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    for (auto v : tri.vertices()) {
        const Triangulation<2>& link = v->buildLink();
        Isomorphism<3> iso = v->buildLinkInclusion();

        EXPECT_EQ(link.size(), v->degree());
        EXPECT_TRUE(link.isConnected());

        if (v->isIdeal()) {
            EXPECT_TRUE(v->isValid());
            EXPECT_TRUE(link.isClosed());
            EXPECT_NE(link.eulerChar(), 2);
        } else if (! v->isStandard()) {
            EXPECT_FALSE(v->isValid());
            EXPECT_FALSE(link.isClosed());
            EXPECT_NE(link.eulerChar(), 1);
        } else if (v->isBoundary()) {
            EXPECT_TRUE(v->isValid());
            EXPECT_FALSE(link.isClosed());
            EXPECT_EQ(link.eulerChar(), 1);
        } else {
            EXPECT_TRUE(v->isValid());
            EXPECT_TRUE(link.isClosed());
            EXPECT_EQ(link.eulerChar(), 2);
        }

        // Make sure the triangulated link is labelled correctly.
        for (size_t j = 0; j < v->degree(); ++j) {
            auto tet = tri.tetrahedron(iso.tetImage(j));

            regina::Perm<4> perm = iso.facetPerm(j);
            int vNum = perm[3];
            EXPECT_EQ(tet->vertex(vNum), v);
            EXPECT_EQ(perm[0], tet->triangleMapping(vNum)[0]);
            EXPECT_EQ(perm[1], tet->triangleMapping(vNum)[1]);
            EXPECT_EQ(perm[2], tet->triangleMapping(vNum)[2]);

            for (int k = 0; k < 3; ++k) {
                auto t = link.triangle(j);
                auto adj = t->adjacentTriangle(k);
                if (adj) {
                    EXPECT_EQ(tet->adjacentTetrahedron(perm[k]),
                        tri.tetrahedron(iso.tetImage(adj->index())));
                    EXPECT_EQ(tet->adjacentGluing(perm[k]),
                        iso.facetPerm(adj->index()) *
                            regina::Perm<4>::extend(t->adjacentGluing(k)) *
                            perm.inverse());
                } else {
                    EXPECT_EQ(tet->adjacentTetrahedron(perm[k]), nullptr);
                }
            }
        }
    }
}

TEST_F(Dim3Test, vertexLinks) {
    testManualCases(verifyVertexLinks);
    runCensusAllClosed(verifyVertexLinks);
    runCensusAllBounded(verifyVertexLinks);
    runCensusAllIdeal(verifyVertexLinks);
}

TEST_F(Dim3Test, orient) {
    testManualCases(TriangulationTest<3>::verifyOrient);
}

TEST_F(Dim3Test, skeleton) {
    testManualCases(TriangulationTest<3>::verifySkeleton);
}

TEST_F(Dim3Test, boundaryLabelling) {
    testManualCases(TriangulationTest<3>::verifyBoundaryLabelling);
}

TEST_F(Dim3Test, edgeAccess) {
    TriangulationTest<3>::edgeAccess();
}

TEST_F(Dim3Test, reordering) {
    testManualCases(TriangulationTest<3>::verifyReordering);
}

TEST_F(Dim3Test, doubleCover) {
    testManualCases(TriangulationTest<3>::verifyDoubleCover);
}

TEST_F(Dim3Test, makeCanonical) {
    testManualCases(TriangulationTest<3>::verifyMakeCanonical);
}

TEST_F(Dim3Test, isomorphismSignature) {
    testManualCases(TriangulationTest<3>::verifyIsomorphismSignature);
}

static void verifyDehydration(const Triangulation<3>& tri, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    if (tri.hasBoundaryFacets() || tri.size() > 25 || tri.countComponents() > 1)
        EXPECT_THROW({
            tri.dehydrate();
        }, regina::NotImplemented);
    else
        EXPECT_NO_THROW({
            std::string str = tri.dehydrate();
            EXPECT_FALSE(str.empty());
            EXPECT_TRUE(Triangulation<3>::rehydrate(str).isIsomorphicTo(tri));
        });
}

TEST_F(Dim3Test, dehydration) {
    testManualCases(verifyDehydration);
}

TEST_F(Dim3Test, pachner) {
    testManualCases(TriangulationTest<3>::verifyPachner);
    runCensusAllClosed(TriangulationTest<3>::verifyPachner, true);
    runCensusAllBounded(TriangulationTest<3>::verifyPachner, true);
    runCensusAllIdeal(TriangulationTest<3>::verifyPachner, true);
    verifyPachnerSimplicial();
}

static void verifyTwoZeroEdgeMove(Triangulation<3> tri, size_t whichEdge,
        const Triangulation<3>& result, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    EXPECT_TRUE(tri.twoZeroMove(tri.edge(whichEdge)));
    EXPECT_TRUE(tri.isIsomorphicTo(result));
    // Note: in all but one of our 2-0 tests, the triangulations are not just
    // isomorphic but also identical.  (The exception is internal-flat-lens.)
}

static void verifyTwoZeroEdgeInvalid(const Triangulation<3>& tri,
        const char* name) {
    // This is intended for cases that have an internal degree two edge,
    // but where the corresponding 2-0 move is not allowed.

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

TEST_F(Dim3Test, twoZeroEdgeMove) {
    // -------------------------------------------
    // Cases where a 2-0 edge move should succeed:
    // -------------------------------------------

    // A one-boundary-face case that Regina 4.1 used to crash on.
    verifyTwoZeroEdgeMove(Triangulation<3>::fromGluings(5, {
            { 0, 1, 2, {3,0,1,2} }, { 0, 2, 4, {3,0,2,1} },
            { 0, 3, 4, {3,0,2,1} }, { 1, 2, 2, {0,1,3,2} },
            { 1, 3, 4, {2,1,3,0} }, { 2, 1, 3, {0,2,3,1} },
            { 2, 2, 3, {2,1,0,3} }, { 3, 3, 4, {0,1,2,3} }}),
        0, Triangulation<3>::fromGluings(3, {
            { 0, 2, 1, {0,1,3,2} }, { 0, 3, 1, {1,2,3,0} },
            { 1, 1, 2, {0,2,3,1} }, { 1, 2, 2, {2,1,0,3} }}),
        "one-boundary-face");

    verifyTwoZeroEdgeMove(Triangulation<3>::fromGluings(3, {
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

        verifyTwoZeroEdgeMove(std::move(orig), 0,
            regina::Example<3>::lst(3, 4), "boundary-layer");
    }

    // A degree two edge gadget wedged between two adjacent internal faces in
    // a LST(3,4,7).
    verifyTwoZeroEdgeMove(Triangulation<3>::fromGluings(5, {
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

        verifyTwoZeroEdgeMove(orig, 5, lens, "internal-loop-twist");
        verifyTwoZeroEdgeMove(std::move(orig), 0, lens, "internal-flat-lens");
    }

    // Two solid Klein bottles glued along a single face.
    verifyTwoZeroEdgeMove(Triangulation<3>::fromGluings(4, {
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
    verifyTwoZeroEdgeMove(Triangulation<3>::fromGluings(8, {
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

    // ----------------------------------------
    // Cases where a 2-0 edge move should fail:
    // ----------------------------------------

    // A degree two edge gadget, with all four outer faces joined together in a
    // simple loop.
    verifyTwoZeroEdgeInvalid(
        regina::SimpleSurfaceBundle(regina::SimpleSurfaceBundle::S2xS1)
            .construct(),
        "round-loop");

    // A degree two edge gadget, with all four outer faces joined together in a
    // crossed loop.
    verifyTwoZeroEdgeInvalid(
        regina::SimpleSurfaceBundle(regina::SimpleSurfaceBundle::S2xS1_TWISTED)
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
static void verifyZeroTwoMove(const Triangulation<3>& tri, const char* name ) {
    SCOPED_TRACE_CSTRING(name);

    Triangulation<3> oriented(tri);
    if (oriented.isOrientable())
        oriented.orient();

    for (size_t i = 0; i < tri.countEdges(); ++i) {
        SCOPED_TRACE_NUMERIC(i);

        size_t deg = oriented.edge(i)->degree();
        for (size_t j = 0; j <= deg; ++j) {
            SCOPED_TRACE_NUMERIC(j);

            for (size_t jj = j; jj <= deg; ++jj) {
                SCOPED_TRACE_NUMERIC(jj);

                Triangulation<3> alt(oriented);
                bool legal = alt.zeroTwoMove(alt.edge(i), j, jj);

                // Check that different versions of zeroTwoMove give
                // isomorphic results.
                {
                    Triangulation<3> alt2(oriented);
                    size_t num[2] = {j, jj};
                    regina::Triangle<3>* t[2];
                    int e[2];
                    for ( int k : {0, 1} ) {
                        if ( num[k] == deg ) {
                            auto emb = alt2.edge(i)->back();
                            t[k] = emb.simplex()->triangle(emb.vertices()[2]);
                            e[k] = emb.simplex()->faceMapping<2>(
                                emb.vertices()[2]).pre(emb.vertices()[3]);
                        } else {
                            auto emb = alt2.edge(i)->embedding(num[k]);
                            t[k] = emb.simplex()->triangle(emb.vertices()[3]);
                            e[k] = emb.simplex()->faceMapping<2>(
                                emb.vertices()[3]).pre(emb.vertices()[2]);
                        }
                    }

                    // Note: this alternate form of the move is legal in more
                    // settings (i.e., legal2 may be true but legal may be
                    // false).  The discrepancy arises when the edge is
                    // internal: in this case the first form of the move
                    // (edge, int, int) cannot have j == deg or jj == deg.
                    bool legal2 = alt2.zeroTwoMove(t[0], e[0], t[1], e[1]);

                    regina::Edge<3>* edge = oriented.edge(i);
                    if (edge->isBoundary() || (j < deg && jj < deg)) {
                        EXPECT_EQ(legal2, legal);
                    } else {
                        EXPECT_FALSE(legal);
                        EXPECT_EQ(legal2, edge->isValid());
                    }

                    if (legal)
                        EXPECT_TRUE(alt.isIsomorphicTo(alt2));
                }

                if (! legal) {
                    // Check that the move was _not_ performed.
                    EXPECT_EQ(alt, oriented);
                    continue;
                }

                // The move was performed (hopefully correctly).

                // Ensure that properties we are about to verify are
                // explicitly recomputed.
                clearProperties(alt);

                EXPECT_EQ(alt.size(), tri.size() + 2);
                EXPECT_EQ(alt.countVertices(), tri.countVertices());
                EXPECT_EQ(alt.isValid(), tri.isValid());
                EXPECT_EQ(alt.isOrientable(), tri.isOrientable());
                if (tri.isOrientable())
                    EXPECT_TRUE(alt.isOriented());
                EXPECT_EQ(alt.isClosed(), tri.isClosed());
                EXPECT_EQ(alt.countBoundaryComponents(),
                    tri.countBoundaryComponents());
                EXPECT_EQ(alt.eulerCharTri(), tri.eulerCharTri());
                EXPECT_EQ(alt.eulerCharManifold(), tri.eulerCharManifold());

                if (tri.isValid()) {
                    EXPECT_EQ(alt.homology<1>(), tri.homology<1>());
                    EXPECT_EQ(alt.homology<2>(), tri.homology<2>());
                }

                // Randomly relabel the tetrahedra, but preserve orientation.
                Isomorphism<3> iso = alt.randomiseLabelling(true);

                // Test the inverse 2-0 move.
                regina::Triangulation<3> inv(alt);
                EXPECT_TRUE(inv.twoZeroMove(
                    inv.tetrahedron(iso.simpImage(inv.size() - 1))->edge(
                        iso.facetPerm(inv.size() - 1)[2],
                        iso.facetPerm(inv.size() - 1)[3])));

                EXPECT_TRUE(inv.isIsomorphicTo(tri));
                if (tri.isOrientable())
                    EXPECT_TRUE(inv.isOriented());
            }
        }
    }
}

TEST_F(Dim3Test, zeroTwoMove) {
    testManualCases(verifyZeroTwoMove, false);
    runCensusAllClosed(verifyZeroTwoMove, true);
    runCensusAllBounded(verifyZeroTwoMove, true);
    runCensusAllIdeal(verifyZeroTwoMove, true);
}

TEST_F(Dim3Test, pinchEdge) {
    // Start with the snapped 1-tetrahedron triangulation of the 3-sphere.
    // Edges 0 and 2 make a Hopf link, and edge 1 is just an interval.
    {
        Triangulation<3> snap = Triangulation<3>::fromGluings(1,
            {{ 0, 0, 0, {0,1} }, { 0, 2, 0, {2,3} }});

        {
            Triangulation<3> tmp(snap);
            tmp.pinchEdge(tmp.edge(0));
            EXPECT_TRUE(tmp.isSolidTorus());
            EXPECT_TRUE(tmp.isOriented());
        }
        {
            Triangulation<3> tmp(snap);
            tmp.pinchEdge(tmp.edge(1));
            EXPECT_TRUE(tmp.isSphere());
            EXPECT_TRUE(tmp.isOriented());
        }
        {
            Triangulation<3> tmp(snap);
            tmp.pinchEdge(tmp.edge(2));
            EXPECT_TRUE(tmp.isSolidTorus());
            EXPECT_TRUE(tmp.isOriented());
        }
    }

    // Move on to the layered 1-tetrahedron triangulation of the 3-sphere.
    // Edge 0 forms a trefoil, and edge 1 is unknotted.
    {
        Triangulation<3> layer = Triangulation<3>::fromGluings(1,
            {{ 0, 0, 0, {1,2,3,0} }, { 0, 2, 0, {2,3} }});

        {
            Triangulation<3> tmp(layer);
            tmp.pinchEdge(tmp.edge(0));
            EXPECT_TRUE(tmp.isValid());
            EXPECT_TRUE(tmp.isIdeal());
            EXPECT_TRUE(tmp.isOriented());
            EXPECT_EQ(tmp.homology(), AbelianGroup{1});
            EXPECT_FALSE(tmp.isSolidTorus());
            ASSERT_EQ(tmp.countBoundaryComponents(), 1);
            EXPECT_TRUE(tmp.boundaryComponent(0)->isOrientable());
            EXPECT_EQ(tmp.boundaryComponent(0)->eulerChar(), 0);
        }
        {
            Triangulation<3> tmp(layer);
            tmp.pinchEdge(tmp.edge(1));
            EXPECT_TRUE(tmp.isSolidTorus());
            EXPECT_TRUE(tmp.isOriented());
        }
    }

    // Now try a 2-tetrahedron ball, where we pinch the internal edge between
    // the two tetrahedra and then truncate the resulting invalid vertex.
    // The result should be a solid torus.
    {
        Triangulation<3> ball = Triangulation<3>::fromGluings(2,
            {{ 0, 0, 1, {2,3} }, { 0, 1, 1, {2,3} }});

        // The internal edge joins vertices 2-3.
        Triangulation<3> tmp(ball);
        tmp.pinchEdge(tmp.tetrahedron(0)->edge(5));
        EXPECT_TRUE(tmp.isOriented());
        tmp.idealToFinite(); // truncate invalid vertex
        EXPECT_TRUE(tmp.isSolidTorus());
    }
}

TEST_F(Dim3Test, barycentricSubdivision) {
    testManualCases(TriangulationTest<3>::verifyBarycentricSubdivision);
}

TEST_F(Dim3Test, tightEncoding) {
    testManualCases(TriangulationTest<3>::verifyTightEncoding);
    runCensusAllClosed(TriangulationTest<3>::verifyTightEncoding);
    runCensusAllBounded(TriangulationTest<3>::verifyTightEncoding);
    runCensusAllIdeal(TriangulationTest<3>::verifyTightEncoding);
}

TEST_F(Dim3Test, homologyH1) {
    TriangulationTest<3>::homologyH1GenericCases();

    EXPECT_EQ(s3.tri.homology<1>(), AbelianGroup());
    EXPECT_EQ(rp3_1.tri.homology<1>(), AbelianGroup(0, {2}));
    EXPECT_EQ(rp3_2.tri.homology<1>(), AbelianGroup(0, {2}));
    EXPECT_EQ(lens3_1.tri.homology<1>(), AbelianGroup(0, {3}));
    EXPECT_EQ(lens8_3.tri.homology<1>(), AbelianGroup(0, {8}));
    EXPECT_EQ(lens7_1_loop.tri.homology<1>(), AbelianGroup(0, {7}));
    EXPECT_EQ(rp3rp3.tri.homology<1>(), AbelianGroup(0, {2,2}));
    EXPECT_EQ(q32xz3.tri.homology<1>(), AbelianGroup(0, {2,6}));
    EXPECT_EQ(q28.tri.homology<1>(), AbelianGroup(0, {4}));
    EXPECT_EQ(weberSeifert.tri.homology<1>(), AbelianGroup(0, {5,5,5}));
    EXPECT_EQ(lens100_1.tri.homology<1>(), AbelianGroup(0, {100}));
    EXPECT_EQ(s3_large.tri.homology<1>(), AbelianGroup());
    EXPECT_EQ(rp3_large.tri.homology<1>(), AbelianGroup(0, {2}));
    EXPECT_EQ(lens8_3_large.tri.homology<1>(), AbelianGroup(0, {8}));
    EXPECT_EQ(q20_large.tri.homology<1>(), AbelianGroup(0, {4}));

    EXPECT_EQ(rp2xs1.tri.homology<1>(), AbelianGroup(1, {2}));

    EXPECT_EQ(lst3_4_7.tri.homology<1>(), AbelianGroup(1));
    EXPECT_EQ(ball_bary.tri.homology<1>(), AbelianGroup());
    EXPECT_EQ(ball_large.tri.homology<1>(), AbelianGroup());
    EXPECT_EQ(ball_large_pillows.tri.homology<1>(), AbelianGroup());
    EXPECT_EQ(ball_large_snapped.tri.homology<1>(), AbelianGroup());

    EXPECT_EQ(figure8.tri.homology<1>(), AbelianGroup(1));
    EXPECT_EQ(trefoil.tri.homology<1>(), AbelianGroup(1));
    EXPECT_EQ(knot18.tri.homology<1>(), AbelianGroup(1));
    EXPECT_EQ(idealGenusTwoHandlebody.tri.homology<1>(), AbelianGroup(2));
    EXPECT_EQ(figure8_bary.tri.homology<1>(), AbelianGroup(1));

    EXPECT_EQ(gieseking.tri.homology<1>(), AbelianGroup(1));
    EXPECT_EQ(idealRP2xI.tri.homology<1>(), AbelianGroup(0, {2}));

    EXPECT_EQ(pinchedSolidTorus.tri.homology<1>(), AbelianGroup(1));
    EXPECT_EQ(pinchedSolidKB.tri.homology<1>(), AbelianGroup(1));
    EXPECT_EQ(invalidRP2xI.tri.homology<1>(), AbelianGroup());

    EXPECT_EQ(disjoint2.tri.homology<1>(), AbelianGroup(3));
    EXPECT_EQ(disjoint3.tri.homology<1>(), AbelianGroup(2));
}

TEST_F(Dim3Test, homologyH2) {
    TriangulationTest<3>::homologyH2GenericCases();

    EXPECT_EQ(s3.tri.homology<2>(), AbelianGroup());
    EXPECT_EQ(rp3_1.tri.homology<2>(), AbelianGroup());
    EXPECT_EQ(rp3_2.tri.homology<2>(), AbelianGroup());
    EXPECT_EQ(lens3_1.tri.homology<2>(), AbelianGroup());
    EXPECT_EQ(lens8_3.tri.homology<2>(), AbelianGroup());
    EXPECT_EQ(lens7_1_loop.tri.homology<2>(), AbelianGroup());
    EXPECT_EQ(rp3rp3.tri.homology<2>(), AbelianGroup());
    EXPECT_EQ(q32xz3.tri.homology<2>(), AbelianGroup());
    EXPECT_EQ(q28.tri.homology<2>(), AbelianGroup());
    EXPECT_EQ(weberSeifert.tri.homology<2>(), AbelianGroup());
    EXPECT_EQ(lens100_1.tri.homology<2>(), AbelianGroup());
    EXPECT_EQ(s3_large.tri.homology<2>(), AbelianGroup());
    EXPECT_EQ(rp3_large.tri.homology<2>(), AbelianGroup());
    EXPECT_EQ(lens8_3_large.tri.homology<2>(), AbelianGroup());
    EXPECT_EQ(q20_large.tri.homology<2>(), AbelianGroup());

    EXPECT_EQ(rp2xs1.tri.homology<2>(), AbelianGroup(0, {2}));

    EXPECT_EQ(lst3_4_7.tri.homology<2>(), AbelianGroup());
    EXPECT_EQ(ball_bary.tri.homology<2>(), AbelianGroup());
    EXPECT_EQ(ball_large.tri.homology<2>(), AbelianGroup());
    EXPECT_EQ(ball_large_pillows.tri.homology<2>(), AbelianGroup());
    EXPECT_EQ(ball_large_snapped.tri.homology<2>(), AbelianGroup());

    EXPECT_EQ(figure8.tri.homology<2>(), AbelianGroup());
    EXPECT_EQ(trefoil.tri.homology<2>(), AbelianGroup());
    EXPECT_EQ(knot18.tri.homology<2>(), AbelianGroup());
    EXPECT_EQ(idealGenusTwoHandlebody.tri.homology<2>(), AbelianGroup());
    EXPECT_EQ(figure8_bary.tri.homology<2>(), AbelianGroup());

    EXPECT_EQ(gieseking.tri.homology<2>(), AbelianGroup());
    EXPECT_EQ(idealRP2xI.tri.homology<2>(), AbelianGroup());

    // Skip the invalid cases, since homology<k>() requires a valid
    // triangulation for k ≥ 2.

    EXPECT_EQ(disjoint2.tri.homology<2>(), AbelianGroup());
    EXPECT_EQ(disjoint3.tri.homology<2>(), AbelianGroup(1));
}

TEST_F(Dim3Test, boundaryHomology) {
    TriangulationTest<3>::boundaryHomologyGenericCases();

    verifyBoundaryH1(lst3_4_7, 0, {2});
    verifyBoundaryH1(ball_bary, 0, {});
    verifyBoundaryH1(ball_large, 0, {});
    verifyBoundaryH1(ball_large_pillows, 0, {});
    verifyBoundaryH1(ball_large_snapped, 0, {});

    verifyBoundaryH1(figure8, 0, {2});
    verifyBoundaryH1(trefoil, 0, {2});
    verifyBoundaryH1(knot18, 0, {2});
    verifyBoundaryH1(idealGenusTwoHandlebody, 0, {4});
    verifyBoundaryH1(figure8_bary, 0, {2});

    verifyBoundaryH1(gieseking, 0, {1, {2}});
    verifyBoundaryH1(idealRP2xI, 0, {0, {2}});
    verifyBoundaryH1(idealRP2xI, 1, {0, {2}});

    verifyBoundaryH1(pinchedSolidTorus, 0, {}); // S^2 ignoring the pinch
    verifyBoundaryH1(pinchedSolidKB, 0, {0, {2}}); // RP^2 ignoring the pinch

    verifyBoundaryH1(disjoint2, 0, {1, {2}});
    verifyBoundaryH1(disjoint2, 1, {4});
    verifyBoundaryH1(disjoint3, 0, {});
    verifyBoundaryH1(disjoint3, 1, {2});

    // Also test the dedicated 3-D homologyBdry() routine.
    // This requires a valid triangulation, so we skip the invalid cases here.

    EXPECT_EQ(ball.tri.homologyBdry(), AbelianGroup());
    EXPECT_EQ(ballBundle.tri.homologyBdry(), AbelianGroup(2));
    EXPECT_EQ(twistedBallBundle.tri.homologyBdry(), AbelianGroup(1, {2}));

    EXPECT_EQ(lst3_4_7.tri.homologyBdry(), AbelianGroup(2));
    EXPECT_EQ(ball_bary.tri.homologyBdry(), AbelianGroup());
    EXPECT_EQ(ball_large.tri.homologyBdry(), AbelianGroup());
    EXPECT_EQ(ball_large_pillows.tri.homologyBdry(), AbelianGroup());
    EXPECT_EQ(ball_large_snapped.tri.homologyBdry(), AbelianGroup());

    EXPECT_EQ(figure8.tri.homologyBdry(), AbelianGroup(2));
    EXPECT_EQ(trefoil.tri.homologyBdry(), AbelianGroup(2));
    EXPECT_EQ(knot18.tri.homologyBdry(), AbelianGroup(2));
    EXPECT_EQ(idealGenusTwoHandlebody.tri.homologyBdry(), AbelianGroup(4));
    EXPECT_EQ(figure8_bary.tri.homologyBdry(), AbelianGroup(2));

    EXPECT_EQ(gieseking.tri.homologyBdry(), AbelianGroup(1, {2}));
    EXPECT_EQ(idealRP2xI.tri.homologyBdry(), AbelianGroup(0, {2,2}));

    EXPECT_EQ(disjoint2.tri.homologyBdry(), AbelianGroup(5, {2}));
    EXPECT_EQ(disjoint3.tri.homologyBdry(), AbelianGroup(2));
}

TEST_F(Dim3Test, fundGroup) {
    TriangulationTest<3>::fundGroupGenericCases();

    EXPECT_EQ(s3.tri.group().recogniseGroup(), "0");
    EXPECT_EQ(rp3_1.tri.group().recogniseGroup(), "Z_2");
    EXPECT_EQ(rp3_2.tri.group().recogniseGroup(), "Z_2");
    EXPECT_EQ(lens3_1.tri.group().recogniseGroup(), "Z_3");
    EXPECT_EQ(lens8_3.tri.group().recogniseGroup(), "Z_8");
    EXPECT_EQ(lens7_1_loop.tri.group().recogniseGroup(), "Z_7");
    EXPECT_EQ(rp3rp3.tri.group().recogniseGroup(), "FreeProduct( Z_2, Z_2 )");
    EXPECT_EQ(q32xz3.tri.group().recogniseGroup(), "");
    EXPECT_EQ(q28.tri.group().recogniseGroup(), "");
    EXPECT_EQ(weberSeifert.tri.group().recogniseGroup(), "");
    EXPECT_EQ(lens100_1.tri.group().recogniseGroup(), "Z_100");
    EXPECT_EQ(s3_large.tri.group().recogniseGroup(), "0");
    EXPECT_EQ(rp3_large.tri.group().recogniseGroup(), "Z_2");
    EXPECT_EQ(lens8_3_large.tri.group().recogniseGroup(), "Z_8");
    EXPECT_EQ(q20_large.tri.group().recogniseGroup(), "");

    EXPECT_EQ(rp2xs1.tri.group().recogniseGroup(), "Z + Z_2");

    EXPECT_EQ(lst3_4_7.tri.group().recogniseGroup(), "Z");
    EXPECT_EQ(ball_bary.tri.group().recogniseGroup(), "0");
    EXPECT_EQ(ball_large.tri.group().recogniseGroup(), "0");
    EXPECT_EQ(ball_large_pillows.tri.group().recogniseGroup(), "0");
    EXPECT_EQ(ball_large_snapped.tri.group().recogniseGroup(), "0");

    EXPECT_EQ(figure8.tri.group().recogniseGroup(),
        "Z~Free(2) w/monodromy a \u21A6 b, b \u21A6 b^2 a^-1 b");
    EXPECT_EQ(trefoil.tri.group().recogniseGroup(),
        "Z~Free(2) w/monodromy a \u21A6 b, b \u21A6 a^-1 b");
    EXPECT_EQ(knot18.tri.group().recogniseGroup(), "");
    EXPECT_EQ(idealGenusTwoHandlebody.tri.group().recogniseGroup(), "Free(2)");
    EXPECT_EQ(figure8_bary.tri.group().recogniseGroup(),
        "Z~Free(2) w/monodromy a \u21A6 b, b \u21A6 b^2 a^-1 b");

    EXPECT_EQ(gieseking.tri.group().recogniseGroup(),
        "Z~Free(2) w/monodromy a \u21A6 b, b \u21A6 b^-1 a");
    EXPECT_EQ(idealRP2xI.tri.group().recogniseGroup(), "Z_2");

    EXPECT_EQ(pinchedSolidTorus.tri.group().recogniseGroup(), "Z");
    EXPECT_EQ(pinchedSolidKB.tri.group().recogniseGroup(), "Z");
    EXPECT_EQ(invalidRP2xI.tri.group().recogniseGroup(), "0");

    // We cannot call group() on disjoint triangulations.
}

static void verifyFundGroupVsH1(const Triangulation<3>& tri, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    regina::GroupPresentation g(tri.group());
    g.intelligentSimplify();

    // Abelianise, and make sure we get H1.
    size_t gen = g.countGenerators();
    size_t rel = g.countRelations();

    regina::MatrixInt m(rel, gen);
    for (size_t i = 0; i < rel; ++i)
        for (const auto& t : g.relation(i).terms())
            m.entry(i, t.generator) += t.exponent;

    EXPECT_EQ(AbelianGroup(std::move(m)), tri.homology());
}

TEST_F(Dim3Test, fundGroupVsH1) {
    testManualCases(verifyFundGroupVsH1);
    runCensusAllClosed(verifyFundGroupVsH1);
    runCensusAllBounded(verifyFundGroupVsH1);
    runCensusAllIdeal(verifyFundGroupVsH1);
}

TEST_F(Dim3Test, chainComplex) {
    testManualCases(TriangulationTest<3>::verifyChainComplex, false);
}

TEST_F(Dim3Test, dualChainComplex) {
    testManualCases(TriangulationTest<3>::verifyDualChainComplex, false);
}

TEST_F(Dim3Test, dualToPrimal) {
    testManualCases(TriangulationTest<3>::verifyDualToPrimal, false);
}

TEST_F(Dim3Test, copyMove) {
    testManualCases(TriangulationTest<3>::verifyCopyMove);
}

static void verifySimplificationName(const TriangulationTest<3>::TestCase& test,
        size_t expectSize, const char* expectName) {
    SCOPED_TRACE_CSTRING(test.name);

    Triangulation<3> t(test.tri);
    if (t.isOrientable())
        t.orient();

    t.intelligentSimplify();

    EXPECT_EQ(t.size(), expectSize);
    EXPECT_EQ(t.isOriented(), test.tri.isOrientable());

    auto std = regina::StandardTriangulation::recognise(t);
    if (std)
        EXPECT_EQ(std->name(), expectName);
    else
        ADD_FAILURE() << "Simplified triangulation not recognised: "
            << t.isoSig();

    // Make sure it does not simplify any further.
    Triangulation<3> t2(t);
    EXPECT_FALSE(t2.intelligentSimplify());
    EXPECT_EQ(t2, t);
}

static void verifySimplificationSig(
        const TriangulationTest<3>::TestCase& test, const char* expectIsoSig) {
    SCOPED_TRACE_CSTRING(test.name);

    Triangulation<3> t(test.tri);
    if (t.isOrientable())
        t.orient();

    t.intelligentSimplify();

    EXPECT_EQ(t.isoSig(), expectIsoSig);
    EXPECT_EQ(t.isOriented(), test.tri.isOrientable());

    // Make sure it does not simplify any further.
    Triangulation<3> t2(t);
    EXPECT_FALSE(t2.intelligentSimplify());
    EXPECT_EQ(t2, t);
}

static void verifyNoSimplification(const TriangulationTest<3>::TestCase& test) {
    SCOPED_TRACE_CSTRING(test.name);

    Triangulation<3> t(test.tri);
    EXPECT_FALSE(t.intelligentSimplify());
    EXPECT_EQ(t, test.tri);
}

TEST_F(Dim3Test, simplification) {
    verifyNoSimplification(empty);
    verifySimplificationName(sphere, 1, "L(1,0)");
    verifySimplificationName(simpSphere, 1, "L(1,0)");
    verifyNoSimplification(sphereBundle);
    verifyNoSimplification(twistedSphereBundle);
    verifySimplificationName(ball, 1, "B3 (3-vtx)");
    verifySimplificationName(ballBundle, 1, "LST(1,2,3)");
    verifySimplificationSig(twistedBallBundle, "cHcbban");

    verifyNoSimplification(s3);
    verifyNoSimplification(rp3_1);
    verifySimplificationName(rp3_2, 2, "L(2,1)");
    verifySimplificationName(lens3_1, 2, "L(3,1) (1)");
    verifyNoSimplification(lens8_3);
    verifySimplificationName(lens7_1_loop, 4, "L(7,1)");
    verifyNoSimplification(rp3rp3);
    verifyNoSimplification(q32xz3);
    verifyNoSimplification(q28);
    verifyNoSimplification(weberSeifert);
    verifyNoSimplification(lens100_1);
    verifySimplificationName(s3_large, 1, "L(1,0)");
    verifySimplificationName(rp3_large, 2, "L(2,1)");
    verifySimplificationName(lens8_3_large, 2, "L(8,3)");
    verifySimplificationName(q20_large, 5, "C~(5)");

    verifyNoSimplification(rp2xs1);

    verifySimplificationName(lst3_4_7, 1, "LST(1,2,3)");
    verifySimplificationName(ball_bary, 1, "B3 (3-vtx)");
    verifySimplificationName(ball_large, 1, "B3 (3-vtx)");
    verifySimplificationName(ball_large_pillows, 1, "B3 (3-vtx)");
    verifySimplificationName(ball_large_snapped, 1, "B3 (3-vtx)");

    verifyNoSimplification(figure8);
    verifyNoSimplification(trefoil);
    // We exclude knot18: this can be simplified, but Regina does not always
    // find the simplification.  I expect that even if it does, this case is
    // large enough to have many possible non-isomorphic results.
    // verifyNoSimplification(knot18);
    // We also exclude idealGenusTwoHandlebody, which has many, many minimal
    // triangulations, making the resulting isosig unpredictable.  Moreover,
    // intelligentSimplify() does not actually find one (it only gets down to
    // 5 tetrahedra, but the minimal triangulations have 4).
    // verifySimplificationSig(idealGenusTwoHandlebody, "eLAkabcddbrgef");
    verifySimplificationName(figure8_bary, 2, "SnapPea m004");

    verifyNoSimplification(gieseking);
    verifySimplificationSig(idealRP2xI, "cMcabbgci");

    verifySimplificationSig(pinchedSolidTorus, "cHcbbad");
    verifySimplificationSig(pinchedSolidKB, "bGad");
    verifyNoSimplification(invalidRP2xI);

    // Note: disjoint2 contains idealGenusTwoHandlebody, which has many, many
    // minimal triangulations.  This makes the resulting isosig unpredictable,
    // and so we exclude it from this test.
    // verifySimplificationSig(disjoint2, "bkaaideLAkbccddapfeo");
    verifySimplificationSig(disjoint3, "bGabcMcabbjajcPcbbbiht");

    // A triangulation with two degree two projective plane cusps.
    // This has an internal vertex that should be removed,
    // but the two projective plane cusps should not be simplified away.
    verifySimplificationSig(
        { Triangulation<3>::rehydrate("cabbbbxww"), "cabbbbxww" }, "cMcabbgci");

    // A triangulation with an invalid edge that simplifies.
    // The invalid edge must not be simplified away.
    verifySimplificationSig({ Triangulation<3>::fromGluings(4, {
            { 0, 3, 2, {} }, { 0, 2, 1, {2,3} },
            { 3, 3, 2, {2,3} }, { 3, 2, 1, {1,0} }
        }), "Invalid edge" }, "bGah");

    // A solid torus that long ago was incorrectly simplified to a ball.
    verifySimplificationName({ Triangulation<3>::fromGluings(3, {
            { 2, 3, 2, {2,3} }, { 2, 1, 1, {0,2,3,1} }, { 2, 0, 0, {3,0,1,2} },
            { 1, 3, 0, {0,3,1,2} }, { 1, 1, 0, {} }
        }), "Custom solid torus" }, 1, "LST(1,2,3)");
}

static void verifySimplifyExhaustive(const char* isoSig,
        int heightNeeded, int nThreads) {
    SCOPED_TRACE_CSTRING(isoSig);

    Triangulation<3> t = Triangulation<3>::fromIsoSig(isoSig);
    size_t initSize = t.size();

    for (int height = 0; height < heightNeeded; ++height) {
        SCOPED_TRACE_NUMERIC(height);
        EXPECT_FALSE(t.simplifyExhaustive(height, nThreads));
        EXPECT_EQ(t.size(), initSize);
    }

    EXPECT_TRUE(t.simplifyExhaustive(heightNeeded, nThreads));
    EXPECT_NE(t.size(), initSize);
}

TEST_F(Dim3Test, simplifyExhaustive) {
    verifySimplifyExhaustive("hLALPkbcbefgfghxwnxark", 3, 1);
    verifySimplifyExhaustive("hLALPkbcbefgfghxwnxark", 3, 2);
}

static void verifyMinimiseBoundaryDoesNothing(const Triangulation<3>& tri,
        const char* name) {
    SCOPED_TRACE_CSTRING(name);

    EXPECT_TRUE(tri.hasMinimalBoundary());

    Triangulation<3> copy(tri);
    EXPECT_FALSE(copy.minimiseBoundary());
    EXPECT_EQ(copy, tri);
}

static void verifyMinimiseBoundary(const Triangulation<3>& tri,
        const char* name) {
    // minimiseBoundary() requires a valid triangulation.
    if (! tri.isValid())
        return;

    SCOPED_TRACE_CSTRING(name);

    if (tri.hasMinimalBoundary()) {
        Triangulation<3> copy(tri);
        EXPECT_FALSE(copy.minimiseBoundary());
        EXPECT_EQ(copy, tri);
    } else {
        Triangulation<3> copy(tri);
        if (copy.isOrientable())
            copy.orient();

        EXPECT_TRUE(copy.minimiseBoundary());
        EXPECT_TRUE(copy.hasMinimalBoundary());
        EXPECT_NE(copy.fVector(), tri.fVector());
        EXPECT_EQ(copy.eulerCharTri(), tri.eulerCharTri());
        EXPECT_EQ(copy.eulerCharManifold(), tri.eulerCharManifold());
        EXPECT_EQ(copy.isOriented(), tri.isOrientable());
        EXPECT_EQ(copy.homology(), tri.homology());

        // Check that we do not break 0-efficiency.
        // This test is exponential time, so we impose a size limit.
        if (tri.size() < 20) /* this uses exponential-time algorithms */
            if (! tri.nonTrivialSphereOrDisc())
                EXPECT_FALSE(copy.nonTrivialSphereOrDisc());
    }
}

TEST_F(Dim3Test, minimiseBoundary) {
    testManualCases(verifyMinimiseBoundary);
    runCensusAllClosed(verifyMinimiseBoundaryDoesNothing);
    runCensusAllIdeal(verifyMinimiseBoundaryDoesNothing);
    runCensusAllBounded(verifyMinimiseBoundary);

    // The cone of a 6-triangle torus whose boundary has no
    // close-book moves at the beginning (so a layering is required).
    {
        const char* sig = "gffjQafeefaaaa";
        verifyMinimiseBoundary(Triangulation<3>::fromIsoSig(sig), sig);
    }
}

static void verifyMinimiseVerticesDoesNothing(const Triangulation<3>& tri,
        const char* name) {
    SCOPED_TRACE_CSTRING(name);

    EXPECT_TRUE(tri.hasMinimalVertices());

    Triangulation<3> copy(tri);
    EXPECT_FALSE(copy.minimiseVertices());
    EXPECT_EQ(copy, tri);
}

static void verifyMinimiseVertices(const Triangulation<3>& tri,
        const char* name) {
    // minimiseVertices() requires a valid triangulation.
    if (! tri.isValid())
        return;

    SCOPED_TRACE_CSTRING(name);

    if (tri.hasMinimalVertices()) {
        Triangulation<3> copy(tri);
        EXPECT_FALSE(copy.minimiseVertices());
        EXPECT_EQ(copy, tri);
    } else {
        Triangulation<3> copy(tri);
        if (copy.isOrientable())
            copy.orient();

        EXPECT_TRUE(copy.minimiseVertices());
        EXPECT_TRUE(copy.hasMinimalVertices());
        EXPECT_NE(copy.fVector(), tri.fVector());
        EXPECT_EQ(copy.eulerCharTri(), tri.eulerCharTri());
        EXPECT_EQ(copy.eulerCharManifold(), tri.eulerCharManifold());
        EXPECT_EQ(copy.isOriented(), tri.isOrientable());
        EXPECT_EQ(copy.homology(), tri.homology());
    }
}

TEST_F(Dim3Test, minimiseVertices) {
    // Note: most of the running time for this test is in computing homology
    // for our handful of large subdivided test cases.
    testManualCases(verifyMinimiseVertices);
    runCensusAllClosed(verifyMinimiseVertices);
    runCensusAllIdeal(verifyMinimiseVertices);
    runCensusAllBounded(verifyMinimiseVertices);

    // The cone of a 6-triangle torus whose boundary has no
    // close-book moves at the beginning (so a layering is required).
    {
        const char* sig = "gffjQafeefaaaa";
        verifyMinimiseVertices(Triangulation<3>::fromIsoSig(sig), sig);
    }

    // Some disconnected triangulations where reductions are required.
    {
        Triangulation<3> t;
        t.insertTriangulation(ball_bary.tri);
        t.insertTriangulation(figure8_bary.tri);
        verifyMinimiseVertices(t, "Subdivided, disconnected, mixed boundaries");
    }
    {
        Triangulation<3> t = disjoint2.tri;
        t.subdivide();
        verifyMinimiseVertices(t, "Subdivided, disconnected, ideal boundaries");
    }
}

static void verifyPuncture(const Triangulation<3>& tri, const char* name) {
    if (tri.isEmpty())
        return;

    SCOPED_TRACE_CSTRING(name);

    std::optional<AbelianGroup> origH2;
    if (tri.isValid())
        origH2 = tri.homology<2>();

    for (size_t location = 0; location < tri.countTriangles(); ++location) {
        SCOPED_TRACE_NUMERIC(location);

        Triangulation<3> punc(tri);
        punc.puncture(punc.triangle(location));

        EXPECT_EQ(punc.size(), tri.size() + 6);
        EXPECT_EQ(punc.isValid(), tri.isValid());
        EXPECT_EQ(punc.isIdeal(), tri.isIdeal());
        EXPECT_EQ(punc.isStandard(), tri.isStandard());
        EXPECT_EQ(punc.isConnected(), tri.isConnected());
        EXPECT_EQ(punc.isOrientable(), tri.isOrientable());
        EXPECT_EQ(punc.isOriented(), tri.isOriented());
        EXPECT_FALSE(punc.isClosed());
        EXPECT_EQ(punc.countBoundaryComponents(),
            tri.countBoundaryComponents() + 1);
        EXPECT_EQ(punc.countBoundaryTriangles(),
            tri.countBoundaryTriangles() + 2);
        EXPECT_EQ(punc.eulerCharTri(), tri.eulerCharTri() + 1);
        EXPECT_EQ(punc.eulerCharManifold(), tri.eulerCharManifold() + 1);

        ASSERT_GE(punc.size(), 2);
        auto p = punc.tetrahedron(punc.size() - 2);
        auto q = punc.tetrahedron(punc.size() - 1);

        auto bc = q->triangle(0)->boundaryComponent();
        ASSERT_NE(bc, nullptr);
        EXPECT_EQ(bc, p->triangle(0)->boundaryComponent());
        EXPECT_EQ(bc->countTriangles(), 2);
        EXPECT_EQ(bc->eulerChar(), 2);
        EXPECT_EQ(q->vertex(1), p->vertex(1));
        EXPECT_EQ(q->vertex(2), p->vertex(3));
        EXPECT_EQ(q->vertex(3), p->vertex(2));

        if (tri.isValid()) {
            EXPECT_EQ(punc.homology<1>(), tri.homology<1>());

            AbelianGroup foundH2 = punc.homology<2>();
            auto component = tri.triangle(location)->component();
            if (component->isClosed()) {
                if (component->isOrientable()) {
                    // Puncture should not change H2.
                } else {
                    // Puncture should change X + Z_2 -> X + Z.
                    ASSERT_GE(foundH2.rank(), 1);
                    foundH2.addRank(-1);
                    foundH2.addTorsion(2);
                }
            } else {
                // Puncture should change X -> X + Z.
                ASSERT_GE(foundH2.rank(), 1);
                foundH2.addRank(-1);
            }
            EXPECT_EQ(foundH2, origH2);
        }
    }
}

TEST_F(Dim3Test, puncture) {
    testManualCases(verifyPuncture, false);
    runCensusAllClosed(verifyPuncture, true);
    runCensusAllBounded(verifyPuncture, true);
    runCensusAllIdeal(verifyPuncture, true);
}

static void verifyConnectedSumWithSelf(const Triangulation<3>& tri,
        const char* name) {
    if (tri.countComponents() != 1)
        return;

    SCOPED_TRACE_CSTRING(name);

    Triangulation<3> t(tri);
    t.connectedSumWith(t);

    {
        AbelianGroup g = tri.homology();
        g.addGroup(g);
        EXPECT_EQ(t.homology(), g);
    }

    EXPECT_EQ(t.isClosed(), tri.isClosed());
    EXPECT_EQ(t.isOrientable(), tri.isOrientable());
    EXPECT_EQ(t.isIdeal(), tri.isIdeal());

    // All of our remaining tests are for closed manifolds.
    // We will also restrict these tests to non-enormous examples, since they
    // use exponential-time algorithms (connected sum decomposition).
    if ((! tri.isClosed()) || tri.size() > 20)
        return;

    try {
        auto oldSummands = tri.summands();
        auto newSummands = t.summands();

        EXPECT_EQ(newSummands.size(), oldSummands.size() * 2);

        if (oldSummands.size() == 1) {
            // Both new summands should be homeomorphic to the original.
            for (const auto& s : newSummands)
                EXPECT_EQ(s.homology(), tri.homology());
        }
    } catch (const regina::UnsolvedCase&) {
        // This could be one of those cases where connected sum decomposition
        // legitimately fails.
        EXPECT_FALSE(tri.isOrientable());
    }
}

TEST_F(Dim3Test, connectedSumWithSelf) {
    testManualCases(verifyConnectedSumWithSelf, false);
}

static void verifyIdealToFinite(const Triangulation<3>& tri,
        const char* name) {
    SCOPED_TRACE_CSTRING(name);

    bool shouldTruncate = false;
    if (tri.isIdeal())
        shouldTruncate = true;
    else if (! tri.isValid()) {
        for (auto v : tri.vertices())
            if (! v->isValid()) {
                shouldTruncate = true;
                break;
            }
    }

    if (! shouldTruncate) {
        // The idealToFinite routine should leave tri unchanged.
        Triangulation<3> finite(tri);
        finite.idealToFinite();
        EXPECT_EQ(finite, tri);
        return;
    }

    Triangulation<3> finite(tri);
    finite.idealToFinite();

    // Ensure that properties we are about to verify are explicitly recomputed.
    clearProperties(finite);

    EXPECT_NE(finite, tri);
    EXPECT_TRUE(finite.hasBoundaryTriangles());
    EXPECT_FALSE(finite.isIdeal());
    EXPECT_EQ(finite.isOrientable(), tri.isOrientable());
    EXPECT_EQ(finite.isClosed(), tri.isClosed());
    EXPECT_EQ(finite.isConnected(), tri.isConnected());
    EXPECT_EQ(finite.countBoundaryComponents(), tri.countBoundaryComponents());

    for (auto v : finite.vertices())
        EXPECT_TRUE(v->linkType() == regina::Vertex<3>::SPHERE ||
            v->linkType() == regina::Vertex<3>::DISC);

    // Make sure any invalid edges are left alone.
    {
        size_t oldInvEdges = 0, newInvEdges = 0;
        for (auto e : tri.edges())
            if (! e->isValid())
                ++oldInvEdges;
        for (auto e : finite.edges())
            if (! e->isValid())
                ++newInvEdges;
        EXPECT_EQ(oldInvEdges, newInvEdges);
    }

    if (tri.isValid()) {
        // Subdivision can change invalid to valid, but never the other way.
        EXPECT_TRUE(finite.isValid());

        // Subdivisions can change these properties for invalid triangulations.
        EXPECT_EQ(finite.eulerCharManifold(), tri.eulerCharManifold());

        // Homology can only be computed for valid triangulations.
        finite.intelligentSimplify();
        EXPECT_EQ(finite.homology(), tri.homology());

        // All boundary components should be topologically the same.
        {
            using BdrySpec = std::pair<long, bool>;

            std::vector<BdrySpec> oldBdry;
            for (auto bc : tri.boundaryComponents())
                oldBdry.emplace_back(bc->eulerChar(), bc->isOrientable());
            std::sort(oldBdry.begin(), oldBdry.end());

            std::vector<BdrySpec> newBdry;
            for (auto bc : finite.boundaryComponents())
                newBdry.emplace_back(bc->eulerChar(), bc->isOrientable());
            std::sort(newBdry.begin(), newBdry.end());

            EXPECT_EQ(oldBdry, newBdry);
        }
    }
}

TEST_F(Dim3Test, idealToFinite) {
    testManualCases(verifyIdealToFinite);
}

static void verifyFiniteToIdeal(const Triangulation<3>& tri, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    if (! tri.hasBoundaryFacets()) {
        // The triangulation should remain unchanged.
        Triangulation<3> other(tri);
        other.finiteToIdeal();
        EXPECT_EQ(other, tri);
        return;
    }

    Triangulation<3> ideal(tri);
    ideal.finiteToIdeal();

    // Ensure that properties we are about to verify are explicitly recomputed.
    clearProperties(ideal);

    bool hasNonSphereBdry = false;
    for (auto bc : tri.boundaryComponents())
        if (bc->build().eulerCharTri() != 2) {
            hasNonSphereBdry = true;
            break;
        }

    size_t invalidEdges = 0;
    for (auto e : tri.edges())
        if (! e->isValid())
            ++invalidEdges;

    EXPECT_NE(ideal, tri);
    EXPECT_FALSE(ideal.hasBoundaryTriangles());
    EXPECT_EQ(ideal.isValid(), invalidEdges == 0);
    EXPECT_EQ(ideal.isIdeal(), hasNonSphereBdry);
    EXPECT_EQ(ideal.isOrientable(), tri.isOrientable());
    EXPECT_EQ(ideal.isClosed(), ! hasNonSphereBdry);
    EXPECT_EQ(ideal.isConnected(), tri.isConnected());

    // Make sure any invalid edges are left alone.
    {
        size_t newInvalidEdges = 0;
        for (auto e : ideal.edges())
            if (! e->isValid())
                ++newInvalidEdges;
        EXPECT_EQ(invalidEdges, newInvalidEdges);
    }

    if (tri.isValid()) {
        // Homology can only be computed for valid triangulations.
        ideal.intelligentSimplify();
        EXPECT_EQ(ideal.homology(), tri.homology());

        // All boundary components should be topologically the same, with the
        // exception that S^2 boundary components should be filled with balls.
        {
            using BdrySpec = std::pair<long, bool>;

            std::vector<BdrySpec> oldBdry;
            for (auto bc : tri.boundaryComponents())
                if (bc->eulerChar() != 2)
                    oldBdry.emplace_back(bc->eulerChar(), bc->isOrientable());
            std::sort(oldBdry.begin(), oldBdry.end());

            std::vector<BdrySpec> newBdry;
            for (auto bc : ideal.boundaryComponents())
                newBdry.emplace_back(bc->eulerChar(), bc->isOrientable());
            std::sort(newBdry.begin(), newBdry.end());

            EXPECT_EQ(oldBdry, newBdry);
        }
    }
}

TEST_F(Dim3Test, finiteToIdeal) {
    testManualCases(verifyFiniteToIdeal);
}

static void verifyRetriangulate(const Triangulation<3>& tri,
        int height, int threads, bool track, size_t count) {
    SCOPED_TRACE_NUMERIC(height);
    SCOPED_TRACE_NUMERIC(threads);

    size_t tot = 0;

    std::unique_ptr<regina::ProgressTrackerOpen> tracker;
    if (track)
        tracker.reset(new regina::ProgressTrackerOpen());

    bool result = tri.retriangulate(height, threads, tracker.get(),
        [&tot, &tri](const Triangulation<3>& alt) {
            ++tot;
            EXPECT_EQ(alt.isValid(), tri.isValid());
            EXPECT_EQ(alt.isOrientable(), tri.isOrientable());
            EXPECT_EQ(alt.countBoundaryComponents(),
                tri.countBoundaryComponents());
            EXPECT_EQ(alt.homology(), tri.homology());
            return false;
        });
    if (track)
        EXPECT_TRUE(tracker->isFinished());
    EXPECT_FALSE(result);
    EXPECT_EQ(tot, count);
}

static void verifyRetriangulate(const TriangulationTest<3>::TestCase& test,
        int height, size_t count) {
    SCOPED_TRACE_CSTRING(test.name);

    // Single-threaded, no tracker:
    verifyRetriangulate(test.tri, height, 1, false, count);
    // Multi-threaded, with and without tracker:
    verifyRetriangulate(test.tri, height, 2, false, count);
    verifyRetriangulate(test.tri, height, 2, true, count);
}

TEST_F(Dim3Test, retriangulate) {
    // The counts here were computed using Regina 6.0 in single-threaded mode.

    verifyRetriangulate(s3, 0, 1);
    verifyRetriangulate(s3, 1, 1);
    verifyRetriangulate(s3, 2, 1);
    verifyRetriangulate(s3, 3, 1);
    verifyRetriangulate(figure8, 0, 1);
    verifyRetriangulate(figure8, 1, 2);
    verifyRetriangulate(figure8, 2, 6);
    verifyRetriangulate(figure8, 3, 61);
    verifyRetriangulate(weberSeifert, 0, 1);
    verifyRetriangulate(weberSeifert, 1, 10);
    verifyRetriangulate(lst3_4_7, 0, 1);
    verifyRetriangulate(lst3_4_7, 1, 3);
    verifyRetriangulate(lst3_4_7, 2, 13);
    verifyRetriangulate(lst3_4_7, 3, 219);
    verifyRetriangulate(q20_large, 0, 1);
    verifyRetriangulate(q20_large, 1, 195);
    verifyRetriangulate(q20_large, 2, 2997);
}

static void verifyAngleStructures(const Triangulation<3>& tri,
        const char* name) {
    SCOPED_TRACE_CSTRING(name);

    // Should there be a generalised angle structure?
    bool expectGeneral = true;
    for (auto v : tri.vertices())
        if (v->isLinkClosed() && v->linkEulerChar() != 0) {
            expectGeneral = false;
            break;
        }

    // Should there be a strict angle structure?
    bool expectStrict = false;
    if (expectGeneral) {
        if (tri.isEmpty()) {
            // The empty angle structure is simultaneously strict and taut.
            expectStrict = true;
        } else if (tri.hasBoundaryFacets()) {
            // All of our manual cases with boundary facets have strict angle
            // structures, and we do not run this through a census with boundary
            // (since this is not a particularly meaningful scenario).  So for
            // the purposes of the test suite, we just set expectStrict to true.
            expectStrict = true;
        } else if (tri.isConnected()) {
            // Since we only run this through a small census, we optimistically
            // assume that there is a strict angle structure iff SnapPea is able
            // to find a geometric structure on the triangulation.
            //
            // While this is not a guaranteed theorem (*), it works very well
            // in practice, and importantly it gives us a way to independently
            // verify Regina's strict angle structure test.
            //
            // (*) The caveats: SnapPea could get things wrong due to round-off
            // error, and also it is possible to have a strict angle structure
            // without fully satisfying Thurston's gluing equations.  But again,
            // neither of these discrepancies are actually observed in very
            // small cases such as these, so we will happily assume that
            // neither happens for the purpose of this test suite.
            if (regina::SnapPeaTriangulation(tri).solutionType() ==
                    regina::SnapPeaTriangulation::geometric_solution)
                expectStrict = true;
        } else {
            // SnapPea does not handle multiple components well.  Run the same
            // test as before, but on each component separately.
            expectStrict = true;
            for (const auto& c : tri.triangulateComponents()) {
                if (regina::SnapPeaTriangulation(c).solutionType() !=
                        regina::SnapPeaTriangulation::geometric_solution) {
                    expectStrict = false;
                    break;
                }
            }
        }
    }

    // Test the triangulation routines that actually find angle structures.
    regina::MatrixInt m = regina::makeAngleEquations(tri);

    // Ensure the strict vs general computations are completely independent.
    Triangulation<3> testGeneral = tri;
    Triangulation<3> testStrict = tri;

    if (expectGeneral) {
        EXPECT_TRUE(testGeneral.hasGeneralAngleStructure());
        EXPECT_NO_THROW({
            const auto& a = testGeneral.generalAngleStructure();
            const auto& v = a.vector();
            EXPECT_EQ(v.size(), m.columns());
            EXPECT_TRUE((m * v).isZero());
            EXPECT_GT(v[v.size() - 1], 0);
        });
    } else {
        EXPECT_FALSE(testGeneral.hasGeneralAngleStructure());
        EXPECT_THROW({ testGeneral.generalAngleStructure(); },
            regina::NoSolution);
    }

    if (expectStrict) {
        EXPECT_TRUE(testStrict.hasStrictAngleStructure());
        EXPECT_NO_THROW({
            const auto& a = testStrict.strictAngleStructure();
            const auto& v = a.vector();
            EXPECT_TRUE(a.isStrict());
            EXPECT_EQ(v.size(), m.columns());
            EXPECT_TRUE((m * v).isZero());
            EXPECT_GT(v[v.size() - 1], 0);
        });
    } else {
        EXPECT_FALSE(testStrict.hasStrictAngleStructure());
        EXPECT_THROW({ testStrict.strictAngleStructure(); },
            regina::NoSolution);
    }
}

TEST_F(Dim3Test, angleStructures) {
    testManualCases(verifyAngleStructures);
    runCensusAllIdeal(verifyAngleStructures);

    // Test a disjoint union of two triangulations that both have strict angle
    // structures.

    {
        Triangulation<3> t = figure8.tri;
        t.insertTriangulation(gieseking.tri);
        verifyAngleStructures(t, "Figure eight U Gieseking");
    }
}

static void verifyZeroEfficiency(const Triangulation<3>& tri,
        const char* name) {
    SCOPED_TRACE_CSTRING(name);

    bool zeroEfficient = tri.isZeroEfficient();

    if (zeroEfficient) {
        // Verify some properties that we know about 0-efficient triangulations.
        if (tri.isClosed() && tri.isConnected()) {
            EXPECT_LE(tri.countVertices(), 2);
            if (tri.countVertices() == 2)
                EXPECT_EQ(tri.homology(), AbelianGroup());
        }
    }

    // Test it the long way, directly from the definition.
    bool expected;
    if (tri.hasTwoSphereBoundaryComponents())
        expected = false;
    else {
        expected = true;
        regina::NormalSurfaces s(tri, regina::NS_STANDARD);
        for (const regina::NormalSurface& f : s) {
            if (! f.isVertexLinking()) {
                if (f.eulerChar() == 2 && (! f.hasRealBoundary())) {
                    // Non-trivial normal sphere
                    expected = false;
                    break;
                } else if (f.eulerChar() == 1 && (! f.hasRealBoundary()) &&
                        (! f.isTwoSided())) {
                    // Normal projective plane that doubles to a normal sphere
                    expected = false;
                    break;
                } else if (f.eulerChar() == 1 && f.hasRealBoundary()) {
                    // Non-trivial normal disc
                    expected = false;
                    break;
                }
            }
        }
    }
    EXPECT_EQ(zeroEfficient, expected);
}

TEST_F(Dim3Test, zeroEfficiency) {
    EXPECT_TRUE(empty.tri.isZeroEfficient());
    EXPECT_FALSE(sphere.tri.isZeroEfficient());
    EXPECT_FALSE(simpSphere.tri.isZeroEfficient());
    EXPECT_FALSE(sphereBundle.tri.isZeroEfficient());
    EXPECT_FALSE(twistedSphereBundle.tri.isZeroEfficient());
    EXPECT_FALSE(ball.tri.isZeroEfficient());
    EXPECT_FALSE(ballBundle.tri.isZeroEfficient());
    EXPECT_FALSE(twistedBallBundle.tri.isZeroEfficient());

    EXPECT_TRUE(s3.tri.isZeroEfficient());
    EXPECT_FALSE(rp3_1.tri.isZeroEfficient());
    EXPECT_FALSE(rp3_2.tri.isZeroEfficient());
    EXPECT_FALSE(lens3_1.tri.isZeroEfficient());
    EXPECT_TRUE(lens8_3.tri.isZeroEfficient());
    EXPECT_FALSE(lens7_1_loop.tri.isZeroEfficient());
    EXPECT_FALSE(rp3rp3.tri.isZeroEfficient());
    EXPECT_TRUE(q32xz3.tri.isZeroEfficient());
    EXPECT_TRUE(q28.tri.isZeroEfficient());
    // Skip weberSeifert, which consumes about 5 seconds on Ben's machine.
    // Skip lens100_1, which is too large (and therefore too slow).
    EXPECT_FALSE(s3_large.tri.isZeroEfficient());
    EXPECT_FALSE(rp3_large.tri.isZeroEfficient());
    EXPECT_FALSE(lens8_3_large.tri.isZeroEfficient());
    EXPECT_FALSE(q20_large.tri.isZeroEfficient());

    // rp2xs1 contains a two-sided RP^2, but not a one-sided RP^2.
    EXPECT_TRUE(rp2xs1.tri.isZeroEfficient());

    EXPECT_FALSE(lst3_4_7.tri.isZeroEfficient());
    EXPECT_FALSE(ball_bary.tri.isZeroEfficient());
    EXPECT_FALSE(ball_large.tri.isZeroEfficient());
    EXPECT_FALSE(ball_large_pillows.tri.isZeroEfficient());
    EXPECT_FALSE(ball_large_snapped.tri.isZeroEfficient());

    EXPECT_TRUE(figure8.tri.isZeroEfficient());
    EXPECT_TRUE(trefoil.tri.isZeroEfficient());
    EXPECT_TRUE(knot18.tri.isZeroEfficient());
    EXPECT_TRUE(idealGenusTwoHandlebody.tri.isZeroEfficient());
    // figure8_bary is too slow (due to the barycentric subdivision).

    EXPECT_TRUE(gieseking.tri.isZeroEfficient());
    // idealRP2xI is too slow (due to the barycentric subdivision).

    EXPECT_FALSE(pinchedSolidTorus.tri.isZeroEfficient());
    EXPECT_FALSE(pinchedSolidKB.tri.isZeroEfficient());
    // In invalidRP2xI, the links of the valid edges are spheres.
    EXPECT_FALSE(invalidRP2xI.tri.isZeroEfficient());

    EXPECT_TRUE(disjoint2.tri.isZeroEfficient());
    EXPECT_FALSE(disjoint3.tri.isZeroEfficient());

    runCensusAllClosed(verifyZeroEfficiency);
    runCensusAllBounded(verifyZeroEfficiency);
    runCensusAllIdeal(verifyZeroEfficiency);
}

TEST_F(Dim3Test, irreducibility) {
    // Preconditions for isIrreducible(): valid, closed, orientable, connected.

    EXPECT_TRUE(empty.tri.isIrreducible());
    EXPECT_TRUE(sphere.tri.isIrreducible());
    EXPECT_TRUE(simpSphere.tri.isIrreducible());
    EXPECT_FALSE(sphereBundle.tri.isIrreducible());
    EXPECT_FALSE(twistedSphereBundle.tri.isIrreducible());

    EXPECT_TRUE(s3.tri.isIrreducible());
    EXPECT_TRUE(rp3_1.tri.isIrreducible());
    EXPECT_TRUE(rp3_2.tri.isIrreducible());
    EXPECT_TRUE(lens3_1.tri.isIrreducible());
    EXPECT_TRUE(lens8_3.tri.isIrreducible());
    EXPECT_TRUE(lens7_1_loop.tri.isIrreducible());
    EXPECT_FALSE(rp3rp3.tri.isIrreducible());
    EXPECT_TRUE(q32xz3.tri.isIrreducible());
    EXPECT_TRUE(q28.tri.isIrreducible());
    // Skip weberSeifert, which consumes about 10 seconds on Ben's machine.
    // Skip lens100_1, which is too large (and therefore too slow).
    EXPECT_TRUE(s3_large.tri.isIrreducible());
    EXPECT_TRUE(rp3_large.tri.isIrreducible());
    EXPECT_TRUE(lens8_3_large.tri.isIrreducible());
    EXPECT_TRUE(q20_large.tri.isIrreducible());
}

static void verifySphere(const Triangulation<3>& tri, bool expected) {
    // The caller must add their own scoped trace before calling this routine.

    clearProperties(tri);
    EXPECT_EQ(tri.isSphere(), expected);

    if (tri.isValid() && tri.isClosed() && tri.isOrientable() &&
            tri.isConnected()) {
        clearProperties(tri);
        EXPECT_EQ(tri.summands().empty(), expected);
    }
}

static void verifySphere6(const Triangulation<3>& tri, const char* name) {
    // PRECONDITION: tri is from the closed census, and has ≤ 6 tetrahedra.
    //
    // In the closed orientable census, the only homology 3-sphere with
    // ≤ 6 tetrahedra is the Poincare homology 3-sphere (which requires
    // at least 5 tetrahedra).  This can be distinguished from the real
    // 3-sphere using the (5,1) Turaev-Viro invariant: for S^3 the invariant
    // is 0.138197, and for the Poincare homology sphere it is 0.947214.

    SCOPED_TRACE_CSTRING(name);

    ASSERT_TRUE(tri.isValid());
    ASSERT_TRUE(tri.isClosed());
    ASSERT_TRUE(tri.isConnected());
    ASSERT_LE(tri.size(), 6);

    if (tri.isOrientable()) {
        // We can check both isSphere() and summands() here.
        bool expect = tri.homology().isTrivial() &&
            (tri.size() < 5 || tri.turaevViroApprox(5, 1) < 0.5);
        EXPECT_EQ(tri.isSphere(), expect);

        clearProperties(tri);
        EXPECT_EQ(tri.summands().empty(), expect);
    } else {
        // In the non-orientable case, summands() _could_ throw an exception.
        // However, let's be brave and hope it works for small census cases.
        EXPECT_FALSE(tri.isSphere());

        clearProperties(tri);
        EXPECT_FALSE(tri.summands().empty());
    }
}

static void verifySphere(const TriangulationTest<3>::TestCase& test,
        bool expected) {
    SCOPED_TRACE_CSTRING(test.name);
    verifySphere(test.tri, expected);
}

static void verifySphere(const char* stringRep, bool expected) {
    SCOPED_TRACE_CSTRING(stringRep);
    verifySphere(Triangulation<3>(stringRep), expected);
}

static void verifySphereFromSignature(const char* signature, bool expected) {
    SCOPED_TRACE_CSTRING(signature);
    verifySphere(regina::Signature(signature).triangulate(), expected);
}

TEST_F(Dim3Test, sphereRecognition) {
    // A selection of cases that are trivially not spheres:
    verifySphere(empty, false);
    verifySphere(sphereBundle, false);
    verifySphere(twistedSphereBundle, false);
    verifySphere(rp3_1, false);
    verifySphere(rp3_2, false);
    verifySphere(lens3_1, false);
    verifySphere(lens8_3, false);
    verifySphere(rp3rp3, false);
    verifySphere(q32xz3, false);
    verifySphere(q28, false);
    verifySphere(rp2xs1, false);
    verifySphere(ball, false);
    verifySphere(lst3_4_7, false);
    verifySphere(figure8, false);
    verifySphere(idealRP2xI, false);
    verifySphere(pinchedSolidTorus, false);
    verifySphere(pinchedSolidKB, false);
    verifySphere(disjoint2, false);
    verifySphere(disjoint3, false);

    // Some simple cases that are spheres:
    verifySphere(sphere, true);
    verifySphere(simpSphere, true);
    verifySphere(s3, true);
    verifySphere(s3_large, true);

    // Spheres obtained from splitting surface signatures:
    verifySphereFromSignature("(a)(a)", true);
    verifySphereFromSignature("(ab)(a)(b)", true);
    verifySphereFromSignature("(ab)(a)(b)", true);
    verifySphereFromSignature("(abc)(a)(b)(c)", true);
    verifySphereFromSignature("(ab)(ac)(b)(c)", true);
    verifySphereFromSignature("(abcd)(a)(b)(c)(d)", true);
    verifySphereFromSignature("(abc)(abd)(c)(d)", true);
    verifySphereFromSignature("(abc)(acd)(b)(d)", true);
    verifySphereFromSignature("(abc)(ad)(b)(c)(d)", true);
    verifySphereFromSignature("(ab)(ac)(bd)(cd)", true);
    verifySphereFromSignature("(ab)(ac)(bd)(c)(d)", true);
    verifySphereFromSignature("(abcd)(aefg)(b)(c)(d)(e)(f)(g)", true);

    // Non-spheres obtained from splitting surface signatures:
    verifySphereFromSignature("(aab)(b)", false);
    verifySphereFromSignature("(ab)(ab)", false);
    verifySphereFromSignature("(aabcb)(c)", false);
    verifySphereFromSignature("(aabc)(b)(c)", false);
    verifySphereFromSignature("(aabcdcb)(d)", false);
    verifySphereFromSignature("(aabcdb)(c)(d)", false);
    verifySphereFromSignature("(aabcd)(b)(c)(d)", false);
    verifySphereFromSignature("(aabc)(bd)(c)(d)", false);
    verifySphereFromSignature("(abac)(bd)(cd)", false);
    verifySphereFromSignature("(abac)(bd)(c)(d)", false);
    verifySphereFromSignature("(abcd)(ac)(bd)", false);
    verifySphereFromSignature("(aab)(bc)(cd)(d)", false);
    verifySphereFromSignature("(abc)(ab)(cd)(d)", false);
    verifySphereFromSignature("(abc)(ad)(bd)(c)", false);
    verifySphereFromSignature("(abac)(b)(c)", false);
    verifySphereFromSignature("(abacdc)(b)(d)", false);
    verifySphereFromSignature("(abcabd)(c)(d)", false);
    verifySphereFromSignature("(abacd)(b)(c)(d)", false);
    verifySphereFromSignature("(aabc)(bd)(cd)", false);
    verifySphereFromSignature("(abacde)(cf)(fg)(b)(d)(e)(g)", false);
    verifySphereFromSignature("(abc)(abc)", false);
    verifySphereFromSignature("(abc)(acb)", false);

    verifySphereFromSignature("(aa)", false);
    verifySphereFromSignature("(aabb)", false);
    verifySphereFromSignature("(abab)", false);
    verifySphereFromSignature("(aabccb)", false);
    verifySphereFromSignature("(abcabc)", false);
    verifySphereFromSignature("(aab)(bcc)", false);
    verifySphereFromSignature("(aab)(bc)(c)", false);
    verifySphereFromSignature("(ab)(ac)(bc)", false);
    verifySphereFromSignature("(aabcddcb)", false);
    verifySphereFromSignature("(abcdabcd)", false);
    verifySphereFromSignature("(aabbc)(cd)(d)", false);
    verifySphereFromSignature("(aabcb)(cdd)", false);
    verifySphereFromSignature("(aabcb)(cd)(d)", false);
    verifySphereFromSignature("(ababc)(cd)(d)", false);
    verifySphereFromSignature("(abac)(bdcd)", false);
    verifySphereFromSignature("(abac)(bcd)(d)", false);
    verifySphereFromSignature("(abac)(bdd)(c)", false);
    verifySphereFromSignature("(abcd)(abcd)", false);
    verifySphereFromSignature("(abcd)(adcb)", false);
    verifySphereFromSignature("(aab)(bcd)(c)(d)", false);
    verifySphereFromSignature("(abc)(abd)(cd)", false);
    verifySphereFromSignature("(abc)(acd)(bd)", false);
    verifySphereFromSignature("(abcdefgh)(abcdefgh)", false);

    verifySphereFromSignature("(aabccd)(b)(d)", false);
    verifySphereFromSignature("(abacbd)(cd)", false);
    verifySphereFromSignature("(aabcdecb)(dfeg)(fg)", false);
    verifySphereFromSignature("(aabbc)(cdef)(egg)(d)(f)", false);
    verifySphereFromSignature("(ababc)(cdef)(egg)(d)(f)", false);
    verifySphereFromSignature("(abcd)(acbe)(dfeg)(f)(g)", false);
    verifySphereFromSignature("(abcd)(adce)(befg)(f)(g)", false);
    verifySphereFromSignature("(abcde)(aff)(cgg)(b)(d)(e)", false);
    verifySphereFromSignature("(abcde)(adf)(bfg)(ce)(g)", false);
    verifySphereFromSignature("(ababc)(cde)(dfg)(fg)(e)", false);
    verifySphereFromSignature("(abcdef)(acegg)(bdf)", false);
    verifySphereFromSignature("(abacde)(dffgg)(b)(c)(e)", false);
    verifySphereFromSignature("(aabccdeffeg)(b)(d)(g)", false);
    verifySphereFromSignature("(aabcde)(cfg)(dgf)(b)(e)", false);

    // And of course the Poincare homology sphere(S3/P120).
    // We'll build this a few different ways.
    {
        SCOPED_TRACE("Poincare (example)");
        verifySphere(Example<3>::poincare(), false);
    }
    {
        // Poincare homology sphere as a plugged triangular solid torus:
        SCOPED_TRACE("Poincare (plugged)");
        verifySphere(Triangulation<3>::fromGluings(5, {
            { 0, 0, 4, {1,0,2,3} },
            { 0, 1, 3, {0,2,3,1} },
            { 0, 2, 1, {0,1,3,2} },
            { 0, 3, 2, {2,1,3,0} },
            { 1, 0, 3, {1,3,2,0} },
            { 1, 1, 2, {0,2,3,1} },
            { 1, 2, 4, {2,1,0,3} },
            { 2, 1, 4, {0,2,3,1} },
            { 2, 3, 3, {3,1,2,0} },
            { 3, 3, 4, {0,1,2,3} }
        }), false);
    }
    {
        SCOPED_TRACE("Poincare (augmented I)");
        verifySphere(regina::Example<3>::augTriSolidTorus(2, -1, 3, 1, 5, -4),
            false);
    }
    {
        SCOPED_TRACE("Poincare (augmented II)");
        verifySphere(regina::Example<3>::augTriSolidTorus(2, -1, 3, -2, 5, 1),
            false);
    }

    // A disjoint union of two spheres:
    {
        SCOPED_TRACE("S^3 U S^3");
        Triangulation<3> tri = s3.tri;
        tri.insertTriangulation(s3.tri);
        verifySphere(tri, false);
    }

    // Homology spheres obtained from the hyperbolic census:
    verifySphere("kLLLPLQkccfeghjiijjlnahgnqqadk", false);
    verifySphere("lLLLLPMQcbcgfhhihjkkktsmgsojfldor", false);
    verifySphere("lLvAvMQQcbefjjkiihkjklaljfxrkvufd", false);
    verifySphere("lvLAAzMQcdefegihjkkjkjwarujwdaapj", false);

    // 3-sphere triangulations that are difficult to simplify
    // (taken from the 2013 Hyamfest paper on Regina).
    // We comment out every second one of these tests for speed.
    verifySphere("jLLvQPQbeghfifiihxxaaxxjxar", true);
    //verifySphere("jLLLAQPbeggfhhhiihhlhegbgdw", true);
    verifySphere("jLLLPQPaegdehfgiibddbsabspr", true);
    //verifySphere("jLLLPQPaegdehfgiibddbsabspk", true);
    verifySphere("jLLLPQPaegdehfgiibddbsabwpr", true);
    //verifySphere("jLLLPQPaegdehfgiibddbsabwpk", true);
    verifySphere("jLLLPQPaegdehfgiibddbsabsdw", true);
    //verifySphere("jLLLPQPaegdehfgiibddbsabsds", true);
    verifySphere("kLAzwwQkbcbdehjihijhlwggliajgr", true);
    //verifySphere("kLAzwwQkbcbdehjihijhlwggliajgk", true);
    verifySphere("kLAzwwQkbcbdehjihijhlwggliawgr", true);
    //verifySphere("kLAzwwQkbcbdehjihijhlwggliawgk", true);
    verifySphere("kLAzwwQkbcbdehjihijhlwggllajgw", true);
    //verifySphere("kLAzwwQkbcbdehjihijhlwggllajgj", true);
    verifySphere("kLAzwwQkbcbdehjihijhlwggllawgw", true);
    //verifySphere("kLAzwwQkbcbdehjihijhlwggllawgj", true);
    verifySphere("kLAzwwQkbcbdehjihijhlwggiiargr", true);
    //verifySphere("kLAzwwQkbcbdehjihijhlwggiiargk", true);
    verifySphere("kLAzwwQkbcbdehjihijhlwggiiakgr", true);
    //verifySphere("kLAzwwQkbcbdehjihijhlwggiiakgk", true);
    verifySphere("kLAzwwQkbcbdehjihijhlwggilargw", true);
    //verifySphere("kLAzwwQkbcbdehjihijhlwggilargj", true);
    verifySphere("kLAzwwQkbcbdehjihijhlwggilakgw", true);
    //verifySphere("kLAzwwQkbcbdehjihijhlwggilakgj", true);
    verifySphere("kLLLAPPkbeggfihjiijhhlhehgdahw", true);
    //verifySphere("kLLvQPPkbeghfifjhjjxxaaxxjxrvc", true);

    // An exhaustive census run:
    runCensusMinClosed(&verifySphere6);
    runCensusAllClosed(&verifySphere6);
}

static void verifyBall(const TriangulationTest<3>::TestCase& test,
        bool expected) {
    SCOPED_TRACE_CSTRING(test.name);
    EXPECT_EQ(test.tri.isBall(), expected);
}

static void verifyBall(const char* stringRep, bool expected) {
    SCOPED_TRACE_CSTRING(stringRep);
    EXPECT_EQ(Triangulation<3>(stringRep).isBall(), expected);
}

TEST_F(Dim3Test, ballRecognition) {
    // A selection of cases that are trivially not balls:
    verifyBall(empty, false);
    verifyBall(s3, false);
    verifyBall(rp3_1, false);
    verifyBall(ballBundle, false);
    verifyBall(twistedBallBundle, false);
    verifyBall(lst3_4_7, false);
    verifyBall("cMcabbgds", false); // Ideal solid torus
    verifyBall(idealRP2xI, false);
    verifyBall(pinchedSolidTorus, false);
    verifyBall(pinchedSolidKB, false);
    verifyBall(disjoint2, false);
    verifyBall(disjoint3, false);

    // Some simple cases that are balls:
    verifyBall(ball, true);
    verifyBall(ball_bary, true);
    verifyBall(ball_large, true);
    verifyBall(ball_large_pillows, true);
    verifyBall(ball_large_snapped, true);

    // Some hand-crafted balls:
    {
        SCOPED_TRACE("Snapped tetrahedron");
        EXPECT_TRUE(Triangulation<3>::fromGluings(1, {
            { 0, 0, 0, {3,1,2,0} }
        }).isBall());
    }
    {
        SCOPED_TRACE("Triangular pillow");
        EXPECT_TRUE(Triangulation<3>::fromGluings(2, {
            { 0, 0, 1, {} },
            { 0, 1, 1, {} },
            { 0, 2, 1, {} }
        }).isBall());
    }
    {
        // This ball used to crash the simplification routines once
        // upon a time.  Throw it into the test suite for good measure.
        SCOPED_TRACE("4-tetrahedron ball");
        EXPECT_TRUE(Triangulation<3>::fromGluings(4, {
            { 0, 2, 0, {0,2} },
            { 0, 1, 1, {2,0,1,3} },
            { 1, 2, 2, {} },
            { 1, 1, 2, {2,0,1,3} },
            { 2, 1, 3, {2,0,1,3} },
            { 3, 2, 3, {1,2} }
        }).isBall());
    }

    // A punctured Poincare homology sphere:
    {
        SCOPED_TRACE("Punctured Poincare homology sphere");
        Triangulation<3> tri = Example<3>::poincare();
        tri.puncture();
        EXPECT_FALSE(tri.isBall());
    }

    // A disjoint union of two balls:
    {
        SCOPED_TRACE("B^3 U B^3");
        Triangulation<3> tri;
        tri.newTetrahedra(2);
        EXPECT_FALSE(tri.isBall());
    }
}

static void verifySolidTorus4(const Triangulation<3>& tri, const char* name) {
    // PRECONDITION: tri is from the bounded census, and has ≤ 4 tetrahedra.
    //
    // In the bounded census, the only orientable triangulations with
    // torus boundary and homology Z with ≤ 4 tetrahedra that are _not_
    // solid tori are isosigs eHLObcdddwun and eHLObcdddwuj, both representing
    // SFS [D: (2,1) (3,-2)].

    SCOPED_TRACE_CSTRING(name);

    ASSERT_TRUE(tri.isValid());
    ASSERT_FALSE(tri.isIdeal());
    ASSERT_TRUE(tri.isConnected());
    ASSERT_TRUE(tri.hasBoundaryFacets());
    ASSERT_LE(tri.size(), 4);

    if (tri.isOrientable() &&
            tri.countBoundaryComponents() == 1 &&
            tri.boundaryComponent(0)->eulerChar() == 0 &&
            tri.homology().isZ()) {
        if (tri.size() < 4) {
            EXPECT_TRUE(tri.isSolidTorus());
        } else {
            std::string sig = tri.isoSig();
            EXPECT_EQ(tri.isSolidTorus(),
                (sig == "eHLObcdddwun" || sig == "eHLObcdddwuj"));
        }
    } else {
        EXPECT_FALSE(tri.isSolidTorus());
    }
}

static void verifySolidTorus(const Triangulation<3>& tri, bool expected) {
    // The caller must add their own scoped trace before calling this routine.

    Triangulation<3> bounded(tri);
    if (bounded.isIdeal())
        bounded.idealToFinite();
    clearProperties(bounded);

    Triangulation<3> ideal(tri);
    if (ideal.hasBoundaryTriangles())
        ideal.finiteToIdeal();
    clearProperties(ideal);

    EXPECT_EQ(bounded.isSolidTorus(), expected);
    EXPECT_EQ(ideal.isSolidTorus(), expected);
}

static void verifySolidTorus(const TriangulationTest<3>::TestCase& test,
        bool expected) {
    SCOPED_TRACE_CSTRING(test.name);
    verifySolidTorus(test.tri, expected);
}

static void verifySolidTorus(const char* stringRep, bool expected) {
    SCOPED_TRACE_CSTRING(stringRep);
    verifySolidTorus(Triangulation<3>(stringRep), expected);
}

TEST_F(Dim3Test, solidTorusRecognition) {
    // A selection of cases that are trivially not solid tori:
    verifySolidTorus(empty, false);
    verifySolidTorus(s3, false);
    verifySolidTorus(rp3_1, false);
    verifySolidTorus(ball, false);
    verifySolidTorus(ball_bary, false);
    verifySolidTorus(ball_large, false);
    verifySolidTorus(ball_large_pillows, false);
    verifySolidTorus(ball_large_snapped, false);
    verifySolidTorus(idealRP2xI, false);
    verifySolidTorus(pinchedSolidTorus, false);
    verifySolidTorus(pinchedSolidKB, false);
    verifySolidTorus(disjoint2, false);
    verifySolidTorus(disjoint3, false);

    // Some simple cases that are solid tori:
    verifySolidTorus(ballBundle, true);
    verifySolidTorus(lst3_4_7, true);
    verifySolidTorus("cMcabbgds", true); // Ideal solid torus

    // Some non-solid-tori with the right boundary and homology:
    verifySolidTorus(figure8, false);
    verifySolidTorus(trefoil, false);
    // Leave out knot18, which is too large and therefore too slow.

    // Some non-minimal layered solid tori:
    {
        SCOPED_TRACE_CSTRING("LST(1,2,3)");
        verifySolidTorus(Example<3>::lst(1, 2), true);
    }
    {
        SCOPED_TRACE_CSTRING("LST(1,20,21)");
        verifySolidTorus(Example<3>::lst(1, 20), true);
    }
    {
        SCOPED_TRACE_CSTRING("LST(1,1,2)");
        verifySolidTorus(Example<3>::lst(1, 1), true);
    }
    {
        SCOPED_TRACE_CSTRING("LST(0,1,1)");
        verifySolidTorus(Example<3>::lst(0, 1), true);
    }

    // A higher-genus handlebody:
    verifySolidTorus(idealGenusTwoHandlebody, false);

    // A solid Klein bottle:
    verifySolidTorus(twistedBallBundle, false);

    // Some more non-trivial not complements, with real boundary:
    verifySolidTorus("fHLykbcdeedwuqs", false);
    verifySolidTorus("fLHPccdeeeqcieh", false);
    verifySolidTorus("oLALzLwMPOcbcbefijklnlmnnxxjnxmitnmfbae", false);
    verifySolidTorus("rLLLvKPjQvQMkacfkljmjlmlppopqqjkgtaxknokbmgwvij", false);
    verifySolidTorus("uLLvMPvwMwAMQkcacfgihjmklnnrqstrqrtnkvjhavkbveekgjxfcvp",
        false);

    // Some Seifert fibred spaces with torus boundary and Z homology:
    verifySolidTorus("eHLObcdddwun", false);
    verifySolidTorus("eHLObcdddwuj", false);

    // The connected sum of the Poincare homology sphere and the solid torus:
    verifySolidTorus("kLHKwvQQcceeijhjijakaawsnrsn", false);

    // Some larger solid tori from the census:
    verifySolidTorus("iHbfPPPbdfgfhhtbedbac", true);
    verifySolidTorus("iHerzQPadgffhhbbiqbao", true);
    verifySolidTorus("iLAvPQacbbgehfgdicdffnf", true);

    // A disjoint union of two solid tori:
    {
        SCOPED_TRACE("LST U LST");
        Triangulation<3> tri = lst3_4_7.tri;
        tri.insertLayeredSolidTorus(1, 2);
        verifySolidTorus(tri, false);
    }

    // An exhaustive census run:
    runCensusAllBounded(&verifySolidTorus4);
}

static void verifyHandlebody(const Triangulation<3>& tri, ssize_t genus) {
    // The caller must add their own scoped trace before calling this routine.
    // If tri is not a handlebody, pass -1 for genus.

    Triangulation<3> bounded(tri);
    if (bounded.isIdeal())
        bounded.idealToFinite();
    clearProperties(bounded);

    Triangulation<3> ideal(tri);
    if (ideal.hasBoundaryTriangles())
        ideal.finiteToIdeal();
    clearProperties(ideal);

    EXPECT_EQ(bounded.recogniseHandlebody(), genus);
    if (genus != 0)
        EXPECT_EQ(ideal.recogniseHandlebody(), genus);
    else
        EXPECT_TRUE(ideal.isSphere());
}

static void verifyHandlebody(const TriangulationTest<3>::TestCase& test,
        ssize_t genus) {
    SCOPED_TRACE_CSTRING(test.name);
    verifyHandlebody(test.tri, genus);
}

static void verifyHandlebody(const char* stringRep, ssize_t genus) {
    SCOPED_TRACE_CSTRING(stringRep);
    verifyHandlebody(Triangulation<3>(stringRep), genus);
}

TEST_F(Dim3Test, handlebodyRecognition) {
    // A selection of cases that are trivially not handlebodies:
    verifyHandlebody(empty, -1);
    verifyHandlebody(s3, -1);
    verifyHandlebody(rp3_1, -1);
    verifyHandlebody(idealRP2xI, -1);
    verifyHandlebody(pinchedSolidTorus, -1);
    verifyHandlebody(pinchedSolidKB, -1);
    verifyHandlebody(disjoint2, -1);
    verifyHandlebody(disjoint3, -1);

    // Some simple cases that are handlebodies:
    verifyHandlebody(ball, 0);
    verifyHandlebody(ball_bary, 0);
    verifyHandlebody(ball_large, 0);
    verifyHandlebody(ball_large_pillows, 0);
    verifyHandlebody(ball_large_snapped, 0);
    verifyHandlebody(ballBundle, 1);
    verifyHandlebody(lst3_4_7, 1);
    verifyHandlebody(idealGenusTwoHandlebody, 2);
    verifyHandlebody("cMcabbgds", 1); // Ideal solid torus

    // Some non-handlebodies with the right boundary and homology:
    verifyHandlebody(figure8, -1);
    verifyHandlebody(trefoil, -1);
    // Leave out knot18, which is too large and therefore too slow.

    // Some non-minimal layered solid tori:
    {
        SCOPED_TRACE_CSTRING("LST(1,20,21)");
        verifyHandlebody(Example<3>::lst(1, 20), 1);
    }
    {
        SCOPED_TRACE_CSTRING("LST(0,1,1)");
        verifyHandlebody(Example<3>::lst(0, 1), 1);
    }

    // A solid Klein bottle:
    verifyHandlebody(twistedBallBundle, -1);

    // Some higher-genus handlebodies and non-handlebodies:
    for (int genus = 0; genus < 4; ++genus) {
        SCOPED_TRACE_NUMERIC(genus);

        // Minimal layered triangulation.
        {
            SCOPED_TRACE("Layered handlebody");
            verifyHandlebody(Example<3>::handlebody(genus), genus);
        }

        // Connected sum with RP^3 (has correct basic properties, but
        // incorrect homology).
        {
            SCOPED_TRACE("Layered handlebody # RP^3");
            Triangulation<3> tri = Example<3>::handlebody(genus);
            tri.connectedSumWith(rp3_1.tri);
            verifyHandlebody(tri, -1);
        }

        // Connected sum with the Poincare homology sphere (even has
        // correct homology, so must use normal surfaces).
        {
            SCOPED_TRACE("Layered handlebody # Poincare");
            Triangulation<3> tri = Example<3>::handlebody(genus);
            tri.connectedSumWith(Example<3>::poincare());
            verifyHandlebody(tri, -1);
        }
    }
}

static void verifyTxI(const Triangulation<3>& tri, bool expected) {
    // The caller must add their own scoped trace before calling this routine.

    Triangulation<3> bounded(tri);
    if (bounded.isIdeal())
        bounded.idealToFinite();
    clearProperties(bounded);

    Triangulation<3> ideal(tri);
    if (ideal.hasBoundaryTriangles())
        ideal.finiteToIdeal();
    clearProperties(ideal);

    EXPECT_EQ(bounded.isTxI(), expected);
    EXPECT_EQ(ideal.isTxI(), expected);
}

static void verifyTxI(const TriangulationTest<3>::TestCase& test,
        bool expected) {
    SCOPED_TRACE_CSTRING(test.name);
    verifyTxI(test.tri, expected);
}

static void verifyTxI(const char* stringRep, bool expected) {
    SCOPED_TRACE_CSTRING(stringRep);
    verifyTxI(Triangulation<3>(stringRep), expected);
}

TEST_F(Dim3Test, TxIRecognition) {
    // An ideal TxI:
    verifyTxI("eLAkbbcddadbdb", true);

    // TODO: Test a TxI with real boundary.

    // A selection of cases that are clearly not TxI:
    verifyTxI(empty, false);
    verifyTxI(ball, false);
    verifyTxI(lst3_4_7, false);
    verifyTxI(idealRP2xI, false);

    // Some homology-TxI manifolds, based on links from 4^2_1 thru 7^2_8
    // in Bailey and Roth's tables from Rolfsen's "Knots and Links"
    // (5^2_1 and 7^2_8 have the same exterior):
    verifyTxI("eLPkbdcddabgbg", false);
    verifyTxI("eLPkbdcddhgggb", false);
    verifyTxI("eLMkbcdddaeeda", false);
    verifyTxI("eLMkbcddddedde", false);
    verifyTxI("gLLMQbcdefffmvftaog", false);
    verifyTxI("fLLQcbecdeepuwsua", false);
    verifyTxI("hLLAPkbcdefgggtsfxjjgb", false);
    verifyTxI("hLLMPkbcdfggfgmvfafwkf", false);
    verifyTxI("hLLzQkcdegffgguvuqpgvk", false);
    verifyTxI("iLLLAQccdegfhhghdcltautwa", false);
    verifyTxI("kLLLALQkceffehijjijiiealshealf", false);
    verifyTxI("eLPkbdcddabobv", false);

    // Finally, the connected sum of the Poincare homology sphere and TxI:
    verifyTxI("pLvwwLuPIIIkaddkomnjlllonobabtlqinfjwjnw", false);
}

static void verifyTV3(const TriangulationTest<3>::TestCase& test) {
    // Verify the Turaev-Viro invariants for r = 3.
    // The expected values are described in the paper of Turaev and Viro.
    //
    // For this test, the triangulation must be valid, closed and non-empty.

    SCOPED_TRACE_CSTRING(test.name);

    // Use a looser error gap; see the main turaevViro() test for the reasons.
    static constexpr const double epsilon = 0.00000001;

    // TODO: Exact cyclotomic polynomials

    double expect = 0.5;
    for (unsigned long i = 0; i < test.tri.homologyH2Z2(); ++i)
        expect += expect;

    // TODO: We are only checking q0 = 2,4.  What about 1,5?
    for (int q0 = 2; q0 <= 4; q0 += 2)
        EXPECT_NEAR(test.tri.turaevViroApprox(3, q0), expect, epsilon);
}

TEST_F(Dim3Test, turaevViro) {
    // Note: our floating-point comparisons uses a looser error gap than
    // EXPECT_DOUBLE_EQ(), since the algorithm has an exponential number of
    // steps and so the possible error could be substantial.

    static constexpr const double epsilon = 0.00000001;

    verifyTV3(sphere);
    verifyTV3(simpSphere);
    verifyTV3(sphereBundle);
    verifyTV3(twistedSphereBundle);

    verifyTV3(s3);
    verifyTV3(rp3_1);
    verifyTV3(rp3_2);
    verifyTV3(lens3_1);
    verifyTV3(lens8_3);
    verifyTV3(lens7_1_loop);
    verifyTV3(rp3rp3);
    verifyTV3(q32xz3);
    verifyTV3(q28);
    verifyTV3(weberSeifert);
    // Skip lens100_1 for now, since this is a very large triangulation.
    verifyTV3(s3_large);
    verifyTV3(rp3_large);
    verifyTV3(lens8_3_large);
    verifyTV3(q20_large);

    verifyTV3(rp2xs1);

    // Verify Turaev-Viro invariants for S^3.
    // The expected values are described in the paper of Turaev and Viro.
    for (int r = 4; r <= 8; ++r) {
        SCOPED_TRACE_NUMERIC(r);

        // TODO: Exact cyclotomic polynomials

        for (int q0 = 1; q0 < 2 * r; q0++) {
            if (std::gcd(q0, r) == 1) {
                SCOPED_TRACE_NUMERIC(q0);

                double pow = 2 * sin(M_PI * q0 / r);
                EXPECT_NEAR(s3.tri.turaevViroApprox(r, q0),
                    (pow * pow) / (2 * r), epsilon);
                EXPECT_NEAR(sphere.tri.turaevViroApprox(r, q0),
                    (pow * pow) / (2 * r), epsilon);
                EXPECT_NEAR(simpSphere.tri.turaevViroApprox(r, q0),
                    (pow * pow) / (2 * r), epsilon);
                EXPECT_NEAR(s3_large.tri.turaevViroApprox(r, q0),
                    (pow * pow) / (2 * r), epsilon);
            }
        }
    }

    // Verify Turaev-Viro invariants for RP^3.
    // The expected values are described in the paper of Turaev and Viro.
    for (int r = 4; r <= 8; ++r) {
        SCOPED_TRACE_NUMERIC(r);

        // TODO: Exact cyclotomic polynomials

        for (int q0 = 1; q0 < 2 * r; q0++) {
            if (std::gcd(q0, r) == 1) {
                SCOPED_TRACE_NUMERIC(q0);

                if (q0 % 2 == r % 2) {
                    EXPECT_NEAR(rp3_1.tri.turaevViroApprox(r, q0), 0, epsilon);
                    EXPECT_NEAR(rp3_2.tri.turaevViroApprox(r, q0), 0, epsilon);
                    EXPECT_NEAR(rp3_large.tri.turaevViroApprox(r, q0), 0,
                        epsilon);
                } else {
                    double real = cos(M_PI * q0 / r) - 1.0;
                    double imag = sin(M_PI * q0 / r);
                    EXPECT_NEAR(rp3_1.tri.turaevViroApprox(r, q0),
                        ((real * real) + (imag * imag)) / r, epsilon);
                    EXPECT_NEAR(rp3_2.tri.turaevViroApprox(r, q0),
                        ((real * real) + (imag * imag)) / r, epsilon);
                    EXPECT_NEAR(rp3_large.tri.turaevViroApprox(r, q0),
                        ((real * real) + (imag * imag)) / r, epsilon);
                }
            }
        }
    }

    // Verify Turaev-Viro invariants for L(3,1).
    // The expected values are described in the paper of Turaev and Viro.
    for (int r = 4; r <= 8; ++r) {
        SCOPED_TRACE_NUMERIC(r);

        // TODO: Exact cyclotomic polynomials

        for (int q0 = 1; q0 < 2 * r; q0++) {
            if (std::gcd(q0, r) == 1) {
                SCOPED_TRACE_NUMERIC(q0);

                double pow = 2 * sin(M_PI * q0 * (((r - 2) / 3) + 1) / r);
                EXPECT_NEAR(lens3_1.tri.turaevViroApprox(r, q0),
                    (pow * pow) / (2 * r), epsilon);
            }
        }
    }

    // Verify Turaev-Viro invariants for S^2 x S^1.
    // The expected values are described in the paper of Turaev and Viro.
    for (int r = 4; r <= 8; ++r) {
        SCOPED_TRACE_NUMERIC(r);

        EXPECT_EQ(sphereBundle.tri.turaevViro(r, true),
            regina::Cyclotomic(2 * r, 1));
        EXPECT_EQ(sphereBundle.tri.turaevViro(r, false),
            regina::Cyclotomic((r % 2 == 0 ? 2 * r : r), 1));

        for (int q0 = 1; q0 < 2 * r; q0++) {
            if (std::gcd(q0, r) == 1) {
                SCOPED_TRACE_NUMERIC(q0);

                EXPECT_NEAR(sphereBundle.tri.turaevViroApprox(r, q0), 1.0,
                    epsilon);
            }
        }
    }
}

static void verifyFillTorus(size_t p1, size_t q1, size_t r1,
        size_t p2, size_t q2, size_t r2, size_t lensP, size_t lensQ) {
    // Fills LST(p1, q1, r1) with the curve (p2, q2, r2) and verifies that
    // we obtain the lens space L(lensP, lensQ).
    SCOPED_TRACE_NUMERIC(p1);
    SCOPED_TRACE_NUMERIC(q1);
    SCOPED_TRACE_NUMERIC(r1);
    SCOPED_TRACE_NUMERIC(p2);
    SCOPED_TRACE_NUMERIC(q2);
    SCOPED_TRACE_NUMERIC(r2);

    // Ensure p1 ≤ q1 ≤ r1.
    if (p1 > q1) {
        std::swap(p1, q1);
        std::swap(p2, q2);
    }
    if (p1 > r1) {
        std::swap(p1, r1);
        std::swap(p2, r2);
    }
    if (q1 > r1) {
        std::swap(q1, r1);
        std::swap(q2, r2);
    }

    // Run the same test five times: once normally, and the
    // others with a random isomorphism.  The purpose of the
    // random isomorphisms is to mix up the order in which
    // the boundary edges are indexed within the boundary component.
    for (int i = 0; i < 5; ++i) {
        Triangulation<3> t;
        regina::Tetrahedron<3>* bdry = t.insertLayeredSolidTorus(p1, q1);
        regina::Edge<3> *e1, *e2, *e3;

        if (i == 0) {
            if (r1 <= 2) {
                e2 = bdry->edge(1, 2);
                e3 = bdry->edge(0, 2);
                e1 = bdry->edge(0, 1);
            } else {
                e1 = bdry->edge(1, 2);
                e2 = bdry->edge(0, 2);
                e3 = bdry->edge(0, 1);
            }
        } else {
            // Apply a random isomorphism.
            size_t idx = bdry->index();

            Isomorphism<3> iso = t.randomiseLabelling(false);
            Perm<4> p = iso.facetPerm(idx);
            if (r1 <= 2) {
                e2 = t.simplex(iso.simpImage(idx))->edge(p[1], p[2]);
                e3 = t.simplex(iso.simpImage(idx))->edge(p[0], p[2]);
                e1 = t.simplex(iso.simpImage(idx))->edge(p[0], p[1]);
            } else {
                e1 = t.simplex(iso.simpImage(idx))->edge(p[1], p[2]);
                e2 = t.simplex(iso.simpImage(idx))->edge(p[0], p[2]);
                e3 = t.simplex(iso.simpImage(idx))->edge(p[0], p[1]);
            }
        }

        ASSERT_TRUE(e1->isBoundary());
        ASSERT_TRUE(e2->isBoundary());
        ASSERT_TRUE(e3->isBoundary());

        t.fillTorus(e1, e2, e3, p2, q2, r2);
        t.intelligentSimplify();

        if (lensP <= 4) {
            // Optimistically hope that we simplified t down to ≤ 2 tetrahedra.
            // For ≤ 2 tetrahedra, we have the right lens space iff we have a
            // closed orientable 3-manifold with the right homology.
            EXPECT_LE(t.size(), 2);
            EXPECT_TRUE(t.isValid());
            EXPECT_TRUE(t.isClosed());
            EXPECT_TRUE(t.isOrientable());
            if (lensP == 0)
                EXPECT_EQ(t.homology(), AbelianGroup(1));
            else if (lensP == 1)
                EXPECT_EQ(t.homology(), AbelianGroup());
            else
                EXPECT_EQ(t.homology(), AbelianGroup(0, {lensP}));
        } else {
            // Optimistically hope that we simplified t down to the layered
            // (and conjecturally minimal) triangulation.
            EXPECT_TRUE(t.isIsomorphicTo(Example<3>::lens(lensP, lensQ)));
        }
    }
}

TEST_F(Dim3Test, fillTorus) {
    // Examples from Jaco-Rubinstein "Layered-triangulations of 3-manifolds":
    verifyFillTorus(0,1,1, 2,1,1, 2,1);
    verifyFillTorus(0,1,1, 1,2,1, 1,0); // S^3
    verifyFillTorus(1,1,2, 2,1,1, 3,1);
    verifyFillTorus(1,1,2, 1,1,2, 0,1); // S^2 x S^1
    verifyFillTorus(1,3,4, 2,1,1, 7,3);
    verifyFillTorus(1,3,4, 1,2,1, 5,1);
    verifyFillTorus(1,3,4, 1,1,2, 2,1);
    verifyFillTorus(2,3,5, 2,1,1, 8,3);
    verifyFillTorus(2,3,5, 1,2,1, 7,2);
    verifyFillTorus(2,3,5, 1,1,2, 1,0); // S^3
    verifyFillTorus(9,7,2, 5,3,8, 62,27);

    // We should obtain L(13,8) = L(13,5) from the following LST gluings:
    verifyFillTorus(1,0,1, 5,13,8, 13,5);
    verifyFillTorus(1,2,1, 5,3,8, 13,5);
    verifyFillTorus(1,2,3, 5,3,2, 13,5);

    // We should obtain L(25,11) = L(25,9) from the following LST gluings:
    verifyFillTorus(1,0,1, 14,25,11, 25,9);
    verifyFillTorus(1,2,1, 14,3,11, 25,9);
    verifyFillTorus(3,2,1, 8,3,11, 25,9);
    verifyFillTorus(3,2,5, 8,3,5, 25,9);
    verifyFillTorus(7,2,5, 2,3,5, 25,9);
    verifyFillTorus(7,2,9, 2,3,1, 25,9);
    verifyFillTorus(7,16,9, 2,1,1, 25,9);
    verifyFillTorus(25,16,9, 0,1,1, 25,9);

    // We should obtain L(42,11) = L(42,19) from the following LST gluings:
    verifyFillTorus(31,11,42, 1,1,0, 42,11);
    verifyFillTorus(31,11,20, 1,1,2, 42,11);
    verifyFillTorus(9,11,20, 3,1,2, 42,11);
    verifyFillTorus(9,11,2, 3,1,4, 42,11);
    verifyFillTorus(9,7,2, 3,7,4, 42,11);
    verifyFillTorus(5,7,2, 11,7,4, 42,11);
    verifyFillTorus(5,3,2, 11,15,4, 42,11);
    verifyFillTorus(1,3,2, 19,15,4, 42,11);
    verifyFillTorus(1,1,2, 19,23,4, 42,11);
    verifyFillTorus(1,1,0, 19,23,42, 42,11);
}

static void verifyMeridian(const Triangulation<3>& tri, const char* name) {
    // If tri is ideal then it will be truncated and simplified.
    //
    // If tri has real boundary then it will _not_ be simplified, and we
    // insist that it must have one vertex coming into this routine.

    SCOPED_TRACE_CSTRING(name);

    Triangulation<3> use(tri); // something we can modify
    if (use.isIdeal()) {
        use.idealToFinite();
        use.intelligentSimplify();
    }
    ASSERT_EQ(use.countVertices(), 1);
    ASSERT_EQ(use.countBoundaryComponents(), 1);

    for (int i = 0; i < 3; ++i) {
        // Try to engineer things so that boundary edge i lives in simplex 0,
        // and appears under all possible edge labellings.
        auto s = use.boundaryComponent(0)->edge(i)->front().simplex();

        for (int j = 0; j < 24; ++j) {
            auto iso = Isomorphism<3>::identity(use.size());
            if (s->index() != 0) {
                iso.simpImage(0) = s->index();
                iso.simpImage(s->index()) = 0;
            }
            iso.facetPerm(s->index()) = Perm<4>::S4[j];
            Triangulation<3> t = iso(use);

            // And now to actually test the meridian.  For this, we use the
            // fact that filling along the meridian should give the 3-sphere.

            regina::Edge<3>* m = t.meridian();
            ASSERT_TRUE(m->isBoundary());

            auto bc = t.boundaryComponent(0);
            ASSERT_EQ(bc->countEdges(), 3);

            auto e1 = (bc->edge(0) == m ? bc->edge(1) : bc->edge(0));
            auto e2 = (bc->edge(2) == m ? bc->edge(1) : bc->edge(2));
            t.fillTorus(m, e1, e2, 0, 1, 1);
            EXPECT_TRUE(t.isSphere());
        }
    }
}

TEST_F(Dim3Test, meridian) {
    // Unknot complements with many different boundary patterns:
    verifyMeridian(Example<3>::lst(0,1), "LST(0,1,1)");
    verifyMeridian(Example<3>::lst(1,1), "LST(1,1,2)");
    verifyMeridian(Example<3>::lst(1,2), "LST(1,2,3)");
    verifyMeridian(Example<3>::lst(1,3), "LST(1,3,4)");
    verifyMeridian(Example<3>::lst(1,4), "LST(1,4,5)");
    verifyMeridian(Example<3>::lst(2,3), "LST(2,3,5)");
    verifyMeridian(Example<3>::lst(2,5), "LST(2,5,7)");
    verifyMeridian(Example<3>::lst(3,4), "LST(3,4,7)");
    verifyMeridian(Example<3>::lst(3,5), "LST(3,5,8)");
    verifyMeridian(Example<3>::lst(4,5), "LST(4,5,9)");
    verifyMeridian(Example<3>::lst(4,7), "LST(4,7,11)");

    // Some non-trivial knots:
    verifyMeridian(figure8.tri, figure8.name);
    verifyMeridian(trefoil.tri, trefoil.name);
    // We skip knot18, which is a bit too slow (since we are computing
    // meridians 72 times for each test case under different isomorphisms).
    // verifyMeridian(knot18.tri, knot18.name);
}

static void verifyMeridianLongitude(const Triangulation<3>& tri,
        const char* name) {
    // If tri is ideal then it will be truncated and simplified.
    //
    // If tri has real boundary then it will _not_ be simplified, and we
    // insist that it must have one vertex coming into this routine.

    SCOPED_TRACE_CSTRING(name);

    Triangulation<3> use(tri); // something we can modify
    if (use.isIdeal()) {
        use.idealToFinite();
        use.intelligentSimplify();
    }
    ASSERT_EQ(use.countVertices(), 1);
    ASSERT_EQ(use.countBoundaryComponents(), 1);

    auto [m, l] = use.meridianLongitude();
    ASSERT_TRUE(m->isBoundary());
    ASSERT_TRUE(l->isBoundary());

    regina::Edge<3>* other = nullptr;
    for (auto e : use.boundaryComponent(0)->edges()) {
        if (e != m && e != l) {
            other = e;
            break;
        }
    }
    ASSERT_NE(other, nullptr);
    ASSERT_TRUE(other->isBoundary());

    // The longitude is the only curve that fills to give homology Z.
    // The meridian is the only curve that fills to give the 3-sphere.
    // More generally, filling the curve p*m + q*l should give homology Z_p.
    {
        Triangulation<3> tmp(use);
        tmp.fillTorus(tmp.translate(m), tmp.translate(l), tmp.translate(other),
            0, 1, 1);
        EXPECT_TRUE(tmp.isSphere());
    }
    {
        Triangulation<3> tmp(use);
        tmp.fillTorus(tmp.translate(m), tmp.translate(l), tmp.translate(other),
            1, 0, 1);
        EXPECT_EQ(tmp.homology(), AbelianGroup(1));
    }
    {
        Triangulation<3> tmp(use);
        tmp.fillTorus(tmp.translate(m), tmp.translate(l), tmp.translate(other),
            2, 3, 5);
        EXPECT_EQ(tmp.homology(), AbelianGroup(0, {3}));
    }
    {
        Triangulation<3> tmp(use);
        tmp.fillTorus(tmp.translate(m), tmp.translate(l), tmp.translate(other),
            2, 3, 1);
        EXPECT_EQ(tmp.homology(), AbelianGroup(0, {3}));
    }
}

TEST_F(Dim3Test, meridianLongitude) {
    // Unknot complements with many different boundary patterns:
    verifyMeridianLongitude(Example<3>::lst(0,1), "LST(0,1,1)");
    verifyMeridianLongitude(Example<3>::lst(1,1), "LST(1,1,2)");
    verifyMeridianLongitude(Example<3>::lst(1,2), "LST(1,2,3)");
    verifyMeridianLongitude(Example<3>::lst(1,3), "LST(1,3,4)");
    verifyMeridianLongitude(Example<3>::lst(1,4), "LST(1,4,5)");
    verifyMeridianLongitude(Example<3>::lst(2,3), "LST(2,3,5)");
    verifyMeridianLongitude(Example<3>::lst(2,5), "LST(2,5,7)");
    verifyMeridianLongitude(Example<3>::lst(3,4), "LST(3,4,7)");
    verifyMeridianLongitude(Example<3>::lst(3,5), "LST(3,5,8)");
    verifyMeridianLongitude(Example<3>::lst(4,5), "LST(4,5,9)");
    verifyMeridianLongitude(Example<3>::lst(4,7), "LST(4,7,11)");

    // Some non-trivial knots:
    verifyMeridianLongitude(figure8.tri, "Figure eight");
    verifyMeridianLongitude(trefoil.tri, "Trefoil");
    verifyMeridianLongitude(knot18.tri, "18-crossing knot");
}

TEST_F(Dim3Test, swapping) {
    Triangulation<3> a = Example<3>::figureEight();
    Triangulation<3> b = Example<3>::weberSeifert();

    a.homology();
    b.homology();

    swap(a, b);

    EXPECT_EQ(a.size(), 23); // swapping tetrahedra
    EXPECT_EQ(b.homology(), AbelianGroup(1)); // swapping cached properties

    std::iter_swap(&a, &b);

    EXPECT_EQ(a.size(), 2);
    EXPECT_EQ(b.homology(), AbelianGroup(0, {5, 5, 5}));
}

TEST_F(Dim3Test, propertyUpdates) {
    // Begin with an empty triangulation and calculate various properties.
    Triangulation<3> t;

    EXPECT_TRUE(t.isValid());
    EXPECT_TRUE(t.isOrientable());
    EXPECT_EQ(t.homology(), AbelianGroup());
    EXPECT_EQ(t.homologyBdry(), AbelianGroup());
    EXPECT_TRUE(t.isZeroEfficient());
    EXPECT_FALSE(t.hasSplittingSurface());
    EXPECT_FALSE(t.hasTwoSphereBoundaryComponents());

    // Add a single tetrahedron.
    t.newTetrahedron();

    EXPECT_FALSE(t.isZeroEfficient());
    EXPECT_TRUE(t.hasSplittingSurface());
    EXPECT_TRUE(t.hasTwoSphereBoundaryComponents());

    // Glue the tetrahedron to itself to form a solid torus.
    t.tetrahedron(0)->join(0, t.tetrahedron(0), {1,2,3,0});

    EXPECT_EQ(t.homology(), AbelianGroup(1));
    EXPECT_EQ(t.homologyBdry(), AbelianGroup(2));

    // Glue the remaining two faces in a non-orientable fashion.
    t.tetrahedron(0)->join(2, t.tetrahedron(0), {1,0,3,2});

    EXPECT_FALSE(t.isValid());
    EXPECT_FALSE(t.isOrientable());
}

TEST_F(Dim3Test, events) {
    /**
     * A struct that watches for changes on a triangulation,
     * and computes and stores isSolidTorus() immediately after a change.
     *
     * We use isSolidTorus() because this is a property managed by the
     * subclass Triangulation<3>, not the parent class TriangulationBase<3>.
     *
     * The main purpose of this test is to ensure that packetWasChanged is
     * (a) fired, (b) fired only once, and (c) fired at the corret time
     * (i.e., after the subclass data has been copied).
     */
    struct Watcher : public regina::PacketListener {
        int solidTorus { -1 };
        int events { 0 };

        void packetWasChanged(regina::Packet& p) {
            solidTorus = (
                static_cast<regina::PacketOf<Triangulation<3>>&>(p).
                    isSolidTorus() ?
                1 : 0);
            ++events;
        }
    };

    {
        auto p = regina::make_packet(Example<3>::lst(3,4));
        Watcher w;
        p->listen(std::addressof(w));

        EXPECT_TRUE(p->isSolidTorus());

        // Copy assignment that changes isSolidTorus()
        *p = rp3_1.tri;

        EXPECT_EQ(w.events, 1);
        EXPECT_EQ(w.solidTorus, 0);
        EXPECT_FALSE(p->isSolidTorus());
    }
    {
        auto p = regina::make_packet(Example<3>::lst(3,4));
        Watcher w;
        p->listen(std::addressof(w));

        EXPECT_TRUE(p->isSolidTorus());

        // Move assignment that changes isSolidTorus()
        // The extra insertTriangulation() is to ensure that the
        // move is not optimised away.
        Triangulation<3> t = rp3_1.tri;
        t.insertTriangulation(t);
        *p = std::move(t);

        EXPECT_EQ(w.events, 1);
        EXPECT_EQ(w.solidTorus, 0);
        EXPECT_FALSE(p->isSolidTorus());
    }
}
