
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
#include "dim4/nperm5.h"
#include "testsuite/dim4/testdim4.h"

using regina::NPerm5;

class NPerm5Test : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NPerm5Test);

    CPPUNIT_TEST(exhaustive);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp() {
        }

        void tearDown() {
        }

        bool looksLikeIdentity(const NPerm5& p) {
            return (p.isIdentity() && p == NPerm5() &&
                p.getPermCode() == 18056 && p.toString() == "01234");
        }

        bool looksEqual(const NPerm5& p, const NPerm5& q) {
            return (p == q && (! (p != q)) && p.toString() == q.toString() &&
                p.getPermCode() == q.getPermCode());
        }

        bool looksEqual(const NPerm5& p, const NPerm5& q,
                const std::string& qStr) {
            return (p == q && (! (p != q)) && p.toString() == q.toString() &&
                p.getPermCode() == q.getPermCode() && p.toString() == qStr);
        }

        bool looksDistinct(const NPerm5& p, const NPerm5& q) {
            return (p != q && (! (p == q)) && p.toString() != q.toString() &&
                p.getPermCode() != q.getPermCode());
        }

        int expectedSign(const NPerm5& p) {
            // Count the number of reorderings.
            int reorderings = 0;

            int a, b;
            for (a = 0; a < 5; ++a)
                for (b = a + 1; b < 5; ++b)
                    if (p[a] > p[b])
                        ++reorderings;

            return ((reorderings % 2 == 0) ? 1 : -1);
        }

        void testPerm(int a, int b, int c, int d, int e) {
            NPerm5 p(a, b, c, d, e);

            std::ostringstream name;
            name << a << b << c << d << e;

            NPerm5 p1(p.getPermCode());
            if (! looksEqual(p1, p, name.str())) {
                std::ostringstream msg;
                msg << "The internal code constructor fails for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            NPerm5 p2(3, d, 2, c, 4, e, 0, a, 1, b);
            if (! looksEqual(p2, p, name.str())) {
                std::ostringstream msg;
                msg << "The 10-argument constructor fails for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            NPerm5 p3(p);
            if (! looksEqual(p3, p, name.str())) {
                std::ostringstream msg;
                msg << "The copy constructor fails for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            NPerm5 p4(4, 2, 3, 0, 1);
            if (! (a == 4 && b == 2 && c == 3 && d == 0 && e == 1)) {
                if (! looksDistinct(p4, p)) {
                    std::ostringstream msg;
                    msg << "The equality/inequality tests fail for "
                        "the permutations 42301 and " << name.str() << ".";
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

            NPerm5 p5(4, 2, 3, 0, 1);
            p5.setPermCode(p3.getPermCode());
            if (! looksEqual(p5, p, name.str())) {
                std::ostringstream msg;
                msg << "The setPermCode() / getPermCode() routines fail for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (! NPerm5::isPermCode(p.getPermCode())) {
                std::ostringstream msg;
                msg << "Routine isPermCode() suggests that the permutation "
                    << name.str() << " has an invalid permutation code.";
                CPPUNIT_FAIL(msg.str());
            }

            if (NPerm5::isPermCode(0))
                CPPUNIT_FAIL("Routine isPermCode() suggests that 0 is a "
                    "valid permutation code (which it is not).");

            if (! looksEqual(p * NPerm5(), p)) {
                std::ostringstream msg;
                msg << "Multiplying permutation " << name.str()
                    << " by the identity does not give " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(NPerm5() * p, p)) {
                std::ostringstream msg;
                msg << "Multiplying the identity by permutation " << name.str()
                    << " does not give " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(p * NPerm5(0, 1), NPerm5(b, a, c, d, e))) {
                std::ostringstream msg;
                msg << "Multiplying permutation " << name.str()
                    << " by (0 <--> 1) does not give the expected result.";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(p * NPerm5(1, 2), NPerm5(a, c, b, d, e))) {
                std::ostringstream msg;
                msg << "Multiplying permutation " << name.str()
                    << " by (1 <--> 2) does not give the expected result.";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(p * NPerm5(2, 3), NPerm5(a, b, d, c, e))) {
                std::ostringstream msg;
                msg << "Multiplying permutation " << name.str()
                    << " by (2 <--> 3) does not give the expected result.";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(p * NPerm5(3, 4), NPerm5(a, b, c, e, d))) {
                std::ostringstream msg;
                msg << "Multiplying permutation " << name.str()
                    << " by (3 <--> 4) does not give the expected result.";
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

            NPerm5 inv = p.inverse();
            if (inv[a] != 0 || inv[b] != 1 || inv[c] != 2 ||
                    inv[d] != 3 || inv[e] != 4) {
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

            if (p[0] != a || p[1] != b || p[2] != c || p[3] != d || p[4] != e) {
                std::ostringstream msg;
                msg << "The element images for permutation " << name.str()
                    << " do not appear to be correct.";
                CPPUNIT_FAIL(msg.str());
            }

            if (p.preImageOf(a) != 0 || p.preImageOf(b) != 1 ||
                    p.preImageOf(c) != 2 || p.preImageOf(d) != 3 ||
                    p.preImageOf(e) != 4) {
                std::ostringstream msg;
                msg << "The element preimages for permutation " << name.str()
                    << " do not appear to be correct.";
                CPPUNIT_FAIL(msg.str());
            }

            if (a != 0 || b != 1 || c != 2 || d != 3 || e != 4) {
                NPerm5 id;
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

            if (a != 4 || b != 3 || c != 2 || d != 1 || e != 0) {
                NPerm5 last(4, 3, 2, 1, 0);
                if (p.compareWith(last) != -1 || last.compareWith(p) != 1) {
                    std::ostringstream msg;
                    msg << "Permutation " << name.str()
                        << " is not reported to be lexicographically "
                           "smaller than 43210.";
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
            NPerm5 id;
            if (! looksLikeIdentity(id))
                CPPUNIT_FAIL("The default NPerm5 constructor does not "
                    "appear to give the identity permutation.");

            for (int i = 0; i < 5; ++i) {
                NPerm5 p(i, i);
                if (! looksLikeIdentity(p)) {
                    std::ostringstream msg;
                    msg << "The permutation that swaps " << i
                        << " with itself does not appear to be the identity.";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            // Test all possible permutations.
            int tested = 0;
            int a, b, c, d, e;
            for (a = 0; a < 5; ++a) {
                for (b = 0; b < 5; ++b) {
                    if (b == a)
                        continue;
                    for (c = 0; c < 5; ++c) {
                        if (c == a || c == b)
                            continue;
                        for (d = 0; d < 5; ++d) {
                            if (d == a || d == b || d == c)
                                continue;
                            e = (10 - a - b - c - d);
                            testPerm(a, b, c, d, e);
                            ++tested;
                        }
                    }
                }
            }

            if (tested != 120)
                CPPUNIT_FAIL("All 120 permutations in S(5) were not tested.");
        }
};

void addNPerm5(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NPerm5Test::suite());
}

