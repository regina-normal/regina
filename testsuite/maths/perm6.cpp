
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

class Perm6Test : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(Perm6Test);

    CPPUNIT_TEST(permCode2);
    CPPUNIT_TEST(sign);
    CPPUNIT_TEST(index);
    CPPUNIT_TEST(exhaustive);
    CPPUNIT_TEST(products);
    CPPUNIT_TEST(compareWith);
    CPPUNIT_TEST(reverse);
    CPPUNIT_TEST(aliases);
    CPPUNIT_TEST(clear);
    CPPUNIT_TEST(order);
    CPPUNIT_TEST(pow);
    CPPUNIT_TEST(rot);
    CPPUNIT_TEST(conjugacy);
    CPPUNIT_TEST(increment);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp() {
        }

        void tearDown() {
        }

        void permCode2() {
            for (int i = 0; i < 720; i++) {
                auto code = Perm<6>::S6[i].permCode2();
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
            for (int i = 0; i < 720; ++i) {
                expected = (i % 2 == 0 ? 1 : -1);
                if (Perm<6>::S6[i].sign() != expected) {
                    std::ostringstream msg;
                    msg << "Permutation #" << i << " was found to have "
                        "sign " << Perm<6>::S6[i].sign()
                        << " instead of " << expected << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void index() {
            for (int i = 0; i < 720; ++i) {
                Perm osn = Perm<6>::orderedS6[i];
                Perm sn = Perm<6>::S6[i];

                if (sn.S6Index() != i) {
                    std::ostringstream msg;
                    msg << "Permutation S6[" << i << "] gives an "
                        "incorrect S6 index of "
                        << sn.S6Index() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
                if (osn.orderedS6Index() != i) {
                    std::ostringstream msg;
                    msg << "Permutation orderedS6[" << i << "] gives an "
                        "incorrect orderedS6 index of "
                        << osn.orderedS6Index() << ".";
                    CPPUNIT_FAIL(msg.str());
                }

                if (sn.sign() != (i % 2 == 0 ? 1 : -1)) {
                    std::ostringstream msg;
                    msg << "Permutation S6[" << i << "] has the wrong sign.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (sn != osn) {
                    if (sn.orderedS6Index() != (i ^ 1) ||
                            osn.S6Index() != (i ^ 1)) {
                        std::ostringstream msg;
                        msg << "Permutation S6/orderedS6[" << i << "] "
                            "differ by more than the last index bit.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
            }
        }

        bool looksLikeIdentity(const Perm<6>& p) {
            return (p.isIdentity() && p == Perm<6>() &&
                p.permCode1() == 181896 && p.permCode2() == 0 &&
                p.str() == "012345");
        }

        bool looksEqual(const Perm<6>& p, const Perm<6>& q) {
            return (p == q && (! (p != q)) && p.str() == q.str() &&
                p.permCode1() == q.permCode1() &&
                p.permCode2() == q.permCode2());
        }

        bool looksEqual(const Perm<6>& p, const Perm<6>& q,
                const std::string& qStr) {
            return (p == q && (! (p != q)) && p.str() == q.str() &&
                p.permCode1() == q.permCode1() &&
                p.permCode2() == q.permCode2() && p.str() == qStr);
        }

        bool looksDistinct(const Perm<6>& p, const Perm<6>& q) {
            return (p != q && (! (p == q)) && p.str() != q.str() &&
                p.permCode1() != q.permCode1() &&
                p.permCode2() != q.permCode2());
        }

        int expectedSign(const Perm<6>& p) {
            // Count the number of reorderings.
            int reorderings = 0;

            int a, b;
            for (a = 0; a < 6; ++a)
                for (b = a + 1; b < 6; ++b)
                    if (p[a] > p[b])
                        ++reorderings;

            return ((reorderings % 2 == 0) ? 1 : -1);
        }

        void testPerm(int a, int b, int c, int d, int e, int f) {
            Perm<6> p(a, b, c, d, e, f);

            std::ostringstream name;
            name << a << b << c << d << e << f;

            Perm<6> p1 = Perm<6>::fromPermCode1(p.permCode1());
            if (! looksEqual(p1, p, name.str())) {
                std::ostringstream msg;
                msg << "The first-generation code constructor fails for "
                    "the permutation " << p << " = " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            Perm<6> p1b = Perm<6>::fromPermCode2(p.permCode2());
            if (! looksEqual(p1b, p, name.str())) {
                std::ostringstream msg;
                msg << "The second-generation code constructor fails for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            Perm<6> p2(3, d, 2, c, 4, e, 5, f, 0, a, 1, b);
            if (! looksEqual(p2, p, name.str())) {
                std::ostringstream msg;
                msg << "The 12-argument constructor fails for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            {
                int arr[6] = { a, b, c, d, e, f };
                Perm<6> parr(arr);
                if (! looksEqual(parr, p, name.str())) {
                    std::ostringstream msg;
                    msg << "The array constructor fails for "
                        "the permutation " << name.str() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            {
                int arrA[6] = { 2, 5, 3, 0, 4, 1 };
                int arrB[6] = { c, f, d, a, e, b };
                Perm<6> parr2(arrA, arrB);
                if (! looksEqual(parr2, p, name.str())) {
                    std::ostringstream msg;
                    msg << "The two-array constructor fails for "
                        "the permutation " << name.str() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            Perm<6> p3(p);
            if (! looksEqual(p3, p, name.str())) {
                std::ostringstream msg;
                msg << "The copy constructor fails for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            Perm<6> p4(4, 2, 3, 0, 5, 1);
            if (! (a == 4 && b == 2 && c == 3 && d == 0 && e == 5 && f == 1)) {
                if (! looksDistinct(p4, p)) {
                    std::ostringstream msg;
                    msg << "The equality/inequality tests fail for "
                        "the permutations 423051 and " << name.str() << ".";
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

            Perm<6> p5(4, 2, 3, 0, 5, 1);
            p5.setPermCode1(p3.permCode1());
            if (! looksEqual(p5, p, name.str())) {
                std::ostringstream msg;
                msg << "The setPermCode1() / permCode1() routines fail for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            Perm<6> p6(4, 2, 3, 0, 5, 1);
            p6.setPermCode2(p3.permCode2());
            if (! looksEqual(p6, p, name.str())) {
                std::ostringstream msg;
                msg << "The setPermCode2() / permCode2() routines fail for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (! Perm<6>::isPermCode1(p.permCode1())) {
                std::ostringstream msg;
                msg << "Routine isPermCode1() suggests that the permutation "
                    << name.str() << " has an invalid permutation code.";
                CPPUNIT_FAIL(msg.str());
            }

            if (! Perm<6>::isPermCode2(p.permCode2())) {
                std::ostringstream msg;
                msg << "Routine isPermCode2() suggests that the permutation "
                    << name.str() << " has an invalid permutation code.";
                CPPUNIT_FAIL(msg.str());
            }

            if (Perm<6>::isPermCode1(0))
                CPPUNIT_FAIL("Routine isPermCode1() suggests that 0 is a "
                    "valid first-generation code (which it is not).");

            if (! Perm<6>::isPermCode2(0))
                CPPUNIT_FAIL("Routine isPermCode2() suggests that 0 is not a "
                    "valid second-generation code (which it is).");

            if (! looksEqual(p * Perm<6>(), p)) {
                std::ostringstream msg;
                msg << "Multiplying permutation " << name.str()
                    << " by the identity does not give " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(Perm<6>() * p, p)) {
                std::ostringstream msg;
                msg << "Multiplying the identity by permutation " << name.str()
                    << " does not give " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(p * Perm<6>(0, 1), Perm<6>(b, a, c, d, e, f))) {
                std::ostringstream msg;
                msg << "Multiplying permutation " << name.str()
                    << " by (0 <--> 1) does not give the expected result.";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(p * Perm<6>(1, 2), Perm<6>(a, c, b, d, e, f))) {
                std::ostringstream msg;
                msg << "Multiplying permutation " << name.str()
                    << " by (1 <--> 2) does not give the expected result.";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(p * Perm<6>(2, 3), Perm<6>(a, b, d, c, e, f))) {
                std::ostringstream msg;
                msg << "Multiplying permutation " << name.str()
                    << " by (2 <--> 3) does not give the expected result.";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(p * Perm<6>(3, 4), Perm<6>(a, b, c, e, d, f))) {
                std::ostringstream msg;
                msg << "Multiplying permutation " << name.str()
                    << " by (3 <--> 4) does not give the expected result.";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(p * Perm<6>(4, 5), Perm<6>(a, b, c, d, f, e))) {
                std::ostringstream msg;
                msg << "Multiplying permutation " << name.str()
                    << " by (4 <--> 5) does not give the expected result.";
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

            Perm<6> inv = p.inverse();
            if (inv[a] != 0 || inv[b] != 1 || inv[c] != 2 ||
                    inv[d] != 3 || inv[e] != 4 || inv[f] != 5) {
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

            if (p[0] != a || p[1] != b || p[2] != c ||
                    p[3] != d || p[4] != e || p[5] != f) {
                std::ostringstream msg;
                msg << "The element images for permutation " << name.str()
                    << " do not appear to be correct.";
                CPPUNIT_FAIL(msg.str());
            }

            if (p.preImageOf(a) != 0 || p.preImageOf(b) != 1 ||
                    p.preImageOf(c) != 2 || p.preImageOf(d) != 3 ||
                    p.preImageOf(e) != 4 || p.preImageOf(f) != 5) {
                std::ostringstream msg;
                msg << "The element preimages for permutation " << name.str()
                    << " do not appear to be correct.";
                CPPUNIT_FAIL(msg.str());
            }

            if (a != 0 || b != 1 || c != 2 || d != 3 || e != 4 || f != 5) {
                Perm<6> id;
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

            if (a != 5 || b != 4 || c != 3 || d != 2 || e != 1 || f != 0) {
                Perm<6> last(5, 4, 3, 2, 1, 0);
                if (p.compareWith(last) != -1 || last.compareWith(p) != 1) {
                    std::ostringstream msg;
                    msg << "Permutation " << name.str()
                        << " is not reported to be lexicographically "
                           "smaller than 543210.";
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
            Perm<6> id;
            if (! looksLikeIdentity(id))
                CPPUNIT_FAIL("The default Perm<6> constructor does not "
                    "appear to give the identity permutation.");

            for (int i = 0; i < 6; ++i) {
                Perm<6> p(i, i);
                if (! looksLikeIdentity(p)) {
                    std::ostringstream msg;
                    msg << "The permutation that swaps " << i
                        << " with itself does not appear to be the identity.";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            // Test all possible permutations.
            int tested = 0;
            int a, b, c, d, e, f;
            for (a = 0; a < 6; ++a) {
                for (b = 0; b < 6; ++b) {
                    if (b == a)
                        continue;
                    for (c = 0; c < 6; ++c) {
                        if (c == a || c == b)
                            continue;
                        for (d = 0; d < 6; ++d) {
                            if (d == a || d == b || d == c)
                                continue;
                            for (e = 0; e < 6; ++e) {
                                if (e == a || e == b || e == c || e == d)
                                    continue;
                                f = (15 - a - b - c - d - e);
                                testPerm(a, b, c, d, e, f);
                                ++tested;
                            }
                        }
                    }
                }
            }

            if (tested != 720)
                CPPUNIT_FAIL("All 720 permutations in S(6) were not tested.");
        }

        void products() {
            unsigned i, j, x;
            Perm<6> p, q, r;

            for (i = 0; i < 720; ++i) {
                p = Perm<6>::S6[i];
                for (j = 0; j < 720; ++j) {
                    q = Perm<6>::S6[j];

                    r = p * q;
                    for (x = 0; x < 6; ++x) {
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
            Perm<6> p, q;

            for (i = 0; i < 720; ++i) {
                p = Perm<6>::orderedS6[i];
                if (p.compareWith(p) != 0) {
                    std::ostringstream msg;
                    msg << "Routine compareWith() does not conclude that "
                        << p.str() << " == " << p.str() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            for (i = 0; i < 720; ++i) {
                p = Perm<6>::orderedS6[i];
                for (j = i + 1; j < 720; ++j) {
                    q = Perm<6>::orderedS6[j];

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
            for (int i = 0; i < 720; i++) {
                Perm<6> p = Perm<6>::S6[i];
                Perm<6> r = p.reverse();

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

        void aliases() {
            for (unsigned i = 0; i < 720; ++i)
                if (Perm<6>::S6[i] != Perm<6>::Sn[i])
                    CPPUNIT_FAIL("Arrays S6 and Sn disagree for Perm<6>.");
        }

        template <int from>
        void clearFrom() {
            // Requires 2 <= from <= n-2.
            Perm<6> rev = Perm<6>().reverse();

            unsigned i, j;
            for (i = 0; i < Perm<from>::nPerms; ++i)
                for (j = 0; j < Perm<6 - from>::nPerms; ++j) {
                    Perm<6> left = Perm<6>::extend(regina::Perm<from>::Sn[i]);
                    Perm<6> right = rev *
                        Perm<6>::extend(regina::Perm<6 - from>::Sn[j]) * rev;
                    Perm<6> p = left * right;
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
            Perm<6> rev = Perm<6>().reverse();
            unsigned i, j;

            for (i = 0; i < Perm<6>::nPerms; ++i) {
                Perm<6> p = Perm<6>::Sn[i];
                p.clear(6);
                if (! looksEqual(p, Perm<6>::Sn[i])) {
                    std::ostringstream msg;
                    msg << "Clearing from position 6 "
                        "gives the wrong result.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
            for (i = 0; i < Perm<5>::nPerms; ++i) {
                Perm<6> left = Perm<6>::extend(regina::Perm<5>::Sn[i]);
                Perm<6> p = left;
                p.clear(5);
                if (! looksEqual(p, left)) {
                    std::ostringstream msg;
                    msg << "Clearing from position 5 "
                        "gives the wrong result.";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            clearFrom<4>();
            clearFrom<3>();
            clearFrom<2>();

            for (j = 0; j < Perm<5>::nPerms; ++j) {
                Perm<6> p = rev *
                    Perm<6>::extend(regina::Perm<5>::Sn[j]) * rev;
                p.clear(1);
                if (! looksLikeIdentity(p)) {
                    std::ostringstream msg;
                    msg << "Clearing from position 1 "
                        "gives the wrong result.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
            for (j = 0; j < Perm<6>::nPerms; ++j) {
                Perm<6> p = Perm<6>::Sn[j];
                p.clear(0);
                if (! looksLikeIdentity(p)) {
                    std::ostringstream msg;
                    msg << "Clearing from position 0 "
                        "gives the wrong result.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void order() {
            int i, j;
            for (i = 0; i < Perm<6>::nPerms; ++i) {
                Perm<6> p = Perm<6>::Sn[i];

                j = 0;
                Perm<6> q;
                do {
                    q = q * p;
                    ++j;
                } while (! q.isIdentity());

                if (j != p.order()) {
                    std::ostringstream msg;
                    msg << "Permutation " << p << "^" << j << " is the "
                        "identity, but the reported order is " << p.order()
                        << "." << std::endl;
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void pow() {
            int i, j;
            for (i = 0; i < Perm<6>::nPerms; ++i) {
                Perm<6> p = Perm<6>::Sn[i];

                if (! p.pow(0).isIdentity()) {
                    std::ostringstream msg;
                    msg << "pow(" << p << ", 0) is not the identity."
                        << std::endl;
                    CPPUNIT_FAIL(msg.str());
                }
                {
                    Perm<6> q;
                    j = 0;
                    do {
                        Perm<6> pow = p.pow(++j);
                        q = q * p;
                        if (! looksEqual(pow, q)) {
                            std::ostringstream msg;
                            msg << "pow(" << p << ", " << j
                                << ") is not " << q << "." << std::endl;
                            CPPUNIT_FAIL(msg.str());
                        }
                    } while (j < 2 * p.order());
                }
                {
                    Perm<6> q;
                    j = 0;
                    do {
                        Perm<6> pow = p.pow(--j);
                        q = q * p.inverse();
                        if (! looksEqual(pow, q)) {
                            std::ostringstream msg;
                            msg << "pow(" << p << ", " << j
                                << ") is not " << q << "." << std::endl;
                            CPPUNIT_FAIL(msg.str());
                        }
                    } while (j > -2 * int(p.order()));
                }
            }
        }

        void rot() {
            int i, j;
            for (i = 0; i < 6; ++i) {
                Perm<6> p = Perm<6>::rot(i);
                for (j = 0; j < 6; ++j)
                    if (p[j] != (i + j) % 6) {
                        std::ostringstream msg;
                        msg << "Rotation " << i << ", position " << j
                            << " gives the wrong image " << p[j] << ".";
                        CPPUNIT_FAIL(msg.str());
                    }
            }
        }

        void conjugacy() {
            for (int i = 0; i < 720; ++i) {
                Perm<6> p = Perm<6>::Sn[i];

                // Manually decide if p is conjugacy minimal.
                bool min = true;
                for (int j = 0; j < 720; ++j) {
                    Perm<6> q = Perm<6>::Sn[j];
                    if ((q * p * q.inverse()).SnIndex() < i) {
                        min = false;
                        break;
                    }
                }

                if (p.isConjugacyMinimal() != min) {
                    std::ostringstream msg;
                    msg << "Permutation " << p << " gives wrong result "
                        "for isConjugacyMinimal().";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void increment() {
            int i = 0;
            Perm<6> p;
            Perm<6> q;

            do {
                if (p != q) {
                    std::ostringstream msg;
                    msg << "Preincrement and postincrement do not match for "
                        "permutation " << i << ".";
                    CPPUNIT_FAIL(msg.str());
                }
                if (p.SnIndex() != i) {
                    std::ostringstream msg;
                    msg << "Increment gives wrong index for permutation "
                        << i << ".";
                    CPPUNIT_FAIL(msg.str());
                }
                ++i; ++p; ++q;
            } while (! p.isIdentity());

            if (i != 720) {
                CPPUNIT_FAIL("Increment does not wrap around after 720 steps.");
            }
            if (! q.isIdentity()) {
                CPPUNIT_FAIL("Preincrement and postincrement do not "
                    "wrap around together.");
            }
        }
};

void addPerm6(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(Perm6Test::suite());
}

