
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

#include "utilities/intutils.h"
#include "maths/integer.h"

static_assert(regina::bitsRequired(32) == 5);
static_assert(regina::bitsRequired(33) == 6);

static_assert(regina::nextPowerOfTwo(32) == 32);
static_assert(regina::nextPowerOfTwo(33) == 64);

static_assert(regina::FaithfulAssignment<
    regina::NativeInteger<4>, regina::NativeInteger<4>>::value);
static_assert(! regina::FaithfulAssignment<
    regina::NativeInteger<8>, regina::NativeInteger<4>>::value);
static_assert(regina::FaithfulAssignment<
    regina::NativeInteger<4>, regina::NativeInteger<8>>::value);
static_assert(regina::FaithfulAssignment<
    regina::NativeInteger<4>, regina::Integer>::value);
static_assert(regina::FaithfulAssignment<
    regina::NativeInteger<4>, regina::LargeInteger>::value);
static_assert(! regina::FaithfulAssignment<
    regina::Integer, regina::NativeInteger<4>>::value);
static_assert(regina::FaithfulAssignment<
    regina::Integer, regina::Integer>::value);
static_assert(regina::FaithfulAssignment<
    regina::Integer, regina::LargeInteger>::value);
static_assert(! regina::FaithfulAssignment<
    regina::LargeInteger, regina::NativeInteger<4>>::value);
static_assert(! regina::FaithfulAssignment<
    regina::LargeInteger, regina::Integer>::value);
static_assert(regina::FaithfulAssignment<
    regina::LargeInteger, regina::LargeInteger>::value);

static_assert(! regina::is_cpp_integer_v<bool>);
static_assert(! regina::is_unsigned_cpp_integer_v<bool>);
static_assert(! regina::is_signed_cpp_integer_v<bool>);

static_assert(regina::is_cpp_integer_v<unsigned char>);
static_assert(regina::is_unsigned_cpp_integer_v<unsigned char>);
static_assert(! regina::is_signed_cpp_integer_v<unsigned char>);

static_assert(regina::is_cpp_integer_v<signed char>);
static_assert(! regina::is_unsigned_cpp_integer_v<signed char>);
static_assert(regina::is_signed_cpp_integer_v<signed char>);

static_assert(regina::is_cpp_integer_v<unsigned long long>);
static_assert(regina::is_unsigned_cpp_integer_v<unsigned long long>);
static_assert(! regina::is_signed_cpp_integer_v<unsigned long long>);

static_assert(regina::is_cpp_integer_v<long long>);
static_assert(! regina::is_unsigned_cpp_integer_v<long long>);
static_assert(regina::is_signed_cpp_integer_v<long long>);

#if defined(INT128_AVAILABLE)
static_assert(regina::is_cpp_integer_v<regina::IntOfSize<16>::utype>);
static_assert(regina::is_unsigned_cpp_integer_v<regina::IntOfSize<16>::utype>);
static_assert(! regina::is_signed_cpp_integer_v<regina::IntOfSize<16>::utype>);

static_assert(regina::is_cpp_integer_v<regina::IntOfSize<16>::type>);
static_assert(! regina::is_unsigned_cpp_integer_v<regina::IntOfSize<16>::type>);
static_assert(regina::is_signed_cpp_integer_v<regina::IntOfSize<16>::type>);
#endif
