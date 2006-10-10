
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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

#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "testsuite/utilities/testutilities.h"
#include "utilities/nrational.h"

using regina::NLargeInteger;
using regina::NRational;

class NRationalTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NRationalTest);

    CPPUNIT_TEST(doubleApprox);

    CPPUNIT_TEST_SUITE_END();

    public:
        static const double epsilon = 0.0000001;
            /** Used for determining whether a number is "close enough"
                to zero.  This helps avoid the inaccuracies inherent in
                using == with floating point numbers. */

    public:
        void setUp() {
        }

        void tearDown() {
        }

        void checkDoubleInRange(const NRational& r,
                double lowerBnd, double upperBnd, const char* name) {
            bool inRange;
            double ans = r.doubleApprox(&inRange);

            if (! inRange) {
                std::ostringstream msg;
                msg << "Rational " << name <<
                    " should be reported as within the range of double.";
                CPPUNIT_FAIL(msg.str());
            }

            if (ans < lowerBnd || ans > upperBnd) {
                std::ostringstream msg;
                msg << "Rational " << name <<
                    " converts to the double " << ans <<
                    ", which is outside the expected range [ "
                    << lowerBnd << ", " << upperBnd << " ].";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void checkDoubleOutOfRange(const NRational& r, const char* name) {
            bool inRange;
            double ans = r.doubleApprox(&inRange);

            if (inRange) {
                std::ostringstream msg;
                msg << "Rational " << name <<
                    " should be reported as outside the range of double.";
                CPPUNIT_FAIL(msg.str());
            }

            if (ans < -epsilon || ans > epsilon) {
                std::ostringstream msg;
                msg << "Rational " << name <<
                    " converts to the double " << ans <<
                    ", which should be zero to indicate out-of-range.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void doubleApprox() {
            checkDoubleOutOfRange(NRational::infinity, "infinity");
            checkDoubleOutOfRange(NRational::undefined, "undefined");

            checkDoubleInRange(NRational::zero, -epsilon, epsilon, "zero");
            checkDoubleInRange(NRational(5, 3), 1.666, 1.667, "5/3");
            checkDoubleInRange(NRational(-5, 3), -1.667, -1.666, "-5/3");

            // Construct something out of int's usual range but
            // well within double's.  Here we aim for around 2^70,
            // or about 7^25.
            NLargeInteger in(7);
            in.raiseToPower(25);
            checkDoubleInRange(NRational(in, 3), 4.470e+20, 4.471e+20,
                "7^25/3");
            checkDoubleInRange(NRational(-in, 3), -4.471e+20, -4.470e+20,
                "-7^25/3");

            // Construct something well out of double's usual range.
            // Here we aim for around 2^10000, or about 13^2702.
            NLargeInteger out(13);
            out.raiseToPower(2702);
            checkDoubleOutOfRange(NRational(out, 2), "13^2702/2");
            checkDoubleOutOfRange(NRational(-out, 2), "-13^2702/2");

            // Check precision bounds close to zero also.
            checkDoubleInRange(NRational(3, in), 2.237e-21, 2.238e-21,
                "3/7^25");
            checkDoubleInRange(NRational(-3, in), -2.238e-21, -2.237e-21,
                "-3/7^25");
            checkDoubleOutOfRange(NRational(2, out), "2/13^2702");
            checkDoubleOutOfRange(NRational(-2, out), "-2/13^2702");
        }
};

void addNRational(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NRationalTest::suite());
}

