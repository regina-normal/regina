
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

#include <cppunit/extensions/HelperMacros.h>
#include "triangulation/dim2.h"
#include "triangulation/example2.h"

#include "testsuite/exhaustive.h"
#include "testsuite/generic/triangulationtest.h"
#include "testsuite/dim2/testdim2.h"

using regina::Triangulation;
using regina::Example;

class Triangulation2Test : public TriangulationTest<2> {
    CPPUNIT_TEST_SUITE(Triangulation2Test);

    // Generic tests:
    CPPUNIT_TEST(copyMove);
    CPPUNIT_TEST(makeCanonical);
    CPPUNIT_TEST(isomorphismSignature);
    CPPUNIT_TEST(orient);
    CPPUNIT_TEST(skeleton);
    CPPUNIT_TEST(reordering);
    CPPUNIT_TEST(doubleCover);
    CPPUNIT_TEST(boundaryEdges);
    CPPUNIT_TEST(boundaryFaces);
    CPPUNIT_TEST(boundaryEuler);
    CPPUNIT_TEST(edgeAccess);
    CPPUNIT_TEST(pachner<0>);
    CPPUNIT_TEST(pachner<1>);
    CPPUNIT_TEST(pachner<2>);
    CPPUNIT_TEST(chainComplex<1>);
    CPPUNIT_TEST(dualChainComplex<1>);
    CPPUNIT_TEST(dualToPrimal<0>);
    CPPUNIT_TEST(dualToPrimal<1>);

    // Dimension-specific tests:
    CPPUNIT_TEST(magic);
    CPPUNIT_TEST(validity);
    CPPUNIT_TEST(connectedness);
    CPPUNIT_TEST(orientability);
    CPPUNIT_TEST(eulerChar);
    CPPUNIT_TEST(barycentricSubdivision);
    CPPUNIT_TEST(tightEncoding);

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
        void setUp() override {
            TriangulationTest<2>::setUp();

            s2Oct = Example<2>::sphereOctahedron();

            torus2 = Example<2>::orientable(2, 0);

            rp2 = Example<2>::rp2();

            disjoint2.insertTriangulation(sphereBundle);
            disjoint2.insertTriangulation(twistedBallBundle);

            disjoint3.insertTriangulation(twistedSphereBundle);
            disjoint3.insertTriangulation(ballBundle);
            disjoint3.insertTriangulation(sphere);
        }

        void tearDown() override {
        }

        /**
         * Run a given test over all hand-coded cases.
         */
        void testManualAll(Triangulation2TestFunction f) {
            f(empty, "Empty");
            f(sphere, "Sphere");
            f(simplicialSphere, "Simplicial sphere");
            f(s2Oct, "Octahedron boundary");
            f(sphereBundle, "Torus");
            f(torus2, "Genus 2 torus");
            f(rp2, "RP^2");
            f(twistedSphereBundle, "Klein bottle");
            f(ball, "Ball");
            f(ballBundle, "Annulus");
            f(twistedBallBundle, "Mobius band");
            f(disjoint2, "Torus U Mobius");
            f(disjoint3, "KB U Annulus U S^2");
        }

        void copyMove() {
            testManualAll(verifyCopyMove);
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

        void skeleton() {
            testManualAll(verifySkeleton);
        }

        void reordering() {
            testManualAll(verifyReordering);
        }

        void doubleCover() {
            testManualAll(verifyDoubleCover);
        }

        void boundaryEdges() {
            testManualAll(verifyBoundaryFacets);
        }

        void boundaryFaces() {
            testManualAll(verifyBoundaryFaces);
        }

        void boundaryEuler() {
            testManualAll([](const Triangulation<2>& tri, const char* name) {
                for (auto bc : tri.boundaryComponents())
                    if (bc->eulerChar() != 0) {
                        std::ostringstream msg;
                        msg << name << ": Contains boundary component "
                            "with non-zero Euler characteristic.";
                        CPPUNIT_FAIL(msg.str());
                    }
            });
        }

        template <int k>
        void pachner() {
            testManualAll(verifyPachner<k>);
            verifyPachnerSimplicial<k>();
        }

        template <int k>
        void chainComplex() {
            testManualAll(verifyChainComplex<k>);
        }

        template <int k>
        void dualChainComplex() {
            testManualAll(verifyDualChainComplex<k>);
        }

        template <int k>
        void dualToPrimal() {
            testManualAll(verifyDualToPrimal<k>);
        }

        static void verifyMagic(const Triangulation<2>& t, const char* name) {
            std::string sig = t.isoSig();

            {
                Triangulation<2> recon(sig);
                if (recon.isoSig() != sig) {
                    std::ostringstream msg;
                    msg << name << ": cannot reconstruct from "
                        "isomorphism signature using magic constructor.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void magic() {
            testManualAll(verifyMagic);

            try {
                Triangulation<2> t("INVALID");
                CPPUNIT_FAIL("The magic constructor did not throw an "
                    "exception for an invalid triangulation description.");
            } catch (const regina::InvalidArgument&) {
            }
        }

        void validity() {
            verifyValid(empty, true, "Empty");
            verifyValid(sphere, true, "Sphere");
            verifyValid(simplicialSphere, true, "Simplicial sphere");
            verifyValid(sphereBundle, true, "Torus");
            verifyValid(twistedSphereBundle, true, "Klein bottle");
            verifyValid(ball, true, "Ball");
            verifyValid(ballBundle, true, "Annulus");
            verifyValid(twistedBallBundle, true, "Mobius band");
            verifyValid(s2Oct, true, "Octahedron boundary");
            verifyValid(torus2, true, "Genus 2 torus");
            verifyValid(rp2, true, "RP^2");
            verifyValid(disjoint2, true, "Torus U Mobius");
            verifyValid(disjoint3, true, "KB U Annulus U S^2");
        }

        void connectedness() {
            verifyConnected(empty, true, "Empty");
            verifyConnected(sphere, true, "Sphere");
            verifyConnected(simplicialSphere, true, "Simplicial sphere");
            verifyConnected(sphereBundle, true, "Torus");
            verifyConnected(twistedSphereBundle, true, "Klein bottle");
            verifyConnected(ball, true, "Ball");
            verifyConnected(ballBundle, true, "Annulus");
            verifyConnected(twistedBallBundle, true, "Mobius band");
            verifyConnected(s2Oct, true, "Octahedron boundary");
            verifyConnected(torus2, true, "Genus 2 torus");
            verifyConnected(rp2, true, "RP^2");
            verifyConnected(disjoint2, false, "Torus U Mobius");
            verifyConnected(disjoint3, false, "KB U Annulus U S^2");
        }

        void orientability() {
            verifyOrientable(empty, true, "Empty");
            verifyOrientable(sphere, true, "Sphere");
            verifyOrientable(simplicialSphere, true, "Simplicial sphere");
            verifyOrientable(sphereBundle, true, "Torus");
            verifyOrientable(twistedSphereBundle, false, "Klein bottle");
            verifyOrientable(ball, true, "Ball");
            verifyOrientable(ballBundle, true, "Annulus");
            verifyOrientable(twistedBallBundle, false, "Mobius band");
            verifyOrientable(s2Oct, true, "Octahedron boundary");
            verifyOrientable(torus2, true, "Genus 2 torus");
            verifyOrientable(rp2, false, "RP^2");
            verifyOrientable(disjoint2, false, "Torus U Mobius");
            verifyOrientable(disjoint3, false, "KB U Annulus U S^2");
        }

        void eulerChar() {
            verifyEulerCharTri(empty, 0, "Empty");
            verifyEulerCharTri(sphere, 2, "Sphere");
            verifyEulerCharTri(simplicialSphere, 2, "Simplicial sphere");
            verifyEulerCharTri(sphereBundle, 0, "Torus");
            verifyEulerCharTri(twistedSphereBundle, 0, "Klein bottle");
            verifyEulerCharTri(ball, 1, "Ball");
            verifyEulerCharTri(ballBundle, 0, "Annulus");
            verifyEulerCharTri(twistedBallBundle, 0, "Mobius band");
            verifyEulerCharTri(s2Oct, 2, "Octahedron boundary");
            verifyEulerCharTri(torus2, -2, "Genus 2 torus");
            verifyEulerCharTri(rp2, 1, "RP^2");
            verifyEulerCharTri(disjoint2, 0, "Torus U Mobius");
            verifyEulerCharTri(disjoint3, 2, "KB U Annulus U S^2");
        }

        static void verifyBary(const Triangulation<2>& tri, const char* name) {
            Triangulation<2> b(tri);
            if (b.isOrientable())
                b.orient();

            b.subdivide();
            clearProperties(b);

            if (tri.hasBoundaryEdges() != b.hasBoundaryEdges()) {
                std::ostringstream msg;
                msg << name
                    << ": Barycentric subdivision breaks boundary edges.";
                CPPUNIT_FAIL(msg.str());
            }

            if (tri.isClosed() != b.isClosed()) {
                std::ostringstream msg;
                msg << name
                    << ": Barycentric subdivision breaks closedness.";
                CPPUNIT_FAIL(msg.str());
            }

            if (tri.isOrientable() != b.isOrientable()) {
                std::ostringstream msg;
                msg << name
                    << ": Barycentric subdivision breaks orientability.";
                CPPUNIT_FAIL(msg.str());
            }

            if (tri.isOrientable() != b.isOriented()) {
                std::ostringstream msg;
                msg << name
                    << ": Barycentric subdivision breaks orientation.";
                CPPUNIT_FAIL(msg.str());
            }

            if (tri.isConnected() != b.isConnected()) {
                std::ostringstream msg;
                msg << name
                    << ": Barycentric subdivision breaks connectedness.";
                CPPUNIT_FAIL(msg.str());
            }

            if (tri.countComponents() != b.countComponents()) {
                std::ostringstream msg;
                msg << name
                    << ": Barycentric subdivision breaks connected components.";
                CPPUNIT_FAIL(msg.str());
            }

            if (tri.countBoundaryComponents() != b.countBoundaryComponents()) {
                std::ostringstream msg;
                msg << name
                    << ": Barycentric subdivision breaks boundary components.";
                CPPUNIT_FAIL(msg.str());
            }

            if (tri.eulerCharTri() != b.eulerCharTri()) {
                std::ostringstream msg;
                msg << name
                    << ": Barycentric subdivision breaks Euler char (tri).";
                CPPUNIT_FAIL(msg.str());
            }

            if (! (tri.homology() == b.homology())) {
                std::ostringstream msg;
                msg << name
                    << ": Barycentric subdivision breaks H1.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void barycentricSubdivision() {
            testManualAll(verifyBary);
        }

        void tightEncoding() {
            testManualAll(verifyTightEncodingWithName);
        }
};

void addTriangulation2(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(Triangulation2Test::suite());
}

