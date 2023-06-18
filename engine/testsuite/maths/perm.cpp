
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

#include "testsuite/maths/permtest.h"

using regina::Perm;

static constexpr int64_t increment[] = {
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
 * Implements additional tests for the "large" permutation classes Perm<n>
 * whose codes are image packs.
 *
 * Every test that uses the idx[...] array _must_ call initialise() to ensure
 * that the array has been set up.
 */
template <int n>
class PermTestLargeImpl : public GeneralPermTest<n> {
    static_assert(Perm<n>::codeType == regina::PERM_CODE_IMAGES);
    static_assert(increment[n] > 1);
    static_assert((Perm<n>::nPerms % increment[n]) > 1);

    protected:
        using typename GeneralPermTest<n>::Index;
        using GeneralPermTest<n>::looksLikeIdentity;
        using GeneralPermTest<n>::looksEqual;
        using GeneralPermTest<n>::looksDistinct;

    private:
        static constexpr Index nIdx = (Perm<n>::nPerms / increment[n]) + 2;
        static Index idx[nIdx];
        static bool initialised;

        static void initialise() {
            if (! initialised) {
                Index pos = 0;
                for (Index i = 0; i < Perm<n>::nPerms; i += increment[n])
                    idx[pos++] = i;
                idx[pos++] = Perm<n>::nPerms - 1;

                initialised = true;
            }
        }

    public:
        static void index() {
            initialise();
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
                EXPECT_EQ(p.compareWith(regina::Perm<n>()), 1);
                EXPECT_EQ(regina::Perm<n>().compareWith(p), -1);
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
            initialise();

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
            initialise();
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
            initialise();
            for (int i = 0; i < nIdx; ++i) {
                auto p = Perm<n>::orderedSn[idx[i]];
                for (int j = 0; j < nIdx; ++j) {
                    auto q = Perm<n>::orderedSn[idx[j]];
                    EXPECT_EQ(p.conjugate(q), q * p * q.inverse());
                }
            }
        }

        static void cachedConjugates() {
            initialise();
            Perm<n>::precompute();
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
            initialise();

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
            initialise();

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
            initialise();
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
            initialise();
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
            initialise();
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
            initialise();
            for (int i = 0; i < nIdx; ++i)
                TightEncodingTest<Perm<n>>::verifyTightEncoding(
                    Perm<n>::orderedSn[idx[i]]);
        }
};

template <int n>
typename PermTestLargeImpl<n>::Index
    PermTestLargeImpl<n>::idx[PermTestLargeImpl<n>::nIdx];

template <int n>
bool PermTestLargeImpl<n>::initialised = false;

// The actual tests follow.
//
// Although our tests are defined for all n that use image packs as codes,
// we only run them for a selection of some n.
//
// Note: n=8 uses 3-bit images with a 32-bit code, and n=9..16 use 4-bit
// images with a 64-bit code.

TEST(PermTestLarge, index) {
    regina::foreach_constexpr<8, 9, 10, 11, 13, 14, 16>([](auto n) {
        PermTestLargeImpl<n>::index();
    });
}
TEST(PermTestLarge, comprehensive) {
    regina::foreach_constexpr<8, 9, 10, 11, 13, 14, 16>([](auto n) {
        PermTestLargeImpl<n>::comprehensive();
    });
}
TEST(PermTestLarge, increment) {
    // This test iterates through all n! permutations, so we will stop at n=11.
    regina::foreach_constexpr<8, 9, 10, 11>([](auto n) {
        GeneralPermTest<n>::increment();
    });
}
TEST(PermTestLarge, products) {
    regina::foreach_constexpr<8, 9, 10, 11, 13, 14, 16>([](auto n) {
        PermTestLargeImpl<n>::products();
    });
}
TEST(PermTestLarge, conjugates) {
    regina::foreach_constexpr<8, 9, 10, 11, 13, 14, 16>([](auto n) {
        PermTestLargeImpl<n>::conjugates();
    });
}
TEST(PermTestLarge, cachedConjugates) {
    // Precomputation requires a lot of space for larger n, so stop at n=11.
    regina::foreach_constexpr<8, 9, 10, 11>([](auto n) {
        PermTestLargeImpl<n>::cachedConjugates();
    });
}
TEST(PermTestLarge, cachedInverse) {
    // This test iterates through all n! permutations, so we will stop at n=11.
    regina::foreach_constexpr<8, 9, 10, 11>([](auto n) {
        GeneralPermTest<n>::cachedInverse();
    });
}
TEST(PermTestLarge, compareWith) {
    regina::foreach_constexpr<8, 9, 10, 11, 13, 14, 16>([](auto n) {
        PermTestLargeImpl<n>::compareWith();
    });
}
TEST(PermTestLarge, lessThan) {
    regina::foreach_constexpr<8, 9, 10, 11, 13, 14, 16>([](auto n) {
        PermTestLargeImpl<n>::lessThan();
    });
}
TEST(PermTestLarge, reverse) {
    regina::foreach_constexpr<8, 9, 10, 11, 13, 14, 16>([](auto n) {
        PermTestLargeImpl<n>::reverse();
    });
}
TEST(PermTestLarge, clear) {
    regina::foreach_constexpr<8, 9, 10, 11, 13, 14, 16>([](auto n) {
        PermTestLargeImpl<n>::clear();
    });
}
TEST(PermTestLarge, order) {
    regina::foreach_constexpr<8, 9, 10, 11, 13, 14, 16>([](auto n) {
        PermTestLargeImpl<n>::order();
    });
}
TEST(PermTestLarge, pow) {
    regina::foreach_constexpr<8, 9, 10, 11, 13, 14, 16>([](auto n) {
        PermTestLargeImpl<n>::pow();
    });
}
TEST(PermTestLarge, rot) {
    regina::foreach_constexpr<8, 9, 10, 11, 13, 14, 16>([](auto n) {
        GeneralPermTest<n>::rot();
    });
}
TEST(PermTestLarge, conjugacyMinimal) {
    // This test iterates through all n! permutations, so we will stop at n=11.
    regina::foreach_constexpr<8, 9, 10, 11>([](auto n) {
        GeneralPermTest<n>::conjugacyMinimal();
    });
}
TEST(PermTestLarge, tightEncoding) {
    regina::foreach_constexpr<8, 9, 10, 11, 13, 14, 16>([](auto n) {
        PermTestLargeImpl<n>::tightEncoding();
    });
}
