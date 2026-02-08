
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

/*! \file utilities/bitmask.h
 *  \brief Provides optimised bitmasks of arbitrary length.
 */

#ifndef __REGINA_BITMASK_H
#ifndef __DOXYGEN
#define __REGINA_BITMASK_H
#endif

#include <algorithm>
#include <iostream>

#include "regina-core.h"
#include "concepts/core.h"
#include "concepts/iterator.h"
#include "utilities/bitmanip.h"

namespace regina {

template <UnsignedCppInteger> class Bitmask1;
template <UnsignedCppInteger, UnsignedCppInteger> class Bitmask2;

/**
 * A bitmask that can store arbitrarily many true-or-false bits.
 *
 * This bitmask packs the bits together, so that (unlike an array of bools)
 * many bits can be stored in a single byte.  As a result, operations on
 * this class are fast because the CPU can work on many bits simultaneously.
 *
 * Nevertheless, this class still has overhead because the bits must be
 * allocated on the heap, and because every operation requires looping
 * through the individual bytes.  For reasonably small bitmasks, see the
 * highly optimised Bitmask1 and Bitmask2 classes instead.
 *
 * Once a bitmask is created, the only way its length (the number of bits)
 * can be changed is by calling reset(size_t).
 *
 * The length of the bitmask is not actually stored in this structure.
 * This means that, upon construction (or reset), the length will be
 * automatically rounded up to the next "raw unit of storage".
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \todo \opt Insist that sizeof(Piece) is a power of two, and replace
 * expensive division/mod operations with cheap bit operations.
 *
 * \warning Because this class may increase the length of the bitmask
 * (rounding up to the next unit of storage), bitwise computations may
 * not give the results that you expect.  In particular, flip() may set
 * additional \c true bits in the "dead space" between the intended length
 * and the actual length, and this may have a flow-on effect for other
 * operations (such as subset testing, bit counting and so on).  Be careful!
 *
 * \ingroup utilities
 */
class Bitmask {
    public:
        /**
         * A class constant indicating whether this class stores
         * bitmasks whose sizes are fixed at compile time.
         *
         * For the general Bitmask class, this is \c false.  For the highly
         * optimised Bitmask1 and Bitmask2 template classes, this is \c true.
         */
        static constexpr bool fixedSize = false;

    private:
        using Piece = unsigned;
            /**< The types of the machine-native pieces into which this
                 bitmask is split. */
        size_t pieces;
            /**< The number of machine-native pieces into which this bitmask
                 is split. */
        Piece* mask;
            /**< The array of pieces, each of which stores 8 * sizeof(Piece)
                 individual bits. */

    public:
        /**
         * Creates a new invalid bitmask.  You must call the one-argument
         * reset(size_t) or use the assignment operator to give the
         * bitmask a length before it can be used.
         *
         * Use of this default constructor is discouraged.  The only
         * reason it exists is to support arrays and containers of
         * bitmasks, where the bitmasks must be created in bulk and then
         * individually assigned lengths.
         *
         * \warning No other routines can be used with this bitmask
         * until it has been assigned a length via reset(size_t) or
         * the assignment operator.  As the single exception, the class
         * destructor is safe to use even if a bitmask has never been
         * initialised.
         */
        Bitmask();

        /**
         * Creates a new bitmask of the given length with all bits set to
         * \c false.
         *
         * \param length the number of bits stored in this bitmask; this must
         * be at least one.
         */
        Bitmask(size_t length);

        /**
         * Creates a clone of the given bitmask.
         *
         * It is fine if the given bitmask is invalid (but in this case,
         * the new bitmask will be invalid also).  Invalid bitmasks must be
         * assigned a length using reset(size_t) or the assignment operator.
         *
         * \param src the bitmask to clone.
         */
        Bitmask(const Bitmask& src);

        /**
         * Moves the contents of the given bitmask into this new bitmask.
         *
         * It is fine if the given bitmask is invalid (but in this case,
         * the new bitmask will be invalid also).  Invalid bitmasks must be
         * assigned a length using reset(size_t) or the assignment operator.
         *
         * The bitmask that was passed (\a src) will no longer be usable.
         *
         * \param src the bitmask whose contents should be moved.
         */
        Bitmask(Bitmask&& src) noexcept;

        /**
         * Destroys this bitmask.
         */
        ~Bitmask();

        /**
         * Returns the value of the given bit of this bitmask.
         *
         * \param index indicates which bit to query; this must be at least
         * zero and strictly less than the length of this bitmask.
         * \return the value of the (\a index)th bit.
         */
        bool get(size_t index) const;

        /**
         * Sets the given bit of this bitmask to the given value.
         *
         * \param index indicates which bit to set; this must be at least zero
         * and strictly less than the length of this bitmask.
         * \param value the value that will be assigned to the (\a index)th bit.
         */
        void set(size_t index, bool value);

        /**
         * Sets all bits in the given sorted list to the given value.
         *
         * This is a convenience routine for setting many bits at once.
         * The indices of the bits to set should be sorted and stored in
         * some container, such as a std::set or a C-style array.  This
         * routine takes iterators over this container, and sets the
         * bits at the corresponding indices to the given value.
         *
         * For example, the following code would set bits 3, 5 and 6 to \c true:
         *
         * \code
         * std::vector<unsigned> indices;
         * indices.push(3); indices.push(5); indices.push(6);
         * bitmask.set(indices.begin(), indices.end(), true);
         * \endcode
         *
         * Likewise, the following code would set bits 1, 4 and 7 to \c false:
         *
         * \code
         * unsigned indices[3] = { 1, 4, 7 };
         * bitmask.set(indices, indices + 3, false);
         * \endcode
         *
         * All other bits of this bitmask are unaffected by this routine.
         *
         * \pre The list of indices described by these iterators is
         * in _sorted_ order.  This is to allow optimisations for
         * larger bitmask types.
         * \pre All indices in the given list are at least zero and
         * strictly less than the length of this bitmask.
         *
         * \python Instead of a pair of iterators, you should pass
         * a Python list (which, as described above, must be a sorted
         * list of indices).
         *
         * \param indexBegin the beginning of the iterator range
         * containing the sorted indices of the bits to set.
         * \param indexEnd the end of the iterator range containing the
         * sorted indices of the bits to set.
         * \param value the value that will be assigned to each of the
         * corresponding bits.
         */
        template <InputIteratorFor<size_t> iterator>
        void set(iterator indexBegin, iterator indexEnd, bool value) {
            Piece* base = mask;
            size_t offset = 0;
            size_t diff;

            for ( ; indexBegin != indexEnd; ++indexBegin) {
                // INV: offset = (base - mask) * 8 * sizeof(Piece)
                // INV: *indexBegin >= offset
                if (*indexBegin >= offset + (8 * sizeof(Piece))) {
                    diff = ((*indexBegin - offset) / (8 * sizeof(Piece)));
                    base += diff;
                    offset += (8 * sizeof(Piece) * diff);
                }

                *base |= (Piece(1) << (*indexBegin - offset));
                if (! value)
                    *base ^= (Piece(1) << (*indexBegin - offset));
            }
        }

        /**
         * Sets all bits of this bitmask to \c false.
         *
         * \warning The length of this bitmask must already have been
         * initialised.  In particular, if the default constructor was
         * used, you must call the one-argument reset(size_t) before you
         * can use this routine.
         */
        void reset();

        /**
         * Resizes this bitmask to the given length and sets all bits to
         * \c false.
         *
         * This routine can be used to change the length (number of
         * bits) of the bitmask if desired.
         *
         * \param length the number of bits to store in this bitmask; this
         * must be at least one.
         */
        void reset(size_t length);

        /**
         * Sets this bitmask to a copy of the given bitmask.
         *
         * If this bitmask is invalid, this assignment operator can be
         * used to initialise it with a length.
         *
         * If this bitmask has already been initialised to a different
         * length from that of the given bitmask, the length of this
         * bitmask will be changed accordingly.
         *
         * If the given bitmask is invalid, this bitmask will become
         * invalid also.  Invalid bitmasks must be assigned a length using
         * reset(size_t) or this assignment operator.
         *
         * \param other the bitmask to clone.
         * \return a reference to this bitmask.
         */
        Bitmask& operator = (const Bitmask& other);

        /**
         * Moves the contents of the given bitmask into this bitmask.
         *
         * It is fine if the given bitmask is invalid (but in this case,
         * the new bitmask will be invalid also).  Invalid bitmasks must be
         * assigned a length using reset(size_t) or the assignment operator.
         *
         * The bitmask that was passed (\a src) will no longer be usable.
         *
         * \param src the bitmask whose contents should be moved.
         * \return a reference to this bitmask.
         */
        Bitmask& operator = (Bitmask&& src) noexcept;

        /**
         * Swaps the contents of this and the given bitmask.
         *
         * \param other the bitmask whose contents should be swapped with this.
         */
        void swap(Bitmask& other) noexcept;

        /**
         * Leaves the first \a numBits bits of this bitmask intact, but
         * sets all subsequent bits to \c false.  In other words, this
         * routine "truncates" this bitmask to the given number of bits.
         *
         * This routine does not change the _length_ of this bitmask
         * (as passed to the contructor or to reset()).
         *
         * \pre \a numBits is at most the length of this bitmask.
         *
         * \param numBits the number of bits that will _not_ be cleared.
         */
        void truncate(size_t numBits);

        /**
         * Sets this to the intersection of this and the given bitmask.
         * Every bit that is unset in \a other will be unset in this bitmask.
         *
         * \pre This and the given bitmask have the same length.
         *
         * \param other the bitmask to intersect with this.
         * \return a reference to this bitmask.
         */
        Bitmask& operator &= (const Bitmask& other);

        /**
         * Sets this to the union of this and the given bitmask.
         * Every bit that is set in \a other will be set in this bitmask.
         *
         * \pre This and the given bitmask have the same length.
         *
         * \param other the bitmask to union with this.
         * \return a reference to this bitmask.
         */
        Bitmask& operator |= (const Bitmask& other);

        /**
         * Sets this to the exclusive disjunction (XOR) of this and the
         * given bitmask.  Every bit that is set in \a other will be
         * flipped in this bitmask.
         *
         * \pre This and the given bitmask have the same length.
         *
         * \param other the bitmask to XOR with this.
         * \return a reference to this bitmask.
         */
        Bitmask& operator ^= (const Bitmask& other);

        /**
         * Sets this to the set difference of this and the given bitmask.
         * Every bit that is set in \a other will be cleared in this bitmask.
         *
         * \pre This and the given bitmask have the same length.
         *
         * \param other the bitmask to XOR with this.
         * \return a reference to this bitmask.
         */
        Bitmask& operator -= (const Bitmask& other);

        /**
         * Negates every bit in this bitmask.  All \c true bits will be
         * set to \c false and vice versa.
         *
         * \warning Because this class may increase the bitmask length
         * (rounding up to the next unit of storage), flip() may set
         * additional \c true bits in the "dead space" between the intended
         * length and the actual length.  This may cause unexpected
         * results for routines such as subset testing, bit counting and
         * so on.  Be careful!
         */
        void flip();

        /**
         * Determines whether this and the given bitmask are identical.
         *
         * \warning As explain in the class notes, bitmasks do not store
         * their exact length; instead the length is rounded up to the
         * next "raw unit of storage".  This means that two bitmasks
         * that were initialised with different lengths may still be
         * considered equal if the two lengths round up to the same value
         * _and_ the extra bits in the longer bitmask are all \c false.
         *
         * \param other the bitmask to compare against this.
         * \return \c true if and only if this and the given bitmask are
         * identical.
         */
        bool operator == (const Bitmask& other) const;

        /**
         * Determines whether this bitmask appears strictly before the given
         * bitmask when bitmasks are sorted in lexicographical order.
         * Here the bit at index 0 is least significant, and the bit at
         * index \a length-1 is most significant.
         *
         * \pre This and the given bitmask have the same length.
         *
         * \warning We do not use \< for this ordering, since the comparison
         * operators (`<`, `≤`, `>`, `≥`) work with the subset relation instead.
         *
         * \param other the bitmask to compare against this.
         * \return \c true if and only if this is lexicographically
         * strictly smaller than the given bitmask.
         */
        bool lessThan(const Bitmask& other) const;

        /**
         * Compares two bitmasks under the subset relation.
         *
         * Here the bitmask \a x is considered less than \a y if the bits that
         * are set in \a x form a strict subset of the bits that are set in
         * \a y.  In other words: `x ≠ y`, and every bit that is set in \a x
         * is also set in \a y.
         *
         * This operator generates all of the usual comparison operators,
         * including `<`, `<=`, `>`, and `>=`.
         *
         * \pre This and the given bitmask have the same length.
         *
         * \note This does not compare bitmasks lexicographically.
         * For lexicographical comparison, use lessThan() instead.
         *
         * \python This spaceship operator `x <=> y` is not available, but the
         * other comparison operators that it generates _are_ available.
         *
         * \param rhs the bitmask to compare against this.
         * \return The result of the subset comparison between this and the
         * given bitmask.
         */
        std::partial_ordering operator <=> (const Bitmask& rhs) const;

        /**
         * Determines whether this bitmask is entirely contained within
         * the union of the two given bitmasks.
         *
         * For this routine to return \c true, every bit that is set
         * in this bitmask must also be set in either \a x or \a y.
         *
         * \pre Both \a x and \a y are the same length as this bitmask.
         *
         * \param x the first bitmask used to form the union.
         * \param y the first bitmask used to form the union.
         * \return \c true if and only if this bitmask is entirely contained
         * within the union of \a x and \a y.
         */
        bool inUnion(const Bitmask& x, const Bitmask& y) const;

        /**
         * Determines whether this bitmask contains the intersection of
         * the two given bitmasks.
         *
         * For this routine to return \c true, every bit that is set in
         * _both_ \a x and \a y must be set in this bitmask also.
         *
         * \pre Both \a x and \a y are the same length as this bitmask.
         *
         * \param x the first bitmask used to form the intersection.
         * \param y the first bitmask used to form the intersection.
         * \return \c true if and only if this bitmask entirely contains
         * the intersection of \a x and \a y.
         */
        bool containsIntn(const Bitmask& x, const Bitmask& y) const;

        /**
         * Returns the number of bits currently set to \c true in this
         * bitmask.
         *
         * \return the number of \c true bits.
         */
        size_t bits() const;

        /**
         * Returns the index of the first \c true bit in this bitmask,
         * or -1 if there are no \c true bits.
         *
         * \return the index of the first \c true bit.
         */
        ssize_t firstBit() const;

        /**
         * Returns the index of the last \c true bit in this bitmask,
         * or -1 if there are no \c true bits.
         *
         * \return the index of the last \c true bit.
         */
        ssize_t lastBit() const;

        /**
         * Determines whether at most one bit is set to \c true in this
         * bitmask.
         *
         * If this bitmask is entirely \c false or if only one bit is set
         * to \c true, then this routine will return \c true.  Otherwise
         * this routine will return \c false.
         *
         * \return \c true if and only if at most one bit is set to \c true.
         */
        bool atMostOneBit() const;

    friend std::ostream& operator << (std::ostream& out, const Bitmask& mask);
};

/**
 * Swaps the contents of the two given bitmasks.
 *
 * This global routine simply calls Bitmask::swap(); it is provided
 * so that Bitmask meets the C++ Swappable requirements.
 *
 * \param a the first bitmask whose contents should be swapped.
 * \param b the second bitmask whose contents should be swapped.
 *
 * \ingroup utilities
 */
void swap(Bitmask& a, Bitmask& b) noexcept;

/**
 * Writes the given bitmask to the given output stream as a sequence of
 * zeroes and ones.
 *
 * Since the length of the bitmask is not stored, the number of bits
 * written might be greater than the length initially assigned to this
 * bitmask (specifically, the length will be rounded up to the next "raw
 * unit of storage").
 *
 * \param out the output stream to which to write.
 * \param mask the bitmask to write.
 * \return a reference to the given output stream.
 *
 * \ingroup utilities
 */
std::ostream& operator << (std::ostream& out, const Bitmask& mask);

/**
 * Writes the given bitmask to the given output stream as a sequence of
 * zeroes and ones.
 *
 * Since the length of the bitmask is not stored, the number of bits
 * written will be `8 * sizeof(T)`.
 *
 * \param out the output stream to which to write.
 * \param mask the bitmask to write.
 * \return a reference to the given output stream.
 *
 * \ingroup utilities
 */
template <UnsignedCppInteger T>
std::ostream& operator << (std::ostream& out, const Bitmask1<T>& mask) {
    for (T bit = 1; bit; bit <<= 1)
        out << ((mask.mask & bit) ? '1' : '0');
    return out;
}

/**
 * A small but extremely fast bitmask class that can store up to
 * `8 * sizeof(T)` true-or-false bits.
 *
 * This bitmask packs all of the bits together into a single variable of
 * type \a T.  This means that operations on bitmasks are extremely
 * fast, because all of the bits can be processed at once.
 *
 * The downside of course is that the number of bits that can be stored
 * is limited to `8 * sizeof(T)`, where \a T is some native unsigned
 * C++ integer type.
 *
 * For another extremely fast bitmask class that can store twice as
 * many bits, see Bitmask2.  For a bitmask class that can store
 * arbitrarily many bits, see Bitmask.
 *
 * These objects are small enough to pass by value and swap with std::swap(),
 * with no need for any specialised move operations or swap functions.
 *
 * \python Python does not support templates, and so instead Regina's
 * python interface offers the classes Bitmask8, Bitmask16, Bitmask32,
 * Bitmask64, Bitmask128, and (if the machine supports 128-bit integers)
 * Bitmask256.  Each of these will be an optimised bitmask class that
 * can hold the corresponding number of bits, and is guaranteed to be an
 * instance of either the C++ Bitmask1<T> class (where possible) or the
 * C++ Bitmask2<T,U> template class (if necessary).
 *
 * \ingroup utilities
 */
template <UnsignedCppInteger T>
class Bitmask1 {
    public:
        /**
         * A class constant indicating whether this class stores
         * bitmasks whose sizes are fixed at compile time.
         *
         * For the general Bitmask class, this is \c false.  For the highly
         * optimised Bitmask1 and Bitmask2 template classes, this is \c true.
         */
        static constexpr bool fixedSize = true;

    private:
        T mask;
            /**< Contains all 8 * sizeof(\a T) bits of this bitmask. */

    public:
        /**
         * Creates a new bitmask with all bits set to \c false.
         */
        inline Bitmask1() : mask(0) {
        }

        /**
         * Creates a new bitmask with all bits set to \c false.
         *
         * The integer argument is merely for compatibility with
         * the Bitmask constructor, and will be ignored.
         *
         * \warning This is _not_ a constructor that initialises the
         * bitmask to a given pattern.
         */
        inline Bitmask1(size_t) : mask(0) {
        }

        /**
         * Creates a clone of the given bitmask.
         */
        inline Bitmask1(const Bitmask1&) = default;

        /**
         * Sets all bits of this bitmask to \c false.
         */
        inline void reset() {
            mask = 0;
        }

        /**
         * Sets all bits of this bitmask to \c false.
         *
         * The integer argument is merely for compatibility with
         * Bitmask::reset(size_t), and will be ignored.
         */
        inline void reset(size_t) {
            mask = 0;
        }

        /**
         * Sets this bitmask to a copy of the given bitmask.
         *
         * \return a reference to this bitmask.
         */
        Bitmask1& operator = (const Bitmask1&) = default;

        /**
         * Leaves the first \a numBits bits of this bitmask intact, but
         * sets all subsequent bits to \c false.  In other words, this
         * routine "truncates" this bitmask to the given number of bits.
         *
         * This routine does not change the _length_ of this bitmask
         * (as passed to the contructor or to reset()).
         *
         * \param numBits the number of bits that will _not_ be cleared.
         */
        inline void truncate(size_t numBits) {
            if (numBits < 8 * sizeof(T))
                mask &= ((T(1) << numBits) - T(1));
        }

        /**
         * Returns the value of the given bit of this bitmask.
         *
         * \param index indicates which bit to query; this must be between
         * 0 and (8 * sizeof(\a T) - 1) inclusive.
         * \return the value of the (\a index)th bit.
         */
        inline bool get(size_t index) const {
            return (mask & (T(1) << index));
        }

        /**
         * Sets the given bit of this bitmask to the given value.
         *
         * \param index indicates which bit to set; this must be between
         * 0 and (8 * sizeof(\a T) - 1) inclusive.
         * \param value the value that will be assigned to the (\a index)th bit.
         */
        inline void set(size_t index, bool value) {
            mask |= (T(1) << index);
            if (! value)
                mask ^= (T(1) << index);
        }

        /**
         * Sets all bits in the given sorted list to the given value.
         *
         * This is a convenience routine for setting many bits at once.
         * The indices of the bits to set should be sorted and stored in
         * some container, such as a std::set or a C-style array.  This
         * routine takes iterators over this container, and sets the
         * bits at the corresponding indices to the given value.
         *
         * For example, the following code would set bits 3, 5 and 6
         * to \c true:
         *
         * \code
         * std::vector<unsigned> indices;
         * indices.push(3); indices.push(5); indices.push(6);
         * bitmask.set(indices.begin(), indices.end(), true);
         * \endcode
         *
         * Likewise, the following code would set bits 1, 4 and 7 to \c false:
         *
         * \code
         * unsigned indices[3] = { 1, 4, 7 };
         * bitmask.set(indices, indices + 3, false);
         * \endcode
         *
         * All other bits of this bitmask are unaffected by this routine.
         *
         * \pre The list of indices described by these iterators is
         * in _sorted_ order.  This is to allow optimisations for
         * larger bitmask types.
         * \pre All indices in the given list are between
         * 0 and (8 * sizeof(\a T) - 1) inclusive.
         *
         * \python Instead of a pair of iterators, you should pass
         * a Python list (which, as described above, must be a sorted
         * list of indices).
         *
         * \param indexBegin the beginning of the iterator range
         * containing the sorted indices of the bits to set.
         * \param indexEnd the end of the iterator range containing the
         * sorted indices of the bits to set.
         * \param value the value that will be assigned to each of the
         * corresponding bits.
         */
        template <InputIteratorFor<size_t> iterator>
        void set(iterator indexBegin, iterator indexEnd, bool value) {
            for ( ; indexBegin != indexEnd; ++indexBegin) {
                mask |= (T(1) << *indexBegin);
                if (! value)
                    mask ^= (T(1) << *indexBegin);
            }
        }

        /**
         * Sets this to the intersection of this and the given bitmask.
         * Every bit that is unset in \a other will be unset in this bitmask.
         *
         * \param other the bitmask to intersect with this.
         * \return a reference to this bitmask.
         */
        inline Bitmask1& operator &= (const Bitmask1& other) {
            mask &= other.mask;
            return *this;
        }

        /**
         * Sets this to the union of this and the given bitmask.
         * Every bit that is set in \a other will be set in this bitmask.
         *
         * \param other the bitmask to union with this.
         * \return a reference to this bitmask.
         */
        inline Bitmask1& operator |= (const Bitmask1& other) {
            mask |= other.mask;
            return *this;
        }

        /**
         * Sets this to the exclusive disjunction (XOR) of this and the
         * given bitmask.  Every bit that is set in \a other will be
         * flipped in this bitmask.
         *
         * \param other the bitmask to XOR with this.
         * \return a reference to this bitmask.
         */
        inline Bitmask1& operator ^= (const Bitmask1& other) {
            mask ^= other.mask;
            return *this;
        }

        /**
         * Sets this to the set difference of this and the given bitmask.
         * Every bit that is set in \a other will be cleared in this bitmask.
         *
         * \param other the bitmask to XOR with this.
         * \return a reference to this bitmask.
         */
        inline Bitmask1& operator -= (const Bitmask1& other) {
            mask |= other.mask;
            mask ^= other.mask;
            return *this;
        }

        /**
         * Negates every bit in this bitmask.  All \c true bits will be
         * set to \c false and vice versa.
         *
         * Unlike the more generic Bitmask, this optimised bitmask
         * class does not store a length.  This means that all
         * 8 * sizeof(\a T) possible bits will be negated.
         */
        inline void flip() {
            mask = ~mask;
        }

        /**
         * Determines whether this and the given bitmask are identical.
         *
         * \return \c true if and only if this and the given bitmask are
         * identical.
         */
        bool operator == (const Bitmask1&) const = default;

        /**
         * Determines whether this bitmask appears strictly before the given
         * bitmask when bitmasks are sorted in lexicographical order.
         * Here the bit at index 0 is least significant, and the bit at
         * index \a length-1 is most significant.
         *
         * \warning We do not use \< for this ordering, since the comparison
         * operators (`<`, `≤`, `>`, `≥`) work with the subset relation instead.
         *
         * \param other the bitmask to compare against this.
         * \return \c true if and only if this is lexicographically
         * strictly smaller than the given bitmask.
         */
        inline bool lessThan(const Bitmask1& other) const {
            return (mask < other.mask);
        }

        /**
         * Compares two bitmasks under the subset relation.
         *
         * Here the bitmask \a x is considered less than \a y if the bits that
         * are set in \a x form a strict subset of the bits that are set in
         * \a y.  In other words: `x ≠ y`, and every bit that is set in \a x
         * is also set in \a y.
         *
         * This operator generates all of the usual comparison operators,
         * including `<`, `<=`, `>`, and `>=`.
         *
         * \note This does not compare bitmasks lexicographically.
         * For lexicographical comparison, use lessThan() instead.
         *
         * \python This spaceship operator `x <=> y` is not available, but the
         * other comparison operators that it generates _are_ available.
         *
         * \param rhs the bitmask to compare against this.
         * \return The result of the subset comparison between this and the
         * given bitmask.
         */
        inline std::partial_ordering operator <=> (const Bitmask1& rhs)
                const {
            return BitManipulator<T>::subsetComparison(mask, rhs.mask);
        }

        /**
         * Determines whether this bitmask is entirely contained within
         * the union of the two given bitmasks.
         *
         * For this routine to return \c true, every bit that is set
         * in this bitmask must also be set in either \a x or \a y.
         *
         * \param x the first bitmask used to form the union.
         * \param y the first bitmask used to form the union.
         * \return \c true if and only if this bitmask is entirely contained
         * within the union of \a x and \a y.
         */
        inline bool inUnion(const Bitmask1& x, const Bitmask1& y)
                const {
            return ((mask & (x.mask | y.mask)) == mask);
        }

        /**
         * Determines whether this bitmask contains the intersection of
         * the two given bitmasks.
         *
         * For this routine to return \c true, every bit that is set in
         * _both_ \a x and \a y must be set in this bitmask also.
         *
         * \param x the first bitmask used to form the intersection.
         * \param y the first bitmask used to form the intersection.
         * \return \c true if and only if this bitmask entirely contains
         * the intersection of \a x and \a y.
         */
        inline bool containsIntn(const Bitmask1& x, const Bitmask1& y)
                const {
            return ((mask | (x.mask & y.mask)) == mask);
        }

        /**
         * Returns the number of bits currently set to \c true in this
         * bitmask.
         *
         * \return the number of \c true bits.
         */
        inline size_t bits() const {
            return BitManipulator<T>::bits(mask);
        }

        /**
         * Returns the index of the first \c true bit in this bitmask,
         * or -1 if there are no \c true bits.
         *
         * \return the index of the first \c true bit.
         */
        inline ssize_t firstBit() const {
            return BitManipulator<T>::firstBit(mask);
        }

        /**
         * Returns the index of the last \c true bit in this bitmask,
         * or -1 if there are no \c true bits.
         *
         * \return the index of the last \c true bit.
         */
        inline ssize_t lastBit() const {
            return BitManipulator<T>::lastBit(mask);
        }

        /**
         * Determines whether at most one bit is set to \c true in this
         * bitmask.
         *
         * If this bitmask is entirely \c false or if only one bit is set
         * to \c true, then this routine will return \c true.  Otherwise
         * this routine will return \c false.
         *
         * \return \c true if and only if at most one bit is set to \c true.
         */
        inline bool atMostOneBit() const {
            return BitManipulator<T>::bits(mask) <= 1;
        }

#ifndef __DOXYGEN
    // Doxygen gets confused by the "<< <" combination here.
    friend std::ostream& operator << <T>(std::ostream& out,
        const Bitmask1& mask);
#endif
};

/**
 * Writes the given bitmask to the given output stream as a sequence of
 * zeroes and ones.
 *
 * Since the length of the bitmask is not stored, the number of bits
 * written will be `8 * sizeof(T) + 8 * sizeof(U)`.
 *
 * \param out the output stream to which to write.
 * \param mask the bitmask to write.
 * \return a reference to the given output stream.
 *
 * \ingroup utilities
 */
template <UnsignedCppInteger T, UnsignedCppInteger U>
std::ostream& operator << (std::ostream& out, const Bitmask2<T, U>& mask) {
    for (T bit = 1; bit; bit <<= 1)
        out << ((mask.low & bit) ? '1' : '0');
    for (U bit = 1; bit; bit <<= 1)
        out << ((mask.high & bit) ? '1' : '0');
    return out;
}

/**
 * A small but extremely fast bitmask class that can store up to
 * `8 * sizeof(T) + 8 * sizeof(U)` true-or-false bits.
 *
 * This bitmask packs all of the bits together into a single variable of
 * type \a T and a single variable of type \a U.  This means that operations
 * on entire bitmasks are extremely fast, because all of the bits can be
 * processed in just two "native" operations.
 *
 * The downside of course is that the number of bits that can be stored
 * is limited to `8 * sizeof(T) + 8 * sizeof(U)`, where \a T and \a U
 * are some native unsigned C++ integer types.
 *
 * For an even faster bitmask class that can only store half as many bits,
 * see Bitmask1.  For a bitmask class that can store arbitrarily many bits,
 * see Bitmask.
 *
 * These objects are small enough to pass by value and swap with std::swap(),
 * with no need for any specialised move operations or swap functions.
 *
 * \python Python does not support templates, and so instead Regina's
 * python interface offers the classes Bitmask8, Bitmask16, Bitmask32,
 * Bitmask64, Bitmask128, and (if the machine supports 128-bit integers)
 * Bitmask256.  Each of these will be an optimised bitmask class that
 * can hold the corresponding number of bits, and is guaranteed to be an
 * instance of either the C++ Bitmask1<T> class (where possible) or the
 * C++ Bitmask2<T,U> template class (if necessary).
 *
 * \ingroup utilities
 */
template <UnsignedCppInteger T, UnsignedCppInteger U = T>
class Bitmask2 {
    public:
        /**
         * A class constant indicating whether this class stores
         * bitmasks whose sizes are fixed at compile time.
         *
         * For the general Bitmask class, this is \c false.  For the highly
         * optimised Bitmask1 and Bitmask2 template classes, this is \c true.
         */
        static constexpr bool fixedSize = true;

    private:
        T low;
            /**< Contains the first 8 * sizeof(\a T) bits of this bitmask. */
        U high;
            /**< Contains the final 8 * sizeof(\a U) bits of this bitmask. */

    public:
        /**
         * Creates a new bitmask with all bits set to \c false.
         */
        inline Bitmask2() : low(0), high(0) {
        }

        /**
         * Creates a new bitmask with all bits set to \c false.
         *
         * The integer argument is merely for compatibility with
         * the Bitmask constructor, and will be ignored.
         *
         * \warning This is _not_ a constructor that initialises the
         * bitmask to a given pattern.
         */
        inline Bitmask2(size_t) : low(0), high(0) {
        }

        /**
         * Creates a clone of the given bitmask.
         */
        inline Bitmask2(const Bitmask2&) = default;

        /**
         * Sets all bits of this bitmask to \c false.
         */
        inline void reset() {
            low = 0;
            high = 0;
        }

        /**
         * Sets all bits of this bitmask to \c false.
         *
         * The integer argument is merely for compatibility with
         * Bitmask::reset(size_t), and will be ignored.
         */
        inline void reset(size_t) {
            low = 0;
            high = 0;
        }

        /**
         * Sets this bitmask to a copy of the given bitmask.
         *
         * \return a reference to this bitmask.
         */
        Bitmask2& operator = (const Bitmask2&) = default;

        /**
         * Leaves the first \a numBits bits of this bitmask intact, but
         * sets all subsequent bits to \c false.  In other words, this
         * routine "truncates" this bitmask to the given number of bits.
         *
         * This routine does not change the _length_ of this bitmask
         * (as passed to the contructor or to reset()).
         *
         * \param numBits the number of bits that will _not_ be cleared.
         */
        inline void truncate(size_t numBits) {
            if (numBits < 8 * sizeof(T)) {
                low &= ((T(1) << numBits) - T(1));
                high = 0;
            } else {
                numBits -= 8 * sizeof(T);
                if (numBits < 8 * sizeof(U))
                    high &= ((U(1) << numBits) - U(1));
            }
        }

        /**
         * Returns the value of the given bit of this bitmask.
         *
         * \param index indicates which bit to query; this must be between
         * 0 and (8 * sizeof(\a T) + 8 * sizeof(\a U) - 1) inclusive.
         * \return the value of the (\a index)th bit.
         */
        inline bool get(size_t index) const {
            if (index < 8 * sizeof(T))
                return (low & (T(1) << index));
            else
                return (high & (U(1) << (index - 8 * sizeof(T))));
        }

        /**
         * Sets the given bit of this bitmask to the given value.
         *
         * \param index indicates which bit to set; this must be between
         * 0 and (8 * sizeof(\a T) + 8 * sizeof(\a U) - 1) inclusive.
         * \param value the value that will be assigned to the (\a index)th bit.
         */
        inline void set(size_t index, bool value) {
            if (index < 8 * sizeof(T)) {
                low |= (T(1) << index);
                if (! value)
                    low ^= (T(1) << index);
            } else {
                high |= (U(1) << (index - 8 * sizeof(T)));
                if (! value)
                    high ^= (U(1) << (index - 8 * sizeof(T)));
            }
        }

        /**
         * Sets all bits in the given sorted list to the given value.
         *
         * This is a convenience routine for setting many bits at once.
         * The indices of the bits to set should be sorted and stored in
         * some container, such as a std::set or a C-style array.  This
         * routine takes iterators over this container, and sets the
         * bits at the corresponding indices to the given value.
         *
         * For example, the following code would set bits 3, 5 and 6
         * to \c true:
         *
         * \code
         * std::vector<unsigned> indices;
         * indices.push(3); indices.push(5); indices.push(6);
         * bitmask.set(indices.begin(), indices.end(), true);
         * \endcode
         *
         * Likewise, the following code would set bits 1, 4 and 7 to \c false:
         *
         * \code
         * unsigned indices[3] = { 1, 4, 7 };
         * bitmask.set(indices, indices + 3, false);
         * \endcode
         *
         * All other bits of this bitmask are unaffected by this routine.
         *
         * \pre The list of indices described by these iterators is
         * in _sorted_ order.  This is to allow optimisations for
         * larger bitmask types.
         * \pre All indices in the given list are between
         * 0 and (8 * sizeof(\a T) + 8 * sizeof(\a U) - 1) inclusive.
         *
         * \python Instead of a pair of iterators, you should pass
         * a Python list (which, as described above, must be a sorted
         * list of indices).
         *
         * \param indexBegin the beginning of the iterator range
         * containing the sorted indices of the bits to set.
         * \param indexEnd the end of the iterator range containing the
         * sorted indices of the bits to set.
         * \param value the value that will be assigned to each of the
         * corresponding bits.
         */
        template <InputIteratorFor<size_t> iterator>
        void set(iterator indexBegin, iterator indexEnd, bool value) {
            // First deal with the bits stored in low.
            for ( ; indexBegin != indexEnd && *indexBegin < 8 * sizeof(T);
                    ++indexBegin) {
                low |= (T(1) << *indexBegin);
                if (! value)
                    low ^= (T(1) << *indexBegin);
            }

            // Now deal with the bits stored in high.
            for ( ; indexBegin != indexEnd; ++indexBegin) {
                high |= (U(1) << ((*indexBegin) - 8 * sizeof(T)));
                if (! value)
                    high ^= (U(1) << ((*indexBegin) - 8 * sizeof(T)));
            }
        }

        /**
         * Sets this to the intersection of this and the given bitmask.
         * Every bit that is unset in \a other will be unset in this bitmask.
         *
         * \param other the bitmask to intersect with this.
         * \return a reference to this bitmask.
         */
        inline Bitmask2& operator &= (const Bitmask2& other) {
            low &= other.low;
            high &= other.high;
            return *this;
        }

        /**
         * Sets this to the union of this and the given bitmask.
         * Every bit that is set in \a other will be set in this bitmask.
         *
         * \param other the bitmask to union with this.
         * \return a reference to this bitmask.
         */
        inline Bitmask2& operator |= (const Bitmask2& other) {
            low |= other.low;
            high |= other.high;
            return *this;
        }

        /**
         * Sets this to the exclusive disjunction (XOR) of this and the
         * given bitmask.  Every bit that is set in \a other will be
         * flipped in this bitmask.
         *
         * \param other the bitmask to XOR with this.
         * \return a reference to this bitmask.
         */
        inline Bitmask2& operator ^= (const Bitmask2& other) {
            low ^= other.low;
            high ^= other.high;
            return *this;
        }

        /**
         * Sets this to the set difference of this and the given bitmask.
         * Every bit that is set in \a other will be cleared in this bitmask.
         *
         * \param other the bitmask to XOR with this.
         * \return a reference to this bitmask.
         */
        inline Bitmask2& operator -= (const Bitmask2& other) {
            low |= other.low;
            low ^= other.low;
            high |= other.high;
            high ^= other.high;
            return *this;
        }

        /**
         * Negates every bit in this bitmask.  All \c true bits will be
         * set to \c false and vice versa.
         *
         * Unlike the more generic Bitmask, this optimised bitmask
         * class does not store a length.  This means that all
         * 8 * sizeof(\a T) + 8 * sizeof(\a U) possible bits will be negated.
         */
        inline void flip() {
            low = ~low;
            high = ~high;
        }

        /**
         * Determines whether this and the given bitmask are identical.
         *
         * \return \c true if and only if this and the given bitmask are
         * identical.
         */
        bool operator == (const Bitmask2& other) const = default;

        /**
         * Determines whether this bitmask appears strictly before the given
         * bitmask when bitmasks are sorted in lexicographical order.
         * Here the bit at index 0 is least significant, and the bit at
         * index \a length-1 is most significant.
         *
         * \warning We do not use \< for this ordering, since the comparison
         * operators (`<`, `≤`, `>`, `≥`) work with the subset relation instead.
         *
         * \param other the bitmask to compare against this.
         * \return \c true if and only if this is lexicographically
         * strictly smaller than the given bitmask.
         */
        inline bool lessThan(const Bitmask2& other) const {
            return (high < other.high ||
                (high == other.high && low < other.low));
        }

        /**
         * Compares two bitmasks under the subset relation.
         *
         * Here the bitmask \a x is considered less than \a y if the bits that
         * are set in \a x form a strict subset of the bits that are set in
         * \a y.  In other words: `x ≠ y`, and every bit that is set in \a x
         * is also set in \a y.
         *
         * This operator generates all of the usual comparison operators,
         * including `<`, `<=`, `>`, and `>=`.
         *
         * \note This does not compare bitmasks lexicographically.
         * For lexicographical comparison, use lessThan() instead.
         *
         * \python This spaceship operator `x <=> y` is not available, but the
         * other comparison operators that it generates _are_ available.
         *
         * \param rhs the bitmask to compare against this.
         * \return The result of the subset comparison between this and the
         * given bitmask.
         */
        std::partial_ordering operator <=> (const Bitmask2& rhs) const {
            auto t = BitManipulator<T>::subsetComparison(low, rhs.low);

            if (t == std::partial_ordering::equivalent) {
                return BitManipulator<U>::subsetComparison(high, rhs.high);
            } else if (t == std::partial_ordering::less) {
                auto u = BitManipulator<U>::subsetComparison(high, rhs.high);
                if (u == std::partial_ordering::equivalent ||
                        u == std::partial_ordering::less)
                    return t; // less
                else
                    return std::partial_ordering::unordered;
            } else if (t == std::partial_ordering::greater) {
                auto u = BitManipulator<U>::subsetComparison(high, rhs.high);
                if (u == std::partial_ordering::equivalent ||
                        u == std::partial_ordering::greater)
                    return t; // greater
                else
                    return std::partial_ordering::unordered;
            } else {
                return t; // unordered
            }
        }

        /**
         * Determines whether this bitmask is entirely contained within
         * the union of the two given bitmasks.
         *
         * For this routine to return \c true, every bit that is set
         * in this bitmask must also be set in either \a x or \a y.
         *
         * \param x the first bitmask used to form the union.
         * \param y the first bitmask used to form the union.
         * \return \c true if and only if this bitmask is entirely contained
         * within the union of \a x and \a y.
         */
        inline bool inUnion(const Bitmask2& x, const Bitmask2& y) const {
            return ((low & (x.low | y.low)) == low &&
                (high & (x.high | y.high)) == high);
        }

        /**
         * Determines whether this bitmask contains the intersection of
         * the two given bitmasks.
         *
         * For this routine to return \c true, every bit that is set in
         * _both_ \a x and \a y must be set in this bitmask also.
         *
         * \param x the first bitmask used to form the intersection.
         * \param y the first bitmask used to form the intersection.
         * \return \c true if and only if this bitmask entirely contains
         * the intersection of \a x and \a y.
         */
        inline bool containsIntn(const Bitmask2& x, const Bitmask2& y) const {
            return ((low | (x.low & y.low)) == low &&
                (high | (x.high & y.high)) == high);
        }

        /**
         * Returns the number of bits currently set to \c true in this
         * bitmask.
         *
         * \return the number of \c true bits.
         */
        inline size_t bits() const {
            return BitManipulator<T>::bits(low) + BitManipulator<U>::bits(high);
        }

        /**
         * Returns the index of the first \c true bit in this bitmask,
         * or -1 if there are no \c true bits.
         *
         * \return the index of the first \c true bit.
         */
        inline ssize_t firstBit() const {
            // -1 case does not work out of the box in the second IF branch
            // due to the 8 * sizeof(T).
            if (low)
                return BitManipulator<T>::firstBit(low);
            else if (high)
                return 8 * sizeof(T) + BitManipulator<U>::firstBit(high);
            else
                return -1;
        }

        /**
         * Returns the index of the last \c true bit in this bitmask,
         * or -1 if there are no \c true bits.
         *
         * \return the index of the last \c true bit.
         */
        inline ssize_t lastBit() const {
            // -1 case works out of the box in the second IF branch.
            if (high)
                return 8 * sizeof(T) + BitManipulator<U>::lastBit(high);
            else
                return BitManipulator<T>::lastBit(low);
        }

        /**
         * Determines whether at most one bit is set to \c true in this
         * bitmask.
         *
         * If this bitmask is entirely \c false or if only one bit is set
         * to \c true, then this routine will return \c true.  Otherwise
         * this routine will return \c false.
         *
         * \return \c true if and only if at most one bit is set to \c true.
         */
        inline bool atMostOneBit() const {
            return (BitManipulator<T>::bits(low) +
                BitManipulator<U>::bits(high)) <= 1;
        }

#ifndef __DOXYGEN
    // Doxygen gets confused by the "<< <" combination here.
    friend std::ostream& operator << <T, U>(std::ostream& out,
        const Bitmask2& mask);
#endif
};

/**
 * Performs some action using an optimised bitmask type that can hold the
 * given number of bits.
 *
 * The reason for using this routine (as opposed to just performing your
 * action using the general Bitmask type) is that, if \a bits is small, this
 * routine will use one of the optimised bitmask types Bitmask1 or Bitmask2.
 *
 * The action should be a templated callable type (e.g., a generic lambda),
 * whose template type parameter adheres to the concept ReginaBitmask.
 * Any arguments that are supplied via \a args will be forwarded through as
 * arguments to \a action, and any return value from \a action will be ignored.
 *
 * As an example:
 *
 * \code
 * usingBitmaskFor(nBits, []<ReginaBitmask BitmaskType>() {
 *     BitmaskType bitmask;
 *     ...
 * });
 * \endcode
 *
 * \nopython
 *
 * \param bits the number of bits that the chosen bitmask type must support.
 * \param action the action to perform using the best possible bitmask type.
 * \param args any arguments that should be forwarded to \a action.
 *
 * \ingroup utilities
 */
template <typename Action, typename... Args>
void usingBitmaskFor(size_t bits, Action&& action, Args&&... args) {
    // Note: The C++ standard says (long long) must be at least 64-bit.
    if (bits <= 8 * sizeof(unsigned))
        action.template operator()<Bitmask1<unsigned>>(
            std::forward<Args>(args)...);
    else if (bits <= 8 * sizeof(unsigned long))
        action.template operator()<Bitmask1<unsigned long>>(
            std::forward<Args>(args)...);
    else if (bits <= 8 * sizeof(unsigned long long))
        action.template operator()<Bitmask1<unsigned long long>>(
            std::forward<Args>(args)...);
#ifdef INT128_AVAILABLE
    else if (bits <= 8 * sizeof(regina::UInt128))
        action.template operator()<Bitmask1<regina::UInt128>>(
            std::forward<Args>(args)...);
    else if (bits <= 16 * sizeof(regina::UInt128))
        action.template operator()<Bitmask2<regina::UInt128>>(
            std::forward<Args>(args)...);
#else
    else if (bits <= 16 * sizeof(unsigned long long))
        action.template operator()<Bitmask2<unsigned long long>>(
            std::forward<Args>(args)...);
#endif
    else
        action.template operator()<Bitmask>(std::forward<Args>(args)...);
}

// Inline functions for Bitmask

inline Bitmask::Bitmask() : pieces(0), mask(nullptr) {
}

inline Bitmask::Bitmask(size_t length) :
        pieces((length - 1) / (8 * sizeof(Piece)) + 1),
        mask(new Piece[pieces]) {
    std::fill(mask, mask + pieces, 0);
}

inline Bitmask::Bitmask(const Bitmask& src) :
        pieces(src.pieces), mask(new Piece[src.pieces]) {
    std::copy(src.mask, src.mask + pieces, mask);
}

inline Bitmask::Bitmask(Bitmask&& src) noexcept :
        pieces(src.pieces), mask(src.mask) {
    src.mask = nullptr;
}

inline Bitmask::~Bitmask() {
    delete[] mask;
}

inline void Bitmask::reset() {
    std::fill(mask, mask + pieces, 0);
}

inline void Bitmask::reset(size_t length) {
    delete[] mask;

    pieces = (length - 1) / (8 * sizeof(Piece)) + 1;
    mask = new Piece[pieces];

    std::fill(mask, mask + pieces, 0);
}

inline Bitmask& Bitmask::operator = (const Bitmask& other) {
    // std::copy exhibits undefined behaviour in the case of self-assignment.
    if (std::addressof(other) == this)
        return *this;

    if (pieces != other.pieces) {
        delete[] mask;
        pieces = other.pieces;
        mask = new Piece[pieces];
    }
    if (pieces)
        std::copy(other.mask, other.mask + pieces, mask);
    return *this;
}

inline Bitmask& Bitmask::operator = (Bitmask&& src) noexcept {
    pieces = src.pieces;
    std::swap(mask, src.mask);
    // Let src dispose of the original contents in its own destructor.
    return *this;
}

inline void Bitmask::swap(Bitmask& other) noexcept {
    std::swap(pieces, other.pieces);
    std::swap(mask, other.mask);
}

inline void Bitmask::truncate(size_t numBits) {
    size_t skip = numBits / (8 * sizeof(Piece));
    numBits = numBits % (8 * sizeof(Piece));

    Piece* piece = mask + skip;
    if (piece < mask + pieces) {
        (*piece) &= ((Piece(1) << numBits) - Piece(1));
        for (++piece; piece < mask + pieces; ++piece)
            *piece = 0;
    }
}

inline bool Bitmask::get(size_t index) const {
    return (mask[index / (8 * sizeof(Piece))] &
        (Piece(1) << (index % (8 * sizeof(Piece)))));
}

inline void Bitmask::set(size_t index, bool value) {
    mask[index / (8 * sizeof(Piece))] |=
        (Piece(1) << (index % (8 * sizeof(Piece))));
    if (! value)
        mask[index / (8 * sizeof(Piece))] ^=
            (Piece(1) << (index % (8 * sizeof(Piece))));
}

inline Bitmask& Bitmask::operator &= (const Bitmask& other) {
    for (size_t i = 0; i < pieces; ++i)
        mask[i] &= other.mask[i];
    return *this;
}

inline Bitmask& Bitmask::operator |= (const Bitmask& other) {
    for (size_t i = 0; i < pieces; ++i)
        mask[i] |= other.mask[i];
    return *this;
}

inline Bitmask& Bitmask::operator ^= (const Bitmask& other) {
    for (size_t i = 0; i < pieces; ++i)
        mask[i] ^= other.mask[i];
    return *this;
}

inline Bitmask& Bitmask::operator -= (const Bitmask& other) {
    for (size_t i = 0; i < pieces; ++i) {
        mask[i] |= other.mask[i];
        mask[i] ^= other.mask[i];
    }
    return *this;
}

inline void Bitmask::flip() {
    for (size_t i = 0; i < pieces; ++i)
        mask[i] = ~mask[i];
}

inline bool Bitmask::operator == (const Bitmask& other) const {
    return std::equal(mask, mask + pieces,
        other.mask, other.mask + other.pieces);
}

inline bool Bitmask::lessThan(const Bitmask& other) const {
    for (ssize_t i = pieces - 1; i >= 0; --i)
        if (mask[i] < other.mask[i])
            return true;
        else if (mask[i] > other.mask[i])
            return false;
    return false;
}

inline std::partial_ordering Bitmask::operator <=> (const Bitmask& rhs) const {
    auto ans = std::partial_ordering::equivalent;

    for (size_t i = 0; i < pieces; ++i) {
        // INV: ans is equivalent, less, or greater (not unordered).
        auto next = BitManipulator<Piece>::subsetComparison(
            mask[i], rhs.mask[i]);
        if (next == std::partial_ordering::unordered)
            return next;

        if (ans == std::partial_ordering::equivalent) {
            ans = next;
        } else if (ans == std::partial_ordering::less) {
            if (next == std::partial_ordering::greater)
                return std::partial_ordering::unordered;
            // ans stays as less
        } else { // ans == greater
            if (next == std::partial_ordering::less)
                return std::partial_ordering::unordered;
            // ans stays as greater
        }
    }

    return ans;
}

inline bool Bitmask::inUnion(const Bitmask& x, const Bitmask& y) const {
    for (size_t i = 0; i < pieces; ++i)
        if ((mask[i] & (x.mask[i] | y.mask[i])) != mask[i])
            return false;
    return true;
}

inline bool Bitmask::containsIntn(const Bitmask& x, const Bitmask& y) const {
    for (size_t i = 0; i < pieces; ++i)
        if ((mask[i] | (x.mask[i] & y.mask[i])) != mask[i])
            return false;
    return true;
}

inline size_t Bitmask::bits() const {
    size_t ans = 0;
    for (size_t i = 0; i < pieces; ++i)
        ans += BitManipulator<Piece>::bits(mask[i]);
    return ans;
}

inline ssize_t Bitmask::firstBit() const {
    for (size_t i = 0; i < pieces; ++i)
        if (mask[i])
            return 8 * sizeof(Piece) * i +
                BitManipulator<Piece>::firstBit(mask[i]);
    return -1;
}

inline ssize_t Bitmask::lastBit() const {
    for (ssize_t i = pieces - 1; i >= 0; --i)
        if (mask[i])
            return 8 * sizeof(Piece) * i +
                BitManipulator<Piece>::lastBit(mask[i]);
    return -1;
}

inline bool Bitmask::atMostOneBit() const {
    unsigned bits = 0;
    for (size_t i = 0; i < pieces; ++i) {
        bits += BitManipulator<Piece>::bits(mask[i]);
        if (bits > 1)
            return false;
    }
    return true;
}

inline void swap(Bitmask& a, Bitmask& b) noexcept {
    a.swap(b);
}

inline std::ostream& operator << (std::ostream& out, const Bitmask& mask) {
    Bitmask::Piece bit;
    for (size_t i = 0; i < mask.pieces; ++i)
        for (bit = 1; bit; bit <<= 1)
            out << ((bit & mask.mask[i]) ? '1' : '0');
    return out;
}

} // namespace regina

#endif

