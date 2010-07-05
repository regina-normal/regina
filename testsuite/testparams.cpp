
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

#include "testparams.h"
#include <cstdlib>
#include <iostream>

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

