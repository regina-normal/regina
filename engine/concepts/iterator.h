
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
 *  \brief Concepts related to iterator types.
 */

#ifndef __REGINA_CONCEPTS_ITERATOR_H
#ifndef __DOXYGEN
#define __REGINA_CONCEPTS_ITERATOR_H
#endif

#include <iterator>
#include "regina-core.h"

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
 * A type that can be iterated over via `begin()` and `end()` member functions.
 *
 * \ingroup concepts
 */
template <typename T>
concept Iterable =
    requires(T x) {
        { x.begin() };
        { x.end() };
        requires std::same_as<decltype(x.begin()), decltype(x.end())>;
        requires std::forward_iterator<decltype(x.begin())>;
    };

} // namespace regina

#endif

