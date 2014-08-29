
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

#include "regina-config.h" // For EXCLUDE_SNAPPEA

#include <cctype>
#include <iostream>
#include <cppunit/Test.h>
#include <cppunit/TestResult.h>
#include <cppunit/TextTestProgressListener.h>
#include "testsuite/testparams.h"
#include "testsuite/algebra/testalgebra.h"
#include "testsuite/angle/testangle.h"
#include "testsuite/census/testcensus.h"
#include "testsuite/dim2/testdim2.h"
#include "testsuite/maths/testmaths.h"
#ifndef EXCLUDE_SNAPPEA
#include "testsuite/snappea/testsnappea.h"
#endif
#include "testsuite/subcomplex/testsubcomplex.h"
#include "testsuite/surfaces/testsurfaces.h"
#include "testsuite/triangulation/testtriangulation.h"
#include "testsuite/utilities/testutilities.h"

/**
 * Improves the readability of the test name "FIXTURE.TEST".
 */
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

/**
 * Used for outputting progress.
 */
class ReginaProgress : public CppUnit::TextTestProgressListener {
    private:
        bool failed;

    public:
        ReginaProgress() : TextTestProgressListener(), failed(false) {
        }

        virtual void startTest(CppUnit::Test* test) {
            std::cout << truncateFixture(test->getName()) << "... ";
            std::cout.flush();
            failed = false;
        }

        virtual void addFailure(const CppUnit::TestFailure&) {
            if (! failed) {
                std::cout << "FAILED." << std::endl;
                failed = true;
            }
        }

        virtual void endTest(CppUnit::Test*) {
            if (! failed)
                std::cout << "ok." << std::endl;
        }
};

/**
 * Runs the entire test suite, outputting progress and results.
 */
bool runAllTests() {
    std::cout << "Regina calculation engine test suite\n\n" << std::flush;
    checkTestParams();

    CppUnit::TextUi::TestRunner runner;
    ReginaProgress progress;

    /**
     * BEGIN REGINA TEST SUITES
     *
     * Individual test suites for various components of Regina should
     * be added below.
     */

    // Utilities:
    addBase64(runner);
    addNBitmask(runner);

    // Maths:
    addNInteger(runner);
    addNRational(runner);
    addNPerm3(runner);
    addNPerm4(runner);
    addNPerm5(runner);
    addNPrimes(runner);
    addNumberTheory(runner);
    addMatrixOps(runner);
    addPermConv(runner);

    // Algebra:
    addNGroupPresentation(runner);

    // Dim2Triangulation:
    addDim2Triangulation(runner);

    // Triangulation:
    addNTriangulation(runner);
    addElementaryMoves(runner);
    addConnectedSumDecomp(runner);
    addNIsomorphism(runner);
    addNHomologicalData(runner);

    // Subcomplexes:
    addNStandardTriangulation(runner);

    // Surfaces:
    addNNormalSurfaceList(runner);
    addIncompressible(runner);

    // Angle structures:
    addNAngleStructureList(runner);

    // Census:
    addNCensus(runner);
    addNFacePairing(runner);
    addDim2Census(runner);
    addDim2EdgePairing(runner);

#ifndef EXCLUDE_SNAPPEA
    // SnapPea:
    addNSnapPeaTriangulation(runner);
#endif

    /**
     * END REGINA TEST SUITES
     */

    runner.eventManager().addListener(&progress);
    return runner.run("", false, true, false);
}

/**
 * The main function, if we're building a standalone program.
 */
#ifndef __NO_TESTSUITE_MAIN
int main(int argc, char* argv[]) {
    return (runAllTests() ? 0 : 1);
}
#endif

