
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
#include "maths/nperm3.h"
#include "triangulation/nperm.h"
#include "testsuite/maths/testmaths.h"

using regina::NPerm3;

class NPerm3Test : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NPerm3Test);

    CPPUNIT_TEST(inverse);
    CPPUNIT_TEST(sign);
    CPPUNIT_TEST(index);
    CPPUNIT_TEST(products);
    CPPUNIT_TEST(exhaustive);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp() {
        }

        void tearDown() {
        }

        void inverse() {
            for (int i = 0; i < 6; i++) {
                if (NPerm3::S3[i].inverse() != NPerm3::S3[NPerm3::invS3[i]]) {
                    std::ostringstream msg;
                    msg << "Permutation #" << i << " was found to have "
                        "inverse " << NPerm3::S3[i].inverse() <<
                        " instead of " << NPerm3::S3[NPerm3::invS3[i]] << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void sign() {
            int expected;
            for (int i = 0; i < 6; ++i) {
                expected = (i % 2 == 0 ? 1 : -1);
                if (NPerm3::S3[i].sign() != expected) {
                    std::ostringstream msg;
                    msg << "Permutation #" << i << " was found to have "
                        "sign " << NPerm3::S3[i].sign()
                        << " instead of " << expected << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void index() {
            for (int i = 0; i < 6; ++i) {
                if (NPerm3::S3[i].S3Index() != i) {
                    std::ostringstream msg;
                    msg << "Permutation S3[" << i << "] gives an "
                        "incorrect S3 index of "
                        << NPerm3::S3[i].S3Index() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
                if (NPerm3::orderedS3[i].orderedS3Index() != i) {
                    std::ostringstream msg;
                    msg << "Permutation orderedS3[" << i << "] gives an "
                        "incorrect orderedS3 index of "
                        << NPerm3::orderedS3[i].orderedS3Index() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        bool looksLikeIdentity(const NPerm3& p) {
            return (p.isIdentity() && p == NPerm3() &&
                p.getPermCode() == 0 && p.toString() == "012");
        }

        bool looksEqual(const NPerm3& p, const NPerm3& q) {
            return (p == q && (! (p != q)) && p.toString() == q.toString() &&
                p.getPermCode() == q.getPermCode());
        }

        bool looksEqual(const NPerm3& p, const NPerm3& q,
                const std::string& qStr) {
            return (p == q && (! (p != q)) && p.toString() == q.toString() &&
                p.getPermCode() == q.getPermCode() && p.toString() == qStr);
        }

        bool looksDistinct(const NPerm3& p, const NPerm3& q) {
            return (p != q && (! (p == q)) && p.toString() != q.toString() &&
                p.getPermCode() != q.getPermCode());
        }

        int expectedSign(const NPerm3& p) {
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
            NPerm3 p(a, b, c);

            std::ostringstream name;
            name << a << b << c;

            NPerm3 p1(p.getPermCode());
            if (! looksEqual(p1, p, name.str())) {
                std::ostringstream msg;
                msg << "The internal code constructor fails for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            NPerm3 p3(p);
            if (! looksEqual(p3, p, name.str())) {
                std::ostringstream msg;
                msg << "The copy constructor fails for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            NPerm3 p4(2, 0, 1);
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

            NPerm3 p5(2, 0, 1);
            p5.setPermCode(p3.getPermCode());
            if (! looksEqual(p5, p, name.str())) {
                std::ostringstream msg;
                msg << "The setPermCode() / getPermCode() routines fail for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (! NPerm3::isPermCode(p.getPermCode())) {
                std::ostringstream msg;
                msg << "Routine isPermCode() suggests that the permutation "
                    << name.str() << " has an invalid permutation code.";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(p * NPerm3(), p)) {
                std::ostringstream msg;
                msg << "Multiplying permutation " << name.str()
                    << " by the identity does not give " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(NPerm3() * p, p)) {
                std::ostringstream msg;
                msg << "Multiplying the identity by permutation " << name.str()
                    << " does not give " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(p * NPerm3(NPerm3::code102), NPerm3(b, a, c))) {
                std::ostringstream msg;
                msg << "Multiplying permutation " << name.str()
                    << " by (0 <--> 1) does not give the expected result.";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(p * NPerm3(NPerm3::code021), NPerm3(a, c, b))) {
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

            NPerm3 inv = p.inverse();
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

            if (p.toString() != name.str()) {
                std::ostringstream msg;
                msg << "The stringification for permutation " << name.str()
                    << " does not appear to be correct.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void products() {
            int a, b, c, d, e, f;

            for (a = 0; a < 3; ++a)
                for (b = 0; b < 3; ++b) {
                    if (b == a)
                        continue;
                    c = 3 - a - b;
                    NPerm3 x(a, b, c);

                    for (d = 0; d < 3; ++d)
                        for (e = 0; e < 3; ++e) {
                            if (e == d)
                                continue;
                            f = 3 - d - e;
                            NPerm3 y(d, e, f);

                            NPerm3 product3 = x * y;
                            regina::NPerm product4 =
                                regina::NPerm(a, b, c, 3) *
                                regina::NPerm(d, e, f, 3);

                            if (product3[0] != product4[0] ||
                                    product3[1] != product4[1] ||
                                    product3[2] != product4[2]) {
                                std::ostringstream msg;
                                msg << "The product is incorrect for "
                                    << x.toString() << " * "
                                    << y.toString() << ".";
                                CPPUNIT_FAIL(msg.str());
                            }
                        }
                }
        }

        void exhaustive() {
            NPerm3 id;
            if (! looksLikeIdentity(id))
                CPPUNIT_FAIL("The default NPerm3 constructor does not "
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
};

void addNPerm3(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NPerm3Test::suite());
}

