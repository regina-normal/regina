
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

/*! \file utilities/bytesequence.h
 *  \brief Provides string-like sequences of raw bytes.
 */

#ifndef __REGINA_BYTESEQUENCE_H
#ifndef __DOXYGEN
#define __REGINA_BYTESEQUENCE_H
#endif

#include <cstdint>
#include <string>
#include "regina-core.h"

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

/**
 * A raw sequence of bytes.
 *
 * Such sequences are (for example) used in Regina for low-memory encodings of
 * isomorphism signatures and knot/link signatures.
 *
 * At present, the public interface for ByteSequence is very limited: it only
 * offers what Regina needs for its own functionality.  This interface may be
 * expanded in future versions of Regina.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \nopython Wherever a ByteSequence is pass into or returned from a function
 * in C++, the corresponding Python function will use a Python `bytes` object
 * instead.
 *
 * \ingroup utilities
 */
class ByteSequence {
    public:
        /**
         * A reference to an individual byte.
         */
        using reference = char&;
        /**
         * A constant reference to an individual byte.
         */
        using const_reference = char const&;
        /**
         * A random-access iterator that gives read-write access to the
         * bytes in this sequence.
         */
        using iterator = std::string::iterator;
        /**
         * A random-access iterator that gives read-only access to the
         * bytes in this sequence.
         */
        using const_iterator = std::string::const_iterator;
        /**
         * The type of an individual byte in this sequence.
         */
        using value_type = std::string::value_type;

    private:
        std::string data_;
            /**< The byte sequence being held.  This will typically _not_ be a
                 printable string, and may contain null characters. */

    public:
        /**
         * Constructs a new empty byte sequence.
         */
        ByteSequence() = default;
        /**
         * Makes a new deep copy of the given byte sequence.
         */
        ByteSequence(const ByteSequence&) = default;
        /**
         * Moves the contents of the given byte sequence into this new
         * sequence.  This is a fast (constant time) operation.
         *
         * The sequence that was passed will no longer be usable.
         */
        ByteSequence(ByteSequence&&) noexcept = default;
        /**
         * Moves the contents of the given string into this new byte sequence.
         * This is a fast (constant time) operation.
         *
         * The string will be treated as a sequence of raw bytes (i.e., there
         * is no assumption about printability, or character encodings).
         *
         * After the construction of this byte sequence, the string that was
         * passed will no longer be usable.
         *
         * \param data a string holding the contents of this new byte sequence.
         */
        explicit ByteSequence(std::string&& data) noexcept :
                data_(std::move(data)) {
        }
        /**
         * Sets this to be a deep copy of the given byte sequence.
         *
         * \return a reference to this byte sequence.
         */
        ByteSequence& operator = (const ByteSequence&) = default;
        /**
         * Moves the contents of the given byte sequence into this sequence.
         * This is a fast (constant time) operation.
         *
         * The sequence that was passed will no longer be usable.
         *
         * \return a reference to this byte sequence.
         */
        ByteSequence& operator = (ByteSequence&&) = default;
        /**
         * Loads the contents of the given string into this byte sequence.
         * Any pre-existing contents of this sequence will be disposed of.
         *
         * The string will be treated as a sequence of raw bytes (i.e., there
         * is no assumption about printability, or character encodings).
         *
         * This is a fast (constant time) operation.  After this operation,
         * the string that was passed will no longer be usable.
         *
         * \param data a string holding the data to load.
         */
        void load(std::string&& data) noexcept {
            data_ = std::move(data);
        }
        /**
         * Determines whether this and the given sequence are identical.
         *
         * \return \c true if and only if this and the given sequence are
         * identical.
         */
        bool operator == (const ByteSequence&) const = default;
        /**
         * Compares two byte sequences lexigraphically.
         *
         * This operator generates all of the usual comparison operators,
         * including `<`, `<=`, `>`, and `>=`.
         *
         * \return the result of the lexicographical comparison between this
         * and the given sequence.
         */
        std::strong_ordering operator <=> (const ByteSequence&) const = default;

        /**
         * Determines whether this sequence is empty.
         *
         * \return `true` if and only if this sequence is empty.
         */
        bool empty() const {
            return data_.empty();
        }
        /**
         * Returns the number of bytes in this sequence.
         *
         * \return the length of this sequence.
         */
        size_t size() const {
            return data_.size();
        }
        /**
         * Returns a read-write random-access iterator pointing to the
         * first byte in this sequence.
         *
         * \return a read-write begin iterator.
         */
        iterator begin() {
            return data_.begin();
        }
        /**
         * Returns a read-only random-access iterator pointing to the
         * first byte in this sequence.
         *
         * \return a read-only begin iterator.
         */
        const_iterator begin() const {
            return data_.begin();
        }
        /**
         * Returns a read-write random-access iterator pointing beyond the
         * last byte in this sequence.
         *
         * Note that, because this iterator is past-the-end, it must not be
         * dereferenced.
         *
         * \return a read-write end iterator.
         */
        iterator end() {
            return data_.end();
        }
        /**
         * Returns a read-only random-access iterator pointing beyond the
         * last byte in this sequence.
         *
         * Note that, because this iterator is past-the-end, it must not be
         * dereferenced.
         *
         * \return a read-only end iterator.
         */
        const_iterator end() const {
            return data_.end();
        }

        /**
         * Appends the given byte to the end of this sequence.
         *
         * \param byte the byte to append.
         */
        void push_back(char byte) {
            data_.push_back(byte);
        }
        /**
         * Appends the given sequence to the end of this sequence.
         *
         * \param rhs the sequence to append.
         * \return a reference to this sequence.
         */
        ByteSequence& operator += (const ByteSequence& rhs) {
            data_ += rhs.data_;
            return *this;
        }
        /**
         * Swaps the contents of this and the given byte sequence.
         *
         * \param other the sequence whose contents are to be swapped with this.
         */
        void swap(ByteSequence& other) {
            data_.swap(other.data_);
        }
        /**
         * Increases the capacity of the internal data storage if necessary
         * to allow for the given number of bytes.
         *
         * Internally, this calls `std::string::reserve(capacity)`.
         * The intent is to avoid unnecessary reallocations as the sequence
         * grows, and also to avoid allocating more memory than necessary.
         *
         * \warning Calling this function may invalide iterators as well as
         * references to individual bytes.
         *
         * \param capacity the number of bytes that you anticipate storing.
         */
        void reserve(size_t capacity) {
            data_.reserve(capacity);
        }
        /**
         * Returns this byte sequence in the form of a C++ string.
         *
         * This returns a reference (not a deep copy), since ByteSequence uses
         * `std::string` for its internal storage.
         *
         * \warning The string that is returned will typically not be printable,
         * and may contain null characters.
         *
         * \return this sequence as a C++ string.
         */
        const std::string& asString() const {
            return data_;
        }
};

/**
 * Swaps the contents of the given byte sequences.
 *
 * This global routine simply calls ByteSequence::swap(); it is provided
 * so that ByteSequence meets the C++ Swappable requirements.
 *
 * \nopython
 *
 * \param a the first byte sequence whose contents should be swapped.
 * \param b the second byte sequence whose contents should be swapped.
 *
 * \ingroup utilities
 */
inline void swap(ByteSequence& a, ByteSequence& b) noexcept {
    a.swap(b);
}

/**
 * Writes the bytes of the given sequence to the given output stream in a
 * printable form.
 *
 * Each byte will be written using two hexadecimal digits, with bytes separated
 * by colons.
 *
 * \param out the output stream to which to write.
 * \param bytes the byte sequence to write.
 * \return a reference to the given output stream.
 */
std::ostream& operator << (std::ostream& out, const ByteSequence& bytes);

} // namespace regina

#endif
