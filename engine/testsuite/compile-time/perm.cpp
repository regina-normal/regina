
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include "maths/perm.h"

using regina::Perm;

static_assert(regina::digit(5) == '5');
static_assert(regina::digit(15) == 'f');
static_assert(regina::factorial(5) == 120);

template <int n,
          int pos /* in (0,n) */,
          int index /* in (0,n!), Sn[index] != orderedSn[index] */>
void testPerm() {
    constexpr Perm<n> swap(pos-1, pos);

    static_assert(0 < pos && pos < n);
    static_assert(0 < index && index < regina::factorial(n));

    static_assert(Perm<n>().isIdentity());
    static_assert(Perm<n>(pos-1, pos) == swap);
    static_assert(Perm<n>(pos, pos-1) == swap);
    static_assert(Perm<n>(pos, pos) == Perm<n>());
    static_assert(Perm<n>(pos, pos).isIdentity());
    static_assert(Perm<n>(swap) == swap);

    static_assert(Perm<n>().compareWith(swap) == -1);
    static_assert(swap.compareWith(Perm<n>()) == 1);
    static_assert(swap.compareWith(Perm<n>(swap)) == 0);

    static_assert(swap[pos] == pos-1);
    static_assert(swap.pre(pos) == pos-1);
    static_assert(Perm<n>().sign() == 1);
    static_assert(swap.sign() == -1);

    static_assert(Perm<n>::rot(0) == Perm<n>());
    static_assert(Perm<n>::rot(pos) != Perm<n>());

    static_assert(Perm<n>().reverse().inverse() == Perm<n>().reverse());
    static_assert(Perm<n>().reverse() * Perm<n>().reverse() == Perm<n>());
    static_assert(Perm<n>::rot(1) * Perm<n>::rot(1) == Perm<n>::rot(2 % n));

    if constexpr (n >= 4 && n <= 7) {
        static_assert(Perm<n>::fromPermCode1(swap.permCode1()) == swap);
        static_assert(Perm<n>::isPermCode1(swap.permCode1()));

        static_assert(Perm<n>::fromPermCode2(swap.permCode2()) == swap);
        static_assert(Perm<n>::isPermCode2(swap.permCode2()));
    } else {
        static_assert(Perm<n>::fromPermCode(swap.permCode()) == swap);
        static_assert(Perm<n>::isPermCode(swap.permCode()));
    }

    static_assert(! Perm<n>().reverse().isIdentity());
    static_assert(Perm<n>().reverse().reverse().isIdentity());

    static_assert(Perm<n>::Sn[0] == Perm<n>());
    static_assert(Perm<n>::orderedSn[0] == Perm<n>());
    static_assert((*Perm<n>::Sn.begin()).isIdentity());
    static_assert((*Perm<n>::orderedSn.begin()).isIdentity());

    if constexpr (n == 2) {
        // For n = 2, Sn and orderedSn are identical.
        static_assert(Perm<n>::Sn[index] == Perm<n>::orderedSn[index]);
        static_assert(*(Perm<n>::Sn.begin() + index) ==
            *(Perm<n>::orderedSn.begin() + index));
        static_assert(Perm<n>::Sn[index].SnIndex() == index);
        static_assert(Perm<n>::orderedSn[index].orderedSnIndex() == index);
        static_assert((*(Perm<n>::Sn.begin() + index)).SnIndex() == index);
        static_assert((*(Perm<n>::orderedSn.begin() + index)).orderedSnIndex()
            == index);
        static_assert(Perm<n>::Sn[index].orderedSnIndex() == index);
        static_assert(Perm<n>::orderedSn[index].SnIndex() == index);
    } else {
        static_assert(Perm<n>::Sn[index] != Perm<n>::orderedSn[index]);
        static_assert(Perm<n>::Sn[index].SnIndex() == index);
        static_assert(Perm<n>::orderedSn[index].orderedSnIndex() == index);
        static_assert(Perm<n>::Sn[index].orderedSnIndex() == (index ^ 1));
        static_assert(Perm<n>::orderedSn[index].SnIndex() == (index ^ 1));

        if constexpr (Perm<n>::codeType == regina::PermCodeType::Index) {
            static_assert(*(Perm<n>::Sn.begin() + index) !=
                *(Perm<n>::orderedSn.begin() + index));
            static_assert((*(Perm<n>::Sn.begin() + index)).SnIndex() == index);
            static_assert((*(Perm<n>::orderedSn.begin() + index)).
                orderedSnIndex() == index);
        }
    }
}

template <int a, int b>
void testConvert() {
    static_assert(a < b);
    static_assert(Perm<a>::contract(Perm<b>(0, 1)) == Perm<a>(0, 1));
    static_assert(Perm<b>::extend(Perm<a>(0, 1)) == Perm<b>(0, 1));
}

void testPermArrays() {
    constexpr std::array<int, 2> a2 { 1, 0 };
    static_assert(Perm<2>(a2) != Perm<2>());

    constexpr std::array<int, 3> a3 { 1, 2, 0 };
    static_assert(Perm<3>(a3) != Perm<3>());
    static_assert(Perm<3>(a3) == Perm<3>(1, 2, 0));

    constexpr std::array<int, 4> a4 { 1, 3, 2, 0 };
    static_assert(Perm<4>(a4) != Perm<4>());
    static_assert(Perm<4>(a4) == Perm<4>(1, 3, 2, 0));
    static_assert(Perm<4>(a4).inverse() == Perm<4>(1,0, 3,1, 2,2, 0,3));

    constexpr std::array<int, 5> a5 { 1, 3, 2, 4, 0 };
    static_assert(Perm<5>(a5) != Perm<5>());
    static_assert(Perm<5>(a5) == Perm<5>(1, 3, 2, 4, 0));
    static_assert(Perm<5>(a5).inverse() == Perm<5>(1,0, 3,1, 2,2, 4,3, 0,4));

    constexpr std::array<int, 6> a6 { 1, 5, 3, 2, 4, 0 };
    static_assert(Perm<6>(a6) != Perm<6>());

    constexpr std::array<int, 7> a7 { 1, 5, 3, 6, 2, 4, 0 };
    static_assert(Perm<7>(a7) != Perm<7>());
}

template void testPerm<2, 1, 1>();
template void testPerm<3, 2, 3>();
template void testPerm<4, 2, 14>();
template void testPerm<5, 3, 50>();
template void testPerm<6, 4, 302>();
template void testPerm<7, 4, 682>();
template void testPerm<8, 5, 1002>();
template void testPerm<16, 9, 3000>();

template void testConvert<2, 3>();
template void testConvert<2, 4>();
template void testConvert<2, 5>();
template void testConvert<2, 6>();
template void testConvert<2, 16>();
template void testConvert<3, 4>();
template void testConvert<3, 5>();
template void testConvert<3, 6>();
template void testConvert<3, 16>();
template void testConvert<4, 5>();
template void testConvert<4, 6>();
template void testConvert<4, 16>();
template void testConvert<5, 6>();
template void testConvert<5, 16>();
template void testConvert<6, 7>();
template void testConvert<6, 16>();
template void testConvert<7, 16>();
