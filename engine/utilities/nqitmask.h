
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

/* end stub */

/*! \file utilities/nqitmask.h
 *  \brief Provides optimised "base 4 bitmasks" of fixed length.
 */

#ifndef __NQITMASK_H
#ifndef __DOXYGEN
#define __NQITMASK_H
#endif

#include <algorithm>
#include <iostream>

#include "regina-core.h"
#include "regina-config.h"

namespace regina {

/**
 * \weakgroup utilities
 * @{
 */

/**
 * A small but extremely fast "base 4 bitmask" class that can store up to
 * 8 * sizeof(\a T) "qits", each equal to 0, 1, 2 or 3.
 *
 * This qitmask packs all of the qits together into two variables of
 * type \a T.  This means that operations on qitmasks are extremely
 * fast, because all of the qits can be processed in just a few native
 * CPU operations.
 *
 * The downside of course is that the number of qits that can be stored
 * is limited to 8 * sizeof(\a T), where \a T must be a native unsigned
 * integer type (such as unsigned char, unsigned int, or unsigned long
 * long).
 *
 * For another extremely fast qitmask class that can store twice as
 * many qits, see NQitmask2.  At present there is no qitmask class
 * in Regina that can store arbitrarily many qits.
 *
 * \pre Type \a T is an unsigned integral numeric type.
 *
 * \testpart
 *
 * \ifacespython Not present.
 */
template <typename T>
class NQitmask1 {
    private:
        T mask1;
            /**< The ith bit of this mask contains the least significant
                 bit of the ith qit (i.e., whether the qit is 1 or 3). */
        T mask2;
            /**< The ith bit of this mask contains the most significant
                 bit of the ith qit (i.e., whether the qit is 2 or 3). */

    public:
        /**
         * Creates a new qitmask with all qits set to 0.
         */
        inline NQitmask1() : mask1(0), mask2(0) {
        }

        /**
         * Creates a clone of the given qitmask.
         *
         * @param cloneMe the qitmask to clone.
         */
        inline NQitmask1(const NQitmask1<T>& cloneMe) :
                mask1(cloneMe.mask1), mask2(cloneMe.mask2) {
        }

        /**
         * Sets all qits of this qitmask to 0.
         */
        inline void reset() {
            mask1 = mask2 = 0;
        }

        /**
         * Sets this qitmask to a copy of the given qitmask.
         *
         * @param other the qitmask to clone.
         * @return a reference to this qitmask.
         */
        NQitmask1<T>& operator = (const NQitmask1<T>& other) {
            mask1 = other.mask1;
            mask2 = other.mask2;
            return *this;
        }

        /**
         * Returns the value of the given qit in this qitmask.
         *
         * @param index indicates which qit to query; this must be between
         * 0 and (8 * sizeof(\a T) - 1) inclusive.
         * @return the value of the (\a index)th qit; this will be
         * either 0, 1, 2 or 3.
         */
        inline char get(unsigned index) const {
            T bit = T(1) << index;
            return ((mask1 & bit) ? 1 : 0) | ((mask2 & bit) ? 2 : 0);
        }

        /**
         * Sets the given qit of this qitmask to the given value.
         *
         * @param index indicates which qit to set; this must be between
         * 0 and (8 * sizeof(\a T) - 1) inclusive.
         * @param value the value that will be assigned to the (\a index)th
         * qit; this must be 0, 1, 2 or 3.
         */
        inline void set(unsigned index, char value) {
            mask1 |= (T(1) << index);
            if (! (value & 1))
                mask1 ^= (T(1) << index);
            mask2 |= (T(1) << index);
            if (! (value & 2))
                mask2 ^= (T(1) << index);
        }

        /**
         * Determines whether this qitmask contains all zeroes.
         *
         * @return \c true if every qit is zero, or \c false otherwise.
         */
        inline bool empty() const {
            return ! (mask1 || mask2);
        }

        /**
         * Determines whether this qitmask contains at least one
         * non-zero qit.
         *
         * @return \c true if at least one qit is non-zero, or
         * \c false otherwise.
         */
        inline bool nonEmpty() const {
            return (mask1 || mask2);
        }

        /**
         * Determines whether this qitmask contains at least one qit
         * with value 3.
         *
         * @return \c true if at least one qit is 3, or \c false otherwise.
         */
        inline bool has3() const {
            return (mask1 & mask2);
        }

        /**
         * Sets this to the sum of this and the given qitmask.
         * Each pair of qits is added modulo 4 (so, for instance, 2 + 3 = 1).
         *
         * @param rhs the qitmask to add to this.
         * @return a reference to this qitmask.
         */
        inline NQitmask1<T>& operator += (const NQitmask1<T>& rhs) {
            mask2 = mask2 ^ rhs.mask2 ^ (mask1 & rhs.mask1);
            mask1 = mask1 ^ rhs.mask1;
            return *this;
        }

        /**
         * Sets this to the difference of this and the given qitmask.
         * Each pair of qits is subtracted modulo 4 (so, for instance,
         * 1 - 3 = 2).
         *
         * @param rhs the qitmask to subtract from this.
         * @return a reference to this qitmask.
         */
        inline NQitmask1<T>& operator -= (const NQitmask1<T>& rhs) {
            mask2 = mask2 ^ rhs.mask2 ^ (rhs.mask1 & ~ mask1);
            mask1 = mask1 ^ rhs.mask1;
            return *this;
        }

        /**
         * Determines whether this and the given qitmask are identical.
         *
         * @param other the qitmask to compare against this.
         * @return \c true if and only if this and the given qitmask are
         * identical.
         */
        inline bool operator == (const NQitmask1<T>& other) const {
            return (mask1 == other.mask1 && mask2 == other.mask2);
        }

        /**
         * Determines whether there is some index at which both this and
         * the given qitmask both have non-zero qits.  That is, there is some
         * index \a i for which <tt>get(i)</tt> and <tt>other.get(i)</tt>
         * are both non-zero.
         *
         * Note that these two qits do not need to be equal; they
         * just both need to be non-zero.  Note also that this only
         * needs to happen at one index; there may be \e other indices at
         * which the qit is zero in one qitmask but not the other.
         *
         * @param other the qitmask to compare with this.
         * @return \c true if there is some index at which this and \a other
         * both have non-zero qits, or \c false otherwise.
         */
        inline bool hasNonZeroMatch(const NQitmask1<T>& other) const {
            return ((mask1 | mask2) & (other.mask1 | other.mask2));
        }

    template <typename X>
    friend std::ostream& operator << (std::ostream& out,
        const NQitmask1<X>& mask);
};

/**
 * Writes the given qitmask to the given output stream as a sequence of
 * digits (0, 1, 2 and/or 3).
 *
 * Since the length of the qitmask is not stored, the number of qits
 * written will be 8 * sizeof(\a T).
 *
 * \ifacespython Not present.
 *
 * @param out the output stream to which to write.
 * @param mask the qitmask to write.
 * @return a reference to the given output stream.
 */
template <typename T>
std::ostream& operator << (std::ostream& out, const NQitmask1<T>& mask) {
    for (T bit = 1; bit; bit <<= 1)
        out << int(((mask.mask1 & bit) ? 1 : 0) | ((mask.mask2 & bit) ? 2 : 0));
    return out;
}

/**
 * A small but extremely fast "base 4 bitmask" class that can store up to
 * 8 * sizeof(\a T) + 8 * sizeof(\a U) "qits", each equal to 0, 1, 2 or 3.
 *
 * This qitmask packs all of the qits together into two variables of
 * type \a T and two variables of type \a U.  This means that operations
 * on entire qitmasks are extremely fast, because all of the qits can be
 * processed in just a few native CPU operations.
 *
 * The downside of course is that the number of qits that can be stored
 * is limited to 8 * sizeof(\a T) + 8 * sizeof(\a U), where \a T and \a U
 * must be native unsigned integer types (such as unsigned char, unsigned int,
 * or unsigned long long).
 *
 * For an even faster qitmask class that can only store half as many qits,
 * see NQitmask1.  At present there is no qitmask class
 * in Regina that can store arbitrarily many qits.
 *
 * \pre Types \a T and \a U are unsigned integral numeric types.
 *
 * \testpart
 *
 * \ifacespython Not present.
 */
template <typename T, typename U = T>
class NQitmask2 {
    private:
        T low1;
            /**< The ith bit of this mask stores the least significant bit
                 of the ith qit (i.e., whether the qit is 1 or 3). */
        T low2;
            /**< The ith bit of this mask stores the most significant bit
                 of the ith qit (i.e., whether the qit is 2 or 3). */
        U high1;
            /**< The ith bit of this mask stores the least significant bit
                 of the (8 * sizeof(\a T) + i)th qit. */
        U high2;
            /**< The ith bit of this mask stores the most significant bit
                 of the (8 * sizeof(\a T) + i)th qit. */

    public:
        /**
         * Creates a new qitmask with all qits set to 0.
         */
        inline NQitmask2() : low1(0), low2(0), high1(0), high2(0) {
        }

        /**
         * Creates a clone of the given qitmask.
         *
         * @param cloneMe the qitmask to clone.
         */
        inline NQitmask2(const NQitmask2<T, U>& cloneMe) :
                low1(cloneMe.low1), low2(cloneMe.low2),
                high1(cloneMe.high1), high2(cloneMe.high2) {
        }

        /**
         * Sets all qits of this qitmask to 0.
         */
        inline void reset() {
            low1 = low2 = 0;
            high1 = high2 = 0;
        }

        /**
         * Sets this qitmask to a copy of the given qitmask.
         *
         * @param other the qitmask to clone.
         * @return a reference to this qitmask.
         */
        NQitmask2<T, U>& operator = (const NQitmask2<T, U>& other) {
            low1 = other.low1;
            low2 = other.low2;
            high1 = other.high1;
            high2 = other.high2;
            return *this;
        }

        /**
         * Returns the value of the given qit in this qitmask.
         *
         * @param index indicates which qit to query; this must be between
         * 0 and (8 * sizeof(\a T) + 8 * sizeof(\a U) - 1) inclusive.
         * @return the value of the (\a index)th qit; this will be
         * either 0, 1, 2 or 3.
         */
        inline char get(unsigned index) const {
            if (index < 8 * sizeof(T)) {
                T bit = T(1) << index;
                return ((low1 & bit) ? 1 : 0) | ((low2 & bit) ? 2 : 0);
            } else {
                U bit = U(1) << (index - 8 * sizeof(T));
                return ((high1 & bit) ? 1 : 0) | ((high2 & bit) ? 2 : 0);
            }
        }

        /**
         * Sets the given qit of this qitmask to the given value.
         *
         * @param index indicates which qit to set; this must be between
         * 0 and (8 * sizeof(\a T) + 8 * sizeof(\a U) - 1) inclusive.
         * @param value the value that will be assigned to the (\a index)th
         * qit; this must be 0, 1, 2 or 3.
         */
        inline void set(unsigned index, char value) {
            if (index < 8 * sizeof(T)) {
                low1 |= (T(1) << index);
                if (! (value & 1))
                    low1 ^= (T(1) << index);
                low2 |= (T(1) << index);
                if (! (value & 2))
                    low2 ^= (T(1) << index);
            } else {
                high1 |= (U(1) << (index - 8 * sizeof(T)));
                if (! (value & 1))
                    high1 ^= (U(1) << (index - 8 * sizeof(T)));
                high2 |= (U(1) << (index - 8 * sizeof(T)));
                if (! (value & 2))
                    high2 ^= (U(1) << (index - 8 * sizeof(T)));
            }
        }

        /**
         * Determines whether this qitmask contains all zeroes.
         *
         * @return \c true if every qit is zero, or \c false otherwise.
         */
        inline bool empty() const {
            return ! (low1 || high1 || low2 || high2);
        }

        /**
         * Determines whether this qitmask contains at least one
         * non-zero qit.
         *
         * @return \c true if at least one qit is non-zero, or
         * \c false otherwise.
         */
        inline bool nonEmpty() const {
            return (low1 || high1 || low2 || high2);
        }

        /**
         * Determines whether this qitmask contains at least one qit
         * with value 3.
         *
         * @return \c true if at least one qit is 3, or \c false otherwise.
         */
        inline bool has3() const {
            return (low1 & low2) || (high1 & high2);
        }

        /**
         * Sets this to the sum of this and the given qitmask.
         * Each pair of qits is added modulo 4 (so, for instance, 2 + 3 = 1).
         *
         * @param rhs the qitmask to add to this.
         * @return a reference to this qitmask.
         */
        inline NQitmask2<T, U>& operator += (const NQitmask2<T, U>& rhs) {
            low2 = low2 ^ rhs.low2 ^ (low1 & rhs.low1);
            low1 = low1 ^ rhs.low1;
            high2 = high2 ^ rhs.high2 ^ (high1 & rhs.high1);
            high1 = high1 ^ rhs.high1;
            return *this;
        }

        /**
         * Sets this to the difference of this and the given qitmask.
         * Each pair of qits is subtracted modulo 4 (so, for instance,
         * 1 - 3 = 2).
         *
         * @param rhs the qitmask to subtract from this.
         * @return a reference to this qitmask.
         */
        inline NQitmask2<T, U>& operator -= (const NQitmask2<T, U>& rhs) {
            low2 = low2 ^ rhs.low2 ^ (rhs.low1 & ~ low1);
            low1 = low1 ^ rhs.low1;
            high2 = high2 ^ rhs.high2 ^ (rhs.high1 & ~ high1);
            high1 = high1 ^ rhs.high1;
            return *this;
        }

        /**
         * Determines whether this and the given qitmask are identical.
         *
         * @param other the qitmask to compare against this.
         * @return \c true if and only if this and the given qitmask are
         * identical.
         */
        inline bool operator == (const NQitmask2<T, U>& other) const {
            return (low1 == other.low1 && low2 == other.low2 &&
                high1 == other.high1 && high2 == other.high2);
        }

        /**
         * Determines whether there is some index at which both this and
         * the given qitmask both have non-zero qits.  That is, there is some
         * index \a i for which <tt>get(i)</tt> and <tt>other.get(i)</tt>
         * are both non-zero.
         *
         * Note that these two qits do not need to be equal; they
         * just both need to be non-zero.  Note also that this only
         * needs to happen at one index; there may be \e other indices at
         * which the qit is zero in one qitmask but not the other.
         *
         * @param other the qitmask to compare with this.
         * @return \c true if there is some index at which this and \a other
         * both have non-zero qits, or \c false otherwise.
         */
        inline bool hasNonZeroMatch(const NQitmask2<T, U>& other) const {
            return ((low1 | low2) & (other.low1 | other.low2)) ||
                ((high1 | high2) & (other.high1 | other.high2));
        }

    template <typename X, typename Y>
    friend std::ostream& operator << (std::ostream& out,
        const NQitmask2<X, Y>& mask);
};

/**
 * Writes the given qitmask to the given output stream as a sequence of
 * digits (0, 1, 2 and/or 3).
 *
 * Since the length of the qitmask is not stored, the number of qits
 * written will be 8 * sizeof(\a T) + 8 * sizeof(\a U).
 *
 * \ifacespython Not present.
 *
 * @param out the output stream to which to write.
 * @param mask the qitmask to write.
 * @return a reference to the given output stream.
 */
template <typename T, typename U>
std::ostream& operator << (std::ostream& out, const NQitmask2<T, U>& mask) {
    for (T bit = 1; bit; bit <<= 1)
        out << int(((mask.low1 & bit) ? 1 : 0) | ((mask.low2 & bit) ? 2 : 0));
    for (U bit = 1; bit; bit <<= 1)
        out << int(((mask.high1 & bit) ? 1 : 0) | ((mask.high2 & bit) ? 2 : 0));
    return out;
}

#ifndef __DOXYGEN
/**
 * An internal template that helps choose the correct qitmask type for
 * a given (hard-coded) number of bits.
 *
 * Please do not use this class directly, since this template is internal
 * and subject to change in future versions of Regina.  Instead please
 * use the convenience typedefs NQitmaskLen8, NQitmaskLen16, NQitmaskLen32
 * and NQitmaskLen64.
 *
 * The reason this template exists is to circumvent the fact that we cannot
 * use sizeof() in a #if statement.  The boolean argument to this template
 * should always be left as the default.
 */
template <bool IntHolds4Bytes = (sizeof(unsigned int) >= 4)>
struct InternalQitmaskLen32;

template <>
struct InternalQitmaskLen32<true> {
    typedef NQitmask1<unsigned int> Type;
};

template <>
struct InternalQitmaskLen32<false> {
    // The standard guarantees that sizeof(long) >= 4.
    typedef NQitmask1<unsigned long> Type;
};

/**
 * An internal template that helps choose the correct qitmask type for
 * a given (hard-coded) number of bits.
 *
 * Please do not use this class directly, since this template is internal
 * and subject to change in future versions of Regina.  Instead please
 * use the convenience typedefs NQitmaskLen8, NQitmaskLen16, NQitmaskLen32
 * and NQitmaskLen64.
 *
 * The reason this template exists is to circumvent the fact that we cannot
 * use sizeof() in a #if statement.  The boolean argument to this template
 * should always be left as the default.
 */
template <bool LongHolds8Bytes = (sizeof(unsigned long) >= 8)>
struct InternalQitmaskLen64;

template <>
struct InternalQitmaskLen64<true> {
    typedef NQitmask1<unsigned long> Type;
};

template <>
struct InternalQitmaskLen64<false> {
#ifdef LONG_LONG_FOUND
    // The C standard guarantees that sizeof(long long) >= 8.
    // However, the C++ standard does not require long long to exist at
    // all (hence the LONG_LONG_FOUND test).
    typedef NQitmask1<unsigned long long> Type;
#else
    // The standard guarantees that sizeof(long) >= 4.
    // Therefore two longs will be enough for 64 bits.
    typedef NQitmask2<unsigned long> Type;
#endif
};
#endif // End block for doxygen to ignore.

/**
 * A convenience typedef that gives a small and extremely fast qitmask
 * class capable of holding at least 8 true-or-false bits.
 *
 * This qitmask class is guaranteed to be an instantiation of the
 * template class NQitmask1.
 *
 * The particular instantiation is subject to change between different
 * platforms, different compilers and/or different versions of Regina.
 *
 * \ifacespython Not present.
 */
typedef NQitmask1<unsigned char> NQitmaskLen8;

/**
 * A convenience typedef that gives a small and extremely fast qitmask
 * class capable of holding at least 16 true-or-false bits.
 *
 * This qitmask class is guaranteed to be an instantiation of the
 * template class NQitmask1.
 *
 * The particular instantiation is subject to change between different
 * platforms, different compilers and/or different versions of Regina.
 *
 * \ifacespython Not present.
 */
typedef NQitmask1<unsigned int> NQitmaskLen16;

/**
 * A convenience typedef that gives a small and extremely fast qitmask
 * class capable of holding at least 32 true-or-false bits.
 *
 * This qitmask class is guaranteed to be an instantiation of the
 * template class NQitmask1.
 *
 * The particular instantiation is subject to change between different
 * platforms, different compilers and/or different versions of Regina.
 *
 * \ifacespython Not present.
 */
typedef InternalQitmaskLen32<>::Type NQitmaskLen32;

/**
 * A convenience typedef that gives a small and extremely fast qitmask
 * class capable of holding at least 64 true-or-false bits.
 *
 * This qitmask class is guaranteed to be an instantiation of
 * \e either the template class NQitmask1 or the template class NQitmask2.
 *
 * The particular instantiation is subject to change between different
 * platforms, different compilers and/or different versions of Regina.
 *
 * \ifacespython Not present.
 */
typedef InternalQitmaskLen64<>::Type NQitmaskLen64;

/*@}*/

} // namespace regina

#endif

