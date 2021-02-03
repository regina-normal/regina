
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

#include <algorithm>
#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "maths/perm.h"
#include "testsuite/maths/testmaths.h"

using regina::Perm;

class Perm5Test : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(Perm5Test);

    CPPUNIT_TEST(permCode2);
    CPPUNIT_TEST(sign);
    CPPUNIT_TEST(index);
    CPPUNIT_TEST(exhaustive);
    CPPUNIT_TEST(products);
    CPPUNIT_TEST(compareWith);
    CPPUNIT_TEST(reverse);
    CPPUNIT_TEST(databases);
    CPPUNIT_TEST(aliases);
    CPPUNIT_TEST(clear);
    CPPUNIT_TEST(S2);
    CPPUNIT_TEST(S3);
    CPPUNIT_TEST(S4);
    CPPUNIT_TEST(rot);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp() {
        }

        void tearDown() {
        }

        void permCode2() {
            for (int i = 0; i < 120; i++) {
                auto code = Perm<5>::S5[i].permCode2();
                if (code != i) {
                    std::ostringstream msg;
                    msg << "Permutation #" << i
                        << " has incorrect second-generation code "
                        << code << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void sign() {
            int expected;
            for (int i = 0; i < 120; ++i) {
                expected = (i % 2 == 0 ? 1 : -1);
                if (Perm<5>::S5[i].sign() != expected) {
                    std::ostringstream msg;
                    msg << "Permutation #" << i << " was found to have "
                        "sign " << Perm<5>::S5[i].sign()
                        << " instead of " << expected << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void index() {
            for (int i = 0; i < 120; ++i) {
                Perm osn = Perm<5>::orderedS5[i];
                Perm sn = Perm<5>::S5[i];

                if (sn.S5Index() != i) {
                    std::ostringstream msg;
                    msg << "Permutation S5[" << i << "] gives an "
                        "incorrect S5 index of "
                        << sn.S5Index() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
                if (osn.orderedS5Index() != i) {
                    std::ostringstream msg;
                    msg << "Permutation orderedS5[" << i << "] gives an "
                        "incorrect orderedS5 index of "
                        << osn.orderedS5Index() << ".";
                    CPPUNIT_FAIL(msg.str());
                }

                if (sn.sign() != (i % 2 == 0 ? 1 : -1)) {
                    std::ostringstream msg;
                    msg << "Permutation S5[" << i << "] has the wrong sign.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (sn != osn) {
                    if (sn.orderedS5Index() != (i ^ 1) ||
                            osn.S5Index() != (i ^ 1)) {
                        std::ostringstream msg;
                        msg << "Permutation S5/orderedS5[" << i << "] "
                            "differ by more than the last index bit.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
            }
        }

        bool looksLikeIdentity(const Perm<5>& p) {
            return (p.isIdentity() && p == Perm<5>() &&
                p.permCode1() == 18056 && p.permCode2() == 0 &&
                p.str() == "01234");
        }

        bool looksEqual(const Perm<5>& p, const Perm<5>& q) {
            return (p == q && (! (p != q)) && p.str() == q.str() &&
                p.permCode1() == q.permCode1() &&
                p.permCode2() == q.permCode2());
        }

        bool looksEqual(const Perm<5>& p, const Perm<5>& q,
                const std::string& qStr) {
            return (p == q && (! (p != q)) && p.str() == q.str() &&
                p.permCode1() == q.permCode1() &&
                p.permCode2() == q.permCode2() && p.str() == qStr);
        }

        bool looksDistinct(const Perm<5>& p, const Perm<5>& q) {
            return (p != q && (! (p == q)) && p.str() != q.str() &&
                p.permCode1() != q.permCode1() &&
                p.permCode2() != q.permCode2());
        }

        int expectedSign(const Perm<5>& p) {
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
            Perm<5> p(a, b, c, d, e);

            std::ostringstream name;
            name << a << b << c << d << e;

            Perm<5> p1 = Perm<5>::fromPermCode1(p.permCode1());
            if (! looksEqual(p1, p, name.str())) {
                std::ostringstream msg;
                msg << "The first-generation code constructor fails for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            Perm<5> p1b = Perm<5>::fromPermCode2(p.permCode2());
            if (! looksEqual(p1b, p, name.str())) {
                std::ostringstream msg;
                msg << "The second-generation code constructor fails for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            Perm<5> p2(3, d, 2, c, 4, e, 0, a, 1, b);
            if (! looksEqual(p2, p, name.str())) {
                std::ostringstream msg;
                msg << "The 10-argument constructor fails for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            {
                int arr[5] = { a, b, c, d, e };
                Perm<5> parr(arr);
                if (! looksEqual(parr, p, name.str())) {
                    std::ostringstream msg;
                    msg << "The array constructor fails for "
                        "the permutation " << name.str() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            {
                int arrA[5] = { 2, 3, 0, 4, 1 };
                int arrB[5] = { c, d, a, e, b };
                Perm<5> parr2(arrA, arrB);
                if (! looksEqual(parr2, p, name.str())) {
                    std::ostringstream msg;
                    msg << "The two-array constructor fails for "
                        "the permutation " << name.str() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            Perm<5> p3(p);
            if (! looksEqual(p3, p, name.str())) {
                std::ostringstream msg;
                msg << "The copy constructor fails for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            Perm<5> p4(4, 2, 3, 0, 1);
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

            Perm<5> p5(4, 2, 3, 0, 1);
            p5.setPermCode1(p3.permCode1());
            if (! looksEqual(p5, p, name.str())) {
                std::ostringstream msg;
                msg << "The setPermCode1() / permCode1() routines fail for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            Perm<5> p6(4, 2, 3, 0, 1);
            p6.setPermCode2(p3.permCode2());
            if (! looksEqual(p6, p, name.str())) {
                std::ostringstream msg;
                msg << "The setPermCode2() / permCode2() routines fail for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (! Perm<5>::isPermCode1(p.permCode1())) {
                std::ostringstream msg;
                msg << "Routine isPermCode1() suggests that the permutation "
                    << name.str() << " has an invalid permutation code.";
                CPPUNIT_FAIL(msg.str());
            }

            if (! Perm<5>::isPermCode2(p.permCode2())) {
                std::ostringstream msg;
                msg << "Routine isPermCode2() suggests that the permutation "
                    << name.str() << " has an invalid permutation code.";
                CPPUNIT_FAIL(msg.str());
            }

            if (Perm<5>::isPermCode1(0))
                CPPUNIT_FAIL("Routine isPermCode1() suggests that 0 is a "
                    "valid first-generation code (which it is not).");

            if (! Perm<5>::isPermCode2(0))
                CPPUNIT_FAIL("Routine isPermCode2() suggests that 0 is not a "
                    "valid second-generation code (which it is).");

            if (! looksEqual(p * Perm<5>(), p)) {
                std::ostringstream msg;
                msg << "Multiplying permutation " << name.str()
                    << " by the identity does not give " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(Perm<5>() * p, p)) {
                std::ostringstream msg;
                msg << "Multiplying the identity by permutation " << name.str()
                    << " does not give " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(p * Perm<5>(0, 1), Perm<5>(b, a, c, d, e))) {
                std::ostringstream msg;
                msg << "Multiplying permutation " << name.str()
                    << " by (0 <--> 1) does not give the expected result.";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(p * Perm<5>(1, 2), Perm<5>(a, c, b, d, e))) {
                std::ostringstream msg;
                msg << "Multiplying permutation " << name.str()
                    << " by (1 <--> 2) does not give the expected result.";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(p * Perm<5>(2, 3), Perm<5>(a, b, d, c, e))) {
                std::ostringstream msg;
                msg << "Multiplying permutation " << name.str()
                    << " by (2 <--> 3) does not give the expected result.";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(p * Perm<5>(3, 4), Perm<5>(a, b, c, e, d))) {
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

            Perm<5> inv = p.inverse();
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
                Perm<5> id;
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
                Perm<5> last(4, 3, 2, 1, 0);
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

            if (p.str() != name.str()) {
                std::ostringstream msg;
                msg << "The stringification for permutation " << name.str()
                    << " does not appear to be correct.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void exhaustive() {
            Perm<5> id;
            if (! looksLikeIdentity(id))
                CPPUNIT_FAIL("The default Perm<5> constructor does not "
                    "appear to give the identity permutation.");

            for (int i = 0; i < 5; ++i) {
                Perm<5> p(i, i);
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

        void products() {
            unsigned i, j, x;
            Perm<5> p, q, r;

            for (i = 0; i < 120; ++i) {
                p = Perm<5>::S5[i];
                for (j = 0; j < 120; ++j) {
                    q = Perm<5>::S5[j];

                    r = p * q;
                    for (x = 0; x < 5; ++x) {
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
            Perm<5> p, q;

            for (i = 0; i < 120; ++i) {
                p = Perm<5>::orderedS5[i];
                if (p.compareWith(p) != 0) {
                    std::ostringstream msg;
                    msg << "Routine compareWith() does not conclude that "
                        << p.str() << " == " << p.str() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            for (i = 0; i < 120; ++i) {
                p = Perm<5>::orderedS5[i];
                for (j = i + 1; j < 120; ++j) {
                    q = Perm<5>::orderedS5[j];

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
            for (int i = 0; i < 120; i++) {
                Perm<5> p = Perm<5>::S5[i];
                Perm<5> r = p.reverse();

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
            for (i = 0; i < 24; ++i) {
                if (Perm<5>::S4[i] != Perm<5>::extend(regina::Perm<4>::S4[i]))
                    CPPUNIT_FAIL("Perm<5> and Perm<4> do not agree on S4.");
            }
            for (i = 0; i < 24; ++i) {
                if (Perm<5>::orderedS4[i] !=
                        Perm<5>::extend(regina::Perm<4>::orderedS4[i]))
                    CPPUNIT_FAIL("Perm<5> and Perm<4> do not agree on orderedS4.");
            }
            for (i = 0; i < 6; ++i) {
                if (Perm<5>::S3[i] != Perm<5>::extend(regina::Perm<4>::S3[i]))
                    CPPUNIT_FAIL("Perm<5> and Perm<4> do not agree on S3.");
            }
            for (i = 0; i < 6; ++i) {
                if (Perm<5>::orderedS3[i] !=
                        Perm<5>::extend(regina::Perm<4>::orderedS3[i]))
                    CPPUNIT_FAIL("Perm<5> and Perm<4> do not agree on orderedS3.");
            }
            for (i = 0; i < 2; ++i) {
                if (Perm<5>::S2[i] != Perm<5>::extend(regina::Perm<4>::S2[i]))
                    CPPUNIT_FAIL("Perm<4> and Perm<4> do not agree on S2.");
            }
        }

        void aliases() {
            unsigned i;

            for (i = 0; i < 120; ++i)
                if (Perm<5>::S5[i] != Perm<5>::Sn[i])
                    CPPUNIT_FAIL("Arrays S5 and Sn disagree for Perm<5>.");

            for (i = 0; i < 24; ++i)
                if (Perm<5>::S4[i] != Perm<5>::Sn_1[i])
                    CPPUNIT_FAIL("Arrays S4 and Sn_1 disagree for Perm<5>.");
        }

        template <int from>
        void clearFrom() {
            // Requires 2 <= from <= n-2.
            Perm<5> rev = Perm<5>().reverse();

            unsigned i, j;
            for (i = 0; i < Perm<from>::nPerms; ++i)
                for (j = 0; j < Perm<5 - from>::nPerms; ++j) {
                    Perm<5> left = Perm<5>::extend(regina::Perm<from>::Sn[i]);
                    Perm<5> right = rev *
                        Perm<5>::extend(regina::Perm<5 - from>::Sn[j]) * rev;
                    Perm<5> p = left * right;
                    p.clear(from);
                    if (! looksEqual(p, left)) {
                        std::ostringstream msg;
                        msg << "Clearing from position " << from
                            << " gives the wrong result.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
        }

        void clear() {
            Perm<5> rev = Perm<5>().reverse();
            unsigned i, j;

            for (i = 0; i < Perm<5>::nPerms; ++i) {
                Perm<5> p = Perm<5>::Sn[i];
                p.clear(5);
                if (! looksEqual(p, Perm<5>::Sn[i])) {
                    std::ostringstream msg;
                    msg << "Clearing from position 5 "
                        "gives the wrong result.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
            for (i = 0; i < Perm<4>::nPerms; ++i) {
                Perm<5> left = Perm<5>::extend(regina::Perm<4>::Sn[i]);
                Perm<5> p = left;
                p.clear(4);
                if (! looksEqual(p, left)) {
                    std::ostringstream msg;
                    msg << "Clearing from position 4 "
                        "gives the wrong result.";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            clearFrom<3>();
            clearFrom<2>();

            for (j = 0; j < Perm<4>::nPerms; ++j) {
                Perm<5> p = rev *
                    Perm<5>::extend(regina::Perm<4>::Sn[j]) * rev;
                p.clear(1);
                if (! looksLikeIdentity(p)) {
                    std::ostringstream msg;
                    msg << "Clearing from position 1 "
                        "gives the wrong result.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
            for (j = 0; j < Perm<5>::nPerms; ++j) {
                Perm<5> p = Perm<5>::Sn[j];
                p.clear(0);
                if (! looksLikeIdentity(p)) {
                    std::ostringstream msg;
                    msg << "Clearing from position 0 "
                        "gives the wrong result.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void S2() {
            for (unsigned i = 0; i < 2; ++i) {
                if (! looksEqual(Perm<5>::S2[i],
                        Perm<5>::extend(Perm<2>::S2[i])))
                    CPPUNIT_FAIL("S2 permutations do not match "
                        "Perm<2> extensions.");
                if (Perm<2>::S2[i] != Perm<2>::contract(Perm<5>::S2[i]))
                    CPPUNIT_FAIL("Contracted S2 permutations do not "
                        "match Perm<2>.");
            }
        }

        void S3() {
            for (unsigned i = 0; i < 6; ++i) {
                if (! looksEqual(Perm<5>::S3[i],
                        Perm<5>::extend(Perm<3>::S3[i])))
                    CPPUNIT_FAIL("S3 permutations do not match "
                        "Perm<3> extensions.");
                if (! looksEqual(Perm<5>::orderedS3[i],
                        Perm<5>::extend(Perm<3>::orderedS3[i])))
                    CPPUNIT_FAIL("S3 permutations do not match "
                        "Perm<3> extensions.");
                if (Perm<3>::S3[i] != Perm<3>::contract(Perm<5>::S3[i]))
                    CPPUNIT_FAIL("Contracted S3 permutations do not "
                        "match Perm<3>.");
                if (Perm<3>::orderedS3[i] !=
                        Perm<3>::contract(Perm<5>::orderedS3[i]))
                    CPPUNIT_FAIL("Contracted S3 permutations do not "
                        "match Perm<3>.");
            }
        }

        void S4() {
            for (unsigned i = 0; i < 24; ++i) {
                if (! looksEqual(Perm<5>::S4[i],
                        Perm<5>::extend(Perm<4>::S4[i])))
                    CPPUNIT_FAIL("S4 permutations do not match "
                        "Perm<4> extensions.");
                if (! looksEqual(Perm<5>::Sn_1[i],
                        Perm<5>::extend(Perm<4>::S4[i])))
                    CPPUNIT_FAIL("S4 permutations do not match "
                        "Perm<4> extensions.");
                if (! looksEqual(Perm<5>::orderedS4[i],
                        Perm<5>::extend(Perm<4>::orderedS4[i])))
                    CPPUNIT_FAIL("S4 permutations do not match "
                        "Perm<4> extensions.");
                if (Perm<4>::S4[i] != Perm<4>::contract(Perm<5>::S4[i]))
                    CPPUNIT_FAIL("Contracted S4 permutations do not "
                        "match Perm<4>.");
                if (Perm<4>::S4[i] != Perm<4>::contract(Perm<5>::Sn_1[i]))
                    CPPUNIT_FAIL("Contracted S4 permutations do not "
                        "match Perm<4>.");
                if (Perm<4>::orderedS4[i] !=
                        Perm<4>::contract(Perm<5>::orderedS4[i]))
                    CPPUNIT_FAIL("Contracted S4 permutations do not "
                        "match Perm<4>.");
            }
        }

        void rot() {
            int i, j;
            for (i = 0; i < 5; ++i) {
                Perm<5> p = Perm<5>::rot(i);
                for (j = 0; j < 5; ++j)
                    if (p[j] != (i + j) % 5) {
                        std::ostringstream msg;
                        msg << "Rotation " << i << ", position " << j
                            << " gives the wrong image " << p[j] << ".";
                        CPPUNIT_FAIL(msg.str());
                    }
            }
        }
};

void addPerm5(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(Perm5Test::suite());
}

