
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

#include "testsuite.h"

#include <cctype>
#include <cstdlib>
#include <iostream>
#include <map>
#include <cppunit/Test.h>
#include <cppunit/TestResult.h>
#include <cppunit/TextTestProgressListener.h>
#include "testsuite/dim3/testdim3.h"
#include "testsuite/dim4/testdim4.h"
#include "testsuite/link/testlink.h"
#include "testsuite/maths/testmaths.h"

namespace {
    static bool checkedParams = false;
    static bool useDetailedTests = false;
}

using TestSet = void (*)(CppUnit::TextUi::TestRunner& runner);

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

bool populateTests(CppUnit::TextTestRunner& runner, int argc, char* argv[]) {
    std::map<std::string, TestSet> sets;
    std::map<std::string, TestSet> aliases;

    // Maths:
    sets.insert(std::make_pair("integer", &addInteger));
    sets.insert(std::make_pair("rational", &addRational));

    // 3-manifold triangulations:
    sets.insert(std::make_pair("triangulation3", &addTriangulation3));
    aliases.insert(std::make_pair("dim3", &addTriangulation3));
    sets.insert(std::make_pair("elementarymoves", &addElementaryMoves));
    sets.insert(std::make_pair("connectedsumdecomp", &addConnectedSumDecomp));
    sets.insert(std::make_pair("isomorphism3", &addIsomorphism3));
    sets.insert(std::make_pair("linkingsurfaces", &addLinkingSurfaces));
    sets.insert(std::make_pair("homologicaldata", &addHomologicalData));

    // 4-manifold triangulations:
    sets.insert(std::make_pair("triangulation4", &addTriangulation4));
    aliases.insert(std::make_pair("dim4", &addTriangulation4));

    if (argc <= 1)
        for (const auto& i : sets)
            (i.second)(runner);
    else {
        for (int i = 1; i < argc; ++i) {
            auto it = sets.find(std::string(argv[i]));
            if (it != sets.end()) {
                (it->second)(runner);
            } else {
                auto alias = aliases.find(std::string(argv[i]));
                if (alias != aliases.end()) {
                    (alias->second)(runner);
                } else {
                    std::cerr << "ERROR: No test set named "
                        << argv[i] << '.' << std::endl;
                    return false;
                }
            }
        }
    }

    return true;
}

