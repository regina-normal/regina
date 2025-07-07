
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

#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "triangulation/dim4.h"
#include "triangulation/example.h"
#include "triangulation/example4.h"

#include "testsuite/generic/triangulationtest.h"
#include "testsuite/generic/testgeneric.h"

using regina::Example;

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

        using TriangulationTest<dim>::verifyCopyMove;
        using TriangulationTest<dim>::verifyMakeCanonical;
        using TriangulationTest<dim>::verifyIsomorphismSignature;
        using TriangulationTest<dim>::verifyOrient;
        using TriangulationTest<dim>::verifySkeleton;
        using TriangulationTest<dim>::verifyDoubleCover;
        using TriangulationTest<dim>::verifyBoundaryFacets;
        using TriangulationTest<dim>::verifyBoundaryFaces;
        using TriangulationTest<dim>::verifyBoundaryBuild;

        using TriangulationTest<dim>::verifyValid;
        using TriangulationTest<dim>::verifyConnected;
        using TriangulationTest<dim>::verifyOrientable;
        using TriangulationTest<dim>::verifyEulerCharTri;
        using TriangulationTest<dim>::verifyBoundaryCount;
        using TriangulationTest<dim>::verifyBoundaryH1;
        using TriangulationTest<dim>::verifyFundGroup;
        using TriangulationTest<dim>::verifyTightEncodingWithName;

    public:
        /**
         * Run a given test over all hand-coded test cases.
         */
        void testManualAll(
                void (*f)(const regina::Triangulation<dim>&, const char*)) {
            f(empty, "Empty");
            f(sphere, "Sphere");
            f(simplicialSphere, "Simplicial sphere");
            f(sphereBundle, "Sphere bundle");
            f(twistedSphereBundle, "Twisted sphere bundle");
            f(ball, "Ball");
            f(ballBundle, "Ball bundle");
            f(twistedBallBundle, "Twisted ball bundle");
        }

        void copyMove() {
            testManualAll(TriangulationTest<dim>::verifyCopyMove);
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

        void skeleton() {
            testManualAll(verifySkeleton);
        }

        void doubleCover() {
            testManualAll(TriangulationTest<dim>::verifyDoubleCover);
        }

        void boundaryFacets() {
            testManualAll(TriangulationTest<dim>::verifyBoundaryFacets);
        }

        void boundaryFaces() {
            testManualAll(TriangulationTest<dim>::verifyBoundaryFaces);
        }

        void boundaryBuild() {
            testManualAll(TriangulationTest<dim>::verifyBoundaryBuild);
        }

        template <int k>
        void pachner() {
            testManualAll(
                TriangulationTest<dim>::template verifyPachner<k>);
            TriangulationTest<dim>::template verifyPachnerSimplicial<k>();
        }

        void validity() {
            verifyValid(empty, true, "Empty");
            verifyValid(sphere, true, "Sphere");
            verifyValid(simplicialSphere, true, "Simplicial sphere");
            verifyValid(sphereBundle, true, "Sphere bundle");
            verifyValid(twistedSphereBundle, true, "Twisted sphere bundle");
            verifyValid(ball, true, "Ball");
            verifyValid(ballBundle, true, "Ball bundle");
            verifyValid(twistedBallBundle, true, "Twisted ball bundle");
        }

        void connectedness() {
            verifyConnected(empty, true, "Empty");
            verifyConnected(sphere, true, "Sphere");
            verifyConnected(simplicialSphere, true, "Simplicial sphere");
            verifyConnected(sphereBundle, true, "Sphere bundle");
            verifyConnected(twistedSphereBundle, true, "Twisted sphere bundle");
            verifyConnected(ball, true, "Ball");
            verifyConnected(ballBundle, true, "Ball bundle");
            verifyConnected(twistedBallBundle, true, "Twisted ball bundle");
        }

        void orientability() {
            verifyOrientable(empty, true, "Empty");
            verifyOrientable(sphere, true, "Sphere");
            verifyOrientable(simplicialSphere, true, "Simplicial sphere");
            verifyOrientable(sphereBundle, true, "Sphere bundle");
            verifyOrientable(twistedSphereBundle, false,
                "Twisted sphere bundle");
            verifyOrientable(ball, true, "Ball");
            verifyOrientable(ballBundle, true, "Ball bundle");
            verifyOrientable(twistedBallBundle, false, "Twisted ball bundle");
        }

        void eulerChar() {
            verifyEulerCharTri(empty, 0, "Empty");
            verifyEulerCharTri(sphere, (dim % 2 ? 0 : 2), "Sphere");
            verifyEulerCharTri(simplicialSphere, (dim % 2 ? 0 : 2),
                "Simplicial sphere");
            verifyEulerCharTri(sphereBundle, 0, "Sphere bundle");
            verifyEulerCharTri(twistedSphereBundle, 0, "Twisted sphere bundle");
            verifyEulerCharTri(ball, 1, "Ball");
            verifyEulerCharTri(ballBundle, 0, "Ball bundle");
            verifyEulerCharTri(twistedBallBundle, 0, "Twisted ball bundle");
        }

        void boundaryCount() {
            verifyBoundaryCount(empty, 0, 0, 0, "Empty");
            verifyBoundaryCount(sphere, 0, 0, 0, "Sphere");
            verifyBoundaryCount(simplicialSphere, 0, 0, 0, "Simplicial sphere");
            verifyBoundaryCount(sphereBundle, 0, 0, 0, "Sphere bundle");
            verifyBoundaryCount(twistedSphereBundle, 0, 0, 0,
                "Twisted sphere bundle");
            verifyBoundaryCount(ball, 1, 0, 0, "Ball");
            verifyBoundaryCount(ballBundle, 1, 0, 0, "Ball bundle");
            verifyBoundaryCount(twistedBallBundle, 1, 0, 0,
                "Twisted ball bundle");
        }

        void boundaryHomology() {
            verifyBoundaryH1(ball, 0, "0", "Ball");
            verifyBoundaryH1(ballBundle, 0, "Z", "Ball bundle");
            verifyBoundaryH1(twistedBallBundle, 0, "Z", "Twisted ball bundle");
        }

        void homologyH1() {
            TriangulationTest<dim>::template verifyHomology<>(empty,
                "0", "Empty");
            TriangulationTest<dim>::template verifyHomology<>(sphere,
                "0", "Sphere");
            TriangulationTest<dim>::template verifyHomology<>(simplicialSphere,
                "0", "Simplicial sphere");
            TriangulationTest<dim>::template verifyHomology<>(sphereBundle,
                "Z", "Sphere bundle");
            TriangulationTest<dim>::template verifyHomology<>(twistedSphereBundle,
                "Z", "Twisted sphere bundle");
            TriangulationTest<dim>::template verifyHomology<>(ball,
                "0", "Ball");
            TriangulationTest<dim>::template verifyHomology<>(ballBundle,
                "Z", "Ball bundle");
            TriangulationTest<dim>::template verifyHomology<>(twistedBallBundle,
                "Z", "Twisted ball bundle");
        }

        void homologyH2() {
            static_assert(dim > 3); // otherwise expected H2 groups are wrong

            // It's a pity that almost all of these examples have trivial H2.
            // We need some more interesting generic constructions.

            TriangulationTest<dim>::template verifyHomology<2>(empty,
                "0", "Empty");
            TriangulationTest<dim>::template verifyHomology<2>(sphere,
                "0", "Sphere");
            TriangulationTest<dim>::template verifyHomology<2>(simplicialSphere,
                "0", "Simplicial sphere");
            TriangulationTest<dim>::template verifyHomology<2>(sphereBundle,
                "0", "Sphere bundle");
            TriangulationTest<dim>::template verifyHomology<2>(
                twistedSphereBundle, "0", "Twisted sphere bundle");
            TriangulationTest<dim>::template verifyHomology<2>(ball,
                "0", "Ball");
            TriangulationTest<dim>::template verifyHomology<2>(ballBundle,
                "0", "Ball bundle");
            TriangulationTest<dim>::template verifyHomology<2>(
                twistedBallBundle, "0", "Twisted ball bundle");

            if constexpr (dim == 5) {
                TriangulationTest<dim>::template verifyHomology<2>(
                    Example<5>::singleCone(Example<4>::sphereBundle()),
                    "0", "Real/ideal S3 x S1 x I");
                TriangulationTest<dim>::template verifyHomology<2>(
                    Example<5>::doubleCone(Example<4>::sphereBundle()),
                    "0", "ideal S3 x S1 x I");

                TriangulationTest<dim>::template verifyHomology<2>(
                    Example<5>::singleCone(Example<4>::twistedSphereBundle()),
                    "0", "Real/ideal S3 x~ S1 x I");
                TriangulationTest<dim>::template verifyHomology<2>(
                    Example<5>::doubleCone(Example<4>::twistedSphereBundle()),
                    "0", "ideal S3 x~ S1 x I");

                TriangulationTest<dim>::template verifyHomology<2>(
                    Example<5>::singleCone(Example<4>::s2xs2()),
                    "2 Z", "Real/ideal S2 x S2 x I");
                TriangulationTest<dim>::template verifyHomology<2>(
                    Example<5>::doubleCone(Example<4>::s2xs2()),
                    "2 Z", "ideal S2 x S2 x I");
            }
        }

        void homologyH3() {
            static_assert(dim > 4); // otherwise expected H3 groups are wrong

            // It's a pity that almost all of these examples have trivial H2.
            // We need some more interesting generic constructions.

            TriangulationTest<dim>::template verifyHomology<3>(empty,
                "0", "Empty");
            TriangulationTest<dim>::template verifyHomology<3>(sphere,
                "0", "Sphere");
            TriangulationTest<dim>::template verifyHomology<3>(simplicialSphere,
                "0", "Simplicial sphere");
            TriangulationTest<dim>::template verifyHomology<3>(sphereBundle,
                "0", "Sphere bundle");
            TriangulationTest<dim>::template verifyHomology<3>(
                twistedSphereBundle, "0", "Twisted sphere bundle");
            TriangulationTest<dim>::template verifyHomology<3>(ball,
                "0", "Ball");
            TriangulationTest<dim>::template verifyHomology<3>(ballBundle,
                "0", "Ball bundle");
            TriangulationTest<dim>::template verifyHomology<3>(
                twistedBallBundle, "0", "Twisted ball bundle");

            if constexpr (dim == 5) {
                TriangulationTest<dim>::template verifyHomology<3>(
                    Example<5>::singleCone(Example<4>::sphereBundle()),
                    "Z", "Real/ideal S3 x S1 x I");
                TriangulationTest<dim>::template verifyHomology<3>(
                    Example<5>::doubleCone(Example<4>::sphereBundle()),
                    "Z", "ideal S3 x S1 x I");

                TriangulationTest<dim>::template verifyHomology<3>(
                    Example<5>::singleCone(Example<4>::twistedSphereBundle()),
                    "Z_2", "Real/ideal S3 x~ S1 x I");
                TriangulationTest<dim>::template verifyHomology<3>(
                    Example<5>::doubleCone(Example<4>::twistedSphereBundle()),
                    "Z_2", "ideal S3 x~ S1 x I");

                TriangulationTest<dim>::template verifyHomology<3>(
                    Example<5>::singleCone(Example<4>::s2xs2()),
                    "0", "Real/ideal S2 x S2 x I");
                TriangulationTest<dim>::template verifyHomology<3>(
                    Example<5>::doubleCone(Example<4>::s2xs2()),
                    "0", "ideal S2 x S2 x I");
            }
        }

        void fundGroup() {
            verifyFundGroup(empty, "0", "Empty");
            verifyFundGroup(sphere, "0", "Sphere");
            verifyFundGroup(simplicialSphere, "0", "Simplicial sphere");
            verifyFundGroup(sphereBundle, "Z", "Sphere bundle");
            verifyFundGroup(twistedSphereBundle, "Z", "Twisted sphere bundle");
            verifyFundGroup(ball, "0", "Ball");
            verifyFundGroup(ballBundle, "Z", "Ball bundle");
            verifyFundGroup(twistedBallBundle, "Z", "Twisted ball bundle");
        }

        void tightEncoding() {
            testManualAll(verifyTightEncodingWithName);
        }
};

class Triangulation5Test : public GenericTriangulationTest<5> {
    CPPUNIT_TEST_SUITE(Triangulation5Test);

    CPPUNIT_TEST(copyMove);
    CPPUNIT_TEST(makeCanonical);
    CPPUNIT_TEST(isomorphismSignature);
    CPPUNIT_TEST(orient);
    CPPUNIT_TEST(skeleton);
    CPPUNIT_TEST(doubleCover);
    CPPUNIT_TEST(boundaryFacets);
    CPPUNIT_TEST(boundaryFaces);
    CPPUNIT_TEST(boundaryBuild);
    CPPUNIT_TEST(edgeAccess);
    CPPUNIT_TEST(pachner<0>);
    CPPUNIT_TEST(pachner<1>);
    CPPUNIT_TEST(pachner<2>);
    CPPUNIT_TEST(pachner<3>);
    CPPUNIT_TEST(pachner<4>);
    CPPUNIT_TEST(pachner<5>);

    CPPUNIT_TEST(validity);
    CPPUNIT_TEST(connectedness);
    CPPUNIT_TEST(orientability);
    CPPUNIT_TEST(eulerChar);
    CPPUNIT_TEST(boundaryCount);
    CPPUNIT_TEST(boundaryHomology);
    CPPUNIT_TEST(homologyH1);
    CPPUNIT_TEST(homologyH2);
    CPPUNIT_TEST(homologyH3);
    CPPUNIT_TEST(fundGroup);
    CPPUNIT_TEST(tightEncoding);

    CPPUNIT_TEST_SUITE_END();
};

class Triangulation6Test : public GenericTriangulationTest<6> {
    CPPUNIT_TEST_SUITE(Triangulation6Test);

    CPPUNIT_TEST(copyMove);
    CPPUNIT_TEST(makeCanonical);
    CPPUNIT_TEST(isomorphismSignature);
    CPPUNIT_TEST(orient);
    CPPUNIT_TEST(skeleton);
    CPPUNIT_TEST(doubleCover);
    CPPUNIT_TEST(boundaryFacets);
    CPPUNIT_TEST(boundaryFaces);
    CPPUNIT_TEST(boundaryBuild);
    CPPUNIT_TEST(edgeAccess);
    CPPUNIT_TEST(pachner<0>);
    CPPUNIT_TEST(pachner<1>);
    CPPUNIT_TEST(pachner<2>);
    CPPUNIT_TEST(pachner<3>);
    CPPUNIT_TEST(pachner<4>);
    CPPUNIT_TEST(pachner<5>);
    CPPUNIT_TEST(pachner<6>);

    CPPUNIT_TEST(validity);
    CPPUNIT_TEST(connectedness);
    CPPUNIT_TEST(orientability);
    CPPUNIT_TEST(eulerChar);
    CPPUNIT_TEST(boundaryCount);
    CPPUNIT_TEST(boundaryHomology);
    CPPUNIT_TEST(homologyH1);
    CPPUNIT_TEST(homologyH2);
    CPPUNIT_TEST(homologyH3);
    CPPUNIT_TEST(fundGroup);
    CPPUNIT_TEST(tightEncoding);

    CPPUNIT_TEST_SUITE_END();
};

class Triangulation8Test : public GenericTriangulationTest<8> {
    CPPUNIT_TEST_SUITE(Triangulation8Test);

    CPPUNIT_TEST(copyMove);
    /**
     * Isomorphism-related routines have running times that include
     * a factor of (dim+1)!, which makes them too slow in higher dimensions.
    CPPUNIT_TEST(makeCanonical);
    CPPUNIT_TEST(isomorphismSignature);
     */
    CPPUNIT_TEST(orient);
    CPPUNIT_TEST(skeleton);
    CPPUNIT_TEST(doubleCover);
    CPPUNIT_TEST(boundaryFacets);
    CPPUNIT_TEST(boundaryFaces);
    CPPUNIT_TEST(boundaryBuild);
    CPPUNIT_TEST(edgeAccess);
    /**
     * Pachner moves are also taking a long time to test thoroughly
     * in higher dimensions.  Since we already have non-standard even
     * dimensions in our test suite (dim = 6), we leave these out for now.
    CPPUNIT_TEST(pachner<0>);
    CPPUNIT_TEST(pachner<1>);
    CPPUNIT_TEST(pachner<2>);
    CPPUNIT_TEST(pachner<3>);
    CPPUNIT_TEST(pachner<4>);
    CPPUNIT_TEST(pachner<5>);
    CPPUNIT_TEST(pachner<6>);
    CPPUNIT_TEST(pachner<7>);
    CPPUNIT_TEST(pachner<8>);
     */

    CPPUNIT_TEST(validity);
    CPPUNIT_TEST(connectedness);
    CPPUNIT_TEST(orientability);
    CPPUNIT_TEST(eulerChar);
    CPPUNIT_TEST(boundaryCount);
    CPPUNIT_TEST(boundaryHomology);
    CPPUNIT_TEST(homologyH1);
    CPPUNIT_TEST(homologyH2);
    CPPUNIT_TEST(homologyH3);
    CPPUNIT_TEST(fundGroup);
    CPPUNIT_TEST(tightEncoding);

    CPPUNIT_TEST_SUITE_END();
};

void addGenericTriangulation(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(Triangulation5Test::suite());
    runner.addTest(Triangulation6Test::suite());
    runner.addTest(Triangulation8Test::suite());
}

