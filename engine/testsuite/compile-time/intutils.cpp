
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

static_assert(std::same_as<std::common_type_t<
    regina::NativeInteger<4>, regina::NativeInteger<4>>,
    regina::NativeInteger<4>>);
#if 0
// Native integers are not constructible/assignable from native integers of
// different sizes.  Therefore NativeInteger<4> and NativeInteger<8> have no
// common type.
static_assert(std::same_as<std::common_type_t<
    regina::NativeInteger<8>, regina::NativeInteger<4>>,
    regina::NativeInteger<8>>);
static_assert(std::same_as<std::common_type_t<
    regina::NativeInteger<4>, regina::NativeInteger<8>>,
    regina::NativeInteger<8>>);
#endif
static_assert(std::same_as<std::common_type_t<
    regina::NativeInteger<4>, regina::Integer>, regina::Integer>);
static_assert(std::same_as<std::common_type_t<
    regina::NativeInteger<4>, regina::LargeInteger>, regina::LargeInteger>);
static_assert(std::same_as<std::common_type_t<
    regina::Integer, regina::NativeInteger<4>>, regina::Integer>);
static_assert(std::same_as<std::common_type_t<
    regina::Integer, regina::Integer>, regina::Integer>);
static_assert(std::same_as<std::common_type_t<
    regina::Integer, regina::LargeInteger>, regina::LargeInteger>);
static_assert(std::same_as<std::common_type_t<
    regina::LargeInteger, regina::NativeInteger<4>>, regina::LargeInteger>);
static_assert(std::same_as<std::common_type_t<
    regina::LargeInteger, regina::Integer>, regina::LargeInteger>);
static_assert(std::same_as<std::common_type_t<
    regina::LargeInteger, regina::LargeInteger>, regina::LargeInteger>);

static_assert(! regina::CppInteger<bool>);
static_assert(! regina::UnsignedCppInteger<bool>);
static_assert(! regina::SignedCppInteger<bool>);

static_assert(regina::CppInteger<unsigned char>);
static_assert(regina::UnsignedCppInteger<unsigned char>);
static_assert(! regina::SignedCppInteger<unsigned char>);

static_assert(regina::CppInteger<signed char>);
static_assert(! regina::UnsignedCppInteger<signed char>);
static_assert(regina::SignedCppInteger<signed char>);

static_assert(regina::CppInteger<unsigned>);
static_assert(regina::UnsignedCppInteger<unsigned>);
static_assert(! regina::SignedCppInteger<unsigned>);

static_assert(regina::CppInteger<int>);
static_assert(! regina::UnsignedCppInteger<int>);
static_assert(regina::SignedCppInteger<int>);

static_assert(regina::CppInteger<unsigned long long>);
static_assert(regina::UnsignedCppInteger<unsigned long long>);
static_assert(! regina::SignedCppInteger<unsigned long long>);

static_assert(regina::CppInteger<long long>);
static_assert(! regina::UnsignedCppInteger<long long>);
static_assert(regina::SignedCppInteger<long long>);

static_assert(regina::CppInteger<size_t>);
static_assert(regina::UnsignedCppInteger<size_t>);
static_assert(! regina::SignedCppInteger<size_t>);

static_assert(regina::CppInteger<ssize_t>);
static_assert(! regina::UnsignedCppInteger<ssize_t>);
static_assert(regina::SignedCppInteger<ssize_t>);

#if defined(INT128_AVAILABLE)
static_assert(regina::CppInteger<regina::UInt128>);
static_assert(regina::UnsignedCppInteger<regina::UInt128>);
static_assert(! regina::SignedCppInteger<regina::UInt128>);

static_assert(regina::CppInteger<regina::Int128>);
static_assert(! regina::UnsignedCppInteger<regina::Int128>);
static_assert(regina::SignedCppInteger<regina::Int128>);
#endif
