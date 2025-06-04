
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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

#include "triangulation/dim4.h" // for boundaries in 5-D
#include "triangulation/example4.h" // for building cones in 5-D

#include "generic/triangulationtest.h"

using regina::Example;

class Dim5Test : public TriangulationTest<5> {
};

TEST_F(Dim5Test, validity) {
    TriangulationTest<5>::validityGenericCases();
}
TEST_F(Dim5Test, connectivity) {
    TriangulationTest<5>::connectivityGenericCases();
}
TEST_F(Dim5Test, orientability) {
    TriangulationTest<5>::orientabilityGenericCases();
}
TEST_F(Dim5Test, eulerChar) {
    TriangulationTest<5>::eulerCharGenericCases();
}
TEST_F(Dim5Test, boundaryBasic) {
    TriangulationTest<5>::boundaryBasicGenericCases();
}
TEST_F(Dim5Test, vertexLinksBasic) {
    TriangulationTest<5>::vertexLinksBasicGenericCases();
}
TEST_F(Dim5Test, orient) {
    testGenericCases(TriangulationTest<5>::verifyOrient);
}
TEST_F(Dim5Test, skeleton) {
    testGenericCases(TriangulationTest<5>::verifySkeleton);
}
TEST_F(Dim5Test, edgeAccess) {
    TriangulationTest<5>::edgeAccess();
}
TEST_F(Dim5Test, reordering) {
    testGenericCases(TriangulationTest<5>::verifyReordering);
}
TEST_F(Dim5Test, doubleCover) {
    testGenericCases(TriangulationTest<5>::verifyDoubleCover);
}
TEST_F(Dim5Test, doubleOverBoundary) {
    testGenericCases(TriangulationTest<5>::verifyDoubleOverBoundary);
}
TEST_F(Dim5Test, makeCanonical) {
    testGenericCases(TriangulationTest<5>::verifyMakeCanonical);
}
TEST_F(Dim5Test, isomorphismSignature) {
    testGenericCases(TriangulationTest<5>::verifyIsomorphismSignature);
}
TEST_F(Dim5Test, pachner) {
    testGenericCases(TriangulationTest<5>::verifyPachner);
    TriangulationTest<5>::verifyPachnerSimplicial();
}
TEST_F(Dim5Test, tightEncoding) {
    testGenericCases(TriangulationTest<5>::verifyTightEncoding);
}
TEST_F(Dim5Test, homologyH1) {
    TriangulationTest<5>::homologyH1GenericCases();
}
TEST_F(Dim5Test, homologyH2) {
    TriangulationTest<5>::homologyH2GenericCases();
}
TEST_F(Dim5Test, homologyH3) {
    TriangulationTest<5>::homologyH3GenericCases();
}
TEST_F(Dim5Test, boundaryHomology) {
    TriangulationTest<5>::boundaryHomologyGenericCases();
}
TEST_F(Dim5Test, fundGroup) {
    TriangulationTest<5>::fundGroupGenericCases();
}
TEST_F(Dim5Test, copyMove) {
    testGenericCases(TriangulationTest<5>::verifyCopyMove);
}

class Dim6Test : public TriangulationTest<6> {
};

TEST_F(Dim6Test, validity) {
    TriangulationTest<6>::validityGenericCases();
}
TEST_F(Dim6Test, connectivity) {
    TriangulationTest<6>::connectivityGenericCases();
}
TEST_F(Dim6Test, orientability) {
    TriangulationTest<6>::orientabilityGenericCases();
}
TEST_F(Dim6Test, eulerChar) {
    TriangulationTest<6>::eulerCharGenericCases();
}
TEST_F(Dim6Test, boundaryBasic) {
    TriangulationTest<6>::boundaryBasicGenericCases();
}
TEST_F(Dim6Test, vertexLinksBasic) {
    TriangulationTest<6>::vertexLinksBasicGenericCases();
}
TEST_F(Dim6Test, orient) {
    testGenericCases(TriangulationTest<6>::verifyOrient);
}
TEST_F(Dim6Test, skeleton) {
    testGenericCases(TriangulationTest<6>::verifySkeleton);
}
TEST_F(Dim6Test, edgeAccess) {
    TriangulationTest<6>::edgeAccess();
}
TEST_F(Dim6Test, reordering) {
    testGenericCases(TriangulationTest<6>::verifyReordering);
}
TEST_F(Dim6Test, doubleCover) {
    testGenericCases(TriangulationTest<6>::verifyDoubleCover);
}
TEST_F(Dim6Test, doubleOverBoundary) {
    testGenericCases(TriangulationTest<6>::verifyDoubleOverBoundary);
}
TEST_F(Dim6Test, makeCanonical) {
    testGenericCases(TriangulationTest<6>::verifyMakeCanonical);
}
TEST_F(Dim6Test, isomorphismSignature) {
    testGenericCases(TriangulationTest<6>::verifyIsomorphismSignature);
}
TEST_F(Dim6Test, pachner) {
    testGenericCases(TriangulationTest<6>::verifyPachner);
    TriangulationTest<6>::verifyPachnerSimplicial();
}
TEST_F(Dim6Test, tightEncoding) {
    testGenericCases(TriangulationTest<6>::verifyTightEncoding);
}
TEST_F(Dim6Test, homologyH1) {
    TriangulationTest<6>::homologyH1GenericCases();
}
TEST_F(Dim6Test, homologyH2) {
    TriangulationTest<6>::homologyH2GenericCases();
}
TEST_F(Dim6Test, homologyH3) {
    TriangulationTest<6>::homologyH3GenericCases();
}
TEST_F(Dim6Test, boundaryHomology) {
    TriangulationTest<6>::boundaryHomologyGenericCases();
}
TEST_F(Dim6Test, fundGroup) {
    TriangulationTest<6>::fundGroupGenericCases();
}
TEST_F(Dim6Test, copyMove) {
    testGenericCases(TriangulationTest<6>::verifyCopyMove);
}

/**
 * For very high dimensions, we skip:
 *
 * - Isomorphism-related routines (makeCanonical and isomorphismSignature),
 *   whose running times include a factor of (dim+1)! ;
 *
 * - Pachner moves, where thorough testing slows things down a lot in higher
 *   dimensions.  Note that we do already test non-standard odd and even
 *   dimensions via dim == 5,6 above.
 */

class Dim8Test : public TriangulationTest<8> {
};

TEST_F(Dim8Test, validity) {
    TriangulationTest<8>::validityGenericCases();
}
TEST_F(Dim8Test, connectivity) {
    TriangulationTest<8>::connectivityGenericCases();
}
TEST_F(Dim8Test, orientability) {
    TriangulationTest<8>::orientabilityGenericCases();
}
TEST_F(Dim8Test, eulerChar) {
    TriangulationTest<8>::eulerCharGenericCases();
}
TEST_F(Dim8Test, boundaryBasic) {
    TriangulationTest<8>::boundaryBasicGenericCases();
}
TEST_F(Dim8Test, vertexLinksBasic) {
    TriangulationTest<8>::vertexLinksBasicGenericCases();
}
TEST_F(Dim8Test, orient) {
    testGenericCases(TriangulationTest<8>::verifyOrient);
}
TEST_F(Dim8Test, skeleton) {
    testGenericCases(TriangulationTest<8>::verifySkeleton);
}
TEST_F(Dim8Test, edgeAccess) {
    TriangulationTest<8>::edgeAccess();
}
TEST_F(Dim8Test, reordering) {
    testGenericCases(TriangulationTest<8>::verifyReordering);
}
TEST_F(Dim8Test, doubleCover) {
    testGenericCases(TriangulationTest<8>::verifyDoubleCover);
}
TEST_F(Dim8Test, doubleOverBoundary) {
    testGenericCases(TriangulationTest<8>::verifyDoubleOverBoundary);
}
TEST_F(Dim8Test, tightEncoding) {
    testGenericCases(TriangulationTest<8>::verifyTightEncoding);
}
TEST_F(Dim8Test, homologyH1) {
    TriangulationTest<8>::homologyH1GenericCases();
}
TEST_F(Dim8Test, homologyH2) {
    TriangulationTest<8>::homologyH2GenericCases();
}
TEST_F(Dim8Test, homologyH3) {
    TriangulationTest<8>::homologyH3GenericCases();
}
TEST_F(Dim8Test, boundaryHomology) {
    TriangulationTest<8>::boundaryHomologyGenericCases();
}
TEST_F(Dim8Test, fundGroup) {
    TriangulationTest<8>::fundGroupGenericCases();
}
TEST_F(Dim8Test, copyMove) {
    testGenericCases(TriangulationTest<8>::verifyCopyMove);
}
