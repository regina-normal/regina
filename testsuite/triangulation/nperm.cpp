
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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
#include "triangulation/nperm.h"
#include "testsuite/triangulation/testtriangulation.h"

using regina::NPerm;

class NPermTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NPermTest);

    CPPUNIT_TEST(inverse);
    CPPUNIT_TEST(sign);
    CPPUNIT_TEST(exhaustive);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp() {
        }

        void tearDown() {
        }

        void inverse() {
            for (int i = 0; i < 24; i++) {
                if (regina::allPermsS4[i].inverse() !=
                        regina::allPermsS4[regina::allPermsS4Inv[i]]) {
                    std::ostringstream msg;
                    msg << "Permutation #" << i << " was found to have "
                        "inverse " << regina::allPermsS4[i].inverse()
                        << " instead of " <<
                        regina::allPermsS4[regina::allPermsS4Inv[i]] << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void sign() {
            int expected;
            for (int i = 0; i < 24; i++) {
                expected = (i % 2 == 0 ? 1 : -1);
                if (regina::allPermsS4[i].sign() != expected) {
                    std::ostringstream msg;
                    msg << "Permutation #" << i << " was found to have "
                        "sign " << regina::allPermsS4[i].sign()
                        << " instead of " << expected << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        bool looksLikeIdentity(const NPerm& p) {
            return (p.isIdentity() && p == NPerm() &&
                p.getPermCode() == 228 && p.toString() == "0123");
        }

        bool looksEqual(const NPerm& p, const NPerm& q) {
            return (p == q && (! (p != q)) && p.toString() == q.toString() &&
                p.getPermCode() == q.getPermCode());
        }

        bool looksEqual(const NPerm& p, const NPerm& q,
                const std::string& qStr) {
            return (p == q && (! (p != q)) && p.toString() == q.toString() &&
                p.getPermCode() == q.getPermCode() && p.toString() == qStr);
        }

        bool looksDistinct(const NPerm& p, const NPerm& q) {
            return (p != q && (! (p == q)) && p.toString() != q.toString() &&
                p.getPermCode() != q.getPermCode());
        }

        int expectedSign(const NPerm& p) {
            // Count the number of reorderings.
            int reorderings = 0;

            int a, b;
            for (a = 0; a < 4; ++a)
                for (b = a + 1; b < 4; ++b)
                    if (p[a] > p[b])
                        ++reorderings;

            return ((reorderings % 2 == 0) ? 1 : -1);
        }

        void testPerm(int a, int b, int c, int d) {
            NPerm p(a, b, c, d);

            std::ostringstream name;
            name << a << b << c << d;

            NPerm p1(p.getPermCode());
            if (! looksEqual(p1, p, name.str())) {
                std::ostringstream msg;
                msg << "The internal code constructor fails for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            NPerm p2(3, d, 2, c, 0, a, 1, b);
            if (! looksEqual(p2, p, name.str())) {
                std::ostringstream msg;
                msg << "The 8-argument constructor fails for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            NPerm p3(p);
            if (! looksEqual(p3, p, name.str())) {
                std::ostringstream msg;
                msg << "The copy constructor fails for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            NPerm p4(2, 3, 1, 0);
            if (! (a == 2 && b == 3 && c == 1 && d == 0)) {
                if (! looksDistinct(p4, p)) {
                    std::ostringstream msg;
                    msg << "The equality/inequality tests fail for "
                        "the permutations 2310 and " << name.str() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            p4 = p;
            if (! looksEqual(p4, p, name.str())) {
                std::ostringstream msg;
                msg << "The assignment operator fails for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            NPerm p5(2, 3, 1, 0);
            p5.setPermCode(p3.getPermCode());
            if (! looksEqual(p5, p, name.str())) {
                std::ostringstream msg;
                msg << "The setPermCode() / getPermCode() routines fail for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (! NPerm::isPermCode(p.getPermCode())) {
                std::ostringstream msg;
                msg << "Routine isPermCode() suggests that the permutation "
                    << name.str() << " has an invalid permutation code.";
                CPPUNIT_FAIL(msg.str());
            }

            if (NPerm::isPermCode(0))
                CPPUNIT_FAIL("Routine isPermCode() suggests that 0 is a "
                    "valid permutation code (which it is not).");

            if (! looksEqual(p * NPerm(), p)) {
                std::ostringstream msg;
                msg << "Multiplying permutation " << name.str()
                    << " by the identity does not give " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(NPerm() * p, p)) {
                std::ostringstream msg;
                msg << "Multiplying the identity by permutation " << name.str()
                    << " does not give " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(p * NPerm(0, 1), NPerm(b, a, c, d))) {
                std::ostringstream msg;
                msg << "Multiplying permutation " << name.str()
                    << " by (0 <--> 1) does not give the expected result.";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(p * NPerm(1, 2), NPerm(a, c, b, d))) {
                std::ostringstream msg;
                msg << "Multiplying permutation " << name.str()
                    << " by (1 <--> 2) does not give the expected result.";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(p * NPerm(2, 3), NPerm(a, b, d, c))) {
                std::ostringstream msg;
                msg << "Multiplying permutation " << name.str()
                    << " by (2 <--> 3) does not give the expected result.";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksLikeIdentity(p * p.inverse())) {
                std::ostringstream msg;
                msg << "Multiplying permutation " << name.str()
                    << " by its inverse does not give the identity.";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksLikeIdentity(p.inverse() * p)) {
                std::ostringstream msg;
                msg << "Multiplying the inverse of permutation " << name.str()
                    << " by the permutation itself does not give the identity.";
                CPPUNIT_FAIL(msg.str());
            }

            NPerm inv = p.inverse();
            if (inv[a] != 0 || inv[b] != 1 || inv[c] != 2 || inv[d] != 3) {
                std::ostringstream msg;
                msg << "The inverse of permutation " << name.str()
                    << " does not appear to be correct.";
                CPPUNIT_FAIL(msg.str());
            }

            if (p.sign() != expectedSign(p)) {
                // Bah.  Just call expectedSign() again.
                std::ostringstream msg;
                msg << "The sign of permutation " << name.str()
                    << " was not " << expectedSign(p) << " as expected.";
                CPPUNIT_FAIL(msg.str());
            }

            if (p[0] != a || p[1] != b || p[2] != c || p[3] != d) {
                std::ostringstream msg;
                msg << "The element images for permutation " << name.str()
                    << " do not appear to be correct.";
                CPPUNIT_FAIL(msg.str());
            }

            if (p.preImageOf(a) != 0 || p.preImageOf(b) != 1 ||
                    p.preImageOf(c) != 2 || p.preImageOf(d) != 3) {
                std::ostringstream msg;
                msg << "The element preimages for permutation " << name.str()
                    << " do not appear to be correct.";
                CPPUNIT_FAIL(msg.str());
            }

            if (a != 0 || b != 1 || c != 2 || d != 3) {
                NPerm id;
                if (p.compareWith(id) != 1 || id.compareWith(p) != -1) {
                    std::ostringstream msg;
                    msg << "Permutation " << name.str()
                        << " is not reported to be lexicographically "
                           "larger than the identity permutation.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (p.isIdentity()) {
                    std::ostringstream msg;
                    msg << "Permutation " << name.str()
                        << " is reported to be the identity permutation.";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            if (a != 3 || b != 2 || c != 1 || d != 0) {
                NPerm last(3, 2, 1, 0);
                if (p.compareWith(last) != -1 || last.compareWith(p) != 1) {
                    std::ostringstream msg;
                    msg << "Permutation " << name.str()
                        << " is not reported to be lexicographically "
                           "smaller than 3210.";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            if (p.compareWith(p) != 0) {
                std::ostringstream msg;
                msg << "Permutation " << name.str()
                    << " is not reported to be lexicographically "
                       "identical to itself.";
                CPPUNIT_FAIL(msg.str());
            }

            if (p.toString() != name.str()) {
                std::ostringstream msg;
                msg << "The stringification for permutation " << name.str()
                    << " does not appear to be correct.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void exhaustive() {
            NPerm id;
            if (! looksLikeIdentity(id))
                CPPUNIT_FAIL("The default NPerm constructor does not "
                    "appear to give the identity permutation.");

            for (int i = 0; i < 4; ++i) {
                NPerm p(i, i);
                if (! looksLikeIdentity(p)) {
                    std::ostringstream msg;
                    msg << "The permutation that swaps " << i
                        << " with itself does not appear to be the identity.";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            // Test all possible permutations.
            int tested = 0;
            int a, b, c, d;
            for (a = 0; a < 4; ++a) {
                for (b = 0; b < 4; ++b) {
                    if (b == a)
                        continue;
                    for (c = 0; c < 4; ++c) {
                        if (c == a || c == b)
                            continue;
                        d = (6 - a - b - c);
                        testPerm(a, b, c, d);
                        ++tested;
                    }
                }
            }

            if (tested != 24)
                CPPUNIT_FAIL("All 24 permutations in S(4) were not tested.");
        }
};

void addNPerm(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NPermTest::suite());
}

