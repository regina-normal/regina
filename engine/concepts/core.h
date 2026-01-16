
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

#include "utilities/intutils.h" // provides concepts for integer types

namespace regina {

class Bitmask;
template <UnsignedCppInteger> class Bitmask1;
template <UnsignedCppInteger, UnsignedCppInteger> class Bitmask2;

/**
 * \defgroup concepts Concepts
 * C++ concepts for describing constraints on types.
 */

/**
 * Indicates that a variable of type \a Source can be assigned to a variable
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
 * A mathematical field.
 *
 * The property of being a field is self-identified through the various
 * constants in the specialisation `RingTraits<T>`.
 *
 * \ingroup concepts
 */
template <typename T>
concept Field = IntegralDomain<T> && RingTraits<T>::inverses;

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

/**
 * A type that holds some ordered sequence of data, and that supports
 * concatenation.  An example of this is `std::string`.
 *
 * Important semantic requirements for this type are:
 *
 * - the operation `x += y` must concatenate \a y to the end of \a x;
 *
 * - the default constructor must create the empty sequence.
 *
 * \ingroup concepts
 */
template <typename T>
concept ConcatenableSequence =
    std::regular<T> &&
    std::totally_ordered<T> &&
    requires(T x, const T y) { { x += y } -> std::same_as<T&>; };

/**
 * A callable type that acts as a strict weak order on the given argument type.
 * Such an object could (for example) be used for comparisons during a sorting
 * operation.
 *
 * This concept is identical to the standard C++ concept
 * `std::strict_weak_order<T, Arg, Arg>`.  It is provided here for convenience
 * so that the argument type does not need to be repeated.
 *
 * \ingroup concepts
 */
template <typename T, typename Arg>
concept StrictWeakOrder = std::strict_weak_order<T, Arg, Arg>;

/**
 * A callable type that takes the given argument types, and whose return value
 * is ignored.  Such objects are often passed into Regina's enumeration
 * routines (e.g., the various triangulation and link census generation
 * routines).
 *
 * Typically the return type for such a callback would be `void` (since Regina
 * will ignore it), though this is not enforced.
 *
 * \ingroup concepts
 */
template <typename T, typename... Args>
concept VoidCallback = std::invocable<T, Args...>;

/**
 * A callable type that takes the given argument types, with a boolean return
 * value indicating whether the current operation should terminate.
 * Such objects are often passed into Regina's enumeration and/or exploration
 * routines (e.g., for enumerating normal surfaces, or exploring nearby
 * retriangulations).
 *
 * The return type for such a callback must be convertible to `bool`.
 * A return value of `false` (no, do not terminate) would typically indicate
 * that the current operation should continue, and `true` (yes, terminate)
 * would typically indicate that it the operation should stop.
 *
 * \ingroup concepts
 */
template <typename T, typename... Args>
concept TerminatingCallback = std::predicate<T, Args...>;

} // namespace regina

#endif

