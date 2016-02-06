
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
#include <iostream>
#include <cppunit/Test.h>
#include <cppunit/TestResult.h>
#include <cppunit/TextTestProgressListener.h>
#include <cppunit/TextTestRunner.h>

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

    CppUnit::TextTestRunner runner;
    ReginaProgress progress;

    populateTests(runner);

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

