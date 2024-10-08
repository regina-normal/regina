
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

/*! \file utilities/fixedarray.h
 *  \brief Provides a fixed-size array whose size is determined at runtime.
 */

#ifndef __REGINA_FIXEDARRAY_H
#ifndef __DOXYGEN
#define __REGINA_FIXEDARRAY_H
#endif

#include <algorithm>
#include "regina-core.h"

namespace regina {

/**
 * A fixed-size array whose size can be computed at runtime.
 *
 * This is essentially a wrapper around `new[]` and `delete[]` that allows the
 * array object to live on the stack, avoiding the need to worry about memory
 * management (particularly in the presence of exceptions).
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \nopython
 *
 * \ingroup utilities
 */
template <typename T>
class FixedArray {
    private:
        T* data_; /**< The elements of this array. */
        size_t size_; /**< The number of elements in this array. */

    public:
        /**
         * A reference to an individual array element.
         */
        using reference = T&;
        /**
         * A constant reference to an individual array element.
         */
        using const_reference = T const&;
        /**
         * A random-access iterator over this array.
         */
        using iterator = T*;
        /**
         * A constant random-access iterator over this array.
         */
        using const_iterator = T const*;
        /**
         * The type of an individual element in this array.
         */
        using value_type = T;
        /**
         * The type used for the number of elements in this array.
         */
        using size_type = size_t;
        /**
         * The type used for a signed distance between two iterators.
         */
        using difference_type = ptrdiff_t;

        /**
         * Constructs a new array of the given size.
         *
         * \pre The type \a T has a default constructor.
         *
         * Every element will be created using the default constructor for \a T.
         *
         * \param size the number of elements in the new array.
         */
        FixedArray(size_t size) : data_(new T[size]), size_(size) {
        }

        /**
         * Constructs a new array of the given size, and initialises every
         * element to the given value.
         *
         * \pre The type \a T has a copy constructor.
         *
         * \param size the number of elements in the new array.
         * \param value the value to assign to every element of the new array.
         */
        FixedArray(size_t size, const T& value) :
                data_(new T[size]), size_(size) {
            std::fill(data_, data_ + size_, value);
        }

        /**
         * Makes a new deep copy of the given array.
         *
         * \pre The type \a T has a copy assignment operator.
         *
         * \param src the array whose contents should be copied.
         */
        FixedArray(const FixedArray& src) :
                data_(new T[src.size_]), size_(src.size_) {
            std::copy(src.begin(), src.end(), data_);
        }

        /**
         * Moves the contents of the given array into this new array.
         *
         * The array \a src that was passed will no longer be usable.
         *
         * \param src the array whose contents should be moved.
         */
        FixedArray(FixedArray&& src) noexcept :
                data_(src.data_), size_(src.size_) {
            src.data_ = nullptr;
        }

        /**
         * Destroys this array.
         */
        ~FixedArray() {
            delete[] data_;
        }

        /**
         * Returns a read-write reference to the array element at the given
         * index.
         *
         * \param index the index of the element to access. This must be
         * between 0 and `size()-1` inclusive.
         *
         * \return a reference to the requested array element.
         */
        T& operator [] (size_t index) {
            return data_[index];
        }

        /**
         * Returns a read-only reference to the array element at the given
         * index.
         *
         * \param index the index of the element to access. This must be
         * between 0 and `size()-1` inclusive.
         *
         * \return a reference to the requested array element.
         */
        const T& operator [] (size_t index) const {
            return data_[index];
        }

        /**
         * Returns the number of elements in this array.
         *
         * \return the array size.
         */
        size_t size() const {
            return size_;
        }

        /**
         * Determines whether this array is empty.
         *
         * \return \c true if and only if this array is empty.
         */
        bool empty() const {
            return size_ == 0;
        }

        /**
         * Returns a read-write random-access iterator pointing to the
         * first element of this array.
         *
         * \return a read-write begin iterator.
         */
        iterator begin() {
            return data_;
        }

        /**
         * Returns a read-only random-access iterator pointing to the
         * first element of this array.
         *
         * \return a read-only begin iterator.
         */
        const_iterator begin() const {
            return data_;
        }

        /**
         * Returns a read-write random-access iterator pointing beyond the
         * last element of this array.
         *
         * Note that, because this iterator is past-the-end, it must not be
         * dereferenced.
         *
         * \return a read-write end iterator.
         */
        iterator end() {
            return data_ + size_;
        }

        /**
         * Returns a read-only random-access iterator pointing beyond the
         * last element of this array.
         *
         * Note that, because this iterator is past-the-end, it must not be
         * dereferenced.
         *
         * \return a read-only end iterator.
         */
        const_iterator end() const {
            return data_ + size_;
        }

        /**
         * Returns a read-only random-access iterator pointing to the
         * first element of this array.
         *
         * \return a read-only begin iterator.
         */
        const_iterator cbegin() const {
            return data_;
        }

        /**
         * Returns a read-only random-access iterator pointing beyond the
         * last element of this array.
         *
         * Note that, because this iterator is past-the-end, it must not be
         * dereferenced.
         *
         * \return a read-only end iterator.
         */
        const_iterator cend() const {
            return data_ + size_;
        }

        /**
         * Moves the contents of the given array into this array.
         *
         * It does not matter whether this and the given array are the same
         * size; this array will be resized as required (and this is a cheap
         * operation).
         *
         * The array that was passed will no longer be usable.
         *
         * \return a reference to this array.
         */
        FixedArray& operator = (FixedArray&& src) noexcept {
            // Let src destroy our original data in its own destructor.
            std::swap(data_, src.data_);
            size_ = src.size_;
            return *this;
        }

        /**
         * Swaps the contents of this and the given array.
         *
         * \param other the array whose contents are to be swapped with this.
         */
        inline void swap(FixedArray<T>& other) noexcept {
            std::swap(data_, other.data_);
            std::swap(size_, other.size_);
        }

        // Disable copy assignment, since we do not want to worry about
        // reallocation if the two arrays are of different sizes.
        FixedArray& operator = (const FixedArray&) = delete;

        /**
         * Determines whether this and the given array are identical.
         *
         * This routine compares the elements of both arrays in order using
         * the standard equality operator for the element type \a T.
         *
         * It is fine if this array and \a rhs have different sizes (in which
         * case this comparison will immediately return \c false).
         *
         * \return \c true if and only if this and the given array are
         * identical.
         */
        inline bool operator == (const FixedArray& rhs) const {
            return size_ == rhs.size_ &&
                std::equal(data_, data_ + size_, rhs.data_);
        }
};

/**
 * Swaps the contents of the given arrays.
 *
 * This global routine simply calls FixedArray<T>::swap(); it is provided
 * so that FixedArray<T> meets the C++ Swappable requirements.
 *
 * \nopython
 *
 * \param a the first array whose contents should be swapped.
 * \param b the second array whose contents should be swapped.
 *
 * \ingroup utilities
 */
template <typename T>
void swap(FixedArray<T>& a, FixedArray<T>& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif
