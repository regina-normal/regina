
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
#include "triangulation/facenumbering.h"
#include "utilities/typeutils.h"

#include "utilities/tightencodingtest.h"

using regina::Perm;

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

static constexpr int64_t increment[] = {
    // Used to run through a sample of permutations when n is large and we
    // cannot afford to test all n! possible permutations.
    //
    // For all of the increments that are > 1:
    // - increment[n] is coprime with (n!);
    // - (n!) % increment[n] > 1;
    // - (n!) / increment[n] is a few hundred.
    //
    // The intent is to iterate through orderedSn[i] where i is a multiple of
    // increment[n], as well as the special case i = n!-1.  We use orderedSn
    // instead of Sn because for large permutation classes, orderedSn involves
    // less work to compute (so should be faster).

    1, 1, 1, 1, 1, 1, 1, /* n=0..6 */
    11, 143, 2431, 12673, 96577, 1255501, 55190041, /* n=7..13 */
    247110827, 4200884059, 59553709307 /* n=14..16 */
};

/**
 * Implements tests that we can use for permutations on \a n elements,
 * for all \a n.
 */
template <int n>
class PermTestImpl {
    protected:
        using Index = typename Perm<n>::Index;
        static constexpr bool usesCode2 = (n >= 4 && n <= 7);
        static constexpr bool iterationFeasible = (n <= 10);

        static bool looksLikeIdentity(const Perm<n>& p) {
            if ((! p.isIdentity()) || (! (p == Perm<n>())))
                return false;
            if (p.str() != identityString<n>)
                return false;
            if constexpr (usesCode2) {
                return (p.permCode1() == identityImagePack<n> &&
                    p.permCode2() == 0);
            } else if constexpr (Perm<n>::codeType ==
                    regina::PermCodeType::Images) {
                return p.permCode() == identityImagePack<n>;
            } else {
                return p.permCode() == 0;
            }
        }

        static bool looksEqual(const Perm<n>& p, const Perm<n>& q) {
            if (p != q || (! (p == q)) || p.str() != q.str())
                return false;
            if constexpr (usesCode2) {
                return p.permCode1() == q.permCode1() &&
                    p.permCode2() == q.permCode2();
            } else {
                return p.permCode() == q.permCode();
            }
        }

        static bool looksEqual(const Perm<n>& p, const Perm<n>& q,
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

        static bool looksDistinct(const Perm<n>& p, const Perm<n>& q) {
            if (p == q || (! (p != q)) || p.str() == q.str())
                return false;
            if constexpr (usesCode2) {
                return p.permCode1() != q.permCode1() &&
                    p.permCode2() != q.permCode2();
            } else {
                return p.permCode() != q.permCode();
            }
        }

    public:
        static void swaps() {
            SCOPED_TRACE_NUMERIC(n);
            for (int i = 0; i < n; ++i)
                for (int j = 0; j < n; ++j) {
                    Perm<n> p(i, j);

                    EXPECT_EQ(p[i], j);
                    EXPECT_EQ(p[j], i);
                    for (int k = 0; k < n; ++k)
                        if (k != i && k != j)
                            EXPECT_EQ(p[k], k);
                }
        }

        static void increment() {
            static_assert(iterationFeasible);
            SCOPED_TRACE_NUMERIC(n);

            Index i = 0;
            Perm<n> p;
            Perm<n> q;
            for (auto r : Perm<n>::Sn) {
                EXPECT_EQ(p, r);
                EXPECT_EQ(q, r);
                EXPECT_FALSE(p != q);
                EXPECT_EQ(r.SnIndex(), i);
                ++i; ++p; q++; // test both pre- and post-increments
            }

            EXPECT_EQ(i, Perm<n>::nPerms);
            EXPECT_TRUE(p.isIdentity());
            EXPECT_TRUE(q.isIdentity());
        }

        static void iterationSign() {
            static_assert(iterationFeasible);
            SCOPED_TRACE_NUMERIC(n);

            Index i = 0;
            auto p = Perm<n>::Sn.begin();
            auto q = Perm<n>::Sn.begin();
            for (auto r : Perm<n>::Sn) {
                EXPECT_TRUE(p);
                EXPECT_TRUE(q);
                EXPECT_EQ(*p, r);
                EXPECT_EQ(*q, r);
                EXPECT_FALSE(*p != *q);
                EXPECT_EQ(r.SnIndex(), i);
                ++i; ++p; q++; // test both pre- and post-increments
            }

            EXPECT_EQ(i, Perm<n>::nPerms);
            EXPECT_EQ(p, Perm<n>::Sn.end());
            EXPECT_EQ(q, Perm<n>::Sn.end());
        }

        static void iterationLex() {
            static_assert(iterationFeasible);
            SCOPED_TRACE_NUMERIC(n);

            Index i = 0;
            auto p = Perm<n>::orderedSn.begin();
            auto q = Perm<n>::orderedSn.begin();
            for (auto r : Perm<n>::orderedSn) {
                EXPECT_TRUE(p);
                EXPECT_TRUE(q);
                EXPECT_EQ(*p, r);
                EXPECT_EQ(*q, r);
                EXPECT_FALSE(*p != *q);
                EXPECT_EQ(r.orderedSnIndex(), i);
                ++i; ++p; q++; // test both pre- and post-increments
            }

            EXPECT_EQ(i, Perm<n>::nPerms);
            EXPECT_EQ(p, Perm<n>::orderedSn.end());
            EXPECT_EQ(q, Perm<n>::orderedSn.end());
        }

        static void cachedInverse() {
            static_assert(iterationFeasible);
            SCOPED_TRACE_NUMERIC(n);

            for (auto p : Perm<n>::Sn) {
                EXPECT_EQ(p.inverse(), p.cachedInverse());
            }
        }

        static void conjugacyMinimal() {
            static_assert(iterationFeasible);
            SCOPED_TRACE_NUMERIC(n);

            for (auto p : Perm<n>::Sn) {
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
            }
        }

        static void rot() {
            SCOPED_TRACE_NUMERIC(n);
            for (int i = 0; i < n; ++i) {
                auto p = Perm<n>::rot(i);
                for (int j = 0; j < n; ++j)
                    EXPECT_EQ(p[j], (i + j) % n);
            }
        }
};

/**
 * Implements additional tests for the "small" permutation classes Perm<n>
 * whose codes are indices into S_n.
 */
template <int n>
class PermTestSmallImpl : public PermTestImpl<n> {
    static_assert(Perm<n>::codeType == regina::PermCodeType::Index);

    protected:
        using typename PermTestImpl<n>::Index;
        using PermTestImpl<n>::usesCode2;
        using PermTestImpl<n>::looksLikeIdentity;
        using PermTestImpl<n>::looksEqual;
        using PermTestImpl<n>::looksDistinct;

    public:
        static void permCode() {
            SCOPED_TRACE_NUMERIC(n);

            for (Index i = 0; i < Perm<n>::nPerms; ++i) {
                if constexpr (usesCode2)
                    EXPECT_EQ(Perm<n>::Sn[i].permCode2(), i);
                else
                    EXPECT_EQ(Perm<n>::Sn[i].permCode(), i);
            }

            if constexpr (usesCode2) {
                EXPECT_FALSE(Perm<n>::isPermCode1(0));
                EXPECT_TRUE(Perm<n>::isPermCode2(0));
            } else {
                EXPECT_TRUE(Perm<n>::isPermCode(0));
            }
        }

        static void sign() {
            SCOPED_TRACE_NUMERIC(n);
            for (Index i = 0; i < Perm<n>::nPerms; ++i)
                EXPECT_EQ(Perm<n>::Sn[i].sign(), (i % 2 == 0 ? 1 : -1));
        }

        static void index() {
            SCOPED_TRACE_NUMERIC(n);
            for (Index i = 0; i < Perm<n>::nPerms; ++i) {
                auto osn = Perm<n>::orderedSn[i];
                auto sn = Perm<n>::Sn[i];

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
            const Perm<n> p(img);

            std::ostringstream nameStream;
            for (int i = 0; i < n; ++i)
                nameStream << img[i];
            std::string name = nameStream.str();

            SCOPED_TRACE_STDSTRING(name);

            // Stringification:

            EXPECT_EQ(p.str(), name);

            // Constructors:

            EXPECT_TRUE(looksEqual(Perm<n>(p), p, name));
            EXPECT_TRUE(looksEqual(Perm<n>(std::array<int, n>(img)), p, name));

            if constexpr (n > 2) {
                // Test the n-argument and 2n-argument constructors.
                //
                // Note that Perm<2> does not have the usual 2-argument
                // constructor that takes { image(0), image(1) }, since
                // this would be confused with the pair swap constructor.
                // Perm<2> is likewise missing the 4-argument constructor
                // { a, image(a), b, image(b) } as well.

                EXPECT_TRUE(looksEqual(
                    std::make_from_tuple<Perm<n>>(img), p, name));

                std::array<int, 2 * n> args;
                for (int i = 0; i < n; ++i) {
                    args[2 * i] = miscPermImg<n>[i];
                    args[2 * i + 1] = img[miscPermImg<n>[i]];
                }
                EXPECT_TRUE(looksEqual(
                    std::make_from_tuple<Perm<n>>(args), p, name));
            }

            // Permutation codes:

            if constexpr (usesCode2) {
                EXPECT_TRUE(looksEqual(
                    Perm<n>::fromPermCode1(p.permCode1()), p, name));
                EXPECT_TRUE(looksEqual(
                    Perm<n>::fromPermCode2(p.permCode2()), p, name));
            } else {
                EXPECT_TRUE(looksEqual(
                    Perm<n>::fromPermCode(p.permCode()), p, name));
            }

            if constexpr (usesCode2) {
                EXPECT_TRUE(Perm<n>::isPermCode1(p.permCode1()));
                EXPECT_TRUE(Perm<n>::isPermCode2(p.permCode2()));
            } else {
                EXPECT_TRUE(Perm<n>::isPermCode(p.permCode()));
            }

            // Setting permutations:

            {
                Perm<n> q(miscPermImg<n>);
                if (img != miscPermImg<n>)
                    EXPECT_TRUE(looksDistinct(q, p));

                q = p;
                EXPECT_TRUE(looksEqual(q, p, name));
            }

            if constexpr (usesCode2) {
                Perm<n> q(miscPermImg<n>);
                q.setPermCode1(p.permCode1());
                EXPECT_TRUE(looksEqual(q, p, name));

                Perm<n> r(miscPermImg<n>);
                r.setPermCode2(p.permCode2());
                EXPECT_TRUE(looksEqual(r, p, name));
            } else {
                Perm<n> q(miscPermImg<n>);
                q.setPermCode(p.permCode());
                EXPECT_TRUE(looksEqual(q, p, name));
            }

            // Products and inverses:

            EXPECT_TRUE(looksEqual(p * Perm<n>(), p));
            EXPECT_TRUE(looksEqual(Perm<n>() * p, p));

            for (int i = 0; i < n - 1; ++i) {
                std::array<int, n> product = img;
                std::swap(product[i], product[i + 1]);

                EXPECT_TRUE(looksEqual(p * Perm<n>(i, i + 1), product));
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
                    EXPECT_EQ(p.compareWith(Perm<n>()), 1);
                    EXPECT_EQ(Perm<n>().compareWith(p), -1);
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
            SCOPED_TRACE_NUMERIC(n);

            // Test the identity permutation.
            EXPECT_TRUE(looksLikeIdentity(Perm<n>()));
            for (int i = 0; i < n; ++i)
                EXPECT_TRUE(looksLikeIdentity(Perm<n>(i, i)));

            // Test all possible permutations.
            int tested = 0;
            std::array<int, n> img;
            for (int i = 0; i < n; ++i)
                img[i] = i;
            do {
                verifyPerm(img);
                ++tested;
            } while (std::next_permutation(img.begin(), img.end()));

            EXPECT_EQ(tested, Perm<n>::nPerms);
        }

        static void products() {
            SCOPED_TRACE_NUMERIC(n);
            for (Index i = 0; i < Perm<n>::nPerms; ++i) {
                auto p = Perm<n>::Sn[i];
                for (Index j = 0; j < Perm<n>::nPerms; ++j) {
                    auto q = Perm<n>::Sn[j];
                    auto r = p * q;
                    for (int x = 0; x < n; ++x)
                        EXPECT_EQ(r[x], p[q[x]]);
                }
            }
        }

        static void cachedProducts() {
            SCOPED_TRACE_NUMERIC(n);

            for (Index i = 0; i < Perm<n>::nPerms; ++i) {
                auto p = Perm<n>::Sn[i];
                for (Index j = 0; j < Perm<n>::nPerms; ++j) {
                    auto q = Perm<n>::Sn[j];
                    auto r = p.cachedComp(q);
                    for (int x = 0; x < n; ++x)
                        EXPECT_EQ(r[x], p[q[x]]);
                }
            }
        }

        static void conjugates() {
            SCOPED_TRACE_NUMERIC(n);
            for (Index i = 0; i < Perm<n>::nPerms; ++i) {
                auto p = Perm<n>::Sn[i];
                for (Index j = 0; j < Perm<n>::nPerms; ++j) {
                    auto q = Perm<n>::Sn[j];
                    EXPECT_EQ(p.conjugate(q), q * p * q.inverse());
                }
            }
        }

        static void cachedConjugates() {
            SCOPED_TRACE_NUMERIC(n);

            for (Index i = 0; i < Perm<n>::nPerms; ++i) {
                auto p = Perm<n>::Sn[i];
                for (Index j = 0; j < Perm<n>::nPerms; ++j) {
                    auto q = Perm<n>::Sn[j];
                    EXPECT_EQ(p.cachedConjugate(q),
                        q.cachedComp(p).cachedComp(q.cachedInverse()));
                }
            }
        }

        static void compareWith() {
            SCOPED_TRACE_NUMERIC(n);

            const auto orderedSn = Perm<n>::orderedSn;

            for (auto p: orderedSn)
                EXPECT_EQ(p.compareWith(p), 0);

            for (auto i = orderedSn.begin(); i != orderedSn.end(); ++i) {
                auto p = *i;
                for (auto j = std::next(i); j != orderedSn.end(); ++j) {
                    auto q = *j;
                    EXPECT_EQ(p.compareWith(q), -1);
                    EXPECT_EQ(q.compareWith(p), 1);
                }
            }
        }

        static void reverse() {
            SCOPED_TRACE_NUMERIC(n);
            for (Index i = 0; i < Perm<n>::nPerms; ++i) {
                auto p = Perm<n>::Sn[i];
                auto r = p.reverse();

                EXPECT_TRUE(looksEqual(p, r.reverse()));
                EXPECT_TRUE(looksDistinct(p, r));

                std::string s = p.str();
                std::reverse(s.begin(), s.end());
                EXPECT_EQ(s, r.str());
            }
        }

        static void clear() {
            SCOPED_TRACE_NUMERIC(n);
            // In gcc7, the constexpr rev seems to cause a linker error for
            // the case n=7 because the lambda wants it to be instantiated.
            // Therefore, under gcc7, we make it a stack variable and capture it
            // in the lambda.  We can drop this hack when we drop gcc7 support.
#if defined(__GNUC__) && __GNUC__ == 7
            const Perm<n> rev = Perm<n>().reverse();
#else
            static constexpr Perm<n> rev = Perm<n>().reverse();
#endif

            for (Index i = 0; i < Perm<n>::nPerms; ++i) {
                auto p = Perm<n>::Sn[i];
                p.clear(n);
                EXPECT_TRUE(looksEqual(p, Perm<n>::Sn[i]));
            }
            if constexpr (n > 2) {
                for (typename Perm<n-1>::Index i = 0; i < Perm<n-1>::nPerms;
                        ++i) {
                    const auto left = Perm<n>::extend(Perm<n-1>::Sn[i]);
                    Perm<n> p = left;
                    p.clear(n - 1);
                    EXPECT_TRUE(looksEqual(p, left));
                }

                if constexpr (n > 3) {
                    // Test clear<2..(n-2)>():
#if defined(__GNUC__) && __GNUC__ == 7
                    regina::for_constexpr<2, n-1>([rev](auto from) {
#else
                    regina::for_constexpr<2, n-1>([](auto from) {
#endif
                        SCOPED_TRACE_NUMERIC(from);
                        for (typename Perm<from>::Index i = 0;
                                i < Perm<from>::nPerms; ++i)
                            for (typename Perm<n - from>::Index j = 0;
                                    j < Perm<n - from>::nPerms; ++j) {
                                auto left = Perm<n>::extend(Perm<from>::Sn[i]);
                                auto right = rev * Perm<n>::extend(
                                    Perm<n - from>::Sn[j]) * rev;
                                auto p = left * right;
                                p.clear(from);
                                EXPECT_TRUE(looksEqual(p, left));
                            }
                    });
                }

                for (typename Perm<n-1>::Index j = 0; j < Perm<n-1>::nPerms;
                        ++j) {
                    auto p = rev * Perm<n>::extend(Perm<n-1>::Sn[j]) * rev;
                    p.clear(1);
                    EXPECT_TRUE(looksLikeIdentity(p));
                }
            } else {
                // The n == 2 case: clear(1) can only send id -> id.
                Perm<n> id;
                id.clear(1);
                EXPECT_TRUE(looksLikeIdentity(id));
            }
            for (Index j = 0; j < Perm<n>::nPerms; ++j) {
                auto p = Perm<n>::Sn[j];
                p.clear(0);
                EXPECT_TRUE(looksLikeIdentity(p));
            }
        }

        static void order() {
            SCOPED_TRACE_NUMERIC(n);

            Perm<n> p;
            do {
                int j = 0;
                Perm<n> q;
                do {
                    q = q * p;
                    ++j;
                } while (! q.isIdentity());
                EXPECT_EQ(j, p.order());
                ++p;
            } while (! p.isIdentity());
        }

        static void cachedOrder() {
            SCOPED_TRACE_NUMERIC(n);

            Perm<n> p;
            do {
                EXPECT_EQ(p.cachedOrder(), p.order());
                ++p;
            } while (! p.isIdentity());
        }

        static void pow() {
            SCOPED_TRACE_NUMERIC(n);
            for (Index i = 0; i < Perm<n>::nPerms; ++i) {
                auto p = Perm<n>::Sn[i];

                EXPECT_TRUE(p.pow(0).isIdentity());
                {
                    Perm<n> q;
                    int j = 0;
                    do {
                        auto pow = p.pow(++j);
                        q = q * p;
                        EXPECT_TRUE(looksEqual(pow, q));
                    } while (j < 2 * p.order());
                }
                {
                    Perm<n> q;
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
            SCOPED_TRACE_NUMERIC(n);

            for (Index i = 0; i < Perm<n>::nPerms; ++i) {
                auto p = Perm<n>::Sn[i];

                EXPECT_TRUE(p.cachedPow(0).isIdentity());
                {
                    Perm<n> q;
                    int j = 0;
                    do {
                        auto pow = p.cachedPow(++j);
                        q = q * p;
                        EXPECT_TRUE(looksEqual(pow, q));
                    } while (j < 2 * p.order());
                }
                {
                    Perm<n> q;
                    int j = 0;
                    do {
                        auto pow = p.cachedPow(--j);
                        q = q * p.inverse();
                        EXPECT_TRUE(looksEqual(pow, q));
                    } while (j > -2 * static_cast<int>(p.order()));
                }
            }
        }

        static void tightEncoding() {
            SCOPED_TRACE_NUMERIC(n);
            for (Index i = 0; i < Perm<n>::nPerms; ++i)
                TightEncodingTest<Perm<n>>::verifyTightEncoding(Perm<n>::Sn[i]);
        }
};

/**
 * Implements additional tests for the "large" permutation classes Perm<n>
 * whose codes are image packs.
 *
 * Any test fixture that uses these tests _must_ call initialise() in its setup
 * code.  It is expensive but harmless to call initialise() more than once.
 */
template <int n>
class PermTestLargeImpl : public PermTestImpl<n> {
    static_assert(Perm<n>::codeType == regina::PermCodeType::Images);
    static_assert(increment[n] > 1);
    static_assert((Perm<n>::nPerms % increment[n]) > 1);

    protected:
        using typename PermTestImpl<n>::Index;
        using PermTestImpl<n>::looksLikeIdentity;
        using PermTestImpl<n>::looksEqual;
        using PermTestImpl<n>::looksDistinct;

    private:
        static constexpr Index nIdx = (Perm<n>::nPerms / increment[n]) + 2;
        static Index idx[nIdx];

    public:
        static void initialise() {
            Index pos = 0;
            for (Index i = 0; i < Perm<n>::nPerms; i += increment[n])
                idx[pos++] = i;
            idx[pos++] = Perm<n>::nPerms - 1;
        }

        static void index() {
            SCOPED_TRACE_NUMERIC(n);
            for (int i = 0; i < nIdx; ++i) {
                auto osn = Perm<n>::orderedSn[idx[i]];
                auto sn = Perm<n>::Sn[idx[i]];

                EXPECT_EQ(osn.orderedSnIndex(), idx[i]);
                EXPECT_EQ(sn.SnIndex(), idx[i]);
                EXPECT_EQ(sn.sign(), (idx[i] % 2 == 0 ? 1 : -1));
                if (sn != osn) {
                    EXPECT_EQ(sn.orderedSnIndex(), (idx[i] ^ 1));
                    EXPECT_EQ(osn.SnIndex(), (idx[i] ^ 1));
                }
            }
        }

        static void verifyPerm(const Perm<n> p, bool isIdentity,
                bool isReverse) {
            std::ostringstream nameStream;
            for (int i = 0; i < n; ++i)
                nameStream <<
                    (p[i] < 10 ? char('0' + p[i]) : char('a' + p[i] - 10));
            std::string name = nameStream.str();

            SCOPED_TRACE_STDSTRING(name);

            // Stringification:

            EXPECT_EQ(p.str(), name);

            // Constructors:

            EXPECT_TRUE(looksEqual(Perm<n>(p), p, name));
            {
                std::array<int, n> arr;
                for (int i = 0; i < n; ++i)
                    arr[i] = p[i];
                EXPECT_TRUE(looksEqual(Perm<n>(arr), p, name));
            }

            // Permutation codes:

            EXPECT_TRUE(looksEqual(Perm<n>::fromPermCode(p.permCode()),
                p, name));
            EXPECT_TRUE(Perm<n>::isPermCode(p.permCode()));

            // Setting permutations:

            {
                Perm<n> q = p * Perm<n>(0, n - 1);
                EXPECT_TRUE(looksDistinct(q, p));
                q = p;
                EXPECT_TRUE(looksEqual(q, p, name));
            }
            {
                Perm<n> q = Perm<n>(0, n - 1) * p;
                EXPECT_TRUE(looksDistinct(q, p));
                q.setPermCode(p.permCode());
                EXPECT_TRUE(looksEqual(q, p, name));
            }

            // Products and inverses:

            EXPECT_TRUE(looksEqual(p * Perm<n>(), p));
            EXPECT_TRUE(looksEqual(Perm<n>() * p, p));

            for (int i = 0; i < n - 1; ++i) {
                std::array<int, n> product;
                for (int j = 0; j < n; ++j)
                    product[j] = p[j];
                std::swap(product[i], product[i+1]);

                EXPECT_TRUE(looksEqual(p * Perm<n>(i, i+1), Perm<n>(product)));
            }

            EXPECT_TRUE(looksLikeIdentity(p * p.inverse()));
            EXPECT_TRUE(looksLikeIdentity(p.inverse() * p));
            {
                auto inv = p.inverse();
                for (int i = 0; i < n; ++i)
                    EXPECT_EQ(inv[p[i]], i);
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

            for (int i = 0; i < n; ++i)
                EXPECT_EQ(p.pre(p[i]), i);

            // Ordering:

            if (! isIdentity) {
                EXPECT_EQ(p.compareWith(Perm<n>()), 1);
                EXPECT_EQ(Perm<n>().compareWith(p), -1);
                EXPECT_FALSE(p.isIdentity());
            }
            if (! isReverse) {
                std::array<int, n> image;
                for (int i = 0; i < n; ++i)
                    image[n - i - 1] = i;
                Perm<n> last(image);

                EXPECT_EQ(p.compareWith(last), -1);
                EXPECT_EQ(last.compareWith(p), 1);
            }
            EXPECT_EQ(p.compareWith(p), 0);
        }

        static void comprehensive() {
            SCOPED_TRACE_NUMERIC(n);

            // Test the identity permutation.
            EXPECT_TRUE(looksLikeIdentity(Perm<n>()));
            for (int i = 0; i < n; ++i)
                EXPECT_TRUE(looksLikeIdentity(Perm<n>(i, i)));

            // Test a comprehensive sample of permutations.
            for (int i = 0; i < nIdx; ++i)
                verifyPerm(Perm<n>::orderedSn[idx[i]], i == 0, i == nIdx - 1);

            // Other miscellaneous tests that are not captured elsewhere:
            EXPECT_FALSE(Perm<n>::isPermCode(0));
        }

        static void products() {
            SCOPED_TRACE_NUMERIC(n);
            for (int i = 0; i < nIdx; ++i) {
                auto p = Perm<n>::orderedSn[idx[i]];
                for (int j = 0; j < nIdx; ++j) {
                    auto q = Perm<n>::orderedSn[idx[j]];
                    auto r = p * q;
                    for (int x = 0; x < n; ++x)
                        EXPECT_EQ(r[x], p[q[x]]);
                }
            }
        }

        static void conjugates() {
            SCOPED_TRACE_NUMERIC(n);
            for (int i = 0; i < nIdx; ++i) {
                auto p = Perm<n>::orderedSn[idx[i]];
                for (int j = 0; j < nIdx; ++j) {
                    auto q = Perm<n>::orderedSn[idx[j]];
                    EXPECT_EQ(p.conjugate(q), q * p * q.inverse());
                }
            }
        }

        static void cachedConjugates() {
            SCOPED_TRACE_NUMERIC(n);
            for (int i = 0; i < nIdx; ++i) {
                auto p = Perm<n>::orderedSn[idx[i]];
                for (int j = 0; j < nIdx; ++j) {
                    auto q = Perm<n>::orderedSn[idx[j]];
                    EXPECT_EQ(p.cachedConjugate(q),
                        q.cachedComp(p).cachedComp(q.cachedInverse()));
                }
            }
        }

        static void compareWith() {
            SCOPED_TRACE_NUMERIC(n);

            for (int i = 0; i < nIdx; ++i) {
                auto p = Perm<n>::orderedSn[idx[i]];
                EXPECT_EQ(p.compareWith(p), 0);
                EXPECT_TRUE(looksEqual(p, p));
            }

            for (int i = 0; i < nIdx; ++i) {
                auto p = Perm<n>::orderedSn[idx[i]];
                for (int j = i + 1; j < nIdx; ++j) {
                    auto q = Perm<n>::orderedSn[idx[j]];
                    EXPECT_EQ(p.compareWith(q), -1);
                    EXPECT_EQ(q.compareWith(p), 1);
                    EXPECT_TRUE(looksDistinct(p, q));
                }
            }
        }

        static void lessThan() {
            SCOPED_TRACE_NUMERIC(n);

            for (int i = 0; i < nIdx; ++i) {
                auto p = Perm<n>::Sn[idx[i]];
                EXPECT_FALSE(p < p);
                if (! p.isIdentity()) {
                    auto prev = Perm<n>::Sn[idx[i] - 1];
                    EXPECT_TRUE(prev < p);
                    EXPECT_FALSE(p < prev);
                }
                if (idx[i] != Perm<n>::nPerms - 1) {
                    auto next = Perm<n>::Sn[idx[i] + 1];
                    EXPECT_TRUE(p < next);
                    EXPECT_FALSE(next < p);
                }
            }

            for (int i = 0; i < nIdx; ++i) {
                auto p = Perm<n>::Sn[idx[i]];
                for (int j = i + 1; j < nIdx; ++j) {
                    auto q = Perm<n>::Sn[idx[j]];
                    EXPECT_TRUE(p < q);
                    EXPECT_FALSE(q < p);
                }
            }
        }

        static void reverse() {
            SCOPED_TRACE_NUMERIC(n);
            for (int i = 0; i < nIdx; ++i) {
                auto p = Perm<n>::orderedSn[idx[i]];
                auto r = p.reverse();

                EXPECT_TRUE(looksEqual(p, r.reverse()));
                EXPECT_TRUE(looksDistinct(p, r));

                std::string s = p.str();
                std::reverse(s.begin(), s.end());
                EXPECT_EQ(s, r.str());
            }
        }

        static void clear() {
            SCOPED_TRACE_NUMERIC(n);

            // We use increment[...] manually instead of the pre-filled array
            // idx[...], since we will be doing this with different values of n.
            static constexpr Perm<n> rev = Perm<n>().reverse();

            for (Index i = 0; i < Perm<n>::nPerms; i += increment[n]) {
                auto p = Perm<n>::orderedSn[i];
                p.clear(n);
                EXPECT_TRUE(looksEqual(p, Perm<n>::orderedSn[i]));
            }
            for (typename Perm<n-1>::Index i = 0; i < Perm<n-1>::nPerms;
                    i += increment[n-1]) {
                const auto left = Perm<n>::extend(Perm<n-1>::orderedSn[i]);
                Perm<n> p = left;
                p.clear(n-1);
                EXPECT_TRUE(looksEqual(p, left));
            }
            regina::for_constexpr<2, n-1>([](auto from) {
                SCOPED_TRACE_NUMERIC(from);
                for (typename Perm<from>::Index i = 0; i < Perm<from>::nPerms;
                        i += increment[from])
                    for (typename Perm<n-from>::Index j = 0;
                            j < Perm<n-from>::nPerms; j += increment[n-from]) {
                        auto left = Perm<n>::extend(Perm<from>::orderedSn[i]);
                        auto right = rev *
                            Perm<n>::extend(Perm<n-from>::orderedSn[j]) * rev;
                        auto p = left * right;
                        p.clear(from);
                        EXPECT_TRUE(looksEqual(p, left));
                    }
            });
            for (typename Perm<n-1>::Index i = 0; i < Perm<n-1>::nPerms;
                    i += increment[n-1]) {
                auto p = rev * Perm<n>::extend(Perm<n-1>::orderedSn[i]) * rev;
                p.clear(1);
                EXPECT_TRUE(looksLikeIdentity(p));
            }
            for (Index i = 0; i < Perm<n>::nPerms; i += increment[n]) {
                Perm p = Perm<n>::orderedSn[i];
                p.clear(0);
                EXPECT_TRUE(looksLikeIdentity(p));
            }
        }

        static void order() {
            SCOPED_TRACE_NUMERIC(n);
            for (int i = 0; i < nIdx; ++i) {
                auto p = Perm<n>::orderedSn[idx[i]];

                int j = 0;
                Perm<n> q;
                do {
                    q = q * p;
                    ++j;
                } while (! q.isIdentity());
                EXPECT_EQ(j, p.order());
            }
        }

        static void pow() {
            SCOPED_TRACE_NUMERIC(n);
            for (int i = 0; i < nIdx; ++i) {
                auto p = Perm<n>::orderedSn[idx[i]];

                EXPECT_TRUE(p.pow(0).isIdentity());
                {
                    Perm<n> q;
                    int j = 0;
                    do {
                        auto pow = p.pow(++j);
                        q = q * p;
                        EXPECT_TRUE(looksEqual(pow, q));
                    } while (! q.isIdentity());
                }
                {
                    Perm<n> q;
                    int j = 0;
                    do {
                        auto pow = p.pow(--j);
                        q = q * p.inverse();
                        EXPECT_TRUE(looksEqual(pow, q));
                    } while (! q.isIdentity());
                }
            }
        }

        static void tightEncoding() {
            SCOPED_TRACE_NUMERIC(n);
            for (int i = 0; i < nIdx; ++i)
                TightEncodingTest<Perm<n>>::verifyTightEncoding(
                    Perm<n>::orderedSn[idx[i]]);
        }
};

template <int n>
typename PermTestLargeImpl<n>::Index
    PermTestLargeImpl<n>::idx[PermTestLargeImpl<n>::nIdx];

class PermTestSmall : public testing::Test {
    protected:
        static void SetUpTestSuite() {
            regina::for_constexpr<2, 8>([](auto n) {
                Perm<n>::precompute();
            });
        }
};

TEST_F(PermTestSmall, permCode) {
    regina::for_constexpr<2, 8>([](auto n) {
        PermTestSmallImpl<n>::permCode();
    });
}
TEST_F(PermTestSmall, sign) {
    regina::for_constexpr<2, 8>([](auto n) {
        PermTestSmallImpl<n>::sign();
    });
}
TEST_F(PermTestSmall, index) {
    regina::for_constexpr<2, 8>([](auto n) {
        PermTestSmallImpl<n>::index();
    });
}
TEST_F(PermTestSmall, exhaustive) {
    regina::for_constexpr<2, 8>([](auto n) {
        PermTestSmallImpl<n>::exhaustive();
    });
}
TEST_F(PermTestSmall, swaps) {
    regina::for_constexpr<2, 8>([](auto n) {
        PermTestImpl<n>::swaps();
    });
}
TEST_F(PermTestSmall, increment) {
    regina::for_constexpr<2, 8>([](auto n) {
        PermTestSmallImpl<n>::increment();
    });
}
TEST_F(PermTestSmall, iterationSign) {
    regina::for_constexpr<2, 8>([](auto n) {
        PermTestSmallImpl<n>::iterationSign();
    });
}
TEST_F(PermTestSmall, iterationLex) {
    regina::for_constexpr<2, 8>([](auto n) {
        PermTestSmallImpl<n>::iterationLex();
    });
}
TEST_F(PermTestSmall, products) {
    regina::for_constexpr<2, 8>([](auto n) {
        PermTestSmallImpl<n>::products();
    });
}
TEST_F(PermTestSmall, cachedProducts) {
    regina::for_constexpr<2, 8>([](auto n) {
        PermTestSmallImpl<n>::cachedProducts();
    });
}
TEST_F(PermTestSmall, conjugates) {
    regina::for_constexpr<2, 8>([](auto n) {
        PermTestSmallImpl<n>::conjugates();
    });
}
TEST_F(PermTestSmall, cachedConjugates) {
    regina::for_constexpr<2, 8>([](auto n) {
        PermTestSmallImpl<n>::cachedConjugates();
    });
}
TEST_F(PermTestSmall, cachedInverse) {
    regina::for_constexpr<2, 8>([](auto n) {
        PermTestSmallImpl<n>::cachedInverse();
    });
}
TEST_F(PermTestSmall, compareWith) {
    regina::for_constexpr<2, 8>([](auto n) {
        PermTestSmallImpl<n>::compareWith();
    });
}
TEST_F(PermTestSmall, reverse) {
    regina::for_constexpr<2, 8>([](auto n) {
        PermTestSmallImpl<n>::reverse();
    });
}
TEST_F(PermTestSmall, clear) {
    regina::for_constexpr<2, 8>([](auto n) {
        PermTestSmallImpl<n>::clear();
    });
}
TEST_F(PermTestSmall, order) {
    regina::for_constexpr<2, 8>([](auto n) {
        PermTestSmallImpl<n>::order();
    });
}
TEST_F(PermTestSmall, cachedOrder) {
    regina::for_constexpr<2, 8>([](auto n) {
        PermTestSmallImpl<n>::cachedOrder();
    });
}
TEST_F(PermTestSmall, pow) {
    regina::for_constexpr<2, 8>([](auto n) {
        PermTestSmallImpl<n>::pow();
    });
}
TEST_F(PermTestSmall, cachedPow) {
    regina::for_constexpr<2, 8>([](auto n) {
        PermTestSmallImpl<n>::cachedPow();
    });
}
TEST_F(PermTestSmall, rot) {
    regina::for_constexpr<2, 8>([](auto n) {
        PermTestImpl<n>::rot();
    });
}
TEST_F(PermTestSmall, conjugacyMinimal) {
    regina::for_constexpr<2, 8>([](auto n) {
        PermTestSmallImpl<n>::conjugacyMinimal();
    });
}
TEST_F(PermTestSmall, tightEncoding) {
    regina::for_constexpr<2, 8>([](auto n) {
        PermTestSmallImpl<n>::tightEncoding();
    });
}
TEST_F(PermTestSmall, aliases) {
    for (int i = 0; i < 2; ++i)
        EXPECT_EQ(Perm<2>::S2[i], Perm<2>::Sn[i]);
    for (int i = 0; i < 6; ++i)
        EXPECT_EQ(Perm<3>::S3[i], Perm<3>::Sn[i]);
    for (int i = 0; i < 24; ++i)
        EXPECT_EQ(Perm<4>::S4[i], Perm<4>::Sn[i]);
    for (int i = 0; i < 120; ++i)
        EXPECT_EQ(Perm<5>::S5[i], Perm<5>::Sn[i]);
    for (Perm<6>::Index i = 0; i < Perm<6>::nPerms; ++i)
        EXPECT_EQ(Perm<6>::S6[i], Perm<6>::Sn[i]);
    for (Perm<7>::Index i = 0; i < Perm<7>::nPerms; ++i)
        EXPECT_EQ(Perm<7>::S7[i], Perm<7>::Sn[i]);
}
TEST_F(PermTestSmall, S2) {
    regina::for_constexpr<3, 6>([](auto n) {
        for (int i = 0; i < 2; ++i) {
            EXPECT_EQ(Perm<n>::S2[i], Perm<n>::extend(Perm<2>::S2[i]));
            EXPECT_EQ(Perm<2>::S2[i], Perm<2>::contract(Perm<n>::S2[i]));
        }
    });
}
TEST_F(PermTestSmall, S3) {
    regina::for_constexpr<4, 6>([](auto n) {
        for (int i = 0; i < 6; ++i) {
            EXPECT_EQ(Perm<n>::S3[i], Perm<n>::extend(Perm<3>::S3[i]));
            EXPECT_EQ(Perm<n>::orderedS3[i],
                Perm<n>::extend(Perm<3>::orderedS3[i]));
            EXPECT_EQ(Perm<3>::S3[i], Perm<3>::contract(Perm<n>::S3[i]));
            EXPECT_EQ(Perm<3>::orderedS3[i],
                Perm<3>::contract(Perm<n>::orderedS3[i]));
        }
    });
}
TEST_F(PermTestSmall, S4) {
    regina::for_constexpr<5, 6>([](auto n) {
        for (int i = 0; i < 24; ++i) {
            EXPECT_EQ(Perm<n>::S4[i], Perm<n>::extend(Perm<4>::S4[i]));
            EXPECT_EQ(Perm<n>::orderedS4[i],
                    Perm<n>::extend(Perm<4>::orderedS4[i]));
            EXPECT_EQ(Perm<4>::S4[i], Perm<4>::contract(Perm<n>::S4[i]));
            EXPECT_EQ(Perm<4>::orderedS4[i],
                Perm<4>::contract(Perm<n>::orderedS4[i]));
        }
    });
}
TEST_F(PermTestSmall, edgePairs) {
    // This test is specific to Perm<4>.
    for (int idx = 0; idx < 24; ++idx) {
        Perm<4> p4 = Perm<4>::S4[idx];
        Perm<6> p6 = p4.pairs();

        for (int i = 0; i < 4; ++i)
            for (int j = i + 1; j < 4; ++j) {
                // Look at how p4 maps the pair {i,j}.
                int e = regina::FaceNumbering<3,1>::edgeNumber[i][j];
                int f = regina::FaceNumbering<3,1>::edgeNumber[p4[i]][p4[j]];
                EXPECT_EQ(p6[e], f);
            }
    }
}

class PermTestLarge : public testing::Test {
    protected:
        // A sample of sizes to use for testing.
        // These _must_ be sizes that use image packs as codes.
        //
        // Note: n=8 uses 3-bit images with a 32-bit code, and n=9..16 use
        // 4-bit images with a 64-bit code.
        using allSizes = std::integer_sequence<int, 8, 9, 10, 11, 13, 14, 16>;

        // Exhaustive iteration takes too much time for larger n.
        using iterableSizes = std::integer_sequence<int, 8, 9, 10>;

        // Precomputation consumes too much space for larger n.
        using precomputableSizes = std::integer_sequence<int, 8, 9, 10, 11>;

        static void SetUpTestSuite() {
            regina::foreach_constexpr(allSizes(), [](auto n) {
                PermTestLargeImpl<n>::initialise();
            });

            regina::foreach_constexpr(precomputableSizes(), [](auto n) {
                Perm<n>::precompute();
            });
        }
};

TEST_F(PermTestLarge, index) {
    regina::foreach_constexpr(allSizes(), [](auto n) {
        PermTestLargeImpl<n>::index();
    });
}
TEST_F(PermTestLarge, comprehensive) {
    regina::foreach_constexpr(allSizes(), [](auto n) {
        PermTestLargeImpl<n>::comprehensive();
    });
}
TEST_F(PermTestLarge, swaps) {
    regina::foreach_constexpr(allSizes(), [](auto n) {
        PermTestImpl<n>::swaps();
    });
}
TEST_F(PermTestLarge, increment) {
    regina::foreach_constexpr(iterableSizes(), [](auto n) {
        PermTestImpl<n>::increment();
    });
}
TEST_F(PermTestLarge, iterationSign) {
    regina::foreach_constexpr(iterableSizes(), [](auto n) {
        PermTestImpl<n>::iterationSign();
    });
}
TEST_F(PermTestLarge, iterationLex) {
    regina::foreach_constexpr(iterableSizes(), [](auto n) {
        PermTestImpl<n>::iterationLex();
    });
}
TEST_F(PermTestLarge, products) {
    regina::foreach_constexpr(allSizes(), [](auto n) {
        PermTestLargeImpl<n>::products();
    });
}
TEST_F(PermTestLarge, conjugates) {
    regina::foreach_constexpr(allSizes(), [](auto n) {
        PermTestLargeImpl<n>::conjugates();
    });
}
TEST_F(PermTestLarge, cachedConjugates) {
    regina::foreach_constexpr(precomputableSizes(), [](auto n) {
        PermTestLargeImpl<n>::cachedConjugates();
    });
}
TEST_F(PermTestLarge, cachedInverse) {
    regina::foreach_constexpr(iterableSizes(), [](auto n) {
        PermTestImpl<n>::cachedInverse();
    });
}
TEST_F(PermTestLarge, compareWith) {
    regina::foreach_constexpr(allSizes(), [](auto n) {
        PermTestLargeImpl<n>::compareWith();
    });
}
TEST_F(PermTestLarge, lessThan) {
    regina::foreach_constexpr(allSizes(), [](auto n) {
        PermTestLargeImpl<n>::lessThan();
    });
}
TEST_F(PermTestLarge, reverse) {
    regina::foreach_constexpr(allSizes(), [](auto n) {
        PermTestLargeImpl<n>::reverse();
    });
}
TEST_F(PermTestLarge, clear) {
    regina::foreach_constexpr(allSizes(), [](auto n) {
        PermTestLargeImpl<n>::clear();
    });
}
TEST_F(PermTestLarge, order) {
    regina::foreach_constexpr(allSizes(), [](auto n) {
        PermTestLargeImpl<n>::order();
    });
}
TEST_F(PermTestLarge, pow) {
    regina::foreach_constexpr(allSizes(), [](auto n) {
        PermTestLargeImpl<n>::pow();
    });
}
TEST_F(PermTestLarge, rot) {
    regina::foreach_constexpr(allSizes(), [](auto n) {
        PermTestImpl<n>::rot();
    });
}
TEST_F(PermTestLarge, conjugacyMinimal) {
    regina::foreach_constexpr(iterableSizes(), [](auto n) {
        PermTestImpl<n>::conjugacyMinimal();
    });
}
TEST_F(PermTestLarge, tightEncoding) {
    regina::foreach_constexpr(allSizes(), [](auto n) {
        PermTestLargeImpl<n>::tightEncoding();
    });
}
