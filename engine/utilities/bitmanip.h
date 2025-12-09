
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

/*! \file utilities/bitmanip.h
 *  \brief Optimised classes for bitwise analysis and manipulation of
 *  native data types.
 */

#ifndef __REGINA_BITMANIP_H
#ifndef __DOXYGEN
#define __REGINA_BITMANIP_H
#endif

#include <bit>
#include <compare>
#include "concepts/core.h"

namespace regina {

/**
 * Contains implementation details for BitManipulator where we optimise
 * according to the underlying data type.
 *
 * End users should use the BitManipulator class, not this class.
 *
 * \pre Type \a T is an unsigned integral numeric type.
 *
 * \nopython Only the end-user class BitManipulator<unsigned long> is
 * available to Python users.
 *
 * \tparam T an unsigned integral numeric type, which we treat as a
 * sequence of \c true and/or \c false bits.
 *
 * \ingroup utilities
 */
template <typename T>
class BitManipulatorByType {
    public:
        /**
         * Indicates whether this class is a template specialisation
         * with extra optimisations.
         *
         * This compile-time constant is set to \c false for the generic
         * implementation, and \c true for all specialisations.
         */
        static constexpr bool specialised = false;

        /**
         * Returns the next largest integer with the same number of \c
         * true bits as \a x.
         *
         * If \a x is the largest such integer (i.e., \a x is of the
         * form 111...1000...0), then this routine returns 0.
         *
         * \param x the integer of type \a T to examine.
         * \return the next largrst integer with the same number of \c true
         * bits, or 0 if this is the largest such integer.
         */
        inline static T nextPermutation(T x) {
            // Based upon http://graphics.stanford.edu/~seander/bithacks.html
            // (which is in the public domain):
            T t = (x | (x - 1)) + 1;
            if (t == 0)
                return 0;
            return t | ((((t & -t) / (x & -x)) >> 1) - 1);
        }
};

// Specialisations for individual types.
// Hide these from doxygen.

#ifndef __DOXYGEN
#ifdef __GNUC__
template <>
class BitManipulatorByType<unsigned char> {
    public:
        static constexpr bool specialised = true;

        inline static unsigned char nextPermutation(unsigned char x) {
            // Based upon http://graphics.stanford.edu/~seander/bithacks.html
            // (which is in the public domain):
            unsigned char t = x | (x - 1);
            if (t == (unsigned char)(-1))
                return 0;
            return (t + 1) | (((~t & -~t) - 1) >> (__builtin_ctz(x) + 1));
        }
};

template <>
class BitManipulatorByType<unsigned int> {
    public:
        static constexpr bool specialised = true;

        inline static unsigned int nextPermutation(unsigned int x) {
            // Based upon http://graphics.stanford.edu/~seander/bithacks.html
            // (which is in the public domain):
            unsigned int t = x | (x - 1);
            if (t == (unsigned int)(-1))
                return 0;
            return (t + 1) | (((~t & -~t) - 1) >> (__builtin_ctz(x) + 1));
        }
};

template <>
class BitManipulatorByType<unsigned long> {
    public:
        static constexpr bool specialised = true;

        inline static unsigned long nextPermutation(unsigned long x) {
            // Based upon http://graphics.stanford.edu/~seander/bithacks.html
            // (which is in the public domain):
            unsigned long t = x | (x - 1);
            if (t == (unsigned long)(-1))
                return 0;
            return (t + 1) | (((~t & -~t) - 1) >> (__builtin_ctzl(x) + 1));
        }
};

template <>
class BitManipulatorByType<unsigned long long> {
    public:
        static constexpr bool specialised = true;

        inline static unsigned long long nextPermutation(unsigned long long x) {
            // Based upon http://graphics.stanford.edu/~seander/bithacks.html
            // (which is in the public domain):
            unsigned long long t = x | (x - 1);
            if (t == (unsigned long long)(-1))
                return 0;
            return (t + 1) | (((~t & -~t) - 1) >> (__builtin_ctzll(x) + 1));
        }
};
#endif // __GNUC__
#endif // __DOXYGEN__

/**
 * An optimised class for bitwise analysis and manipulation of native
 * C++ integer types.
 *
 * Whilst BitManipulator has a generic implementation, all or most native types
 * \a T have template specialisations that are carefully optimised (precisely
 * what gets specialised depends upon properties of the compiler).
 *
 * \pre The size in bits of type \a T is a power of two.
 *
 * \python For Python users, the class BitManipulator represents the
 * C++ type BitManipulator<unsigned long>.  In particular, you should be aware
 * that BitManipulator is designed specifically to work with native C++ integer
 * types, and _cannot_ handle Python's arbitrary-precision integers.  It is
 * up to you to ensure that any Python integers that you pass into the
 * BitManipulator routines are small enough to fit inside a C++ unsigned long.
 */
template <UnsignedCppInteger T>
class BitManipulator : public BitManipulatorByType<T> {
    static_assert(sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4 ||
        sizeof(T) == 8 || sizeof(T) == 16 || sizeof(T) == 32 ||
        sizeof(T) == 64 || sizeof(T) == 128,
        "BitManipulator can only work with data types whose size is a "
        "power of two.");
    public:
        /**
         * Returns the number of bits that are set to 1 in the given integer.
         *
         * The implementation uses `std::popcount()` where possible, and a
         * hand-rolled implementation where `std::popcount()` might be
         * unavailable (e.g., for 128-bit integers).
         *
         * \param x the integer of type \a T to examine.
         * \return the number of bits that are set.
         */
        inline static constexpr int bits(T x) {
            if constexpr (sizeof(T) > sizeof(unsigned long long)) {
                using HalfSize = typename IntOfSize<sizeof(T) / 2>::utype;
                return std::popcount(static_cast<HalfSize>(x)) +
                    std::popcount(static_cast<HalfSize>(x >> (4 * sizeof(T))));
            } else {
                return std::popcount(x);
            }
        }

        /**
         * Returns the index of the first \c true bit in the given
         * integer, or -1 if the given integer is zero.
         * Bits are indexed from 0 upwards, starting at the least
         * significant bit.
         *
         * \param x the integer of type \a T to examine.
         * \return the position of the first \c true bit, or -1 if there
         * are no \c true bits.
         */
        inline static constexpr int firstBit(T x) {
            if (! x)
                return -1;

            // This binary search will break if size is not a power of two.
            unsigned chunkSize = sizeof(T) << 3; // 8 * sizeof(T)
            unsigned chunkStart = 0;

            while (chunkSize > 1) {
                chunkSize >>= 1;
                if (! (x & (((T(1) << chunkSize) - T(1)) << chunkStart)))
                    chunkStart += chunkSize;
            }
            return chunkStart;
        }

        /**
         * Returns the index of the last \c true bit in the given
         * integer, or -1 if the given integer is zero.
         * Bits are indexed from 0 upwards, starting at the least
         * significant bit.
         *
         * \param x the integer of type \a T to examine.
         * \return the position of the last \c true bit, or -1 if there
         * are no \c true bits.
         */
        inline static constexpr int lastBit(T x) {
            if (! x)
                return -1;

            // This binary search will break if sizeof(T) is not a power of two.
            unsigned chunkSize = sizeof(T) << 3; // 8 * sizeof(T)
            unsigned chunkStart = 0;

            while (chunkSize > 1) {
                chunkSize >>= 1;
                if (x & (((T(1) << chunkSize) - T(1))
                        << (chunkStart + chunkSize)))
                    chunkStart += chunkSize;
            }
            return chunkStart;
        }

        /**
         * Returns a copy of the given integer with two bits swapped.
         * Bits are indexed from 0 upwards, starting at the least
         * significant bit.
         *
         * The two indices \a index0 and \a index1 may be the same (in which
         * case the given bitmask will be returned unchanged).
         *
         * \param x the bitmask to examine.
         * \param index0 the index of the first bit to swap.
         * \param index1 the index of the second bit to swap.
         * \return a copy of \a x with bits \a index0 and \a index1 swapped.
         */
        inline static constexpr T swapBits(T x, int index0, int index1) {
            // TODO: There is surely a slicker way to do this.
            T mask0 = (T(1) << index0);
            T mask1 = (T(1) << index1);
            T bit0 = (x & mask0);
            T bit1 = (x & mask1);
            if ((bit0 && bit1) || ! (bit0 || bit1)) {
                // Both bits stay the same.
                return x;
            } else {
                // Both bits get flipped.
                return x ^ (mask0 | mask1);
            }
        }

        /**
         * Compares the bits of two integers under the subset relation.
         *
         * Here \a x is considered less than \a y if the bits that are set in
         * \a x form a strict subset of the bits that are set in \a y.
         *
         * \nopython This is not available for Python users, since Python does
         * not have access to the standard C++ type `std::partial_ordering`,
         * and since there is no "natural" way to present a partial ordering
         * as an integer.
         *
         * \param x the first integer to examine.
         * \param y the second integer to examine.
         * \return A three-way comparison result, indicating whether the bits
         * of \a x are equal to, a strict subset of, a strict superset of,
         * or incomparable to the bits of \a y.  These outcomes are indicated
         * by the return values `equivalent`, `less`, `greater`, and
         * `unordered` respectively.
         */
        inline static std::partial_ordering subsetComparison(T x, T y) {
            if (x == y)
                return std::partial_ordering::equivalent;
            else if ((x | y) == y)
                return std::partial_ordering::less;
            else if ((x | y) == x)
                return std::partial_ordering::greater;
            else
                return std::partial_ordering::unordered;
        }
};

} // namespace regina

#endif
