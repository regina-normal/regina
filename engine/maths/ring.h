
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

#include "utilities/intutils.h"

namespace regina {

/**
 * A helper class that instructs Regina how to do mathematical operations
 * with objects from any ring type \a T.
 *
 * Before specialising `RingTraits<T>`, you should ensure that \a T satisfies
 * the following requirements:
 *
 * - \a T has a default constructor (which is allowed to leave the object
 *   uninitialised);
 *
 * - \a T has a copy constructor, an assignment operator, and
 *   equality/inequality tests;
 *
 * - \a T supports the binary operators `+`, `-` and `*`, and unary operators
 *   `+=`, `-=` and `*=`.
 *
 * The type `RingTraits<T>` should then provide:
 *
 * - class constants `zero` and `one`, which are objects of type \a T that
 *   hold the additive and multiplicative identities respectively.
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
template <typename T>
struct RingTraits;

#ifndef __DOXYGEN
// Don't confuse doxygen with specialisations.

/**
 * Provides the specialisation of RingType for a native C++ integer or
 * floating point type.
 *
 * \param T the C++ integer or floating point type being described.
 *
 * \ingroup maths
 */
#define NATIVE_RINGTYPE(T) \
template <> \
struct RingTraits<T> { \
    static constexpr T zero = 0; \
    static constexpr T one = 1; \
}

NATIVE_RINGTYPE(int8_t);
NATIVE_RINGTYPE(int16_t);
NATIVE_RINGTYPE(int32_t);
NATIVE_RINGTYPE(int64_t);
#ifdef INT128_AVAILABLE
NATIVE_RINGTYPE(IntOfSize<16>::type);
#endif

NATIVE_RINGTYPE(float);
NATIVE_RINGTYPE(double);
NATIVE_RINGTYPE(long double);

#endif // __DOXYGEN

} // namespace regina

#endif

