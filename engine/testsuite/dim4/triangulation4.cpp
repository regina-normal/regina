
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

#include "algebra/intersectionform.h"
#include "manifold/manifold.h"
#include "subcomplex/standardtri.h"
#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"
#include "triangulation/example3.h"
#include "triangulation/example4.h"

#include "generic/triangulationtest.h"
#include "testexhaustive.h"

using regina::AbelianGroup;
using regina::Example;
using regina::Triangulation;

// Large examples of some specific manifolds, created from framed links.
// All are given as isosigs.  Thanks to Rhuaidi Burke for these.

static const char* large_s2xs2[] = {
    "GLvAvPPALvzzQPwAvQMMQQQQQQPkcffiigjjlorrnmmwssyyxBBzAAEECAzzCBBDDDEAEDCCxFFFFaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "GLvAvLPALvQwMAPQLwLQPQQQQQPkcfflgjjmpsrssrnonrtwvvzzyCCAECEEEBBBDBDDyAADCFFFFaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "GLvAvLzALPwLAQMQQwPLQMPQQQPkcfflgnmqrrqusvvopouwtxttqtAAzBBBCDDDDECCzCEEEFFFFaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" };

static const char* large_s2xs2Twisted[] = {
    "KLvAvLPALLMLMAzQLwLQPMPQQQQAQMQcfflgjjmprrrtsnonswvvAAzDDBFFFGCCGECEEzBBEDDHHHIIIIJJJJaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "KLvAvLPALvQwMAPQLwvAPQQQQQvQQQQcfflgjjmpsrssrnonrtwvvzzAAGCGGGBBBDBDDEEEDCIIIIHJJJJHHHaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "KLvAvLPwLLLPwQAQAAPMLQwAQQQAAQQcfflgjjqstvquxxvxovttsrzsAABBEEBBDFCHFECCCAAEGHIIIIJJJJaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "KLvAvLPwLLLPwQAQQAPMvQMQQQLPQMQcfflgjjqstvquxxvxovttsrwwszzAAEAACFFBEDBBBzzGIGHIHGJJJJaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" };

static const char* large_cp2[] = {
    "uLvAwPPAMMQLAPPQPkcfffgggjjkllllmnnpooqqrsrrsttttaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "yLvAvMPwAzLPQQQALQAQQcfffkgjjnnpotrtpnqnqqporowvvwwwxvvxxxaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "yLvAvAAAvQAQLQwMPQQMQcffjhhgiloopppqqqsoosrtvtwttuwuuwxxxxaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "GLvAvMPwAPLPMPQQzQMMPLAPMPQkcfffkgjjnnpposqsnnuutoropvwwvvxvyzyBBADDCEFEFFEEFaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "GLvAvLQALMMQPMwvQMQMPPMPwQQkcffilgjjloopnnnmqmnmsmxxxxyvuutzAzBBBCDCBFFEFEEEFaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "yLvAvMPAAPMMMQvPPQQMQcfffkgjjjmmoonnnnqpmpputwvwuututwxxxxaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "yLvAvMPAAPMMMQvPPQQMQcfffkgjjjmmoonnnnqpmpputwvwuututwxxxxaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" };

static const char* large_cp2_reflect[] = {
    "yLvAvLQALMMQPMwzQQQMQcffilgjjloopnnnmqmnmsmuwwwwttvuutxxxxaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "yLvAvPPAvMQQAwwAAQQQQcffgggjjjpoqoopoqqmrsuuwwvxxxuwuvvvxwaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "yLvAvAAAvQAQLAwMQAQQQcffjhhgiloopppqqqsooruwvuvuuxxxwvvwwxaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "yLvAvAQAvMAQAzMAQAPQQcffjhhgkiknoooqqqnprtruurvssrxxxxwwwwaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "yLvAvAQAvMAQAzMAQAPQQcffjhhgkiknoooqqqnprtruurvssrxxxxwwwwaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "yLvAvMPwAPLPMPQQzQMQQcfffkgjjnnpposqsnnuutoropvwwvvxvxxwwxaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" };

class Dim4Test : public TriangulationTest<4> {
    protected:
        // Closed orientable triangulations:
        TestCase s4_doubleCone {
            Example<4>::doubleCone(Example<3>::threeSphere()),
            "S^4 (double cone)" };
        TestCase cp2 { Example<4>::cp2(), "CP^2" };
        TestCase s2xs2 { Example<4>::s2xs2(), "S^2 x S^2" };
        TestCase s2xs2Twisted { Example<4>::s2xs2Twisted(), "S^2 x~ S^2" };
        TestCase k3 { Example<4>::k3(), "K3" };

        // Closed non-orientable triangulations:
        TestCase rp4 { Example<4>::rp4(), "RP^4" };

        // Bounded orientable triangulations:
        TestCase ball_folded { Triangulation<4>::fromGluings(1, {
                { 0, 2, 0, {2,4} }
            }), "B^4 (folded)" };
        TestCase ball_cone { Example<4>::singleCone(Example<3>::threeSphere()),
            "B^4 (cone)" };
        // Layer one pentachoron onto another over a single edge.  Then fold
        // together the two remaining facets of one of the pentachora.
        TestCase ball_layerAndFold { Triangulation<4>::fromGluings(2, {
                { 0, 0, 1, {} },
                { 0, 1, 1, {} },
                { 0, 2, 1, {} },
                { 0, 3, 0, {3,4} }
            }), "B^4 (layer and fold)" };

        // Ideal orientable triangulations:
        TestCase idealPoincareProduct {
            Example<4>::doubleCone(Example<3>::poincare()),
            "Ideal Poincare x I" };
        TestCase idealCappellShaneson { Example<4>::cappellShaneson(),
            "Cappell-Shaneson" };

        // Both ideal and real boundary:
        TestCase mixedPoincareProduct {
            Example<4>::singleCone(Example<3>::poincare()),
            "Mixed Poincare x I" };

        // Invalid triangulations:
        // - A double cone over the figure eight knot complement, where:
        //   * all three vertices are invalid (closed cusped 3-manifold links);
        //   * the edges joining each cone point to the centre vertex are
        //     invalid (torus links).
        TestCase idealFigEightProduct {
            Example<4>::doubleCone(Example<3>::figureEight()),
            "Fig_8 x I (double cone)" };
        // - A single cone over the figure eight knot complement, where:
        //   * both vertices are invalid (one has a closed cusped link,
        //     one has an invalid 3-manifold link);
        //   * the edge joining both vertices is invalid (torus link).
        TestCase mixedFigEightProduct {
            Example<4>::singleCone(Example<3>::figureEight()),
            "Fig_8 x I (single cone)" };
        // - Some "tetrahedral pillows" whose two facets are identified via a
        //   permutation in S_4, each containing a cycle that yields a bad
        //   self-identification of a lower-dimensional face.
        TestCase pillow_twoCycle { Triangulation<4>::fromGluings(2, {
                { 0, 0, 1, {} },
                { 0, 1, 1, {} },
                { 0, 2, 1, {} },
                { 0, 3, 1, {} },
                { 0, 4, 1, {1, 2} }
            }), "Invalid 2-cycle pillow" };
        TestCase pillow_threeCycle { Triangulation<4>::fromGluings(2, {
                { 0, 0, 1, {} },
                { 0, 1, 1, {} },
                { 0, 2, 1, {} },
                { 0, 3, 1, {} },
                { 0, 4, 1, {2, 0, 1, 3, 4} }
            }), "Invalid 3-cycle pillow" };
        TestCase pillow_fourCycle { Triangulation<4>::fromGluings(2, {
                { 0, 0, 1, {} },
                { 0, 1, 1, {} },
                { 0, 2, 1, {} },
                { 0, 3, 1, {} },
                { 0, 4, 1, {3, 2, 0, 1, 4} }
            }), "Invalid 4-cycle pillow" };

        // Disconnected triangulations (we build these in the constructor):
        TestCase disjoint2 { {}, "(S^3 x S^1) U B^4" };
        TestCase disjoint3 { {}, "RP^4 U B^4 U (ideal Poincare x I)" };

        Dim4Test() {
            disjoint2.tri.insertTriangulation(sphereBundle.tri);
            disjoint2.tri.insertTriangulation(ball_layerAndFold.tri);

            disjoint3.tri.insertTriangulation(rp4.tri);
            disjoint3.tri.insertTriangulation(ball_layerAndFold.tri);
            disjoint3.tri.insertTriangulation(idealPoincareProduct.tri);
        }

        /**
         * Run a given test over all of the example triangulations stored in
         * this test fixture.
         */
        void testManualCases(void (*f)(const Triangulation<4>&, const char*),
                bool includeLarge = true) {
            testGenericCases(f);

            f(s4_doubleCone.tri, s4_doubleCone.name);
            f(cp2.tri, cp2.name);
            f(s2xs2.tri, s2xs2.name);
            f(s2xs2Twisted.tri, s2xs2Twisted.name);
            if (includeLarge)
                f(k3.tri, k3.name);

            f(rp4.tri, rp4.name);

            f(ball_folded.tri, ball_folded.name);
            f(ball_cone.tri, ball_cone.name);
            f(ball_layerAndFold.tri, ball_layerAndFold.name);

            if (includeLarge)
                f(idealPoincareProduct.tri, idealPoincareProduct.name);
            f(idealCappellShaneson.tri, idealCappellShaneson.name);

            if (includeLarge)
                f(mixedPoincareProduct.tri, mixedPoincareProduct.name);

            f(idealFigEightProduct.tri, idealFigEightProduct.name);
            f(mixedFigEightProduct.tri, mixedFigEightProduct.name);
            f(pillow_twoCycle.tri, pillow_twoCycle.name);
            f(pillow_threeCycle.tri, pillow_threeCycle.name);
            f(pillow_fourCycle.tri, pillow_fourCycle.name);

            f(disjoint2.tri, disjoint2.name);
            if (includeLarge)
                f(disjoint3.tri, disjoint3.name);
        }
};

TEST_F(Dim4Test, magic) {
    // Verify the "magic" string-based constructor.
    testManualCases([](const Triangulation<4>& t, const char* name) {
        SCOPED_TRACE_CSTRING(name);

        std::string sig = t.isoSig();
        Triangulation<4> recon(sig);
        EXPECT_EQ(recon.isoSig(), sig);
    });

    EXPECT_THROW({
        Triangulation<4> t("INVALID");
    }, regina::InvalidArgument);
}

static void verifyInvalid(const TriangulationTest<4>::TestCase& test,
        size_t invalidVertices, std::array<size_t, 3> invalidEdges,
        size_t invalidTriangles) {
    // Invalid edges: (total, bad link, bad self-identification)
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
        size_t found = 0, foundLink = 0, foundID = 0;
        for (auto f : test.tri.edges()) {
            EXPECT_EQ(! f->isValid(),
                f->hasBadLink() || f->hasBadIdentification());
            if (! f->isValid())
                ++found;
            if (f->hasBadLink())
                ++foundLink;
            if (f->hasBadIdentification())
                ++foundID;
        }
        EXPECT_EQ(found, invalidEdges[0]);
        EXPECT_EQ(foundLink, invalidEdges[1]);
        EXPECT_EQ(foundID, invalidEdges[2]);
    }
    {
        size_t found = 0;
        for (auto f : test.tri.triangles()) {
            EXPECT_FALSE(f->hasBadLink());
            EXPECT_EQ(! f->isValid(), f->hasBadIdentification());
            if (f->hasBadIdentification())
                ++found;
        }
        EXPECT_EQ(found, invalidTriangles);
    }
}

TEST_F(Dim4Test, validity) {
    TriangulationTest<4>::validityGenericCases();

    verifyValid(s4_doubleCone);
    verifyValid(cp2);
    verifyValid(s2xs2);
    verifyValid(s2xs2Twisted);
    verifyValid(k3);

    verifyValid(rp4);

    verifyValid(ball_folded);
    verifyValid(ball_cone);
    verifyValid(ball_layerAndFold);

    verifyValid(idealPoincareProduct);
    verifyValid(idealCappellShaneson);

    verifyValid(mixedPoincareProduct);

    verifyInvalid(idealFigEightProduct, 3, {2, 2, 0}, 0);
    verifyInvalid(mixedFigEightProduct, 2, {1, 1, 0}, 0);
    verifyInvalid(pillow_twoCycle, 2, {2, 1, 1}, 2);
    verifyInvalid(pillow_threeCycle, 0, {0, 0, 0}, 1);
    verifyInvalid(pillow_fourCycle, 0, {1, 1, 1}, 0);

    verifyValid(disjoint2);
    verifyValid(disjoint3);
}

TEST_F(Dim4Test, connectivity) {
    TriangulationTest<4>::connectivityGenericCases();

    EXPECT_TRUE(s4_doubleCone.tri.isConnected());
    EXPECT_TRUE(cp2.tri.isConnected());
    EXPECT_TRUE(s2xs2.tri.isConnected());
    EXPECT_TRUE(s2xs2Twisted.tri.isConnected());
    EXPECT_TRUE(k3.tri.isConnected());

    EXPECT_TRUE(rp4.tri.isConnected());

    EXPECT_TRUE(ball_folded.tri.isConnected());
    EXPECT_TRUE(ball_cone.tri.isConnected());
    EXPECT_TRUE(ball_layerAndFold.tri.isConnected());

    EXPECT_TRUE(idealPoincareProduct.tri.isConnected());
    EXPECT_TRUE(idealCappellShaneson.tri.isConnected());

    EXPECT_TRUE(mixedPoincareProduct.tri.isConnected());

    EXPECT_TRUE(idealFigEightProduct.tri.isConnected());
    EXPECT_TRUE(mixedFigEightProduct.tri.isConnected());
    EXPECT_TRUE(pillow_twoCycle.tri.isConnected());
    EXPECT_TRUE(pillow_threeCycle.tri.isConnected());
    EXPECT_TRUE(pillow_fourCycle.tri.isConnected());

    EXPECT_FALSE(disjoint2.tri.isConnected());
    EXPECT_FALSE(disjoint3.tri.isConnected());
}

TEST_F(Dim4Test, orientability) {
    TriangulationTest<4>::orientabilityGenericCases();

    EXPECT_TRUE(s4_doubleCone.tri.isOrientable());
    EXPECT_TRUE(cp2.tri.isOrientable());
    EXPECT_TRUE(s2xs2.tri.isOrientable());
    EXPECT_TRUE(s2xs2Twisted.tri.isOrientable());
    EXPECT_TRUE(k3.tri.isOrientable());

    EXPECT_FALSE(rp4.tri.isOrientable());

    EXPECT_TRUE(ball_folded.tri.isOrientable());
    EXPECT_TRUE(ball_cone.tri.isOrientable());
    EXPECT_TRUE(ball_layerAndFold.tri.isOrientable());

    EXPECT_TRUE(idealPoincareProduct.tri.isOrientable());
    EXPECT_TRUE(idealCappellShaneson.tri.isOrientable());

    EXPECT_TRUE(mixedPoincareProduct.tri.isOrientable());

    EXPECT_TRUE(idealFigEightProduct.tri.isOrientable());
    EXPECT_TRUE(mixedFigEightProduct.tri.isOrientable());
    EXPECT_FALSE(pillow_twoCycle.tri.isOrientable());
    EXPECT_TRUE(pillow_threeCycle.tri.isOrientable());
    EXPECT_FALSE(pillow_fourCycle.tri.isOrientable());

    EXPECT_TRUE(disjoint2.tri.isOrientable());
    EXPECT_FALSE(disjoint3.tri.isOrientable());
}

TEST_F(Dim4Test, eulerChar) {
    TriangulationTest<4>::eulerCharGenericCases();

    EXPECT_EQ(s4_doubleCone.tri.eulerCharTri(), 2);
    EXPECT_EQ(s4_doubleCone.tri.eulerCharManifold(), 2);
    EXPECT_EQ(cp2.tri.eulerCharTri(), 3);
    EXPECT_EQ(cp2.tri.eulerCharManifold(), 3);
    EXPECT_EQ(s2xs2.tri.eulerCharTri(), 4);
    EXPECT_EQ(s2xs2.tri.eulerCharManifold(), 4);
    EXPECT_EQ(s2xs2Twisted.tri.eulerCharTri(), 4);
    EXPECT_EQ(s2xs2Twisted.tri.eulerCharManifold(), 4);
    EXPECT_EQ(k3.tri.eulerCharTri(), 24);
    EXPECT_EQ(k3.tri.eulerCharManifold(), 24);

    EXPECT_EQ(rp4.tri.eulerCharTri(), 1);
    EXPECT_EQ(rp4.tri.eulerCharManifold(), 1);

    EXPECT_EQ(ball_folded.tri.eulerCharTri(), 1);
    EXPECT_EQ(ball_folded.tri.eulerCharManifold(), 1);
    EXPECT_EQ(ball_cone.tri.eulerCharTri(), 1);
    EXPECT_EQ(ball_cone.tri.eulerCharManifold(), 1);
    EXPECT_EQ(ball_layerAndFold.tri.eulerCharTri(), 1);
    EXPECT_EQ(ball_layerAndFold.tri.eulerCharManifold(), 1);

    EXPECT_EQ(idealPoincareProduct.tri.eulerCharTri(), 2);
    EXPECT_EQ(idealPoincareProduct.tri.eulerCharManifold(), 0);
    EXPECT_EQ(idealCappellShaneson.tri.eulerCharTri(), 1);
    EXPECT_EQ(idealCappellShaneson.tri.eulerCharManifold(), 0);

    EXPECT_EQ(mixedPoincareProduct.tri.eulerCharTri(), 1);
    EXPECT_EQ(mixedPoincareProduct.tri.eulerCharManifold(), 0);

    EXPECT_EQ(idealFigEightProduct.tri.eulerCharTri(), 1);
    EXPECT_EQ(mixedFigEightProduct.tri.eulerCharTri(), 1);
    EXPECT_EQ(pillow_twoCycle.tri.eulerCharTri(), 2);
    EXPECT_EQ(pillow_threeCycle.tri.eulerCharTri(), 2);
    EXPECT_EQ(pillow_fourCycle.tri.eulerCharTri(), 0);

    EXPECT_EQ(disjoint2.tri.eulerCharTri(), 1);
    EXPECT_EQ(disjoint2.tri.eulerCharManifold(), 1);
    EXPECT_EQ(disjoint3.tri.eulerCharTri(), 4);
    EXPECT_EQ(disjoint3.tri.eulerCharManifold(), 2);
}

TEST_F(Dim4Test, boundaryBasic) {
    TriangulationTest<4>::boundaryBasicGenericCases();

    verifyBoundaryBasic(s4_doubleCone, {}, {}, {});
    verifyBoundaryBasic(cp2, {}, {}, {});
    verifyBoundaryBasic(s2xs2, {}, {}, {});
    verifyBoundaryBasic(s2xs2Twisted, {}, {}, {});
    verifyBoundaryBasic(k3, {}, {}, {});

    verifyBoundaryBasic(rp4, {}, {}, {});

    verifyBoundaryBasic(ball_folded, {0}, {}, {});
    verifyBoundaryBasic(ball_cone, {0}, {}, {});
    verifyBoundaryBasic(ball_layerAndFold, {0}, {}, {});

    verifyBoundaryBasic(idealPoincareProduct, {}, {0, 0}, {});
    verifyBoundaryBasic(idealCappellShaneson, {}, {0}, {});

    verifyBoundaryBasic(mixedPoincareProduct, {0}, {0}, {});

    verifyBoundaryBasic(idealFigEightProduct, {}, {}, {2, 1, 1});
    verifyBoundaryBasic(mixedFigEightProduct, {1}, {}, {1});
    verifyBoundaryBasic(pillow_twoCycle, {}, {}, {0, 0});
    verifyBoundaryBasic(pillow_threeCycle, {}, {0}, {});
    verifyBoundaryBasic(pillow_fourCycle, {}, {}, {});

    verifyBoundaryBasic(disjoint2, {0}, {}, {});
    verifyBoundaryBasic(disjoint3, {0}, {0, 0}, {});
}

static void verifyBoundaryManifolds(const TriangulationTest<4>::TestCase& test,
        std::initializer_list<const char*> expect) {
    SCOPED_TRACE_CSTRING(test.name);
    ASSERT_EQ(test.tri.countBoundaryComponents(), expect.size());

    auto it = expect.begin();
    for (auto bc : test.tri.boundaryComponents()) {
        Triangulation<3> t = bc->build();
        t.simplify();

        if (auto std = regina::StandardTriangulation::recognise(t)) {
            if (auto mfd = std->manifold())
                EXPECT_EQ(mfd->name(), *it);
            else
                EXPECT_EQ(t.isoSig(), *it);
        } else
            EXPECT_EQ(t.isoSig(), *it);
        ++it;
    }
}

TEST_F(Dim4Test, boundaryManifolds) {
    verifyBoundaryManifolds(ball, { "S3" });
    verifyBoundaryManifolds(ball_folded, { "S3" });
    verifyBoundaryManifolds(ball_cone, { "S3" });
    verifyBoundaryManifolds(ball_layerAndFold, { "S3" });
    verifyBoundaryManifolds(ballBundle, { "S2 x S1" });
    verifyBoundaryManifolds(twistedBallBundle, { "S2 x~ S1" });

    verifyBoundaryManifolds(idealPoincareProduct, { "S3/P120", "S3/P120" });
    verifyBoundaryManifolds(idealCappellShaneson, { "S2 x S1" });

    verifyBoundaryManifolds(mixedPoincareProduct, { "S3/P120", "S3/P120" });

    verifyBoundaryManifolds(idealFigEightProduct,
        { "dLQacccbjkg" /* ideal TxI */,
          "Figure eight knot complement", "Figure eight knot complement" });
    verifyBoundaryManifolds(mixedFigEightProduct,
        { "Figure eight knot complement", "Figure eight knot complement" });
    // For pillow_twoCycle, both boundary components are RP2 x I with one ideal
    // vertex and one invald edge (whose midpoint becomes the second RP2 cusp).
    verifyBoundaryManifolds(pillow_twoCycle, { "cPcbbbaac", "cPcbbbaac" });
    verifyBoundaryManifolds(pillow_threeCycle, { "L(3,1)" });

    verifyBoundaryManifolds(disjoint2, { "S3" });
    verifyBoundaryManifolds(disjoint3, { "S3", "S3/P120", "S3/P120" });
}

TEST_F(Dim4Test, boundaryPinching) {
    testManualCases(verifyBoundaryPinching);
}

TEST_F(Dim4Test, vertexLinksBasic) {
    TriangulationTest<4>::vertexLinksBasicGenericCases();

    // Note: for ideal and invalid vertices, we are already testing the
    // vertex link manifolds via boundaryManifolds().

    verifyVertexLinksBasic(s4_doubleCone, 3, 0);
    verifyVertexLinksBasic(cp2, 4, 0);
    verifyVertexLinksBasic(s2xs2, 5, 0);
    verifyVertexLinksBasic(s2xs2Twisted, 5, 0);
    verifyVertexLinksBasic(k3, 24, 0);

    verifyVertexLinksBasic(rp4, 3, 0);

    verifyVertexLinksBasic(ball_folded, 0, 4);
    verifyVertexLinksBasic(ball_cone, 1, 1);
    verifyVertexLinksBasic(ball_layerAndFold, 0, 4);

    verifyVertexLinksBasic(idealPoincareProduct, 1, 0, 2);
    verifyVertexLinksBasic(idealCappellShaneson, 0, 0, 1);

    verifyVertexLinksBasic(mixedPoincareProduct, 0, 1, 1);

    verifyVertexLinksBasic(idealFigEightProduct, 0, 0, 0, 3);
    verifyVertexLinksBasic(mixedFigEightProduct, 0, 0, 0, 2);
    verifyVertexLinksBasic(pillow_twoCycle, 2, 0, 0, 2);
    verifyVertexLinksBasic(pillow_threeCycle, 2, 0, 1, 0);
    verifyVertexLinksBasic(pillow_fourCycle, 2, 0, 0, 0);

    verifyVertexLinksBasic(disjoint2, 1, 4);
    verifyVertexLinksBasic(disjoint3, 4, 4, 2);
}

static void verifyVertexLinks(const Triangulation<4>& tri, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    for (auto v : tri.vertices()) {
        const Triangulation<3>& link = v->buildLink();
        Isomorphism<4> iso = v->buildLinkInclusion();

        EXPECT_EQ(link.size(), v->degree());
        EXPECT_TRUE(link.isConnected());

        if (v->isValid()) {
            if (v->isBoundary()) {
                if (v->boundaryComponent()->size() > 0) {
                    EXPECT_TRUE(link.isBall());
                } else {
                    EXPECT_TRUE(link.isClosed());
                    EXPECT_TRUE(! link.isSphere());
                }
            } else
                EXPECT_TRUE(link.isSphere());
        } else {
            // Invalid vertex.
            if (! v->isBoundary()) {
                ADD_FAILURE() << "Invalid vertex not marked as boundary";
            } else if (v->boundaryComponent()->countTetrahedra() > 0) {
                // Link should have boundary faces but not be a 3-ball.
                EXPECT_TRUE(link.hasBoundaryTriangles());
                EXPECT_FALSE(link.isBall());
            } else {
                // Link should have no boundary faces, but not
                // be a closed 3-manifold.
                EXPECT_FALSE(link.hasBoundaryTriangles());
                EXPECT_FALSE(link.isClosed());
            }
        }

        // Make sure the triangulated link is labelled correctly.
        for (size_t j = 0; j < v->degree(); ++j) {
            auto pent = tri.pentachoron(iso.pentImage(j));

            regina::Perm<5> perm = iso.facetPerm(j);
            int vNum = perm[4];
            EXPECT_EQ(pent->vertex(vNum), v);
            EXPECT_EQ(perm[0], pent->tetrahedronMapping(vNum)[0]);
            EXPECT_EQ(perm[1], pent->tetrahedronMapping(vNum)[1]);
            EXPECT_EQ(perm[2], pent->tetrahedronMapping(vNum)[2]);
            EXPECT_EQ(perm[3], pent->tetrahedronMapping(vNum)[3]);

            for (int k = 0; k < 4; ++k) {
                auto tet = link.tetrahedron(j);
                auto adj = tet->adjacentTetrahedron(k);
                if (adj) {
                    EXPECT_EQ(pent->adjacentPentachoron(perm[k]),
                        tri.pentachoron(iso.pentImage(adj->index())));
                    EXPECT_EQ(pent->adjacentGluing(perm[k]),
                        iso.facetPerm(adj->index()) *
                            regina::Perm<5>::extend(tet->adjacentGluing(k)) *
                            perm.inverse());
                } else {
                    EXPECT_EQ(pent->adjacentPentachoron(perm[k]), nullptr);
                }
            }
        }
    }
}

TEST_F(Dim4Test, vertexLinks) {
    testManualCases(verifyVertexLinks);
    runCensusAllBounded(verifyVertexLinks);
    runCensusAllNoBdry(verifyVertexLinks);
}

static void verifyEdgeLinks(const Triangulation<4>& tri, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    for (auto e : tri.edges()) {
        const Triangulation<2>& link = e->buildLink();
        Isomorphism<4> iso = e->buildLinkInclusion();

        EXPECT_EQ(link.size(), e->degree());
        EXPECT_TRUE(link.isConnected());
        EXPECT_EQ(link.isClosed(), ! e->isBoundary());
        if (e->isValid())
            EXPECT_EQ(link.eulerChar(), (e->isBoundary() ? 1 : 2));

        // Make sure the triangulated edge link is labelled correctly.
        for (size_t j = 0; j < e->degree(); ++j) {
            auto pent = tri.pentachoron(iso.pentImage(j));

            regina::Perm<5> perm = iso.facetPerm(j);
            int eNum = regina::Edge<4>::edgeNumber[perm[3]][perm[4]];
            EXPECT_EQ(pent->edge(eNum), e);
            EXPECT_EQ(perm[0], pent->triangleMapping(eNum)[0]);
            EXPECT_EQ(perm[1], pent->triangleMapping(eNum)[1]);
            EXPECT_EQ(perm[2], pent->triangleMapping(eNum)[2]);
            EXPECT_EQ(perm[3], pent->edgeMapping(eNum)[0]);
            EXPECT_EQ(perm[4], pent->edgeMapping(eNum)[1]);

            for (int k = 0; k < 3; ++k) {
                auto t = link.triangle(j);
                auto adj = t->adjacentTriangle(k);
                if (adj) {
                    EXPECT_EQ(pent->adjacentPentachoron(perm[k]),
                        tri.pentachoron(iso.pentImage(adj->index())));
                    // Note: we expect broken gluings with reverse
                    // self-identifications.
                    if (! e->hasBadIdentification())
                        EXPECT_EQ(pent->adjacentGluing(perm[k]),
                            iso.facetPerm(adj->index()) *
                                Perm<5>::extend(t->adjacentGluing(k)) *
                                perm.inverse());
                } else {
                    EXPECT_EQ(pent->adjacentPentachoron(perm[k]), nullptr);
                }
            }
        }

        // Make sure the edge link matches what happens on the vertex links.
        auto pent = e->front().pentachoron();
        regina::Perm<5> perm = e->front().vertices();
        for (int j = 0; j < 2; ++j) {

            // In the vertex link at the jth end of this edge,
            // find the vertex that this edge projects down to.
            Vertex<4>* v = pent->vertex(perm[j]);
            const Triangulation<3>& vLink = v->buildLink();

            size_t k;
            for (k = 0; k < v->degree(); ++k)
                if (v->embedding(k).pentachoron() == pent &&
                        v->embedding(k).vertex() == perm[j])
                    break;
            EXPECT_LT(k, v->degree());

            Vertex<3>* match = vLink.tetrahedron(k)->vertex(
                pent->tetrahedronMapping(perm[j]).pre(perm[1-j]));

            if (! e->hasBadIdentification()) {
                EXPECT_TRUE(match->buildLink().isIsomorphicTo(link));
            } else {
                // It's hard to guarantee much in this setting, sigh.
                EXPECT_EQ(match->degree(), 2 * e->degree());
            }
        }

    }
}

TEST_F(Dim4Test, edgeLinks) {
    // Notes:
    // - idealFigEightProduct -> has torus link
    // - mixedFigEightProduct -> has torus link
    // - pillow_fourCycle -> has PP link
    testManualCases(verifyEdgeLinks);
    runCensusAllBounded(verifyEdgeLinks);
    runCensusAllNoBdry(verifyEdgeLinks);
}

TEST_F(Dim4Test, orient) {
    testManualCases(TriangulationTest<4>::verifyOrient);
}

TEST_F(Dim4Test, skeleton) {
    testManualCases(TriangulationTest<4>::verifySkeleton);
}

TEST_F(Dim4Test, boundaryLabelling) {
    testManualCases(TriangulationTest<4>::verifyBoundaryLabelling);
}

TEST_F(Dim4Test, edgeAccess) {
    TriangulationTest<4>::edgeAccess();
}

TEST_F(Dim4Test, reordering) {
    testManualCases(TriangulationTest<4>::verifyReordering);
}

TEST_F(Dim4Test, doubleCover) {
    testManualCases(TriangulationTest<4>::verifyDoubleCover);
}

TEST_F(Dim4Test, makeCanonical) {
    testManualCases(TriangulationTest<4>::verifyMakeCanonical);
}

TEST_F(Dim4Test, isomorphismSignature) {
    testManualCases(TriangulationTest<4>::verifyIsomorphismSignature);
}

TEST_F(Dim4Test, pachner) {
    testManualCases(TriangulationTest<4>::verifyPachner);
    runCensusAllBounded(TriangulationTest<4>::verifyPachner);
    runCensusAllNoBdry(TriangulationTest<4>::verifyPachner);
    TriangulationTest<4>::verifyPachnerSimplicial();
}

TEST_F(Dim4Test, move20Vertex) {
    // Note: we need at least 3 pentachora for 2-0 moves to become legal.
    testManualCases(TriangulationTest<4>::verify20Vertex);
    runCensusAllBounded(TriangulationTest<4>::verify20Vertex);
    runCensusAllNoBdry(TriangulationTest<4>::verify20Vertex);
}

TEST_F(Dim4Test, move20Edge) {
    // Note: we need at least 3 pentachora for 2-0 moves to become legal.
    testManualCases(TriangulationTest<4>::verify20Edge);
    runCensusAllBounded(TriangulationTest<4>::verify20Edge);
    runCensusAllNoBdry(TriangulationTest<4>::verify20Edge);
}

TEST_F(Dim4Test, move20Triangle) {
    // Note: we need at least 3 pentachora for 2-0 moves to become legal.
    testManualCases(TriangulationTest<4>::verify20Triangle);
    runCensusAllBounded(TriangulationTest<4>::verify20Triangle);
    runCensusAllNoBdry(TriangulationTest<4>::verify20Triangle);
}

TEST_F(Dim4Test, shellBoundary) {
    testManualCases(TriangulationTest<4>::verifyShellBoundary);
    runCensusAllBounded(TriangulationTest<4>::verifyShellBoundary);
    runCensusAllNoBdry(TriangulationTest<4>::verifyShellBoundary);

    // Some additional cases where the subface identification criterion
    // matters but none of the other criteria fail.  These are included here
    // because such a scenario is not picked up by the small census used by
    // runCensusAllBounded().
    std::string sigs[] = {
        "dHHIbcccWakaka2a", "dHHIbccc4asasa2a", "dHHIbcccWa0a0a2a",
        "dHHIbccc4aebeb2a", "dHHcaacccalb1a", "dHHcaacccaZanb"
    };
    for (const auto& sig: sigs) {
        TriangulationTest<4>::verifyShellBoundary(
            Triangulation<4>::fromIsoSig(sig), sig.c_str());
    }
}

TEST_F(Dim4Test, barycentricSubdivision) {
    testManualCases(TriangulationTest<4>::verifyBarycentricSubdivision,
        false /* small triangulations only */);

    // The following triangulation remains invalid under barycentric
    // subdivision, since it has an edge with an RP^2 link and subdivision
    // cannot save that.
    {
        Triangulation<4> tri(pillow_twoCycle.tri);
        EXPECT_FALSE(tri.isValid());
        tri.subdivide(); // allow subdivide() to update validity, if it can
        EXPECT_FALSE(tri.isValid());
        clearProperties(tri); // force validity to be explicitly recomputed
        EXPECT_FALSE(tri.isValid());
    }
    // The following triangulations should change from invalid to valid
    // under barycentric subdivision, since their problem is faces with bad
    // self-identifications (which subdivision fixes).
    {
        Triangulation<4> tri(pillow_threeCycle.tri);
        EXPECT_FALSE(tri.isValid());
        tri.subdivide(); // allow subdivide() to update validity, if it can
        EXPECT_TRUE(tri.isValid());
        clearProperties(tri); // force validity to be explicitly recomputed
        EXPECT_TRUE(tri.isValid());
    }
    {
        Triangulation<4> tri(pillow_fourCycle.tri);
        EXPECT_FALSE(tri.isValid());
        tri.subdivide(); // allow subdivide() to update validity, if it can
        EXPECT_TRUE(tri.isValid());
        clearProperties(tri); // force validity to be explicitly recomputed
        EXPECT_TRUE(tri.isValid());
    }
}

TEST_F(Dim4Test, tightEncoding) {
    testManualCases(TriangulationTest<4>::verifyTightEncoding);
    runCensusAllBounded(TriangulationTest<4>::verifyTightEncoding);
    runCensusAllNoBdry(TriangulationTest<4>::verifyTightEncoding);
}

TEST_F(Dim4Test, homologyH1) {
    TriangulationTest<4>::homologyH1GenericCases();

    EXPECT_EQ(s4_doubleCone.tri.homology<1>(), AbelianGroup());
    EXPECT_EQ(cp2.tri.homology<1>(), AbelianGroup());
    EXPECT_EQ(s2xs2.tri.homology<1>(), AbelianGroup());
    EXPECT_EQ(s2xs2Twisted.tri.homology<1>(), AbelianGroup());
    EXPECT_EQ(k3.tri.homology<1>(), AbelianGroup());

    EXPECT_EQ(rp4.tri.homology<1>(), AbelianGroup(0, {2}));

    EXPECT_EQ(ball_folded.tri.homology<1>(), AbelianGroup());
    EXPECT_EQ(ball_cone.tri.homology<1>(), AbelianGroup());
    EXPECT_EQ(ball_layerAndFold.tri.homology<1>(), AbelianGroup());

    EXPECT_EQ(idealPoincareProduct.tri.homology<1>(), AbelianGroup());
    EXPECT_EQ(idealCappellShaneson.tri.homology<1>(), AbelianGroup(1));

    EXPECT_EQ(mixedPoincareProduct.tri.homology<1>(), AbelianGroup());

    EXPECT_EQ(idealFigEightProduct.tri.homology<1>(), AbelianGroup(1));
    EXPECT_EQ(mixedFigEightProduct.tri.homology<1>(), AbelianGroup(1));
    EXPECT_EQ(pillow_twoCycle.tri.homology<1>(), AbelianGroup());
    EXPECT_EQ(pillow_threeCycle.tri.homology<1>(), AbelianGroup());
    EXPECT_EQ(pillow_fourCycle.tri.homology<1>(), AbelianGroup(0, {4}));

    EXPECT_EQ(disjoint2.tri.homology<1>(), AbelianGroup(1));
    EXPECT_EQ(disjoint3.tri.homology<1>(), AbelianGroup(0, {2}));
}

TEST_F(Dim4Test, homologyH2) {
    TriangulationTest<4>::homologyH2GenericCases();

    EXPECT_EQ(s4_doubleCone.tri.homology<2>(), AbelianGroup());
    EXPECT_EQ(cp2.tri.homology<2>(), AbelianGroup(1));
    EXPECT_EQ(s2xs2.tri.homology<2>(), AbelianGroup(2));
    EXPECT_EQ(s2xs2Twisted.tri.homology<2>(), AbelianGroup(2));
    EXPECT_EQ(k3.tri.homology<2>(), AbelianGroup(22));

    EXPECT_EQ(rp4.tri.homology<2>(), AbelianGroup());

    EXPECT_EQ(ball_folded.tri.homology<2>(), AbelianGroup());
    EXPECT_EQ(ball_cone.tri.homology<2>(), AbelianGroup());
    EXPECT_EQ(ball_layerAndFold.tri.homology<2>(), AbelianGroup());

    EXPECT_EQ(idealPoincareProduct.tri.homology<2>(), AbelianGroup());
    EXPECT_EQ(idealCappellShaneson.tri.homology<2>(), AbelianGroup());

    EXPECT_EQ(mixedPoincareProduct.tri.homology<2>(), AbelianGroup());

    // Skip the invalid cases, since homology<k>() requires a valid
    // triangulation for k ≥ 2.

    EXPECT_EQ(disjoint2.tri.homology<2>(), AbelianGroup());
    EXPECT_EQ(disjoint3.tri.homology<2>(), AbelianGroup());
}

TEST_F(Dim4Test, homologyH3) {
    TriangulationTest<4>::homologyH3GenericCases();

    EXPECT_EQ(s4_doubleCone.tri.homology<3>(), AbelianGroup());
    EXPECT_EQ(cp2.tri.homology<3>(), AbelianGroup());
    EXPECT_EQ(s2xs2.tri.homology<3>(), AbelianGroup());
    EXPECT_EQ(s2xs2Twisted.tri.homology<3>(), AbelianGroup());
    EXPECT_EQ(k3.tri.homology<3>(), AbelianGroup());

    EXPECT_EQ(rp4.tri.homology<3>(), AbelianGroup(0, {2}));

    EXPECT_EQ(ball_folded.tri.homology<3>(), AbelianGroup());
    EXPECT_EQ(ball_cone.tri.homology<3>(), AbelianGroup());
    EXPECT_EQ(ball_layerAndFold.tri.homology<3>(), AbelianGroup());

    EXPECT_EQ(idealPoincareProduct.tri.homology<3>(), AbelianGroup(1));
    EXPECT_EQ(idealCappellShaneson.tri.homology<3>(), AbelianGroup());

    EXPECT_EQ(mixedPoincareProduct.tri.homology<3>(), AbelianGroup(1));

    // Skip the invalid cases, since homology<k>() requires a valid
    // triangulation for k ≥ 2.

    EXPECT_EQ(disjoint2.tri.homology<3>(), AbelianGroup(1));
    EXPECT_EQ(disjoint3.tri.homology<3>(), AbelianGroup(1, {2}));
}

TEST_F(Dim4Test, boundaryHomology) {
    TriangulationTest<4>::boundaryHomologyGenericCases();

    verifyBoundaryH1(ball_folded, 0, {});
    verifyBoundaryH1(ball_cone, 0, {});
    verifyBoundaryH1(ball_layerAndFold, 0, {});

    verifyBoundaryH1(idealPoincareProduct, 0, {});
    verifyBoundaryH1(idealPoincareProduct, 1, {});
    verifyBoundaryH1(idealCappellShaneson, 0, {1});

    verifyBoundaryH1(mixedPoincareProduct, 0, {});
    verifyBoundaryH1(mixedPoincareProduct, 1, {});

    verifyBoundaryH1(idealFigEightProduct, 0, {2}); // suspension of torus
    verifyBoundaryH1(idealFigEightProduct, 1, {1}); // figure 8 knot complement
    verifyBoundaryH1(idealFigEightProduct, 2, {1}); // figure 8 knot complement
    verifyBoundaryH1(mixedFigEightProduct, 0, {1}); // figure 8 knot complement
    verifyBoundaryH1(mixedFigEightProduct, 1, {1}); // figure 8 knot complement
    verifyBoundaryH1(pillow_twoCycle, 0, {0, {2}});
    verifyBoundaryH1(pillow_twoCycle, 1, {0, {2}});
    verifyBoundaryH1(pillow_threeCycle, 0, {0, {3}}); // L(3,1)

    verifyBoundaryH1(disjoint2, 0, {});
    verifyBoundaryH1(disjoint3, 0, {});
    verifyBoundaryH1(disjoint3, 1, {});
    verifyBoundaryH1(disjoint3, 2, {});
}

TEST_F(Dim4Test, fundGroup) {
    TriangulationTest<4>::fundGroupGenericCases();

    EXPECT_EQ(s4_doubleCone.tri.group().recogniseGroup(), "0");
    EXPECT_EQ(cp2.tri.group().recogniseGroup(), "0");
    EXPECT_EQ(s2xs2.tri.group().recogniseGroup(), "0");
    EXPECT_EQ(s2xs2Twisted.tri.group().recogniseGroup(), "0");
    EXPECT_EQ(k3.tri.group().recogniseGroup(), "0");

    EXPECT_EQ(rp4.tri.group().recogniseGroup(), "Z_2");

    EXPECT_EQ(ball_folded.tri.group().recogniseGroup(), "0");
    EXPECT_EQ(ball_cone.tri.group().recogniseGroup(), "0");
    EXPECT_EQ(ball_layerAndFold.tri.group().recogniseGroup(), "0");

    EXPECT_EQ(idealPoincareProduct.tri.group().recogniseGroup(), "");
    // Computed using Regina 7.4:
    EXPECT_EQ(idealCappellShaneson.tri.group().recogniseGroup(),
        "Z~3 Z w/monodromy a \u21A6 b, b \u21A6 c, c \u21A6 c^-1 a");

    EXPECT_EQ(mixedPoincareProduct.tri.group().recogniseGroup(), "");

    EXPECT_EQ(idealFigEightProduct.tri.group().recogniseGroup(),
        "Z~Free(2) w/monodromy a \u21A6 b, b \u21A6 b^2 a^-1 b");
    EXPECT_EQ(mixedFigEightProduct.tri.group().recogniseGroup(),
        "Z~Free(2) w/monodromy a \u21A6 b, b \u21A6 b^2 a^-1 b");
    EXPECT_EQ(pillow_twoCycle.tri.group().recogniseGroup(), "0");
    EXPECT_EQ(pillow_threeCycle.tri.group().recogniseGroup(), "0");
    EXPECT_EQ(pillow_fourCycle.tri.group().recogniseGroup(), "Z_4");

    // We cannot call group() on disjoint triangulations.
}

TEST_F(Dim4Test, chainComplex) {
    testManualCases(TriangulationTest<4>::verifyChainComplex);
}

TEST_F(Dim4Test, dualChainComplex) {
    testManualCases(TriangulationTest<4>::verifyDualChainComplex);
}

TEST_F(Dim4Test, dualToPrimal) {
    testManualCases(TriangulationTest<4>::verifyDualToPrimal);
}

TEST_F(Dim4Test, copyMove) {
    testManualCases(TriangulationTest<4>::verifyCopyMove);
}

static void verifyFourFourMove(const Triangulation<4>& tri, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    Triangulation<4> oriented(tri);
    if (oriented.isOrientable())
        oriented.orient();

    for (auto e : tri.edges()) {
        Triangulation<4> alt(oriented);

        if (! alt.fourFourMove(alt.edge(e->index()))) {
            // Check that the move was _not_ performed.
            EXPECT_EQ(alt, oriented);
            continue;
        }

        // The move was performed.

        // Ensure that properties we are about to verify are explicitly
        // recomputed.
        clearProperties(alt);

        EXPECT_EQ(alt.size(), tri.size());
        EXPECT_EQ(alt.countVertices(), tri.countVertices());
        EXPECT_EQ(alt.isValid(), tri.isValid());
        EXPECT_EQ(alt.isOrientable(), tri.isOrientable());
        if (tri.isOrientable())
            EXPECT_TRUE(alt.isOriented());
        EXPECT_EQ(alt.isClosed(), tri.isClosed());
        EXPECT_EQ(alt.countBoundaryComponents(), tri.countBoundaryComponents());
        EXPECT_EQ(alt.eulerCharTri(), tri.eulerCharTri());
        EXPECT_EQ(alt.eulerCharManifold(), tri.eulerCharManifold());

        if (tri.isValid()) {
            EXPECT_EQ(alt.homology<1>(), tri.homology<1>());
            EXPECT_EQ(alt.homology<2>(), tri.homology<2>());
        }

        // Randomly relabel the pentachora, but preserve orientation.
        alt.randomiseLabelling(true);

        // Ensure that there exists an inverse 4-4 move.
        bool found = false;
        for (auto e : alt.edges())
            if (alt.fourFourMove(e, true, false)) {
                Triangulation<4> inv(alt);
                EXPECT_TRUE(inv.fourFourMove(inv.edge(e->index()),
                    false, true));

                // Don't clear properties from inv, since what we're about to
                // test does not rely on computed topological properties.
                if (tri.isOrientable())
                    EXPECT_TRUE(inv.isOriented());
                if (inv.isIsomorphicTo(tri)) {
                    found = true;
                    break;
                }
            }
        EXPECT_TRUE(found);
    }
}

TEST_F(Dim4Test, fourFourMove) {
    testManualCases(verifyFourFourMove);
    runCensusAllBounded(verifyFourFourMove);
    runCensusAllNoBdry(verifyFourFourMove);
}

static void verifySnapEdge(const Triangulation<4>& tri, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    Triangulation<4> oriented(tri);
    if (oriented.isOrientable())
        oriented.orient();

    for (auto e : tri.edges()) {
        Triangulation<4> alt(oriented);

        bool res = alt.snapEdge(alt.edge(e->index()));
        EXPECT_EQ(res, (e->vertex(0) != e->vertex(1) &&
            ! ( e->vertex(0)->isBoundary() && e->vertex(1)->isBoundary())));
        if (! res) {
            // Check that the move is _not_ performed.
            EXPECT_EQ(alt, oriented);
            continue;
        }

        // The move was performed.

        // Ensure that properties we are about to verify are explicitly
        // recomputed.
        clearProperties(alt);

        EXPECT_EQ(alt.size(), tri.size() + 4);
        EXPECT_EQ(alt.countVertices(), tri.countVertices() - 1);
        EXPECT_EQ(alt.isValid(), tri.isValid());
        EXPECT_EQ(alt.isOrientable(), tri.isOrientable());
        if (tri.isOrientable())
            EXPECT_TRUE(alt.isOriented());
        EXPECT_EQ(alt.isClosed(), tri.isClosed());
        EXPECT_EQ(alt.countBoundaryComponents(), tri.countBoundaryComponents());
        EXPECT_EQ(alt.eulerCharTri(), tri.eulerCharTri());
        EXPECT_EQ(alt.eulerCharManifold(), tri.eulerCharManifold());

        if (tri.size() <= HOMOLOGY_THRESHOLD && tri.isValid()) {
            EXPECT_EQ(alt.homology<1>(), tri.homology<1>());
            EXPECT_EQ(alt.homology<2>(), tri.homology<2>());
        }
    }
}

TEST_F(Dim4Test, snapEdge) {
    testManualCases(verifySnapEdge);
    runCensusAllBounded(verifySnapEdge);
    runCensusAllNoBdry(verifySnapEdge);
}

static void verifyIdealToFinite(const Triangulation<4>& tri, const char* name) {
    SCOPED_TRACE_CSTRING(name);

    bool shouldTruncate = false;
    if (tri.isValid() && ! tri.isIdeal()) {
        // Should not truncate any vertices.
    } else {
        for (auto v : tri.vertices())
            if (v->isIdeal() || ! v->isValid()) {
                shouldTruncate = true;
                break;
            }
    }

    if (! shouldTruncate) {
        // The idealToFinite routine should leave tri unchanged.
        Triangulation<4> other(tri);
        other.idealToFinite();
        EXPECT_EQ(other, tri);
        return;
    }

    // Test the same triangulation under some random isomorphisms,
    // since the idealToFinite() code implements cases separately for
    // truncating differently-labelled vertices.
    //
    // We use just a couple of different isomorphisms here, since this
    // is a slow test and we are running it over a great many examples.
    for (unsigned i = 0; i < 2; ++i) {
        Triangulation<4> other(tri);
        if (i > 0)
            other.randomiseLabelling(false);

        other.idealToFinite();

        // Ensure that properties we are about to verify are explicitly
        // recomputed.
        clearProperties(other);

        EXPECT_NE(other, tri);
        EXPECT_TRUE(other.hasBoundaryTetrahedra());
        EXPECT_FALSE(other.isIdeal());
        EXPECT_EQ(other.isOrientable(), tri.isOrientable());
        EXPECT_EQ(other.isClosed(), tri.isClosed());
        EXPECT_EQ(other.isConnected(), tri.isConnected());

        if (tri.isValid()) {
            // Subdivision can change invalid to valid, but never the other way.
            EXPECT_TRUE(other.isValid());

            // Subdivisions can change these properties for
            // invalid triangulations.
            EXPECT_EQ(other.eulerCharManifold(), tri.eulerCharManifold());
            EXPECT_EQ(other.countBoundaryComponents(),
                    tri.countBoundaryComponents());

            // Homology can only be computed for valid triangulations.
            other.simplify();

            EXPECT_EQ(other.homology<1>(), tri.homology<1>());
            EXPECT_EQ(other.homology<2>(), tri.homology<2>());
        }
    }
}

TEST_F(Dim4Test, idealToFinite) {
    testManualCases(verifyIdealToFinite);
    runCensusAllBounded(verifyIdealToFinite); // Never change
    runCensusAllNoBdry(verifyIdealToFinite); // Sometimes change
}

static void verifyIntersectionForm(const TriangulationTest<4>::TestCase& test,
        size_t rank, long signature, bool even) {
    SCOPED_TRACE_CSTRING(test.name);

    regina::IntersectionForm f = test.tri.intersectionForm();

    EXPECT_EQ(f.matrix().det().abs(), 1);
    EXPECT_EQ(f.rank(), rank);
    EXPECT_EQ(f.signature(), signature);
    EXPECT_EQ(f.even(), even);

    if (signature != 0) {
        Triangulation<4> alt = test.tri;
        alt.reflect();
        regina::IntersectionForm altForm = alt.intersectionForm();

        EXPECT_EQ(altForm.matrix().det().abs(), 1);
        EXPECT_EQ(altForm.rank(), rank);
        EXPECT_EQ(altForm.signature(), -signature);
        EXPECT_EQ(altForm.even(), even);
    }

    // Flip some top-dimensional simplices, but not the first
    // since we want to preserve the orientation.
    auto iso = Isomorphism<4>::identity(test.tri.size());
    for (size_t i = 1; i < test.tri.size(); i += 2)
        iso.facetPerm(i) = Perm<5>(1,3);
    regina::IntersectionForm flip = iso(test.tri).intersectionForm();
    EXPECT_EQ(flip.rank(), f.rank());
    EXPECT_EQ(flip.signature(), f.signature());
    EXPECT_EQ(flip.even(), f.even());
}

static void verifyIntersectionFormSimple(const Triangulation<4>& tri,
        const char* name) {
    if (! tri.isOrientable())
        return;

    SCOPED_TRACE_CSTRING(name);

    regina::IntersectionForm f = tri.intersectionForm();

    EXPECT_EQ(f.matrix().det().abs(), 1);

    // Tests for simply connected manifolds:
    if (tri.group().countGenerators() == 0) {
        // Verify Rohlin's theorem:
        if (f.even())
            EXPECT_EQ(f.signature() % 16, 0);
    }
}

TEST_F(Dim4Test, intersectionForm) {
    runCensusAllClosed(verifyIntersectionFormSimple);

    // Tests with specific solutions for some closed orientable manifolds:

    // Simply connected:
    verifyIntersectionForm(sphere, 0, 0, true);
    verifyIntersectionForm(simpSphere, 0, 0, true);
    verifyIntersectionForm(s4_doubleCone, 0, 0, true);
    verifyIntersectionForm(cp2, 1, 1, false);
    verifyIntersectionForm(s2xs2, 2, 0, true);
    verifyIntersectionForm(s2xs2Twisted, 2, 0, false);
    verifyIntersectionForm(k3, 22, -16, true);

    // Not simply connected:
    verifyIntersectionForm(sphereBundle, 0, 0, true);

    // Some larger examples:
    for (const char* sig : large_s2xs2)
        verifyIntersectionForm({ Triangulation<4>::fromIsoSig(sig), sig },
            2, 0, true);
    for (const char* sig : large_s2xs2Twisted)
        verifyIntersectionForm({ Triangulation<4>::fromIsoSig(sig), sig },
            2, 0, false);
    for (const char* sig : large_cp2)
        verifyIntersectionForm({ Triangulation<4>::fromIsoSig(sig), sig },
            1, 1, false);
    for (const char* sig : large_cp2_reflect)
        verifyIntersectionForm({ Triangulation<4>::fromIsoSig(sig), sig },
            1, -1, false);
}

static void verifyIBundle(const Triangulation<3>& tri, const char* name) {
    // For now, only work with compact triangulations.
    if ((! tri.isValid()) || tri.isIdeal())
        return;

    SCOPED_TRACE_CSTRING(name);

    Triangulation<4> b = Example<4>::iBundle(tri);

    EXPECT_TRUE(b.isValid());
    EXPECT_EQ(b.isOrientable(), tri.isOrientable());
    EXPECT_EQ(b.countComponents(), tri.countComponents());
    {
        size_t expectBdryComp = 2 * tri.countComponents();
        for (auto c : tri.components())
            if (c->countBoundaryFacets())
                --expectBdryComp;
        EXPECT_EQ(b.countBoundaryComponents(), expectBdryComp);
    }
    EXPECT_EQ(b.eulerCharTri(), tri.eulerCharTri());
    EXPECT_EQ(b.eulerCharManifold(), tri.eulerCharManifold());
    EXPECT_EQ(b.countBoundaryFacets(),
        2 * tri.countTetrahedra() + 20 * tri.countBoundaryFacets());

    // TODO: Check isomorphisms from tri onto the boundary of b.

    // Simplify the triangulation before running any more expensive tests.
    b.simplify();

    EXPECT_EQ(b.homology<1>(), tri.homology<1>());
    EXPECT_EQ(b.homology<2>(), tri.homology<2>());
}

TEST_F(Dim4Test, iBundle) {
    runCensusAllClosed(verifyIBundle);
    runCensusAllBounded(verifyIBundle);
}

static void verifyS1Bundle(const Triangulation<3>& tri, const char* name) {
    // For now, only work with compact triangulations.
    if ((! tri.isValid()) || tri.isIdeal())
        return;

    SCOPED_TRACE_CSTRING(name);

    Triangulation<4> b = Example<4>::s1Bundle(tri);

    EXPECT_TRUE(b.isValid());
    EXPECT_EQ(b.isOrientable(), tri.isOrientable());
    EXPECT_EQ(b.countComponents(), tri.countComponents());
    EXPECT_EQ(b.countBoundaryComponents(), tri.countBoundaryComponents());
    EXPECT_EQ(b.eulerCharTri(), 0);
    EXPECT_EQ(b.eulerCharManifold(), 0);
    EXPECT_EQ(b.countBoundaryFacets(), 20 * tri.countBoundaryTriangles());

    // Simplify the triangulation before running any more expensive tests.
    b.simplify();

    {
        regina::AbelianGroup expectH1 = tri.homology<1>();
        expectH1.addRank();
        EXPECT_EQ(b.homology<1>(), expectH1);
    }
    {
        regina::AbelianGroup expectH2(tri.homology<2>());
        expectH2.addGroup(tri.homology<1>());
        EXPECT_EQ(b.homology<2>(), expectH2);
    }
}

TEST_F(Dim4Test, s1Bundle) {
    runCensusAllClosed(verifyS1Bundle);
    runCensusAllBounded(verifyS1Bundle);
}

static void verifyBundleWithMonodromy(const Triangulation<3>& tri,
        const char* name) {
    // For now, only work with compact triangulations.
    if ((! tri.isValid()) || tri.isIdeal())
        return;

    SCOPED_TRACE_CSTRING(name);

    tri.findAllIsomorphisms(tri, [&tri, name](
            const Isomorphism<3>& aut) {
        Triangulation<4> b = Example<4>::bundleWithMonodromy(tri, aut);

        EXPECT_TRUE(b.isValid());
        // We can lose orientability, but we cannot gain it.
        if (! tri.isOrientable())
            EXPECT_FALSE(b.isOrientable());
        EXPECT_EQ(b.countComponents(), tri.countComponents());
        EXPECT_EQ(b.countBoundaryComponents(), tri.countBoundaryComponents());
        EXPECT_EQ(b.eulerCharTri(), 0);
        EXPECT_EQ(b.eulerCharManifold(), 0);
        EXPECT_EQ(b.countBoundaryFacets(), 20 * tri.countBoundaryTriangles());

        // TODO: It would be nice to test something about homology here.
        // Note that, since we are passing a combinatorial isomorphism,
        // b must have (M x S1) as a finite sheeted cover.

        return false;
    });
}

TEST_F(Dim4Test, bundleWithMonodromy) {
    runCensusAllClosed(verifyBundleWithMonodromy, false /* small */);
    runCensusAllBounded(verifyBundleWithMonodromy, false /* small */);

    // A case for which using inverse isomorphism gluings will definitely break
    // things, since the gluings are not involutions.  This is the two-vertex,
    // two-tetrahedron L(3,1); the degree two vertex is 0(0) == 1(1).
    verifyBundleWithMonodromy(Triangulation<3>::fromGluings(2, {
        { 0, 0, 1, {1,3,0,2} },
        { 0, 1, 1, {1,2,3,0} },
        { 0, 2, 1, {1,2,3,0} },
        { 0, 3, 1, {1,2,3,0} }
    }), "Hand-coded L(3,1)");
}

static void verifyRetriangulate(const Triangulation<4>& tri,
        int height, int threads, bool track, size_t count) {
    SCOPED_TRACE_NUMERIC(height);
    SCOPED_TRACE_NUMERIC(threads);

    size_t tot = 0;

    std::unique_ptr<regina::ProgressTrackerOpen> tracker;
    if (track)
        tracker.reset(new regina::ProgressTrackerOpen());

    bool result = tri.retriangulate(height, threads, tracker.get(),
            [&tot, &tri](const Triangulation<4>& alt) {
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

static void verifyRetriangulate(const TriangulationTest<4>::TestCase& test,
        int height, size_t count) {
    SCOPED_TRACE_CSTRING(test.name);

    // Single-threaded, no tracker:
    verifyRetriangulate(test.tri, height, 1, false, count);
    // Multi-threaded, with and without tracker:
    verifyRetriangulate(test.tri, height, 2, false, count);
    verifyRetriangulate(test.tri, height, 2, true, count);
}

TEST_F(Dim4Test, retriangulate) {
    // The counts here were computed using Regina 6.0 in single-threaded mode.
    //
    // The counts that are commented out are too slow, though they can be
    // brought back in again as the retriangulation code gets faster.

    verifyRetriangulate(empty, 0, 1);
    verifyRetriangulate(empty, 1, 1);
    verifyRetriangulate(empty, 2, 1);
    verifyRetriangulate(s4_doubleCone, 0, 1);
    verifyRetriangulate(s4_doubleCone, 1, 1);
    verifyRetriangulate(s4_doubleCone, 2, 15);
    // verifyRetriangulate(s4_doubleCone, 4, 12316);
    verifyRetriangulate(rp4, 0, 1);
    verifyRetriangulate(rp4, 1, 1);
    verifyRetriangulate(rp4, 2, 53);
    // verifyRetriangulate(rp4, 4, 3854489);
    verifyRetriangulate(ball_layerAndFold, 0, 1);
    verifyRetriangulate(ball_layerAndFold, 1, 1);
    verifyRetriangulate(ball_layerAndFold, 2, 4);
    verifyRetriangulate(ball_layerAndFold, 4, 863);
    verifyRetriangulate(idealCappellShaneson, 0, 1);
    verifyRetriangulate(idealCappellShaneson, 1, 1);
    verifyRetriangulate(idealCappellShaneson, 2, 9);
    verifyRetriangulate(idealCappellShaneson, 4, 1610);
}

TEST_F(Dim4Test, events) {
    /**
     * A struct that watches for changes on a triangulation,
     * and computes and stores rank(H2) immediately after a change.
     *
     * We use H2 because this is a property managed by the subclass
     * Triangulation<4>, not the parent class TriangulationBase<4>.
     *
     * The main purpose of this test is to ensure that packetWasChanged is
     * (a) fired, (b) fired only once, and (c) fired at the corret time
     * (i.e., after the subclass data has been copied).
     */
    struct Watcher : public regina::PacketListener {
        ssize_t rank { -1 };
        int events { 0 };

        void packetWasChanged(regina::Packet& p) {
            rank = static_cast<regina::PacketOf<Triangulation<4>>&>
                (p).homology<2>().rank();
            ++events;
        }
    };

    {
        auto p = regina::make_packet(
            Example<4>::doubleCone(Example<3>::s2xs1()));
        Watcher w;
        p->listen(std::addressof(w));

        EXPECT_EQ(p->homology<2>().rank(), 1);

        // Copy assignment that changes H2
        *p = rp4.tri;

        EXPECT_EQ(w.events, 1);
        EXPECT_EQ(w.rank, 0);
        EXPECT_EQ(p->homology<2>().rank(), 0);
    }
    {
        auto p = regina::make_packet(
            Example<4>::doubleCone(Example<3>::s2xs1()));
        Watcher w;
        p->listen(std::addressof(w));

        EXPECT_EQ(p->homology<2>().rank(), 1);

        // Move assignment that changes H2
        // The extra insertTriangulation() is to ensure that the move is not
        // optimised away.
        Triangulation<4> t = rp4.tri;
        t.insertTriangulation(t);
        *p = std::move(t);

        EXPECT_EQ(w.events, 1);
        EXPECT_EQ(w.rank, 0);
        EXPECT_EQ(p->homology<2>().rank(), 0);
    }
}
