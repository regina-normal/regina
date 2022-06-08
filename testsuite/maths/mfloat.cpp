
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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
#ifdef __REGINA_MFLOAT

#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "maths/mfloat.h"
#include "utilities/exception.h"
#include "utilities/stringutils.h"
#include "testsuite/utilities/testutilities.h"

using regina::MFloat;

class MFloatTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(MFloatTest);

    CPPUNIT_TEST(constructFromInteger);
	CPPUNIT_TEST(constructFromDouble);
	CPPUNIT_TEST(verifyAssignments);
	CPPUNIT_TEST(basicArithmetic);

    CPPUNIT_TEST_SUITE_END();

    public:
        static constexpr double epsilon = 0.0000001;
            /** Used for determining whether a number is "close enough"
                to zero.  This helps avoid the inaccuracies inherent in
                using == with floating point numbers. */

    public:
        void setUp() override {
        }

        void tearDown() override {
        }

        template <typename T>
        std::string str(T x) {
            std::ostringstream ans;
            ans << x;
            return ans.str();
        }

        bool inEpsilon(double d1,double d2) {
			return d1-d2<epsilon && d2-d1 <epsilon;
		}

        void verifyConstructi(unsigned long val) {
            MFloat num(val);
            if (!inEpsilon(num.getDouble(),val)) {
                std::ostringstream msg;
                msg << "MFloat " << val <<
                    " incorrectly assigned from ul.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void verifyConstructd(double val) {
            MFloat num(val);
            if (!inEpsilon(num.getDouble(),val)) {
                std::ostringstream msg;
                msg << "MFloat " << val <<
                    " incorrectly assigned from ul.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void constructFromInteger() {
            verifyConstructi(-1);
            verifyConstructi(0);
            verifyConstructi(1);
        }

        void constructFromDouble() {
            verifyConstructd(-1);
            verifyConstructd(0);
            verifyConstructd(1);
        }

		void verifyAssignments() {
			MFloat a;
			a = (double) 1;
			MFloat b = a;
			b = (double) 2;
			MFloat c(double(3));
			MFloat d(c);
			d = (double) 4;


			if (!inEpsilon(a.getDouble(),1)
				&& !inEpsilon(b.getDouble(),2)
				&& !inEpsilon(c.getDouble(),4)
				&& !inEpsilon(d.getDouble(),4)
			) {
                std::ostringstream msg;
                msg << "Incorrect MFloat assignment.";
                CPPUNIT_FAIL(msg.str());
            }
		}

        void verifyArithmetic(double v1, double v2) {
            MFloat num1(v1);
			MFloat num2(v2);

			MFloat sum(v1+v2);
			MFloat diff(v1-v2);
			MFloat prod(v1*v2);
			MFloat div(v1/v2);

			MFloat tmp((double)0);
			tmp = num1;
			tmp += num2;
            if (!inEpsilon(sum.getDouble(),tmp.getDouble())) {
                std::ostringstream msg;
                msg << "Incorrect MFloat sum " << sum.getDouble() <<
                    ".";
                CPPUNIT_FAIL(msg.str());
            }

            tmp = num1;
			tmp -= num2;
            if (!inEpsilon(diff.getDouble(),tmp.getDouble())) {
                std::ostringstream msg;
                msg << "Incorrect MFloat diff " << diff.getDouble() <<
                    ".";
                CPPUNIT_FAIL(msg.str());
            }

            tmp = num1;
			tmp *= num2;
            if (!inEpsilon(prod.getDouble(),tmp.getDouble())) {
                std::ostringstream msg;
                msg << "Incorrect MFloat prod " << prod.getDouble() <<
                    ".";
                CPPUNIT_FAIL(msg.str());
            }

            tmp = num1;
			tmp /= num2;
            if (!inEpsilon(div.getDouble(),tmp.getDouble())) {
                std::ostringstream msg;
                msg << "Incorrect MFloat div " << div.getDouble() <<
                    ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void basicArithmetic() {
			verifyArithmetic(1,1);
			verifyArithmetic(-1,1);
			verifyArithmetic(1,-1);
			verifyArithmetic(0,1);
		}

};
#endif
