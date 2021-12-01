
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
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

#include <cmath>
#include <iomanip>
#include <cppunit/extensions/HelperMacros.h>
#include "link/examplelink.h"
#include "link/link.h"
#include "maths/matrix.h"
#include "snappea/examplesnappea.h"
#include "snappea/snappeatriangulation.h"
#include "surfaces/normalsurfaces.h"
#include "triangulation/example3.h"
#include "triangulation/dim3.h"

#include "testsuite/exhaustive.h"
#include "testsuite/snappea/testsnappea.h"

using regina::Example;
using regina::ExampleSnapPea;
using regina::Perm;
using regina::SnapPeaTriangulation;
using regina::Tetrahedron;
using regina::Triangulation;

class SnapPeaTriangulationTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(SnapPeaTriangulationTest);

    CPPUNIT_TEST(copyMove);
    CPPUNIT_TEST(incompatible);
    CPPUNIT_TEST(volume);
    CPPUNIT_TEST(flat);
    CPPUNIT_TEST(degenerate);
    CPPUNIT_TEST(spunBoundaries);
    CPPUNIT_TEST(stability);
    CPPUNIT_TEST(filling);
    CPPUNIT_TEST(link);
    CPPUNIT_TEST(swapping);

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
        void setUp() override {
            // Keep the kernel quiet.  It interferes with the test
            // suite's running progress messages.
            SnapPeaTriangulation::disableKernelMessages();

            m2_1 = Triangulation<3>::rehydrate("cabbbbaei");
            m2_2 = Triangulation<3>::rehydrate("cabbbbapt");
            m3_9 = Triangulation<3>::rehydrate("dagacccfwkn");
            m4_52 = Triangulation<3>::rehydrate("ebdbcdddaqhie");
            m4_1_2 = Triangulation<3>::rehydrate("eahbcdddhsssj");
            m4_4_2 = Triangulation<3>::rehydrate("ebdbcdddddddx");

            n1_1 = Triangulation<3>::rehydrate("baaaade");
            n2_1 = Triangulation<3>::rehydrate("cabbbbabw");
            n2_1_2 = Triangulation<3>::rehydrate("cabbbbcdw");
            n4_14 = Triangulation<3>::rehydrate("eahdccddakfhq");
            n4_9_2 = Triangulation<3>::rehydrate("ebdbcdddcemre");
            n4_1_2_1 = Triangulation<3>::rehydrate("eahbcdddjxxxj");

            // Note: the non-orientable manifold below is the same as
            // Example<3>::smallClosedNonOrblHyperbolic()),
            // but if we build it from Example<3> then we
            // seem to get a degenerate solution.  Using the isosig
            // gives the same triangulation with a different labelling,
            // which seems to prod SnapPea into finding a better
            // (non_geometric) solution instead.
            closedHypOr = Example<3>::smallClosedOrblHyperbolic();
            closedHypNor = Triangulation<3>::fromIsoSig(
                "lLLLALAQccegffiijkikkkknawmhvwcls");
            weberSeifert = Example<3>::weberSeifert();

            {
                auto [t, s] = flatOr.newTetrahedra<2>();
                t->join(0, s, Perm<4>(0,1,2,3));
                t->join(1, s, Perm<4>(0,1,2,3));
                t->join(2, s, Perm<4>(1,3,2,0));
                t->join(3, s, Perm<4>(1,2,0,3));
            }
            {
                auto [t, s] = flatNor.newTetrahedra<2>();
                t->join(0, s, Perm<4>(0,1,2,3));
                t->join(1, s, Perm<4>(2,1,0,3));
                t->join(2, s, Perm<4>(1,3,2,0));
                t->join(3, s, Perm<4>(2,1,0,3));
            }
            {
                auto [t, s] = degenerateOr.newTetrahedra<2>();
                t->join(0, t, Perm<4>(1,0,2,3));
                t->join(2, s, Perm<4>(1,2,0,3));
                t->join(3, s, Perm<4>(0,2,3,1));
                s->join(2, s, Perm<4>(1,2,3,0));
            }
            {
                auto [t, s] = degenerateNor.newTetrahedra<2>();
                t->join(0, t, Perm<4>(1,0,2,3));
                t->join(2, s, Perm<4>(1,2,0,3));
                t->join(3, s, Perm<4>(0,3,2,1));
                s->join(2, s, Perm<4>(0,2,3,1));
            }

            lst123.insertLayeredSolidTorus(1, 2);

            m2_1_m2_1 = Triangulation<3>::rehydrate("cabbbbaei");
            m2_1_m2_1.insertTriangulation(m2_1_m2_1);

            {
                auto [t, s] = genusTwoTorusCusp.newTetrahedra<2>();
                t->join(0, s, Perm<4>(0,2,3,1));
                t->join(1, s, Perm<4>(2,1,3,0));
                t->join(2, s, Perm<4>(1,3,2,0));
                t->join(3, s, Perm<4>(2,0,1,3));
            }
            {
                auto [t, s] = projPlaneCusps.newTetrahedra<2>();
                t->join(0, t, Perm<4>(1,0,2,3));
                t->join(2, s, Perm<4>(1,2,0,3));
                t->join(3, s, Perm<4>(3,2,0,1));
                s->join(2, s, Perm<4>(0,2,3,1));
            }
            {
                auto [t, s] = genusFourNonOrCusp.newTetrahedra<2>();
                t->join(0, t, Perm<4>(1,2,0,3));
                t->join(2, s, Perm<4>(1,2,0,3));
                t->join(3, s, Perm<4>(0,2,3,1));
                s->join(2, s, Perm<4>(0,2,3,1));
            }

            cuspedTorus.insertLayeredSolidTorus(1, 2);
            cuspedTorus.finiteToIdeal();

            {
                auto t = edgeInvalid.newTetrahedron();
                t->join(0, t, Perm<4>(1,0,3,2));
                t->join(2, t, Perm<4>(1,0,3,2));
            }
        }

        void tearDown() override {
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
            if (! a.isIdenticalTo(b))
                return false;

            if (a.isoSig() != b.isoSig())
                return false;
            if (a.snapPea() != b.snapPea())
                return false;

            return true;
        }

        static void verifyCopyMoveNull(const SnapPeaTriangulation& t,
                const char* name) {
            SnapPeaTriangulation copy(t);
            if (! copy.isNull()) {
                std::ostringstream msg;
                msg << name << ": copy constructed not null.";
                CPPUNIT_FAIL(msg.str());
            }

            SnapPeaTriangulation move(std::move(copy));
            if (! move.isNull()) {
                std::ostringstream msg;
                msg << name << ": move constructed not null.";
                CPPUNIT_FAIL(msg.str());
            }

            SnapPeaTriangulation figureEight = ExampleSnapPea::figureEight();

            SnapPeaTriangulation copyAss(figureEight);
            copyAss = t;
            if (! copyAss.isNull()) {
                std::ostringstream msg;
                msg << name << ": copy assigned not null.";
                CPPUNIT_FAIL(msg.str());
            }

            SnapPeaTriangulation moveAss(figureEight);
            moveAss = std::move(copyAss);
            if (! moveAss.isNull()) {
                std::ostringstream msg;
                msg << name << ": move assigned not null.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        static void verifyCopyMove(const SnapPeaTriangulation& t,
                const char* name) {
            if (t.isNull()) {
                verifyCopyMoveNull(t, name);
                return;
            }

            if (t.countCusps() == 0) {
                std::ostringstream msg;
                msg << name << ": not null but no cusps.";
                CPPUNIT_FAIL(msg.str());
            }

            if (t.countVertices() == 0) {
                std::ostringstream msg;
                msg << name << ": not null but no vertices.";
                CPPUNIT_FAIL(msg.str());
            }

            const regina::Cusp* c0 = std::addressof(t.cusp(0));
            regina::Vertex<3>* v0 = t.vertex(0);

            SnapPeaTriangulation copy(t);
            if (! looksIdentical(copy, t)) {
                std::ostringstream msg;
                msg << name << ": copy constructed not identical to original.";
                CPPUNIT_FAIL(msg.str());
            }

            const regina::Cusp* c1 = (copy.countCusps() == 0 ? nullptr :
                std::addressof(copy.cusp(0)));
            regina::Vertex<3>* v1 = (copy.countVertices() == 0 ? nullptr :
                copy.vertex(0));
            if (c1 == c0) {
                std::ostringstream msg;
                msg << name << ": copy constructed uses the same cusps.";
                CPPUNIT_FAIL(msg.str());
            }
            if (v1 == v0) {
                std::ostringstream msg;
                msg << name << ": copy constructed uses the same vertices.";
                CPPUNIT_FAIL(msg.str());
            }

            SnapPeaTriangulation move(std::move(copy));
            if (! looksIdentical(move, t)) {
                std::ostringstream msg;
                msg << name << ": move constructed not identical to original.";
                CPPUNIT_FAIL(msg.str());
            }

            const regina::Cusp* c2 = (move.countCusps() == 0 ? nullptr :
                std::addressof(move.cusp(0)));
            regina::Vertex<3>* v2 = (move.countVertices() == 0 ? nullptr :
                move.vertex(0));
            if (c2 != c1) {
                std::ostringstream msg;
                msg << name << ": move constructed does not use the "
                    "same cusps.";
                CPPUNIT_FAIL(msg.str());
            }
            if (v2 != v1) {
                std::ostringstream msg;
                msg << name << ": move constructed does not use the "
                    "same vertices.";
                CPPUNIT_FAIL(msg.str());
            }

            SnapPeaTriangulation copyAss;
            copyAss.newSimplex(); // Give it something to overwrite.
            copyAss = t;
            if (! looksIdentical(copyAss, t)) {
                std::ostringstream msg;
                msg << name << ": copy assigned not identical to original.";
                CPPUNIT_FAIL(msg.str());
            }

            const regina::Cusp* c3 = (copyAss.countCusps() == 0 ? nullptr :
                std::addressof(copyAss.cusp(0)));
            regina::Vertex<3>* v3 = (copyAss.countVertices() == 0 ? nullptr :
                copyAss.vertex(0));
            if (c3 == c0) {
                std::ostringstream msg;
                msg << name << ": copy assigned uses the same cusps.";
                CPPUNIT_FAIL(msg.str());
            }
            if (v3 == v0) {
                std::ostringstream msg;
                msg << name << ": copy assigned uses the same vertices.";
                CPPUNIT_FAIL(msg.str());
            }

            SnapPeaTriangulation moveAss;
            moveAss.newSimplex(); // Give it something to overwrite.
            moveAss = std::move(copyAss);
            if (! looksIdentical(moveAss, t)) {
                std::ostringstream msg;
                msg << name << ": move assigned not identical to original.";
                CPPUNIT_FAIL(msg.str());
            }

            const regina::Cusp* c4 = (moveAss.countCusps() == 0 ? nullptr :
                std::addressof(moveAss.cusp(0)));
            regina::Vertex<3>* v4 = (moveAss.countVertices() == 0 ? nullptr :
                moveAss.vertex(0));
            if (c4 != c3) {
                std::ostringstream msg;
                msg << name << ": move assigned does not use the "
                    "same cusps.";
                CPPUNIT_FAIL(msg.str());
            }
            if (v4 != v3) {
                std::ostringstream msg;
                msg << name << ": move assigned does not use the "
                    "same vertices.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void copyMove() {
            verifyCopyMove(m2_1, "M 2_1");
            verifyCopyMove(m2_2, "M 2_2");
            verifyCopyMove(m3_9, "M 3_9");
            verifyCopyMove(m4_52, "M 4_52");
            verifyCopyMove(m4_1_2, "M 4_1^2");
            verifyCopyMove(m4_4_2, "M 4_4^2");

            verifyCopyMove(n1_1, "N 1_1");
            verifyCopyMove(n2_1, "N 2_1");
            verifyCopyMove(n2_1_2, "N 2_1^2");
            verifyCopyMove(n4_14, "N 4_14");
            verifyCopyMove(n4_9_2, "N 4_9^2");
            verifyCopyMove(n4_1_2_1, "N 4_1^2,1");

            verifyCopyMove(closedHypOr, "or_0.94270736");
            verifyCopyMove(closedHypNor, "nor_2.02988321");
            verifyCopyMove(weberSeifert, "Weber-Seifert");

            verifyCopyMove(flatOr, "Flat orientable");
            verifyCopyMove(flatNor, "Flat non-orientable");
            verifyCopyMove(degenerateOr, "Degenerate orientable");
            verifyCopyMove(degenerateNor, "Degenerate non-orientable");

            verifyCopyMove(empty, "Empty");
            verifyCopyMove(lst123, "LST(1,2,3)");
            verifyCopyMove(m2_1_m2_1, "M 2_1 U M 2_1");
            verifyCopyMove(genusTwoTorusCusp, "Genus two torus cusp");
            verifyCopyMove(projPlaneCusps, "Two projective plane cusps");
            verifyCopyMove(genusFourNonOrCusp,
                "Genus four non-orientable cusp");
            verifyCopyMove(cuspedTorus, "Cusped solid torus");
            verifyCopyMove(edgeInvalid, "Two invalid edges");
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

        void testVolume(const SnapPeaTriangulation& s,
                double vol, unsigned places, const char* name) {
            // Verify the volume to the given number of decimal places.
            // Places are counted after the decimal point in standard
            // (non-scientific) notation.
            if (s.isNull()) {
                std::ostringstream msg;
                msg << "Triangulation " << name <<
                    " could not be represented in SnapPea format.";
                CPPUNIT_FAIL(msg.str());
            }

            auto [foundVol, precision] = s.volumeWithPrecision();
            if (precision < static_cast<int>(places)) {
                std::ostringstream msg;
                msg << "Volume for " << name << " has a precision of "
                    << precision << " places, which is less than the desired "
                    << places << " places.";
                CPPUNIT_FAIL(msg.str());
            }

            double epsilon = 0.5;
            for (unsigned i = 0; i < places; i++)
                epsilon /= 10;

            if (foundVol > vol + epsilon || foundVol < vol - epsilon) {
                std::ostringstream msg;
                msg << "Volume for " << name << " should be "
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

        void testFlat(const SnapPeaTriangulation& s, const char* triName,
                unsigned places) {
            // Verify that the triangulation has a flat solution and the
            // volume is zero to the given number of decimal places.
            // Places are counted after the decimal point in standard
            // (non-scientific) notation.

            if (s.isNull()) {
                std::ostringstream msg;
                msg << "Triangulation " << triName <<
                    " could not be represented in SnapPea format.";
                CPPUNIT_FAIL(msg.str());
            }

            if (s.solutionType() != SnapPeaTriangulation::flat_solution) {
                std::ostringstream msg;
                msg << triName << " has a solution type that is not flat.";
                CPPUNIT_FAIL(msg.str());
            }

            auto [foundVol, precision] = s.volumeWithPrecision();
            if (precision < static_cast<int>(places)) {
                std::ostringstream msg;
                msg << triName << " has a volume with a precision of "
                    << precision << " places, which is less than the desired "
                    << places << " places.";
                CPPUNIT_FAIL(msg.str());
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
            SnapPeaTriangulation t(Example<3>::figureEight());

            regina::NormalSurfaces s(t, regina::NS_QUAD);
            if (s.size() != 4)
                CPPUNIT_FAIL(
                    "The figure 8 knot complement should have 4 vertex "
                    "surfaces in quad space.");

            bool found[4] = {}; // initialises to false
            for (const regina::NormalSurface& f : s) {
                regina::MatrixInt m = f.boundaryIntersections();
                if (m.rows() != 1 || m.columns() != 2) {
                    CPPUNIT_FAIL(
                        "Figure 8 knot complement: boundaryIntersections() "
                        "should give 1x2 matrices.");
                }
                if (m.entry(0, 0) == 1 && m.entry(0, 1) == 4)
                    found[0] = true;
                else if (m.entry(0, 0) == 1 && m.entry(0, 1) == -4)
                    found[1] = true;
                else if (m.entry(0, 0) == -1 && m.entry(0, 1) == 4)
                    found[2] = true;
                else if (m.entry(0, 0) == -1 && m.entry(0, 1) == -4)
                    found[3] = true;
                else {
                    std::ostringstream msg;
                    msg << "Figure 8 knot complement: boundaryIntersections() "
                        "gives unexpected result ("
                        << m.entry(0, 0) << ", " << m.entry(0, 1) << ").";
                    CPPUNIT_FAIL(msg.str());
                }
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
        }

        static void testStability(const Triangulation<3>& tri, const char*) {
            // Just make sure SnapPea can work with the triangulation
            // without crashing.
            try {
                SnapPeaTriangulation s(tri);
                s.volume();
                s.randomize();
                s.volume();
                Triangulation<3> t(s);
            } catch (const regina::SnapPeaIsNull&) {
            }
        }

        void stability() {
            runCensusAllNoBdry(&testStability);
        }

        void testFilledHomology(SnapPeaTriangulation s, int m, int l,
                const std::string& expectedH1, const char* name) {
            // We pass s by value because we are going to edit it here.

            if (s.isNull()) {
                std::ostringstream msg;
                msg << "Null SnapPea triangulation for " << name << ".";
                CPPUNIT_FAIL(msg.str());
            }

            s.fill(m, l);

            const regina::AbelianGroup& h1 = s.homologyFilled();
            if (h1.str() != expectedH1) {
                std::ostringstream msg;
                msg << "Filling (" << m << ", " << l << ") for "
                    << name << " gives homology "
                    << h1.str() << ", not " << expectedH1 << ".";
                CPPUNIT_FAIL(msg.str());
            }

            const regina::GroupPresentation& fg = s.fundamentalGroupFilled();
            if (fg.abelianisation().str() != expectedH1) {
                std::ostringstream msg;
                msg << "Filling (" << m << ", " << l << ") for "
                    << name << " gives fundamental group " << fg.str()
                    << ", which does not abelianise to " << expectedH1 << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (m == 0 && l == 0) {
                try {
                    Triangulation<3> t = s.filledAll();

                    std::ostringstream msg;
                    msg << "Calling filledAll() on " << name
                        << " with no cusps filled "
                        "should have thrown an exception.";
                    CPPUNIT_FAIL(msg.str());
                } catch (const regina::FailedPrecondition&) {
                }

                SnapPeaTriangulation t = s.filledPartial();
                regina::AbelianGroup reg = t.homology();
                if (reg.str() != expectedH1) {
                    std::ostringstream msg;
                    msg << "Permanent filling (" << m << ", " << l << ") for "
                        << name << " gives homology "
                        << reg.str() << ", not " << expectedH1 << ".";
                    CPPUNIT_FAIL(msg.str());
                }

                const regina::AbelianGroup& snap = s.homologyFilled();
                if (snap.str() != expectedH1) {
                    std::ostringstream msg;
                    msg << "Permanent filling (" << m << ", " << l << ") for "
                        << name << " gives homology "
                        << snap.str() << ", not " << expectedH1 << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            } else if (s.countBoundaryComponents() == 1) {
                try {
                    SnapPeaTriangulation t = s.filledPartial();

                    std::ostringstream msg;
                    msg << "Calling filledPartial() on " << name
                        << " with the one cusp filled should have "
                        "thrown an exception.";
                    CPPUNIT_FAIL(msg.str());
                } catch (const regina::FailedPrecondition&) {
                }

                Triangulation<3> t = s.filledAll();
                regina::AbelianGroup ans = t.homology();
                if (ans.str() != expectedH1) {
                    std::ostringstream msg;
                    msg << "Permanent filling (" << m << ", " << l << ") for "
                        << name << " gives homology "
                        << ans.str() << ", not " << expectedH1 << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            } else {
                try {
                    Triangulation<3> t = s.filledAll();

                    std::ostringstream msg;
                    msg << "Calling filledAll() on " << name
                        << " with only one cusped filled "
                        "should have thrown an exception.";
                    CPPUNIT_FAIL(msg.str());
                } catch (const regina::FailedPrecondition&) {
                }

                SnapPeaTriangulation t = s.filledPartial();
                regina::AbelianGroup reg = t.homology();
                if (reg.str() != expectedH1) {
                    std::ostringstream msg;
                    msg << "Permanent filling (" << m << ", " << l << ") for "
                        << name << " gives homology "
                        << reg.str() << ", not " << expectedH1 << ".";
                    CPPUNIT_FAIL(msg.str());
                }

                const regina::AbelianGroup& snap = s.homologyFilled();
                if (snap.str() != expectedH1) {
                    std::ostringstream msg;
                    msg << "Permanent filling (" << m << ", " << l << ") for "
                        << name << " gives homology "
                        << snap.str() << ", not " << expectedH1 << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void filling() {
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

        void link() {
            testVolume(regina::ExampleLink::figureEight(),
                2.02988321282, 9, "Figure eight");
            testFilledHomology(regina::ExampleLink::figureEight(),
                1, 1, "0", "Figure eight");
            testFilledHomology(regina::ExampleLink::figureEight(),
                -3, 7, "Z_3", "Figure eight");

            testFlat(regina::ExampleLink::trefoil(), "Trefoil", 9);
            testFilledHomology(regina::ExampleLink::trefoil(),
                1, 1, "0", "Trefoil");
            testFilledHomology(regina::ExampleLink::trefoil(),
                -3, 7, "Z_3", "Trefoil");

            testVolume(regina::ExampleLink::whitehead(),
                3.66386237671, 9, "Whitehead link");
            testFilledHomology(regina::ExampleLink::whitehead(),
                1, 1, "Z", "Whitehead link");
            testFilledHomology(regina::ExampleLink::whitehead(),
                -3, 7, "Z + Z_3", "Whitehead link");
        }

        void swapping() {
            {
                SnapPeaTriangulation a = ExampleSnapPea::figureEight();
                SnapPeaTriangulation b = ExampleSnapPea::whiteheadLink();

                a.volume();
                b.volume();

                swap(a, b);

                if (a.isNull() || b.isNull()) {
                    CPPUNIT_FAIL("swap() nullified the triangulation(s).");
                }

                if (a.countCusps() != 2) {
                    CPPUNIT_FAIL("swap() did not swap cusps correctly.");
                }
                if (std::floor(a.volume()) != 3) {
                    CPPUNIT_FAIL("swap() did not swap snappea data correctly.");
                }

                std::iter_swap(&a, &b);

                if (a.countCusps() != 1) {
                    CPPUNIT_FAIL(
                        "std::iter_swap() did not swap cusps correctly.");
                }
                if (std::floor(a.volume()) != 2) {
                    CPPUNIT_FAIL(
                        "std::iter_swap() did not swap snappea data correctly.");
                }
            }
            {
                SnapPeaTriangulation a = ExampleSnapPea::figureEight();
                SnapPeaTriangulation b = ExampleSnapPea::whiteheadLink();

                a.volume();
                b.volume();

                swap(static_cast<Triangulation<3>&>(a), b);

                if (! (a.isNull() && b.isNull())) {
                    CPPUNIT_FAIL("swap() did not nullify triangulation(s) "
                        "where required.");
                }
            }
            {
                SnapPeaTriangulation a = ExampleSnapPea::figureEight();
                SnapPeaTriangulation b = ExampleSnapPea::whiteheadLink();

                a.volume();
                b.volume();

                swap(a, static_cast<Triangulation<3>&>(b));

                if (! (a.isNull() && b.isNull())) {
                    CPPUNIT_FAIL("swap() did not nullify triangulation(s) "
                        "where required.");
                }
            }
            {
                SnapPeaTriangulation a = ExampleSnapPea::figureEight();
                SnapPeaTriangulation b = ExampleSnapPea::whiteheadLink();

                a.volume();
                b.volume();

                swap(static_cast<Triangulation<3>&>(a),
                    static_cast<Triangulation<3>&>(b));

                if (! (a.isNull() && b.isNull())) {
                    CPPUNIT_FAIL("swap() did not nullify triangulation(s) "
                        "where required.");
                }
            }
        }
};

void addSnapPeaTriangulation(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(SnapPeaTriangulationTest::suite());
}

