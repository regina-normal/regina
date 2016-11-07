
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
#include "testsuite/generic/generictriangulation.h"
#include "testsuite/dim2/testdim2.h"

using regina::Triangulation;
using regina::Example;

class Triangulation2Test : public TriangulationTest<2> {
    CPPUNIT_TEST_SUITE(Triangulation2Test);

    // Generic tests:
    CPPUNIT_TEST(boundaryEdges);
    CPPUNIT_TEST(makeCanonical);
    CPPUNIT_TEST(isomorphismSignature);
    CPPUNIT_TEST(orient);
    CPPUNIT_TEST(doubleCover);

    // Dimension-specific tests:
    CPPUNIT_TEST(eltMove13);

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

        void boundaryEdges() {
            testManualAll(verifyBoundaryFacets);
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

        void doubleCover() {
            testManualAll(verifyDoubleCover);
        }

        void eltMove13() {
            testManualAll(verifyEltMove13);
        }
};

void addTriangulation2(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(Triangulation2Test::suite());
}

