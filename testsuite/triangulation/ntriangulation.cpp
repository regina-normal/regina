
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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

#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "algebra/nabeliangroup.h"
#include "triangulation/ntriangulation.h"
#include "testsuite/triangulation/testtriangulation.h"

using regina::NAbelianGroup;
using regina::NPerm;
using regina::NTetrahedron;
using regina::NTriangulation;

class NTriangulationTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NTriangulationTest);

    CPPUNIT_TEST(validity);
    CPPUNIT_TEST(orientability);
    CPPUNIT_TEST(boundaryComponents);
    CPPUNIT_TEST(homologyH1);
    CPPUNIT_TEST(homologyH1Bdry);

    CPPUNIT_TEST_SUITE_END();

    private:
        NTriangulation lens8_3;
            /**< The lens space L(8,3) : closed, orbl. */
        NTriangulation lens100_1;
            /**< The lens space L(8,3) : closed, orbl. */
        NTriangulation lst3_4_7;
            /**< The layered solid torus LST(3,4,7) : bounded, orbl. */
        NTriangulation figure8;
            /**< The figure eight knot complement : ideal, orbl. */
        NTriangulation rp2xs1;
            /**< The product RP^2 x S^1 : closed, non-orbl. */
        NTriangulation solidKB;
            /**< A solid Klein bottle : bounded, non-orbl. */
        NTriangulation gieseking;
            /**< The Gieseking manifold : ideal, non-orbl. */

    public:
        void setUp() {
            // Some of our triangulations can be constructed automatically.
            lens8_3.insertLensSpace(8, 3);
            lens100_1.insertLensSpace(100, 1);
            lst3_4_7.insertLayeredSolidTorus(3, 4);

            // The rest alas must be done manually.
            NTetrahedron* r;
            NTetrahedron* s;
            NTetrahedron* t;

            // The two-tetrahedron figure eight knot complement is
            // described at the beginning of chapter 8 of Richard
            // Rannard's PhD thesis.
            r = new NTetrahedron();
            s = new NTetrahedron();
            r->joinTo(0, s, NPerm(1, 3, 0, 2));
            r->joinTo(1, s, NPerm(2, 0, 3, 1));
            r->joinTo(2, s, NPerm(0, 3, 2, 1));
            r->joinTo(3, s, NPerm(2, 1, 0, 3));
            figure8.addTetrahedron(r);
            figure8.addTetrahedron(s);

            // A three-tetrahedron Klein bottle and a three-tetrahedron
            // RP^2 x S^1 formed by identifying its boundary faces are
            // described in section 3.5.1 of Ben Burton's PhD thesis.
            r = new NTetrahedron();
            s = new NTetrahedron();
            t = new NTetrahedron();
            s->joinTo(0, r, NPerm(0, 1, 2, 3));
            s->joinTo(3, r, NPerm(3, 0, 1, 2));
            s->joinTo(1, t, NPerm(3, 0, 1, 2));
            s->joinTo(2, t, NPerm(0, 1, 2, 3));
            solidKB.addTetrahedron(r);
            solidKB.addTetrahedron(s);
            solidKB.addTetrahedron(t);

            rp2xs1.insertTriangulation(solidKB);
            r = rp2xs1.getTetrahedron(0);
            t = rp2xs1.getTetrahedron(2);
            r->joinTo(1, t, NPerm(2, 3, 0, 1));
            r->joinTo(3, t, NPerm(2, 3, 0, 1));
            rp2xs1.gluingsHaveChanged();

            // The Gieseking manifold is simple enough; it has only one
            // tetrahedron.
            r = new NTetrahedron();
            r->joinTo(0, r, NPerm(1, 2, 0, 3));
            r->joinTo(2, r, NPerm(0, 2, 3, 1));
            gieseking.addTetrahedron(r);
        }

        void tearDown() {
        }

        void validity() {
            CPPUNIT_ASSERT_MESSAGE("L(8,3) is not valid.",
                lens8_3.isValid());
            CPPUNIT_ASSERT_MESSAGE("L(100,1) is not valid.",
                lens100_1.isValid());
            CPPUNIT_ASSERT_MESSAGE("LST(3,4,7) is not valid.",
                lst3_4_7.isValid());
            CPPUNIT_ASSERT_MESSAGE("The figure eight knot complement "
                "is not valid.",
                figure8.isValid());
            CPPUNIT_ASSERT_MESSAGE("RP^2 x S^1 is not valid.",
                rp2xs1.isValid());
            CPPUNIT_ASSERT_MESSAGE("The solid Klein bottle is not valid.",
                solidKB.isValid());
            CPPUNIT_ASSERT_MESSAGE("The Gieseking manifold is not valid.",
                gieseking.isValid());
        }

        void orientability() {
            CPPUNIT_ASSERT_MESSAGE("L(8,3) is not orientable.",
                lens8_3.isOrientable());
            CPPUNIT_ASSERT_MESSAGE("L(100,1) is not orientable.",
                lens100_1.isOrientable());
            CPPUNIT_ASSERT_MESSAGE("LST(3,4,7) is not orientable.",
                lst3_4_7.isOrientable());
            CPPUNIT_ASSERT_MESSAGE("The figure eight knot complement "
                "is not orientable.",
                figure8.isOrientable());
            CPPUNIT_ASSERT_MESSAGE("RP^2 x S^1 is orientable.",
                ! rp2xs1.isOrientable());
            CPPUNIT_ASSERT_MESSAGE("The solid Klein bottle is orientable.",
                ! solidKB.isOrientable());
            CPPUNIT_ASSERT_MESSAGE("The Gieseking manifold is orientable.",
                ! gieseking.isOrientable());
        }

        void boundaryComponents() {
            CPPUNIT_ASSERT_MESSAGE("L(8,3) has boundary components.",
                lens8_3.getNumberOfBoundaryComponents() == 0);
            CPPUNIT_ASSERT_MESSAGE("L(100,1) has boundary components.",
                lens100_1.getNumberOfBoundaryComponents() == 0);
            CPPUNIT_ASSERT_MESSAGE("LST(3,4,7) has no boundary components.",
                lst3_4_7.getNumberOfBoundaryComponents() > 0);
            CPPUNIT_ASSERT_MESSAGE("The figure eight knot complement "
                "has no boundary components.",
                figure8.getNumberOfBoundaryComponents() > 0);
            CPPUNIT_ASSERT_MESSAGE("RP^2 x S^1 has boundary components.",
                rp2xs1.getNumberOfBoundaryComponents() == 0);
            CPPUNIT_ASSERT_MESSAGE("The solid Klein bottle "
                "has no boundary components.",
                solidKB.getNumberOfBoundaryComponents() > 0);
            CPPUNIT_ASSERT_MESSAGE("The Gieseking manifold "
                "has no boundary components.",
                gieseking.getNumberOfBoundaryComponents() > 0);

            // TODO: Test the individual boundary components.
            // TODO: Check that nobody has too many boundary components.
        }

        void verifyGroup(const NAbelianGroup& g, const std::string& grpName,
                unsigned rank, unsigned long torsionDegree) {
            // Construct the error message.
            std::ostringstream msg;
            msg << grpName << " is " << g.toString() << ", not ";
            if (rank == 0 && torsionDegree <= 1)
                msg << "0";
            else if (rank == 0)
                msg << "Z_" << torsionDegree;
            else if (torsionDegree <= 1)
                msg << rank << " Z";
            else
                msg << rank << " Z + Z_" << torsionDegree;
            msg << '.';

            // Check the group.
            if (g.getRank() != rank)
                CPPUNIT_FAIL(msg.str());
            else {
                // Ranks match.
                if (torsionDegree <= 1) {
                    if (g.getNumberOfInvariantFactors() != 0)
                        CPPUNIT_FAIL(msg.str());
                } else {
                    if (g.getNumberOfInvariantFactors() != 1 ||
                            g.getInvariantFactor(0) != torsionDegree)
                        CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void homologyH1() {
            verifyGroup(lens8_3.getHomologyH1(),
                "H1(L(8,3))", 0, 8);
            verifyGroup(lens100_1.getHomologyH1(),
                "H1(L(100,1))", 0, 100);
            verifyGroup(lst3_4_7.getHomologyH1(),
                "H1(LST(3,4,7))", 1, 0);
            verifyGroup(figure8.getHomologyH1(),
                "H1(figure eight knot complement)", 1, 1);
            verifyGroup(rp2xs1.getHomologyH1(),
                "H1(RP^2 x S^1)", 1, 2);
            verifyGroup(solidKB.getHomologyH1(),
                "H1(solid Klein bottle)", 1, 0);
            verifyGroup(gieseking.getHomologyH1(),
                "H1(Gieseking manifold)", 1, 0);
        }

        void homologyH1Bdry() {
            verifyGroup(lens8_3.getHomologyH1Bdry(),
                "Boundary H1(L(8,3))", 0, 0);
            verifyGroup(lens100_1.getHomologyH1Bdry(),
                "Boundary H1(L(100,1))", 0, 0);
            verifyGroup(lst3_4_7.getHomologyH1Bdry(),
                "Boundary H1(LST(3,4,7))", 2, 0);
            verifyGroup(figure8.getHomologyH1Bdry(),
                "Boundary H1(figure eight knot complement)", 2, 0);
            verifyGroup(rp2xs1.getHomologyH1Bdry(),
                "Boundary H1(RP^2 x S^1)", 0, 0);
            verifyGroup(solidKB.getHomologyH1Bdry(),
                "Boundary H1(solid Klein bottle)", 1, 2);
            verifyGroup(gieseking.getHomologyH1Bdry(),
                "Boundary H1(Gieseking manifold)", 1, 2);
        }
};

void addNTriangulation(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NTriangulationTest::suite());
}

