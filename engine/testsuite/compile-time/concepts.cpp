
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

#include "concepts/core.h"
#include "concepts/maths.h"
#include "maths/integer.h"
#include "maths/matrix.h"
#include "maths/vector.h"

using regina::Integer;
using regina::LargeInteger;
using regina::MatrixInt;
using regina::NativeInteger;
using regina::Vector;

// Some of our tests below use types that are not compile-time constructible
// (e.g., regina::Vector).  For these tests we work with pointers, so that
// there is no need to ever construct actual objects, and therefore our tests
// can remain compile-time.

// Integer tests:

namespace {
    // --- Testing adherence to CppInteger ---

    template <regina::CppInteger T>
    constexpr bool cppInt(T* x) { return true; }

    template <typename T>
    constexpr bool cppInt(T* x) { return false; }

    // --- Testing adherence to SignedCppInteger ---

    template <regina::SignedCppInteger T>
    constexpr bool signedCppInt(T* x) { return true; }

    template <typename T>
    constexpr bool signedCppInt(T* x) { return false; }

    // --- Testing adherence to UnsignedCppInteger ---

    template <regina::UnsignedCppInteger T>
    constexpr bool unsignedCppInt(T* x) { return true; }

    template <typename T>
    constexpr bool unsignedCppInt(T* x) { return false; }

    // --- The actual tests ---

    static_assert(cppInt((char*)(nullptr)));

    static_assert(cppInt((signed char*)(nullptr)));
    static_assert(signedCppInt((signed char*)(nullptr)));
    static_assert(! unsignedCppInt((signed char*)(nullptr)));

    static_assert(cppInt((unsigned char*)(nullptr)));
    static_assert(! signedCppInt((unsigned char*)(nullptr)));
    static_assert(unsignedCppInt((unsigned char*)(nullptr)));

    static_assert(cppInt((int*)(nullptr)));
    static_assert(signedCppInt((int*)(nullptr)));
    static_assert(! unsignedCppInt((int*)(nullptr)));

    static_assert(cppInt((unsigned*)(nullptr)));
    static_assert(! signedCppInt((unsigned*)(nullptr)));
    static_assert(unsignedCppInt((unsigned*)(nullptr)));

    static_assert(cppInt((size_t*)(nullptr)));
    static_assert(! signedCppInt((size_t*)(nullptr)));
    static_assert(unsignedCppInt((size_t*)(nullptr)));

    static_assert(cppInt((ssize_t*)(nullptr)));
    static_assert(signedCppInt((ssize_t*)(nullptr)));
    static_assert(! unsignedCppInt((ssize_t*)(nullptr)));

    #if defined(INT128_AVAILABLE)
    static_assert(cppInt((regina::IntOfSize<16>::type*)(nullptr)));
    static_assert(signedCppInt((regina::IntOfSize<16>::type*)(nullptr)));
    static_assert(! unsignedCppInt((regina::IntOfSize<16>::type*)(nullptr)));

    static_assert(cppInt((regina::IntOfSize<16>::utype*)(nullptr)));
    static_assert(! signedCppInt((regina::IntOfSize<16>::utype*)(nullptr)));
    static_assert(unsignedCppInt((regina::IntOfSize<16>::utype*)(nullptr)));
    #endif

    static_assert(! cppInt((bool*)(nullptr)));
    static_assert(! signedCppInt((bool*)(nullptr)));
    static_assert(! unsignedCppInt((bool*)(nullptr)));

    static_assert(! cppInt((Integer*)(nullptr)));
    static_assert(! signedCppInt((Integer*)(nullptr)));
    static_assert(! unsignedCppInt((Integer*)(nullptr)));

    static_assert(! cppInt((LargeInteger*)(nullptr)));
    static_assert(! signedCppInt((LargeInteger*)(nullptr)));
    static_assert(! unsignedCppInt((LargeInteger*)(nullptr)));

    static_assert(! cppInt((NativeInteger<8>*)(nullptr)));
    static_assert(! signedCppInt((NativeInteger<8>*)(nullptr)));
    static_assert(! unsignedCppInt((NativeInteger<8>*)(nullptr)));
}

// Vector tests:

namespace {
    // A strict subclass of Vector<...>.
    template <typename T>
    class SubVector : public Vector<T> {};

    // --- Testing adherence to ArbitraryPrecisionIntegerVector ---

    template <regina::ArbitraryPrecisionIntegerVector T>
    constexpr bool apiVec(T* x) { return true; }

    template <typename T>
    constexpr bool apiVec(T* x) { return false; }

    // --- Testing adherence to IntegerVector ---

    template <regina::IntegerVector T>
    constexpr bool intVec(T* x) { return true; }

    template <typename T>
    constexpr bool intVec(T* x) { return false; }

    // --- The actual tests ---

    static_assert(apiVec((Vector<Integer>*)(nullptr)));
    static_assert(intVec((Vector<Integer>*)(nullptr)));

    static_assert(apiVec((SubVector<Integer>*)(nullptr)));
    static_assert(intVec((SubVector<Integer>*)(nullptr)));

    static_assert(! apiVec((Vector<NativeInteger<8>>*)(nullptr)));
    static_assert(intVec((Vector<NativeInteger<8>>*)(nullptr)));

    static_assert(! apiVec((SubVector<NativeInteger<8>>*)(nullptr)));
    static_assert(intVec((SubVector<NativeInteger<8>>*)(nullptr)));

    static_assert(! apiVec((Vector<int>*)(nullptr)));
    static_assert(! intVec((Vector<int>*)(nullptr)));

    static_assert(! apiVec((SubVector<int>*)(nullptr)));
    static_assert(! intVec((SubVector<int>*)(nullptr)));

    static_assert(! apiVec((MatrixInt*)(nullptr)));
    static_assert(! intVec((MatrixInt*)(nullptr)));
}

