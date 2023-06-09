
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

#include "link/examplelink.h"
#include "link/link.h"
#include "maths/matrix.h"
#include "snappea/examplesnappea.h"
#include "snappea/snappeatriangulation.h"
#include "surface/normalsurfaces.h"
#include "triangulation/example3.h"
#include "triangulation/dim3.h"

#include "testexhaustive.h"
#include "testhelper.h"

using regina::Example;
using regina::ExampleLink;
using regina::ExampleSnapPea;
using regina::Perm;
using regina::SnapPeaTriangulation;
using regina::Tetrahedron;
using regina::Triangulation;

class SnapPeaTest : public testing::Test {
    protected:
        // Orientable manifolds from the Callahan-Hildebrand-Weeks census:
        Triangulation<3> m2_1 { Triangulation<3>::rehydrate("cabbbbaei") };
        Triangulation<3> m2_2 { Triangulation<3>::rehydrate("cabbbbapt") };
        Triangulation<3> m3_9 { Triangulation<3>::rehydrate("dagacccfwkn") };
        Triangulation<3> m4_52 { Triangulation<3>::rehydrate("ebdbcdddaqhie") };
        Triangulation<3> m4_1_2 {
            Triangulation<3>::rehydrate("eahbcdddhsssj") };
        Triangulation<3> m4_4_2 {
            Triangulation<3>::rehydrate("ebdbcdddddddx") };

        // Non-orientable manifolds from the Callahan-Hildebrand-Weeks census:
        Triangulation<3> n1_1 { Triangulation<3>::rehydrate("baaaade") };
        Triangulation<3> n2_1 { Triangulation<3>::rehydrate("cabbbbabw") };
        Triangulation<3> n2_1_2 { Triangulation<3>::rehydrate("cabbbbcdw") };
        Triangulation<3> n4_14 { Triangulation<3>::rehydrate("eahdccddakfhq") };
        Triangulation<3> n4_9_2 {
            Triangulation<3>::rehydrate("ebdbcdddcemre") };
        Triangulation<3> n4_1_2_1 {
            Triangulation<3>::rehydrate("eahbcdddjxxxj") };

        // Closed manifolds from the Hodgson-Weeks census:
        Triangulation<3> closedHypOr {
            Example<3>::smallClosedOrblHyperbolic() };
        // Note: the non-orientable manifold below is the same as
        // Example<3>::smallClosedNonOrblHyperbolic()), but if we build it
        // from Example<3> then we seem to get a degenerate solution.  Using
        // the isosig gives the same triangulation with a different labelling,
        // which seems to prod SnapPea into finding a better solution instead.
        Triangulation<3> closedHypNor {
            Triangulation<3>::fromIsoSig("lLLLALAQccegffiijkikkkknawmhvwcls") };

        // The Weber-Seifert dodecahedral space:
        Triangulation<3> weberSeifert { Example<3>::weberSeifert() };

        // Triangulations with flat and degenerate solutions (found through an
        // exhaustive census of small ideal triangulations - they do not
        // appear in SnapPea's hyperbolic census):
        Triangulation<3> flatOr = Triangulation<3>::fromGluings(2, {
            { 0, 0, 1, {0,1,2,3} },
            { 0, 1, 1, {0,1,2,3} },
            { 0, 2, 1, {1,3,2,0} },
            { 0, 3, 1, {1,2,0,3} }});
        Triangulation<3> flatNor = Triangulation<3>::fromGluings(2, {
            { 0, 0, 1, {0,1,2,3} },
            { 0, 1, 1, {2,1,0,3} },
            { 0, 2, 1, {1,3,2,0} },
            { 0, 3, 1, {2,1,0,3} }});
        Triangulation<3> degenerateOr = Triangulation<3>::fromGluings(2, {
            { 0, 0, 0, {1,0,2,3} },
            { 0, 2, 1, {1,2,0,3} },
            { 0, 3, 1, {0,2,3,1} },
            { 1, 2, 1, {1,2,3,0} }});
        Triangulation<3> degenerateNor = Triangulation<3>::fromGluings(2, {
            { 0, 0, 0, {1,0,2,3} },
            { 0, 2, 1, {1,2,0,3} },
            { 0, 3, 1, {0,3,2,1} },
            { 1, 2, 1, {0,2,3,1} }});

        SnapPeaTest() {
            // Keep the kernel quiet.  It interferes with the test
            // suite's running progress messages.
            SnapPeaTriangulation::disableKernelMessages();
        }

        static void testVolume(const SnapPeaTriangulation& s,
                double vol, int places, const char* name) {
            // Verify the volume to the given number of decimal places.
            // Places are counted after the decimal point in standard
            // (non-scientific) notation.
            SCOPED_TRACE_CSTRING(name);
            SCOPED_TRACE_NUMERIC(vol);
            SCOPED_TRACE_NUMERIC(places);

            ASSERT_FALSE(s.isNull());

            auto [foundVol, precision] = s.volumeWithPrecision();
            EXPECT_GE(precision, places);

            double epsilon = 0.5;
            for (int i = 0; i < places; ++i)
                epsilon /= 10;

            EXPECT_NEAR(foundVol, vol, epsilon);
        }

        static void testFlat(const SnapPeaTriangulation& s, const char* name,
                int places) {
            // Verify that the triangulation has a flat solution and the
            // volume is zero to the given number of decimal places.
            // Places are counted after the decimal point in standard
            // (non-scientific) notation.
            SCOPED_TRACE_CSTRING(name);
            SCOPED_TRACE_NUMERIC(places);

            ASSERT_FALSE(s.isNull());

            EXPECT_EQ(s.solutionType(), SnapPeaTriangulation::flat_solution);

            auto [foundVol, precision] = s.volumeWithPrecision();
            EXPECT_GE(precision, places);

            double epsilon = 0.5;
            for (int i = 0; i < places; ++i)
                epsilon /= 10;

            EXPECT_NEAR(foundVol, 0, epsilon);
        }

        void testDegenerate(Triangulation<3>& tri, const char* name) {
            // Verify that the triangulation has a degenerate solution.
            // For now we don't test volumes for degenerate solutions, since
            // these can go all over the shop.
            SCOPED_TRACE_CSTRING(name);

            SnapPeaTriangulation s(tri);
            ASSERT_FALSE(s.isNull());

            EXPECT_EQ(s.solutionType(),
                SnapPeaTriangulation::degenerate_solution);
        }

        static void testFilledHomology(SnapPeaTriangulation s, int m, int l,
                const std::string& expectedH1, const char* name) {
            // We pass s by value because we are going to edit it here.
            SCOPED_TRACE_CSTRING(name);
            SCOPED_TRACE_NUMERIC(m);
            SCOPED_TRACE_NUMERIC(l);

            ASSERT_FALSE(s.isNull());

            s.fill(m, l);

            EXPECT_EQ(s.homologyFilled().str(), expectedH1);
            EXPECT_EQ(s.fundamentalGroupFilled().abelianisation().str(),
                expectedH1);

            if (m == 0 && l == 0) {
                EXPECT_THROW({
                    Triangulation<3> t = s.filledAll();
                }, regina::FailedPrecondition);

                SnapPeaTriangulation t = s.filledPartial();
                EXPECT_EQ(t.homology().str(), expectedH1);
                EXPECT_EQ(t.homologyFilled().str(), expectedH1);
            } else if (s.countBoundaryComponents() == 1) {
                EXPECT_THROW({
                    SnapPeaTriangulation t = s.filledPartial();
                }, regina::FailedPrecondition);

                Triangulation<3> t = s.filledAll();
                regina::AbelianGroup ans = t.homology();
                EXPECT_EQ(t.homology().str(), expectedH1);
            } else {
                EXPECT_THROW({
                    Triangulation<3> t = s.filledAll();
                }, regina::FailedPrecondition);

                SnapPeaTriangulation t = s.filledPartial();
                EXPECT_EQ(t.homology().str(), expectedH1);
                EXPECT_EQ(t.homologyFilled().str(), expectedH1);
            }
        }
};

TEST_F(SnapPeaTest, volume) {
    testVolume(m2_1,   2.0298832128, 9, "M 2_1");
    testVolume(m2_2,   2.0298832128, 9, "M 2_2");
    testVolume(m3_9,   2.9441064867, 9, "M 3_9");
    testVolume(m4_52,  4.0597664256, 9, "M 4_52");
    testVolume(m4_1_2, 3.6638623767, 9, "M 4_1^2");
    testVolume(m4_4_2, 4.0597664256, 9, "M 4_4^2");

    testVolume(n1_1,     1.0149416064, 9, "N 1_1");
    testVolume(n2_1,     1.8319311884, 9, "N 2_1");
    testVolume(n2_1_2,   2.0298832128, 9, "N 2_1^2");
    testVolume(n4_14,    3.9696478012, 9, "N 4_14");
    testVolume(n4_9_2,   4.0597664256, 9, "N 4_9^2");
    testVolume(n4_1_2_1, 3.6638623767, 9, "N 4_1^2,1");

    testVolume(closedHypOr, 0.94270736, 7, "or_0.94270736");
    testVolume(closedHypNor, 2.02988321, 7, "nor_2.02988321");
    testVolume(weberSeifert, 11.1990647, 6, "Weber-Seifert");
}

TEST_F(SnapPeaTest, flat) {
    testFlat(flatOr, "Flat orientable", 9);
    testFlat(flatNor, "Flat non-orientable", 9);
}

TEST_F(SnapPeaTest, degenerate) {
    testDegenerate(degenerateOr, "Degenerate orientable");
    testDegenerate(degenerateNor, "Degenerate non-orientable");

    // Our last triangulation is a solid torus with a cusped boundary and an
    // internal (finite) vertex.
    {
        Triangulation<3> cuspedTorus;
        cuspedTorus.insertLayeredSolidTorus(1, 2);
        cuspedTorus.finiteToIdeal();
        testDegenerate(cuspedTorus, "Cusped solid torus with finite vertex");
    }
}

TEST_F(SnapPeaTest, filling) {
    // For now, just a couple of tests to ensure that SnapPea's filled
    // homology routines appear to be functioning correctly.

    // 1 boundary component, orientable:
    testFilledHomology(m2_1, 0, 0, "Z", "M 2_1");
    testFilledHomology(m2_1, 1, 1, "0", "M 2_1");
    testFilledHomology(m2_1, -3, 7, "Z_3", "M 2_1");

    // 2 boundary components, orientable:
    testFilledHomology(m4_4_2, 0, 0, "2 Z", "M 4_4^2");
    testFilledHomology(m4_4_2, 1, 1, "Z", "M 4_4^2");
    testFilledHomology(m4_4_2, -3, 7, "Z + Z_3", "M 4_4^2");

    // 1 boundary component, non-orientable:
    testFilledHomology(n1_1, 0, 0, "Z", "N 1_1");
    testFilledHomology(n1_1, 1, 0, "Z", "N 1_1");
    testFilledHomology(n1_1, -1, 0, "Z", "N 1_1");

    // 2 boundary components, non-orientable:
    testFilledHomology(n4_9_2, 0, 0, "Z + Z_2", "N 4_9^2");
    testFilledHomology(n4_9_2, 1, 0, "Z", "N 4_9^2");
    testFilledHomology(n4_9_2, -1, 0, "Z", "N 4_9^2");
}

TEST_F(SnapPeaTest, link) {
    testVolume(ExampleLink::figureEight(), 2.02988321282, 9, "Figure eight");
    testFilledHomology(ExampleLink::figureEight(), 1, 1, "0", "Figure eight");
    testFilledHomology(ExampleLink::figureEight(), -3, 7, "Z_3",
        "Figure eight");

    testFlat(ExampleLink::trefoil(), "Trefoil", 9);
    testFilledHomology(ExampleLink::trefoil(), 1, 1, "0", "Trefoil");
    testFilledHomology(ExampleLink::trefoil(), -3, 7, "Z_3", "Trefoil");

    testVolume(ExampleLink::whitehead(), 3.66386237671, 9, "Whitehead link");
    testFilledHomology(ExampleLink::whitehead(), 1, 1, "Z", "Whitehead link");
    testFilledHomology(ExampleLink::whitehead(), -3, 7, "Z + Z_3",
        "Whitehead link");
}

TEST_F(SnapPeaTest, spunBoundaries) {
    SnapPeaTriangulation t(Example<3>::figureEight());

    regina::NormalSurfaces s(t, regina::NS_QUAD);
    ASSERT_EQ(s.size(), 4);

    bool found[4] = {}; // initialises to false
    for (const regina::NormalSurface& f : s) {
        regina::MatrixInt m = f.boundaryIntersections();
        ASSERT_EQ(m.rows(), 1);
        ASSERT_EQ(m.columns(), 2);
        if (m.entry(0, 0) == 1 && m.entry(0, 1) == 4)
            found[0] = true;
        else if (m.entry(0, 0) == 1 && m.entry(0, 1) == -4)
            found[1] = true;
        else if (m.entry(0, 0) == -1 && m.entry(0, 1) == 4)
            found[2] = true;
        else if (m.entry(0, 0) == -1 && m.entry(0, 1) == -4)
            found[3] = true;
        else
            ADD_FAILURE() << "Unexpected boundary intersections: " << m.str();
    }

    EXPECT_TRUE(found[0]) << "Missing boundary intersections: [1, 4]";
    EXPECT_TRUE(found[1]) << "Missing boundary intersections: [1, -4]";
    EXPECT_TRUE(found[2]) << "Missing boundary intersections: [-1, 4]";
    EXPECT_TRUE(found[3]) << "Missing boundary intersections: [-1, -4]";
}

TEST_F(SnapPeaTest, incompatible) {
    // Verify that SnapPea _cannot_ work with these triangulations.

    // In most of these cases, we also run extra sanity checks to verify that
    // the triangulations are what we think they are, since otherwise a typo
    // in the gluing code could well go unnoticed.

    {
        // The empty triangulation
        Triangulation<3> empty;

        SnapPeaTriangulation snappea(empty);
        EXPECT_TRUE(snappea.isNull());
        EXPECT_EQ(snappea.size(), 0);
    }
    {
        // A (1,2,3) layered solid torus
        Triangulation<3> lst123;
        lst123.insertLayeredSolidTorus(1, 2);

        EXPECT_TRUE(lst123.isValid());
        EXPECT_TRUE(lst123.isConnected());
        EXPECT_TRUE(lst123.isOrientable());
        EXPECT_FALSE(lst123.isIdeal());
        EXPECT_TRUE(lst123.isStandard());
        EXPECT_TRUE(lst123.hasBoundaryTriangles());

        SnapPeaTriangulation snappea(lst123);
        EXPECT_TRUE(snappea.isNull());
        EXPECT_EQ(snappea.size(), 0);
    }
    {
        // Two disconnected copies of m2_1
        Triangulation<3> m2_1_m2_1;
        m2_1_m2_1 = Triangulation<3>::rehydrate("cabbbbaei");
        m2_1_m2_1.insertTriangulation(m2_1_m2_1);

        EXPECT_TRUE(m2_1_m2_1.isValid());
        EXPECT_FALSE(m2_1_m2_1.isConnected());
        EXPECT_TRUE(m2_1_m2_1.isOrientable());
        EXPECT_TRUE(m2_1_m2_1.isIdeal());
        EXPECT_TRUE(m2_1_m2_1.isStandard());
        EXPECT_FALSE(m2_1_m2_1.hasBoundaryTriangles());

        SnapPeaTriangulation snappea(m2_1_m2_1);
        EXPECT_TRUE(snappea.isNull());
        EXPECT_EQ(snappea.size(), 0);
    }
    {
        // An orientable triangulation with a genus two torus cusp
        Triangulation<3> genusTwoTorusCusp = Triangulation<3>::fromGluings(2, {
            { 0, 0, 1, {0,2,3,1} },
            { 0, 1, 1, {2,1,3,0} },
            { 0, 2, 1, {1,3,2,0} },
            { 0, 3, 1, {2,0,1,3} }});

        EXPECT_TRUE(genusTwoTorusCusp.isValid());
        EXPECT_TRUE(genusTwoTorusCusp.isConnected());
        EXPECT_TRUE(genusTwoTorusCusp.isOrientable());
        EXPECT_TRUE(genusTwoTorusCusp.isIdeal());
        EXPECT_FALSE(genusTwoTorusCusp.isStandard());
        EXPECT_FALSE(genusTwoTorusCusp.hasBoundaryTriangles());

        SnapPeaTriangulation snappea(genusTwoTorusCusp);
        EXPECT_TRUE(snappea.isNull());
        EXPECT_EQ(snappea.size(), 0);
    }
    {
        // A non-orientable triangulation with two projective plane cusps
        Triangulation<3> projPlaneCusps = Triangulation<3>::fromGluings(2, {
            { 0, 0, 0, {1,0,2,3} },
            { 0, 2, 1, {1,2,0,3} },
            { 0, 3, 1, {3,2,0,1} },
            { 1, 2, 1, {0,2,3,1} }});

        EXPECT_TRUE(projPlaneCusps.isValid());
        EXPECT_TRUE(projPlaneCusps.isConnected());
        EXPECT_FALSE(projPlaneCusps.isOrientable());
        EXPECT_TRUE(projPlaneCusps.isIdeal());
        EXPECT_FALSE(projPlaneCusps.isStandard());
        EXPECT_FALSE(projPlaneCusps.hasBoundaryTriangles());

        SnapPeaTriangulation snappea(projPlaneCusps);
        EXPECT_TRUE(snappea.isNull());
        EXPECT_EQ(snappea.size(), 0);
    }
    {
        // A triangulation with a genus four non-orientable cusp
        Triangulation<3> genusFourNonOrCusp = Triangulation<3>::fromGluings(2, {
            { 0, 0, 0, {1,2,0,3} },
            { 0, 2, 1, {1,2,0,3} },
            { 0, 3, 1, {0,2,3,1} },
            { 1, 2, 1, {0,2,3,1} }});

        EXPECT_TRUE(genusFourNonOrCusp.isValid());
        EXPECT_TRUE(genusFourNonOrCusp.isConnected());
        EXPECT_FALSE(genusFourNonOrCusp.isOrientable());
        EXPECT_TRUE(genusFourNonOrCusp.isIdeal());
        EXPECT_FALSE(genusFourNonOrCusp.isStandard());
        EXPECT_FALSE(genusFourNonOrCusp.hasBoundaryTriangles());

        SnapPeaTriangulation snappea(genusFourNonOrCusp);
        EXPECT_TRUE(snappea.isNull());
        EXPECT_EQ(snappea.size(), 0);
    }
    {
        // A triangulation with two invalid edges but whose vertices all have
        // 2-sphere links
        Triangulation<3> edgeInvalid = Triangulation<3>::fromGluings(1, {
            { 0, 0, 0, {1,0,3,2} },
            { 0, 2, 0, {1,0,3,2} }});

        EXPECT_FALSE(edgeInvalid.isValid());
        EXPECT_TRUE(edgeInvalid.isConnected());
        EXPECT_FALSE(edgeInvalid.isOrientable());
        EXPECT_FALSE(edgeInvalid.isIdeal());
        EXPECT_TRUE(edgeInvalid.isStandard());
        EXPECT_FALSE(edgeInvalid.hasBoundaryTriangles());

        SnapPeaTriangulation snappea(edgeInvalid);
        EXPECT_TRUE(snappea.isNull());
        EXPECT_EQ(snappea.size(), 0);
    }

    // And finally, verify that good inputs would _not_ have passed the
    // nullity tests above:
    {
        SnapPeaTriangulation snappea(Example<3>::whitehead());
        EXPECT_FALSE(snappea.isNull());
        EXPECT_NE(snappea.size(), 0);
    }
}

static bool looksIdentical(const SnapPeaTriangulation& a,
        const SnapPeaTriangulation& b) {
    if (a.size() != b.size())
        return false;
    if (a.countComponents() != b.countComponents())
        return false;
    if (a.countCusps() != b.countCusps())
        return false;
    if (a.countCompleteCusps() != b.countCompleteCusps())
        return false;
    if (a.countFilledCusps() != b.countFilledCusps())
        return false;
    if (a != b)
        return false;

    if (a.isoSig() != b.isoSig())
        return false;
    if (a.snapPea() != b.snapPea())
        return false;

    return true;
}

static void verifyCopyMoveNull(const SnapPeaTriangulation& t,
        const char* name) {
    SCOPED_TRACE_CSTRING(name);

    ASSERT_TRUE(t.isNull());
    EXPECT_EQ(t.size(), 0);

    SnapPeaTriangulation copy(t);
    EXPECT_TRUE(copy.isNull());

    SnapPeaTriangulation move(std::move(copy));
    EXPECT_TRUE(move.isNull());

    SnapPeaTriangulation copyAss(ExampleSnapPea::figureEight());
    copyAss = t;
    EXPECT_TRUE(copyAss.isNull());

    SnapPeaTriangulation moveAss(ExampleSnapPea::figureEight());
    moveAss = std::move(copyAss);
    EXPECT_TRUE(moveAss.isNull());
}

static void verifyCopyMoveNonNull(const SnapPeaTriangulation& t,
        const char* name) {
    SCOPED_TRACE_CSTRING(name);

    ASSERT_FALSE(t.isNull());
    EXPECT_NE(t.size(), 0);

    ASSERT_GT(t.countCusps(), 0);
    ASSERT_GT(t.countVertices(), 0);
    const regina::Cusp* c0 = std::addressof(t.cusp(0));
    regina::Vertex<3>* v0 = t.vertex(0);

    SnapPeaTriangulation copy(t);
    EXPECT_TRUE(looksIdentical(copy, t));

    // Copy construction should use different cusps and vertices.
    ASSERT_GT(copy.countCusps(), 0);
    ASSERT_GT(copy.countVertices(), 0);
    const regina::Cusp* c1 = std::addressof(copy.cusp(0));
    regina::Vertex<3>* v1 = copy.vertex(0);
    EXPECT_NE(c1, c0);
    EXPECT_NE(v1, v0);

    SnapPeaTriangulation move(std::move(copy));
    EXPECT_TRUE(looksIdentical(move, t));

    // Move construction should use the same cusps and vertices.
    ASSERT_GT(move.countCusps(), 0);
    ASSERT_GT(move.countVertices(), 0);
    const regina::Cusp* c2 = std::addressof(move.cusp(0));
    regina::Vertex<3>* v2 = move.vertex(0);
    EXPECT_EQ(c2, c1);
    EXPECT_EQ(v2, v1);

    SnapPeaTriangulation copyAss;
    copyAss.newSimplex(); // Give it something to overwrite.
    copyAss = t;
    EXPECT_TRUE(looksIdentical(copyAss, t));

    // Copy assignment should use different cusps and vertices.
    ASSERT_GT(copyAss.countCusps(), 0);
    ASSERT_GT(copyAss.countVertices(), 0);
    const regina::Cusp* c3 = std::addressof(copyAss.cusp(0));
    regina::Vertex<3>* v3 = copyAss.vertex(0);
    EXPECT_NE(c3, c0);
    EXPECT_NE(v3, v0);

    SnapPeaTriangulation moveAss;
    moveAss.newSimplex(); // Give it something to overwrite.
    moveAss = std::move(copyAss);
    EXPECT_TRUE(looksIdentical(moveAss, t));

    // Move assignment should use the same cusps and vertices.
    ASSERT_GT(moveAss.countCusps(), 0);
    ASSERT_GT(moveAss.countVertices(), 0);
    const regina::Cusp* c4 = std::addressof(moveAss.cusp(0));
    regina::Vertex<3>* v4 = moveAss.vertex(0);
    EXPECT_EQ(c4, c3);
    EXPECT_EQ(v4, v3);
}

TEST_F(SnapPeaTest, copyMove) {
    verifyCopyMoveNonNull(m2_1, "M 2_1");
    verifyCopyMoveNonNull(m2_2, "M 2_2");
    verifyCopyMoveNonNull(m3_9, "M 3_9");
    verifyCopyMoveNonNull(m4_52, "M 4_52");
    verifyCopyMoveNonNull(m4_1_2, "M 4_1^2");
    verifyCopyMoveNonNull(m4_4_2, "M 4_4^2");

    verifyCopyMoveNonNull(n1_1, "N 1_1");
    verifyCopyMoveNonNull(n2_1, "N 2_1");
    verifyCopyMoveNonNull(n2_1_2, "N 2_1^2");
    verifyCopyMoveNonNull(n4_14, "N 4_14");
    verifyCopyMoveNonNull(n4_9_2, "N 4_9^2");
    verifyCopyMoveNonNull(n4_1_2_1, "N 4_1^2,1");

    verifyCopyMoveNonNull(closedHypOr, "or_0.94270736");
    verifyCopyMoveNonNull(closedHypNor, "nor_2.02988321");
    verifyCopyMoveNonNull(weberSeifert, "Weber-Seifert");

    verifyCopyMoveNonNull(flatOr, "Flat orientable");
    verifyCopyMoveNonNull(flatNor, "Flat non-orientable");
    verifyCopyMoveNonNull(degenerateOr, "Degenerate orientable");
    verifyCopyMoveNonNull(degenerateNor, "Degenerate non-orientable");

    // Try a couple of null SnapPea triangulations also.
    // See incompatible() for details on where these triangulations came from.

    verifyCopyMoveNull(Triangulation<3>(), "Empty");
    verifyCopyMoveNull(Example<3>::lst(1,2), "LST(1,2,3)");
    {
        Triangulation<3> t(m2_1);
        t.insertTriangulation(m2_1);
        verifyCopyMoveNull(t, "M 2_1 U M 2_1");
    }
}

TEST_F(SnapPeaTest, swapping) {
    // Verify that swap() via the SnapPeaTriangulation interface swaps cusps
    // and snappea data correctly.
    {
        SnapPeaTriangulation a = ExampleSnapPea::figureEight();
        SnapPeaTriangulation b = ExampleSnapPea::whiteheadLink();

        a.volume();
        b.volume();

        swap(a, b);

        ASSERT_FALSE(a.isNull());
        ASSERT_FALSE(b.isNull());
        EXPECT_EQ(a.countCusps(), 2);
        EXPECT_EQ(b.countCusps(), 1);
        EXPECT_EQ(std::floor(a.volume()), 3);
        EXPECT_EQ(std::floor(b.volume()), 2);

        std::iter_swap(&a, &b);

        ASSERT_FALSE(a.isNull());
        ASSERT_FALSE(b.isNull());
        EXPECT_EQ(a.countCusps(), 1);
        EXPECT_EQ(b.countCusps(), 2);
        EXPECT_EQ(std::floor(a.volume()), 2);
        EXPECT_EQ(std::floor(b.volume()), 3);
    }

    // Verify that swap() via the Triangulation<3> interface nullifies both
    // SnapPea triangulations.
    {
        SnapPeaTriangulation a = ExampleSnapPea::figureEight();
        SnapPeaTriangulation b = ExampleSnapPea::whiteheadLink();

        a.volume();
        b.volume();

        swap(static_cast<Triangulation<3>&>(a), b);

        EXPECT_TRUE(a.isNull());
        EXPECT_TRUE(b.isNull());
    }
    {
        // Verify that swap() via the Triangulation<3> interface nullifies
        // both SnapPea triangulations.
        SnapPeaTriangulation a = ExampleSnapPea::figureEight();
        SnapPeaTriangulation b = ExampleSnapPea::whiteheadLink();

        a.volume();
        b.volume();

        swap(a, static_cast<Triangulation<3>&>(b));

        EXPECT_TRUE(a.isNull());
        EXPECT_TRUE(b.isNull());
    }
    {
        SnapPeaTriangulation a = ExampleSnapPea::figureEight();
        SnapPeaTriangulation b = ExampleSnapPea::whiteheadLink();

        a.volume();
        b.volume();

        swap(static_cast<Triangulation<3>&>(a),
            static_cast<Triangulation<3>&>(b));

        EXPECT_TRUE(a.isNull());
        EXPECT_TRUE(b.isNull());
    }
}

static void testStability(const Triangulation<3>& tri, const char* sig) {
    SCOPED_TRACE_CSTRING(sig);

    // Just make sure SnapPea can work with the triangulation without crashing.
    EXPECT_NO_THROW({
        try {
            SnapPeaTriangulation s(tri);
            s.volume();
            s.randomize();
            s.volume();
            Triangulation<3> t(s);
        } catch (const regina::SnapPeaIsNull&) {
            // Null triangulations we are happy to deal with.
        }
    });
}

TEST_F(SnapPeaTest, stability) {
    runCensusAllNoBdry(&testStability);
}
