
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

#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "maths/nperm4.h"
#include "testsuite/triangulation/testtriangulation.h"

using regina::NPerm4;

class NPerm4Test : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NPerm4Test);

    CPPUNIT_TEST(inverse);
    CPPUNIT_TEST(sign);
    CPPUNIT_TEST(index);
    CPPUNIT_TEST(exhaustive);
    CPPUNIT_TEST(products);
    CPPUNIT_TEST(compareWith);
    CPPUNIT_TEST(deprecatedArrays);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp() {
        }

        void tearDown() {
        }

        void inverse() {
            for (int i = 0; i < 24; i++) {
                if (NPerm4::S4[i].inverse() != NPerm4::S4[NPerm4::invS4[i]]) {
                    std::ostringstream msg;
                    msg << "Permutation #" << i << " was found to have "
                        "inverse " << NPerm4::S4[i].inverse() << " instead of "
                        << NPerm4::S4[NPerm4::invS4[i]] << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void sign() {
            int expected;
            for (int i = 0; i < 24; i++) {
                expected = (i % 2 == 0 ? 1 : -1);
                if (NPerm4::S4[i].sign() != expected) {
                    std::ostringstream msg;
                    msg << "Permutation #" << i << " was found to have "
                        "sign " << NPerm4::S4[i].sign()
                        << " instead of " << expected << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void index() {
            for (int i = 0; i < 24; ++i) {
                if (NPerm4::S4[i].S4Index() != i) {
                    std::ostringstream msg;
                    msg << "Permutation S4[" << i << "] gives an "
                        "incorrect S4 index of "
                        << NPerm4::S4[i].S4Index() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
                if (NPerm4::orderedS4[i].orderedS4Index() != i) {
                    std::ostringstream msg;
                    msg << "Permutation orderedS4[" << i << "] gives an "
                        "incorrect orderedS4 index of "
                        << NPerm4::orderedS4[i].orderedS4Index() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        bool looksLikeIdentity(const NPerm4& p) {
            return (p.isIdentity() && p == NPerm4() &&
                p.getPermCode() == 228 && p.getPermCode2() == 0 &&
                p.toString() == "0123");
        }

        bool looksEqual(const NPerm4& p, const NPerm4& q) {
            return (p == q && (! (p != q)) && p.toString() == q.toString() &&
                p.getPermCode() == q.getPermCode() &&
                p.getPermCode2() == q.getPermCode2());
        }

        bool looksEqual(const NPerm4& p, const NPerm4& q,
                const std::string& qStr) {
            return (p == q && (! (p != q)) && p.toString() == q.toString() &&
                p.getPermCode() == q.getPermCode() &&
                p.getPermCode2() == q.getPermCode2() && p.toString() == qStr);
        }

        bool looksDistinct(const NPerm4& p, const NPerm4& q) {
            return (p != q && (! (p == q)) && p.toString() != q.toString() &&
                p.getPermCode() != q.getPermCode() &&
                p.getPermCode2() != q.getPermCode2());
        }

        int expectedSign(const NPerm4& p) {
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
            NPerm4 p(a, b, c, d);

            std::ostringstream name;
            name << a << b << c << d;

            NPerm4 p0 = NPerm4::fromPermCode(p.getPermCode());
            if (! looksEqual(p0, p, name.str())) {
                std::ostringstream msg;
                msg << "The first-generation code constructor fails for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            NPerm4 p1 = NPerm4::fromPermCode2(p.getPermCode2());
            if (! looksEqual(p1, p, name.str())) {
                std::ostringstream msg;
                msg << "The second-generation code constructor fails for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            NPerm4 p2(3, d, 2, c, 0, a, 1, b);
            if (! looksEqual(p2, p, name.str())) {
                std::ostringstream msg;
                msg << "The 8-argument constructor fails for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            NPerm4 p3(p);
            if (! looksEqual(p3, p, name.str())) {
                std::ostringstream msg;
                msg << "The copy constructor fails for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            NPerm4 p4(2, 3, 1, 0);
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

            NPerm4 p5(2, 3, 1, 0);
            p5.setPermCode(p3.getPermCode());
            if (! looksEqual(p5, p, name.str())) {
                std::ostringstream msg;
                msg << "The setPermCode() / getPermCode() routines fail for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            NPerm4 p6(3, 2, 0, 1);
            p6.setPermCode2(p3.getPermCode2());
            if (! looksEqual(p6, p, name.str())) {
                std::ostringstream msg;
                msg << "The setPermCode2() / getPermCode2() routines fail for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (! NPerm4::isPermCode(p.getPermCode())) {
                std::ostringstream msg;
                msg << "Routine isPermCode() suggests that the permutation "
                    << name.str() << " has an invalid first-generation code.";
                CPPUNIT_FAIL(msg.str());
            }

            if (! NPerm4::isPermCode2(p.getPermCode2())) {
                std::ostringstream msg;
                msg << "Routine isPermCode2() suggests that the permutation "
                    << name.str() << " has an invalid second-generation code.";
                CPPUNIT_FAIL(msg.str());
            }

            if (NPerm4::isPermCode(0))
                CPPUNIT_FAIL("Routine isPermCode() suggests that 0 is a "
                    "valid first-generation code (which it is not).");

            if (! NPerm4::isPermCode2(0))
                CPPUNIT_FAIL("Routine isPermCode2() suggests that 0 is not a "
                    "valid second-generation code (which it is).");

            if (! looksEqual(p * NPerm4(), p)) {
                std::ostringstream msg;
                msg << "Multiplying permutation " << name.str()
                    << " by the identity does not give " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(NPerm4() * p, p)) {
                std::ostringstream msg;
                msg << "Multiplying the identity by permutation " << name.str()
                    << " does not give " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(p * NPerm4(0, 1), NPerm4(b, a, c, d))) {
                std::ostringstream msg;
                msg << "Multiplying permutation " << name.str()
                    << " by (0 <--> 1) does not give the expected result.";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(p * NPerm4(1, 2), NPerm4(a, c, b, d))) {
                std::ostringstream msg;
                msg << "Multiplying permutation " << name.str()
                    << " by (1 <--> 2) does not give the expected result.";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(p * NPerm4(2, 3), NPerm4(a, b, d, c))) {
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

            NPerm4 inv = p.inverse();
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
                NPerm4 id;
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
                NPerm4 last(3, 2, 1, 0);
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
            NPerm4 id;
            if (! looksLikeIdentity(id))
                CPPUNIT_FAIL("The default NPerm4 constructor does not "
                    "appear to give the identity permutation.");

            for (int i = 0; i < 4; ++i) {
                NPerm4 p(i, i);
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

        void products() {
            unsigned i, j, x;
            NPerm4 p, q, r;

            for (i = 0; i < 24; ++i) {
                p = NPerm4::S4[i];
                for (j = 0; j < 24; ++j) {
                    q = NPerm4::S4[j];

                    r = p * q;
                    for (x = 0; x < 4; ++x) {
                        if (r[x] != p[q[x]]) {
                            std::ostringstream msg;
                            msg << "Multiplication fails for the product "
                                << p.toString() << " * " << q.toString() << ".";
                            CPPUNIT_FAIL(msg.str());
                        }
                    }
                }
            }
        }

        void compareWith() {
            unsigned i, j;
            NPerm4 p, q;

            for (i = 0; i < 24; ++i) {
                p = NPerm4::orderedS4[i];
                if (p.compareWith(p) != 0) {
                    std::ostringstream msg;
                    msg << "Routine compareWith() does not conclude that "
                        << p.toString() << " == " << p.toString() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            for (i = 0; i < 24; ++i) {
                p = NPerm4::orderedS4[i];
                for (j = i + 1; j < 24; ++j) {
                    q = NPerm4::orderedS4[j];

                    if (p.compareWith(q) != -1) {
                        std::ostringstream msg;
                        msg << "Routine compareWith() does not conclude that "
                            << p.toString() << " < " << q.toString() << ".";
                        CPPUNIT_FAIL(msg.str());
                    }
                    if (q.compareWith(p) != 1) {
                        std::ostringstream msg;
                        msg << "Routine compareWith() does not conclude that "
                            << q.toString() << " > " << p.toString() << ".";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
            }
        }

        void deprecatedArrays() {
            unsigned i;

            for (i = 0; i < 24; ++i) {
                if (regina::allPermsS4[i] != NPerm4::S4[i])
                    CPPUNIT_FAIL("The deprecated regina::allPermsS4 "
                        "does not match the new NPerm4::S4.");
                if (regina::allPermsS4Inv[i] != NPerm4::invS4[i])
                    CPPUNIT_FAIL("The deprecated regina::allPermsS4 "
                        "does not match the new NPerm4::S4.");
                if (regina::orderedPermsS4[i] != NPerm4::orderedS4[i])
                    CPPUNIT_FAIL("The deprecated regina::allPermsS4 "
                        "does not match the new NPerm4::S4.");
            }

            for (i = 0; i < 6; ++i) {
                if (regina::allPermsS3[i] != NPerm4::S3[i])
                    CPPUNIT_FAIL("The deprecated regina::allPermsS4 "
                        "does not match the new NPerm4::S4.");
                if (regina::allPermsS3Inv[i] != NPerm4::invS3[i])
                    CPPUNIT_FAIL("The deprecated regina::allPermsS4 "
                        "does not match the new NPerm4::S4.");
                if (regina::orderedPermsS3[i] != NPerm4::orderedS3[i])
                    CPPUNIT_FAIL("The deprecated regina::allPermsS4 "
                        "does not match the new NPerm4::S4.");
            }

            for (i = 0; i < 2; ++i) {
                if (regina::allPermsS2[i] != NPerm4::S2[i])
                    CPPUNIT_FAIL("The deprecated regina::allPermsS4 "
                        "does not match the new NPerm4::S4.");
                if (regina::allPermsS2Inv[i] != NPerm4::invS2[i])
                    CPPUNIT_FAIL("The deprecated regina::allPermsS4 "
                        "does not match the new NPerm4::S4.");
            }
        }
};

void addNPerm4(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NPerm4Test::suite());
}

