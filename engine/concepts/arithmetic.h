
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

/*! \file concepts/arithmetic.h
 *  \brief Concepts related to arithmetical operations.
 */

#ifndef __REGINA_CONCEPTS_ARITHMETIC_H
#ifndef __DOXYGEN
#define __REGINA_CONCEPTS_ARITHMETIC_H
#endif

#include <concepts>
#include "regina-core.h"

namespace regina {

/**
 * A type that supports very basic interoperability with integer values,
 * via construction, assignment, and equality/inequality testing.
 *
 * \ingroup concepts
 */
template <typename T>
concept IntegerCompatible =
    std::constructible_from<T, int> &&
    std::assignable_from<T&, int> &&
    std::equality_comparable_with<T, int>;

/**
 * A type that supports interoperability with integer values via construction,
 * assignment, equality/inequality testing, and comparisons.  The comparisons
 * must yield a total order.
 *
 * \ingroup concepts
 */
template <typename T>
concept IntegerComparable =
    IntegerCompatible<T> &&
    std::totally_ordered_with<T, int>;

/**
 * A type that has the necessary operations to behave like a mathematical ring.
 *
 * \ingroup concepts
 */
template <typename T>
concept RingLike =
    std::regular<T> &&
    requires(const T a, const T b, T x) {
        { a + b } -> std::convertible_to<T>;
        { a - b } -> std::convertible_to<T>;
        { a * b } -> std::convertible_to<T>;
        { -a } -> std::convertible_to<T>;
        { x += a } -> std::same_as<T&>;
        { x -= a } -> std::same_as<T&>;
        { x *= a } -> std::same_as<T&>;
    };

template <RingLike> struct RingTraits;

/**
 * A type that behaves like a mathematical ring, and for which the
 * specialisation `RingTraits<T>` is available.
 *
 * \ingroup concepts
 */
template <typename T>
concept Ring =
    RingLike<T> &&
    requires {
        { RingTraits<T>::zero } -> std::convertible_to<T>;
        { RingTraits<T>::one } -> std::convertible_to<T>;
    };

/**
 * A ring with no zero divisors.
 *
 * The property of having no zero divisors is self-identified through the
 * specialisation `RingTraits<T>`.
 *
 * \ingroup concepts
 */
template <typename T>
concept Domain = Ring<T> && ! RingTraits<T>::zeroDivisors;

/**
 * A commutative ring with no zero divisors.
 *
 * Commutativity and the property of having no zero divisors are both
 * self-identified through the specialisation `RingTraits<T>`.
 *
 * \ingroup concepts
 */
template <typename T>
concept IntegralDomain = Domain<T> && RingTraits<T>::commutative;

/**
 * A type suitable to use for coefficients in Regina's polynomial-like classes.
 *
 * This concept is tailored to Regina's own requirements, and so is stricter
 * than the mathematical requirements for polynomial coefficients.  For example,
 * we insist here on no zero divisors (to support division-related algorithms),
 * and we insist on default constructors that initialise to zero (to simplify
 * algorithm implementations).
 *
 * \ingroup concepts
 */
template <typename T>
concept CoefficientDomain =
    Domain<T> && IntegerComparable<T> && RingTraits<T>::zeroInitialised;

} // namespace regina

#endif

