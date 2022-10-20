
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

/*! \file utilities/qitmask.h
 *  \brief Provides optimised "base 4 bitmasks" of fixed length.
 */

#ifndef __REGINA_QITMASK_H
#ifndef __DOXYGEN
#define __REGINA_QITMASK_H
#endif

#include <algorithm>
#include <iostream>

#include "regina-core.h"
#include "regina-config.h"

namespace regina {

template <typename T>
class Qitmask1;

/**
 * Writes the given qitmask to the given output stream as a sequence of
 * digits (0, 1, 2 and/or 3).
 *
 * Since the length of the qitmask is not stored, the number of qits
 * written will be 8 * sizeof(\a T).
 *
 * \param out the output stream to which to write.
 * \param mask the qitmask to write.
 * \return a reference to the given output stream.
 *
 * \ingroup utilities
 */
template <typename T>
std::ostream& operator << (std::ostream& out, const Qitmask1<T>& mask) {
    for (T bit = 1; bit; bit <<= 1)
        out << int(((mask.mask1 & bit) ? 1 : 0) | ((mask.mask2 & bit) ? 2 : 0));
    return out;
}

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
 * many qits, see Qitmask2.  At present there is no qitmask class
 * in Regina that can store arbitrarily many qits.
 *
 * These objects are small enough to pass by value and swap with std::swap(),
 * with no need for any specialised move operations or swap functions.
 *
 * \pre Type \a T is an unsigned integral numeric type.
 *
 * \ifacespython Python does not support templates, and so instead Regina's
 * python interface offers the classes Qitmask8, Qitmask16, Qitmask32,
 * Qitmask64, Qitmask128, and (if the machine supports 128-bit integers)
 * Qitmask256.  Each of these will be an optimised qitmask class that
 * can hold the corresponding number of bits, and is guaranteed to be an
 * instance of either the C++ Qitmask1<T> class (where possible) or the
 * C++ Qitmask2<T,U> template class (if necessary).
 *
 * \ingroup utilities
 */
template <typename T>
class Qitmask1 {
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
        inline Qitmask1() : mask1(0), mask2(0) {
        }

        /**
         * Creates a clone of the given qitmask.
         *
         * \param cloneMe the qitmask to clone.
         */
        inline Qitmask1(const Qitmask1<T>& cloneMe) = default;

        /**
         * Sets all qits of this qitmask to 0.
         */
        inline void reset() {
            mask1 = mask2 = 0;
        }

        /**
         * Sets this qitmask to a copy of the given qitmask.
         *
         * \param other the qitmask to clone.
         * \return a reference to this qitmask.
         */
        Qitmask1<T>& operator = (const Qitmask1<T>& other) = default;

        /**
         * Returns the value of the given qit in this qitmask.
         *
         * \param index indicates which qit to query; this must be between
         * 0 and (8 * sizeof(\a T) - 1) inclusive.
         * \return the value of the (\a index)th qit; this will be
         * either 0, 1, 2 or 3.
         */
        inline uint8_t get(size_t index) const {
            // We cast back down to T because, if T is smaller than int, then
            // the shift operator promotes up to an int.
            T bit = static_cast<T>(T(1) << index);
            return ((mask1 & bit) ? 1 : 0) | ((mask2 & bit) ? 2 : 0);
        }

        /**
         * Sets the given qit of this qitmask to the given value.
         *
         * \param index indicates which qit to set; this must be between
         * 0 and (8 * sizeof(\a T) - 1) inclusive.
         * \param value the value that will be assigned to the (\a index)th
         * qit; this must be 0, 1, 2 or 3.
         */
        inline void set(size_t index, uint8_t value) {
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
         * \return \c true if every qit is zero, or \c false otherwise.
         */
        inline bool empty() const {
            return ! (mask1 || mask2);
        }

        /**
         * Determines whether this qitmask contains at least one
         * non-zero qit.
         *
         * \return \c true if at least one qit is non-zero, or
         * \c false otherwise.
         */
        inline bool nonEmpty() const {
            return (mask1 || mask2);
        }

        /**
         * Determines whether this qitmask contains at least one qit
         * with value 3.
         *
         * \return \c true if at least one qit is 3, or \c false otherwise.
         */
        inline bool has3() const {
            return (mask1 & mask2);
        }

        /**
         * Sets this to the sum of this and the given qitmask.
         * Each pair of qits is added modulo 4 (so, for instance, 2 + 3 = 1).
         *
         * \param rhs the qitmask to add to this.
         * \return a reference to this qitmask.
         */
        inline Qitmask1<T>& operator += (const Qitmask1<T>& rhs) {
            mask2 = mask2 ^ rhs.mask2 ^ (mask1 & rhs.mask1);
            mask1 = mask1 ^ rhs.mask1;
            return *this;
        }

        /**
         * Sets this to the difference of this and the given qitmask.
         * Each pair of qits is subtracted modulo 4 (so, for instance,
         * 1 - 3 = 2).
         *
         * \param rhs the qitmask to subtract from this.
         * \return a reference to this qitmask.
         */
        inline Qitmask1<T>& operator -= (const Qitmask1<T>& rhs) {
            mask2 = mask2 ^ rhs.mask2 ^ (rhs.mask1 & ~ mask1);
            mask1 = mask1 ^ rhs.mask1;
            return *this;
        }

        /**
         * Determines whether this and the given qitmask are identical.
         *
         * \param other the qitmask to compare against this.
         * \return \c true if and only if this and the given qitmask are
         * identical.
         */
        inline bool operator == (const Qitmask1<T>& other) const {
            return (mask1 == other.mask1 && mask2 == other.mask2);
        }

        /**
         * Determines whether this and the given qitmask are different.
         *
         * \param other the qitmask to compare against this.
         * \return \c true if and only if this and the given qitmask are
         * different.
         */
        inline bool operator != (const Qitmask1<T>& other) const {
            return (mask1 != other.mask1 || mask2 != other.mask2);
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
         * \param other the qitmask to compare with this.
         * \return \c true if there is some index at which this and \a other
         * both have non-zero qits, or \c false otherwise.
         */
        inline bool hasNonZeroMatch(const Qitmask1<T>& other) const {
            return ((mask1 | mask2) & (other.mask1 | other.mask2));
        }

    friend std::ostream& operator << <T>(std::ostream& out,
        const Qitmask1<T>& mask);
};

template <typename T, typename U>
class Qitmask2;

/**
 * Writes the given qitmask to the given output stream as a sequence of
 * digits (0, 1, 2 and/or 3).
 *
 * Since the length of the qitmask is not stored, the number of qits
 * written will be 8 * sizeof(\a T) + 8 * sizeof(\a U).
 *
 * \param out the output stream to which to write.
 * \param mask the qitmask to write.
 * \return a reference to the given output stream.
 *
 * \ingroup utilities
 */
template <typename T, typename U>
std::ostream& operator << (std::ostream& out, const Qitmask2<T, U>& mask) {
    for (T bit = 1; bit; bit <<= 1)
        out << int(((mask.low1 & bit) ? 1 : 0) | ((mask.low2 & bit) ? 2 : 0));
    for (U bit = 1; bit; bit <<= 1)
        out << int(((mask.high1 & bit) ? 1 : 0) | ((mask.high2 & bit) ? 2 : 0));
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
 * see Qitmask1.  At present there is no qitmask class
 * in Regina that can store arbitrarily many qits.
 *
 * These objects are small enough to pass by value and swap with std::swap(),
 * with no need for any specialised move operations or swap functions.
 *
 * \pre Types \a T and \a U are unsigned integral numeric types.
 *
 * \ifacespython Python does not support templates, and so instead Regina's
 * python interface offers the classes Qitmask8, Qitmask16, Qitmask32,
 * Qitmask64, Qitmask128, and (if the machine supports 128-bit integers)
 * Qitmask256.  Each of these will be an optimised qitmask class that
 * can hold the corresponding number of bits, and is guaranteed to be an
 * instance of either the C++ Qitmask1<T> class (where possible) or the
 * C++ Qitmask2<T,U> template class (if necessary).
 *
 * \ingroup utilities
 */
template <typename T, typename U = T>
class Qitmask2 {
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
        inline Qitmask2() : low1(0), low2(0), high1(0), high2(0) {
        }

        /**
         * Creates a clone of the given qitmask.
         *
         * \param cloneMe the qitmask to clone.
         */
        inline Qitmask2(const Qitmask2<T, U>& cloneMe) = default;

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
         * \param other the qitmask to clone.
         * \return a reference to this qitmask.
         */
        Qitmask2<T, U>& operator = (const Qitmask2<T, U>& other) = default;

        /**
         * Returns the value of the given qit in this qitmask.
         *
         * \param index indicates which qit to query; this must be between
         * 0 and (8 * sizeof(\a T) + 8 * sizeof(\a U) - 1) inclusive.
         * \return the value of the (\a index)th qit; this will be
         * either 0, 1, 2 or 3.
         */
        inline uint8_t get(size_t index) const {
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
         * \param index indicates which qit to set; this must be between
         * 0 and (8 * sizeof(\a T) + 8 * sizeof(\a U) - 1) inclusive.
         * \param value the value that will be assigned to the (\a index)th
         * qit; this must be 0, 1, 2 or 3.
         */
        inline void set(size_t index, uint8_t value) {
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
         * \return \c true if every qit is zero, or \c false otherwise.
         */
        inline bool empty() const {
            return ! (low1 || high1 || low2 || high2);
        }

        /**
         * Determines whether this qitmask contains at least one
         * non-zero qit.
         *
         * \return \c true if at least one qit is non-zero, or
         * \c false otherwise.
         */
        inline bool nonEmpty() const {
            return (low1 || high1 || low2 || high2);
        }

        /**
         * Determines whether this qitmask contains at least one qit
         * with value 3.
         *
         * \return \c true if at least one qit is 3, or \c false otherwise.
         */
        inline bool has3() const {
            return (low1 & low2) || (high1 & high2);
        }

        /**
         * Sets this to the sum of this and the given qitmask.
         * Each pair of qits is added modulo 4 (so, for instance, 2 + 3 = 1).
         *
         * \param rhs the qitmask to add to this.
         * \return a reference to this qitmask.
         */
        inline Qitmask2<T, U>& operator += (const Qitmask2<T, U>& rhs) {
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
         * \param rhs the qitmask to subtract from this.
         * \return a reference to this qitmask.
         */
        inline Qitmask2<T, U>& operator -= (const Qitmask2<T, U>& rhs) {
            low2 = low2 ^ rhs.low2 ^ (rhs.low1 & ~ low1);
            low1 = low1 ^ rhs.low1;
            high2 = high2 ^ rhs.high2 ^ (rhs.high1 & ~ high1);
            high1 = high1 ^ rhs.high1;
            return *this;
        }

        /**
         * Determines whether this and the given qitmask are identical.
         *
         * \param other the qitmask to compare against this.
         * \return \c true if and only if this and the given qitmask are
         * identical.
         */
        inline bool operator == (const Qitmask2<T, U>& other) const {
            return (low1 == other.low1 && low2 == other.low2 &&
                high1 == other.high1 && high2 == other.high2);
        }

        /**
         * Determines whether this and the given qitmask are different.
         *
         * \param other the qitmask to compare against this.
         * \return \c true if and only if this and the given qitmask are
         * different.
         */
        inline bool operator != (const Qitmask2<T, U>& other) const {
            return (low1 != other.low1 || low2 != other.low2 ||
                high1 != other.high1 || high2 != other.high2);
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
         * \param other the qitmask to compare with this.
         * \return \c true if there is some index at which this and \a other
         * both have non-zero qits, or \c false otherwise.
         */
        inline bool hasNonZeroMatch(const Qitmask2<T, U>& other) const {
            return ((low1 | low2) & (other.low1 | other.low2)) ||
                ((high1 | high2) & (other.high1 | other.high2));
        }

    friend std::ostream& operator << <T, U>(std::ostream& out,
        const Qitmask2<T, U>& mask);
};

} // namespace regina

#endif

