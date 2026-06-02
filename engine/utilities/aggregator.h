
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

/*! \file utilities/aggregator.h
 *  \brief Provides structures to help with aggregating values in algorithms.
 */

#ifndef __REGINA_AGGREGATE_H
#ifndef __DOXYGEN
#define __REGINA_AGGREGATE_H
#endif

#include "utilities/exception.h"
#include <compare>
#include <concepts>
#include <type_traits>
#include <utility> // for std::swap

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

/**
 * A structure to help compute the maximum of many values of type \a T, taking
 * multiplicity into account.
 *
 * We refer to individual values of type \a T as _atomic_ values.  This
 * structure maintains the maximum value encountered so far, along with the
 * number of atomic values encountered so far that are equivalent to that
 * maximum.
 *
 * Note that we do not require `T::operator <=>` to be a total order (this
 * allows us to work with floating-point types, for example, where `NaN` is
 * incomparable with any other value).  However, aggregating incomparable
 * atomic values is an error, and may result in an exception being thrown.
 * See aggregate() for further details.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  Its implementations of these rely upon type \a T; that is,
 * the efficiency of moving and swapping objects of type `MaxAggregator<T>`
 * depends upon the efficiency of moving and swapping objects of type \a T.
 *
 * \nopython
 *
 * \ingroup utilities
 */
template <typename T>
requires std::regular<T> && std::three_way_comparable<T>
class MaxAggregator {
    public:
        /**
         * The result of a three-way comparison on type \a T.
         *
         * This would typically be one of the standard C++ ordering types,
         * such as `std::strong_ordering`.
         */
        using OrderType = decltype(T() <=> T());

    private:
        T max_;
            /**< The maximum atomic value encountered so far.
                 This is undefined if `count_ == 0. */
        size_t count_ { 0 };
            /**< The number of atomic values encountered so far that are
                 equivalent to \a max_, or 0 if no atomic values have been
                 encountered at all. */

    public:
        /**
         * Creates a new aggregator that has not yet encountered any
         * atomic values at all.
         */
        constexpr MaxAggregator() = default;
        /**
         * Creates a new copy of the given aggregator.
         */
        constexpr MaxAggregator(const MaxAggregator&) = default;
        /**
         * Moves the contents of the given aggregator into this new aggregator.
         *
         * This constructor is not marked `noexcept`, since its throwing
         * behaviour depends upon the throwing behaviour of the move
         * constructor for type \a T.
         *
         * The aggregator that is passed will no longer be usable.
         */
        constexpr MaxAggregator(MaxAggregator&&) = default;

        /**
         * Determines whether or not this aggregator has encountered any
         * atomic values at all.
         *
         * \return \c true if and only if this aggregator has _not_ yet
         * encountered any atomic values.
         */
        constexpr bool empty() const {
            return count_ == 0;
        }

        /**
         * Returns the maximum atomic value that this aggregator has
         * encountered so far.
         *
         * If no values have been encountered at all then this routine will
         * throw an exception.
         *
         * \exception NoSolution No atomic values have yet been encountered.
         *
         * \return the maximum atomic value that has been encountered.
         */
        constexpr const T& value() const {
            if (count_ == 0)
                throw NoSolution();
            return max_;
        }

        /**
         * Returns the number of atomic values encountered so far that are
         * equivalent to the current maximum.
         *
         * If no values have been encountered at all then this routine will
         * return zero.
         *
         * \return the number of values equivalent to the current maximum.
         */
        constexpr size_t count() const {
            return count_;
        }

        /**
         * Sets this to be a copy of the given aggregator.
         *
         * \return a reference to this aggregator.
         */
        constexpr MaxAggregator& operator = (const MaxAggregator&) = default;
        /**
         * Moves the contents of the given aggregator into this aggregator.
         *
         * This operator is not marked `noexcept`, since its throwing
         * behaviour depends upon the throwing behaviour of the move assignment
         * operator for type \a T.
         *
         * \return a reference to this aggregator.
         */
        constexpr MaxAggregator& operator = (MaxAggregator&&) = default;

        /**
         * Swaps the contents of this and the given aggregator.
         *
         * This operation is not marked `noexcept`, since its throwing behaviour
         * depends upon the throwing behaviour of the swap operation for type
         * \a T.
         *
         * \param other the aggregator whose contents are to be swapped with
         * this.
         */
        constexpr void swap(MaxAggregator& other) noexcept
                requires std::swappable<T> {
            using std::swap;
            swap(max_, other.max_); // uses T's swap operation, if defined
            swap(count_, other.count_);
        }

        /**
         * Determines whether this and the given aggregator hold equivalent
         * maximum values with the same multiplicity.
         *
         * Two aggregators that have not encountered any atomic values at all
         * will be considered equal.
         *
         * \param rhs the aggregator to compare with this.
         * \return \c true if and only if this and the given aggregator hold
         * equivalent values with the same multiplicity.
         */
        bool operator == (const MaxAggregator& rhs) const {
            if (count_ == 0)
                return rhs.count_ == 0; // max_ should be ignored in this case
            else
                return max_ == rhs.max_ && count_ == rhs.count_;
        }

        /**
         * Compares the results of this and the given aggregator.
         *
         * For aggregators \a x and \a y, `x < y` means that either \a x holds
         * a smaller maximum value than \a y, or they hold equivalent values
         * but \a x has encountered that value fewer times.
         *
         * An aggregator that has not encountered any atomic values at all
         * is considered smaller than any aggregator that has.
         *
         * If type \a T is partially ordered but not totally ordered, then if
         * \a x and \a y hold incomparable maximum values, the aggregators
         * themselves will be considered incomparable also (i.e., this
         * comparison will return `std::partial_ordering::unordered`).
         *
         * \param rhs the aggregator to compare with this.
         * \return the result of the comparison between this and the given
         * aggregator.
         */
        OrderType operator <=> (const MaxAggregator& rhs) const {
            if (count_ == 0) {
                return rhs.count_ == 0 ? OrderType::equivalent :
                    OrderType::less;
            } else if (rhs.count_ == 0) {
                return OrderType::greater;
            } else {
                // Both aggregators have seen at least one atomic value.
                if (auto c = max_ <=> rhs.max_; c != 0)
                    return c;
                return count_ <=> rhs.count_;
            }
        }

        /**
         * Re-initialises this aggregator as having encountered no atomic
         * values at all.
         */
        constexpr void reset() {
            count_ = 0;
        }

        /**
         * Re-initialises this aggregator as having encountered only the given
         * atomic value, exactly once.
         *
         * This is equivalent to calling `reset()` and then `aggregate(value)`.
         *
         * \param value any single atomic value.
         */
        constexpr void reset(const T& value) {
            max_ = value;
            count_ = 1;
        }

        /**
         * Aggregates the given atomic value into the overall results.
         *
         * \exception NoSolution Type \a T is only partially ordered,
         * not totally ordered, and the given value is incomparable with the
         * current maximum.
         *
         * \param value any single atomic value.
         */
        constexpr void aggregate(const T& value) {
            if (count_ == 0) {
                max_ = value;
                count_ = 1;
            } else {
                auto cmp = max_ <=> value;
                if constexpr (std::is_same_v<OrderType, std::partial_ordering>)
                    if (cmp == OrderType::unordered)
                        throw NoSolution();
                if (cmp == OrderType::less) {
                    max_ = value;
                    count_ = 1;
                } else if (cmp == OrderType::equivalent) {
                    ++count_;
                }
            }
        }

        /**
         * Aggregates the results of some other aggregator into the results of
         * this aggregator.
         *
         * This is equivalent to aggregating every individual atomic value
         * that has previously been encountered by \a other.
         *
         * \exception NoSolution Type \a T is only partially ordered,
         * not totally ordered, and the maximum values held by this and the
         * given aggregator are incomparable.
         *
         * \param other the aggregator whose results should be incorporated
         * into this.
         */
        constexpr void aggregate(const MaxAggregator& other) {
            if (other.count_ == 0) {
                return;
            } else if (count_ == 0) {
                *this = other;
            } else {
                auto cmp = max_ <=> other.max_;
                if constexpr (std::is_same_v<OrderType, std::partial_ordering>)
                    if (cmp == OrderType::unordered)
                        throw NoSolution();
                if (cmp == OrderType::less)
                    *this = other;
                else if (cmp == OrderType::equivalent)
                    count_ += other.count_;
            }
        }
};

/**
 * Swaps the contents of the given aggregators.
 *
 * This global routine simply calls MaxAggregator<T>::swap(); it is provided
 * so that MaxAggregator<T> meets the C++ Swappable requirements.
 *
 * This operation is not marked `noexcept`, since its throwing behaviour
 * depends upon the throwing behaviour of the swap operation for type \a T.
 *
 * \nopython
 *
 * \param a the first aggregator whose contents should be swapped.
 * \param b the second aggregator whose contents should be swapped.
 *
 * \ingroup utilities
 */
template <typename T>
requires std::swappable<T>
constexpr void swap(MaxAggregator<T>& a, MaxAggregator<T>& b) {
    a.swap(b);
}

} // namespace regina

#endif

