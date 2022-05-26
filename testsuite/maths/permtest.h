
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
#include <array>
#include <sstream>
#include <type_traits>
#include <cppunit/extensions/HelperMacros.h>
#include "maths/perm.h"
#include "testsuite/utilities/tightencodingtest.h"

using regina::Perm;

template <int n> unsigned long identityImagePack;
template <> inline unsigned long identityImagePack<4> = 228;
template <> inline unsigned long identityImagePack<5> = 18056;
template <> inline unsigned long identityImagePack<6> = 181896;
template <> inline unsigned long identityImagePack<7> = 1754760;

template <int n> const char* identityString;
template <> inline const char* identityString<2> = "01";
template <> inline const char* identityString<3> = "012";
template <> inline const char* identityString<4> = "0123";
template <> inline const char* identityString<5> = "01234";
template <> inline const char* identityString<6> = "012345";
template <> inline const char* identityString<7> = "0123456";

template <int n> Perm<n> lastPerm;
template <> inline Perm<2> lastPerm<2> { 1, 0 };
template <> inline Perm<3> lastPerm<3> { 2, 1, 0 };
template <> inline Perm<4> lastPerm<4> { 3, 2, 1, 0 };
template <> inline Perm<5> lastPerm<5> { 4, 3, 2, 1, 0 };
template <> inline Perm<6> lastPerm<6> { 5, 4, 3, 2, 1, 0 };
template <> inline Perm<7> lastPerm<7> { 6, 5, 4, 3, 2, 1, 0 };

template <int n> std::array<int, n> miscPermImg;
template <> inline std::array<int, 2> miscPermImg<2> { 1, 0 };
template <> inline std::array<int, 3> miscPermImg<3> { 2, 0, 1 };
template <> inline std::array<int, 4> miscPermImg<4> { 2, 3, 1, 0 };
template <> inline std::array<int, 5> miscPermImg<5> { 4, 2, 3, 0, 1 };
template <> inline std::array<int, 6> miscPermImg<6> { 4, 2, 3, 0, 5, 1 };
template <> inline std::array<int, 7> miscPermImg<7> { 4, 6, 2, 3, 0, 5, 1 };

/**
 * Inherited by the "small" permutation test classes, corresponding to
 * classes Perm<n> whose codes are indices into S_n.
 */
template <int n>
class SmallPermTest :
        public CppUnit::TestFixture, public TightEncodingTest<Perm<n>> {
    static_assert(Perm<n>::codeType == regina::PERM_CODE_INDEX);

    public:
        using TightEncodingTest<Perm<n>>::verifyTightEncoding;

    private:
        static constexpr bool requiresPrecompute = (n == 6 || n == 7);
        static constexpr bool usesCode2 = (n >= 4 && n <= 7);
        using Index = typename Perm<n>::Index;
        static constexpr Index nPerms = Perm<n>::nPerms;

    public:
        void setUp() override {
            if constexpr (requiresPrecompute) {
                Perm<n>::precompute();
            }
        }

        void tearDown() override {
        }

        void permCode() {
            for (Index i = 0; i < nPerms; ++i) {
                if constexpr (usesCode2) {
                    auto code = Perm<n>::Sn[i].permCode2();
                    if (code != i) {
                        std::ostringstream msg;
                        msg << "Permutation #" << i
                            << " has incorrect second-generation code "
                            << code << ".";
                        CPPUNIT_FAIL(msg.str());
                    }
                } else {
                    auto code = Perm<n>::Sn[i].permCode();
                    if (code != i) {
                        std::ostringstream msg;
                        msg << "Permutation #" << i << " has incorrect "
                            "permutation code " << code << ".";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
            }
        }

        void sign() {
            int expected;
            for (Index i = 0; i < nPerms; ++i) {
                expected = (i % 2 == 0 ? 1 : -1);
                if (Perm<n>::Sn[i].sign() != expected) {
                    std::ostringstream msg;
                    msg << "Permutation #" << i << " was found to have "
                        "sign " << Perm<n>::Sn[i].sign()
                        << " instead of " << expected << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void index() {
            for (Index i = 0; i < nPerms; ++i) {
                Perm osn = Perm<n>::orderedSn[i];
                Perm sn = Perm<n>::Sn[i];

                if (sn.SnIndex() != i) {
                    std::ostringstream msg;
                    msg << "Permutation Sn[" << i << "] gives an "
                        "incorrect Sn index of " << sn.SnIndex() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
                if (osn.orderedSnIndex() != i) {
                    std::ostringstream msg;
                    msg << "Permutation orderedSn[" << i << "] gives an "
                        "incorrect orderedSn index of "
                        << osn.orderedSnIndex() << ".";
                    CPPUNIT_FAIL(msg.str());
                }

                if (sn.sign() != (i % 2 == 0 ? 1 : -1)) {
                    std::ostringstream msg;
                    msg << "Permutation Sn[" << i << "] has the wrong sign.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (sn != osn) {
                    if (sn.orderedSnIndex() != (i ^ 1) ||
                            osn.SnIndex() != (i ^ 1)) {
                        std::ostringstream msg;
                        msg << "Permutation Sn/orderedSn[" << i << "] "
                            "differ by more than the last index bit.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
            }
        }

        bool looksLikeIdentity(const Perm<n>& p) {
            if ((! p.isIdentity()) || (! (p == Perm<n>())))
                return false;
            if (p.str() != identityString<n>)
                return false;
            if constexpr (usesCode2) {
                return (p.permCode1() == identityImagePack<n> &&
                    p.permCode2() == 0);
            } else {
                return p.permCode() == 0;
            }
        }

        bool looksEqual(const Perm<n>& p, const Perm<n>& q) {
            if (p != q || (! (p == q)) || p.str() != q.str())
                return false;
            if constexpr (usesCode2) {
                return p.permCode1() == q.permCode1() &&
                    p.permCode2() == q.permCode2();
            } else {
                return p.permCode() == q.permCode();
            }
        }

        bool looksEqual(const Perm<n>& p, const Perm<n>& q,
                const std::string& qStr) {
            if (p != q || (! (p == q)) || p.str() != q.str() || p.str() != qStr)
                return false;
            if constexpr (usesCode2) {
                return p.permCode1() == q.permCode1() &&
                    p.permCode2() == q.permCode2();
            } else {
                return p.permCode() == q.permCode();
            }
        }

        bool looksDistinct(const Perm<n>& p, const Perm<n>& q) {
            if (p == q || (! (p != q)) || p.str() == q.str())
                return false;
            if constexpr (usesCode2) {
                return p.permCode1() != q.permCode1() &&
                    p.permCode2() != q.permCode2();
            } else {
                return p.permCode() != q.permCode();
            }
        }

        int expectedSign(const Perm<n>& p) {
            // Count the number of reorderings.
            int reorderings = 0;

            int a, b;
            for (a = 0; a < n; ++a)
                for (b = a + 1; b < n; ++b)
                    if (p[a] > p[b])
                        ++reorderings;

            return ((reorderings % 2 == 0) ? 1 : -1);
        }

        // If args is a std::array<int, nArgs> containing { a0, a1, ... }, then
        // permWithConstructorArgs(args, std::make_index_sequence<nArgs>())
        // will return Perm<n>(a0, a1, ...).
        //
        // Basically this lets us construct a std::array and then use its
        // values as a sequence of constructor arguments, without needing
        // to hard-code the number of arguments into the syntax of the code.
        template <size_t nArgs, size_t... Indices>
        Perm<n> permWithConstructorArgs(const std::array<int, nArgs>& args,
                std::index_sequence<Indices...>) {
            return Perm<n>(std::get<Indices>(args)...);
        }

        void testPerm(const std::array<int, n>& img) {
            Perm<n> p(img);

            std::ostringstream name;
            for (int i = 0; i < n; ++i)
                name << img[i];

            if constexpr (usesCode2) {
                Perm<n> p1 = Perm<n>::fromPermCode1(p.permCode1());
                if (! looksEqual(p1, p, name.str())) {
                    std::ostringstream msg;
                    msg << "The first-generation code constructor fails for "
                        "the permutation " << p << " = " << name.str() << ".";
                    CPPUNIT_FAIL(msg.str());
                }

                Perm<n> p1b = Perm<n>::fromPermCode2(p.permCode2());
                if (! looksEqual(p1b, p, name.str())) {
                    std::ostringstream msg;
                    msg << "The second-generation code constructor fails for "
                        "the permutation " << name.str() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            } else {
                Perm<n> p1 = Perm<n>::fromPermCode(p.permCode());
                if (! looksEqual(p1, p, name.str())) {
                    std::ostringstream msg;
                    msg << "The code constructor fails for the permutation "
                        << p << " = " << name.str() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            if constexpr (n > 2) {
                // Test the n-argument and 2n-argument constructors.
                // This involves some template magic to convert an
                // integer array into a sequence of constructor arguments
                // whose length depends on n.

                // Note that Perm<2> does not have the usual 2-argument
                // constructor that takes { image(0), image(1) }, since
                // this would be confused with the pair swap constructor.
                // Perm<2> is likewise missing the 4-argument constructor
                // { a, image(a), b, image(b) } as well.

                std::array<int, n> args1 = img;
                Perm<n> p2a = permWithConstructorArgs(
                    args1, std::make_index_sequence<n>());
                if (! looksEqual(p2a, p, name.str())) {
                    std::ostringstream msg;
                    msg << "The " << n << "-argument constructor fails for "
                        "the permutation " << name.str() << ".";
                    CPPUNIT_FAIL(msg.str());
                }

                std::array<int, 2 * n> args2;
                for (int i = 0; i < n; ++i) {
                    args2[2 * i] = miscPermImg<n>[i];
                    args2[2 * i + 1] = img[miscPermImg<n>[i]];
                }
                Perm<n> p2b = permWithConstructorArgs(
                    args2, std::make_index_sequence<2 * n>());
                if (! looksEqual(p2b, p, name.str())) {
                    std::ostringstream msg;
                    msg << "The " << (2 * n) << "-argument constructor fails "
                        "for the permutation " << name.str() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            {
                std::array<int, n> arr = img;
                Perm<n> parr(arr);
                if (! looksEqual(parr, p, name.str())) {
                    std::ostringstream msg;
                    msg << "The array constructor fails for "
                        "the permutation " << name.str() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            /*
            {
                int arrA[n], arrB[n];
                for (int i = 0; i < n; ++i) {
                    arrA[i] = miscPermImg<n>[i];
                    arrB[i] = img[miscPermImg<n>[i]];
                }
                Perm<n> parr2(arrA, arrB);
                if (! looksEqual(parr2, p, name.str())) {
                    std::ostringstream msg;
                    msg << "The two-array constructor fails for "
                        "the permutation " << name.str() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }
            */

            Perm<n> p3(p);
            if (! looksEqual(p3, p, name.str())) {
                std::ostringstream msg;
                msg << "The copy constructor fails for "
                    "the permutation " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if constexpr (usesCode2) {
                if (! Perm<n>::isPermCode1(p.permCode1())) {
                    std::ostringstream msg;
                    msg << "Routine isPermCode1() suggests that the "
                        "permutation " << name.str()
                        << " has an invalid permutation code.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (! Perm<n>::isPermCode2(p.permCode2())) {
                    std::ostringstream msg;
                    msg << "Routine isPermCode2() suggests that the "
                        "permutation " << name.str()
                        << " has an invalid permutation code.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (Perm<n>::isPermCode1(0))
                    CPPUNIT_FAIL("Routine isPermCode1() suggests that 0 is a "
                        "valid first-generation code (which it is not).");

                if (! Perm<n>::isPermCode2(0))
                    CPPUNIT_FAIL("Routine isPermCode2() suggests that 0 is "
                        "not a valid second-generation code (which it is).");
            } else {
                if (! Perm<n>::isPermCode(p.permCode())) {
                    std::ostringstream msg;
                    msg << "Routine isPermCode() suggests that the "
                        "permutation " << name.str()
                        << " has an invalid permutation code.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (! Perm<n>::isPermCode(0))
                    CPPUNIT_FAIL("Routine isPermCode() suggests that 0 is "
                        "not a valid permutation code (which it is).");
            }

            Perm<n> p4(miscPermImg<n>);
            if (img != miscPermImg<n>) {
                if (! looksDistinct(p4, p)) {
                    std::ostringstream msg;
                    msg << "The equality/inequality tests fail for "
                        "miscPermImg and the permutation " << name.str() << ".";
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

            if constexpr (usesCode2) {
                Perm<n> p5(miscPermImg<n>);
                p5.setPermCode1(p3.permCode1());
                if (! looksEqual(p5, p, name.str())) {
                    std::ostringstream msg;
                    msg << "The setPermCode1() / permCode1() routines fail "
                        "for the permutation " << name.str() << ".";
                    CPPUNIT_FAIL(msg.str());
                }

                Perm<n> p6(miscPermImg<n>);
                p6.setPermCode2(p3.permCode2());
                if (! looksEqual(p6, p, name.str())) {
                    std::ostringstream msg;
                    msg << "The setPermCode2() / permCode2() routines fail "
                        "for the permutation " << name.str() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            } else {
                Perm<n> p5(miscPermImg<n>);
                p5.setPermCode(p3.permCode());
                if (! looksEqual(p5, p, name.str())) {
                    std::ostringstream msg;
                    msg << "The setPermCode() / permCode() routines fail "
                        "for the permutation " << name.str() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            if (! looksEqual(p * Perm<n>(), p)) {
                std::ostringstream msg;
                msg << "Multiplying permutation " << name.str()
                    << " by the identity does not give " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (! looksEqual(Perm<n>() * p, p)) {
                std::ostringstream msg;
                msg << "Multiplying the identity by permutation " << name.str()
                    << " does not give " << name.str() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            for (int i = 0; i < n - 1; ++i) {
                std::array<int, n> product = img;
                std::swap(product[i], product[i + 1]);

                if (! looksEqual(p * Perm<n>(i, i + 1), product)) {
                    std::ostringstream msg;
                    msg << "Multiplying permutation " << name.str()
                        << " by the swap (" << i << " <--> " << (i + 1)
                        << ") does not give the expected result.";
                    CPPUNIT_FAIL(msg.str());
                }
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

            Perm<n> inv = p.inverse();
            for (int i = 0; i < n; ++i)
                if (inv[img[i]] != i) {
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

            for (int i = 0; i < n; ++i)
                if (p[i] != img[i]) {
                    std::ostringstream msg;
                    msg << "The element images for permutation " << name.str()
                        << " do not appear to be correct.";
                    CPPUNIT_FAIL(msg.str());
                }

            for (int i = 0; i < n; ++i)
                if (p.pre(img[i]) != i) {
                    std::ostringstream msg;
                    msg << "The element preimages for permutation "
                        << name.str() << " do not appear to be correct.";
                    CPPUNIT_FAIL(msg.str());
                }

            {
                bool isFirst = true;
                for (int i = 0; i < n; ++i)
                    if (img[i] != i) {
                        isFirst = false;
                        break;
                    }
                if (! isFirst) {
                    Perm<n> id;
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
            }

            {
                bool isLast = true;
                for (int i = 0; i < n; ++i)
                    if (img[i] != n - 1 - i) {
                        isLast = false;
                        break;
                    }
                if (! isLast) {
                    if (p.compareWith(lastPerm<n>) != -1 ||
                            lastPerm<n>.compareWith(p) != 1) {
                        std::ostringstream msg;
                        msg << "Permutation " << name.str()
                            << " is not reported to be lexicographically "
                               "smaller than " << lastPerm<n> << ".";
                        CPPUNIT_FAIL(msg.str());
                    }
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
            Perm<n> id;
            if (! looksLikeIdentity(id))
                CPPUNIT_FAIL("The default permutation constructor does not "
                    "appear to give the identity permutation.");

            for (int i = 0; i < n; ++i) {
                Perm<n> p(i, i);
                if (! looksLikeIdentity(p)) {
                    std::ostringstream msg;
                    msg << "The permutation that swaps " << i
                        << " with itself does not appear to be the identity.";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            // Test all possible permutations.
            int tested = 0;
            std::array<int, n> img;
            for (int i = 0; i < n; ++i)
                img[i] = i;
            do {
                testPerm(img);
                ++tested;
            } while (std::next_permutation(img.begin(), img.end()));

            if (tested != nPerms) {
                std::ostringstream msg;
                msg << "All " << nPerms << " permutations in S("
                    << n << ") were not tested.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void swaps() {
            for (int i = 0; i < n; ++i)
                for (int j = 0; j < n; ++j) {
                    Perm<n> p(i, j);

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
                    for (int k = 0; k < n; ++k) {
                        if (k != i && k != j && p[k] != k) {
                            std::ostringstream msg;
                            msg << "The (" << i << ", " << j << ") swap "
                                "gives the wrong image for " << k << ".";
                            CPPUNIT_FAIL(msg.str());
                        }
                    }
                }
        }

        void products() {
            Perm<n> p, q, r;
            for (Index i = 0; i < nPerms; ++i) {
                p = Perm<n>::Sn[i];
                for (Index j = 0; j < nPerms; ++j) {
                    q = Perm<n>::Sn[j];

                    r = p * q;
                    for (int x = 0; x < n; ++x) {
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

        void cachedProducts() {
            Perm<n> p, q, r;
            for (Index i = 0; i < nPerms; ++i) {
                p = Perm<n>::Sn[i];
                for (Index j = 0; j < nPerms; ++j) {
                    q = Perm<n>::Sn[j];

                    r = p.cachedComp(q);
                    for (int x = 0; x < n; ++x) {
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
            Perm<n> p, q;

            for (Index i = 0; i < nPerms; ++i) {
                p = Perm<n>::orderedSn[i];
                if (p.compareWith(p) != 0) {
                    std::ostringstream msg;
                    msg << "Routine compareWith() does not conclude that "
                        << p.str() << " == " << p.str() << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            for (Index i = 0; i < nPerms; ++i) {
                p = Perm<n>::orderedSn[i];
                for (Index j = i + 1; j < Perm<n>::nPerms; ++j) {
                    q = Perm<n>::orderedSn[j];

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
            for (Index i = 0; i < nPerms; ++i) {
                Perm<n> p = Perm<n>::Sn[i];
                Perm<n> r = p.reverse();

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

        template <int from = n - 2>
        void clearFrom() {
            static_assert(2 <= from && from <= n - 2);

            // This routine tests all variants clear<from>(), ..., clear<2>().
            Perm<n> rev = Perm<n>().reverse();

            typename Perm<from>::Index i;
            typename Perm<n - from>::Index j;
            for (i = 0; i < Perm<from>::nPerms; ++i)
                for (j = 0; j < Perm<n - from>::nPerms; ++j) {
                    Perm<n> left = Perm<n>::extend(regina::Perm<from>::Sn[i]);
                    Perm<n> right = rev *
                        Perm<n>::extend(regina::Perm<n - from>::Sn[j]) * rev;
                    Perm<n> p = left * right;
                    p.clear(from);
                    if (! looksEqual(p, left)) {
                        std::ostringstream msg;
                        msg << "Clearing from position " << from
                            << " gives the wrong result.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }

            if constexpr (from > 2) {
                clearFrom<from - 1>();
            }
        }

        void clear() {
            Perm<n> rev = Perm<n>().reverse();

            for (Index i = 0; i < nPerms; ++i) {
                Perm<n> p = Perm<n>::Sn[i];
                p.clear(n);
                if (! looksEqual(p, Perm<n>::Sn[i])) {
                    std::ostringstream msg;
                    msg << "Clearing from position " << n
                        << " gives the wrong result.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
            if constexpr (n > 2) {
                for (typename Perm<n-1>::Index i = 0; i < Perm<n-1>::nPerms;
                        ++i) {
                    Perm<n> left = Perm<n>::extend(regina::Perm<n-1>::Sn[i]);
                    Perm<n> p = left;
                    p.clear(n - 1);
                    if (! looksEqual(p, left)) {
                        std::ostringstream msg;
                        msg << "Clearing from position " << (n - 1)
                            << " gives the wrong result.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }

                if constexpr (n > 3) {
                    clearFrom(); // tests clear<2..(n-2)>().
                }

                for (typename Perm<n-1>::Index j = 0; j < Perm<n-1>::nPerms;
                        ++j) {
                    Perm<n> p = rev *
                        Perm<n>::extend(Perm<n-1>::Sn[j]) * rev;
                    p.clear(1);
                    if (! looksLikeIdentity(p)) {
                        std::ostringstream msg;
                        msg << "Clearing from position 1 "
                            "gives the wrong result.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
            } else {
                // The n == 2 case: clear(1) can only send id -> id.
                Perm<n> id;
                id.clear(1);
                if (! looksLikeIdentity(id)) {
                    CPPUNIT_FAIL("Clearing from position 1 "
                        "gives the wrong result.");
                }
            }
            for (Index j = 0; j < nPerms; ++j) {
                Perm<n> p = Perm<n>::Sn[j];
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
            for (Index i = 0; i < nPerms; ++i) {
                Perm<n> p = Perm<n>::Sn[i];

                int j = 0;
                Perm<n> q;
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
            for (Index i = 0; i < nPerms; ++i) {
                Perm<n> p = Perm<n>::Sn[i];

                if (! p.pow(0).isIdentity()) {
                    std::ostringstream msg;
                    msg << "pow(" << p << ", 0) is not the identity."
                        << std::endl;
                    CPPUNIT_FAIL(msg.str());
                }
                {
                    Perm<n> q;
                    int j = 0;
                    do {
                        Perm<n> pow = p.pow(++j);
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
                    Perm<n> q;
                    int j = 0;
                    do {
                        Perm<n> pow = p.pow(--j);
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

        void cachedPow() {
            for (Index i = 0; i < nPerms; ++i) {
                Perm<n> p = Perm<n>::Sn[i];

                if (! p.cachedPow(0).isIdentity()) {
                    std::ostringstream msg;
                    msg << "cachedPow(" << p << ", 0) is not the identity."
                        << std::endl;
                    CPPUNIT_FAIL(msg.str());
                }
                {
                    Perm<n> q;
                    int j = 0;
                    do {
                        Perm<n> pow = p.cachedPow(++j);
                        q = q * p;
                        if (! looksEqual(pow, q)) {
                            std::ostringstream msg;
                            msg << "cachedPow(" << p << ", " << j
                                << ") is not " << q << "." << std::endl;
                            CPPUNIT_FAIL(msg.str());
                        }
                    } while (j < 2 * p.order());
                }
                {
                    Perm<n> q;
                    int j = 0;
                    do {
                        Perm<n> pow = p.cachedPow(--j);
                        q = q * p.inverse();
                        if (! looksEqual(pow, q)) {
                            std::ostringstream msg;
                            msg << "cachedPow(" << p << ", " << j
                                << ") is not " << q << "." << std::endl;
                            CPPUNIT_FAIL(msg.str());
                        }
                    } while (j > -2 * int(p.order()));
                }
            }
        }

        void rot() {
            int i, j;
            for (i = 0; i < n; ++i) {
                Perm<n> p = Perm<n>::rot(i);
                for (j = 0; j < n; ++j)
                    if (p[j] != (i + j) % n) {
                        std::ostringstream msg;
                        msg << "Rotation " << i << ", position " << j
                            << " gives the wrong image " << p[j] << ".";
                        CPPUNIT_FAIL(msg.str());
                    }
            }
        }

        void conjugacy() {
            for (Index i = 0; i < nPerms; ++i) {
                Perm<n> p = Perm<n>::Sn[i];

                // Manually decide if p is conjugacy minimal.
                bool min = true;
                for (Index j = 0; j < nPerms; ++j) {
                    Perm<n> q = Perm<n>::Sn[j];
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
            Index i = 0;
            Perm<n> p;
            Perm<n> q;

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

            if (i != nPerms) {
                std::ostringstream msg;
                msg << "Increment does not wrap around after "
                    << nPerms << " steps.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! q.isIdentity()) {
                CPPUNIT_FAIL("Preincrement and postincrement do not "
                    "wrap around together.");
            }
        }

        void tightEncoding() {
            for (Index i = 0; i < nPerms; ++i)
                verifyTightEncoding(Perm<n>::Sn[i]);
        }
};

