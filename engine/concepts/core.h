
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

/*! \file concepts/core.h
 *  \brief Concepts related to core C++ and Regina types.
 */

#ifndef __REGINA_CONCEPTS_CORE_H
#ifndef __DOXYGEN
#define __REGINA_CONCEPTS_CORE_H
#endif

#include <concepts>
#include "utilities/intutils.h"

namespace regina {

template <bool> class IntegerBase;
template <int> class NativeInteger;
// More forward declarations appear later, once we have the concepts necessary
// to make them.

/**
 * \defgroup concepts Concepts
 * C++ concepts for describing constraints on types.
 */

/**
 * Indicates that a variable of type \a Source can be assigned to a variablei
 * of type \a Target.  This is identical to `std::assignable_from`, but with
 * the arguments in the opposite order.
 *
 * Typically \a Target would be an lvalue reference.
 *
 * \ingroup concepts
 */
template <typename Source, typename Target>
concept AssignableTo = std::assignable_from<Target, Source>;

/**
 * Indicates that a variable of type \a Source can be used to construct a
 * variable of type \a Target.  This is identical to `std::constructible_from`,
 * but with the arguments in the opposite order.
 *
 * \ingroup concepts
 */
template <typename Source, typename Target>
concept CanConstruct = std::constructible_from<Target, Source>;

/**
 * One of the standard non-boolean C++ integer types, without making any
 * special accommodations for 128-bit integer compiler extensions.
 *
 * This concept is exactly like `std::integral` but with `bool` excluded.
 *
 * \ingroup concepts
 */
template <typename T>
concept StandardCppInteger = std::integral<T> && ! std::same_as<T, bool>;

/**
 * A native non-boolean C++ integer type, allowing for 128-bit integers also
 * if these are supported by the compiler.
 *
 * See the constant regina::is_cpp_integer_v for further details.
 *
 * \ingroup concepts
 */
template <typename T>
concept CppInteger = is_cpp_integer_v<T>;

/**
 * A signed native non-boolean C++ integer type, allowing for 128-bit integers
 * also if these are supported by the compiler.
 *
 * See the constant regina::is_signed_cpp_integer_v for further details.
 *
 * \ingroup concepts
 */
template <typename T>
concept SignedCppInteger = is_signed_cpp_integer_v<T>;

/**
 * An unsigned native non-boolean C++ integer type, allowing for 128-bit
 * integers also if these are supported by the compiler.
 *
 * See the constant regina::is_unsigned_cpp_integer_v for further details.
 *
 * \ingroup concepts
 */
template <typename T>
concept UnsignedCppInteger = is_unsigned_cpp_integer_v<T>;

/**
 * One of Regina's arbitrary precision integer types (Integer or LargeInteger).
 *
 * \ingroup concepts
 */
template <typename T>
concept ArbitraryPrecisionInteger =
    std::is_same_v<IntegerBase<true>, T> ||
    std::is_same_v<IntegerBase<false>, T>;

/**
 * One of Regina's own integer types (Integer, LargeInteger, or NativeInteger).
 *
 * An important feature of all of Regina's integer types is that their default
 * constructors initialise the integers to zero.
 *
 * \ingroup concepts
 */
template <typename T>
concept ReginaInteger =
    ArbitraryPrecisionInteger<T> ||
    requires(T x) { { NativeInteger(x) } -> std::same_as<T>; };

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

// Forward declarations that require the concept RingLike:
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

// Forward declarations that require the concept UnsignedCppInteger:
class Bitmask;
template <UnsignedCppInteger> class Bitmask1;
template <UnsignedCppInteger, UnsignedCppInteger> class Bitmask2;

/**
 * One of Regina's own bitmask types.
 *
 * \ingroup concepts
 */
template <typename T>
concept ReginaBitmask =
    std::same_as<T, Bitmask> ||
    requires(T x) { { Bitmask1(x) } -> std::same_as<T>; } ||
    requires(T x) { { Bitmask2(x) } -> std::same_as<T>; };

} // namespace regina

#endif

