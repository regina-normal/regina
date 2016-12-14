
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

#include "testsuite.h"

#include <cctype>
#include <cstdlib>
#include <iostream>
#include <cppunit/Test.h>
#include <cppunit/TestResult.h>
#include <cppunit/TextTestProgressListener.h>
#include "testsuite/algebra/testalgebra.h"
#include "testsuite/angle/testangle.h"
#include "testsuite/census/testcensus.h"
#include "testsuite/dim2/testdim2.h"
#include "testsuite/dim3/testtriangulation.h"
#include "testsuite/dim4/testdim4.h"
#include "testsuite/generic/testgeneric.h"
#include "testsuite/maths/testmaths.h"
#include "testsuite/snappea/testsnappea.h"
#include "testsuite/subcomplex/testsubcomplex.h"
#include "testsuite/surfaces/testsurfaces.h"
#include "testsuite/utilities/testutilities.h"

namespace {
    static bool checkedParams = false;
    static bool useDetailedTests = false;
}

void checkTestParams() {
    if (checkedParams)
        return;

    char* env = getenv("REGINA_DETAILED_TESTS");
    if (env && *env) {
        std::cout << "Running the detailed (but slow) test suite."
            << std::endl;
        std::cout << "To disable this, unset the "
            "REGINA_DETAILED_TESTS environment variable."
            << std::endl << std::endl;
        useDetailedTests = true;
    }

    checkedParams = true;
}

bool detailedTests() {
    // Just in case.
    checkTestParams();

    return useDetailedTests;
}

std::string truncateFixture(const std::string& testName) {
    static const std::string genericFixturePrefix("ATestFixtureType.");
    static const unsigned genericFixtureLen(genericFixturePrefix.length());

    unsigned len = testName.length();

    // Remove the fixture type altogether if it's the generic type.
    if (len > genericFixtureLen)
        if (testName.substr(0, genericFixtureLen) == genericFixturePrefix)
            return testName.substr(genericFixtureLen, len - genericFixtureLen);

    // Otherwise prune any leading digits from the fixture name.
    unsigned pos = 0;
    while (pos < len && isdigit(testName[pos]))
        pos++;
    if (pos > 0 && pos < len)
        return testName.substr(pos, len - pos);

    // Otherwise don't modify anything.
    return testName;
}

void populateTests(CppUnit::TextTestRunner& runner) {
    // Utilities:
    addBase64(runner);
    addBitmask(runner);

    // Maths:
    addInteger(runner);
    addRational(runner);
    addPerm2(runner);
    addPerm3(runner);
    addPerm4(runner);
    addPerm5(runner);
    addPerm(runner);
    addPrimes(runner);
    addNumberTheory(runner);
    addMatrixOps(runner);
    addPermConv(runner);

    // Algebra:
    addGroupPresentation(runner);

    // 2-manifold triangulations:
    addTriangulation2(runner);

    // 3-manifold triangulations:
    addTriangulation3(runner);
    addElementaryMoves(runner);
    addConnectedSumDecomp(runner);
    addIsomorphism3(runner);
    addHomologicalData(runner);

    // 4-manifold triangulations:
    addTriangulation4(runner);

    // Generic triangulations:
    addFaceNumbering(runner);
    addGenericTriangulation(runner);

    // Subcomplexes:
    addStandardTriangulation(runner);

    // Surfaces:
    addNormalSurfaces(runner);
    addIncompressible(runner);

    // Angle structures:
    addAngleStructures(runner);

    // Census:
    addCensus3(runner);
    addFacetPairing3(runner);
    addCensus2(runner);
    addFacetPairing2(runner);
    addCensus4(runner);
    addFacetPairing4(runner);

    // SnapPea:
    addSnapPeaTriangulation(runner);
}

