
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

#include "triangulation/dim2.h"
#include "triangulation/example2.h"

#include "generic/triangulationtest.h"

using regina::Triangulation;
using regina::Example;

class Dim2Test : public TriangulationTest<2> {
    protected:
        // Closed orientable triangulations:
        TestCase s2Oct { Example<2>::sphereOctahedron(),
            "Octahedron boundary" };
        TestCase torus2 { Example<2>::orientable(2, 0), "Genus 2 torus" };

        // Closed non-orientable triangulations:
        TestCase rp2 { Example<2>::rp2(), "RP^2" };

        // Disconnected triangulations (we build these in the constructor):
        TestCase disjoint2 { {}, "Torus U Mobius" };
        TestCase disjoint3 { {}, "KB U Annulus U S^2" };

        Dim2Test() {
            disjoint2.tri.insertTriangulation(sphereBundle.tri);
            disjoint2.tri.insertTriangulation(twistedBallBundle.tri);

            disjoint3.tri.insertTriangulation(twistedSphereBundle.tri);
            disjoint3.tri.insertTriangulation(ballBundle.tri);
            disjoint3.tri.insertTriangulation(sphere.tri);
        }

        /**
         * Run a given test over all of the example triangulations stored in
         * this test fixture.
         */
        void testManualCases(void (*f)(const Triangulation<2>&, const char*)) {
            testGenericCases(f);

            f(s2Oct.tri, s2Oct.name);
            f(torus2.tri, torus2.name);
            f(rp2.tri, rp2.name);
            f(disjoint2.tri, disjoint2.name);
            f(disjoint3.tri, disjoint3.name);
        }
};

TEST_F(Dim2Test, magic) {
    // Verify the "magic" string-based constructor.
    testManualCases([](const Triangulation<2>& t, const char* name) {
        std::string sig = t.isoSig();
        Triangulation<2> recon(sig);
        EXPECT_EQ(recon.isoSig(), sig);
    });

    EXPECT_THROW({
        Triangulation<2> t("INVALID");
    }, regina::InvalidArgument);
}
TEST_F(Dim2Test, validity) {
    TriangulationTest<2>::validityGenericCases();

    verifyValidity(s2Oct, true);
    verifyValidity(torus2, true);
    verifyValidity(rp2, true);
    verifyValidity(disjoint2, true);
    verifyValidity(disjoint3, true);
}
TEST_F(Dim2Test, connectivity) {
    TriangulationTest<2>::connectivityGenericCases();

    verifyConnectivity(s2Oct, true);
    verifyConnectivity(torus2, true);
    verifyConnectivity(rp2, true);
    verifyConnectivity(disjoint2, false);
    verifyConnectivity(disjoint3, false);
}
TEST_F(Dim2Test, orientability) {
    TriangulationTest<2>::orientabilityGenericCases();

    verifyOrientability(s2Oct, true);
    verifyOrientability(torus2, true);
    verifyOrientability(rp2, false);
    verifyOrientability(disjoint2, false);
    verifyOrientability(disjoint3, false);
}
TEST_F(Dim2Test, eulerCharTri) {
    TriangulationTest<2>::eulerCharTriGenericCases();

    verifyEulerCharTri(s2Oct, 2);
    verifyEulerCharTri(torus2, -2);
    verifyEulerCharTri(rp2, 1);
    verifyEulerCharTri(disjoint2, 0);
    verifyEulerCharTri(disjoint3, 2);
}
TEST_F(Dim2Test, boundaryCount) {
    TriangulationTest<2>::boundaryCountGenericCases();
}
TEST_F(Dim2Test, boundaryEuler) {
    testManualCases(TriangulationTest<2>::verifyBoundaryEuler);
}
TEST_F(Dim2Test, orient) {
    testManualCases(TriangulationTest<2>::verifyOrient);
}
TEST_F(Dim2Test, skeleton) {
    testManualCases(TriangulationTest<2>::verifySkeleton);
}
TEST_F(Dim2Test, edgeAccess) {
    TriangulationTest<2>::edgeAccess();
}
TEST_F(Dim2Test, reordering) {
    testManualCases(TriangulationTest<2>::verifyReordering);
}
TEST_F(Dim2Test, doubleCover) {
    testManualCases(TriangulationTest<2>::verifyDoubleCover);
}
TEST_F(Dim2Test, makeCanonical) {
    testManualCases(TriangulationTest<2>::verifyMakeCanonical);
}
TEST_F(Dim2Test, isomorphismSignature) {
    testManualCases(TriangulationTest<2>::verifyIsomorphismSignature);
}
TEST_F(Dim2Test, pachner) {
    testManualCases(TriangulationTest<2>::verifyPachner);
}
TEST_F(Dim2Test, pachnerSimplicial) {
    TriangulationTest<2>::verifyPachnerSimplicial();
}
TEST_F(Dim2Test, barycentricSubdivision) {
    testManualCases(TriangulationTest<2>::verifyBarycentricSubdivision);
}
TEST_F(Dim2Test, tightEncoding) {
    testManualCases(TriangulationTest<2>::verifyTightEncoding);
}
TEST_F(Dim2Test, homologyH1) {
    TriangulationTest<2>::homologyH1GenericCases();
}
TEST_F(Dim2Test, fundGroup) {
    TriangulationTest<2>::fundGroupGenericCases();
}
TEST_F(Dim2Test, chainComplex) {
    testManualCases(TriangulationTest<2>::verifyChainComplex);
}
TEST_F(Dim2Test, dualChainComplex) {
    testManualCases(TriangulationTest<2>::verifyDualChainComplex);
}
TEST_F(Dim2Test, dualToPrimal) {
    testManualCases(TriangulationTest<2>::verifyDualToPrimal);
}
TEST_F(Dim2Test, copyMove) {
    testManualCases(TriangulationTest<2>::verifyCopyMove);
}
