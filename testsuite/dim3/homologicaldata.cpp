
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
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

#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "triangulation/example3.h"
#include "triangulation/homologicaldata.h"
#include "triangulation/dim3.h"
#include "testsuite/dim3/testtriangulation.h"

using regina::Example;
using regina::HomologicalData;
using regina::Triangulation;

/**
 * Thanks to Ryan Budney for supplying the questions and answers for
 * these tests!
 */
class HomologicalDataTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(HomologicalDataTest);

    CPPUNIT_TEST(homologyConsistency);
    CPPUNIT_TEST(bdryManifoldMapH1);
    CPPUNIT_TEST(standardCells);
    CPPUNIT_TEST(dualCells);
    CPPUNIT_TEST(torsionRankVector);
    CPPUNIT_TEST(torsionSigmaVector);
    CPPUNIT_TEST(torsionLegendreSymbolVector);
    CPPUNIT_TEST(embeddabilityComment);

    CPPUNIT_TEST_SUITE_END();

    private:
        // Some triangulations are pointers; this is because they are
        // constructed on the fly by Example<3>.

        // Closed orientable:
        Triangulation<3>* s3;
            /**< A one-tetrahedron 3-sphere. */
        Triangulation<3>* s2xs1;
            /**< The two-tetrahedron S^2 x S^1. */
        Triangulation<3> lens3_1;
            /**< The layered lens space L(3,1). */
        Triangulation<3> lens4_1;
            /**< The layered lens space L(4,1). */
        Triangulation<3> lens7_1;
            /**< The layered lens space L(7,1). */
        Triangulation<3> d88xz15;
            /**< The orbit manifold S^3 / D_88 x Z_15, also known
                 as SFS [S2: (2,1) (2,1) (11,19)]. */
        Triangulation<3>* poincare;
            /**< The poincare homology sphere. */
        Triangulation<3>* weberSeifert;
            /**< The Weber-Seifert dodecahedral space. */
        Triangulation<3> torusBundleA;
            /**< The torus bundle T x I / [ 0,1 | -1,0 ], built using an
                 augmented triangular solid torus. */
        Triangulation<3> torusBundleB;
            /**< The torus bundle T x I / [ -1,1 | -1,0 ], built using an
                 augmented triangular solid torus. */
        Triangulation<3> twistedKBxS1;
            /**< The orientable twisted product KB/n2 x~ S^1. */
        Triangulation<3> closedHypA;
            /**< The manifold with volume 1.01494161 from the
                 Hodgson-Weeks closed orientable census. */
        Triangulation<3> closedHypB;
            /**< The manifold with volume 2.45402944 from the
                 Hodgson-Weeks closed orientable census. */
        Triangulation<3> closedHypC;
            /**< The manifold with volume 1.26370924 from Regina's
                 closed orientable census. */

        // Closed non-orientable:
        Triangulation<3> norA;
            /**< The non-orientable manifold SFS [M_/n2: (2,1)]. */
        Triangulation<3> norB;
            /**< The non-orientable manifold SFS [RP2: (2,1) (2,1)]. */
        Triangulation<3> norTorusBundle;
            /**< The non-orientable torus bundle T x I / [ 2,1 | 1,0 ]. */

        // Ideal:
        Triangulation<3>* gieseking;
            /**< The Gieseking manifold. */
        Triangulation<3>* figureEight;
            /**< The figure eight knot complement. */
        Triangulation<3> m003;
            /**< The manifold m003 from the cusped hyperbolic census of
                 Callahan, Hildebrand and Weeks, as shipped with SnapPea. */
        Triangulation<3> m041;
            /**< The manifold m041 from the cusped hyperbolic census of
                 Callahan, Hildebrand and Weeks, as shipped with SnapPea. */
        Triangulation<3> m045;
            /**< The manifold m045 from the cusped hyperbolic census of
                 Callahan, Hildebrand and Weeks, as shipped with SnapPea. */
        Triangulation<3> s028;
            /**< The manifold s028 from the cusped hyperbolic census of
                 Callahan, Hildebrand and Weeks, as shipped with SnapPea. */
        Triangulation<3> s887;
            /**< The manifold s887 from the cusped hyperbolic census of
                 Callahan, Hildebrand and Weeks, as shipped with SnapPea. */
        Triangulation<3> s955;
            /**< The manifold s955 from the cusped hyperbolic census of
                 Callahan, Hildebrand and Weeks, as shipped with SnapPea. */
        Triangulation<3> genusTwoBdry;
            /**< An ideal triangulation with a single genus two cusp. */

        // Bounded:
        Triangulation<3> lst3_4_7;
            /**< The layered solid torus LST(3,4,7). */

    public:
        void setUp() {
            // First deal with ready-made example triangulations.
            s3 = Example<3>::threeSphere();
            s2xs1 = Example<3>::s2xs1();
            poincare = Example<3>::poincareHomologySphere();
            weberSeifert = Example<3>::weberSeifert();
            gieseking = Example<3>::gieseking();
            figureEight = Example<3>::figureEight();

            // Next deal with triangulations that are easy to build.
            lens3_1.insertLayeredLensSpace(3, 1);
            lens4_1.insertLayeredLensSpace(4, 1);
            lens7_1.insertLayeredLensSpace(7, 1);
            d88xz15.insertAugTriSolidTorus(2, -1, 2, -1, 11, -30);
            torusBundleA.insertAugTriSolidTorus(2, 1, 4, -7, 4, 1);
            torusBundleB.insertAugTriSolidTorus(3, -1, 3, -1, 3, -1);
            lst3_4_7.insertLayeredSolidTorus(3, 4);

            // The others we recreate using dehydration strings.
            // The dehydration strings were obtained by running
            // Triangulation<3>::dehydrate() over triangulations from
            // the relevant census data files.
            twistedKBxS1.insertRehydration("gepaacdfefefknakanx");
            closedHypA.insertRehydration("jgpadaaeffghfiihirmxitdagbj");
            closedHypB.insertRehydration("mnnjjcabehfgjijkijllaaqabhoehrtab");
            closedHypC.insertRehydration("jlncbaabdghfggiiiafxhbccqei");
            norA.insertRehydration("jofbdaabccfhgihiiffhofoxohx");
            norB.insertRehydration("gepaacdfefefaaknnkx");
            norTorusBundle.insertRehydration("hipabcedfggfggbgforro");
            m003.insertRehydration("cabbbbmlq");
            m041.insertRehydration("eahcdbddbokpv");
            m045.insertRehydration("eahccdcddbfof");
            s028.insertRehydration("gkfacaccdeffffohhhf");
            s887.insertRehydration("gknaaacefffennqbnjf");
            s955.insertRehydration("gbpaabcfdffefohfxhf");
            genusTwoBdry.insertRehydration("eanadccdnxfno");
        }

        void tearDown() {
            // Delete triangulations that were dynamically allocated.
            delete s3;
            delete s2xs1;
            delete poincare;
            delete weberSeifert;
            delete gieseking;
            delete figureEight;
        }

        void verifyHomologyConsistency(const Triangulation<3>& tri,
                const char* name) {
            HomologicalData dat(tri);

            std::string fromStandard, fromDual;
            for (int i = 0; i <= 3; ++i) {
                fromStandard = dat.homology(i).str();
                fromDual = dat.dualHomology(i).str();

                if (fromStandard != fromDual) {
                    std::ostringstream msg;
                    msg << name << ": Homologies computed from "
                        "standard and dual complexes do not agree "
                        "(H_" << i << " = " << fromStandard
                        << " vs " << fromDual << ").";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            std::string fromTri = tri.homology().str();
            fromDual = dat.dualHomology(1).str();

            if (fromTri != fromDual) {
                std::ostringstream msg;
                msg << name << ": Homologies computed from "
                    "triangulation and dual complexes do not agree "
                    "(H_1 = " << fromTri << " vs " << fromDual << ").";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void homologyConsistency() {
            verifyHomologyConsistency(*s3, "S^3");
            verifyHomologyConsistency(*s2xs1, "S^2 x S^1");
            verifyHomologyConsistency(lens3_1, "L(3,1)");
            verifyHomologyConsistency(lens4_1, "L(4,1)");
            verifyHomologyConsistency(lens7_1, "L(7,1)");
            verifyHomologyConsistency(*poincare, "Poincare homology sphere");
            verifyHomologyConsistency(*weberSeifert, "Weber-Seifert");
            verifyHomologyConsistency(d88xz15, "S^3 / D_88 x Z_15");
            verifyHomologyConsistency(closedHypA,
                "Closed Hyp (vol=1.01494161)");
            verifyHomologyConsistency(closedHypB,
                "Closed Hyp (vol=2.45402944)");
            verifyHomologyConsistency(closedHypC,
                "Closed Hyp (vol=1.26370924)");
            verifyHomologyConsistency(torusBundleA, "T x I / [ 0,1 | -1,0 ]");
            verifyHomologyConsistency(torusBundleB, "T x I / [ -1,1 | -1,0 ]");
            verifyHomologyConsistency(twistedKBxS1, "KB/n2 x~ S^1");
            verifyHomologyConsistency(norA, "SFS [M_/n2: (2,1)]");
            verifyHomologyConsistency(norB, "SFS [RP2: (2,1) (2,1)]");
            verifyHomologyConsistency(norTorusBundle, "T x I / [ 2,1 | 1,0 ]");
            verifyHomologyConsistency(lst3_4_7, "LST(3,4,7)");
            verifyHomologyConsistency(*gieseking, "Gieseking manifold");
            verifyHomologyConsistency(*figureEight,
                "Figure eight knot complement");
            verifyHomologyConsistency(m003, "SnapPea m003");
            verifyHomologyConsistency(m041, "SnapPea m041");
            verifyHomologyConsistency(m045, "SnapPea m045");
            verifyHomologyConsistency(s028, "SnapPea s028");
            verifyHomologyConsistency(s887, "SnapPea s887");
            verifyHomologyConsistency(s955, "SnapPea s955");
            verifyHomologyConsistency(genusTwoBdry,
                "Manifold with genus two cusp");
        }

        void verifyBdryManifoldMapH1(const Triangulation<3>& tri,
                const char* name, const char* ans) {
            HomologicalData dat(tri);
            std::string val = dat.bdryHomologyMap(1).str();
            if (val != ans) {
                std::ostringstream msg;
                msg << name << ": Map from H1(bdry) to H1(mfd) is "
                    << val << ", not " << ans << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void bdryManifoldMapH1() {
            verifyBdryManifoldMapH1(lens7_1, "L(7,1)", "zero map");
            verifyBdryManifoldMapH1(d88xz15, "S^3 / D_88 x Z_15", "zero map");
            verifyBdryManifoldMapH1(*weberSeifert, "Weber-Seifert", "zero map");
            verifyBdryManifoldMapH1(closedHypA, "Closed Hyp (vol=1.01494161)",
                "zero map");
            verifyBdryManifoldMapH1(closedHypB, "Closed Hyp (vol=2.45402944)",
                "zero map");
            verifyBdryManifoldMapH1(norA, "SFS [M_/n2: (2,1)]", "zero map");
            verifyBdryManifoldMapH1(s028, "SnapPea s028",
                "kernel Z | cokernel Z_16 | image Z");
            verifyBdryManifoldMapH1(s955, "SnapPea s955",
                "kernel Z | cokernel Z_20 | image Z + Z_2");
            verifyBdryManifoldMapH1(lst3_4_7, "LST(3,4,7)",
                "epic, with kernel Z");
        }

        void verifyStandardCells(Triangulation<3>& tri, const char* name,
                unsigned long c0, unsigned long c1, unsigned long c2,
                unsigned long c3) {
            HomologicalData dat(tri);
            unsigned long ans[4] = { c0, c1, c2, c3 };

            unsigned long val;
            for (int i = 0; i < 4; i++) {
                val = dat.countStandardCells(i);
                if (val != ans[i]) {
                    std::ostringstream msg;
                    msg << name << ": Number of standard cells of dimension "
                        << i << " is " << val << ", not " << ans[i] << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void standardCells() {
            verifyStandardCells(lens7_1, "L(7,1)", 1, 5, 8, 4);
            verifyStandardCells(d88xz15, "S^3 / D_88 x Z_15", 1, 10, 18, 9);
            verifyStandardCells(*weberSeifert, "Weber-Seifert", 1, 24, 46, 23);
            verifyStandardCells(closedHypA, "Closed Hyp (vol=1.01494161)",
                1, 10, 18, 9);
            verifyStandardCells(closedHypB, "Closed Hyp (vol=2.45402944)",
                1, 13, 24, 12);
            verifyStandardCells(norA, "SFS [M_/n2: (2,1)]", 1, 10, 18, 9);
            verifyStandardCells(s028, "SnapPea s028", 12, 42, 36, 6);
            verifyStandardCells(s955, "SnapPea s955", 12, 42, 36, 6);
            verifyStandardCells(lst3_4_7, "LST(3,4,7)", 1, 5, 7, 3);
        }

        void verifyDualCells(Triangulation<3>& tri, const char* name,
                unsigned long c0, unsigned long c1, unsigned long c2,
                unsigned long c3) {
            HomologicalData dat(tri);
            unsigned long ans[4] = { c0, c1, c2, c3 };

            unsigned long val;
            for (int i = 0; i < 4; i++) {
                val = dat.countDualCells(i);
                if (val != ans[i]) {
                    std::ostringstream msg;
                    msg << name << ": Number of dual cells of dimension "
                        << i << " is " << val << ", not " << ans[i] << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void dualCells() {
            verifyDualCells(lens7_1, "L(7,1)", 4, 8, 5, 1);
            verifyDualCells(d88xz15, "S^3 / D_88 x Z_15", 9, 18, 10, 1);
            verifyDualCells(*weberSeifert, "Weber-Seifert", 23, 46, 24, 1);
            verifyDualCells(closedHypA, "Closed Hyp (vol=1.01494161)",
                9, 18, 10, 1);
            verifyDualCells(closedHypB, "Closed Hyp (vol=2.45402944)",
                12, 24, 13, 1);
            verifyDualCells(norA, "SFS [M_/n2: (2,1)]", 9, 18, 10, 1);
            verifyDualCells(s028, "SnapPea s028", 6, 12, 6, 0);
            verifyDualCells(s955, "SnapPea s955", 6, 12, 6, 0);
            verifyDualCells(lst3_4_7, "LST(3,4,7)", 3, 5, 2, 0);
        }

        void verifyTorsionRankVector(Triangulation<3>& tri,
                const char* name, const char* ans) {
            HomologicalData dat(tri);
            std::string val = dat.torsionRankVectorString();
            if (val != ans) {
                std::ostringstream msg;
                msg << name << ": Torsion form rank vector is "
                    << val << ", not " << ans << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void torsionRankVector() {
            verifyTorsionRankVector(lens7_1, "L(7,1)", "7(1)");
            verifyTorsionRankVector(d88xz15, "S^3 / D_88 x Z_15",
                "2(0 0 1) 3(1) 5(1)");
            verifyTorsionRankVector(*weberSeifert, "Weber-Seifert", "5(3)");
            verifyTorsionRankVector(closedHypA, "Closed Hyp (vol=1.01494161)",
                "2(1) 3(2)");
            verifyTorsionRankVector(closedHypB, "Closed Hyp (vol=2.45402944)",
                "7(2)");
            verifyTorsionRankVector(norA, "SFS [M_/n2: (2,1)]", "2(1 1)");
            verifyTorsionRankVector(s028, "SnapPea s028", "2(0 0 0 1)");
            verifyTorsionRankVector(s955, "SnapPea s955", "2(0 1) 5(1)");
            verifyTorsionRankVector(lst3_4_7, "LST(3,4,7)", "no torsion");
        }

        void verifyTorsionSigmaVector(Triangulation<3>& tri,
                const char* name, const char* ans) {
            HomologicalData dat(tri);
            std::string val = dat.torsionSigmaVectorString();
            if (val != ans) {
                std::ostringstream msg;
                msg << name << ": 2-torsion sigma vector is "
                    << val << ", not " << ans << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void torsionSigmaVector() {
            verifyTorsionSigmaVector(lens7_1, "L(7,1)", "no 2-torsion");
            verifyTorsionSigmaVector(d88xz15, "S^3 / D_88 x Z_15", "7 7 inf");
            verifyTorsionSigmaVector(*weberSeifert, "Weber-Seifert",
                "no 2-torsion");
            verifyTorsionSigmaVector(closedHypA, "Closed Hyp (vol=1.01494161)",
                "inf");
            verifyTorsionSigmaVector(closedHypB, "Closed Hyp (vol=2.45402944)",
                "no 2-torsion");
            verifyTorsionSigmaVector(norA, "SFS [M_/n2: (2,1)]",
                "manifold is non-orientable");
            verifyTorsionSigmaVector(s028, "SnapPea s028", "7 7 7 inf");
            verifyTorsionSigmaVector(s955, "SnapPea s955", "inf 0");
            verifyTorsionSigmaVector(lst3_4_7, "LST(3,4,7)", "no 2-torsion");
        }

        void verifyTorsionLegendreSymbolVector(Triangulation<3>& tri,
                const char* name, const char* ans) {
            HomologicalData dat(tri);
            std::string val = dat.torsionLegendreSymbolVectorString();
            if (val != ans) {
                std::ostringstream msg;
                msg << name << ": Odd p-torsion Legendre symbol vector is "
                    << val << ", not " << ans << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void torsionLegendreSymbolVector() {
            verifyTorsionLegendreSymbolVector(lens7_1, "L(7,1)", "7(1)");
            verifyTorsionLegendreSymbolVector(d88xz15, "S^3 / D_88 x Z_15",
                "3(1) 5(1)");
            verifyTorsionLegendreSymbolVector(*weberSeifert,
                "Weber-Seifert", "5(1)");
            verifyTorsionLegendreSymbolVector(closedHypA,
                "Closed Hyp (vol=1.01494161)", "3(1)");
            verifyTorsionLegendreSymbolVector(closedHypB,
                "Closed Hyp (vol=2.45402944)", "7(-1)");
            verifyTorsionLegendreSymbolVector(norA, "SFS [M_/n2: (2,1)]",
                "manifold is non-orientable");
            verifyTorsionLegendreSymbolVector(s028, "SnapPea s028",
                "no odd p-torsion");
            verifyTorsionLegendreSymbolVector(s955, "SnapPea s955", "5(-1)");
            verifyTorsionLegendreSymbolVector(lst3_4_7, "LST(3,4,7)",
                "no odd p-torsion");
        }

        void verifyEmbeddability(const Triangulation<3>& tri,
                const char* name, const char* ans) {
            HomologicalData dat(tri);
            std::string val = dat.embeddabilityComment();
            if (val != ans) {
                std::ostringstream msg;
                msg << name << ": Embeddability comment is \"" << val
                    << "\", not \"" << ans << "\".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void embeddabilityComment() {
            // Watch the comment for S^3 change once we run a
            // three-sphere recognition.
            verifyEmbeddability(*s3, "S^3", "Manifold is a homology 3-sphere.");
            s3->isThreeSphere();
            verifyEmbeddability(*s3, "S^3", "This manifold is S^3.");

            verifyEmbeddability(*s2xs1, "S^2 x S^1", "No information.");
            verifyEmbeddability(*poincare, "Poincare homology sphere",
                "Manifold is a homology 3-sphere.");
            verifyEmbeddability(*weberSeifert,
                "Weber-Seifert dodecahedral space",
                "Does not embed in homology 4-sphere.  "
                "Manifold is a rational homology sphere.");
            verifyEmbeddability(lens3_1, "L(3,1)",
                "Does not embed in homology 4-sphere.  "
                "Manifold is a rational homology sphere.");
            verifyEmbeddability(lens4_1, "L(4,1)",
                "This manifold, once-punctured, does not embed in a "
                "homology 4-sphere.  Manifold is a rational homology sphere.");
            verifyEmbeddability(closedHypC, "Closed Hyp (vol=1.26370924)",
                "The torsion linking form is of hyperbolic type.  "
                "Manifold is a rational homology sphere.");
            verifyEmbeddability(torusBundleA, "T x I / [ 0,1 | -1,0 ]",
                "This manifold, once-punctured, does not embed in a "
                "homology 4-sphere.");
            verifyEmbeddability(torusBundleB, "T x I / [ -1,1 | -1,0 ]",
                "Does not embed in homology 4-sphere.");
            verifyEmbeddability(twistedKBxS1, "KB/n2 x~ S^1",
                "The torsion linking form is of hyperbolic type.");
            verifyEmbeddability(norB, "SFS [RP2: (2,1) (2,1)]",
                "Orientation cover has hyperbolic torsion linking form.");
            verifyEmbeddability(norTorusBundle, "T x I / [ 2,1 | 1,0 ]",
                "Does not embed in homology 4-sphere.");
            verifyEmbeddability(*gieseking, "Gieseking manifold",
                "Orientation cover satisfies KK 2-torsion condition.");
            verifyEmbeddability(*figureEight, "Figure eight knot complement",
                "Embeds in a homology 3-sphere as a knot complement.");
            verifyEmbeddability(m003, "SnapPea m003",
                "Embeds in rational homology 3-sphere.  KK 2-tor "
                "condition satisfied.");
            verifyEmbeddability(m041, "SnapPea m041",
                "Does not embed in homology 4-sphere.");
            verifyEmbeddability(m045, "SnapPea m045",
                "Embeds in rational homology 3-sphere but not "
                "homology 4-sphere.");
            verifyEmbeddability(s887, "SnapPea s887",
                "Embeds in a homology 3-sphere as a link complement.");
            verifyEmbeddability(genusTwoBdry, "Manifold with genus two cusp",
                "Embeds in a homology 3-sphere as a graph complement.");
        }
};

void addHomologicalData(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(HomologicalDataTest::suite());
}

