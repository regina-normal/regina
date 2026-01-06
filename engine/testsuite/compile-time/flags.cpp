
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

#include <concepts>
#include <limits>
#include "utilities/flags.h"

using regina::Flags;

namespace {
    enum class A /* C++ standard says this should default to int */ {
        ax = 0x0001,
        ay = 0x0002,
        az = std::numeric_limits<int>::max()
    };

    enum class B : unsigned {
        bx = 0x0001,
        by = 0x0002,
        bz = std::numeric_limits<unsigned>::max()
    };

    // This test ensures that integer promotion behaves correctly when doing
    // bitwise AND/OR/NOT operations.
    enum class C : char {
        cx = 0x01,
        cy = 0x02,
        cz = 0x7F
    };

    enum D {
        dx = 0x0001,
        dy = 0x0002,
        dz = std::numeric_limits<unsigned>::max()
    };

    enum E {
        ex = 0x0000000000000001,
        ey = 0x0000000000000002,
        ez = 0x8000000000000000
    };
}

// The C++ standard says that scoped enums will be based on int by default,
// and that unscoped enums will use a base type no larger than int unless some
// enumeration value cannot fit into a signed or unsigned int.
static_assert(std::same_as<Flags<A>::BaseInt, int>);
static_assert(std::same_as<Flags<B>::BaseInt, unsigned>);
static_assert(std::same_as<Flags<C>::BaseInt, char>);
static_assert(sizeof(Flags<D>::BaseInt) <= sizeof(int));
static_assert(sizeof(Flags<E>::BaseInt) >= 8);

static_assert((Flags<A>(A::ax) | A::ay).baseValue() == 0x0003);
static_assert((Flags<B>(B::bx) | B::by).baseValue() == 0x0003);
static_assert((Flags<C>(C::cx) | C::cy).baseValue() == 0x03);
static_assert((Flags<D>(dx) | dy).baseValue() == 0x0003);
static_assert((Flags<E>(ex) | ey).baseValue() == 0x0000000000000003);

static_assert((Flags<A>(A::ax) ^ A::az).baseValue() ==
    std::numeric_limits<int>::max() - 1);
static_assert((Flags<B>(B::bx) ^ B::bz).baseValue() ==
    std::numeric_limits<unsigned>::max() - 1);
static_assert((Flags<C>(C::cx) ^ C::cz).baseValue() == 0x7E);
static_assert((Flags<D>(dx) ^ dz).baseValue() ==
    std::numeric_limits<unsigned>::max() - 1);
static_assert((Flags<E>(ex) ^ ez).baseValue() == 0x8000000000000001);

