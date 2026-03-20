
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

/*! \file utilities/sequence.h
 *  \brief Support for temporary lightweight sequences.
 */

#ifndef __REGINA_SEQUENCE_H
#ifndef __DOXYGEN
#define __REGINA_SEQUENCE_H
#endif

#include <iostream>
#include <concepts> // Don't include this first - see QTBUG-83160
#include "concepts/iterator.h"

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

/**
 * A lightweight class for storing a random-access sequence of objects.
 *
 * This class is intended as a lightweight substitute for std::vector,
 * especially when working with temporary sequences that are frequently
 * created and destroyed, such as sequence-valued keys or values in maps.
 * The underlying storage just uses a native C-style array, and the
 * C++ class wrapper provides the usual mechanisms for safe and simple
 * memory management.
 *
 * The size (number of elements) of a sequence can be changed, but this
 * should not be done lightly.  Unlike std::vector, resizing a sequence
 * is an expensive operation that deletes all existing contents of the
 * sequence and forces a reallocation of the underlying storage.
 * See init() for details.
 *
 * This class is very similar in nature to FixedArray, but was born from
 * different needs.  It is possible that these two classes will be unified in
 * some future version of Regina.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \nopython
 *
 * \ingroup utilities
 */
template <std::semiregular T>
class LightweightSequence {
    public:
        using iterator = T*;
            /**< An iterator type for read-write access to the elements
                of a sequence.  Such a type can be dereferenced
                (yielding a reference to type \a T), and manipulated
                using the usual pointer arithmetic (such as `p++`,
                `--p`, `p += n`, and so on). */
        using const_iterator = const T*;
            /**< An iterator type for read-only access to the elements
                of a sequence.  Such a type can be dereferenced
                (yielding a const reference to type \a T), and manipulated
                using the usual pointer arithmetic (such as `p++`,
                `--p`, `p += n`, and so on). */

    private:
        T* data_;
            /**< The elements of the sequence, stored as a C-style array. */
        size_t size_;
            /**< The number of elements in the sequence. */

    public:
        /**
         * Creates a new empty sequence; that is, a sequence of size zero.
         *
         * This sequence can be resized by calling init().
         */
        LightweightSequence();
        /**
         * Create a new sequence containing the given number of
         * elements.  The elements themselves will be initialised using
         * the default constructor for type \a T.
         *
         * \param size the number of elements in the new sequence.
         */
        explicit LightweightSequence(size_t size);
        /**
         * Create a new sequence containing the given elements.
         *
         * \param elements the elements to place in the new sequence.
         */
        LightweightSequence(std::initializer_list<T> elements);
        /**
         * Create a new sequence containing the given elements, which
         * may be given through a combination of individual elements and
         * iterator pairs.
         *
         * Specifically, this constructor will step through the \a elements
         * arguments in turn:
         *
         * - If the next argument is of type \a T (or can be implicitly
         *   converted to type \a T), then the corresponding value will be
         *   added onto the end of the sequence.
         *
         * - If the next argument is a LightweightSequence iterator, then this
         *   must be followed by _another_ LightweightSequence iterator.
         *   The range defined by this iterator pair will be copied onto
         *   the end of the sequence.
         *
         * The routine requires you to pass the entire length of the sequence
         * in advance; this is so the necessary memory can be pre-allocated
         * without requiring any unnecessary arithmetic.
         *
         * \pre The argument \a size is precisely the sum of the lengths of all
         * iterator ranges plus the number of standalone elements in the
         * argument list \a elements.
         *
         * \param size the number of elements in the new sequence.
         * \param elements the elements and/or iterator ranges with which to
         * fill the sequence, as described above.
         */
        template <typename... Args>
        requires ((std::convertible_to<Args, T> || std::convertible_to<Args,
            typename LightweightSequence<T>::const_iterator>) && ...)
        explicit LightweightSequence(size_t size, Args&&... elements);
        /**
         * Create a copy of the given sequence.
         *
         * This induces a deep copy of \a src, in that all of the elements of
         * \a src will be copied into the new sequence.
         *
         * \param src the sequence to copy.
         */
        LightweightSequence(const LightweightSequence& src);
        /**
         * Moves the contents of the given sequence to this new sequence.
         * This is a fast (constant time) operation.
         *
         * The sequence that was passed (\a src) will no longer be usable.
         *
         * \param src the sequence to move.
         */
        LightweightSequence(LightweightSequence&& src) noexcept;
        /**
         * Destroys this sequence and all of its elements.
         *
         * All elements of the sequence will be destroyed using the
         * destructor for type \a T.  If the elements are pointers whose
         * pointee objects need to be deleted also, you must do this
         * separately before destroying the sequence itself.
         */
        ~LightweightSequence();

        /**
         * Resizes this sequence to contain the given number of elements.
         *
         * All existing elements in this sequence will be destroyed, using the
         * default destructor for type \a T.  If the elements are pointers
         * whose pointee objects need to be deleted also, you must do this
         * separately before calling init().
         *
         * The elements of the sequence after this routine is called
         * will be initialised using the default constructor for type \a T.
         *
         * \warning Calling init() is an expensive operation, in that it will
         * always force a reallocation of the underlying storage (even if the
         * new size is smaller than the old).
         *
         * \param size the number of elements that the sequence will
         * contain after this routine is called.
         */
        void init(size_t size = 0);

        /**
         * Returns the number of elements in this sequence.  This can be
         * changed (in a destructive way) by calling init().
         */
        size_t size() const;

        /**
         * Determines whether this sequence is empty.
         *
         * This is true if and only if `size() == 0`.
         *
         * \return \c true if and only if this sequence is empty.
         */
        bool empty() const;

        /**
         * Returns a copy of the element at the given index in the sequence.
         *
         * \param pos the index of the requested element; this must be
         * between 0 and size()-1 inclusive.
         * \return a copy of the requested element.
         */
        T operator [] (size_t pos) const;
        /**
         * Returns a reference to the element at the given index in the
         * sequence.
         *
         * \param pos the index of the requested element; this must be
         * between 0 and size()-1 inclusive.
         * \return a reference to the requested element.
         */
        T& operator [] (size_t pos);

        /**
         * Returns a read-write iterator that points to the first element
         * of the sequence.
         *
         * Note that an iterator is simply a pointer to an element of
         * the sequence, and so by dereferencing an iterator you can
         * change the corresponding element of the sequence directly.
         *
         * \return a read-write begin iterator.
         */
        iterator begin();
        /**
         * Returns a read-only iterator that points to the first element
         * of the sequence.
         *
         * Note that a const_iterator is simply a const pointer to an element
         * of the sequence, and so by dereferencing a const_iterator you can
         * access (but not change) the corresponding element of the sequence.
         *
         * \return a read-only begin iterator.
         */
        const_iterator begin() const;
        /**
         * Returns a read-write iterator that points beyond the last element
         * of the sequence.
         *
         * Note that, because this iterator is past-the-end, it must not be
         * dereferenced.
         *
         * \return a read-write past-the-end iterator.
         */
        iterator end();
        /**
         * Returns a read-only iterator that points beyond the last element
         * of the sequence.
         *
         * Note that, because this iterator is past-the-end, it must not be
         * dereferenced.
         *
         * \return a read-only past-the-end iterator.
         */
        const_iterator end() const;
        /**
         * Returns a copy of the first element in this sequence.
         *
         * \pre This sequence is non-empty.
         *
         * \return a copy of the first element.
         */
        T front() const;
        /**
         * Returns a reference to the first element in this sequence.
         *
         * \pre This sequence is non-empty.
         *
         * \return a reference to the first element.
         */
        T& front();
        /**
         * Returns a copy of the last element in this sequence.
         *
         * \pre This sequence is non-empty.
         *
         * \return a copy of the last element.
         */
        T back() const;
        /**
         * Returns a reference to the last element in this sequence.
         *
         * \pre This sequence is non-empty.
         *
         * \return a reference to the last element.
         */
        T& back();

        /**
         * Converts this into a copy of the given sequence.
         * Any existing elements of this sequence will be deleted.
         *
         * This induces a deep copy of \a src, in that all of the elements of
         * \a src will be copied into the new sequence.
         *
         * \param src the sequence to copy.
         * \return a reference to this sequence.
         */
        LightweightSequence<T>& operator = (const LightweightSequence& src);
        /**
         * Moves the contents of the given sequence to this sequence.
         * This is a fast (constant time) operation.
         *
         * The sequence that was passed (\a src) will no longer be usable.
         *
         * \param src the sequence to move.
         * \return a reference to this sequence.
         */
        LightweightSequence<T>& operator = (LightweightSequence&& src) noexcept;

        /**
         * Swaps the contents of this and the given sequence.
         *
         * \param other the sequence whose contents are to be swapped with this.
         */
        void swap(LightweightSequence<T>& other) noexcept;

        /**
         * Tests whether this and the given sequence are identical.
         *
         * The sequences need not be the same size, though if the sizes
         * are different then this routine will return \c false immediately.
         *
         * \param rhs the sequence to compare with this.
         * \return \c true if and only if this and the given sequence
         * are identical.
         */
        bool operator == (const LightweightSequence& rhs) const;

        /**
         * Compares two sequences lexicographically.
         * The sequences need not be the same size.
         *
         * This generates all of the usual comparison operators, including
         * `<`, `<=`, `>`, and `>=`.
         *
         * \param rhs the sequence to compare this with.
         * \return The result of the lexicographical comparison between this
         * and the given sequence.  This will be of the strongest possible
         * comparison category type for comparing objects of type \a T
         * (so, for example, the return type will be `std::strong_ordering`
         * when working with sequences of integers).
         */
        auto operator <=> (const LightweightSequence& rhs) const;

    private:
        /**
         * Implementation details for the iterators-and-values constructor.
         */
        template <typename... Args>
        requires ((std::convertible_to<Args, T> || std::convertible_to<Args,
            typename LightweightSequence<T>::const_iterator>) && ...)
        void fill(T* dest, const T* from, const T* to, Args&&... args) {
            fill(std::copy(from, to, dest), std::forward<Args>(args)...);
        }

        /**
         * Implementation details for the iterators-and-values constructor.
         */
        template <typename... Args>
        requires ((std::convertible_to<Args, T> || std::convertible_to<Args,
            typename LightweightSequence<T>::const_iterator>) && ...)
        void fill(T* dest, T elt, Args&&... args) {
            *dest = std::move(elt);
            fill(dest + 1, std::forward<Args>(args)...);
        }

        /**
         * Implementation details for the iterators-and-values constructor.
         */
        void fill(T* /* dest */) {
        }
};

/**
 * Writes the given sequence to the given output stream.  No newline
 * will be written.
 *
 * The sequence will be written in the form `(a, b, c, ...)`.
 *
 * \pre An object \a x of type \a T can be written to an output stream
 * using the syntax `out << x`.
 *
 * \param out the output stream to which to write.
 * \param s the sequence to write.
 * \return a reference to the given output stream.
 *
 * \ingroup utilities
 */
template <std::semiregular T>
std::ostream& operator << (std::ostream& out, const LightweightSequence<T>& s);

/**
 * Swaps the contents of the given sequences.
 *
 * This global routine simply calls LightweightSequence<T>::swap(); it is
 * provided so that LightweightSequence<T> meets the C++ Swappable requirements.
 *
 * \param a the first sequence whose contents should be swapped.
 * \param b the second sequence whose contents should be swapped.
 *
 * \ingroup utilities
 */
template <std::semiregular T>
void swap(LightweightSequence<T>& a, LightweightSequence<T>& b) noexcept;

// Inline functions:

template <std::semiregular T>
inline LightweightSequence<T>::LightweightSequence() :
        data_(nullptr), size_(0) {
}

template <std::semiregular T>
inline LightweightSequence<T>::LightweightSequence(size_t size) :
        data_(new T[size]), size_(size) {
}

template <std::semiregular T>
inline LightweightSequence<T>::LightweightSequence(
        std::initializer_list<T> elements) :
        data_(new T[elements.size()]), size_(elements.size()) {
    std::copy(elements.begin(), elements.end(), data_);
}

template <std::semiregular T>
template <typename... Args>
requires ((std::convertible_to<Args, T> || std::convertible_to<Args,
    typename LightweightSequence<T>::const_iterator>) && ...)
inline LightweightSequence<T>::LightweightSequence(size_t size,
        Args&&... args) : data_(new T[size]), size_(size) {
    fill(data_, std::forward<Args>(args)...);
}

template <std::semiregular T>
inline LightweightSequence<T>::LightweightSequence(
        const LightweightSequence& src) :
        data_(new T[src.size_]), size_(src.size_) {
    std::copy(src.data_, src.data_ + size_, data_);
}

template <std::semiregular T>
inline LightweightSequence<T>::LightweightSequence(
        LightweightSequence&& src) noexcept:
        data_(src.data_), size_(src.size_) {
    src.data_ = nullptr;
}

template <std::semiregular T>
inline LightweightSequence<T>::~LightweightSequence() {
    delete[] data_;
}

template <std::semiregular T>
inline void LightweightSequence<T>::init(size_t size) {
    delete[] data_;
    data_ = (size ? new T[size] : nullptr);
    size_ = size;
}

template <std::semiregular T>
inline size_t LightweightSequence<T>::size() const {
    return size_;
}

template <std::semiregular T>
inline bool LightweightSequence<T>::empty() const {
    return size_ == 0;
}

template <std::semiregular T>
inline T LightweightSequence<T>::operator [] (size_t pos) const {
    return data_[pos];
}

template <std::semiregular T>
inline T& LightweightSequence<T>::operator [] (size_t pos) {
    return data_[pos];
}

template <std::semiregular T>
inline T LightweightSequence<T>::front() const {
    return *data_;
}

template <std::semiregular T>
inline T& LightweightSequence<T>::front() {
    return *data_;
}

template <std::semiregular T>
inline T LightweightSequence<T>::back() const {
    return data_[size_ - 1];
}

template <std::semiregular T>
inline T& LightweightSequence<T>::back() {
    return data_[size_ - 1];
}

template <std::semiregular T>
inline typename LightweightSequence<T>::iterator
        LightweightSequence<T>::begin() {
    return data_;
}

template <std::semiregular T>
inline typename LightweightSequence<T>::const_iterator
        LightweightSequence<T>::begin() const {
    return data_;
}

template <std::semiregular T>
inline typename LightweightSequence<T>::iterator
        LightweightSequence<T>::end() {
    return data_ + size_;
}

template <std::semiregular T>
inline typename LightweightSequence<T>::const_iterator
        LightweightSequence<T>::end() const {
    return data_ + size_;
}

template <std::semiregular T>
inline LightweightSequence<T>& LightweightSequence<T>::operator = (
        const LightweightSequence& src) {
    if (std::addressof(src) == this)
        return *this;

    size_ = src.size_;

    delete[] data_;
    data_ = new T[size_];
    std::copy(src.data_, src.data_ + size_, data_);

    return *this;
}

template <std::semiregular T>
inline LightweightSequence<T>& LightweightSequence<T>::operator = (
        LightweightSequence&& src) noexcept {
    size_ = src.size_;
    std::swap(data_, src.data_);
    // Let src dispose of the original contents in its own destructor.
    return *this;
}

template <std::semiregular T>
inline void LightweightSequence<T>::swap(LightweightSequence<T>& other)
        noexcept {
    std::swap(size_, other.size_);
    std::swap(data_, other.data_);
}

template <std::semiregular T>
inline bool LightweightSequence<T>::operator == (
        const LightweightSequence& rhs) const {
    if (size_ != rhs.size_)
        return false;
    for (size_t i = 0; i < size_; ++i)
        if (data_[i] != rhs.data_[i])
            return false;
    return true;
}

template <std::semiregular T>
inline auto LightweightSequence<T>::operator <=> (
        const LightweightSequence& rhs) const {
#if defined(LEXCMP_FOUND)
    return std::lexicographical_compare_three_way(
        data_, data_ + size_, rhs.data_, rhs.data_ + rhs.size_);
#else
    auto i = data_;
    auto j = rhs.data_;
    for ( ; i != data_ + size_ && j != rhs.data_ + rhs.size_; ++i, ++j)
        if (auto c = (*i <=> *j); c != 0)
            return c;
    if (i != data_ + size_)
        return std::strong_ordering::greater; // LHS is longer
    if (j != rhs.data_ + rhs.size_)
        return std::strong_ordering::less; // RHS is longer
    return std::strong_ordering::equal; // sequences are identical
#endif
}

template <std::semiregular T>
inline std::ostream& operator << (std::ostream& out,
        const LightweightSequence<T>& s) {
    out << '(';
    for (size_t i = 0; i < s.size(); ++i) {
        if (i > 0)
            out << ", ";
        out << s[i];
    }
    return out << ')';
}

template <std::semiregular T>
inline void swap(LightweightSequence<T>& a, LightweightSequence<T>& b)
        noexcept {
    a.swap(b);
}

} // namespace regina

#endif

