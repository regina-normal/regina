
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
#include "triangulation/dim4.h"
#include "triangulation/example.h"

#include "testsuite/generic/triangulationtest.h"
#include "testsuite/generic/testgeneric.h"

template <int dim>
class GenericTriangulationTest : public TriangulationTest<dim> {
    private:
        using TriangulationTest<dim>::empty;
        using TriangulationTest<dim>::sphere;
        using TriangulationTest<dim>::simplicialSphere;
        using TriangulationTest<dim>::sphereBundle;
        using TriangulationTest<dim>::twistedSphereBundle;
        using TriangulationTest<dim>::ball;
        using TriangulationTest<dim>::ballBundle;
        using TriangulationTest<dim>::twistedBallBundle;

        using TriangulationTest<dim>::verifyMakeCanonical;
        using TriangulationTest<dim>::verifyIsomorphismSignature;
        using TriangulationTest<dim>::verifyOrient;
        using TriangulationTest<dim>::verifyDoubleCover;
        using TriangulationTest<dim>::verifyBoundaryFacets;
        using TriangulationTest<dim>::verifyBoundaryBuild;

        using TriangulationTest<dim>::verifyValid;
        using TriangulationTest<dim>::verifyConnected;
        using TriangulationTest<dim>::verifyOrientable;
        using TriangulationTest<dim>::verifyEulerCharTri;
        using TriangulationTest<dim>::verifyBoundaryCount;
        using TriangulationTest<dim>::verifyBoundaryH1;
        using TriangulationTest<dim>::verifyHomology;
        using TriangulationTest<dim>::verifyFundGroup;

    public:
        /**
         * Run a given test over all hand-coded test cases.
         */
        void testManualAll(void (*f)(regina::Triangulation<dim>*)) {
            f(&empty);
            f(&sphere);
            f(&simplicialSphere);
            f(&sphereBundle);
            f(&twistedSphereBundle);
            f(&ball);
            f(&ballBundle);
            f(&twistedBallBundle);
        }

        void makeCanonical() {
            testManualAll(TriangulationTest<dim>::verifyMakeCanonical);
        }

        void isomorphismSignature() {
            testManualAll(TriangulationTest<dim>::verifyIsomorphismSignature);
        }

        void orient() {
            testManualAll(TriangulationTest<dim>::verifyOrient);
        }

        void doubleCover() {
            testManualAll(TriangulationTest<dim>::verifyDoubleCover);
        }

        void boundaryFacets() {
            testManualAll(TriangulationTest<dim>::verifyBoundaryFacets);
        }

        void boundaryBuild() {
            testManualAll(TriangulationTest<dim>::verifyBoundaryBuild);
        }

        void validity() {
            verifyValid(empty);
            verifyValid(sphere);
            verifyValid(simplicialSphere);
            verifyValid(sphereBundle);
            verifyValid(twistedSphereBundle);
            verifyValid(ball);
            verifyValid(ballBundle);
            verifyValid(twistedBallBundle);
        }

        void connectedness() {
            verifyConnected(empty);
            verifyConnected(sphere);
            verifyConnected(simplicialSphere);
            verifyConnected(sphereBundle);
            verifyConnected(twistedSphereBundle);
            verifyConnected(ball);
            verifyConnected(ballBundle);
            verifyConnected(twistedBallBundle);
        }

        void orientability() {
            verifyOrientable(empty);
            verifyOrientable(sphere);
            verifyOrientable(simplicialSphere);
            verifyOrientable(sphereBundle);
            verifyOrientable(twistedSphereBundle, false);
            verifyOrientable(ball);
            verifyOrientable(ballBundle);
            verifyOrientable(twistedBallBundle, false);
        }

        void eulerChar() {
            verifyEulerCharTri(empty, 0);
            verifyEulerCharTri(sphere, (dim % 2 ? 0 : 2));
            verifyEulerCharTri(simplicialSphere, (dim % 2 ? 0 : 2));
            verifyEulerCharTri(sphereBundle, 0);
            verifyEulerCharTri(twistedSphereBundle, 0);
            verifyEulerCharTri(ball, 1);
            verifyEulerCharTri(ballBundle, 0);
            verifyEulerCharTri(twistedBallBundle, 0);
        }

        void boundaryCount() {
            verifyBoundaryCount(empty, 0);
            verifyBoundaryCount(sphere, 0);
            verifyBoundaryCount(simplicialSphere, 0);
            verifyBoundaryCount(sphereBundle, 0);
            verifyBoundaryCount(twistedSphereBundle, 0);
            verifyBoundaryCount(ball, 1);
            verifyBoundaryCount(ballBundle, 1);
            verifyBoundaryCount(twistedBallBundle, 1);
        }

        void boundaryHomology() {
            verifyBoundaryH1(ball, 0, "0");
            verifyBoundaryH1(ballBundle, 0, "Z");
            verifyBoundaryH1(twistedBallBundle, 0, "Z");
        }

        void homologyH1() {
            verifyHomology(empty, "0");
            verifyHomology(sphere, "0");
            verifyHomology(simplicialSphere, "0");
            verifyHomology(sphereBundle, "Z");
            verifyHomology(twistedSphereBundle, "Z");
            verifyHomology(ball, "0");
            verifyHomology(ballBundle, "Z");
            verifyHomology(twistedBallBundle, "Z");
        }

        void fundGroup() {
            verifyFundGroup(empty, "0");
            verifyFundGroup(sphere, "0");
            verifyFundGroup(simplicialSphere, "0");
            verifyFundGroup(sphereBundle, "Z");
            verifyFundGroup(twistedSphereBundle, "Z");
            verifyFundGroup(ball, "0");
            verifyFundGroup(ballBundle, "Z");
            verifyFundGroup(twistedBallBundle, "Z");
        }
};

class Triangulation5Test : public GenericTriangulationTest<5> {
    CPPUNIT_TEST_SUITE(Triangulation5Test);

    CPPUNIT_TEST(makeCanonical);
    CPPUNIT_TEST(isomorphismSignature);
    CPPUNIT_TEST(orient);
    CPPUNIT_TEST(doubleCover);
    CPPUNIT_TEST(boundaryFacets);
    CPPUNIT_TEST(boundaryBuild);

    CPPUNIT_TEST(validity);
    CPPUNIT_TEST(connectedness);
    CPPUNIT_TEST(orientability);
    CPPUNIT_TEST(eulerChar);
    CPPUNIT_TEST(boundaryCount);
    CPPUNIT_TEST(boundaryHomology);
    CPPUNIT_TEST(homologyH1);
    CPPUNIT_TEST(fundGroup);

    CPPUNIT_TEST_SUITE_END();
};

class Triangulation6Test : public GenericTriangulationTest<6> {
    CPPUNIT_TEST_SUITE(Triangulation6Test);

    CPPUNIT_TEST(makeCanonical);
    CPPUNIT_TEST(isomorphismSignature);
    CPPUNIT_TEST(orient);
    CPPUNIT_TEST(doubleCover);
    CPPUNIT_TEST(boundaryFacets);
    CPPUNIT_TEST(boundaryBuild);

    CPPUNIT_TEST(validity);
    CPPUNIT_TEST(connectedness);
    CPPUNIT_TEST(orientability);
    CPPUNIT_TEST(eulerChar);
    CPPUNIT_TEST(boundaryCount);
    CPPUNIT_TEST(boundaryHomology);
    CPPUNIT_TEST(homologyH1);
    CPPUNIT_TEST(fundGroup);

    CPPUNIT_TEST_SUITE_END();
};

class Triangulation8Test : public GenericTriangulationTest<8> {
    CPPUNIT_TEST_SUITE(Triangulation8Test);

    /**
     * Isomorphism-related routines have running times that include
     * a factor of (dim+1)!, which makes them Too slow in higher dimensions.
    CPPUNIT_TEST(makeCanonical);
    CPPUNIT_TEST(isomorphismSignature);
     */
    CPPUNIT_TEST(orient);
    CPPUNIT_TEST(doubleCover);
    CPPUNIT_TEST(boundaryFacets);
    CPPUNIT_TEST(boundaryBuild);

    CPPUNIT_TEST(validity);
    CPPUNIT_TEST(connectedness);
    CPPUNIT_TEST(orientability);
    CPPUNIT_TEST(eulerChar);
    CPPUNIT_TEST(boundaryCount);
    CPPUNIT_TEST(boundaryHomology);
    CPPUNIT_TEST(homologyH1);
    CPPUNIT_TEST(fundGroup);

    CPPUNIT_TEST_SUITE_END();
};

void addGenericTriangulation(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(Triangulation5Test::suite());
    runner.addTest(Triangulation6Test::suite());
    runner.addTest(Triangulation8Test::suite());
}

