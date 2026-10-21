
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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

#include "maths/integer.h"

// Just a few tests for now that hit the non-trivial parts of NativeInteger.

using N4 = regina::NativeInteger<4>;

// We require the GCD to be non-negative.
static_assert(N4(12).gcd(18) == 6);
static_assert(N4(12).gcd(-18) == 6);
static_assert(N4(-12).gcd(18) == 6);
static_assert(N4(-12).gcd(-18) == 6);
static_assert(N4(0).gcd(6) == 6);
static_assert(N4(0).gcd(-6) == 6);
static_assert(N4(6).gcd(0) == 6);
static_assert(N4(-6).gcd(0) == 6);
static_assert(N4(0).gcd(0) == 0);

// Note: we make no promises about the sign of the LCM.
static_assert(N4(12).lcm(18) == 36);
static_assert(N4(12).lcm(-18) == -36);
static_assert(N4(-12).lcm(18) == -36);
static_assert(N4(-12).lcm(-18) == 36);
static_assert(N4(0).lcm(6) == 0);
static_assert(N4(0).lcm(-6) == 0);
static_assert(N4(6).lcm(0) == 0);
static_assert(N4(-6).lcm(0) == 0);
static_assert(N4(0).lcm(0) == 0);

