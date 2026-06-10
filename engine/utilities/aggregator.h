
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

#ifndef __REGINA_AGGREGATOR_H
#ifndef __DOXYGEN
#define __REGINA_AGGREGATOR_H
#endif

#include "utilities/exception.h"
#include <compare>
#include <concepts>
#include <optional>
#include <set>
#include <type_traits>
#include <utility> // for std::swap

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

/**
 * A structure that aggregates many values of type \a Value into a single value
 * of some related type (which is often \a Value itself).
 *
 * Some notes on the expected API:
 *
 * - the operator `+=` can be used to incorporate another value into the
 *   aggregator, or to combine the results of two aggregators;
 *
 * - the result of the aggregation can be extracted by calling `result()`,
 *   whose return value must be convertible to the value type `T::Result`;
 *
 * - it should be possible to call `result()`, then incorporate additional
 *   values, and then call `result()` again (with the exception that you may
 *   also implement a move variant of `result()`, which would as usual
 *   invalidate the aggregator and forbid any further operations);
 *
 * - the function `empty()` should identify whether no values have yet been
 *   incorporated;
 *
 * - the function `reset()` can be used to reset the state of the aggregator
 *   as though no values have been incoprorated, or just a single given value
 *   has been incorporated.
 *
 * Regarding errors:
 *
 * - The return value from `result()` should be able to indicate when no values
 *   have yet been incorporated.  You should document in your aggregator class
 *   how it does this (e.g., see MinAggregator, which returns \nullopt in such
 *   scenarios).  Generic code that works with any aggregator type should use
 *   `empty()` to determine whether or not any values have been incorporated.
 *
 * - If you cannot aggregate some combinations of values, then the operator
 *   `+=` may throw a NoSolution exception.  If this could happen, you should
 *   document this in your aggregator class (e.g., see `MinAggregator<double>`,
 *   which cannot combine real numbers with `NaN`).
 *
 * \nopython
 *
 * \ingroup utilities
 */
template <typename T, typename Value>
concept Aggregator =
    std::semiregular<T> &&
    requires (T agg, const T const_agg, const Value v) {
        typename T::Result;
        requires ! std::is_reference_v<typename T::Result>;
        { agg += v } -> std::same_as<T&>;
        { agg += const_agg } -> std::same_as<T&>;
        { const_agg.result() } -> std::convertible_to<typename T::Result>;
        { const_agg.empty() } -> std::same_as<bool>;
        { agg.reset() };
        { agg.reset(v) };
    };

/**
 * An aggregator that computes the minimum of all values seen.
 *
 * Note that we do not require `Value::operator <=>` to be a total order
 * (this allows us to work with floating-point types, for example, where `NaN`
 * is incomparable with any other value).  However, aggregating incomparable
 * values is an error, and may result in an exception being thrown.  It is the
 * programmer's responsibility to ensure that all values that _are_ supplied
 * are totally ordered.
 *
 * The function `result()` does not throw exceptions; instead it returns
 * \nullopt if no values have been supplied.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  Its implementations of these are only as efficient as the
 * move/swap operations for the type \a Value.
 *
 * \nopython
 *
 * \ingroup utilities
 */
template <typename Value>
requires std::copyable<Value> && std::three_way_comparable<Value>
class MinAggregator {
    public:
        using Result = std::optional<Value>;
            /**< A type that holds the minimum value seen so far, or \nullopt
                 if no values have been supplied. */

    private:
        std::optional<Value> result_;
            /**< The minimum value seen so far, or \nullopt if no values have
                 been supplied. */

        using OrderType = decltype(Value() <=> Value());
            /**< The result of a three-way comparison on \a Value.  This would
                 typically be one of the standard C++ ordering types, such as
                 `std::strong_ordering`. */

    public:
        /**
         * Creates a new aggregator that has not yet seen any values at all.
         */
        constexpr MinAggregator() = default;
        /**
         * Creates a new aggregator that holds the same result as the
         * given aggregator.
         */
        constexpr MinAggregator(const MinAggregator&) = default;
        /**
         * Moves the contents of the given aggregator into this new aggregator.
         *
         * This constructor is not marked `noexcept`, since its throwing
         * behaviour depends upon the throwing behaviour of the move
         * constructor for the type \a Value.
         *
         * The aggregator that is passed will no longer be usable.
         */
        constexpr MinAggregator(MinAggregator&&) = default;
        /**
         * Sets this aggregator's result to be the same as the given
         * aggregator's.
         *
         * \return a reference to this aggregator.
         */
        constexpr MinAggregator& operator = (const MinAggregator&) = default;
        /**
         * Moves the contents of the given aggregator into this aggregator.
         *
         * This operator is not marked `noexcept`, since its throwing
         * behaviour depends upon the throwing behaviour of the move assignment
         * operator for the type \a Value.
         *
         * \return a reference to this aggregator.
         */
        constexpr MinAggregator& operator = (MinAggregator&&) = default;

        /**
         * Determines whether this aggregator has seen any values at all.
         *
         * \return \c true if and only if this aggregator has _not_ yet
         * seen any values.
         */
        constexpr bool empty() const {
            return ! result_;
        }

        /**
         * Returns the smallest value that has been incorporated into this
         * aggregator.  Here "smallest" is with respect to the ordering on the
         * type \a Value.
         *
         * If no values have been given at all then this routine will return
         * \nullopt.
         *
         * \return the smallest value seen so far, or \nullopt if no values
         * have been seen at all.
         */
        constexpr const std::optional<Value>& result() const& {
            return result_;
        }

        /**
         * Returns the smallest value that has been incorporated into this
         * aggregator, moving the result out of this aggregator.
         *
         * After calling this move variant of result(), this aggregator will
         * become unusable.
         *
         * See the `const` version of result() for further details.
         *
         * \return the smallest value seen so far, or \nullopt if no values
         * have been seen at all.
         */
        constexpr std::optional<Value>&& result() && {
            return std::move(result_);
        }

        /**
         * Incorporates the given value into this aggregator.
         *
         * \exception NoSolution The type \a Value is only partially ordered,
         * not totally ordered, and the given value is incomparable with the
         * minimum value seen so far.
         *
         * \param value the value to incorporate into this aggregator.
         * \return a reference to this aggregator.
         */
        constexpr MinAggregator& operator += (const Value& value) {
            if constexpr (std::is_same_v<OrderType, std::partial_ordering>) {
                if (! result_)
                    result_ = value;
                else {
                    auto cmp = value <=> *result_;
                    if (cmp == std::partial_ordering::unordered)
                        throw NoSolution();
                    else if (cmp == std::partial_ordering::less)
                        result_ = value;
                }
            } else {
                if ((! result_) || value < *result_)
                    result_ = value;
            }
            return *this;
        }

        /**
         * Incorporates the given value into this aggregator, moving the
         * contents out of the given value if needed.
         *
         * After calling this move variant of `+=`, the object \a value that
         * was passed may become unusable.
         *
         * \exception NoSolution The type \a Value is only partially ordered,
         * not totally ordered, and the given value is incomparable with the
         * minimum value seen so far.
         *
         * \param value the value to incorporate into this aggregator.
         * \return a reference to this aggregator.
         */
        constexpr MinAggregator& operator += (Value&& value) {
            if constexpr (std::is_same_v<OrderType, std::partial_ordering>) {
                if (! result_)
                    result_ = std::move(value);
                else {
                    auto cmp = value <=> *result_;
                    if (cmp == std::partial_ordering::unordered)
                        throw NoSolution();
                    else if (cmp == std::partial_ordering::less)
                        result_ = std::move(value);
                }
            } else {
                if ((! result_) || value < *result_)
                    result_ = std::move(value);
            }
            return *this;
        }

        /**
         * Incorporates the results of some other aggregator into this
         * aggregator.  The effect will be as though every value that had
         * previously been incorporated into the given aggregator had been
         * incoporated into this aggregator also.
         *
         * \exception NoSolution The type \a Value is only partially ordered,
         * not totally ordered, and the minimum values seen so far by this and
         * the given aggregator are incomparable.
         *
         * \param other the aggregator whose results should be incorporated
         * into this aggregator.
         * \return a reference to this aggregator.
         */
        constexpr MinAggregator& operator += (const MinAggregator& other) {
            if (other.result_)
                return (*this) += other.result_;
            else
                return *this; // no additional values to incorporate
        }

        /**
         * Incorporates the results of some other aggregator into this
         * aggregator, moving the result out of the given aggregator if needed.
         * The effect will be as though every value that had previously been
         * incorporated into the given aggregator had been incoporated into
         * this aggregator also.
         *
         * After calling this move variant of `+=`, the aggregator that was
         * passed may become unusable.
         *
         * \exception NoSolution The type \a Value is only partially ordered,
         * not totally ordered, and the minimum values seen so far by this and
         * the given aggregator are incomparable.
         *
         * \param other the aggregator whose results should be incorporated
         * into this aggregator.
         * \return a reference to this aggregator.
         */
        constexpr MinAggregator& operator += (MinAggregator&& other) {
            if (other.result_)
                return (*this) += std::move(other.result_);
            else
                return *this; // no additional values to incorporate
        }

        /**
         * Re-initialises this aggregator as though it has seen no values at
         * all.
         *
         * After calling this function, empty() will return `true` and
         * result() will return \nullopt.
         */
        constexpr void reset() {
            result_.reset();
        }

        /**
         * Re-initialises this aggregator as though it has seen only the given
         * value and no others.
         *
         * After calling this function, empty() will return `false` and
         * result() will return a copy of `value`.
         *
         * \param value the value with which to initialise this aggregator.
         */
        constexpr void reset(const Value& value) {
            result_ = value;
        }

        /**
         * Re-initialises this aggregator as though it has seen only the given
         * value, which will be moved into the stored result.
         *
         * After calling this function: empty() will return `false`,
         * result() will return a copy of `value`, and the object \a value
         * that was passed will become unusable.
         *
         * \param value the value to move into the result of this aggregator.
         */
        constexpr void reset(Value&& value) {
            result_ = std::move(value);
        }

        /**
         * Swaps the results held by this and the given aggregator.
         *
         * This operation is not marked `noexcept`, since its throwing behaviour
         * depends upon the swap operation for the type \a Value.
         *
         * \param other the aggregator whose results are to be swapped with
         * this.
         */
        constexpr void swap(MinAggregator& other)
                requires std::swappable<Value> {
            result_.swap(other.result_);
        }
};

/**
 * An aggregator that computes the set of all distinct values that have been
 * seen.  Essentially, what this aggregator provides is the ability to
 * eliminate duplicate values.
 *
 * Note that we do not require `Value::operator <=>` to be a total order
 * (this allows us to work with floating-point types, for example, where `NaN`
 * is incomparable with any other value).  However, aggregating incomparable
 * values is an error which will not be detected and which will result in
 * undefined behaviour.  It is the programmer's responsibility to ensure that
 * all values that _are_ supplied are totally ordered.
 *
 * The function `result()` does not throw exceptions; instead it returns
 * the empty set if no values have been supplied.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \nopython
 *
 * \ingroup utilities
 */
template <typename Value>
requires std::copyable<Value> && std::three_way_comparable<Value>
class SetAggregator {
    public:
        using Result = std::set<Value>;
            /**< A type that holds a set of distinct values. */

    private:
        std::set<Value> result_;
            /**< The set of all distinct values seen so far. */

    public:
        /**
         * Creates a new aggregator that has not yet seen any values at all.
         */
        SetAggregator() = default;
        /**
         * Creates a new aggregator that holds the same result as the
         * given aggregator.
         */
        SetAggregator(const SetAggregator&) = default;
        /**
         * Moves the contents of the given aggregator into this new aggregator.
         *
         * The aggregator that is passed will no longer be usable.
         */
        SetAggregator(SetAggregator&&) noexcept = default;
        /**
         * Sets this aggregator's result to be the same as the given
         * aggregator's.
         *
         * \return a reference to this aggregator.
         */
        SetAggregator& operator = (const SetAggregator&) = default;
        /**
         * Moves the contents of the given aggregator into this aggregator.
         *
         * \return a reference to this aggregator.
         */
        SetAggregator& operator = (SetAggregator&&) noexcept = default;

        /**
         * Determines whether this aggregator has seen any values at all.
         *
         * \return \c true if and only if this aggregator has _not_ yet
         * seen any values.
         */
        bool empty() const {
            return ! result_.empty();
        }

        /**
         * Returns the set of all distinct values that have been incorporated
         * into this aggregator.  The values will be sorted according to the
         * ordering on the type \a Value.
         *
         * If no values have been given at all then this routine will return
         * the empty set.
         *
         * \return the set of all distinct values seen so far.
         */
        const std::set<Value>& result() const& {
            return result_;
        }

        /**
         * Returns the set of all distinct values that have been incorporated
         * into this aggregator, moving the result out of this aggregator.
         *
         * After calling this move variant of result(), this aggregator will
         * become unusable.
         *
         * See the `const` version of result() for further details.
         *
         * \return the set of all distinct values seen so far.
         */
        std::set<Value>&& result() && {
            return std::move(result_);
        }

        /**
         * Incorporates the given value into this aggregator.
         *
         * \pre If the type \a Value is only partially ordered (not totally
         * ordered), then the given value is comparable with all other values
         * that have already been supplied.
         *
         * \param value the value to incorporate into this aggregator.
         * \return a reference to this aggregator.
         */
        SetAggregator& operator += (const Value& value) {
            result_.insert(value);
            return *this;
        }

        /**
         * Incorporates the given value into this aggregator, moving the
         * contents out of the given value if needed.
         *
         * After calling this move variant of `+=`, the object \a value that
         * was passed may become unusable.
         *
         * \pre If the type \a Value is only partially ordered (not totally
         * ordered), then the given value is comparable with all other values
         * that have already been supplied.
         *
         * \param value the value to incorporate into this aggregator.
         * \return a reference to this aggregator.
         */
        SetAggregator& operator += (Value&& value) {
            result_.insert(std::move(value));
            return *this;
        }

        /**
         * Incorporates the results of some other aggregator into this
         * aggregator.  The effect will be as though every value that had
         * previously been incorporated into the given aggregator had been
         * incoporated into this aggregator also.
         *
         * \pre If the type \a Value is only partially ordered (not totally
         * ordered), then all values seen so far by this and the given
         * aggregator are comparable.
         *
         * \param other the aggregator whose results should be incorporated
         * into this aggregator.
         * \return a reference to this aggregator.
         */
        SetAggregator& operator += (const SetAggregator& other) {
            result_.insert(other.result_.begin(), other.result_.end());
            return *this;
        }

        /**
         * Incorporates the results of some other aggregator into this
         * aggregator, moving the results out of the given aggregator if needed.
         * The effect will be as though every value that had previously been
         * incorporated into the given aggregator had been incoporated into
         * this aggregator also.
         *
         * After calling this move variant of `+=`, the aggregator that was
         * passed may become unusable.
         *
         * \pre If the type \a Value is only partially ordered (not totally
         * ordered), then all values seen so far by this and the given
         * aggregator are comparable.
         *
         * \param other the aggregator whose results should be incorporated
         * into this aggregator.
         * \return a reference to this aggregator.
         */
        SetAggregator& operator += (SetAggregator&& other) {
            result_.merge(std::move(other.result_));
            return *this;
        }

        /**
         * Re-initialises this aggregator as though it has seen no values at
         * all.
         *
         * After calling this function, empty() will return `true` and
         * result() will return the empty set.
         */
        void reset() {
            result_.clear();
        }

        /**
         * Re-initialises this aggregator as though it has seen only the given
         * value and no others.
         *
         * After calling this function, empty() will return `false` and
         * result() will return a set of size one containing only `value`.
         *
         * \param value the value with which to initialise this aggregator.
         */
        void reset(const Value& value) {
            result_.clear();
            result_.insert(value);
        }

        /**
         * Re-initialises this aggregator as though it has seen only the given
         * value, which will be moved into the resulting set.
         *
         * After calling this function: empty() will return `false`,
         * result() will return a set of size one containing only `value`,
         * and the object \a value that was passed will become unusable.
         *
         * \param value the value to move into the resulting set for this
         * aggregator.
         */
        void reset(Value&& value) {
            result_.clear();
            result_.insert(std::move(value));
        }

        /**
         * Swaps the results held by this and the given aggregator.
         *
         * \param other the aggregator whose results are to be swapped with
         * this.
         */
        void swap(SetAggregator& other) {
            result_.swap(other.result_);
        }
};

/**
 * An aggregator that computes the maximum of all values seen, taking
 * multiplicity into account.  Specifically, this aggregator maintains the
 * the maximum value encountered so far, along with the number of values
 * encountered so far that are equivalent to that maximum.
 *
 * Note that we do not require `Value::operator <=>` to be a total order
 * (this allows us to work with floating-point types, for example, where `NaN`
 * is incomparable with any other value).  However, aggregating incomparable
 * values is an error, and may result in an exception being thrown.  It is the
 * programmer's responsibility to ensure that all values that _are_ supplied
 * are totally ordered.
 *
 * The function `result()` does not throw exceptions; instead if no values
 * have been supplied it will return a pair \a p whose associated count
 * (`p.second`) is zero.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  Its implementations of these are only as efficient as the
 * move/swap operations for the type \a Value.
 *
 * \nopython
 *
 * \ingroup utilities
 */
template <typename Value>
requires std::regular<Value> && std::three_way_comparable<Value>
class MaxCountAggregator {
    public:
        using Result = std::pair<Value, size_t>;
            /**< A type that holds the maximum value seen so far, along with
                 the number of values seen so far that are equivalent to it. */

        using OrderType = decltype(Value() <=> Value());
            /**< The result of a three-way comparison on \a Value.  This would
                 typically be one of the standard C++ ordering types, such as
                 `std::strong_ordering`. */

    private:
        std::pair<Value, size_t> result_ { {}, 0 };
            /**< The maximum value encountered so far, and the number of
                 values encountered so far that are equivalent to it.  If
                 `result_.second == 0`, then `result_.first` will be
                 undefined. */

    public:
        /**
         * Creates a new aggregator that has not yet seen any values at all.
         */
        constexpr MaxCountAggregator() = default;
        /**
         * Creates a new aggregator that holds the same results as the
         * given aggregator.
         */
        constexpr MaxCountAggregator(const MaxCountAggregator&) = default;
        /**
         * Moves the contents of the given aggregator into this new aggregator.
         *
         * This constructor is not marked `noexcept`, since its throwing
         * behaviour depends upon the throwing behaviour of the move
         * constructor for type \a Value.
         *
         * The aggregator that is passed will no longer be usable.
         */
        constexpr MaxCountAggregator(MaxCountAggregator&&) = default;
        /**
         * Sets this aggregator's results to be the same as the given
         * aggregator's.
         *
         * \return a reference to this aggregator.
         */
        constexpr MaxCountAggregator& operator = (const MaxCountAggregator&) =
            default;
        /**
         * Moves the contents of the given aggregator into this aggregator.
         *
         * This operator is not marked `noexcept`, since its throwing
         * behaviour depends upon the throwing behaviour of the move assignment
         * operator for the type \a Value.
         *
         * \return a reference to this aggregator.
         */
        constexpr MaxCountAggregator& operator = (MaxCountAggregator&&) =
            default;

        /**
         * Determines whether this aggregator has seen any values at all.
         *
         * \return \c true if and only if this aggregator has _not_ yet
         * seen any values.
         */
        constexpr bool empty() const {
            return result_.second == 0;
        }

        /**
         * Returns the maximum value that has been incorporated into this
         * aggregator, along with a count indicating how many values have been
         * incorporated that are equivalent to that maximum.  Here both
         * "maximum" and "equivalent" are with respect to the ordering on the
         * type \a Value.
         *
         * If no values have been given at all then this routine will return
         * a pair \a p whose associated count (`p.second)` is zero, but whose
         * associated maximum (`p.first`) is undefined.
         *
         * \return the maximum value seen so far and the number of times that
         * it has been seen, as described above.
         */
        constexpr const std::pair<Value, size_t>& result() const& {
            return result_;
        }

        /**
         * Returns the maximum value seen so far along with the associated
         * count, moving the results out of this aggregator.
         *
         * After calling this move variant of result(), this aggregator will
         * become unusable.
         *
         * See the `const` version of result() for further details.
         *
         * \return the maximum value seen so far and the number of times that
         * it has been seen, as described above.
         */
        constexpr std::pair<Value, size_t>&& result() && {
            return std::move(result_);
        }

        /**
         * Incorporates the given value into this aggregator.
         *
         * \exception NoSolution The type \a Value is only partially ordered,
         * not totally ordered, and the given value is incomparable with the
         * maximum value seen so far.
         *
         * \param value the value to incorporate into this aggregator.
         * \return a reference to this aggregator.
         */
        constexpr MaxCountAggregator& operator += (const Value& value) {
            if (result_.second == 0) {
                result_.first = value;
                result_.second = 1;
            } else {
                auto cmp = result_.first <=> value;
                if constexpr (std::is_same_v<OrderType, std::partial_ordering>)
                    if (cmp == OrderType::unordered)
                        throw NoSolution();
                if (cmp == OrderType::less) {
                    result_.first = value;
                    result_.second = 1;
                } else if (cmp == OrderType::equivalent) {
                    ++result_.second;
                }
            }
            return *this;
        }

        /**
         * Incorporates the given value into this aggregator, moving the
         * contents out of the given value if needed.
         *
         * After calling this move variant of `+=`, the object \a value that
         * was passed may become unusable.
         *
         * \exception NoSolution The type \a Value is only partially ordered,
         * not totally ordered, and the given value is incomparable with the
         * maximum value seen so far.
         *
         * \param value the value to incorporate into this aggregator.
         * \return a reference to this aggregator.
         */
        constexpr MaxCountAggregator& operator += (Value&& value) {
            if (result_.second == 0) {
                result_.first = std::move(value);
                result_.second = 1;
            } else {
                auto cmp = result_.first <=> value;
                if constexpr (std::is_same_v<OrderType, std::partial_ordering>)
                    if (cmp == OrderType::unordered)
                        throw NoSolution();
                if (cmp == OrderType::less) {
                    result_.first = std::move(value);
                    result_.second = 1;
                } else if (cmp == OrderType::equivalent) {
                    ++result_.second;
                }
            }
            return *this;
        }

        /**
         * Incorporates the results of some other aggregator into this
         * aggregator.  The effect will be as though every value that had
         * previously been incorporated into the given aggregator had been
         * incoporated into this aggregator also.
         *
         * \exception NoSolution The type \a Value is only partially ordered,
         * not totally ordered, and the maximum values seen so far by this and
         * the given aggregator are incomparable.
         *
         * \param other the aggregator whose results should be incorporated
         * into this aggregator.
         * \return a reference to this aggregator.
         */
        constexpr MaxCountAggregator& operator +=
                (const MaxCountAggregator& other) {
            if (other.result_.second == 0) {
                return *this;
            } else if (result_.second == 0) {
                return *this = other;
            } else {
                auto cmp = result_.first <=> other.result_.first;
                if constexpr (std::is_same_v<OrderType, std::partial_ordering>)
                    if (cmp == OrderType::unordered)
                        throw NoSolution();
                if (cmp == OrderType::less)
                    *this = other;
                else if (cmp == OrderType::equivalent)
                    result_.second += other.result_.second;
                return *this;
            }
        }

        /**
         * Incorporates the results of some other aggregator into this
         * aggregator, moving the result out of the given aggregator if needed.
         * The effect will be as though every value that had previously been
         * incorporated into the given aggregator had been incoporated into
         * this aggregator also.
         *
         * After calling this move variant of `+=`, the aggregator that was
         * passed may become unusable.
         *
         * \exception NoSolution The type \a Value is only partially ordered,
         * not totally ordered, and the maximum values seen so far by this and
         * the given aggregator are incomparable.
         *
         * \param other the aggregator whose results should be incorporated
         * into this aggregator.
         * \return a reference to this aggregator.
         */
        constexpr MaxCountAggregator& operator += (MaxCountAggregator&& other) {
            if (other.result_.second == 0) {
                return *this;
            } else if (result_.second == 0) {
                return *this = std::move(other);
            } else {
                auto cmp = result_.first <=> other.result_.first;
                if constexpr (std::is_same_v<OrderType, std::partial_ordering>)
                    if (cmp == OrderType::unordered)
                        throw NoSolution();
                if (cmp == OrderType::less)
                    *this = std::move(other);
                else if (cmp == OrderType::equivalent)
                    result_.second += other.result_.second;
                return *this;
            }
        }

        /**
         * Re-initialises this aggregator as though it has seen no values at
         * all.
         *
         * After calling this function, empty() will return `true` and
         * `result().second` will be zero.
         */
        constexpr void reset() {
            result_.second = 0;
        }

        /**
         * Re-initialises this aggregator as though it has seen only the given
         * value and no others.
         *
         * After calling this function, empty() will return `false` and
         * result() will return the pair `(value, 1)`.
         *
         * \param value the value with which to initialise this aggregator.
         */
        constexpr void reset(const Value& value) {
            result_.first = value;
            result_.second = 1;
        }

        /**
         * Re-initialises this aggregator as though it has seen only the given
         * value, which will be moved into the stored result.
         *
         * After calling this function: empty() will return `false`,
         * result() will return the pair `(value, 1)`, and the object \a value
         * that was passed will become unusable.
         *
         * \param value the value to move into the result of this aggregator.
         */
        constexpr void reset(Value&& value) {
            result_.first = std::move(value);
            result_.second = 1;
        }

        /**
         * Swaps the results held by this and the given aggregator.
         *
         * This operation is not marked `noexcept`, since its throwing behaviour
         * depends upon the swap operation for the type \a Value.
         *
         * \param other the aggregator whose results are to be swapped with
         * this.
         */
        constexpr void swap(MaxCountAggregator& other) noexcept
                requires std::swappable<Value> {
            result_.swap(other.result_);
        }

        /**
         * Determines whether this and the given aggregator hold equivalent
         * maximum values with the same multiplicity.
         *
         * This equality test is provided because you cannot compare
         * `result() == rhs.result()`, since _that_ comparison would be
         * undefined if both aggregators have not yet encountered any values
         * at all.  That is, comparisons need to be made on the aggregators
         * directly, not their results.
         *
         * Two aggregators that have not encountered any values at all will be
         * considered equal.
         *
         * \param rhs the aggregator to compare with this.
         * \return \c true if and only if this and the given aggregator hold
         * equivalent values with the same multiplicity.
         */
        bool operator == (const MaxCountAggregator& rhs) const {
            if (result_.second == 0)
                return rhs.result_.second == 0; // ignore result_.first
            else
                return result_ == rhs.result_;
        }

        /**
         * Compares the results of this and the given aggregator.
         *
         * This comparison is provided because you cannot compare
         * `result() <=> rhs.result()`, since _that_ comparison would be
         * undefined if one or both aggregators had not yet encountered any
         * values at all.  That is, comparisons need to be made on the
         * aggregators directly, not their results.
         *
         * For aggregators \a x and \a y, `x < y` means that either \a x holds
         * a smaller maximum value than \a y, or they hold equivalent values
         * but \a x has encountered that value fewer times.
         *
         * An aggregator that has not encountered any values at all is
         * considered smaller than any aggregator that has.  Two aggregators
         * that have not encountered any values at all will be considered equal.
         *
         * If the type \a Value is partially ordered but not totally ordered,
         * then if \a x and \a y hold incomparable maximum values, the
         * aggregators themselves will be considered incomparable also (i.e.,
         * this comparison will return `std::partial_ordering::unordered`).
         *
         * \param rhs the aggregator to compare with this.
         * \return the result of the comparison between this and the given
         * aggregator.
         */
        OrderType operator <=> (const MaxCountAggregator& rhs) const {
            if (result_.second == 0) {
                return rhs.result_.second == 0 ? OrderType::equivalent :
                    OrderType::less;
            } else if (rhs.result_.second == 0) {
                return OrderType::greater;
            } else {
                // Both aggregators have seen at least one atomic value.
                return result_ <=> rhs.result_;
            }
        }
};

/**
 * Swaps the contents of the given aggregators.
 *
 * This global routine simply calls `A::swap()`; it is provided so that
 * Regina's aggregator types meet the C++ Swappable requirements.
 *
 * This operation is not marked `noexcept`, since its throwing behaviour
 * depends upon the swap operation for the type \a Value.
 *
 * \nopython
 *
 * \param a the first aggregator whose contents should be swapped.
 * \param b the second aggregator whose contents should be swapped.
 *
 * \ingroup utilities
 */
template <std::swappable Value, Aggregator<Value> A>
constexpr void swap(A& a, A& b) {
    a.swap(b);
}

} // namespace regina

#endif

