
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

#include <string>

namespace CppUnit {
    class TextTestRunner;
}

/**
 * Populates the given test suite with individual tests for various
 * components of Regina.
 */
void populateTests(CppUnit::TextTestRunner& runner);

/**
 * Improves the readability of the test name "FIXTURE.TEST".
 */
std::string truncateFixture(const std::string& testName);

/**
 * Reads all optional test parameters from the environment, and
 * writes any relevant informative messages to std::cout.
 */
void checkTestParams();

/**
 * Has the user explicitly asked to run a more detailed (but
 * significantly slower) suite of tests?
 *
 * This is true if and only if the environment variable
 * REGINA_DETAILED_TESTS is defined and non-empty.
 *
 * The routine checkTestParams() must be called before this routine is used.
 */
bool detailedTests();

