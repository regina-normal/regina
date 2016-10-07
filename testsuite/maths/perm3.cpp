
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

class Perm3Test : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(Perm3Test);

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
            for (int i = 0; i < 6; i++) {
                if (Perm<3>::S3[i].inverse() != Perm<3>::S3[Perm<3>::invS3[i]]) {
                    std::ostringstream msg;
                    msg << "Permutation #" << i << " was found to have "
                        "inverse " << Perm<3>::S3[i].inverse() <<
                        " instead of " << Perm<3>::S3[Perm<3>::invS3[i]] << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void sign() {
            int expected;
            for (int i = 0; i < 6; ++i) {
                expected = (i % 2 == 0 ? 1 : -1);
                if (Perm<3>::S3[i].sign() != expected) {
                    std::ostringstream msg;
                    msg << "Permutation #" << i << " was found to have "
                        "sign " << Perm<3>::S3[i].sign()
                        << " instead of " << expected << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void index() {
            for (int i = 0; i < 6; ++i) {
                if (Perm<3>::S3[i].S3Index() != i) {
                    std::ostringstream msg;
                    msg << "Permutation S3[" << i << "] gives an "
                        "incorrect S3 index of "
                        << Perm<3>::S3[i].S3Index() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
                if (Perm<3>::orderedS3[i].orderedS3Index() != i) {
                    std::ostringstream msg;
                    msg << "Permutation orderedS3[" << i << "] gives an "
                        "incorrect orderedS3 index of "
                        << Perm<3>::orderedS3[i].orderedS3Index() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        bool looksLikeIdentity(const Perm<3>& p) {
            return (p.isIdentity() && p == Perm<3>() &&
                p.permCode() == 0 && p.str() == "012");
        }

        bool looksEqual(const Perm<3>& p, const Perm<3>& q) {
            return (p == q && (! (p != q)) && p.str() == q.str() &&
                p.permCode() == q.permCode());
        }

        bool looksEqual(const Perm<3>& p, const Perm<3>& q,
                const std::string& qStr) {
            return (p == q && (! (p != q)) && p.str() == q.str() &&
                p.permCode() == q.permCode() && p.str() == qStr);
        }

        bool looksDistinct(const Perm<3>& p, const Perm<3>& q) {
            return (p != q && (! (p == q)) && p.str() != q.str() &&
                p.permCode() != q.permCode());
        }

        int expectedSign(const Perm<3>& p) {
            // Count the number of reorderings.
            int reorderings = 0;

            int a, b;
            for (a = 0; a < 3; ++a)
                for (b = a + 1; b < 3; ++b)
                    if (p[a] > p[b])
                        ++reorderings;

            return ((reorderings % 2 == 0) ? 1 : -1);
        }

        void testPerm(int a, int b, int c) {
            Perm<3> p(a, b, c);

            std::ostringstream name;
            name << a << b << c;

            Perm<3> p1 = Perm<3>::fromPermCode(p.permCode());
            if (! looksEqual(p1, p, name.str())) {
                std::ostringstream msg;
                msg << "The internal code constructor fails for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            {
                int arr[3] = { a, b, c };
                Perm<3> parr(arr);
                if (! looksEqual(parr, p, name.str())) {
                    std::ostringstream msg;
                    msg << "The array constructor fails for "
                        "the permutation " << name.str() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            {
                int arrA[3] = { 1, 2, 0 };
                int arrB[3] = { b, c, a };
                Perm<3> parr2(arrA, arrB);
                if (! looksEqual(parr2, p, name.str())) {
                    std::ostringstream msg;
                    msg << "The two-array constructor fails for "
                        "the permutation " << name.str() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            Perm<3> p3(p);
            if (! looksEqual(p3, p, name.str())) {
                std::ostringstream msg;
                msg << "The copy constructor fails for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            Perm<3> p4(2, 0, 1);
            if (! (a == 2 && b == 0 && c == 1)) {
                if (! looksDistinct(p4, p)) {
                    std::ostringstream msg;
                    msg << "The equality/inequality tests fail for "
                        "the permutations 201 and " << name.str() << ".";
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

            Perm<3> p5(2, 0, 1);
            p5.setPermCode(p3.permCode());
            if (! looksEqual(p5, p, name.str())) {
                std::ostringstream msg;
                msg << "The setPermCode() / permCode() routines fail for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (! Perm<3>::isPermCode(p.permCode())) {
                std::ostringstream msg;
                msg << "Routine isPermCode() suggests that the permutation "
                    << name.str() << " has an invalid permutation code.";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(p * Perm<3>(), p)) {
                std::ostringstream msg;
                msg << "Multiplying permutation " << name.str()
                    << " by the identity does not give " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(Perm<3>() * p, p)) {
                std::ostringstream msg;
                msg << "Multiplying the identity by permutation " << name.str()
                    << " does not give " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(p * Perm<3>::fromPermCode(Perm<3>::code102),
                    Perm<3>(b, a, c))) {
                std::ostringstream msg;
                msg << "Multiplying permutation " << name.str()
                    << " by (0 <--> 1) does not give the expected result.";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(p * Perm<3>::fromPermCode(Perm<3>::code021),
                    Perm<3>(a, c, b))) {
                std::ostringstream msg;
                msg << "Multiplying permutation " << name.str()
                    << " by (1 <--> 2) does not give the expected result.";
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

            Perm<3> inv = p.inverse();
            if (inv[a] != 0 || inv[b] != 1 || inv[c] != 2) {
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

            if (p[0] != a || p[1] != b || p[2] != c) {
                std::ostringstream msg;
                msg << "The element images for permutation " << name.str()
                    << " do not appear to be correct.";
                CPPUNIT_FAIL(msg.str());
            }

            if (p.preImageOf(a) != 0 || p.preImageOf(b) != 1 ||
                    p.preImageOf(c) != 2) {
                std::ostringstream msg;
                msg << "The element preimages for permutation " << name.str()
                    << " do not appear to be correct.";
                CPPUNIT_FAIL(msg.str());
            }

            if (a != 0 || b != 1 || c != 2) {
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
            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j) {
                    Perm<3> p(i, j);

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
                    if (i != j) {
                        if (p[3-i-j] != 3-i-j) {
                            std::ostringstream msg;
                            msg << "The (" << i << ", " << j << ") swap "
                                "gives the wrong image for " << 3-i-j << ".";
                            CPPUNIT_FAIL(msg.str());
                        }
                    } else {
                        if (p[(i+1)%3] != (i+1)%3) {
                            std::ostringstream msg;
                            msg << "The (" << i << ", " << j << ") swap "
                                "gives the wrong image for " << (i+1)%3 << ".";
                            CPPUNIT_FAIL(msg.str());
                        }
                        if (p[(i+2)%3] != (i+2)%3) {
                            std::ostringstream msg;
                            msg << "The (" << i << ", " << j << ") swap "
                                "gives the wrong image for " << (i+2)%3 << ".";
                            CPPUNIT_FAIL(msg.str());
                        }
                    }
                }
        }

        void products() {
            // A direct test.
            unsigned i, j, x;
            Perm<3> p, q, r;

            for (i = 0; i < 6; ++i) {
                p = Perm<3>::S3[i];
                for (j = 0; j < 6; ++j) {
                    q = Perm<3>::S3[j];

                    r = p * q;
                    for (x = 0; x < 3; ++x) {
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
            int a, b, c, d, e, f;
            for (a = 0; a < 3; ++a)
                for (b = 0; b < 3; ++b) {
                    if (b == a)
                        continue;
                    c = 3 - a - b;
                    Perm<3> x(a, b, c);

                    for (d = 0; d < 3; ++d)
                        for (e = 0; e < 3; ++e) {
                            if (e == d)
                                continue;
                            f = 3 - d - e;
                            Perm<3> y(d, e, f);

                            Perm<3> product3 = x * y;
                            regina::Perm<4> product4 =
                                regina::Perm<4>(a, b, c, 3) *
                                regina::Perm<4>(d, e, f, 3);

                            if (product3[0] != product4[0] ||
                                    product3[1] != product4[1] ||
                                    product3[2] != product4[2]) {
                                std::ostringstream msg;
                                msg << "The product is incorrect for "
                                    << x.str() << " * " << y.str() << ".";
                                CPPUNIT_FAIL(msg.str());
                            }
                        }
                }
        }

        void exhaustive() {
            Perm<3> id;
            if (! looksLikeIdentity(id))
                CPPUNIT_FAIL("The default Perm<3> constructor does not "
                    "appear to give the identity permutation.");

            // Test all possible permutations.
            int tested = 0;
            int a, b, c;
            for (a = 0; a < 3; ++a) {
                for (b = 0; b < 3; ++b) {
                    if (b == a)
                        continue;
                    c = (3 - a - b);
                    testPerm(a, b, c);
                    ++tested;
                }
            }

            if (tested != 6)
                CPPUNIT_FAIL("All 6 permutations in S(3) were not tested.");
        }

        void compareWith() {
            unsigned i, j;
            Perm<3> p, q;

            for (i = 0; i < 6; ++i) {
                p = Perm<3>::orderedS3[i];
                if (p.compareWith(p) != 0) {
                    std::ostringstream msg;
                    msg << "Routine compareWith() does not conclude that "
                        << p.str() << " == " << p.str() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            for (i = 0; i < 6; ++i) {
                p = Perm<3>::orderedS3[i];
                for (j = i + 1; j < 6; ++j) {
                    q = Perm<3>::orderedS3[j];

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
            for (int i = 0; i < 6; i++) {
                Perm<3> p = Perm<3>::S3[i];
                Perm<3> r = p.reverse();

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

            for (i = 0; i < 6; ++i)
                if (Perm<3>::S3[i] != Perm<3>::Sn[i])
                    CPPUNIT_FAIL("Arrays S3 and Sn disagree for Perm<3>.");

            for (i = 0; i < 2; ++i)
                if (Perm<3>::S2[i] != Perm<3>::Sn_1[i])
                    CPPUNIT_FAIL("Arrays S2 and Sn_1 disagree for Perm<3>.");
        }
};

void addPerm3(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(Perm3Test::suite());
}

