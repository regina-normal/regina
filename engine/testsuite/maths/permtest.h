
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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
#include "maths/perm.h"
#include "utilities/typeutils.h"

#include "utilities/tightencodingtest.h"

template <int n> const char* identityString;
template <> inline const char* identityString<2> = "01";
template <> inline const char* identityString<3> = "012";
template <> inline const char* identityString<4> = "0123";
template <> inline const char* identityString<5> = "01234";
template <> inline const char* identityString<6> = "012345";
template <> inline const char* identityString<7> = "0123456";
template <> inline const char* identityString<8> = "01234567";
template <> inline const char* identityString<9> = "012345678";
template <> inline const char* identityString<10> = "0123456789";
template <> inline const char* identityString<11> = "0123456789a";
template <> inline const char* identityString<12> = "0123456789ab";
template <> inline const char* identityString<13> = "0123456789abc";
template <> inline const char* identityString<14> = "0123456789abcd";
template <> inline const char* identityString<15> = "0123456789abcde";
template <> inline const char* identityString<16> = "0123456789abcdef";

template <int n> uint64_t identityImagePack;
template <> inline uint64_t identityImagePack<4> = 228;
template <> inline uint64_t identityImagePack<5> = 18056;
template <> inline uint64_t identityImagePack<6> = 181896;
template <> inline uint64_t identityImagePack<7> = 1754760;
template <> inline uint64_t identityImagePack<8> = 16434824;
template <> inline uint64_t identityImagePack<9> = 36344967696;
template <> inline uint64_t identityImagePack<10> = 654820258320;
template <> inline uint64_t identityImagePack<11> = 11649936536080;
template <> inline uint64_t identityImagePack<12> = 205163983024656;
template <> inline uint64_t identityImagePack<13> = 3582863703552528;
template <> inline uint64_t identityImagePack<14> = 62129658859368976;
template <> inline uint64_t identityImagePack<15> = 1070935975390360080;
template <> inline uint64_t identityImagePack<16> = 18364758544493064720u;

template <int n> regina::Perm<n> lastPerm;
template <> inline regina::Perm<2> lastPerm<2> { 1, 0 };
template <> inline regina::Perm<3> lastPerm<3> { 2, 1, 0 };
template <> inline regina::Perm<4> lastPerm<4> { 3, 2, 1, 0 };
template <> inline regina::Perm<5> lastPerm<5> { 4, 3, 2, 1, 0 };
template <> inline regina::Perm<6> lastPerm<6> { 5, 4, 3, 2, 1, 0 };
template <> inline regina::Perm<7> lastPerm<7> { 6, 5, 4, 3, 2, 1, 0 };

template <int n> std::array<int, n> miscPermImg;
template <> inline std::array<int, 2> miscPermImg<2> { 1, 0 };
template <> inline std::array<int, 3> miscPermImg<3> { 2, 0, 1 };
template <> inline std::array<int, 4> miscPermImg<4> { 2, 3, 1, 0 };
template <> inline std::array<int, 5> miscPermImg<5> { 4, 2, 3, 0, 1 };
template <> inline std::array<int, 6> miscPermImg<6> { 4, 2, 3, 0, 5, 1 };
template <> inline std::array<int, 7> miscPermImg<7> { 4, 6, 2, 3, 0, 5, 1 };

/**
 * Implements several tests for permutations on \a n elements, for all \a n.
 *
 * Test fixtures for individual \a n should use either GeneralPermTest<n> or
 * SmallPermTest<n> as a base class, since this base class provides some
 * useful constants and type aliases.
 */
template <int n>
class GeneralPermTest : public testing::Test {
    protected:
        using Index = typename regina::Perm<n>::Index;
        static constexpr Index nPerms = regina::Perm<n>::nPerms;
        static constexpr bool usesCode2 = (n >= 4 && n <= 7);
        static constexpr bool iterationFeasible = (n <= 11);

        GeneralPermTest() {
            if constexpr (iterationFeasible)
                regina::Perm<n>::precompute();
        }

        static bool looksLikeIdentity(const regina::Perm<n>& p) {
            if ((! p.isIdentity()) || (! (p == regina::Perm<n>())))
                return false;
            if (p.str() != identityString<n>)
                return false;
            if constexpr (usesCode2) {
                return (p.permCode1() == identityImagePack<n> &&
                    p.permCode2() == 0);
            } else if constexpr (regina::Perm<n>::codeType ==
                    regina::PERM_CODE_IMAGES) {
                return p.permCode() == identityImagePack<n>;
            } else {
                return p.permCode() == 0;
            }
        }

        static bool looksEqual(const regina::Perm<n>& p,
                const regina::Perm<n>& q) {
            if (p != q || (! (p == q)) || p.str() != q.str())
                return false;
            if constexpr (usesCode2) {
                return p.permCode1() == q.permCode1() &&
                    p.permCode2() == q.permCode2();
            } else {
                return p.permCode() == q.permCode();
            }
        }

        static bool looksEqual(const regina::Perm<n>& p,
                const regina::Perm<n>& q, const std::string& qStr) {
            if (p != q || (! (p == q)) || p.str() != q.str() || p.str() != qStr)
                return false;
            if constexpr (usesCode2) {
                return p.permCode1() == q.permCode1() &&
                    p.permCode2() == q.permCode2();
            } else {
                return p.permCode() == q.permCode();
            }
        }

        static bool looksDistinct(const regina::Perm<n>& p,
                const regina::Perm<n>& q) {
            if (p == q || (! (p != q)) || p.str() == q.str())
                return false;
            if constexpr (usesCode2) {
                return p.permCode1() != q.permCode1() &&
                    p.permCode2() != q.permCode2();
            } else {
                return p.permCode() != q.permCode();
            }
        }

        static void increment() {
            static_assert(iterationFeasible);

            Index i = 0;
            regina::Perm<n> p;
            regina::Perm<n> q;
            do {
                EXPECT_EQ(p, q);
                EXPECT_FALSE(p != q);
                EXPECT_EQ(p.SnIndex(), i);
                ++i; ++p; q++; // test both pre- and post-increments
            } while (! p.isIdentity());

            EXPECT_EQ(i, nPerms);
            EXPECT_TRUE(q.isIdentity());
        }

        static void cachedInverse() {
            static_assert(iterationFeasible);

            regina::Perm<n> p;
            do {
                EXPECT_EQ(p.inverse(), p.cachedInverse());
                ++p;
            } while (! p.isIdentity());
        }

        static void conjugacyMinimal() {
            static_assert(iterationFeasible);

            regina::Perm<n> p;
            do {
                // Manually decide if p is conjugacy minimal.
                bool min = true;
                int prevCycle = 0;
                int currCycle = 0;
                for (int j = 0; j < n; ++j) {
                    if (p[j] > j + 1) {
                        min = false;
                        break;
                    } else if (p[j] == j + 1) {
                        ++currCycle;
                    } else {
                        // We have closed off a cycle.
                        ++currCycle;
                        if (currCycle < prevCycle) {
                            min = false;
                            break;
                        }
                        prevCycle = currCycle;
                        currCycle = 0;
                    }
                }

                EXPECT_EQ(p.isConjugacyMinimal(), min);
                ++p;
            } while (! p.isIdentity());
        }
};

/**
 * Implements additional tests for the "small" permutation classes Perm<n>
 * whose codes are indices into S_n.
 */
template <int n>
class SmallPermTest : public GeneralPermTest<n> {
    static_assert(regina::Perm<n>::codeType == regina::PERM_CODE_INDEX);

    protected:
        using typename GeneralPermTest<n>::Index;
        using GeneralPermTest<n>::nPerms;
        using GeneralPermTest<n>::usesCode2;
        using GeneralPermTest<n>::looksLikeIdentity;
        using GeneralPermTest<n>::looksEqual;
        using GeneralPermTest<n>::looksDistinct;

        static void permCode() {
            for (Index i = 0; i < nPerms; ++i) {
                if constexpr (usesCode2)
                    EXPECT_EQ(regina::Perm<n>::Sn[i].permCode2(), i);
                else
                    EXPECT_EQ(regina::Perm<n>::Sn[i].permCode(), i);
            }

            if constexpr (usesCode2) {
                EXPECT_FALSE(regina::Perm<n>::isPermCode1(0));
                EXPECT_TRUE(regina::Perm<n>::isPermCode2(0));
            } else {
                EXPECT_TRUE(regina::Perm<n>::isPermCode(0));
            }
        }

        static void sign() {
            for (Index i = 0; i < nPerms; ++i)
                EXPECT_EQ(regina::Perm<n>::Sn[i].sign(), (i % 2 == 0 ? 1 : -1));
        }

        static void index() {
            for (Index i = 0; i < nPerms; ++i) {
                auto osn = regina::Perm<n>::orderedSn[i];
                auto sn = regina::Perm<n>::Sn[i];

                EXPECT_EQ(sn.SnIndex(), i);
                EXPECT_EQ(osn.orderedSnIndex(), i);
                EXPECT_EQ(sn.sign(), (i % 2 == 0 ? 1 : -1));
                if (sn != osn) {
                    EXPECT_EQ(sn.orderedSnIndex(), i ^ 1);
                    EXPECT_EQ(osn.SnIndex(), i ^ 1);
                }
            }
        }

        static void verifyPerm(const std::array<int, n>& img) {
            const regina::Perm<n> p(img);

            std::ostringstream nameStream;
            for (int i = 0; i < n; ++i)
                nameStream << img[i];
            std::string name = nameStream.str();

            SCOPED_TRACE_STDSTRING(name);

            // Stringification:

            EXPECT_EQ(p.str(), name);

            // Constructors:

            EXPECT_TRUE(looksEqual(regina::Perm<n>(p), p, name));
            EXPECT_TRUE(looksEqual(
                regina::Perm<n>(std::array<int, n>(img)), p, name));

            if constexpr (n > 2) {
                // Test the n-argument and 2n-argument constructors.
                //
                // Note that Perm<2> does not have the usual 2-argument
                // constructor that takes { image(0), image(1) }, since
                // this would be confused with the pair swap constructor.
                // Perm<2> is likewise missing the 4-argument constructor
                // { a, image(a), b, image(b) } as well.

                EXPECT_TRUE(looksEqual(
                    std::make_from_tuple<regina::Perm<n>>(img), p, name));

                std::array<int, 2 * n> args;
                for (int i = 0; i < n; ++i) {
                    args[2 * i] = miscPermImg<n>[i];
                    args[2 * i + 1] = img[miscPermImg<n>[i]];
                }
                EXPECT_TRUE(looksEqual(
                    std::make_from_tuple<regina::Perm<n>>(args), p, name));
            }

            // Permutation codes:

            if constexpr (usesCode2) {
                EXPECT_TRUE(looksEqual(
                    regina::Perm<n>::fromPermCode1(p.permCode1()), p, name));
                EXPECT_TRUE(looksEqual(
                    regina::Perm<n>::fromPermCode2(p.permCode2()), p, name));
            } else {
                EXPECT_TRUE(looksEqual(
                    regina::Perm<n>::fromPermCode(p.permCode()), p, name));
            }

            if constexpr (usesCode2) {
                EXPECT_TRUE(regina::Perm<n>::isPermCode1(p.permCode1()));
                EXPECT_TRUE(regina::Perm<n>::isPermCode2(p.permCode2()));
            } else {
                EXPECT_TRUE(regina::Perm<n>::isPermCode(p.permCode()));
            }

            // Setting permutations:

            {
                regina::Perm<n> p4(miscPermImg<n>);
                if (img != miscPermImg<n>)
                    EXPECT_TRUE(looksDistinct(p4, p));

                p4 = p;
                EXPECT_TRUE(looksEqual(p4, p, name));
            }

            if constexpr (usesCode2) {
                regina::Perm<n> q(miscPermImg<n>);
                q.setPermCode1(p.permCode1());
                EXPECT_TRUE(looksEqual(q, p, name));

                regina::Perm<n> r(miscPermImg<n>);
                r.setPermCode2(p.permCode2());
                EXPECT_TRUE(looksEqual(r, p, name));
            } else {
                regina::Perm<n> q(miscPermImg<n>);
                q.setPermCode(p.permCode());
                EXPECT_TRUE(looksEqual(q, p, name));
            }

            // Products and inverses:

            EXPECT_TRUE(looksEqual(p * regina::Perm<n>(), p));
            EXPECT_TRUE(looksEqual(regina::Perm<n>() * p, p));

            for (int i = 0; i < n - 1; ++i) {
                std::array<int, n> product = img;
                std::swap(product[i], product[i + 1]);

                EXPECT_TRUE(looksEqual(p * regina::Perm<n>(i, i + 1), product));
            }

            EXPECT_TRUE(looksLikeIdentity(p * p.inverse()));
            EXPECT_TRUE(looksLikeIdentity(p.inverse() * p));
            {
                auto inv = p.inverse();
                for (int i = 0; i < n; ++i)
                    EXPECT_EQ(inv[img[i]], i);
            }

            // Signs:

            {
                int reorderings = 0;
                for (int a = 0; a < n; ++a)
                    for (int b = a + 1; b < n; ++b)
                        if (p[a] > p[b])
                            ++reorderings;
                EXPECT_EQ(p.sign(), ((reorderings % 2 == 0) ? 1 : -1));
            }

            // Images and preimages:

            for (int i = 0; i < n; ++i) {
                EXPECT_EQ(p[i], img[i]);
                EXPECT_EQ(p.pre(img[i]), i);
            }

            // Ordering:

            {
                bool isFirst = true;
                for (int i = 0; i < n; ++i)
                    if (img[i] != i) {
                        isFirst = false;
                        break;
                    }
                if (! isFirst) {
                    EXPECT_EQ(p.compareWith(regina::Perm<n>()), 1);
                    EXPECT_EQ(regina::Perm<n>().compareWith(p), -1);
                    EXPECT_FALSE(p.isIdentity());
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
                    EXPECT_EQ(p.compareWith(lastPerm<n>), -1);
                    EXPECT_EQ(lastPerm<n>.compareWith(p), 1);
                }
            }
            EXPECT_EQ(p.compareWith(p), 0);
        }

        static void exhaustive() {
            // Test the identity permutation.
            EXPECT_TRUE(looksLikeIdentity(regina::Perm<n>()));
            for (int i = 0; i < n; ++i)
                EXPECT_TRUE(looksLikeIdentity(regina::Perm<n>(i, i)));

            // Test all possible permutations.
            int tested = 0;
            std::array<int, n> img;
            for (int i = 0; i < n; ++i)
                img[i] = i;
            do {
                verifyPerm(img);
                ++tested;
            } while (std::next_permutation(img.begin(), img.end()));

            EXPECT_EQ(tested, nPerms);
        }

        static void swaps() {
            for (int i = 0; i < n; ++i)
                for (int j = 0; j < n; ++j) {
                    regina::Perm<n> p(i, j);

                    EXPECT_EQ(p[i], j);
                    EXPECT_EQ(p[j], i);
                    for (int k = 0; k < n; ++k)
                        if (k != i && k != j)
                            EXPECT_EQ(p[k], k);
                }
        }

        static void products() {
            for (Index i = 0; i < nPerms; ++i) {
                auto p = regina::Perm<n>::Sn[i];
                for (Index j = 0; j < nPerms; ++j) {
                    auto q = regina::Perm<n>::Sn[j];
                    auto r = p * q;
                    for (int x = 0; x < n; ++x)
                        EXPECT_EQ(r[x], p[q[x]]);
                }
            }
        }

        static void cachedProducts() {
            for (Index i = 0; i < nPerms; ++i) {
                auto p = regina::Perm<n>::Sn[i];
                for (Index j = 0; j < nPerms; ++j) {
                    auto q = regina::Perm<n>::Sn[j];
                    auto r = p.cachedComp(q);
                    for (int x = 0; x < n; ++x)
                        EXPECT_EQ(r[x], p[q[x]]);
                }
            }
        }

        static void conjugates() {
            for (Index i = 0; i < nPerms; ++i) {
                auto p = regina::Perm<n>::Sn[i];
                for (Index j = 0; j < nPerms; ++j) {
                    auto q = regina::Perm<n>::Sn[j];
                    EXPECT_EQ(p.conjugate(q), q * p * q.inverse());
                }
            }
        }

        static void cachedConjugates() {
            for (Index i = 0; i < nPerms; ++i) {
                auto p = regina::Perm<n>::Sn[i];
                for (Index j = 0; j < nPerms; ++j) {
                    auto q = regina::Perm<n>::Sn[j];
                    EXPECT_EQ(p.cachedConjugate(q),
                        q.cachedComp(p).cachedComp(q.cachedInverse()));
                }
            }
        }

        static void compareWith() {
            for (Index i = 0; i < nPerms; ++i) {
                auto p = regina::Perm<n>::orderedSn[i];
                EXPECT_EQ(p.compareWith(p), 0);
            }

            for (Index i = 0; i < nPerms; ++i) {
                auto p = regina::Perm<n>::orderedSn[i];
                for (Index j = i + 1; j < nPerms; ++j) {
                    auto q = regina::Perm<n>::orderedSn[j];
                    EXPECT_EQ(p.compareWith(q), -1);
                    EXPECT_EQ(q.compareWith(p), 1);
                }
            }
        }

        static void reverse() {
            for (Index i = 0; i < nPerms; ++i) {
                auto p = regina::Perm<n>::Sn[i];
                auto r = p.reverse();

                EXPECT_TRUE(looksEqual(p, r.reverse()));
                EXPECT_TRUE(looksDistinct(p, r));

                std::string s = p.str();
                std::reverse(s.begin(), s.end());
                EXPECT_EQ(s, r.str());
            }
        }

        static void clear() {
            auto rev = regina::Perm<n>().reverse();

            for (Index i = 0; i < nPerms; ++i) {
                auto p = regina::Perm<n>::Sn[i];
                p.clear(n);
                EXPECT_TRUE(looksEqual(p, regina::Perm<n>::Sn[i]));
            }
            if constexpr (n > 2) {
                for (typename regina::Perm<n-1>::Index i = 0;
                        i < regina::Perm<n-1>::nPerms; ++i) {
                    const auto left = regina::Perm<n>::extend(
                        regina::Perm<n-1>::Sn[i]);
                    regina::Perm<n> p = left;
                    p.clear(n - 1);
                    EXPECT_TRUE(looksEqual(p, left));
                }

                if constexpr (n > 3) {
                    // Test clear<2..(n-2)>():
                    regina::for_constexpr<2, n-1>([](auto from) {
                        auto rev = regina::Perm<n>().reverse();

                        for (typename regina::Perm<from>::Index i = 0;
                                i < regina::Perm<from>::nPerms; ++i)
                            for (typename regina::Perm<n - from>::Index j = 0;
                                    j < regina::Perm<n - from>::nPerms; ++j) {
                                auto left = regina::Perm<n>::extend(
                                    regina::Perm<from>::Sn[i]);
                                auto right = rev * regina::Perm<n>::extend(
                                    regina::Perm<n - from>::Sn[j]) * rev;
                                auto p = left * right;
                                p.clear(from);
                                EXPECT_TRUE(looksEqual(p, left));
                            }
                    });
                }

                for (typename regina::Perm<n-1>::Index j = 0;
                        j < regina::Perm<n-1>::nPerms; ++j) {
                    auto p = rev *
                        regina::Perm<n>::extend(regina::Perm<n-1>::Sn[j]) * rev;
                    p.clear(1);
                    EXPECT_TRUE(looksLikeIdentity(p));
                }
            } else {
                // The n == 2 case: clear(1) can only send id -> id.
                regina::Perm<n> id;
                id.clear(1);
                EXPECT_TRUE(looksLikeIdentity(id));
            }
            for (Index j = 0; j < nPerms; ++j) {
                auto p = regina::Perm<n>::Sn[j];
                p.clear(0);
                EXPECT_TRUE(looksLikeIdentity(p));
            }
        }

        static void order() {
            for (Index i = 0; i < nPerms; ++i) {
                auto p = regina::Perm<n>::Sn[i];

                int j = 0;
                regina::Perm<n> q;
                do {
                    q = q * p;
                    ++j;
                } while (! q.isIdentity());

                EXPECT_EQ(j, p.order());
            }
        }

        static void pow() {
            for (Index i = 0; i < nPerms; ++i) {
                auto p = regina::Perm<n>::Sn[i];

                EXPECT_TRUE(p.pow(0).isIdentity());
                {
                    regina::Perm<n> q;
                    int j = 0;
                    do {
                        auto pow = p.pow(++j);
                        q = q * p;
                        EXPECT_TRUE(looksEqual(pow, q));
                    } while (j < 2 * p.order());
                }
                {
                    regina::Perm<n> q;
                    int j = 0;
                    do {
                        auto pow = p.pow(--j);
                        q = q * p.inverse();
                        EXPECT_TRUE(looksEqual(pow, q));
                    } while (j > -2 * static_cast<int>(p.order()));
                }
            }
        }

        static void cachedPow() {
            for (Index i = 0; i < nPerms; ++i) {
                auto p = regina::Perm<n>::Sn[i];

                EXPECT_TRUE(p.cachedPow(0).isIdentity());
                {
                    regina::Perm<n> q;
                    int j = 0;
                    do {
                        auto pow = p.cachedPow(++j);
                        q = q * p;
                        EXPECT_TRUE(looksEqual(pow, q));
                    } while (j < 2 * p.order());
                }
                {
                    regina::Perm<n> q;
                    int j = 0;
                    do {
                        auto pow = p.cachedPow(--j);
                        q = q * p.inverse();
                        EXPECT_TRUE(looksEqual(pow, q));
                    } while (j > -2 * static_cast<int>(p.order()));
                }
            }
        }

        static void rot() {
            for (int i = 0; i < n; ++i) {
                auto p = regina::Perm<n>::rot(i);
                for (int j = 0; j < n; ++j)
                    EXPECT_EQ(p[j], (i + j) % n);
            }
        }

        static void tightEncoding() {
            for (Index i = 0; i < nPerms; ++i)
                TightEncodingTest<regina::Perm<n>>::verifyTightEncoding(
                    regina::Perm<n>::Sn[i]);
        }
};

