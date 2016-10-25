
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

#include <algorithm>
#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "maths/perm.h"
#include "testsuite/dim3/testtriangulation.h"

using regina::Perm;

class Perm4Test : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(Perm4Test);

    CPPUNIT_TEST(inverse);
    CPPUNIT_TEST(sign);
    CPPUNIT_TEST(index);
    CPPUNIT_TEST(exhaustive);
    CPPUNIT_TEST(products);
    CPPUNIT_TEST(compareWith);
    CPPUNIT_TEST(reverse);
    CPPUNIT_TEST(databases);
    CPPUNIT_TEST(aliases);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp() {
        }

        void tearDown() {
        }

        void inverse() {
            for (int i = 0; i < 24; i++) {
                if (Perm<4>::S4[i].inverse() != Perm<4>::S4[Perm<4>::invS4[i]]) {
                    std::ostringstream msg;
                    msg << "Permutation #" << i << " was found to have "
                        "inverse " << Perm<4>::S4[i].inverse() << " instead of "
                        << Perm<4>::S4[Perm<4>::invS4[i]] << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void sign() {
            int expected;
            for (int i = 0; i < 24; i++) {
                expected = (i % 2 == 0 ? 1 : -1);
                if (Perm<4>::S4[i].sign() != expected) {
                    std::ostringstream msg;
                    msg << "Permutation #" << i << " was found to have "
                        "sign " << Perm<4>::S4[i].sign()
                        << " instead of " << expected << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void index() {
            for (int i = 0; i < 24; ++i) {
                if (Perm<4>::S4[i].S4Index() != i) {
                    std::ostringstream msg;
                    msg << "Permutation S4[" << i << "] gives an "
                        "incorrect S4 index of "
                        << Perm<4>::S4[i].S4Index() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
                if (Perm<4>::orderedS4[i].orderedS4Index() != i) {
                    std::ostringstream msg;
                    msg << "Permutation orderedS4[" << i << "] gives an "
                        "incorrect orderedS4 index of "
                        << Perm<4>::orderedS4[i].orderedS4Index() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        bool looksLikeIdentity(const Perm<4>& p) {
            return (p.isIdentity() && p == Perm<4>() &&
                p.permCode() == 228 && p.permCode2() == 0 &&
                p.str() == "0123");
        }

        bool looksEqual(const Perm<4>& p, const Perm<4>& q) {
            return (p == q && (! (p != q)) && p.str() == q.str() &&
                p.permCode() == q.permCode() &&
                p.permCode2() == q.permCode2());
        }

        bool looksEqual(const Perm<4>& p, const Perm<4>& q,
                const std::string& qStr) {
            return (p == q && (! (p != q)) && p.str() == q.str() &&
                p.permCode() == q.permCode() &&
                p.permCode2() == q.permCode2() && p.str() == qStr);
        }

        bool looksDistinct(const Perm<4>& p, const Perm<4>& q) {
            return (p != q && (! (p == q)) && p.str() != q.str() &&
                p.permCode() != q.permCode() &&
                p.permCode2() != q.permCode2());
        }

        int expectedSign(const Perm<4>& p) {
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
            Perm<4> p(a, b, c, d);

            std::ostringstream name;
            name << a << b << c << d;

            Perm<4> p0 = Perm<4>::fromPermCode(p.permCode());
            if (! looksEqual(p0, p, name.str())) {
                std::ostringstream msg;
                msg << "The first-generation code constructor fails for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            Perm<4> p1 = Perm<4>::fromPermCode2(p.permCode2());
            if (! looksEqual(p1, p, name.str())) {
                std::ostringstream msg;
                msg << "The second-generation code constructor fails for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            Perm<4> p2(3, d, 2, c, 0, a, 1, b);
            if (! looksEqual(p2, p, name.str())) {
                std::ostringstream msg;
                msg << "The 8-argument constructor fails for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            {
                int arr[4] = { a, b, c, d };
                Perm<4> parr(arr);
                if (! looksEqual(parr, p, name.str())) {
                    std::ostringstream msg;
                    msg << "The array constructor fails for "
                        "the permutation " << name.str() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            {
                int arrA[4] = { 1, 3, 2, 0 };
                int arrB[4] = { b, d, c, a };
                Perm<4> parr2(arrA, arrB);
                if (! looksEqual(parr2, p, name.str())) {
                    std::ostringstream msg;
                    msg << "The two-array constructor fails for "
                        "the permutation " << name.str() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            Perm<4> p3(p);
            if (! looksEqual(p3, p, name.str())) {
                std::ostringstream msg;
                msg << "The copy constructor fails for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            Perm<4> p4(2, 3, 1, 0);
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

            Perm<4> p5(2, 3, 1, 0);
            p5.setPermCode(p3.permCode());
            if (! looksEqual(p5, p, name.str())) {
                std::ostringstream msg;
                msg << "The setPermCode() / permCode() routines fail for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            Perm<4> p6(3, 2, 0, 1);
            p6.setPermCode2(p3.permCode2());
            if (! looksEqual(p6, p, name.str())) {
                std::ostringstream msg;
                msg << "The setPermCode2() / permCode2() routines fail for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (! Perm<4>::isPermCode(p.permCode())) {
                std::ostringstream msg;
                msg << "Routine isPermCode() suggests that the permutation "
                    << name.str() << " has an invalid first-generation code.";
                CPPUNIT_FAIL(msg.str());
            }

            if (! Perm<4>::isPermCode2(p.permCode2())) {
                std::ostringstream msg;
                msg << "Routine isPermCode2() suggests that the permutation "
                    << name.str() << " has an invalid second-generation code.";
                CPPUNIT_FAIL(msg.str());
            }

            if (Perm<4>::isPermCode(0))
                CPPUNIT_FAIL("Routine isPermCode() suggests that 0 is a "
                    "valid first-generation code (which it is not).");

            if (! Perm<4>::isPermCode2(0))
                CPPUNIT_FAIL("Routine isPermCode2() suggests that 0 is not a "
                    "valid second-generation code (which it is).");

            if (! looksEqual(p * Perm<4>(), p)) {
                std::ostringstream msg;
                msg << "Multiplying permutation " << name.str()
                    << " by the identity does not give " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(Perm<4>() * p, p)) {
                std::ostringstream msg;
                msg << "Multiplying the identity by permutation " << name.str()
                    << " does not give " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(p * Perm<4>(0, 1), Perm<4>(b, a, c, d))) {
                std::ostringstream msg;
                msg << "Multiplying permutation " << name.str()
                    << " by (0 <--> 1) does not give the expected result.";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(p * Perm<4>(1, 2), Perm<4>(a, c, b, d))) {
                std::ostringstream msg;
                msg << "Multiplying permutation " << name.str()
                    << " by (1 <--> 2) does not give the expected result.";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(p * Perm<4>(2, 3), Perm<4>(a, b, d, c))) {
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

            Perm<4> inv = p.inverse();
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
                Perm<4> id;
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
                Perm<4> last(3, 2, 1, 0);
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

            if (p.str() != name.str()) {
                std::ostringstream msg;
                msg << "The stringification for permutation " << name.str()
                    << " does not appear to be correct.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void exhaustive() {
            Perm<4> id;
            if (! looksLikeIdentity(id))
                CPPUNIT_FAIL("The default Perm<4> constructor does not "
                    "appear to give the identity permutation.");

            for (int i = 0; i < 4; ++i) {
                Perm<4> p(i, i);
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
            Perm<4> p, q, r;

            for (i = 0; i < 24; ++i) {
                p = Perm<4>::S4[i];
                for (j = 0; j < 24; ++j) {
                    q = Perm<4>::S4[j];

                    r = p * q;
                    for (x = 0; x < 4; ++x) {
                        if (r[x] != p[q[x]]) {
                            std::ostringstream msg;
                            msg << "Multiplication fails for the product "
                                << p.str() << " * " << q.str() << ".";
                            CPPUNIT_FAIL(msg.str());
                        }
                    }
                }
            }
        }

        void compareWith() {
            unsigned i, j;
            Perm<4> p, q;

            for (i = 0; i < 24; ++i) {
                p = Perm<4>::orderedS4[i];
                if (p.compareWith(p) != 0) {
                    std::ostringstream msg;
                    msg << "Routine compareWith() does not conclude that "
                        << p.str() << " == " << p.str() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            for (i = 0; i < 24; ++i) {
                p = Perm<4>::orderedS4[i];
                for (j = i + 1; j < 24; ++j) {
                    q = Perm<4>::orderedS4[j];

                    if (p.compareWith(q) != -1) {
                        std::ostringstream msg;
                        msg << "Routine compareWith() does not conclude that "
                            << p.str() << " < " << q.str() << ".";
                        CPPUNIT_FAIL(msg.str());
                    }
                    if (q.compareWith(p) != 1) {
                        std::ostringstream msg;
                        msg << "Routine compareWith() does not conclude that "
                            << q.str() << " > " << p.str() << ".";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
            }
        }

        void reverse() {
            for (int i = 0; i < 24; i++) {
                Perm<4> p = Perm<4>::S4[i];
                Perm<4> r = p.reverse();

                if (! looksEqual(p, r.reverse())) {
                    std::ostringstream msg;
                    msg << "Permutation #" << i << " indicates that "
                        "reverse() is not idempotent.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (! looksDistinct(p, r)) {
                    std::ostringstream msg;
                    msg << "Permutation #" << i << " indicates that "
                        "reverse() is not a different permutation.";
                    CPPUNIT_FAIL(msg.str());
                }

                std::string s = p.str();
                std::reverse(s.begin(), s.end());
                if (s != r.str()) {
                    std::ostringstream msg;
                    msg << "Reverse of permutation #" << i << " does not have "
                        "the reverse string representation.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void databases() {
            unsigned i;
            for (i = 0; i < 6; ++i) {
                if (Perm<4>::S3[i] != Perm<4>::extend(regina::Perm<3>::S3[i]))
                    CPPUNIT_FAIL("Perm<4> and Perm<3> do not agree on S3.");
            }
            for (i = 0; i < 6; ++i) {
                if (Perm<4>::orderedS3[i] !=
                        Perm<4>::extend(regina::Perm<3>::orderedS3[i]))
                    CPPUNIT_FAIL("Perm<4> and Perm<3> do not agree on orderedS3.");
            }
            for (i = 0; i < 2; ++i) {
                if (Perm<4>::S2[i] != Perm<4>::extend(regina::Perm<3>::S2[i]))
                    CPPUNIT_FAIL("Perm<4> and Perm<3> do not agree on S2.");
            }
        }

        void aliases() {
            unsigned i;

            for (i = 0; i < 24; ++i)
                if (Perm<4>::S4[i] != Perm<4>::Sn[i])
                    CPPUNIT_FAIL("Arrays S4 and Sn disagree for Perm<4>.");

            for (i = 0; i < 6; ++i)
                if (Perm<4>::S3[i] != Perm<4>::Sn_1[i])
                    CPPUNIT_FAIL("Arrays S3 and Sn_1 disagree for Perm<4>.");
        }
};

void addPerm4(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(Perm4Test::suite());
}

