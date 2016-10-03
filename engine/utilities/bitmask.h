
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

/*! \file utilities/bitmask.h
 *  \brief Provides optimised bitmasks of arbitrary length.
 */

#ifndef __BITMASK_H
#ifndef __DOXYGEN
#define __BITMASK_H
#endif

#include <algorithm>
#include <iostream>

#include "regina-core.h"
#include "regina-config.h"
#include "utilities/bitmanip.h"

namespace regina {

/**
 * \weakgroup utilities
 * @{
 */

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
 * \todo \opt Insist that sizeof(Piece) is a power of two, and replace
 * expensive division/mod operations with cheap bit operations.
 *
 * \warning Because this class may increase the length of the bitmask
 * (rounding up to the next unit of storage), bitwise computations may
 * not give the results that you expect.  In particular, flip() may set
 * additional \c true bits in the "dead space" between the intended length
 * and the actual length, and this may have a flow-on effect for other
 * operations (such as subset testing, bit counting and so on).  Be
 * careful!
 *
 * \ifacespython Not present.
 */
class REGINA_API Bitmask {
    private:
        typedef unsigned Piece;
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
         * @param length the number of bits stored in this bitmask; this must
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
         * @param cloneMe the bitmask to clone.
         */
        Bitmask(const Bitmask& cloneMe);

        /**
         * Destroys this bitmask.
         */
        ~Bitmask();

        /**
         * Returns the value of the given bit of this bitmask.
         *
         * @param index indicates which bit to query; this must be at least
         * zero and strictly less than the length of this bitmask.
         * @return the value of the (\a index)th bit.
         */
        bool get(size_t index) const;

        /**
         * Sets the given bit of this bitmask to the given value.
         *
         * @param index indicates which bit to set; this must be at least zero
         * and strictly less than the length of this bitmask.
         * @param value the value that will be assigned to the (\a index)th bit.
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
         * \pre \a ForwardIterator is a forward iterator type that iterates
         * over integer values.
         * \pre The list of indices described by these iterators is
         * in \e sorted order.  This is to allow optimisations for
         * larger bitmask types.
         * \pre All indices in the given list are at least zero and
         * strictly less than the length of this bitmask.
         *
         * @param indexBegin the beginning of the iterator range
         * containing the sorted indices of the bits to set.
         * @param indexEnd the end of the iterator range containing the
         * sorted indices of the bits to set.
         * @param value the value that will be assigned to each of the
         * corresponding bits.
         */
        template <typename ForwardIterator>
        void set(ForwardIterator indexBegin, ForwardIterator indexEnd,
                bool value) {
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
         * @param length the number of bits to store in this bitmask; this
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
         * @param other the bitmask to clone.
         * @return a reference to this bitmask.
         */
        Bitmask& operator = (const Bitmask& other);

        /**
         * Leaves the first \a numBits bits of this bitmask intact, but
         * sets all subsequent bits to \c false.  In other words, this
         * routine "truncates" this bitmask to the given number of bits.
         *
         * This routine does not change the \e length of this bitmask
         * (as passed to the contructor or to reset()).
         *
         * \pre \a numBits is at most the length of this bitmask.
         *
         * @param numBits the number of bits that will \e not be cleared.
         */
        void truncate(size_t numBits);

        /**
         * Sets this to the intersection of this and the given bitmask.
         * Every bit that is unset in \a other will be unset in this bitmask.
         *
         * \pre This and the given bitmask have the same length.
         *
         * @param other the bitmask to intersect with this.
         * @return a reference to this bitmask.
         */
        Bitmask& operator &= (const Bitmask& other);

        /**
         * Sets this to the union of this and the given bitmask.
         * Every bit that is set in \a other will be set in this bitmask.
         *
         * \pre This and the given bitmask have the same length.
         *
         * @param other the bitmask to union with this.
         * @return a reference to this bitmask.
         */
        Bitmask& operator |= (const Bitmask& other);

        /**
         * Sets this to the exclusive disjunction (XOR) of this and the
         * given bitmask.  Every bit that is set in \a other will be
         * flipped in this bitmask.
         *
         * \pre This and the given bitmask have the same length.
         *
         * @param other the bitmask to XOR with this.
         * @return a reference to this bitmask.
         */
        Bitmask& operator ^= (const Bitmask& other);

        /**
         * Sets this to the set difference of this and the given bitmask.
         * Every bit that is set in \a other will be cleared in this bitmask.
         *
         * \pre This and the given bitmask have the same length.
         *
         * @param other the bitmask to XOR with this.
         * @return a reference to this bitmask.
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
         * \pre This and the given bitmask have the same length.
         *
         * @param other the bitmask to compare against this.
         * @return \c true if and only if this and the given bitmask are
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
         * \warning We do not use &lt; for this operation, since &lt;=
         * represents the subset operation.
         *
         * @param other the bitmask to compare against this.
         * @return \c true if and only if this is lexicographically
         * strictly smaller than the given bitmask.
         */
        bool lessThan(const Bitmask& other) const;

        /**
         * Determines whether this bitmask is entirely contained within
         * the given bitmask.
         *
         * For this routine to return \c true, every bit that is set
         * in this bitmask must also be set in the given bitmask.
         *
         * \pre This and the given bitmask have the same length.
         *
         * \warning This operation does not compare bitmasks
         * lexicographically; moreover, it only describes a partial
         * order, not a total order.  To compare bitmasks
         * lexicographically, use lessThan() instead.
         *
         * @param other the bitmask to compare against this.
         * @return \c true if and only if this bitmask is entirely contained
         * within the given bitmask.
         */
        bool operator <= (const Bitmask& other) const;

        /**
         * Determines whether this bitmask is entirely contained within
         * the union of the two given bitmasks.
         *
         * For this routine to return \c true, every bit that is set
         * in this bitmask must also be set in either \a x or \a y.
         *
         * \pre Both \a x and \a y are the same length as this bitmask.
         *
         * @param x the first bitmask used to form the union.
         * @param y the first bitmask used to form the union.
         * @return \c true if and only if this bitmask is entirely contained
         * within the union of \a x and \a y.
         */
        bool inUnion(const Bitmask& x, const Bitmask& y) const;

        /**
         * Determines whether this bitmask contains the intersection of
         * the two given bitmasks.
         *
         * For this routine to return \c true, every bit that is set in
         * \e both \a x and \a y must be set in this bitmask also.
         *
         * \pre Both \a x and \a y are the same length as this bitmask.
         *
         * @param x the first bitmask used to form the intersection.
         * @param y the first bitmask used to form the intersection.
         * @return \c true if and only if this bitmask entirely contains
         * the intersection of \a x and \a y.
         */
        bool containsIntn(const Bitmask& x, const Bitmask& y) const;

        /**
         * Returns the number of bits currently set to \c true in this
         * bitmask.
         *
         * @return the number of \c true bits.
         */
        size_t bits() const;

        /**
         * Returns the index of the first \c true bit in this bitmask,
         * or -1 if there are no \c true bits.
         *
         * @return the index of the first \c true bit.
         */
        long firstBit() const;

        /**
         * Returns the index of the last \c true bit in this bitmask,
         * or -1 if there are no \c true bits.
         *
         * @return the index of the last \c true bit.
         */
        long lastBit() const;

        /**
         * Determines whether at most one bit is set to \c true in this
         * bitmask.
         *
         * If this bitmask is entirely \c false or if only one bit is set
         * to \c true, then this routine will return \c true.  Otherwise
         * this routine will return \c false.
         *
         * @return \c true if and only if at most one bit is set to \c true.
         */
        bool atMostOneBit() const;

    friend std::ostream& operator << (std::ostream& out, const Bitmask& mask);
};

/**
 * Writes the given bitmask to the given output stream as a sequence of
 * zeroes and ones.
 *
 * Since the length of the bitmask is not stored, the number of bits
 * written might be greater than the length initially assigned to this
 * bitmask (specifically, the length will be rounded up to the next "raw
 * unit of storage").
 *
 * \ifacespython Not present.
 *
 * @param out the output stream to which to write.
 * @param mask the bitmask to write.
 * @return a reference to the given output stream.
 */
REGINA_API std::ostream& operator << (std::ostream& out, const Bitmask& mask);

template <typename T>
class Bitmask1;

/**
 * Writes the given bitmask to the given output stream as a sequence of
 * zeroes and ones.
 *
 * Since the length of the bitmask is not stored, the number of bits
 * written will be 8 * sizeof(\a T).
 *
 * \ifacespython Not present.
 *
 * @param out the output stream to which to write.
 * @param mask the bitmask to write.
 * @return a reference to the given output stream.
 */
template <typename T>
std::ostream& operator << (std::ostream& out, const Bitmask1<T>& mask) {
    for (T bit = 1; bit; bit <<= 1)
        out << ((mask.mask & bit) ? '1' : '0');
    return out;
}

/**
 * A small but extremely fast bitmask class that can store up to
 * 8 * sizeof(\a T) true-or-false bits.
 *
 * This bitmask packs all of the bits together into a single variable of
 * type \a T.  This means that operations on bitmasks are extremely
 * fast, because all of the bits can be processed at once.
 *
 * The downside of course is that the number of bits that can be stored
 * is limited to 8 * sizeof(\a T), where \a T must be a native unsigned
 * integer type (such as unsigned char, unsigned int, or unsigned long
 * long).
 *
 * For another extremely fast bitmask class that can store twice as
 * many bits, see Bitmask2.  For a bitmask class that can store
 * arbitrarily many bits, see Bitmask.
 *
 * \pre Type \a T is an unsigned integral numeric type.
 *
 * \ifacespython Not present.
 */
template <typename T>
class Bitmask1 {
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
         * \warning This is \e not a constructor that initialises the
         * bitmask to a given pattern.
         */
        inline Bitmask1(size_t) : mask(0) {
        }

        /**
         * Creates a clone of the given bitmask.
         *
         * @param cloneMe the bitmask to clone.
         */
        inline Bitmask1(const Bitmask1<T>& cloneMe) : mask(cloneMe.mask) {
        }

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
         * @param other the bitmask to clone.
         * @return a reference to this bitmask.
         */
        Bitmask1<T>& operator = (const Bitmask1<T>& other) {
            mask = other.mask;
            return *this;
        }

        /**
         * Leaves the first \a numBits bits of this bitmask intact, but
         * sets all subsequent bits to \c false.  In other words, this
         * routine "truncates" this bitmask to the given number of bits.
         *
         * This routine does not change the \e length of this bitmask
         * (as passed to the contructor or to reset()).
         *
         * @param numBits the number of bits that will \e not be cleared.
         */
        inline void truncate(size_t numBits) {
            if (numBits < 8 * sizeof(T))
                mask &= ((T(1) << numBits) - T(1));
        }

        /**
         * Returns the value of the given bit of this bitmask.
         *
         * @param index indicates which bit to query; this must be between
         * 0 and (8 * sizeof(\a T) - 1) inclusive.
         * @return the value of the (\a index)th bit.
         */
        inline bool get(size_t index) const {
            return (mask & (T(1) << index));
        }

        /**
         * Sets the given bit of this bitmask to the given value.
         *
         * @param index indicates which bit to set; this must be between
         * 0 and (8 * sizeof(\a T) - 1) inclusive.
         * @param value the value that will be assigned to the (\a index)th bit.
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
         * \pre \a ForwardIterator is a forward iterator type that iterates
         * over integer values.
         * \pre The list of indices described by these iterators is
         * in \e sorted order.  This is to allow optimisations for
         * larger bitmask types.
         * \pre All indices in the given list are between
         * 0 and (8 * sizeof(\a T) - 1) inclusive.
         *
         * @param indexBegin the beginning of the iterator range
         * containing the sorted indices of the bits to set.
         * @param indexEnd the end of the iterator range containing the
         * sorted indices of the bits to set.
         * @param value the value that will be assigned to each of the
         * corresponding bits.
         */
        template <typename ForwardIterator>
        void set(ForwardIterator indexBegin, ForwardIterator indexEnd,
                bool value) {
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
         * @param other the bitmask to intersect with this.
         * @return a reference to this bitmask.
         */
        inline Bitmask1<T>& operator &= (const Bitmask1<T>& other) {
            mask &= other.mask;
            return *this;
        }

        /**
         * Sets this to the union of this and the given bitmask.
         * Every bit that is set in \a other will be set in this bitmask.
         *
         * @param other the bitmask to union with this.
         * @return a reference to this bitmask.
         */
        inline Bitmask1<T>& operator |= (const Bitmask1<T>& other) {
            mask |= other.mask;
            return *this;
        }

        /**
         * Sets this to the exclusive disjunction (XOR) of this and the
         * given bitmask.  Every bit that is set in \a other will be
         * flipped in this bitmask.
         *
         * @param other the bitmask to XOR with this.
         * @return a reference to this bitmask.
         */
        inline Bitmask1<T>& operator ^= (const Bitmask1<T>& other) {
            mask ^= other.mask;
            return *this;
        }

        /**
         * Sets this to the set difference of this and the given bitmask.
         * Every bit that is set in \a other will be cleared in this bitmask.
         *
         * @param other the bitmask to XOR with this.
         * @return a reference to this bitmask.
         */
        inline Bitmask1<T>& operator -= (const Bitmask1<T>& other) {
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
         * @param other the bitmask to compare against this.
         * @return \c true if and only if this and the given bitmask are
         * identical.
         */
        inline bool operator == (const Bitmask1<T>& other) const {
            return (mask == other.mask);
        }

        /**
         * Determines whether this bitmask appears strictly before the given
         * bitmask when bitmasks are sorted in lexicographical order.
         * Here the bit at index 0 is least significant, and the bit at
         * index \a length-1 is most significant.
         *
         * \warning We do not use &lt; for this operation, since &lt;=
         * represents the subset operation.
         *
         * @param other the bitmask to compare against this.
         * @return \c true if and only if this is lexicographically
         * strictly smaller than the given bitmask.
         */
        inline bool lessThan(const Bitmask1<T>& other) const {
            return (mask < other.mask);
        }

        /**
         * Determines whether this bitmask is entirely contained within
         * the given bitmask.
         *
         * For this routine to return \c true, every bit that is set
         * in this bitmask must also be set in the given bitmask.
         *
         * \warning This operation does not compare bitmasks
         * lexicographically; moreover, it only describes a partial
         * order, not a total order.  To compare bitmasks
         * lexicographically, use lessThan() instead.
         *
         * @param other the bitmask to compare against this.
         * @return \c true if and only if this bitmask is entirely contained
         * within the given bitmask.
         */
        inline bool operator <= (const Bitmask1<T>& other) const {
            return ((mask | other.mask) == other.mask);
        }

        /**
         * Determines whether this bitmask is entirely contained within
         * the union of the two given bitmasks.
         *
         * For this routine to return \c true, every bit that is set
         * in this bitmask must also be set in either \a x or \a y.
         *
         * @param x the first bitmask used to form the union.
         * @param y the first bitmask used to form the union.
         * @return \c true if and only if this bitmask is entirely contained
         * within the union of \a x and \a y.
         */
        inline bool inUnion(const Bitmask1<T>& x, const Bitmask1<T>& y)
                const {
            return ((mask & (x.mask | y.mask)) == mask);
        }

        /**
         * Determines whether this bitmask contains the intersection of
         * the two given bitmasks.
         *
         * For this routine to return \c true, every bit that is set in
         * \e both \a x and \a y must be set in this bitmask also.
         *
         * @param x the first bitmask used to form the intersection.
         * @param y the first bitmask used to form the intersection.
         * @return \c true if and only if this bitmask entirely contains
         * the intersection of \a x and \a y.
         */
        inline bool containsIntn(const Bitmask1<T>& x, const Bitmask1<T>& y)
                const {
            return ((mask | (x.mask & y.mask)) == mask);
        }

        /**
         * Returns the number of bits currently set to \c true in this
         * bitmask.
         *
         * @return the number of \c true bits.
         */
        inline size_t bits() const {
            return BitManipulator<T>::bits(mask);
        }

        /**
         * Returns the index of the first \c true bit in this bitmask,
         * or -1 if there are no \c true bits.
         *
         * @return the index of the first \c true bit.
         */
        inline long firstBit() const {
            return BitManipulator<T>::firstBit(mask);
        }

        /**
         * Returns the index of the last \c true bit in this bitmask,
         * or -1 if there are no \c true bits.
         *
         * @return the index of the last \c true bit.
         */
        inline long lastBit() const {
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
         * @return \c true if and only if at most one bit is set to \c true.
         */
        inline bool atMostOneBit() const {
            return BitManipulator<T>::bits(mask) <= 1;
        }

    friend std::ostream& operator << <T>(std::ostream& out,
        const Bitmask1<T>& mask);
};

template <typename T, typename U>
class Bitmask2;

/**
 * Writes the given bitmask to the given output stream as a sequence of
 * zeroes and ones.
 *
 * Since the length of the bitmask is not stored, the number of bits
 * written will be 8 * sizeof(\a T) + 8 * sizeof(\a U).
 *
 * \ifacespython Not present.
 *
 * @param out the output stream to which to write.
 * @param mask the bitmask to write.
 * @return a reference to the given output stream.
 */
template <typename T, typename U>
std::ostream& operator << (std::ostream& out, const Bitmask2<T, U>& mask) {
    for (T bit = 1; bit; bit <<= 1)
        out << ((mask.low & bit) ? '1' : '0');
    for (U bit = 1; bit; bit <<= 1)
        out << ((mask.high & bit) ? '1' : '0');
    return out;
}

/**
 * A small but extremely fast bitmask class that can store up to
 * 8 * sizeof(\a T) + 8 * sizeof(\a U) true-or-false bits.
 *
 * This bitmask packs all of the bits together into a single variable of
 * type \a T and a single variable of type \a U.  This means that operations
 * on entire bitmasks are extremely fast, because all of the bits can be
 * processed in just two "native" operations.
 *
 * The downside of course is that the number of bits that can be stored
 * is limited to 8 * sizeof(\a T) + 8 * sizeof(\a U), where \a T and \a U
 * must be native unsigned integer types (such as unsigned char, unsigned int,
 * or unsigned long long).
 *
 * For an even faster bitmask class that can only store half as many bits,
 * see Bitmask1.  For a bitmask class that can store arbitrarily many bits,
 * see Bitmask.
 *
 * \pre Types \a T and \a U are unsigned integral numeric types.
 *
 * \ifacespython Not present.
 */
template <typename T, typename U = T>
class Bitmask2 {
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
         * \warning This is \e not a constructor that initialises the
         * bitmask to a given pattern.
         */
        inline Bitmask2(size_t) : low(0), high(0) {
        }

        /**
         * Creates a clone of the given bitmask.
         *
         * @param cloneMe the bitmask to clone.
         */
        inline Bitmask2(const Bitmask2<T, U>& cloneMe) :
                low(cloneMe.low), high(cloneMe.high) {
        }

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
         * @param other the bitmask to clone.
         * @return a reference to this bitmask.
         */
        Bitmask2<T, U>& operator = (const Bitmask2<T, U>& other) {
            low = other.low;
            high = other.high;
            return *this;
        }

        /**
         * Leaves the first \a numBits bits of this bitmask intact, but
         * sets all subsequent bits to \c false.  In other words, this
         * routine "truncates" this bitmask to the given number of bits.
         *
         * This routine does not change the \e length of this bitmask
         * (as passed to the contructor or to reset()).
         *
         * @param numBits the number of bits that will \e not be cleared.
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
         * @param index indicates which bit to query; this must be between
         * 0 and (8 * sizeof(\a T) + 8 * sizeof(\a U) - 1) inclusive.
         * @return the value of the (\a index)th bit.
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
         * @param index indicates which bit to set; this must be between
         * 0 and (8 * sizeof(\a T) + 8 * sizeof(\a U) - 1) inclusive.
         * @param value the value that will be assigned to the (\a index)th bit.
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
         * \pre \a ForwardIterator is a forward iterator type that iterates
         * over integer values.
         * \pre The list of indices described by these iterators is
         * in \e sorted order.  This is to allow optimisations for
         * larger bitmask types.
         * \pre All indices in the given list are between
         * 0 and (8 * sizeof(\a T) + 8 * sizeof(\a U) - 1) inclusive.
         *
         * @param indexBegin the beginning of the iterator range
         * containing the sorted indices of the bits to set.
         * @param indexEnd the end of the iterator range containing the
         * sorted indices of the bits to set.
         * @param value the value that will be assigned to each of the
         * corresponding bits.
         */
        template <typename ForwardIterator>
        void set(ForwardIterator indexBegin, ForwardIterator indexEnd,
                bool value) {
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
         * @param other the bitmask to intersect with this.
         * @return a reference to this bitmask.
         */
        inline Bitmask2<T, U>& operator &= (const Bitmask2<T, U>& other) {
            low &= other.low;
            high &= other.high;
            return *this;
        }

        /**
         * Sets this to the union of this and the given bitmask.
         * Every bit that is set in \a other will be set in this bitmask.
         *
         * @param other the bitmask to union with this.
         * @return a reference to this bitmask.
         */
        inline Bitmask2<T, U>& operator |= (const Bitmask2<T, U>& other) {
            low |= other.low;
            high |= other.high;
            return *this;
        }

        /**
         * Sets this to the exclusive disjunction (XOR) of this and the
         * given bitmask.  Every bit that is set in \a other will be
         * flipped in this bitmask.
         *
         * @param other the bitmask to XOR with this.
         * @return a reference to this bitmask.
         */
        inline Bitmask2<T, U>& operator ^= (const Bitmask2<T, U>& other) {
            low ^= other.low;
            high ^= other.high;
            return *this;
        }

        /**
         * Sets this to the set difference of this and the given bitmask.
         * Every bit that is set in \a other will be cleared in this bitmask.
         *
         * @param other the bitmask to XOR with this.
         * @return a reference to this bitmask.
         */
        inline Bitmask2<T, U>& operator -= (const Bitmask2<T, U>& other) {
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
         * @param other the bitmask to compare against this.
         * @return \c true if and only if this and the given bitmask are
         * identical.
         */
        inline bool operator == (const Bitmask2<T, U>& other) const {
            return (low == other.low && high == other.high);
        }

        /**
         * Determines whether this bitmask appears strictly before the given
         * bitmask when bitmasks are sorted in lexicographical order.
         * Here the bit at index 0 is least significant, and the bit at
         * index \a length-1 is most significant.
         *
         * \warning We do not use &lt; for this operation, since &lt;=
         * represents the subset operation.
         *
         * @param other the bitmask to compare against this.
         * @return \c true if and only if this is lexicographically
         * strictly smaller than the given bitmask.
         */
        inline bool lessThan(const Bitmask2<T, U>& other) const {
            return (high < other.high ||
                (high == other.high && low < other.low));
        }

        /**
         * Determines whether this bitmask is entirely contained within
         * the given bitmask.
         *
         * For this routine to return \c true, every bit that is set
         * in this bitmask must also be set in the given bitmask.
         *
         * \warning This operation does not compare bitmasks
         * lexicographically; moreover, it only describes a partial
         * order, not a total order.  To compare bitmasks
         * lexicographically, use lessThan() instead.
         *
         * @param other the bitmask to compare against this.
         * @return \c true if and only if this bitmask is entirely contained
         * within the given bitmask.
         */
        inline bool operator <= (const Bitmask2<T, U>& other) const {
            return ((low | other.low) == other.low &&
                (high | other.high) == other.high);
        }

        /**
         * Determines whether this bitmask is entirely contained within
         * the union of the two given bitmasks.
         *
         * For this routine to return \c true, every bit that is set
         * in this bitmask must also be set in either \a x or \a y.
         *
         * @param x the first bitmask used to form the union.
         * @param y the first bitmask used to form the union.
         * @return \c true if and only if this bitmask is entirely contained
         * within the union of \a x and \a y.
         */
        inline bool inUnion(const Bitmask2<T, U>& x, const Bitmask2<T, U>& y)
                const {
            return ((low & (x.low | y.low)) == low &&
                (high & (x.high | y.high)) == high);
        }

        /**
         * Determines whether this bitmask contains the intersection of
         * the two given bitmasks.
         *
         * For this routine to return \c true, every bit that is set in
         * \e both \a x and \a y must be set in this bitmask also.
         *
         * @param x the first bitmask used to form the intersection.
         * @param y the first bitmask used to form the intersection.
         * @return \c true if and only if this bitmask entirely contains
         * the intersection of \a x and \a y.
         */
        inline bool containsIntn(const Bitmask2<T, U>& x,
                const Bitmask2<T, U>& y) const {
            return ((low | (x.low & y.low)) == low &&
                (high | (x.high & y.high)) == high);
        }

        /**
         * Returns the number of bits currently set to \c true in this
         * bitmask.
         *
         * @return the number of \c true bits.
         */
        inline size_t bits() const {
            return BitManipulator<T>::bits(low) + BitManipulator<U>::bits(high);
        }

        /**
         * Returns the index of the first \c true bit in this bitmask,
         * or -1 if there are no \c true bits.
         *
         * @return the index of the first \c true bit.
         */
        inline long firstBit() const {
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
         * @return the index of the last \c true bit.
         */
        inline long lastBit() const {
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
         * @return \c true if and only if at most one bit is set to \c true.
         */
        inline bool atMostOneBit() const {
            return (BitManipulator<T>::bits(low) +
                BitManipulator<U>::bits(high)) <= 1;
        }

    friend std::ostream& operator << <T, U>(std::ostream& out,
        const Bitmask2<T, U>& mask);
};

#ifndef __DOXYGEN
/**
 * An internal template that helps choose the correct bitmask type for
 * a given (hard-coded) number of bits.
 *
 * Please do not use this class directly, since this template is internal
 * and subject to change in future versions of Regina.  Instead please
 * use the convenience typedefs BitmaskLen8, BitmaskLen16, BitmaskLen32
 * and BitmaskLen64.
 *
 * The reason this template exists is to circumvent the fact that we cannot
 * use sizeof() in a #if statement.  The boolean argument to this template
 * should always be left as the default.
 */
template <bool IntHolds4Bytes = (sizeof(unsigned int) >= 4)>
struct InternalBitmaskLen32;

template <>
struct InternalBitmaskLen32<true> {
    typedef Bitmask1<unsigned int> Type;
};

template <>
struct InternalBitmaskLen32<false> {
    // The standard guarantees that sizeof(long) >= 4.
    typedef Bitmask1<unsigned long> Type;
};

/**
 * An internal template that helps choose the correct bitmask type for
 * a given (hard-coded) number of bits.
 *
 * Please do not use this class directly, since this template is internal
 * and subject to change in future versions of Regina.  Instead please
 * use the convenience typedefs BitmaskLen8, BitmaskLen16, BitmaskLen32
 * and BitmaskLen64.
 *
 * The reason this template exists is to circumvent the fact that we cannot
 * use sizeof() in a #if statement.  The boolean argument to this template
 * should always be left as the default.
 */
template <bool LongHolds8Bytes = (sizeof(unsigned long) >= 8)>
struct InternalBitmaskLen64;

template <>
struct InternalBitmaskLen64<true> {
    typedef Bitmask1<unsigned long> Type;
};

template <>
struct InternalBitmaskLen64<false> {
    // The C standard guarantees that sizeof(long long) >= 8.
    typedef Bitmask1<unsigned long long> Type;
};
#endif // End block for doxygen to ignore.

/**
 * A convenience typedef that gives a small and extremely fast bitmask
 * class capable of holding at least 8 true-or-false bits.
 *
 * This bitmask class is guaranteed to be an instantiation of the
 * template class Bitmask1.
 *
 * The particular instantiation is subject to change between different
 * platforms, different compilers and/or different versions of Regina.
 *
 * \ifacespython Not present.
 */
typedef Bitmask1<unsigned char> BitmaskLen8;

/**
 * A convenience typedef that gives a small and extremely fast bitmask
 * class capable of holding at least 16 true-or-false bits.
 *
 * This bitmask class is guaranteed to be an instantiation of the
 * template class Bitmask1.
 *
 * The particular instantiation is subject to change between different
 * platforms, different compilers and/or different versions of Regina.
 *
 * \ifacespython Not present.
 */
typedef Bitmask1<unsigned int> BitmaskLen16;

/**
 * A convenience typedef that gives a small and extremely fast bitmask
 * class capable of holding at least 32 true-or-false bits.
 *
 * This bitmask class is guaranteed to be an instantiation of the
 * template class Bitmask1.
 *
 * The particular instantiation is subject to change between different
 * platforms, different compilers and/or different versions of Regina.
 *
 * \ifacespython Not present.
 */
typedef InternalBitmaskLen32<>::Type BitmaskLen32;

/**
 * A convenience typedef that gives a small and extremely fast bitmask
 * class capable of holding at least 64 true-or-false bits.
 *
 * This bitmask class is guaranteed to be an instantiation of
 * \e either the template class Bitmask1 or the template class Bitmask2.
 *
 * The particular instantiation is subject to change between different
 * platforms, different compilers and/or different versions of Regina.
 *
 * \ifacespython Not present.
 */
typedef InternalBitmaskLen64<>::Type BitmaskLen64;

/*@}*/

// Inline functions for Bitmask

inline Bitmask::Bitmask() : pieces(0), mask(0) {
}

inline Bitmask::Bitmask(size_t length) :
        pieces((length - 1) / (8 * sizeof(Piece)) + 1),
        mask(new Piece[pieces]) {
    std::fill(mask, mask + pieces, 0);
}

inline Bitmask::Bitmask(const Bitmask& cloneMe) :
        pieces(cloneMe.pieces),
        mask(new Piece[cloneMe.pieces]) {
    std::copy(cloneMe.mask, cloneMe.mask + pieces, mask);
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
    if (pieces != other.pieces) {
        delete[] mask;
        pieces = other.pieces;
        mask = new Piece[pieces];
    }
    if (pieces)
        std::copy(other.mask, other.mask + pieces, mask);
    return *this;
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
    return std::equal(mask, mask + pieces, other.mask);
}

inline bool Bitmask::lessThan(const Bitmask& other) const {
    for (long i = pieces - 1; i >= 0; --i)
        if (mask[i] < other.mask[i])
            return true;
        else if (mask[i] > other.mask[i])
            return false;
    return false;
}

inline bool Bitmask::operator <= (const Bitmask& other) const {
    for (size_t i = 0; i < pieces; ++i)
        if ((mask[i] | other.mask[i]) != other.mask[i])
            return false;
    return true;
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

inline long Bitmask::firstBit() const {
    for (size_t i = 0; i < pieces; ++i)
        if (mask[i])
            return 8 * sizeof(Piece) * i +
                BitManipulator<Piece>::firstBit(mask[i]);
    return -1;
}

inline long Bitmask::lastBit() const {
    for (long i = pieces - 1; i >= 0; --i)
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

inline std::ostream& operator << (std::ostream& out, const Bitmask& mask) {
    Bitmask::Piece bit;
    for (size_t i = 0; i < mask.pieces; ++i)
        for (bit = 1; bit; bit <<= 1)
            out << ((bit & mask.mask[i]) ? '1' : '0');
    return out;
}

} // namespace regina

#endif

