
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

#include <cmath>
#include <iomanip>
#include <cppunit/extensions/HelperMacros.h>
#include "maths/matrix.h"
#include "snappea/snappeatriangulation.h"
#include "surfaces/normalsurfaces.h"
#include "triangulation/example3.h"
#include "triangulation/dim3.h"

#include "testsuite/exhaustive.h"
#include "testsuite/snappea/testsnappea.h"

using regina::Example;
using regina::Perm;
using regina::SnapPeaTriangulation;
using regina::Tetrahedron;
using regina::Triangulation;

class SnapPeaTriangulationTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(SnapPeaTriangulationTest);

    CPPUNIT_TEST(incompatible);
    CPPUNIT_TEST(volume);
    CPPUNIT_TEST(flat);
    CPPUNIT_TEST(degenerate);
    CPPUNIT_TEST(spunBoundaries);
    CPPUNIT_TEST(stability);
    CPPUNIT_TEST(filling);

    CPPUNIT_TEST_SUITE_END();

    private:
        /**
         * Triangulations from "A Census of Cusped Hyperbolic 3-Manifolds",
         * Callahan, Hildebrand and Weeks, Mathematics of Computation 68/225,
         * 1999, pp 321--332.
         */

        // Orientable 3-manifolds:
        Triangulation<3> m2_1, m2_2, m3_9, m4_52, m4_1_2, m4_4_2;

        // Non-orientable 3-manifolds:
        Triangulation<3> n1_1, n2_1, n2_1_2, n4_14, n4_9_2, n4_1_2_1;

        /**
         * Triangulations from "Symmetries, isometries and length
         * spectra of closed hyperbolic three-manifolds",
         * Craig D. Hodgson and Jeffrey R. Weeks,
         * Experiment. Math. 3/4, 1994, pp 261--274.
         *
         * Note that for the time being these triangulations will be
         * rejected by SnapPeaTriangulation, since most SnapPea
         * functions require an ideal triangulation.
         */
        Triangulation<3> closedHypOr, closedHypNor;

        /**
         * The Weber-Seifert dodecahedral space:
         */
        Triangulation<3> weberSeifert;

        /**
         * Triangulations of 3-manifolds whose reported volume should be zero.
         *
         * These were found through an exhaustive census of small ideal
         * triangulations (they do not appear in SnapPea's hyperbolic census).
         */
        Triangulation<3> flatOr, flatNor, degenerateOr, degenerateNor;

        /**
         * Triangulations that SnapPea should refuse to deal with.
         */
        Triangulation<3> empty;
            /**< An empty triangulation. */
        Triangulation<3> lst123;
            /**< A (1,2,3) layered solid torus. */
        Triangulation<3> m2_1_m2_1;
            /**< Two disconnected copies of m2_1. */
        Triangulation<3> genusTwoTorusCusp;
            /**< A orientable triangulation with a genus two torus cusp. */
        Triangulation<3> projPlaneCusps;
            /**< A non-orientable triangulation with two projective plane
                 cusps. */
        Triangulation<3> genusFourNonOrCusp;
            /**< A triangulation with a genus four non-orientable cusp
             * (i.e., a non-orientable analogue of the two-holed torus). */
        Triangulation<3> cuspedTorus;
            /**< A solid torus with a cusped boundary and a finite vertex. */
        Triangulation<3> edgeInvalid;
            /**< A triangulation with two invalid edges but whose
                 vertices all have 2-sphere links. */

    public:
        void copyAndDelete(Triangulation<3>& dest, Triangulation<3>* source) {
            dest.insertTriangulation(*source);
            delete source;
        }

        void setUp() {
            // Keep the kernel quiet.  It interferes with the test
            // suite's running progress messages.
            SnapPeaTriangulation::disableKernelMessages();

            Tetrahedron<3>* t;
            Tetrahedron<3>* s;

            m2_1.insertRehydration("cabbbbaei");
            m2_1.setLabel("M 2_1");
            m2_2.insertRehydration("cabbbbapt");
            m2_2.setLabel("M 2_2");
            m3_9.insertRehydration("dagacccfwkn");
            m3_9.setLabel("M 3_9");
            m4_52.insertRehydration("ebdbcdddaqhie");
            m4_52.setLabel("M 4_52");
            m4_1_2.insertRehydration("eahbcdddhsssj");
            m4_1_2.setLabel("M 4_1^2");
            m4_4_2.insertRehydration("ebdbcdddddddx");
            m4_4_2.setLabel("M 4_4^2");

            n1_1.insertRehydration("baaaade");
            n1_1.setLabel("N 1_1");
            n2_1.insertRehydration("cabbbbabw");
            n2_1.setLabel("N 2_1");
            n2_1_2.insertRehydration("cabbbbcdw");
            n2_1_2.setLabel("N 2_1^2");
            n4_14.insertRehydration("eahdccddakfhq");
            n4_14.setLabel("N 4_14");
            n4_9_2.insertRehydration("ebdbcdddcemre");
            n4_9_2.setLabel("N 4_9^2");
            n4_1_2_1.insertRehydration("eahbcdddjxxxj");
            n4_1_2_1.setLabel("N 4_1^2,1");

            // Note: the non-orientable manifold below is the same as
            // Example<3>::smallClosedNonOrblHyperbolic()),
            // but if we build it from Example<3> then we
            // seem to get a degenerate solution.  Using the isosig
            // gives the same triangulation with a different labelling,
            // which seems to prod SnapPea into finding a better
            // (non_geometric) solution instead.
            copyAndDelete(closedHypOr,
                Example<3>::smallClosedOrblHyperbolic());
            closedHypOr.setLabel("or_0.94270736");
            copyAndDelete(closedHypNor, Triangulation<3>::fromIsoSig(
                "lLLLALAQccegffiijkikkkknawmhvwcls"));
            closedHypNor.setLabel("nor_2.02988321");

            copyAndDelete(weberSeifert,
                Example<3>::weberSeifert());
            weberSeifert.setLabel("Weber-Seifert");

            t = flatOr.newTetrahedron();
            s = flatOr.newTetrahedron();
            t->join(0, s, Perm<4>(0,1,2,3));
            t->join(1, s, Perm<4>(0,1,2,3));
            t->join(2, s, Perm<4>(1,3,2,0));
            t->join(3, s, Perm<4>(1,2,0,3));

            t = flatNor.newTetrahedron();
            s = flatNor.newTetrahedron();
            t->join(0, s, Perm<4>(0,1,2,3));
            t->join(1, s, Perm<4>(2,1,0,3));
            t->join(2, s, Perm<4>(1,3,2,0));
            t->join(3, s, Perm<4>(2,1,0,3));

            t = degenerateOr.newTetrahedron();
            s = degenerateOr.newTetrahedron();
            t->join(0, t, Perm<4>(1,0,2,3));
            t->join(2, s, Perm<4>(1,2,0,3));
            t->join(3, s, Perm<4>(0,2,3,1));
            s->join(2, s, Perm<4>(1,2,3,0));

            t = degenerateNor.newTetrahedron();
            s = degenerateNor.newTetrahedron();
            t->join(0, t, Perm<4>(1,0,2,3));
            t->join(2, s, Perm<4>(1,2,0,3));
            t->join(3, s, Perm<4>(0,3,2,1));
            s->join(2, s, Perm<4>(0,2,3,1));

            lst123.insertLayeredSolidTorus(1, 2);
            m2_1_m2_1.insertRehydration("cabbbbaei");
            m2_1_m2_1.insertRehydration("cabbbbaei");

            t = genusTwoTorusCusp.newTetrahedron();
            s = genusTwoTorusCusp.newTetrahedron();
            t->join(0, s, Perm<4>(0,2,3,1));
            t->join(1, s, Perm<4>(2,1,3,0));
            t->join(2, s, Perm<4>(1,3,2,0));
            t->join(3, s, Perm<4>(2,0,1,3));

            t = projPlaneCusps.newTetrahedron();
            s = projPlaneCusps.newTetrahedron();
            t->join(0, t, Perm<4>(1,0,2,3));
            t->join(2, s, Perm<4>(1,2,0,3));
            t->join(3, s, Perm<4>(3,2,0,1));
            s->join(2, s, Perm<4>(0,2,3,1));

            t = genusFourNonOrCusp.newTetrahedron();
            s = genusFourNonOrCusp.newTetrahedron();
            t->join(0, t, Perm<4>(1,2,0,3));
            t->join(2, s, Perm<4>(1,2,0,3));
            t->join(3, s, Perm<4>(0,2,3,1));
            s->join(2, s, Perm<4>(0,2,3,1));

            cuspedTorus.insertLayeredSolidTorus(1, 2);
            cuspedTorus.finiteToIdeal();

            t = edgeInvalid.newTetrahedron();
            t->join(0, t, Perm<4>(1,0,3,2));
            t->join(2, t, Perm<4>(1,0,3,2));
        }

        void tearDown() {
        }

        void testIncompatible(Triangulation<3>& tri, const char* message) {
            SnapPeaTriangulation s(tri);
            CPPUNIT_ASSERT_MESSAGE(message, s.isNull());
        }

        void incompatible() {
            // As well as verifying that SnapPea cannot work with the
            // various triangulations, run sanity checks to ensure we
            // got the gluings right.  Otherwise a typo in the setUp()
            // code could well go unnoticed.

            testIncompatible(empty, "The empty triangulation "
                "should not be representable in SnapPea format.");

            CPPUNIT_ASSERT_MESSAGE("LST(1,2,3) "
                "appears to have been incorrectly constructed.",
                lst123.isValid() && lst123.isConnected() &&
                lst123.isOrientable() && (! lst123.isIdeal()) &&
                lst123.hasBoundaryTriangles());
            testIncompatible(lst123, "LST(1,2,3) "
                "should not be representable in SnapPea format.");

            CPPUNIT_ASSERT_MESSAGE("The disconnected double M 2_1 "
                "appears to have been incorrectly constructed.",
                m2_1_m2_1.isValid() && (! m2_1_m2_1.isConnected()) &&
                m2_1_m2_1.isOrientable() && m2_1_m2_1.isIdeal() &&
                m2_1_m2_1.isStandard() && (! m2_1_m2_1.hasBoundaryTriangles()));
            testIncompatible(m2_1_m2_1, "The disconnected double M 2_1 "
                "should not be representable in SnapPea format.");

            CPPUNIT_ASSERT_MESSAGE(
                "The triangulation with a genus two torus cusp "
                "appears to have been incorrectly constructed.",
                genusTwoTorusCusp.isValid() &&
                genusTwoTorusCusp.isConnected() &&
                genusTwoTorusCusp.isOrientable() &&
                genusTwoTorusCusp.isIdeal() &&
                (! genusTwoTorusCusp.isStandard()) &&
                (! genusTwoTorusCusp.hasBoundaryTriangles()));
            testIncompatible(genusTwoTorusCusp,
                "A triangulation with a genus two torus cusp "
                "should not be representable in SnapPea format.");

            CPPUNIT_ASSERT_MESSAGE(
                "The triangulation with two projective plane cusps "
                "appears to have been incorrectly constructed.",
                projPlaneCusps.isValid() &&
                projPlaneCusps.isConnected() &&
                (! projPlaneCusps.isOrientable()) &&
                projPlaneCusps.isIdeal() &&
                (! projPlaneCusps.isStandard()) &&
                (! projPlaneCusps.hasBoundaryTriangles()));
            testIncompatible(projPlaneCusps,
                "A triangulation with two projective plane cusps "
                "should not be representable in SnapPea format.");

            CPPUNIT_ASSERT_MESSAGE(
                "The triangulation with a genus four non-orientable cusp "
                "appears to have been incorrectly constructed.",
                genusFourNonOrCusp.isValid() &&
                genusFourNonOrCusp.isConnected() &&
                (! genusFourNonOrCusp.isOrientable()) &&
                genusFourNonOrCusp.isIdeal() &&
                (! genusFourNonOrCusp.isStandard()) &&
                (! genusFourNonOrCusp.hasBoundaryTriangles()));
            testIncompatible(genusFourNonOrCusp,
                "A triangulation with a genus four non-orientable cusp "
                "should not be representable in SnapPea format.");

            CPPUNIT_ASSERT_MESSAGE(
                "The triangulation with two invalid edges "
                "appears to have been incorrectly constructed.",
                (! edgeInvalid.isValid()) &&
                edgeInvalid.isConnected() &&
                (! edgeInvalid.isOrientable()) &&
                (! edgeInvalid.isIdeal()) &&
                edgeInvalid.isStandard() &&
                (! edgeInvalid.hasBoundaryTriangles()));
            testIncompatible(edgeInvalid,
                "A triangulation with two invalid edges "
                "should not be representable in SnapPea format.");
        }

        void testVolume(Triangulation<3>& tri, double vol, unsigned places) {
            // Verify the volume to the given number of decimal places.
            // Places are counted after the decimal point in standard
            // (non-scientific) notation.
            SnapPeaTriangulation s(tri);
            {
                std::ostringstream msg;
                msg << "Triangulation " << tri.label() <<
                    " could not be represented in SnapPea format.";

                CPPUNIT_ASSERT_MESSAGE(msg.str(), ! s.isNull());
            }

            int precision;
            double foundVol = s.volume(precision);
            if (precision < static_cast<int>(places)) {
                std::ostringstream msg;
                msg << "Volume for " << tri.label() <<
                    " has a precision of " << precision
                    << " places, which is less than the desired "
                    << places << " places.";
                CPPUNIT_FAIL(msg.str());
            }

            double epsilon = 0.5;
            for (unsigned i = 0; i < places; i++)
                epsilon /= 10;

            if (foundVol > vol + epsilon || foundVol < vol - epsilon) {
                std::ostringstream msg;
                msg << "Volume for " << tri.label() << " should be "
                    << std::setprecision(
                        places + static_cast<int>(ceil(log10(vol))))
                    << vol << ", not "
                    << std::setprecision(
                        precision + static_cast<int>(ceil(log10(foundVol))))
                    << foundVol << '.';
                CPPUNIT_FAIL(msg.str());
            }
        }

        void volume() {
            testVolume(m2_1,   2.0298832128, 9);
            testVolume(m2_2,   2.0298832128, 9);
            testVolume(m3_9,   2.9441064867, 9);
            testVolume(m4_52,  4.0597664256, 9);
            testVolume(m4_1_2, 3.6638623767, 9);
            testVolume(m4_4_2, 4.0597664256, 9);

            testVolume(n1_1,     1.0149416064, 9);
            testVolume(n2_1,     1.8319311884, 9);
            testVolume(n2_1_2,   2.0298832128, 9);
            testVolume(n4_14,    3.9696478012, 9);
            testVolume(n4_9_2,   4.0597664256, 9);
            testVolume(n4_1_2_1, 3.6638623767, 9);

            testVolume(closedHypOr, 0.94270736, 7);
            testVolume(closedHypNor, 2.02988321, 7);
            testVolume(weberSeifert, 11.1990647, 6);
        }

        void testZeroVolume(const char* triName, double foundVol,
                int usePrecision) {
            // Test whether the given volume is zero to the given number
            // of decimal places.  If the number of decimal places is
            // negative, zero places will be used instead.

            if (usePrecision < 0)
                usePrecision = 0;

            // The trouble here is that we need to take a log to
            // calculate the output precision for the volume.  If the
            // volume _is_ zero however, we can't do this.  So we test
            // first, and only construct the error message if the test
            // fails.
            double epsilon = 0.5;
            for (int i = 0; i < usePrecision; i++)
                epsilon /= 10;

            if (foundVol <= epsilon && foundVol >= -epsilon)
                return;

            // FAILURE!  Build the error message and die.
            int showPrecision =
                usePrecision + static_cast<int>(ceil(log10(fabs(foundVol))));
            if (showPrecision < 3)
                showPrecision = 3;

            std::ostringstream msg;
            msg << triName << " should have a volume of zero, not "
                << std::setprecision(showPrecision) << foundVol
                << " (precision: " << usePrecision << ").";

            CPPUNIT_FAIL(msg.str());
        }

        void testFlat(Triangulation<3>& tri, const char* triName,
                unsigned places) {
            // Verify that the triangulation has a flat solution and the
            // volume is zero to the given number of decimal places.
            // Places are counted after the decimal point in standard
            // (non-scientific) notation.
            SnapPeaTriangulation s(tri);
            {
                std::ostringstream msg;
                msg << triName <<
                    " could not be represented in SnapPea format.";

                CPPUNIT_ASSERT_MESSAGE(msg.str(), ! s.isNull());
            }

            {
                std::ostringstream msg;
                msg << triName << " has a solution type that is not flat.";

                CPPUNIT_ASSERT_MESSAGE(msg.str(), s.solutionType() ==
                    SnapPeaTriangulation::flat_solution);
            }

            int precision;
            double foundVol = s.volume(precision);
            {
                std::ostringstream msg;
                msg << triName << " has a volume with a precision of "
                    << precision << " places, which is less than the desired "
                    << places << " places.";

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    precision >= static_cast<int>(places));
            }

            // Dumb down the precision to our given maximum.
            testZeroVolume(triName, foundVol, places);
        }

        void flat() {
            testFlat(flatOr, "The orientable flat triangulation", 9);
            testFlat(flatNor, "The non-orientable flat triangulation", 9);
        }

        void testDegenerate(Triangulation<3>& tri, const char* triName) {
            // Verify that the triangulation has a degenerate solution
            // and the volume is zero.  The volume is tested to whatever
            // precision is reported (up to a maximum of maxPlaces),
            // but the precision itself has no lower limit imposed -- this
            // allows flexibility for different floating point behaviours of
            // different chipsets.
            // Places are counted after the decimal point in standard
            // (non-scientific) notation.
            SnapPeaTriangulation s(tri);
            {
                std::ostringstream msg;
                msg << triName <<
                    " could not be represented in SnapPea format.";

                CPPUNIT_ASSERT_MESSAGE(msg.str(), ! s.isNull());
            }

            {
                std::ostringstream msg;
                msg << triName <<
                    " has a solution type that is not degenerate.";

                CPPUNIT_ASSERT_MESSAGE(msg.str(), s.solutionType() ==
                    SnapPeaTriangulation::degenerate_solution);
            }

            // Don't test volumes for degenerate solutions, since these
            // can go all over the shop.
        }

        void degenerate() {
            testDegenerate(degenerateOr,
                "The orientable degenerate triangulation");
            testDegenerate(degenerateNor,
                "The non-orientable degenerate triangulation");
            testDegenerate(cuspedTorus,
                "A cusped solid torus with an additional finite vertex");
        }

        void spunBoundaries() {
            Triangulation<3>* f8 =
                Example<3>::figureEight();
            SnapPeaTriangulation* t = new SnapPeaTriangulation(*f8);

            regina::NormalSurfaces* s =
                regina::NormalSurfaces::enumerate(t, regina::NS_QUAD);
            if (s->size() != 4)
                CPPUNIT_FAIL(
                    "The figure 8 knot complement should have 4 vertex "
                    "surfaces in quad space.");

            regina::MatrixInt* m;
            bool found[4];
            for (int i = 0; i < s->size(); ++i) {
                m = s->surface(i)->boundaryIntersections();
                if (m->rows() != 1 || m->columns() != 2) {
                    CPPUNIT_FAIL(
                        "Figure 8 knot complement: boundaryIntersections() "
                        "should give 1x2 matrices.");
                }
                if (m->entry(0, 0) == 1 && m->entry(0, 1) == 4)
                    found[0] = true;
                else if (m->entry(0, 0) == 1 && m->entry(0, 1) == -4)
                    found[1] = true;
                else if (m->entry(0, 0) == -1 && m->entry(0, 1) == 4)
                    found[2] = true;
                else if (m->entry(0, 0) == -1 && m->entry(0, 1) == -4)
                    found[3] = true;
                else {
                    std::ostringstream msg;
                    msg << "Figure 8 knot complement: boundaryIntersections() "
                        "gives unexpected result ("
                        << m->entry(0, 0) << ", " << m->entry(0, 1) << ").";
                    CPPUNIT_FAIL(msg.str());
                }
                delete m;
            }

            if (! found[0])
                CPPUNIT_FAIL("Figure 8 knot complement: did not find "
                    "boundary slope (1, 4).");
            if (! found[1])
                CPPUNIT_FAIL("Figure 8 knot complement: did not find "
                    "boundary slope (1, -4).");
            if (! found[2])
                CPPUNIT_FAIL("Figure 8 knot complement: did not find "
                    "boundary slope (-1, 4).");
            if (! found[3])
                CPPUNIT_FAIL("Figure 8 knot complement: did not find "
                    "boundary slope (-1, -4).");

            delete s;
            delete t;
            delete f8;
        }

        static void testStability(Triangulation<3>* tri) {
            // Just make sure SnapPea can work with the triangulation
            // without crashing.
            SnapPeaTriangulation s(*tri);
            s.volume();
            s.randomize();
            s.volume();
            Triangulation<3> t(s);
        }

        void stability() {
            runCensusAllNoBdry(&testStability);
        }

        void testFilledHomology(const Triangulation<3>& tri, int m, int l,
                const std::string& expectedH1) {
            SnapPeaTriangulation s(tri);
            if (s.isNull()) {
                std::ostringstream msg;
                msg << "Null SnapPea triangulation for " << tri.label() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            s.fill(m, l);

            const regina::AbelianGroup* h1 = s.homologyFilled();
            if (! h1) {
                std::ostringstream msg;
                msg << "Could not compute filled homology for "
                    << tri.label() << ".";
                CPPUNIT_FAIL(msg.str());
            }
            if (h1->str() != expectedH1) {
                std::ostringstream msg;
                msg << "Filling (" << m << ", " << l << ") for "
                    << tri.label() << " gives homology "
                    << h1->str() << ", not " << expectedH1 << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void filling() {
            // For now, just a couple of tests to ensure that SnapPea's filled
            // homology routines appear to be functioning correctly.

            // 1 boundary component, orientable:
            testFilledHomology(m2_1, 0, 0, "Z");
            testFilledHomology(m2_1, 1, 1, "0");
            testFilledHomology(m2_1, -3, 7, "Z_3");

            // 2 boundary components, orientable:
            testFilledHomology(m4_4_2, 0, 0, "2 Z");
            testFilledHomology(m4_4_2, 1, 1, "Z");
            testFilledHomology(m4_4_2, -3, 7, "Z + Z_3");

            // 1 boundary component, non-orientable:
            testFilledHomology(n1_1, 0, 0, "Z");
            testFilledHomology(n1_1, 1, 0, "Z");
            testFilledHomology(n1_1, -1, 0, "Z");

            // 2 boundary components, non-orientable:
            testFilledHomology(n4_9_2, 0, 0, "Z + Z_2");
            testFilledHomology(n4_9_2, 1, 0, "Z");
            testFilledHomology(n4_9_2, -1, 0, "Z");
        }
};

void addSnapPeaTriangulation(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(SnapPeaTriangulationTest::suite());
}

