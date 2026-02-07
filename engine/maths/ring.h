
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

/*! \file maths/ring.h
 *  \brief Utilities for writing generic code that can work in arbitrary
 *  (mathematical) rings.
 */

#ifndef __REGINA_RINGUTILS_H
#ifndef __DOXYGEN
#define __REGINA_RINGUTILS_H
#endif

#include "concepts/core.h"
#include "utilities/intutils.h" // for INT128_AVAILABLE

namespace regina {

/**
 * A helper class that assists Regina in doing mathematical operations with
 * objects of any ring-like type \a T.
 *
 * The concept `RingLike<T>` already ensures that \a T provides the basic
 * syntax and mathematical operations for working over a ring.
 *
 * What `RingTraits<T>` provides in addition to this is:
 *
 * - class constants `RingTraits<T>::zero` and `RingTraits<T>::one`, which are
 *   objects of type \a T that hold the additive and multiplicative identities
 *   respectively;
 *
 * - a compile-time boolean constant `RingTraits<T>::commutative`, which
 *   identifies whether multiplication is commutative in the ring;
 *
 * - a compile-time boolean constant `RingTraits<T>::zeroInitialised`, which
 *   identifies whether the default constructor for \a T initialises the value
 *   to zero (as opposed to leaving it uninitialised);
 *
 * - a compile-time boolean constant `RingTraits<T>::zeroDivisors`, which is
 *   \c true if and only if it is possible to multiply two non-zero values of
 *   type \a T to obtain zero.  Here some judgement calls must be made.  For
 *   example, we treat native C++ integer arithmetic as having zero divisors
 *   because this is a well-defined integer arithmetic modulo some power of two,
 *   and divisors of zero are often small and easy to produce (e.g., `16 × 16`
 *   for the type `uint8_t`).  On the other hand, we treat native floating point
 *   arithmetic as _not_ having zero divisors, even though it is possible to
 *   obtain zero by exceeding the available floating point precision, because
 *   such a scenario feels more like the user exceeding the limits of the
 *   hardware.  Likewise, for polynomial types, we assume that exponents will
 *   never grow so large that they overflow and create zero divisors that way.
 *
 * - a compile-time boolean constant `RingTraits<T>::inverses`, which is
 *   \c true if and only if every non-zero object of type \a T has a
 *   multiplicative inverse.  As with `RingTraits<T>::zeroDivisors`,
 *   judgement calls must be made; for example, we treat native floating point
 *   arithmetic as having inverses despite the difficulties around precision
 *   and overflow.
 *
 * Regina specialises RingTraits for its own ring-like classes where this
 * makes sense (e.g., Regina's own integer, rational and polynomial classes),
 * and also provides implementations for native C++ signed integer and
 * floating point types.
 *
 * \nopython
 *
 * \ingroup maths
 */
template <RingLike T>
struct RingTraits;

#ifndef __DOXYGEN
// Don't confuse doxygen with specialisations.

/**
 * Provides the specialisation of RingType for a native C++ integer type.
 *
 * \param T the C++ integer type being described.
 *
 * \ingroup maths
 */
#define NATIVE_INTEGER_RINGTYPE(T) \
template <> \
struct RingTraits<T> { \
    static constexpr T zero = 0; \
    static constexpr T one = 1; \
    static constexpr bool commutative = true; \
    static constexpr bool zeroInitialised = false; \
    static constexpr bool zeroDivisors = true; \
    static constexpr bool inverses = false; \
}

NATIVE_INTEGER_RINGTYPE(int8_t);
NATIVE_INTEGER_RINGTYPE(int16_t);
NATIVE_INTEGER_RINGTYPE(int32_t);
NATIVE_INTEGER_RINGTYPE(int64_t);
#ifdef INT128_AVAILABLE
NATIVE_INTEGER_RINGTYPE(Int128);
#endif

/**
 * Provides the specialisation of RingType for a native C++ floating point type.
 *
 * \param T the C++ floating point type being described.
 *
 * \ingroup maths
 */
#define NATIVE_FLOATING_POINT_RINGTYPE(T) \
template <> \
struct RingTraits<T> { \
    static constexpr T zero = 0; \
    static constexpr T one = 1; \
    static constexpr bool commutative = true; \
    static constexpr bool zeroInitialised = false; \
    static constexpr bool zeroDivisors = false; \
    static constexpr bool inverses = true; \
}

NATIVE_FLOATING_POINT_RINGTYPE(float);
NATIVE_FLOATING_POINT_RINGTYPE(double);
NATIVE_FLOATING_POINT_RINGTYPE(long double);

#endif // __DOXYGEN

} // namespace regina

#endif

