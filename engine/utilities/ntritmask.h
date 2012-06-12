
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

/*! \file utilities/ntritmask.h
 *  \brief Provides optimised "ternary bitmasks" of fixed length.
 */

#ifndef __NTRITMASK_H
#ifndef __DOXYGEN
#define __NTRITMASK_H
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
 * many trits, see NTritmask2.  At present there is no tritmask class
 * in Regina that can store arbitrarily many trits.
 *
 * \pre Type \a T is an unsigned integral numeric type.
 *
 * \testpart
 *
 * \ifacespython Not present.
 */
template <typename T>
class NTritmask1 {
    private:
        T mask1;
            /**< The ith bit of this mask indicates whether the ith trit
                 is non-zero (1 or 2). */
        T mask2;
            /**< The ith bit of this mask indicates whether the ith trit
                 is precisely 2. */

    public:
        /**
         * Creates a new tritmask with all bits set to \c false.
         */
        inline NTritmask1() : mask(0) {
        }

        /**
         * Creates a new tritmask with all bits set to \c false.
         *
         * The integer argument is merely for compatibility with
         * the NTritmask constructor, and will be ignored.
         *
         * \warning This is \e not a constructor that initialises the
         * tritmask to a given pattern.
         */
        inline NTritmask1(unsigned) : mask(0) {
        }

        /**
         * Creates a clone of the given tritmask.
         *
         * @param cloneMe the tritmask to clone.
         */
        inline NTritmask1(const NTritmask1<T>& cloneMe) : mask(cloneMe.mask) {
        }

        /**
         * Sets all bits of this tritmask to \c false.
         */
        inline void reset() {
            mask = 0;
        }

        /**
         * Sets this tritmask to a copy of the given tritmask.
         *
         * @param other the tritmask to clone.
         * @return a reference to this tritmask.
         */
        NTritmask1<T>& operator = (const NTritmask1<T>& other) {
            mask = other.mask;
            return *this;
        }

        /**
         * Returns the value of the given bit of this tritmask.
         *
         * @param index indicates which bit to query; this must be between
         * 0 and (8 * sizeof(\a T) - 1) inclusive.
         * @return the value of the (\a index)th bit.
         */
        inline bool get(unsigned index) const {
            return (mask & (T(1) << index));
        }

        /**
         * Sets the given bit of this tritmask to the given value.
         *
         * @param index indicates which bit to set; this must be between
         * 0 and (8 * sizeof(\a T) - 1) inclusive.
         * @param value the value that will be assigned to the (\a index)th bit.
         */
        inline void set(unsigned index, bool value) {
            mask |= (T(1) << index);
            if (! value)
                mask ^= (T(1) << index);
        }

        /**
         * Sets this to the intersection of this and the given tritmask.
         * Every bit that is unset in \a other will be unset in this
         * tritmask.
         *
         * @param other the tritmask to intersect with this.
         * @return a reference to this tritmask.
         */
        inline NTritmask1<T>& operator &= (const NTritmask1<T>& other) {
            mask &= other.mask;
            return *this;
        }

        /**
         * Sets this to the union of this and the given tritmask.
         * Every bit that is set in \a other will be set in this tritmask.
         *
         * @param other the tritmask to union with this.
         * @return a reference to this tritmask.
         */
        inline NTritmask1<T>& operator |= (const NTritmask1<T>& other) {
            mask |= other.mask;
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
        inline NTritmask1<T>& operator -= (const NTritmask1<T>& rhs) {
            mask1 = (mask1 & ~ rhs.mask1) | (mask2 & ~ rhs.mask1);
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
        inline bool operator == (const NTritmask1<T>& other) const {
            return (mask1 == other.mask1 && mask2 == other.mask2);
        }

    template <typename X>
    friend std::ostream& operator << (std::ostream& out,
        const NTritmask1<X>& mask);
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
std::ostream& operator << (std::ostream& out, const NTritmask1<T>& mask) {
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
 * see NTritmask1.  At present there is no tritmask class
 * in Regina that can store arbitrarily many trits.
 *
 * \pre Types \a T and \a U are unsigned integral numeric types.
 *
 * \testpart
 *
 * \ifacespython Not present.
 */
template <typename T, typename U = T>
class NTritmask2 {
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
         * Creates a new tritmask with all bits set to \c false.
         */
        inline NTritmask2() : low(0), high(0) {
        }

        /**
         * Creates a new tritmask with all bits set to \c false.
         *
         * The integer argument is merely for compatibility with
         * the NTritmask constructor, and will be ignored.
         *
         * \warning This is \e not a constructor that initialises the
         * tritmask to a given pattern.
         */
        inline NTritmask2(unsigned) : low(0), high(0) {
        }

        /**
         * Creates a clone of the given tritmask.
         *
         * @param cloneMe the tritmask to clone.
         */
        inline NTritmask2(const NTritmask2<T, U>& cloneMe) :
                low(cloneMe.low), high(cloneMe.high) {
        }

        /**
         * Sets all bits of this tritmask to \c false.
         */
        inline void reset() {
            low = 0;
            high = 0;
        }

        /**
         * Sets this tritmask to a copy of the given tritmask.
         *
         * @param other the tritmask to clone.
         * @return a reference to this tritmask.
         */
        NTritmask2<T, U>& operator = (const NTritmask2<T, U>& other) {
            low = other.low;
            high = other.high;
            return *this;
        }

        /**
         * Returns the value of the given bit of this tritmask.
         *
         * @param index indicates which bit to query; this must be between
         * 0 and (8 * sizeof(\a T) + 8 * sizeof(\a U) - 1) inclusive.
         * @return the value of the (\a index)th bit.
         */
        inline bool get(unsigned index) const {
            if (index < 8 * sizeof(T))
                return (low & (T(1) << index));
            else
                return (high & (U(1) << (index - 8 * sizeof(T))));
        }

        /**
         * Sets the given bit of this tritmask to the given value.
         *
         * @param index indicates which bit to set; this must be between
         * 0 and (8 * sizeof(\a T) + 8 * sizeof(\a U) - 1) inclusive.
         * @param value the value that will be assigned to the (\a index)th bit.
         */
        inline void set(unsigned index, bool value) {
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
         * Sets this to the intersection of this and the given tritmask.
         * Every bit that is unset in \a other will be unset in this
         * tritmask.
         *
         * @param other the tritmask to intersect with this.
         * @return a reference to this tritmask.
         */
        inline NTritmask2<T, U>& operator &= (const NTritmask2<T, U>& other) {
            low &= other.low;
            high &= other.high;
            return *this;
        }

        /**
         * Sets this to the union of this and the given tritmask.
         * Every bit that is set in \a other will be set in this tritmask.
         *
         * @param other the tritmask to union with this.
         * @return a reference to this tritmask.
         */
        inline NTritmask2<T, U>& operator |= (const NTritmask2<T, U>& other) {
            low |= other.low;
            high |= other.high;
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
        inline NTritmask2<T, U>& operator -= (const NTritmask2<T, U>& rhs) {
            low1 = (low1 & ~ rhs.low1) | (low2 & ~ rhs.low1);
            low2 = (low2 & ~ rhs.low1);
            high1 = (high1 & ~ rhs.high1) | (high2 & ~ rhs.high1);
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
        inline bool operator == (const NTritmask2<T, U>& other) const {
            return (low1 == other.low1 && low2 == other.low2 &&
                high1 == other.high1 && high2 == other.high2);
        }

    template <typename X, typename Y>
    friend std::ostream& operator << (std::ostream& out,
        const NTritmask2<X, Y>& mask);
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
std::ostream& operator << (std::ostream& out, const NTritmask2<T, U>& mask) {
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
 * use the convenience typedefs NTritmaskLen8, NTritmaskLen16, NTritmaskLen32
 * and NTritmaskLen64.
 *
 * The reason this template exists is to circumvent the fact that we cannot
 * use sizeof() in a #if statement.  The boolean argument to this template
 * should always be left as the default.
 */
template <bool IntHolds4Bytes = (sizeof(unsigned int) >= 4)>
struct InternalTritmaskLen32;

template <>
struct InternalTritmaskLen32<true> {
    typedef NTritmask1<unsigned int> Type;
};

template <>
struct InternalTritmaskLen32<false> {
    // The standard guarantees that sizeof(long) >= 4.
    typedef NTritmask1<unsigned long> Type;
};

/**
 * An internal template that helps choose the correct tritmask type for
 * a given (hard-coded) number of bits.
 *
 * Please do not use this class directly, since this template is internal
 * and subject to change in future versions of Regina.  Instead please
 * use the convenience typedefs NTritmaskLen8, NTritmaskLen16, NTritmaskLen32
 * and NTritmaskLen64.
 *
 * The reason this template exists is to circumvent the fact that we cannot
 * use sizeof() in a #if statement.  The boolean argument to this template
 * should always be left as the default.
 */
template <bool LongHolds8Bytes = (sizeof(unsigned long) >= 8)>
struct InternalTritmaskLen64;

template <>
struct InternalTritmaskLen64<true> {
    typedef NTritmask1<unsigned long> Type;
};

template <>
struct InternalTritmaskLen64<false> {
#ifdef LONG_LONG_FOUND
    // The C standard guarantees that sizeof(long long) >= 8.
    // However, the C++ standard does not require long long to exist at
    // all (hence the LONG_LONG_FOUND test).
    typedef NTritmask1<unsigned long long> Type;
#else
    // The standard guarantees that sizeof(long) >= 4.
    // Therefore two longs will be enough for 64 bits.
    typedef NTritmask2<unsigned long> Type;
#endif
};
#endif // End block for doxygen to ignore.

/**
 * A convenience typedef that gives a small and extremely fast tritmask
 * class capable of holding at least 8 true-or-false bits.
 *
 * This tritmask class is guaranteed to be an instantiation of the
 * template class NTritmask1.
 *
 * The particular instantiation is subject to change between different
 * platforms, different compilers and/or different versions of Regina.
 *
 * \ifacespython Not present.
 */
typedef NTritmask1<unsigned char> NTritmaskLen8;

/**
 * A convenience typedef that gives a small and extremely fast tritmask
 * class capable of holding at least 16 true-or-false bits.
 *
 * This tritmask class is guaranteed to be an instantiation of the
 * template class NTritmask1.
 *
 * The particular instantiation is subject to change between different
 * platforms, different compilers and/or different versions of Regina.
 *
 * \ifacespython Not present.
 */
typedef NTritmask1<unsigned int> NTritmaskLen16;

/**
 * A convenience typedef that gives a small and extremely fast tritmask
 * class capable of holding at least 32 true-or-false bits.
 *
 * This tritmask class is guaranteed to be an instantiation of the
 * template class NTritmask1.
 *
 * The particular instantiation is subject to change between different
 * platforms, different compilers and/or different versions of Regina.
 *
 * \ifacespython Not present.
 */
typedef InternalTritmaskLen32<>::Type NTritmaskLen32;

/**
 * A convenience typedef that gives a small and extremely fast tritmask
 * class capable of holding at least 64 true-or-false bits.
 *
 * This tritmask class is guaranteed to be an instantiation of
 * \e either the template class NTritmask1 or the template class NTritmask2.
 *
 * The particular instantiation is subject to change between different
 * platforms, different compilers and/or different versions of Regina.
 *
 * \ifacespython Not present.
 */
typedef InternalTritmaskLen64<>::Type NTritmaskLen64;

/*@}*/

} // namespace regina

#endif

