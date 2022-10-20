
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

/*! \file utilities/sequence.h
 *  \brief Support for temporary lightweight sequences.
 */

#ifndef __REGINA_SEQUENCE_H
#ifndef __DOXYGEN
#define __REGINA_SEQUENCE_H
#endif

#include <iostream>
#include "regina-core.h"

namespace regina {

/**
 * A lightweight class for storing a random-access sequence of objects.
 *
 * This class is intended as a lightweight substitute for std::vector,
 * especially when working with temporary sequences that are frequently
 * created and destroyed.  The underlying storage just uses a native
 * C-style array, and the C++ class wrapper provides the usual mechanisms
 * for safe and simple memory management.
 *
 * The size (number of elements) of a sequence can be changed, but this
 * should not be done lightly.  Unlike std::vector, resizing a sequence
 * is an expensive operation that deletes all existing contents of the
 * sequence and forces a reallocation of the underlying storage.
 * See init() for details.
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
class LightweightSequence {
    public:
        using iterator = T*;
            /**< An iterator type for read-write access to the elements
                of a sequence.  Such a type can be dereferenced
                (yielding a reference to type \a T), and manipulated
                using the usual pointer arithmetic (such as <tt>p++</tt>,
                <tt>--p</tt>, <tt>p += n</tt>, and so on). */
        using const_iterator = const T*;
            /**< An iterator type for read-only access to the elements
                of a sequence.  Such a type can be dereferenced
                (yielding a const reference to type \a T), and manipulated
                using the usual pointer arithmetic (such as <tt>p++</tt>,
                <tt>--p</tt>, <tt>p += n</tt>, and so on). */

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
         *   must be followed by \e another LightweightSequence iterator.
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
         * Tests whether this sequence is lexicographically smaller than the
         * given sequence.  The sequences need not be the same size.
         *
         * \param rhs the sequence to compare with this.
         * \return \c true if this is strictly lexicographically
         * smaller than \a rhs, or \c false if this is either
         * lexicographically greater than or equal to \a rhs.
         */
        bool operator < (const LightweightSequence& rhs) const;

        /**
         * A binary function object for comparing subsequences, for use in
         * associative containers whose keys are sequences.
         *
         * This is a very specialised comparison object, for use in the
         * following settings:
         *
         * - We are interested in comparing just some, not necessarily all,
         *   of the elements of each sequence.  The indices of the elements
         *   to compare are passed to the constructor of this comparison object.
         *
         * - The actual objects that we compare are not the sequences
         *   themselves, but iterators that point to (key, value) pairs,
         *   whose keys are sequences.
         *
         * More precisely: suppose the indices of the elements to
         * compare are \a i0, \a i1, \a i2, ..., and that we are comparing
         * iterators \a a, \a b.  Then this function object will consider the
         * sequences <tt>s = a->first</tt> and <tt>t = b->first</tt>,
         * and will lexicographically compare their subsequences
         * <tt>s[i0], s[i1], ...</tt> and <tt>t[i0], t[i1], ...</tt>.
         *
         * Note that the indices \a i0, \a i1, ... do not need to be in
         * increasing order.
         *
         * This class is meant to be lightweight: it merely stores a
         * reference to the list of elements to compare, and it is safe
         * and fast to pass around by value.  The cost of this is that
         * the caller \e must ensure that the list of elements to compare
         * (which is a C-style array) has a lifespan at least as long as
         * this object.  This behaviour is new as of Regina 5.96; in past
         * versions of Regina the list of elements was copied on construction.
         *
         * \tparam IndexIterator the iterator type used to store the range
         * of indices that define the subsequences being compared (that is,
         * the range of indices \a i0, \a i1, \a i2, ...).  This would be
         * deducible from the constructor arguments, were it not for a
         * gcc bug (#79501) that prevents us from declaring a deduction guide.
         */
        template <typename IndexIterator>
        class SubsequenceCompareFirst {
            private:
                IndexIterator beginSub_;
                    /**< The beginning of the range of indices of elements to
                         compare in each sequence. */
                IndexIterator endSub_;
                    /**< The beginning of the range of indices of elements to
                         compare in each sequence. */

            public:
                /**
                 * Creates a new function object.
                 *
                 * As explained in the class notes, this object compares
                 * just some, not necessarily all, elements of two
                 * sequences.  The indices of the elements to compare
                 * should be passed to this constructor using an iterator range.
                 * Specifically, the range defined by \a beginSub and \a endSub
                 * should contain the indices \a i0, \a i1, ..., as described
                 * in the class notes.
                 *
                 * \warning This class merely copies the iterators \a beginSub
                 * and \a endSub, and does not take a deep copy of the range
                 * that they define.  The caller of this routine must ensure
                 * that these iterators remain valid for the entire lifespan of
                 * this function object and any function objects that are
                 * copied from it.
                 *
                 * \param beginSub the beginning of the range of indices of
                 * elements to compare from each sequence.
                 * \param endSub the end (i.e., a past-the-end iterator) of
                 * the range of indices of elements to compare from each
                 * sequence.
                 */
                SubsequenceCompareFirst(IndexIterator beginSub,
                    IndexIterator endSub);
                /**
                 * Copies the given function object into this new object.
                 */
                SubsequenceCompareFirst(const SubsequenceCompareFirst&) =
                    default;

                /**
                 * Copies the given function object into this object.
                 *
                 * \return a reference to this function object.
                 */
                SubsequenceCompareFirst& operator = (
                    const SubsequenceCompareFirst&) = default;

                /**
                 * Tests whether the subsequences referred to by the
                 * given pair of iterators are identical.
                 *
                 * See the class notes for details on how each iterator
                 * is converted into a subsequence.
                 *
                 * \param a an iterator indicating the first of the two
                 * subsequences to compare.
                 * \param b an iterator indicating the second of the two
                 * subsequences to compare.
                 * \return \c true if and only if the two subsequences
                 * are identical.
                 */
                template <typename SeqIterator>
                bool equal(SeqIterator a, SeqIterator b) const;
                /**
                 * Lexicographically compares the subsequences referred to
                 * by the given pair of iterators.
                 *
                 * See the class notes for details on how each iterator
                 * is converted into a subsequence.
                 *
                 * This member function is identical to the bracket operator.
                 *
                 * \param a an iterator indicating the first of the two
                 * subsequences to compare.
                 * \param b an iterator indicating the second of the two
                 * subsequences to compare.
                 * \return \c true if and only if the subsequence
                 * indicated by \a a is lexicographically smaller than
                 * the subsequence indicated by \a b.
                 */
                template <typename SeqIterator>
                bool less(SeqIterator a, SeqIterator b) const;
                /**
                 * Lexicographically compares the subsequences referred to
                 * by the given pair of iterators.
                 *
                 * See the class notes for details on how each iterator
                 * is converted into a subsequence.
                 *
                 * This bracket operator is identical to the less()
                 * member function.
                 *
                 * \param a an iterator indicating the first of the two
                 * subsequences to compare.
                 * \param b an iterator indicating the second of the two
                 * subsequences to compare.
                 * \return \c true if and only if the subsequence
                 * indicated by \a a is lexicographically smaller than
                 * the subsequence indicated by \a b.
                 */
                template <typename SeqIterator>
                bool operator () (SeqIterator a, SeqIterator b) const;
        };

#if 0 // gcc bug #79501 incorrectly marks this as an error.
        template <class IndexIterator>
        SubsequenceCompareFirst(IndexIterator, IndexIterator) ->
            SubsequenceCompareFirst<IndexIterator>;
#endif

    private:
        /**
         * Implementation details for the iterators-and-values constructor.
         */
        template <typename... Args>
        void fill(T* dest, const T* from, const T* to, Args&&... args) {
            fill(std::copy(from, to, dest), std::forward<Args>(args)...);
        }

        /**
         * Implementation details for the iterators-and-values constructor.
         */
        template <typename... Args>
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
 * The sequence will be written in the form <tt>(a, b, c, ...)</tt>.
 *
 * \pre An object \a x of type \a T can be written to an output stream
 * using the syntax <tt>out << x</tt>.
 *
 * \param out the output stream to which to write.
 * \param s the sequence to write.
 * \return a reference to the given output stream.
 *
 * \ingroup utilities
 */
template <typename T>
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
template <typename T>
void swap(LightweightSequence<T>& a, LightweightSequence<T>& b) noexcept;

// Inline functions:

template <typename T>
inline LightweightSequence<T>::LightweightSequence() :
        data_(nullptr), size_(0) {
}

template <typename T>
inline LightweightSequence<T>::LightweightSequence(size_t size) :
        data_(new T[size]), size_(size) {
}

template <typename T>
template <typename... Args>
inline LightweightSequence<T>::LightweightSequence(size_t size,
        Args&&... args) : data_(new T[size]), size_(size) {
    fill(data_, std::forward<Args>(args)...);
}

template <typename T>
inline LightweightSequence<T>::LightweightSequence(
        const LightweightSequence& src) :
        data_(new T[src.size_]), size_(src.size_) {
    std::copy(src.data_, src.data_ + size_, data_);
}

template <typename T>
inline LightweightSequence<T>::LightweightSequence(
        LightweightSequence&& src) noexcept:
        data_(src.data_), size_(src.size_) {
    src.data_ = nullptr;
}

template <typename T>
inline LightweightSequence<T>::~LightweightSequence() {
    delete[] data_;
}

template <typename T>
inline void LightweightSequence<T>::init(size_t size) {
    delete[] data_;
    data_ = (size ? new T[size] : nullptr);
    size_ = size;
}

template <typename T>
inline size_t LightweightSequence<T>::size() const {
    return size_;
}

template <typename T>
inline T LightweightSequence<T>::operator [] (size_t pos) const {
    return data_[pos];
}

template <typename T>
inline T& LightweightSequence<T>::operator [] (size_t pos) {
    return data_[pos];
}

template <typename T>
inline typename LightweightSequence<T>::iterator
        LightweightSequence<T>::begin() {
    return data_;
}

template <typename T>
inline typename LightweightSequence<T>::const_iterator
        LightweightSequence<T>::begin() const {
    return data_;
}

template <typename T>
inline typename LightweightSequence<T>::iterator
        LightweightSequence<T>::end() {
    return data_ + size_;
}

template <typename T>
inline typename LightweightSequence<T>::const_iterator
        LightweightSequence<T>::end() const {
    return data_ + size_;
}

template <typename T>
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

template <typename T>
inline LightweightSequence<T>& LightweightSequence<T>::operator = (
        LightweightSequence&& src) noexcept {
    size_ = src.size_;
    std::swap(data_, src.data_);
    // Let src dispose of the original contents in its own destructor.
    return *this;
}

template <typename T>
inline void LightweightSequence<T>::swap(LightweightSequence<T>& other)
        noexcept {
    std::swap(size_, other.size);
    std::swap(data_, other.data_);
}

template <typename T>
inline bool LightweightSequence<T>::operator == (
        const LightweightSequence& rhs) const {
    if (size_ != rhs.size_)
        return false;
    for (size_t i = 0; i < size_; ++i)
        if (data_[i] != rhs.data_[i])
            return false;
    return true;
}

template <typename T>
inline bool LightweightSequence<T>::operator < (
        const LightweightSequence<T>& rhs) const {
    for (size_t i = 0; i < rhs.size_; ++i)
        if (i >= size_ || data_[i] < rhs.data_[i])
            return true;
        else if (rhs.data_[i] < data_[i])
            return false;
    // The sequences match for the first rhs.size_ elements, and
    // this sequence is at least as long as rhs.
    return false;
}

template <typename T>
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

template <typename T>
template <typename IndexIterator>
inline LightweightSequence<T>::SubsequenceCompareFirst<IndexIterator>::
        SubsequenceCompareFirst(IndexIterator beginSub, IndexIterator endSub) :
        beginSub_(beginSub), endSub_(endSub) {
}

template <typename T>
template <typename IndexIterator>
template <typename SeqIterator>
inline bool LightweightSequence<T>::SubsequenceCompareFirst<IndexIterator>::
        equal(SeqIterator a, SeqIterator b) const {
    for (auto it = beginSub_; it != endSub_; ++it)
        if (a->first[*it] != b->first[*it])
            return false;
    return true;
}

template <typename T>
template <typename IndexIterator>
template <typename SeqIterator>
inline bool LightweightSequence<T>::SubsequenceCompareFirst<IndexIterator>::
        less(SeqIterator a, SeqIterator b) const {
    for (auto it = beginSub_; it != endSub_; ++it)
        if (a->first[*it] < b->first[*it])
            return true;
        else if (a->first[*it] > b->first[*it])
            return false;
    return false;
}

template <typename T>
template <typename IndexIterator>
template <typename SeqIterator>
inline bool LightweightSequence<T>::SubsequenceCompareFirst<IndexIterator>::
        operator () (SeqIterator a, SeqIterator b) const {
    for (auto it = beginSub_; it != endSub_; ++it)
        if (a->first[*it] < b->first[*it])
            return true;
        else if (a->first[*it] > b->first[*it])
            return false;
    return false;
}

template <typename T>
inline void swap(LightweightSequence<T>& a, LightweightSequence<T>& b)
        noexcept {
    a.swap(b);
}

} // namespace regina

#endif

