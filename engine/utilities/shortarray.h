
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 2021-2022, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/*! \file utilities/shortarray.h
 *  \brief Provides short stack-based arrays of bounded size.
 */

#ifndef __REGINA_SHORTARRAY_H
#ifndef __DOXYGEN
#define __REGINA_SHORTARRAY_H
#endif

#include <vector>
#include "regina-core.h"

namespace regina {

/**
 * A short stack-based array of bounded size.
 *
 * ShortArray represents a stack-based array whose size is bounded above by
 * the compile-time constant \a maxSize, but whose size at runtime can vary
 * between 0 and \a maxSize inclusive.
 *
 * Here "stack-based" means that the array does not use dynamic memory
 * allocation; instead it reserves space for \a maxSize elements directly
 * on the stack.  In this sense, it is analogous to std::array<T, maxSize>
 * or indeed a plain C-style array T[maxSize].  Like these other types,
 * it is fast to access with very little space or time overhead, but it
 * cannot be moved or swapped in constant time.
 *
 * Where ShortArray differs from these other types is that its size can
 * vary at runtime.  Its default constructor initialises it to size zero, and
 * it supports push_back() and pop_back() operations and a size() query which
 * are all very fast.  The size is, however, limited to \a maxSize, and
 * any attempt to push additional elements beyond this limit will result
 * in undefined behaviour.
 *
 * This class was designed with very small arrays in mind; an example is
 * the list of embeddings for a (<i>dim</i>-1)-dimensional face in a
 * <i>dim</i>-dimensional triangulationw, which always has size 1 or 2.
 *
 * This class does not implement move constructors or move assignment,
 * since this cannot be done any faster than a linear-time copy operation.
 * For the same reason, it does not implement its own custom swap functions.
 *
 * \nopython
 *
 * \ingroup utilities
 */
template <typename T, size_t maxSize>
class ShortArray {
    public:
        /**
         * The type of object stored in the array.
         */
        using value_type = T;
        /**
         * The type used for indexing into this array.
         */
        using size_type = size_t;
        /**
         * A non-const reference to an array element.
         */
        using reference = T&;
        /**
         * A const reference to an array element.
         */
        using const_reference = T const&;
        /**
         * An iterator type that provides non-const access to the array
         * elements.
         */
        using iterator = T*;
        /**
         * An iterator type that provides const access to the array elements.
         */
        using const_iterator = const T*;

    private:
        T data_[maxSize]; /**< The elements of the array. */
        size_t size_ = 0; /**< The number of elements in the array. */

    public:
        /**
         * Constructs a new empty array.
         */
        constexpr ShortArray() = default;

        /**
         * Constructs a new copy of the given array.
         */
        constexpr ShortArray(const ShortArray&) = default;

        /**
         * Sets this to be a copy of the given array.
         *
         * @return a reference to this array.
         */
        ShortArray& operator = (const ShortArray&) = default;

        /**
         * Determines if this array is currently empty.
         *
         * @return \c true if and only if the array contains no elements.
         */
        constexpr bool empty() const {
            return (size_ == 0);
        }

        /**
         * Returns the number of elements currently in this array.
         *
         * @return the array size.
         */
        constexpr size_t size() const {
            return size_;
        }

        /**
         * Returns the maximum number of elements that can be held by
         * this array.
         *
         * @return the maximum allowable number of elements.
         */
        constexpr size_t max_size() const {
            return maxSize;
        }

        /**
         * Gives write access to the array element at the given index.
         *
         * \param index the index of the element to access; this must be
         * between 0 and size()-1 inclusive.
         * @return a reference to the array element at the given index.
         */
        T& operator [] (size_t index) {
            return data_[index];
        }

        /**
         * Gives read-only access to the array element at the given index.
         *
         * \param index the index of the element to access; this must be
         * between 0 and size()-1 inclusive.
         * @return a const reference to the array element at the given index.
         */
        constexpr const T& operator [] (size_t index) const {
            return data_[index];
        }

        /**
         * Returns the first element in the array.
         *
         * \pre The array is non-empty.
         *
         * @return a const reference to the first element.
         */
        constexpr const T& front() const {
            return *data_;
        }

        /**
         * Returns the last element in the array.
         *
         * \pre The array is non-empty.
         *
         * @return a const reference to the last element.
         */
        constexpr const T& back() const {
            return data_[size_ - 1];
        }

        /**
         * Returns a read-write iterator pointing to the beginning of this
         * array.
         *
         * @return an iterator pointing to the first element.
         */
        iterator begin() {
            return data_;
        }

        /**
         * Returns a read-only iterator pointing to the beginning of this
         * array.
         *
         * @return an iterator pointing to the first element.
         */
        const_iterator begin() const {
            return data_;
        }

        /**
         * Returns a read-write iterator pointing past the end of this array.
         *
         * @return an iterator after the last element.
         */
        iterator end() {
            return data_ + size_;
        }

        /**
         * Returns a read-only iterator pointing past the end of this array.
         *
         * @return an iterator after the last element.
         */
        const_iterator end() const {
            return data_ + size_;
        }

        /**
         * Pushes a copy of the given item onto the end of this array.
         *
         * \pre The array has size strictly less than \a maxSize.
         *
         * \param item the item to add to this array.
         */
        void push_back(const T& item) {
            data_[size_++] = item;
        }

        /**
         * Moves the given item onto the end of this array.
         *
         * \pre The array has size strictly less than \a maxSize.
         *
         * \param item the item to add to this array.
         */
        void push_back(T&& item) {
            data_[size_++] = std::move(item);
        }

        /**
         * Removes the last item from this array.
         *
         * \pre The array is non-empty.
         */
        void pop_back() {
            --size_;
        }

        /**
         * Removes all elements from this array.
         */
        void clear() {
            size_ = 0;
        }
};

} // namespace regina

#endif
