
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

#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "maths/nrational.h"
#include "utilities/stringutils.h"
#include "testsuite/utilities/testutilities.h"

#define HUGE_INTEGER "12364981726394781629378461923786491874569283746672"

using regina::NInteger;
using regina::NIntegerBase;
using regina::NLargeInteger;
using regina::NRational;

class NRationalTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NRationalTest);

    CPPUNIT_TEST(constructFromInteger);
    CPPUNIT_TEST(constructInfinity);
    CPPUNIT_TEST(constructUndefined);
    CPPUNIT_TEST(assignFromInteger);
    CPPUNIT_TEST(extractToNInteger);
    CPPUNIT_TEST(doubleApprox);

    CPPUNIT_TEST_SUITE_END();

    public:
        static const double epsilon;
            /** Used for determining whether a number is "close enough"
                to zero.  This helps avoid the inaccuracies inherent in
                using == with floating point numbers.  See the bottom
                of this file for its definition. */

    public:
        void setUp() {
        }

        void tearDown() {
        }

        template <typename T>
        std::string str(T x) {
            std::ostringstream ans;
            ans << x;
            return ans.str();
        }

        void verifyConstructFinite(long val) {
            NRational r(val);
            if (r.numerator() != val) {
                std::ostringstream msg;
                msg << "Rational (long)" << val <<
                    " reports an incorrect numerator.";
                CPPUNIT_FAIL(msg.str());
            }
            if (r.denominator() != 1) {
                std::ostringstream msg;
                msg << "Rational (long)" << val <<
                    " reports an incorrect denominator.";
                CPPUNIT_FAIL(msg.str());
            }
            if (str(r) != str(val)) {
                std::ostringstream msg;
                msg << "Rational (long)" << val <<
                    " reports an incorrect string.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        template <bool supportInfinity>
        void verifyConstructFinite(const NIntegerBase<supportInfinity>& val) {
            NRational r(val);
            if (r.numerator() != val) {
                std::ostringstream msg;
                msg << "Rational NIntegerBase<...>" << val <<
                    " reports an incorrect numerator.";
                CPPUNIT_FAIL(msg.str());
            }
            if (r.denominator() != 1) {
                std::ostringstream msg;
                msg << "Rational NIntegerBase<...>" << val <<
                    " reports an incorrect denominator.";
                CPPUNIT_FAIL(msg.str());
            }
            if (str(r) != val.stringValue()) {
                std::ostringstream msg;
                msg << "Rational NIntegerBase<...>" << val <<
                    " reports an incorrect string.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void constructFromInteger() {
            verifyConstructFinite(-1);
            verifyConstructFinite(0);
            verifyConstructFinite(1);
            verifyConstructFinite(LONG_MAX);
            verifyConstructFinite(LONG_MIN);
            verifyConstructFinite(NInteger(LONG_MAX) + 1);
            verifyConstructFinite(NLargeInteger(LONG_MAX) + 1);
            verifyConstructFinite(NInteger(LONG_MIN) - 1);
            verifyConstructFinite(NLargeInteger(LONG_MIN) - 1);
            verifyConstructFinite(NInteger(HUGE_INTEGER));
            verifyConstructFinite(NLargeInteger(HUGE_INTEGER));
            verifyConstructFinite(NInteger("-" HUGE_INTEGER));
            verifyConstructFinite(NLargeInteger("-" HUGE_INTEGER));
        }

        void verifyInfinite(const NRational& r, const char* name) {
            if (r.numerator() != 1) {
                std::ostringstream msg;
                msg << "Rational " << name <<
                    " is not correctly set to infinity.";
                CPPUNIT_FAIL(msg.str());
            }
            if (r.denominator() != 0) {
                std::ostringstream msg;
                msg << "Rational " << name <<
                    " is not correctly set to infinity.";
                CPPUNIT_FAIL(msg.str());
            }
            if (str(r) != "Inf") {
                std::ostringstream msg;
                msg << "Rational " << name <<
                    " is not correctly set to infinity.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void constructInfinity() {
            verifyInfinite(NRational(1, 0), "1/0");
            verifyInfinite(NRational(-1, 0), "-1/0");
            verifyInfinite(NRational(3, 0), "3/0");
            verifyInfinite(NRational(-2, 0), "-2/0");
            verifyInfinite(NRational(LONG_MAX, 0), "LONG_MAX/0");
            verifyInfinite(NRational(LONG_MIN, 0), "LONG_MIN/0");
            verifyInfinite(NRational(NInteger(LONG_MAX) + 1,
                NInteger::zero),
                "(NInteger(LONG_MAX)+1)/0");
            verifyInfinite(NRational(NLargeInteger(LONG_MAX) + 1,
                NLargeInteger::zero),
                "(NLargeInteger(LONG_MAX)+1)/0");
            verifyInfinite(NRational(NInteger(LONG_MIN) - 1,
                NInteger::zero),
                "(NInteger(LONG_MIN)-1)/0");
            verifyInfinite(NRational(NLargeInteger(LONG_MIN) - 1,
                NLargeInteger::zero),
                "(NLargeInteger(LONG_MIN)-1)/0");
            verifyInfinite(NRational(NLargeInteger::infinity),
                "NRational(NLargeInteger::infinity)");
        }

        void verifyUndefined(const NRational& r, const char* name) {
            if (r.numerator() != 0) {
                std::ostringstream msg;
                msg << "Rational " << name <<
                    " is not correctly set to undefined.";
                CPPUNIT_FAIL(msg.str());
            }
            if (r.denominator() != 0) {
                std::ostringstream msg;
                msg << "Rational " << name <<
                    " is not correctly set to undefined.";
                CPPUNIT_FAIL(msg.str());
            }
            if (str(r) != "Undef") {
                std::ostringstream msg;
                msg << "Rational " << name <<
                    " is not correctly set to undefined.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void constructUndefined() {
            verifyUndefined(NRational(0, 0), "0/0");
            verifyUndefined(NRational(NInteger::zero, NInteger::zero),
                "NInteger::zero/NInteger::zero");
            verifyUndefined(NRational(NLargeInteger::zero,
                NLargeInteger::zero),
                "NLargeInteger::zero/NLargeInteger::zero");
        }

        void verifyAssignFinite(long val) {
            NRational r = 6;
            r = val;
            if (r.numerator() != val) {
                std::ostringstream msg;
                msg << "Rational assigned to (long)" << val <<
                    " reports an incorrect numerator.";
                CPPUNIT_FAIL(msg.str());
            }
            if (r.denominator() != 1) {
                std::ostringstream msg;
                msg << "Rational assigned to (long)" << val <<
                    " reports an incorrect denominator.";
                CPPUNIT_FAIL(msg.str());
            }
            if (str(r) != str(val)) {
                std::ostringstream msg;
                msg << "Rational assigned to (long)" << val <<
                    " reports an incorrect string.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        template <bool supportInfinity>
        void verifyAssignFinite(const NIntegerBase<supportInfinity>& val) {
            NRational r = 6;
            r = val;
            if (r.numerator() != val) {
                std::ostringstream msg;
                msg << "Rational assigned to NIntegerBase<...>" << val <<
                    " reports an incorrect numerator.";
                CPPUNIT_FAIL(msg.str());
            }
            if (r.denominator() != 1) {
                std::ostringstream msg;
                msg << "Rational assigned to NIntegerBase<...>" << val <<
                    " reports an incorrect denominator.";
                CPPUNIT_FAIL(msg.str());
            }
            std::ostringstream out;
            out << r;
            if (out.str() != val.stringValue()) {
                std::ostringstream msg;
                msg << "Rational assigned to NIntegerBase<...>" << val <<
                    " reports an incorrect string.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void assignFromInteger() {
            verifyAssignFinite(-1);
            verifyAssignFinite(0);
            verifyAssignFinite(1);
            verifyAssignFinite(LONG_MAX);
            verifyAssignFinite(LONG_MIN);
            verifyAssignFinite(NInteger(LONG_MAX) + 1);
            verifyAssignFinite(NLargeInteger(LONG_MAX) + 1);
            verifyAssignFinite(NInteger(LONG_MIN) - 1);
            verifyAssignFinite(NLargeInteger(LONG_MIN) - 1);
            verifyAssignFinite(NInteger(HUGE_INTEGER));
            verifyAssignFinite(NLargeInteger(HUGE_INTEGER));
            verifyAssignFinite(NInteger("-" HUGE_INTEGER));
            verifyAssignFinite(NLargeInteger("-" HUGE_INTEGER));

            NRational r = 0;
            r = NLargeInteger::infinity;
            verifyInfinite(r, "NRational = NLargeInteger::infinity");
        }

        void verifyNumDen(long val) {
            NRational r(val);
            if (r.numerator() != val) {
                std::ostringstream msg;
                msg << "Rational (long)" << val <<
                    " reports an incorrect numerator.";
                CPPUNIT_FAIL(msg.str());
            }
            if (r.denominator() != 1) {
                std::ostringstream msg;
                msg << "Rational (long)" << val <<
                    " reports an incorrect denominator.";
                CPPUNIT_FAIL(msg.str());
            }

            r.invert();
            if (r.numerator() != (val >= 0 ? 1 : -1)) {
                std::ostringstream msg;
                msg << "Rational (long)" << val <<
                    " inverted reports an incorrect numerator.";
                CPPUNIT_FAIL(msg.str());
            }
            // Cast to NInteger because -LONG_MIN does not fit into a long.
            if (r.denominator() != (val >= 0 ?
                    NInteger(val) : -NInteger(val))) {
                std::ostringstream msg;
                msg << "Rational (long)" << val <<
                    " inverted reports an incorrect denominator.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        template <bool supportInfinity>
        void verifyNumDen(const NIntegerBase<supportInfinity>& val) {
            NRational r(val);
            if (r.numerator() != val) {
                std::ostringstream msg;
                msg << "Rational (NIntegerBase<...>)" << val <<
                    " reports an incorrect numerator.";
                CPPUNIT_FAIL(msg.str());
            }
            if (r.denominator() != 1) {
                std::ostringstream msg;
                msg << "Rational (NIntegerBase<...>)" << val <<
                    " reports an incorrect denominator.";
                CPPUNIT_FAIL(msg.str());
            }

            r.invert();
            if (r.numerator() != (val >= 0 ? 1 : -1)) {
                std::ostringstream msg;
                msg << "Rational (NIntegerBase<...>)" << val <<
                    " inverted reports an incorrect numerator.";
                CPPUNIT_FAIL(msg.str());
            }
            if (r.denominator() != (val >= 0 ? val : -val)) {
                std::ostringstream msg;
                msg << "Rational (NIntegerBase<...>)" << val <<
                    " inverted reports an incorrect denominator.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void extractToNInteger() {
            verifyNumDen(-1);
            verifyNumDen(0);
            verifyNumDen(1);
            verifyNumDen(LONG_MAX);
            verifyNumDen(LONG_MIN);
            verifyNumDen(NInteger(LONG_MAX) + 1);
            verifyNumDen(NLargeInteger(LONG_MAX) + 1);
            verifyNumDen(NInteger(LONG_MIN) - 1);
            verifyNumDen(NLargeInteger(LONG_MIN) - 1);
            verifyNumDen(NInteger(HUGE_INTEGER));
            verifyNumDen(NLargeInteger(HUGE_INTEGER));
            verifyNumDen(NInteger("-" HUGE_INTEGER));
            verifyNumDen(NLargeInteger("-" HUGE_INTEGER));
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
            checkDoubleInRange(NRational(in, NLargeInteger(3)),
                4.470e+20, 4.471e+20, "7^25/3");
            checkDoubleInRange(NRational(-in, NLargeInteger(3)),
                -4.471e+20, -4.470e+20, "-7^25/3");

            // Construct something well out of double's usual range.
            // Here we aim for around 2^10000, or about 13^2702.
            NLargeInteger out(13);
            out.raiseToPower(2702);
            checkDoubleOutOfRange(NRational(out, NLargeInteger(2)),
                "13^2702/2");
            checkDoubleOutOfRange(NRational(-out, NLargeInteger(2)),
                "-13^2702/2");

            // Check precision bounds close to zero also.
            checkDoubleInRange(NRational(NLargeInteger(3), in),
                2.237e-21, 2.238e-21, "3/7^25");
            checkDoubleInRange(NRational(NLargeInteger(-3), in),
                -2.238e-21, -2.237e-21, "-3/7^25");
            checkDoubleOutOfRange(NRational(NLargeInteger(2), out),
                "2/13^2702");
            checkDoubleOutOfRange(NRational(NLargeInteger(-2), out),
                "-2/13^2702");
        }
};

const double NRationalTest::epsilon = 0.0000001;

void addNRational(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NRationalTest::suite());
}

