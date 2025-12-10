
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
#include "maths/arrow.h"
#include "maths/cyclotomic.h"
#include "maths/integer.h"
#include "maths/matrix.h"
#include "maths/polynomial.h"
#include "maths/vector.h"

using regina::Integer;
using regina::LargeInteger;
using regina::MatrixInt;
using regina::NativeInteger;
using regina::Vector;

using regina::ArbitraryPrecisionIntegerVector;
using regina::CppInteger;
using regina::IntegerVector;
using regina::SignedCppInteger;
using regina::StandardCppInteger;
using regina::UnsignedCppInteger;

namespace {
    template <typename T>
    class SubVector : public Vector<T> {};
}

static_assert(CppInteger<char>);
static_assert(StandardCppInteger<char>);

static_assert(CppInteger<signed char>);
static_assert(StandardCppInteger<signed char>);
static_assert(SignedCppInteger<signed char>);
static_assert(! UnsignedCppInteger<signed char>);

static_assert(CppInteger<unsigned char>);
static_assert(StandardCppInteger<unsigned char>);
static_assert(! SignedCppInteger<unsigned char>);
static_assert(UnsignedCppInteger<unsigned char>);

static_assert(CppInteger<int>);
static_assert(StandardCppInteger<int>);
static_assert(SignedCppInteger<int>);
static_assert(! UnsignedCppInteger<int>);

static_assert(CppInteger<unsigned>);
static_assert(StandardCppInteger<unsigned>);
static_assert(! SignedCppInteger<unsigned>);
static_assert(UnsignedCppInteger<unsigned>);

static_assert(CppInteger<size_t>);
static_assert(StandardCppInteger<size_t>);
static_assert(! SignedCppInteger<size_t>);
static_assert(UnsignedCppInteger<size_t>);

static_assert(CppInteger<ssize_t>);
static_assert(StandardCppInteger<ssize_t>);
static_assert(SignedCppInteger<ssize_t>);
static_assert(! UnsignedCppInteger<ssize_t>);

#if defined(INT128_AVAILABLE)
// 128-bit integers might or might not be standard C++ types.
// Therefore we do not test adherence to StandardCppInteger here.
static_assert(CppInteger<regina::IntOfSize<16>::type>);
static_assert(SignedCppInteger<regina::IntOfSize<16>::type>);
static_assert(! UnsignedCppInteger<regina::IntOfSize<16>::type>);

static_assert(CppInteger<regina::IntOfSize<16>::utype>);
static_assert(! SignedCppInteger<regina::IntOfSize<16>::utype>);
static_assert(UnsignedCppInteger<regina::IntOfSize<16>::utype>);
#endif

static_assert(! CppInteger<bool>);
static_assert(! StandardCppInteger<bool>);
static_assert(! SignedCppInteger<bool>);
static_assert(! UnsignedCppInteger<bool>);

static_assert(! CppInteger<Integer>);
static_assert(! StandardCppInteger<Integer>);
static_assert(! SignedCppInteger<Integer>);
static_assert(! UnsignedCppInteger<Integer>);

static_assert(! CppInteger<LargeInteger>);
static_assert(! StandardCppInteger<LargeInteger>);
static_assert(! SignedCppInteger<LargeInteger>);
static_assert(! UnsignedCppInteger<LargeInteger>);

static_assert(! CppInteger<NativeInteger<8>>);
static_assert(! StandardCppInteger<NativeInteger<8>>);
static_assert(! SignedCppInteger<NativeInteger<8>>);
static_assert(! UnsignedCppInteger<NativeInteger<8>>);

static_assert(ArbitraryPrecisionIntegerVector<Vector<Integer>>);
static_assert(IntegerVector<Vector<Integer>>);

static_assert(ArbitraryPrecisionIntegerVector<SubVector<Integer>>);
static_assert(IntegerVector<SubVector<Integer>>);

static_assert(! ArbitraryPrecisionIntegerVector<Vector<NativeInteger<8>>>);
static_assert(IntegerVector<Vector<NativeInteger<8>>>);

static_assert(! ArbitraryPrecisionIntegerVector<SubVector<NativeInteger<8>>>);
static_assert(IntegerVector<SubVector<NativeInteger<8>>>);

static_assert(! ArbitraryPrecisionIntegerVector<Vector<int>>);
static_assert(! IntegerVector<Vector<int>>);

static_assert(! ArbitraryPrecisionIntegerVector<SubVector<int>>);
static_assert(! IntegerVector<SubVector<int>>);

static_assert(! ArbitraryPrecisionIntegerVector<MatrixInt>);
static_assert(! IntegerVector<MatrixInt>);

static_assert(regina::Ring<int>);
static_assert(! regina::Ring<unsigned>);
static_assert(regina::RingLike<unsigned>);
static_assert(regina::Ring<regina::Arrow>);
static_assert(regina::Ring<regina::Polynomial<Integer>>);
static_assert(! regina::Ring<regina::Cyclotomic>);
static_assert(regina::RingLike<regina::Cyclotomic>);

