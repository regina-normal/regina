
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
#include "testsuite/maths/testmaths.h"

using regina::Perm;

class Perm2Test : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(Perm2Test);

    CPPUNIT_TEST(inverse);
    CPPUNIT_TEST(sign);
    CPPUNIT_TEST(index);
    CPPUNIT_TEST(swaps);
    CPPUNIT_TEST(products);
    CPPUNIT_TEST(exhaustive);
    CPPUNIT_TEST(compareWith);
    CPPUNIT_TEST(reverse);
    CPPUNIT_TEST(aliases);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp() {
        }

        void tearDown() {
        }

        void inverse() {
            for (int i = 0; i < 2; i++) {
                if (Perm<2>::S2[i].inverse() != Perm<2>::S2[Perm<2>::invS2[i]]) {
                    std::ostringstream msg;
                    msg << "Permutation #" << i << " was found to have "
                        "inverse " << Perm<2>::S2[i].inverse() <<
                        " instead of " << Perm<2>::S2[Perm<2>::invS2[i]] << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void sign() {
            int expected;
            for (int i = 0; i < 2; ++i) {
                expected = (i % 2 == 0 ? 1 : -1);
                if (Perm<2>::S2[i].sign() != expected) {
                    std::ostringstream msg;
                    msg << "Permutation #" << i << " was found to have "
                        "sign " << Perm<2>::S2[i].sign()
                        << " instead of " << expected << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void index() {
            for (int i = 0; i < 2; ++i) {
                if (Perm<2>::S2[i].S2Index() != i) {
                    std::ostringstream msg;
                    msg << "Permutation S2[" << i << "] gives an "
                        "incorrect S2 index of "
                        << Perm<2>::S2[i].S2Index() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
                if (Perm<2>::orderedS2[i].orderedS2Index() != i) {
                    std::ostringstream msg;
                    msg << "Permutation orderedS2[" << i << "] gives an "
                        "incorrect orderedS2 index of "
                        << Perm<2>::orderedS2[i].orderedS2Index() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        bool looksLikeIdentity(const Perm<2>& p) {
            return (p.isIdentity() && p == Perm<2>() &&
                p.permCode() == 0 && p.str() == "01");
        }

        bool looksEqual(const Perm<2>& p, const Perm<2>& q) {
            return (p == q && (! (p != q)) && p.str() == q.str() &&
                p.permCode() == q.permCode());
        }

        bool looksEqual(const Perm<2>& p, const Perm<2>& q,
                const std::string& qStr) {
            return (p == q && (! (p != q)) && p.str() == q.str() &&
                p.permCode() == q.permCode() && p.str() == qStr);
        }

        bool looksDistinct(const Perm<2>& p, const Perm<2>& q) {
            return (p != q && (! (p == q)) && p.str() != q.str() &&
                p.permCode() != q.permCode());
        }

        int expectedSign(const Perm<2>& p) {
            // Count the number of reorderings.
            int reorderings = 0;

            int a, b;
            for (a = 0; a < 2; ++a)
                for (b = a + 1; b < 2; ++b)
                    if (p[a] > p[b])
                        ++reorderings;

            return ((reorderings % 2 == 0) ? 1 : -1);
        }

        void testPerm(int a) {
            Perm<2> p = Perm<2>::S2[a]; // 0 -> a, 1 -> 1-a.

            std::ostringstream name;
            name << a << (1-a);

            Perm<2> p1 = Perm<2>::fromPermCode(p.permCode());
            if (! looksEqual(p1, p, name.str())) {
                std::ostringstream msg;
                msg << "The internal code constructor fails for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            {
                int arr[2] = { a, 1 - a };
                Perm<2> parr(arr);
                if (! looksEqual(parr, p, name.str())) {
                    std::ostringstream msg;
                    msg << "The array constructor fails for "
                        "the permutation " << name.str() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            {
                int arrA[2] = { 1, 0 };
                int arrB[2] = { 1 - a, a };
                Perm<2> parr2(arrA, arrB);
                if (! looksEqual(parr2, p, name.str())) {
                    std::ostringstream msg;
                    msg << "The two-array constructor fails for "
                        "the permutation " << name.str() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            Perm<2> p3(p);
            if (! looksEqual(p3, p, name.str())) {
                std::ostringstream msg;
                msg << "The copy constructor fails for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            Perm<2> p4 = Perm<2>::S2[1 - a];
            if (! looksDistinct(p4, p)) {
                std::ostringstream msg;
                msg << "The inequality test fails for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            p4 = p;
            if (! looksEqual(p4, p, name.str())) {
                std::ostringstream msg;
                msg << "The assignment operator fails for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            Perm<2> p5(1, 0);
            p5.setPermCode(p3.permCode());
            if (! looksEqual(p5, p, name.str())) {
                std::ostringstream msg;
                msg << "The setPermCode() / permCode() routines fail for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (! Perm<2>::isPermCode(p.permCode())) {
                std::ostringstream msg;
                msg << "Routine isPermCode() suggests that the permutation "
                    << name.str() << " has an invalid permutation code.";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(p * Perm<2>(), p)) {
                std::ostringstream msg;
                msg << "Multiplying permutation " << name.str()
                    << " by the identity does not give " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(Perm<2>() * p, p)) {
                std::ostringstream msg;
                msg << "Multiplying the identity by permutation " << name.str()
                    << " does not give " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(p * Perm<2>::fromPermCode(1), Perm<2>::S2[1 - a])) {
                std::ostringstream msg;
                msg << "Multiplying permutation " << name.str()
                    << " by (0 <--> 1) does not give the expected result.";
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

            Perm<2> inv = p.inverse();
            if (inv[a] != 0 || inv[1 - a] != 1) {
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

            if (p[0] != a || p[1] != 1 - a) {
                std::ostringstream msg;
                msg << "The element images for permutation " << name.str()
                    << " do not appear to be correct.";
                CPPUNIT_FAIL(msg.str());
            }

            if (p.preImageOf(a) != 0 || p.preImageOf(1 - a) != 1) {
                std::ostringstream msg;
                msg << "The element preimages for permutation " << name.str()
                    << " do not appear to be correct.";
                CPPUNIT_FAIL(msg.str());
            }

            if (a != 0) {
                if (p.isIdentity()) {
                    std::ostringstream msg;
                    msg << "Permutation " << name.str()
                        << " is reported to be the identity permutation.";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            if (p.str() != name.str()) {
                std::ostringstream msg;
                msg << "The stringification for permutation " << name.str()
                    << " does not appear to be correct.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void swaps() {
            for (int i = 0; i < 2; ++i)
                for (int j = 0; j < 2; ++j) {
                    Perm<2> p(i, j);

                    if (p[i] != j) {
                        std::ostringstream msg;
                        msg << "The (" << i << ", " << j << ") swap "
                            "gives the wrong image for " << i << ".";
                        CPPUNIT_FAIL(msg.str());
                    }
                    if (p[j] != i) {
                        std::ostringstream msg;
                        msg << "The (" << i << ", " << j << ") swap "
                            "gives the wrong image for " << j << ".";
                        CPPUNIT_FAIL(msg.str());
                    }
                    if (i == j) {
                        if (p[(i+1)%2] != (i+1)%2) {
                            std::ostringstream msg;
                            msg << "The (" << i << ", " << j << ") swap "
                                "gives the wrong image for " << (i+1)%2 << ".";
                            CPPUNIT_FAIL(msg.str());
                        }
                    }
                }
        }

        void products() {
            // A direct test.
            unsigned i, j, x;
            Perm<2> p, q, r;

            for (i = 0; i < 2; ++i) {
                p = Perm<2>::S2[i];
                for (j = 0; j < 2; ++j) {
                    q = Perm<2>::S2[j];

                    r = p * q;
                    for (x = 0; x < 2; ++x) {
                        if (r[x] != p[q[x]]) {
                            std::ostringstream msg;
                            msg << "Multiplication fails for the product "
                                << p.str() << " * " << q.str() << ".";
                            CPPUNIT_FAIL(msg.str());
                        }
                    }
                }
            }

            // An indirect test (using Perm<4> to verify).
            int a, d;
            for (a = 0; a < 2; ++a) {
                Perm<2> x = Perm<2>::S2[a]; // 0 -> a, 1 -> 1-a.

                for (d = 0; d < 2; ++d) {
                    Perm<2> y = Perm<2>::S2[d]; // 0 -> d, 1 -> 1-d.

                    Perm<2> product2 = x * y;
                    regina::Perm<4> product4 =
                        regina::Perm<4>(a, 1 - a, 2, 3) *
                        regina::Perm<4>(d, 1 - d, 2, 3);

                    if (product2[0] != product4[0] ||
                            product2[1] != product4[1]) {
                        std::ostringstream msg;
                        msg << "The product is incorrect for "
                            << x.str() << " * " << y.str() << ".";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
            }
        }

        void exhaustive() {
            Perm<2> id;
            if (! looksLikeIdentity(id))
                CPPUNIT_FAIL("The default Perm<2> constructor does not "
                    "appear to give the identity permutation.");

            // Test all possible permutations.
            int tested = 0;
            for (int a = 0; a < 2; ++a) {
                testPerm(a);
                ++tested;
            }

            if (tested != 2)
                CPPUNIT_FAIL("All 2 permutations in S(2) were not tested.");
        }

        void compareWith() {
            unsigned i, j;
            Perm<2> p, q;

            for (i = 0; i < 2; ++i) {
                p = Perm<2>::orderedS2[i];
                if (p.compareWith(p) != 0) {
                    std::ostringstream msg;
                    msg << "Routine compareWith() does not conclude that "
                        << p.str() << " == " << p.str() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            for (i = 0; i < 2; ++i) {
                p = Perm<2>::orderedS2[i];
                for (j = i + 1; j < 2; ++j) {
                    q = Perm<2>::orderedS2[j];

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
            for (int i = 0; i < 2; i++) {
                Perm<2> p = Perm<2>::S2[i];
                Perm<2> r = p.reverse();

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
            unsigned i;

            for (i = 0; i < 2; ++i)
                if (Perm<2>::S2[i] != Perm<2>::Sn[i])
                    CPPUNIT_FAIL("Arrays S2 and Sn disagree for Perm<2>.");

            for (i = 0; i < 1; ++i)
                if (Perm<2>::S1[i] != Perm<2>::Sn_1[i])
                    CPPUNIT_FAIL("Arrays S1 and Sn_1 disagree for Perm<2>.");
        }
};

void addPerm2(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(Perm2Test::suite());
}

