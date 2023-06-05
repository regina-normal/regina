
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
        // TODO: sort out the names drama
        // Closed orientable triangulations:
        Triangulation<2> s2Oct { Example<2>::sphereOctahedron() };
        static constexpr const char* s2OctName = "Octahedron boundary";
        Triangulation<2> torus2 { Example<2>::orientable(2, 0) };
        static constexpr const char* torus2Name = "Genus 2 torus";

        // Closed non-orientable triangulations:
        Triangulation<2> rp2 { Example<2>::rp2() };
        static constexpr const char* rp2Name = "RP^2";

        // Disconnected triangulations (we set these up in the constructor):
        Triangulation<2> disjoint2;
        static constexpr const char* disjoint2Name = "Torus U Mobius";
        Triangulation<2> disjoint3;
        static constexpr const char* disjoint3Name = "KB U Annulus U S^2";

        Dim2Test() {
            disjoint2.insertTriangulation(sphereBundle);
            disjoint2.insertTriangulation(twistedBallBundle);

            disjoint3.insertTriangulation(twistedSphereBundle);
            disjoint3.insertTriangulation(ballBundle);
            disjoint3.insertTriangulation(sphere);
        }

        /**
         * Run a given test over all of the example triangulations stored in
         * this test fixture.
         */
        void testManualCases(void (*f)(const Triangulation<2>&, const char*)) {
            testGenericCases(f);

            f(s2Oct, s2OctName);
            f(torus2, torus2Name);
            f(rp2, rp2Name);
            f(disjoint2, disjoint2Name);
            f(disjoint3, disjoint3Name);
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

    verifyValidity(s2Oct, true, s2OctName);
    verifyValidity(torus2, true, torus2Name);
    verifyValidity(rp2, true, rp2Name);
    verifyValidity(disjoint2, true, disjoint2Name);
    verifyValidity(disjoint3, true, disjoint3Name);
}
TEST_F(Dim2Test, connectivity) {
    TriangulationTest<2>::connectivityGenericCases();

    verifyConnectivity(s2Oct, true, s2OctName);
    verifyConnectivity(torus2, true, torus2Name);
    verifyConnectivity(rp2, true, rp2Name);
    verifyConnectivity(disjoint2, false, disjoint2Name);
    verifyConnectivity(disjoint3, false, disjoint3Name);
}
TEST_F(Dim2Test, orientability) {
    TriangulationTest<2>::orientabilityGenericCases();

    verifyOrientability(s2Oct, true, s2OctName);
    verifyOrientability(torus2, true, torus2Name);
    verifyOrientability(rp2, false, rp2Name);
    verifyOrientability(disjoint2, false, disjoint2Name);
    verifyOrientability(disjoint3, false, disjoint3Name);
}
TEST_F(Dim2Test, eulerCharTri) {
    TriangulationTest<2>::eulerCharTriGenericCases();

    verifyEulerCharTri(s2Oct, 2, s2OctName);
    verifyEulerCharTri(torus2, -2, torus2Name);
    verifyEulerCharTri(rp2, 1, rp2Name);
    verifyEulerCharTri(disjoint2, 0, disjoint2Name);
    verifyEulerCharTri(disjoint3, 2, disjoint3Name);
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
