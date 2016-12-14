
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

#include <cppunit/extensions/HelperMacros.h>
#include "triangulation/example2.h"
#include "triangulation/dim2.h"

#include "testsuite/exhaustive.h"
#include "testsuite/generic/triangulationtest.h"
#include "testsuite/dim2/testdim2.h"

using regina::Triangulation;
using regina::Example;

class Triangulation2Test : public TriangulationTest<2> {
    CPPUNIT_TEST_SUITE(Triangulation2Test);

    // Generic tests:
    CPPUNIT_TEST(makeCanonical);
    CPPUNIT_TEST(isomorphismSignature);
    CPPUNIT_TEST(orient);
    CPPUNIT_TEST(doubleCover);
    CPPUNIT_TEST(boundaryEdges);

    // Dimension-specific tests:
    CPPUNIT_TEST(validity);
    CPPUNIT_TEST(connectedness);
    CPPUNIT_TEST(orientability);
    CPPUNIT_TEST(eulerChar);
    CPPUNIT_TEST(eltMove13);
    CPPUNIT_TEST(barycentricSubdivision);

    CPPUNIT_TEST_SUITE_END();

    private:
        // Closed orientable:
        Triangulation<2> s2Oct;
            /**< A 2-sphere with eight triangles. */
        Triangulation<2> torus2;
            /**< A genus two torus. */

        // Closed non-orientable:
        Triangulation<2> rp2;
            /**< A projective plane with two triangles. */

        // Disconnected triangulations:
        Triangulation<2> disjoint2;
            /**< A disjoint union of two triangulations. */
        Triangulation<2> disjoint3;
            /**< A disjoint union of three triangulations. */

    public:
        void copyAndDelete(Triangulation<2>& dest, Triangulation<2>* source) {
            dest.insertTriangulation(*source);
            dest.setLabel(source->label());
            delete source;
        }

        void setUp() {
            TriangulationTest<2>::setUp();

            copyAndDelete(s2Oct, Example<2>::sphereOctahedron());
            s2Oct.setLabel("Octahedron boundary");

            copyAndDelete(torus2, Example<2>::orientable(2, 0));
            torus2.setLabel("Genus 2 torus");

            copyAndDelete(rp2, Example<2>::rp2());
            rp2.setLabel("RP^2");

            disjoint2.insertTriangulation(sphereBundle);
            disjoint2.insertTriangulation(twistedBallBundle);
            disjoint2.setLabel("Torus U Mobius");

            disjoint3.insertTriangulation(twistedSphereBundle);
            disjoint3.insertTriangulation(ballBundle);
            disjoint3.insertTriangulation(sphere);
            disjoint3.setLabel("KB U Annulus U S^2");
        }

        void tearDown() {
        }

        /**
         * Run a given test over all hand-coded cases.
         */
        void testManualAll(Triangulation2TestFunction f) {
            f(&empty);
            f(&sphere);
            f(&simplicialSphere);
            f(&s2Oct);
            f(&sphereBundle);
            f(&torus2);
            f(&rp2);
            f(&twistedSphereBundle);
            f(&ball);
            f(&ballBundle);
            f(&twistedBallBundle);
            f(&disjoint2);
            f(&disjoint3);
        }

        void makeCanonical() {
            testManualAll(verifyMakeCanonical);
        }

        void isomorphismSignature() {
            testManualAll(verifyIsomorphismSignature);
        }

        void orient() {
            testManualAll(verifyOrient);
        }

        void doubleCover() {
            testManualAll(verifyDoubleCover);
        }

        void boundaryEdges() {
            testManualAll(verifyBoundaryFacets);
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
            verifyValid(s2Oct);
            verifyValid(torus2);
            verifyValid(rp2);
            verifyValid(disjoint2);
            verifyValid(disjoint3);
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
            verifyConnected(s2Oct);
            verifyConnected(torus2);
            verifyConnected(rp2);
            verifyConnected(disjoint2, false);
            verifyConnected(disjoint3, false);
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
            verifyOrientable(s2Oct);
            verifyOrientable(torus2);
            verifyOrientable(rp2, false);
            verifyOrientable(disjoint2, false);
            verifyOrientable(disjoint3, false);
        }

        void eulerChar() {
            verifyEulerCharTri(empty, 0);
            verifyEulerCharTri(sphere, 2);
            verifyEulerCharTri(simplicialSphere, 2);
            verifyEulerCharTri(sphereBundle, 0);
            verifyEulerCharTri(twistedSphereBundle, 0);
            verifyEulerCharTri(ball, 1);
            verifyEulerCharTri(ballBundle, 0);
            verifyEulerCharTri(twistedBallBundle, 0);
            verifyEulerCharTri(s2Oct, 2);
            verifyEulerCharTri(torus2, -2);
            verifyEulerCharTri(rp2, 1);
            verifyEulerCharTri(disjoint2, 0);
            verifyEulerCharTri(disjoint3, 2);
        }

        static void verifyEltMove13(Triangulation<2>* tri) {
            unsigned long n = tri->size();
            for (unsigned long i = 0; i < n; ++i) {
                Triangulation<2> large(*tri);
                large.oneThreeMove(large.triangle(i));

                if (large.size() != n + 2) {
                    std::ostringstream msg;
                    msg << tri->label() << ", tri " << i << ": "
                        << "1-3 move gives wrong # triangles.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (large.isOrientable() != tri->isOrientable()) {
                    std::ostringstream msg;
                    msg << tri->label() << ", tri " << i << ": "
                        << "1-3 move changes orientability.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (large.isClosed() != tri->isClosed()) {
                    std::ostringstream msg;
                    msg << tri->label() << ", tri " << i << ": "
                        << "1-3 move changes closedness.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (large.countBoundaryComponents() !=
                        tri->countBoundaryComponents()) {
                    std::ostringstream msg;
                    msg << tri->label() << ", tri " << i << ": "
                        << "1-3 move changes # boundary components.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (large.eulerChar() != tri->eulerChar()) {
                    std::ostringstream msg;
                    msg << tri->label() << ", tri " << i << ": "
                        << "1-3 move changes Euler characteristic.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void eltMove13() {
            testManualAll(verifyEltMove13);
        }

        static void verifyBary(Triangulation<2>* tri) {
            Triangulation<2> b(*tri);
            b.barycentricSubdivision();

            if (tri->hasBoundaryEdges() != b.hasBoundaryEdges()) {
                std::ostringstream msg;
                msg << tri->label()
                    << ": Barycentric subdivision breaks boundary edges.";
                CPPUNIT_FAIL(msg.str());
            }

            if (tri->isClosed() != b.isClosed()) {
                std::ostringstream msg;
                msg << tri->label()
                    << ": Barycentric subdivision breaks closedness.";
                CPPUNIT_FAIL(msg.str());
            }

            if (tri->isOrientable() != b.isOrientable()) {
                std::ostringstream msg;
                msg << tri->label()
                    << ": Barycentric subdivision breaks orientability.";
                CPPUNIT_FAIL(msg.str());
            }

            if (tri->isConnected() != b.isConnected()) {
                std::ostringstream msg;
                msg << tri->label()
                    << ": Barycentric subdivision breaks connectedness.";
                CPPUNIT_FAIL(msg.str());
            }

            if (tri->countComponents() != b.countComponents()) {
                std::ostringstream msg;
                msg << tri->label()
                    << ": Barycentric subdivision breaks connected components.";
                CPPUNIT_FAIL(msg.str());
            }

            if (tri->countBoundaryComponents() != b.countBoundaryComponents()) {
                std::ostringstream msg;
                msg << tri->label()
                    << ": Barycentric subdivision breaks boundary components.";
                CPPUNIT_FAIL(msg.str());
            }

            if (tri->eulerCharTri() != b.eulerCharTri()) {
                std::ostringstream msg;
                msg << tri->label()
                    << ": Barycentric subdivision breaks Euler char (tri).";
                CPPUNIT_FAIL(msg.str());
            }

            if (! (tri->homology() == b.homology())) {
                std::ostringstream msg;
                msg << tri->label()
                    << ": Barycentric subdivision breaks H1.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void barycentricSubdivision() {
            testManualAll(verifyBary);
        }

};

void addTriangulation2(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(Triangulation2Test::suite());
}

