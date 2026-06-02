
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

/*! \file concepts/core.h
 *  \brief Concepts related to iterator types.
 */

#ifndef __REGINA_CONCEPTS_ITERATOR_H
#ifndef __DOXYGEN
#define __REGINA_CONCEPTS_ITERATOR_H
#endif

#include <iterator>
#include "concepts/core.h"

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

class Packet;

/**
 * An input iterator whose dereferenced values can be assigned or converted
 * to the type \a Target.
 *
 * Both construction and the assignment operator should be supported, and
 * implicit conversion should be supported also.
 *
 * \ingroup concepts
 */
template <typename T, typename Target>
concept InputIteratorFor =
    std::input_iterator<T> &&
    std::assignable_from<Target&, decltype(*std::declval<T&>())> &&
    std::constructible_from<Target, decltype(*std::declval<T&>())> &&
    std::convertible_to<decltype(*std::declval<T&>()), Target>;

/**
 * A forward iterator whose dereferenced values can be assigned or converted
 * to the type \a Target.
 *
 * Both construction and the assignment operator should be supported, and
 * implicit conversion should be supported also.
 *
 * \ingroup concepts
 */
template <typename T, typename Target>
concept ForwardIteratorFor =
    std::forward_iterator<T> &&
    std::assignable_from<Target&, decltype(*std::declval<T&>())> &&
    std::constructible_from<Target, decltype(*std::declval<T&>())> &&
    std::convertible_to<decltype(*std::declval<T&>()), Target>;

/**
 * A bidirectional iterator whose dereferenced values can be assigned or
 * converted to the type \a Target.
 *
 * Both construction and the assignment operator should be supported, and
 * implicit conversion should be supported also.
 *
 * \ingroup concepts
 */
template <typename T, typename Target>
concept BidirectionalIteratorFor =
    std::bidirectional_iterator<T> &&
    std::assignable_from<Target&, decltype(*std::declval<T&>())> &&
    std::constructible_from<Target, decltype(*std::declval<T&>())> &&
    std::convertible_to<decltype(*std::declval<T&>()), Target>;

/**
 * A random access iterator whose dereferenced values can be assigned or
 * converted to the type \a Target.
 *
 * Both construction and the assignment operator should be supported, and
 * implicit conversion should be supported also.
 *
 * \ingroup concepts
 */
template <typename T, typename Target>
concept RandomAccessIteratorFor =
    std::random_access_iterator<T> &&
    std::assignable_from<Target&, decltype(*std::declval<T&>())> &&
    std::constructible_from<Target, decltype(*std::declval<T&>())> &&
    std::convertible_to<decltype(*std::declval<T&>()), Target>;

/**
 * An input iterator that knows when iteration has finished.
 *
 * Specifically, such an iterator should have a `bool` operator that returns
 * \c true if and only if the iterator is dereferenceable.
 *
 * \ingroup concepts
 */
template <typename T>
concept SelfSentinelInputIterator =
    std::input_iterator<T> &&
    requires(T x) { bool(x); };

/**
 * An input iterator that iterates over characters.  Such an iterator would
 * typically be used when reading characters from an input stream or a string.
 *
 * Dereferencing the iterator should yield a `char`, possibly as a reference
 * and possibly `const`.
 *
 * \ingroup concepts
 */
template <typename T>
concept CharIterator =
    std::input_iterator<T> &&
    std::same_as<char, std::remove_cvref_t<decltype(*std::declval<T&>())>>;

/**
 * An input iterator that iterates over packets.
 *
 * Dereferencing the iterator should yield a reference type `Packet&` (possibly
 * `const`).  Moreover, the iterator should have a `bool` operator that returns
 * \c true if and only if the iterator is dereferenceable.
 *
 * \ingroup concepts
 */
template <typename T>
concept PacketIterator =
    SelfSentinelInputIterator<T> &&
    (std::same_as<Packet&, decltype(*std::declval<T&>())> ||
        std::same_as<const Packet&, decltype(*std::declval<T&>())>);

/**
 * A container-like type that can be iterated over via `begin()` and `end()`
 * member functions.
 *
 * The corresponding iterator type must be a forward iterator.
 *
 * \ingroup concepts
 */
template <typename T>
concept Iterable =
    requires(T x) {
        { x.begin() } -> std::forward_iterator;
        { x.end() } -> std::same_as<decltype(x.begin())>;
    };

/**
 * A container-like type that can be iterated over via `begin()`
 * and `end()` member functions, and whose elements can be assigned or
 * converted to the type \a Target.
 *
 * The corresponding iterator type must be a forward iterator.
 *
 * Here _elements_ means the values obtained when dereferencing iterators.
 * When converting elements to the type \a Target, both construction and the
 * assignment operator should be supported, and implicit conversion should be
 * supported also.
 *
 * \ingroup concepts
 */
template <typename T, typename Target>
concept IterableFor =
    Iterable<T> &&
    ForwardIteratorFor<decltype(std::declval<T>().begin()), Target>;

/**
 * A container-like type that can be iterated over both forwards and backwards
 * via `begin()` and `end()` member functions, and whose elements can be
 * assigned or converted to the type \a Target.
 *
 * The corresponding iterator type must be a bidirectional iterator.
 *
 * Here _elements_ means the values obtained when dereferencing iterators.
 * When converting elements to the type \a Target, both construction and the
 * assignment operator should be supported, and implicit conversion should be
 * supported also.
 *
 * \ingroup concepts
 */
template <typename T, typename Target>
concept BidirectionalIterableFor =
    Iterable<T> &&
    BidirectionalIteratorFor<decltype(std::declval<T>().begin()), Target>;

/**
 * A container-like type that can be iterated over in a random access manner
 * via `begin()` and `end()` member functions, and whose elements can be
 * assigned or converted to the type \a Target.
 *
 * The corresponding iterator type must be a random access iterator.
 *
 * Here _elements_ means the values obtained when dereferencing iterators.
 * When converting elements to the type \a Target, both construction and the
 * assignment operator should be supported, and implicit conversion should be
 * supported also.
 *
 * \ingroup concepts
 */
template <typename T, typename Target>
concept RandomAccessIterableFor =
    Iterable<T> &&
    RandomAccessIteratorFor<decltype(std::declval<T>().begin()), Target>;

/**
 * An output iterator type.
 *
 * The reason for using OutputIterator instead of std::output_iterator is that
 * this concept does not require you to specify the output type in advance.
 * Instead, the output type is deduced automatically via `std::iter_value_t`.
 *
 * \ingroup concepts
 */
template <typename T>
concept OutputIterator =
    std::input_or_output_iterator<T> &&
    std::output_iterator<T, std::iter_value_t<T>>;

/**
 * A container-like type whose elements can be access via indexing.
 *
 * We do _not_ require that the elements be accessible via iteration.
 * We do however require some other parts of a standard container interface,
 * including `T::value_type` and `T::size()`.
 *
 * \ingroup concepts
 */
template <typename T>
concept IndexedContainer =
    requires(T x, size_t index) {
        typename T::value_type;
        { x.size() } -> std::convertible_to<size_t>;
        { x[index] } -> SameModCVRef<typename T::value_type>;
    };

} // namespace regina

#endif

