
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

/*! \file utilities/tritmask.h
 *  \brief Provides optimised "ternary bitmasks" of fixed length.
 */

#ifndef __TRITMASK_H
#ifndef __DOXYGEN
#define __TRITMASK_H
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
 * A small but extremely fast "ternary bitmask" class that can store up to
 * 8 * sizeof(\a T) "trits", each equal to 0, 1 or 2.
 *
 * This tritmask packs all of the trits together into two variables of
 * type \a T.  This means that operations on tritmasks are extremely
 * fast, because all of the trits can be processed in just a few native
 * CPU operations.
 *
 * The downside of course is that the number of trits that can be stored
 * is limited to 8 * sizeof(\a T), where \a T must be a native unsigned
 * integer type (such as unsigned char, unsigned int, or unsigned long
 * long).
 *
 * For another extremely fast tritmask class that can store twice as
 * many trits, see Tritmask2.  At present there is no tritmask class
 * in Regina that can store arbitrarily many trits.
 *
 * \pre Type \a T is an unsigned integral numeric type.
 *
 * \ifacespython Not present.
 */
template <typename T>
class Tritmask1 {
    private:
        T mask1;
            /**< The ith bit of this mask indicates whether the ith trit
                 is non-zero (1 or 2). */
        T mask2;
            /**< The ith bit of this mask indicates whether the ith trit
                 is precisely 2. */

    public:
        /**
         * Creates a new tritmask with all trits set to 0.
         */
        inline Tritmask1() : mask1(0), mask2(0) {
        }

        /**
         * Creates a clone of the given tritmask.
         *
         * @param cloneMe the tritmask to clone.
         */
        inline Tritmask1(const Tritmask1<T>& cloneMe) :
                mask1(cloneMe.mask1), mask2(cloneMe.mask2) {
        }

        /**
         * Sets all trits of this tritmask to 0.
         */
        inline void reset() {
            mask1 = mask2 = 0;
        }

        /**
         * Sets this tritmask to a copy of the given tritmask.
         *
         * @param other the tritmask to clone.
         * @return a reference to this tritmask.
         */
        Tritmask1<T>& operator = (const Tritmask1<T>& other) {
            mask1 = other.mask1;
            mask2 = other.mask2;
            return *this;
        }

        /**
         * Returns the value of the given trit in this tritmask.
         *
         * @param index indicates which trit to query; this must be between
         * 0 and (8 * sizeof(\a T) - 1) inclusive.
         * @return the value of the (\a index)th trit; this will be
         * either 0, 1 or 2.
         */
        inline char get(unsigned index) const {
            return (
                (mask2 & (T(1) << index)) ? 2 :
                (mask1 & (T(1) << index)) ? 1 : 0);
        }

        /**
         * Sets the given trit of this tritmask to the given value.
         *
         * @param index indicates which trit to set; this must be between
         * 0 and (8 * sizeof(\a T) - 1) inclusive.
         * @param value the value that will be assigned to the (\a index)th
         * trit; this must be 0, 1 or 2.
         */
        inline void set(unsigned index, char value) {
            mask1 |= (T(1) << index);
            if (value == 0)
                mask1 ^= (T(1) << index);
            mask2 |= (T(1) << index);
            if (value != 2)
                mask2 ^= (T(1) << index);
        }

        /**
         * Determines whether this tritmask contains all zeroes.
         *
         * @return \c true if every trit is zero, or \c false otherwise.
         */
        inline bool empty() const {
            return ! mask1;
        }

        /**
         * Determines whether this tritmask contains at least one
         * non-zero trit.
         *
         * @return \c true if at least one trit is non-zero, or
         * \c false otherwise.
         */
        inline bool nonEmpty() const {
            return mask1;
        }

        /**
         * Determines whether this tritmask contains at least one trit
         * with value 2.
         *
         * @return \c true if at least one trit is 2, or \c false otherwise.
         */
        inline bool has2() const {
            return mask2;
        }

        /**
         * Sets this to the minimum of this and the given tritmask.
         * That is, the ith trit will be set to the minimum of the
         * ith trit in this tritmask and the ith trit in \a other.
         *
         * This is a "trit" version of boolean AND.
         *
         * @param rhs the tritmask to "min" with this.
         * @return a reference to this tritmask.
         */
        inline Tritmask1<T>& minWith(const Tritmask1<T>& rhs) {
            mask1 &= rhs.mask1;
            mask2 &= rhs.mask2;
            return *this;
        }

        /**
         * Sets this to the maximum of this and the given tritmask.
         * That is, the ith trit will be set to the maximum of the
         * ith trit in this tritmask and the ith trit in \a other.
         *
         * This is a "trit" version of boolean OR.
         *
         * @param rhs the tritmask to "max" with this.
         * @return a reference to this tritmask.
         */
        inline Tritmask1<T>& maxWith(const Tritmask1<T>& rhs) {
            mask1 |= rhs.mask1;
            mask2 |= rhs.mask2;
            return *this;
        }

        /**
         * Sets this to the sum of this and the given tritmask.
         * When adding trits, any digit greater than 2 will simply be
         * replaced with 2.  That is:
         *
         * - 2+2 = 1+2 = 1+1 = 2;
         *
         * - 0+x = x for any \a x.
         *
         * @param rhs the tritmask to add to this.
         * @return a reference to this tritmask.
         */
        inline Tritmask1<T>& operator += (const Tritmask1<T>& rhs) {
            mask2 = (mask1 & rhs.mask1) | mask2 | rhs.mask2;
            mask1 = (mask1 | rhs.mask1);
            return *this;
        }

        /**
         * Sets this to the difference of this and the given tritmask.
         * When subtracting trits, any "negative digit" will simply be
         * replaced with zero.  That is:
         *
         * - 2-0 = 2, 2-1 = 1, and 2-2 = 0;
         *
         * - 1-0 = 1, and 1-1 = 1-2 = 0;
         *
         * - 0-x = 0 for any \a x.
         *
         * @param rhs the tritmask to subtract from this.
         * @return a reference to this tritmask.
         */
        inline Tritmask1<T>& operator -= (const Tritmask1<T>& rhs) {
            mask1 = (mask1 & ~ rhs.mask1) | (mask2 & ~ rhs.mask2);
            mask2 = (mask2 & ~ rhs.mask1);
            return *this;
        }

        /**
         * Determines whether this and the given tritmask are identical.
         *
         * @param other the tritmask to compare against this.
         * @return \c true if and only if this and the given tritmask are
         * identical.
         */
        inline bool operator == (const Tritmask1<T>& other) const {
            return (mask1 == other.mask1 && mask2 == other.mask2);
        }

    friend std::ostream& operator << (std::ostream& out,
        const Tritmask1<T>& mask);
};

/**
 * Writes the given tritmask to the given output stream as a sequence of
 * digits (0, 1 and/or 2).
 *
 * Since the length of the tritmask is not stored, the number of trits
 * written will be 8 * sizeof(\a T).
 *
 * \ifacespython Not present.
 *
 * @param out the output stream to which to write.
 * @param mask the tritmask to write.
 * @return a reference to the given output stream.
 */
template <typename T>
std::ostream& operator << (std::ostream& out, const Tritmask1<T>& mask) {
    for (T bit = 1; bit; bit <<= 1)
        out << ((mask.mask2 & bit) ? '2' : (mask.mask1 & bit) ? '1' : '0');
    return out;
}

/**
 * A small but extremely fast "ternary bitmask" class that can store up to
 * 8 * sizeof(\a T) + 8 * sizeof(\a U) "trits", each equal to 0, 1 or 2.
 *
 * This tritmask packs all of the trits together into two variables of
 * type \a T and two variables of type \a U.  This means that operations
 * on entire tritmasks are extremely fast, because all of the trits can be
 * processed in just a few native CPU operations.
 *
 * The downside of course is that the number of trits that can be stored
 * is limited to 8 * sizeof(\a T) + 8 * sizeof(\a U), where \a T and \a U
 * must be native unsigned integer types (such as unsigned char, unsigned int,
 * or unsigned long long).
 *
 * For an even faster tritmask class that can only store half as many trits,
 * see Tritmask1.  At present there is no tritmask class
 * in Regina that can store arbitrarily many trits.
 *
 * \pre Types \a T and \a U are unsigned integral numeric types.
 *
 * \ifacespython Not present.
 */
template <typename T, typename U = T>
class Tritmask2 {
    private:
        T low1;
            /**< The ith bit of this mask indicates whether the ith trit
                 is non-zero (1 or 2). */
        T low2;
            /**< The ith bit of this mask indicates whether the ith trit
                 is precisely 2. */
        U high1;
            /**< The ith bit of this mask indicates whether the
                 (8 * sizeof(\a T) + i)th trit is non-zero (1 or 2). */
        U high2;
            /**< The ith bit of this mask indicates whether the
                 (8 * sizeof(\a T) + i)th trit is precisely 2. */

    public:
        /**
         * Creates a new tritmask with all trits set to 0.
         */
        inline Tritmask2() : low1(0), low2(0), high1(0), high2(0) {
        }

        /**
         * Creates a clone of the given tritmask.
         *
         * @param cloneMe the tritmask to clone.
         */
        inline Tritmask2(const Tritmask2<T, U>& cloneMe) :
                low1(cloneMe.low1), low2(cloneMe.low2),
                high1(cloneMe.high1), high2(cloneMe.high2) {
        }

        /**
         * Sets all trits of this tritmask to 0.
         */
        inline void reset() {
            low1 = low2 = 0;
            high1 = high2 = 0;
        }

        /**
         * Sets this tritmask to a copy of the given tritmask.
         *
         * @param other the tritmask to clone.
         * @return a reference to this tritmask.
         */
        Tritmask2<T, U>& operator = (const Tritmask2<T, U>& other) {
            low1 = other.low1;
            low2 = other.low2;
            high1 = other.high1;
            high2 = other.high2;
            return *this;
        }

        /**
         * Returns the value of the given trit in this tritmask.
         *
         * @param index indicates which trit to query; this must be between
         * 0 and (8 * sizeof(\a T) + 8 * sizeof(\a U) - 1) inclusive.
         * @return the value of the (\a index)th trit; this will be
         * either 0, 1 or 2.
         */
        inline char get(unsigned index) const {
            if (index < 8 * sizeof(T))
                return (
                    (low2 & (T(1) << index)) ? 2 :
                    (low1 & (T(1) << index)) ? 1 : 0);
            else
                return (
                    (high2 & (U(1) << (index - 8 * sizeof(T)))) ? 2 :
                    (high1 & (U(1) << (index - 8 * sizeof(T)))) ? 1 : 0);
        }

        /**
         * Sets the given trit of this tritmask to the given value.
         *
         * @param index indicates which trit to set; this must be between
         * 0 and (8 * sizeof(\a T) + 8 * sizeof(\a U) - 1) inclusive.
         * @param value the value that will be assigned to the (\a index)th
         * trit; this must be 0, 1 or 2.
         */
        inline void set(unsigned index, char value) {
            if (index < 8 * sizeof(T)) {
                low1 |= (T(1) << index);
                if (value == 0)
                    low1 ^= (T(1) << index);
                low2 |= (T(1) << index);
                if (value != 2)
                    low2 ^= (T(1) << index);
            } else {
                high1 |= (U(1) << (index - 8 * sizeof(T)));
                if (value == 0)
                    high1 ^= (U(1) << (index - 8 * sizeof(T)));
                high2 |= (U(1) << (index - 8 * sizeof(T)));
                if (value != 2)
                    high2 ^= (U(1) << (index - 8 * sizeof(T)));
            }
        }

        /**
         * Determines whether this tritmask contains all zeroes.
         *
         * @return \c true if every trit is zero, or \c false otherwise.
         */
        inline bool empty() const {
            return ! (low1 || high1);
        }

        /**
         * Determines whether this tritmask contains at least one
         * non-zero trit.
         *
         * @return \c true if at least one trit is non-zero, or
         * \c false otherwise.
         */
        inline bool nonEmpty() const {
            return (low1 || high1);
        }

        /**
         * Determines whether this tritmask contains at least one trit
         * with value 2.
         *
         * @return \c true if at least one trit is 2, or \c false otherwise.
         */
        inline bool has2() const {
            return (low2 || high2);
        }

        /**
         * Sets this to the minimum of this and the given tritmask.
         * That is, the ith trit will be set to the minimum of the
         * ith trit in this tritmask and the ith trit in \a other.
         *
         * This is a "trit" version of boolean AND.
         *
         * @param rhs the tritmask to "min" with this.
         * @return a reference to this tritmask.
         */
        inline Tritmask2<T, U>& minWith(const Tritmask2<T, U>& rhs) {
            low1 &= rhs.low1;
            low2 &= rhs.low2;
            high1 &= rhs.high1;
            high2 &= rhs.high2;
            return *this;
        }

        /**
         * Sets this to the maximum of this and the given tritmask.
         * That is, the ith trit will be set to the maximum of the
         * ith trit in this tritmask and the ith trit in \a other.
         *
         * This is a "trit" version of boolean OR.
         *
         * @param rhs the tritmask to "max" with this.
         * @return a reference to this tritmask.
         */
        inline Tritmask2<T, U>& maxWith(const Tritmask2<T, U>& rhs) {
            low1 |= rhs.low1;
            low2 |= rhs.low2;
            high1 |= rhs.high1;
            high2 |= rhs.high2;
            return *this;
        }

        /**
         * Sets this to the sum of this and the given tritmask.
         * When adding trits, any digit greater than 2 will simply be
         * replaced with 2.  That is:
         *
         * - 2+2 = 1+2 = 1+1 = 2;
         *
         * - 0+x = x for any \a x.
         *
         * @param rhs the tritmask to add to this.
         * @return a reference to this tritmask.
         */
        inline Tritmask2<T, U>& operator += (const Tritmask2<T, U>& rhs) {
            low2 = (low1 & rhs.low1) | low2 | rhs.low2;
            low1 = (low1 | rhs.low1);
            high2 = (high1 & rhs.high1) | high2 | rhs.high2;
            high1 = (high1 | rhs.high1);
            return *this;
        }

        /**
         * Sets this to the difference of this and the given tritmask.
         * When subtracting trits, any "negative digit" will simply be
         * replaced with zero.  That is:
         *
         * - 2-0 = 2, 2-1 = 1, and 2-2 = 0;
         *
         * - 1-0 = 1, and 1-1 = 1-2 = 0;
         *
         * - 0-x = 0 for any \a x.
         *
         * @param rhs the tritmask to subtract from this.
         * @return a reference to this tritmask.
         */
        inline Tritmask2<T, U>& operator -= (const Tritmask2<T, U>& rhs) {
            low1 = (low1 & ~ rhs.low1) | (low2 & ~ rhs.low2);
            low2 = (low2 & ~ rhs.low1);
            high1 = (high1 & ~ rhs.high1) | (high2 & ~ rhs.high2);
            high2 = (high2 & ~ rhs.high1);
            return *this;
        }

        /**
         * Determines whether this and the given tritmask are identical.
         *
         * @param other the tritmask to compare against this.
         * @return \c true if and only if this and the given tritmask are
         * identical.
         */
        inline bool operator == (const Tritmask2<T, U>& other) const {
            return (low1 == other.low1 && low2 == other.low2 &&
                high1 == other.high1 && high2 == other.high2);
        }

    friend std::ostream& operator << (std::ostream& out,
        const Tritmask2<T, U>& mask);
};

/**
 * Writes the given tritmask to the given output stream as a sequence of
 * digits (0, 1 and/or 2).
 *
 * Since the length of the tritmask is not stored, the number of trits
 * written will be 8 * sizeof(\a T) + 8 * sizeof(\a U).
 *
 * \ifacespython Not present.
 *
 * @param out the output stream to which to write.
 * @param mask the tritmask to write.
 * @return a reference to the given output stream.
 */
template <typename T, typename U>
std::ostream& operator << (std::ostream& out, const Tritmask2<T, U>& mask) {
    for (T bit = 1; bit; bit <<= 1)
        out << ((mask.low2 & bit) ? '2' : (mask.low1 & bit) ? '1' : '0');
    for (U bit = 1; bit; bit <<= 1)
        out << ((mask.high2 & bit) ? '2' : (mask.high1 & bit) ? '1' : '0');
    return out;
}

#ifndef __DOXYGEN
/**
 * An internal template that helps choose the correct tritmask type for
 * a given (hard-coded) number of bits.
 *
 * Please do not use this class directly, since this template is internal
 * and subject to change in future versions of Regina.  Instead please
 * use the convenience typedefs TritmaskLen8, TritmaskLen16, TritmaskLen32
 * and TritmaskLen64.
 *
 * The reason this template exists is to circumvent the fact that we cannot
 * use sizeof() in a #if statement.  The boolean argument to this template
 * should always be left as the default.
 */
template <bool IntHolds4Bytes = (sizeof(unsigned int) >= 4)>
struct InternalTritmaskLen32;

template <>
struct InternalTritmaskLen32<true> {
    typedef Tritmask1<unsigned int> Type;
};

template <>
struct InternalTritmaskLen32<false> {
    // The standard guarantees that sizeof(long) >= 4.
    typedef Tritmask1<unsigned long> Type;
};

/**
 * An internal template that helps choose the correct tritmask type for
 * a given (hard-coded) number of bits.
 *
 * Please do not use this class directly, since this template is internal
 * and subject to change in future versions of Regina.  Instead please
 * use the convenience typedefs TritmaskLen8, TritmaskLen16, TritmaskLen32
 * and TritmaskLen64.
 *
 * The reason this template exists is to circumvent the fact that we cannot
 * use sizeof() in a #if statement.  The boolean argument to this template
 * should always be left as the default.
 */
template <bool LongHolds8Bytes = (sizeof(unsigned long) >= 8)>
struct InternalTritmaskLen64;

template <>
struct InternalTritmaskLen64<true> {
    typedef Tritmask1<unsigned long> Type;
};

template <>
struct InternalTritmaskLen64<false> {
    // The C standard guarantees that sizeof(long long) >= 8.
    typedef Tritmask1<unsigned long long> Type;
};
#endif // End block for doxygen to ignore.

/**
 * A convenience typedef that gives a small and extremely fast tritmask
 * class capable of holding at least 8 true-or-false bits.
 *
 * This tritmask class is guaranteed to be an instantiation of the
 * template class Tritmask1.
 *
 * The particular instantiation is subject to change between different
 * platforms, different compilers and/or different versions of Regina.
 *
 * \ifacespython Not present.
 */
typedef Tritmask1<unsigned char> TritmaskLen8;

/**
 * A convenience typedef that gives a small and extremely fast tritmask
 * class capable of holding at least 16 true-or-false bits.
 *
 * This tritmask class is guaranteed to be an instantiation of the
 * template class Tritmask1.
 *
 * The particular instantiation is subject to change between different
 * platforms, different compilers and/or different versions of Regina.
 *
 * \ifacespython Not present.
 */
typedef Tritmask1<unsigned int> TritmaskLen16;

/**
 * A convenience typedef that gives a small and extremely fast tritmask
 * class capable of holding at least 32 true-or-false bits.
 *
 * This tritmask class is guaranteed to be an instantiation of the
 * template class Tritmask1.
 *
 * The particular instantiation is subject to change between different
 * platforms, different compilers and/or different versions of Regina.
 *
 * \ifacespython Not present.
 */
typedef InternalTritmaskLen32<>::Type TritmaskLen32;

/**
 * A convenience typedef that gives a small and extremely fast tritmask
 * class capable of holding at least 64 true-or-false bits.
 *
 * This tritmask class is guaranteed to be an instantiation of
 * \e either the template class Tritmask1 or the template class Tritmask2.
 *
 * The particular instantiation is subject to change between different
 * platforms, different compilers and/or different versions of Regina.
 *
 * \ifacespython Not present.
 */
typedef InternalTritmaskLen64<>::Type TritmaskLen64;

/*@}*/

} // namespace regina

#endif

