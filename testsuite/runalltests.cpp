
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include <iostream>
#include <cppunit/Test.h>
#include <cppunit/TestResult.h>
#include <cppunit/TextTestProgressListener.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

/**
 * Removes "FIXTURE." from "FIXTURE.TEST" since the fixture name
 * seems to always be "ATestFixtureType" unless RTTI is used.
 */
std::string truncateFixture(const std::string& testName) {
    std::string::size_type pos = testName.find('.');

    // Don't truncate if the first period is right at the end.
    if (pos + 1 < testName.length())
        return testName.substr(pos + 1, testName.length() - pos - 1);
    else
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

    CppUnit::TextUi::TestRunner runner;
    ReginaProgress progress;

    runner.addTest(CppUnit::TestFactoryRegistry::getRegistry().makeTest());
    runner.eventManager().addListener(&progress);
    return runner.run("", false, true, false);
}

/**
 * The main function, if we're building a standalone program.
 */
#ifndef __NO_TESTSUITE_MAIN
int main(int argc, char* argv[]) {
    return runAllTests();
}
#endif

