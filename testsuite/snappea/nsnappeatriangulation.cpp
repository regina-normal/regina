
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2004, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include <cmath>
#include <iomanip>
#include <cppunit/extensions/HelperMacros.h>
#include "snappea/nsnappeatriangulation.h"
#include "triangulation/ntriangulation.h"
#include "testsuite/snappea/testsnappea.h"

using regina::NPerm;
using regina::NSnapPeaTriangulation;
using regina::NTetrahedron;
using regina::NTriangulation;

// Some of the larger triangulations we will hard-code here.
int closedHypOrAdjTet[9][4] = {
    {6, 8, 2, 8},
    {6, 8, 3, 7},
    {7, 0, 3, 4},
    {1, 5, 5, 2},
    {2, 6, 5, 7},
    {3, 8, 3, 4},
    {0, 4, 7, 1},
    {1, 4, 2, 6},
    {1, 0, 5, 0}
};

int closedHypOrAdjPerm[9][4][4] = {
    { {0,1,3,2}, {3,1,2,0}, {0,2,1,3}, {0,2,1,3} },
    { {3,1,2,0}, {1,0,2,3}, {3,2,0,1}, {2,3,1,0} },
    { {2,0,3,1}, {0,2,1,3}, {0,1,3,2}, {3,1,2,0} },
    { {2,3,1,0}, {3,2,0,1}, {2,1,0,3}, {0,1,3,2} },
    { {3,1,2,0}, {0,1,3,2}, {0,1,3,2}, {3,2,0,1} },
    { {2,1,0,3}, {0,2,1,3}, {2,3,1,0}, {0,1,3,2} },
    { {0,1,3,2}, {0,1,3,2}, {0,1,3,2}, {3,1,2,0} },
    { {3,2,0,1}, {2,3,1,0}, {1,3,0,2}, {0,1,3,2} },
    { {1,0,2,3}, {3,1,2,0}, {0,2,1,3}, {0,2,1,3} }
};

int closedHypNorAdjTet[11][4] = {
    {8, 2, 8, 2},
    {5, 3, 2, 9},
    {1, 4, 0, 0},
    {6, 1, 4, 6},
    {10, 2, 10, 3},
    {7, 7, 6, 1},
    {8, 3, 3, 5},
    {5, 9, 8, 5},
    {0, 0, 6, 7},
    {10, 10, 1, 7},
    {9, 4, 4, 9}
};

int closedHypNorAdjPerm[11][4][4] = {
    { {1,3,2,0}, {0,3,2,1}, {2,1,0,3}, {3,1,0,2} },
    { {3,0,1,2}, {3,1,0,2}, {2,1,0,3}, {1,0,3,2} },
    { {2,1,0,3}, {3,1,2,0}, {2,1,3,0}, {0,3,2,1} },
    { {2,1,3,0}, {2,1,3,0}, {2,0,3,1}, {0,3,2,1} },
    { {2,1,0,3}, {3,1,2,0}, {3,2,1,0}, {1,3,0,2} },
    { {3,1,2,0}, {1,0,3,2}, {0,1,3,2}, {1,2,3,0} },
    { {2,1,0,3}, {0,3,2,1}, {3,1,0,2}, {0,1,3,2} },
    { {1,0,3,2}, {0,3,2,1}, {0,1,3,2}, {3,1,2,0} },
    { {2,1,0,3}, {3,0,2,1}, {2,1,0,3}, {0,1,3,2} },
    { {3,1,2,0}, {2,0,1,3}, {1,0,3,2}, {0,3,2,1} },
    { {1,2,0,3}, {3,2,1,0}, {2,1,0,3}, {3,1,2,0} }
};

class NSnapPeaTriangulationTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NSnapPeaTriangulationTest);

    CPPUNIT_TEST(incompatible);
    CPPUNIT_TEST(volume);

    CPPUNIT_TEST_SUITE_END();

    private:
        /**
         * Triangulations from "A Census of Cusped Hyperbolic 3-Manifolds",
         * Callahan, Hildebrand and Weeks, Mathematics of Computation 68/225,
         * 1999, pp 321--332.
         */

        // Orientable 3-manifolds:
        NTriangulation m2_1, m2_2, m3_9, m4_52, m4_1_2, m4_4_2;

        // Non-orientable 3-manifolds:
        NTriangulation n1_1, n2_1, n2_1_2, n4_14, n4_9_2, n4_1_2_1;

        /**
         * Triangulations from "Symmetries, isometries and length
         * spectra of closed hyperbolic three-manifolds",
         * Craig D. Hodgson and Jeffrey R. Weeks,
         * Experiment. Math. 3/4, 1994, pp 261--274.
         *
         * Note that for the time being these triangulations will be
         * rejected by NSnapPeaTriangulation, since most SnapPea
         * functions require an ideal triangulation.
         */
        NTriangulation closedHypOr, closedHypNor;

        /**
         * Triangulations of 3-manifolds whose reported volume should be zero.
         */
        // TODO

        /**
         * Triangulations that SnapPea should refuse to deal with.
         */
        NTriangulation empty;
            /**< An empty triangulation. */
        NTriangulation lst123;
            /**< A (1,2,3) layered solid torus. */
        NTriangulation m2_1_m2_1;
            /**< Two disconnected copies of m2_1. */
        NTriangulation genusTwoTorusCusp;
            /**< A orientable triangulation with a genus two torus cusp. */
        NTriangulation projPlaneCusps;
            /**< A non-orientable triangulation with two projective plane
                 cusps. */
        NTriangulation genusFourNonOrCusp;
            /**< A triangulation with a genus four non-orientable cusp
             * (i.e., a non-orientable analogue of the two-holed torus). */
        NTriangulation cuspedTorus;
            /**< A solid torus with a cusped boundary and a finite vertex. */
        NTriangulation edgeInvalid;
            /**< A triangulation with two invalid edges but whose
                 vertices all have 2-sphere links. */

    public:
        void setUp() {
            // Keep the kernel quiet.  It interferes with the test
            // suite's running progress messages.
            NSnapPeaTriangulation::disableKernelMessages();

            NTetrahedron* t;
            NTetrahedron* s;

            m2_1.insertRehydration("cabbbbaei");
            m2_2.insertRehydration("cabbbbapt");
            m3_9.insertRehydration("dagacccfwkn");
            m4_52.insertRehydration("ebdbcdddaqhie");
            m4_1_2.insertRehydration("eahbcdddhsssj");
            m4_4_2.insertRehydration("ebdbcdddddddx");

            n1_1.insertRehydration("baaaade");
            n2_1.insertRehydration("cabbbbabw");
            n2_1_2.insertRehydration("cabbbbcdw");
            n4_14.insertRehydration("eahdccddakfhq");
            n4_9_2.insertRehydration("ebdbcdddcemre");
            n4_1_2_1.insertRehydration("eahbcdddjxxxj");

            cuspedTorus.insertLayeredSolidTorus(1, 2);
            cuspedTorus.cuspBoundary();

            lst123.insertLayeredSolidTorus(1, 2);
            m2_1_m2_1.insertRehydration("cabbbbaei");
            m2_1_m2_1.insertRehydration("cabbbbaei");

            t = new NTetrahedron();
            s = new NTetrahedron();
            t->joinTo(0, s, NPerm(0,2,3,1));
            t->joinTo(1, s, NPerm(2,1,3,0));
            t->joinTo(2, s, NPerm(1,3,2,0));
            t->joinTo(3, s, NPerm(2,0,1,3));
            genusTwoTorusCusp.addTetrahedron(t);
            genusTwoTorusCusp.addTetrahedron(s);

            t = new NTetrahedron();
            s = new NTetrahedron();
            t->joinTo(0, t, NPerm(1,0,2,3));
            t->joinTo(2, s, NPerm(1,2,0,3));
            t->joinTo(3, s, NPerm(3,2,0,1));
            s->joinTo(2, s, NPerm(0,2,3,1));
            projPlaneCusps.addTetrahedron(t);
            projPlaneCusps.addTetrahedron(s);

            t = new NTetrahedron();
            s = new NTetrahedron();
            t->joinTo(0, t, NPerm(1,2,0,3));
            t->joinTo(2, s, NPerm(1,2,0,3));
            t->joinTo(3, s, NPerm(0,2,3,1));
            s->joinTo(2, s, NPerm(0,2,3,1));
            genusFourNonOrCusp.addTetrahedron(t);
            genusFourNonOrCusp.addTetrahedron(s);

            t = new NTetrahedron();
            t->joinTo(0, t, NPerm(1,0,3,2));
            t->joinTo(2, t, NPerm(1,0,3,2));
            edgeInvalid.addTetrahedron(t);

            NTetrahedron** tet;
            NPerm p;
            int i, j;

            tet = new NTetrahedron*[9];
            for (i = 0; i < 9; i++)
                tet[i] = new NTetrahedron();
            for (i = 0; i < 9; i++)
                for (j = 0; j < 4; j++)
                    if (! tet[i]->getAdjacentTetrahedron(j)) {
                        p = NPerm(closedHypOrAdjPerm[i][j][0],
                             closedHypOrAdjPerm[i][j][1],
                             closedHypOrAdjPerm[i][j][2],
                             closedHypOrAdjPerm[i][j][3]);
                        tet[i]->joinTo(j, tet[closedHypOrAdjTet[i][j]], p);
                    }
            for (i = 0; i < 9; i++)
                closedHypOr.addTetrahedron(tet[i]);
            delete[] tet;

            tet = new NTetrahedron*[11];
            for (i = 0; i < 11; i++)
                tet[i] = new NTetrahedron();
            for (i = 0; i < 11; i++)
                for (j = 0; j < 4; j++)
                    if (! tet[i]->getAdjacentTetrahedron(j)) {
                        p = NPerm(closedHypNorAdjPerm[i][j][0],
                             closedHypNorAdjPerm[i][j][1],
                             closedHypNorAdjPerm[i][j][2],
                             closedHypNorAdjPerm[i][j][3]);
                        tet[i]->joinTo(j, tet[closedHypNorAdjTet[i][j]], p);
                    }
            for (i = 0; i < 11; i++)
                closedHypNor.addTetrahedron(tet[i]);
            delete[] tet;
        }

        void tearDown() {
        }

        void testIncompatible(NTriangulation& tri, const char* message) {
            NSnapPeaTriangulation s(tri);
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
                lst123.hasBoundaryFaces());
            testIncompatible(lst123, "LST(1,2,3) "
                "should not be representable in SnapPea format.");

            CPPUNIT_ASSERT_MESSAGE("The disconnected double M 2_1 "
                "appears to have been incorrectly constructed.",
                m2_1_m2_1.isValid() && (! m2_1_m2_1.isConnected()) &&
                m2_1_m2_1.isOrientable() && m2_1_m2_1.isIdeal() &&
                m2_1_m2_1.isStandard() && (! m2_1_m2_1.hasBoundaryFaces()));
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
                (! genusTwoTorusCusp.hasBoundaryFaces()));
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
                (! projPlaneCusps.hasBoundaryFaces()));
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
                (! genusFourNonOrCusp.hasBoundaryFaces()));
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
                (! edgeInvalid.hasBoundaryFaces()));
            testIncompatible(edgeInvalid,
                "A triangulation with two invalid edges "
                "should not be representable in SnapPea format.");

            CPPUNIT_ASSERT_MESSAGE(
                "The small closed orientable hyperbolic triangulation "
                "appears to have been incorrectly constructed.",
                closedHypOr.isValid() &&
                closedHypOr.isConnected() &&
                closedHypOr.isOrientable() &&
                (! closedHypOr.isIdeal()) &&
                closedHypOr.isStandard() &&
                (! closedHypOr.hasBoundaryFaces()));
            testIncompatible(closedHypOr,
                "A closed orientable hyperbolic triangulation "
                "should not be representable in SnapPea format.");

            CPPUNIT_ASSERT_MESSAGE(
                "The small closed non-orientable hyperbolic triangulation "
                "appears to have been incorrectly constructed.",
                closedHypNor.isValid() &&
                closedHypNor.isConnected() &&
                (! closedHypNor.isOrientable()) &&
                (! closedHypNor.isIdeal()) &&
                closedHypNor.isStandard() &&
                (! closedHypNor.hasBoundaryFaces()));
            testIncompatible(closedHypNor,
                "A closed non-orientable hyperbolic triangulation "
                "should not be representable in SnapPea format.");

            CPPUNIT_ASSERT_MESSAGE(
                "The cusped solid torus with finite vertex "
                "appears to have been incorrectly constructed.",
                cuspedTorus.isValid() &&
                cuspedTorus.isConnected() &&
                cuspedTorus.isOrientable() &&
                cuspedTorus.isIdeal() &&
                cuspedTorus.isStandard() &&
                (! cuspedTorus.hasBoundaryFaces()));
            testIncompatible(cuspedTorus,
                "A cusped solid torus with an additional finite vertex "
                "should not be representable in SnapPea format.");
        }

        void testVolume(NTriangulation& tri, const char* triName,
                double vol, unsigned places) {
            // Verify the volume to the given number of decimal places.
            // Places are counted after the decimal point in standard
            // (non-scientific) notation.
            NSnapPeaTriangulation s(tri);
            {
                std::ostringstream msg;
                msg << "Triangulation " << triName <<
                    " could not be represented in SnapPea format.";

                CPPUNIT_ASSERT_MESSAGE(msg.str(), ! s.isNull());
            }

            int precision;
            double foundVol = s.volume(precision);
            {
                std::ostringstream msg;
                msg << "Volume for " << triName <<
                    " has a precision of " << precision
                    << " places, which is less than the desired "
                    << places << " places.";

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    precision >= static_cast<int>(places));
            }

            double epsilon = 0.5;
            for (unsigned i = 0; i < places; i++)
                epsilon /= 10;

            {
                std::ostringstream msg;
                msg << "Volume for " << triName << " should be "
                    << std::setprecision(
                        places + static_cast<int>(ceil(log10(vol))))
                    << vol << ", not "
                    << std::setprecision(
                        precision + static_cast<int>(ceil(log10(foundVol))))
                    << foundVol << '.';

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    foundVol <= vol + epsilon && foundVol >= vol - epsilon);
            }
        }

        void volume() {
            testVolume(m2_1,   "M 2_1",   2.0298832128, 9);
            testVolume(m2_2,   "M 2_2",   2.0298832128, 9);
            testVolume(m3_9,   "M 3_9",   2.9441064867, 9);
            testVolume(m4_52,  "M 4_52",  4.0597664256, 9);
            testVolume(m4_1_2, "M 4_1^2", 3.6638623767, 9);
            testVolume(m4_4_2, "M 4_4^2", 4.0597664256, 9);

            testVolume(n1_1,     "N 1_1",     1.0149416064, 9);
            testVolume(n2_1,     "N 2_1",     1.8319311884, 9);
            testVolume(n2_1_2,   "N 2_1^2",   2.0298832128, 9);
            testVolume(n4_14,    "N 4_14",    3.9696478012, 9);
            testVolume(n4_9_2,   "N 4_9^2",   4.0597664256, 9);
            testVolume(n4_1_2_1, "N 4_1^2,1", 3.6638623767, 9);

            // testVolume(closedHypOr, "or_0.94270736", 0.94270736, 7);
            // testVolume(closedHypNor, "nor_2.02988321", 2.02988121, 7);
        }
};

void addNSnapPeaTriangulation(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NSnapPeaTriangulationTest::suite());
}

