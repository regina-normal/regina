
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

/*! \file utilities/bitmanip.h
 *  \brief Optimised classes for bitwise analysis and manipulation of
 *  native data types.
 */

#ifndef __BITMANIP_H
#ifndef __DOXYGEN
#define __BITMANIP_H
#endif

#include "regina-core.h"
#include "regina-config.h"

namespace regina {

/**
 * \weakgroup utilities
 * @{
 */

/**
 * Contains implementation details for BitManipulator where we optimise
 * according to the underlying data type.
 *
 * End users should use the BitManipulator class, not this class.
 *
 * \pre Type \a T is an unsigned integral numeric type.
 *
 * @tparam T an unsigned integral numeric type, which we treat as a
 * sequence of \c true and/or \c false bits.
 */
template <typename T>
class BitManipulatorByType {
    public:
        enum {
            /**
             * Indicates whether this class is a template specialisation
             * of BitManipulatorByType with extra optimisations.
             *
             * This compile-time constant is set to 0 for the generic
             * implementation of BitManipulatorByType, and 1 for all
             * specialisations.
             */
            specialised = 0
        };

        /**
         * Returns the next largest integer with the same number of \c
         * true bits as \a x.
         *
         * If \a x is the largest such integer (i.e., \a x is of the
         * form 111...1000...0), then this routine returns 0.
         *
         * @param x the integer of type \a T to examine.
         * @return the next largrst integer with the same number of \c true
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
        enum {
            specialised = 1
        };

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
        enum {
            specialised = 1
        };

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
        enum {
            specialised = 1
        };

        inline static unsigned long nextPermutation(unsigned long x) {
            // Based upon http://graphics.stanford.edu/~seander/bithacks.html
            // (which is in the public domain):
            unsigned long t = x | (x - 1);
            if (t == (unsigned long)(-1))
                return 0;
            return (t + 1) | (((~t & -~t) - 1) >> (__builtin_ctzl(x) + 1));
        }
};

#ifdef INT128_AVAILABLE
template <>
class BitManipulatorByType<unsigned long long> {
    public:
        enum {
            specialised = 1
        };

        inline static unsigned long long nextPermutation(unsigned long long x) {
            // Based upon http://graphics.stanford.edu/~seander/bithacks.html
            // (which is in the public domain):
            unsigned long long t = x | (x - 1);
            if (t == (unsigned long long)(-1))
                return 0;
            return (t + 1) | (((~t & -~t) - 1) >> (__builtin_ctzll(x) + 1));
        }
};
#endif // INT128_AVAILABLE
#endif // __GNUC__
#endif // __DOXYGEN__

/**
 * Contains implementation details for BitManipulator where we optimise
 * according to the size of the underlying data type.
 *
 * End users should use the BitManipulator class, not this class.
 *
 * \pre Type \a T is an unsigned integral numeric type.
 * \pre The argument \a size is a power of two, and is at most sizeof(\a T).
 *
 * @tparam T an unsigned integral numeric type, which we treat as a
 * sequence of \c true and/or \c false bits.
 * @tparam size the number of \e bytes of \a T to examine.  Any higher-order
 * bits will be ignored by the implementations in this class.
 */
template <typename T, unsigned size = sizeof(T)>
class BitManipulatorBySize {
    public:
        enum {
            /**
             * Indicates whether this class is a template specialisation
             * of BitManipulatorBySize with extra optimisations.
             *
             * This compile-time constant is set to 0 for the generic
             * implementation of BitManipulatorBySize, and 1 for all
             * specialisations.
             */
            specialised = 0
        };

        /**
         * Returns the number of bits that are set to 1 in the given integer.
         *
         * @param x the integer of type \a T to examine.
         * @return the number of bits that are set.
         */
        inline static int bits(T x) {
            return BitManipulatorBySize<T, (size >> 1)>::bits(x) +
                BitManipulatorBySize<T, (size >> 1)>::bits(x >> (4 * size));
        }
};

// Specialisations for individual type sizes.
// Hide these from doxygen.

#ifndef __DOXYGEN

template <typename T>
class BitManipulatorBySize<T, 1> {
    public:
        enum {
            specialised = 1
        };

        inline static int bits(T x) {
            x = (x & T(0x55)) + ((x & T(0xAA)) >> 1);
            x = (x & T(0x33)) + ((x & T(0xCC)) >> 2);
            return (x & T(0x0F)) + ((x & T(0xF0)) >> 4);
        }
};

template <typename T>
class BitManipulatorBySize<T, 2> {
    public:
        enum {
            specialised = 1
        };

        inline static int bits(T x) {
            x = (x & T(0x5555)) + ((x & T(0xAAAA)) >> 1);
            x = (x & T(0x3333)) + ((x & T(0xCCCC)) >> 2);
            x = (x & T(0x0F0F)) + ((x & T(0xF0F0)) >> 4);
            return (x & T(0x00FF)) + ((x & T(0xFF00)) >> 8);
        }
};

template <typename T>
class BitManipulatorBySize<T, 4> {
    public:
        enum {
            specialised = 1
        };

        inline static int bits(T x) {
            x = (x & T(0x55555555)) + ((x & T(0xAAAAAAAA)) >> 1);
            x = (x & T(0x33333333)) + ((x & T(0xCCCCCCCC)) >> 2);
            x = (x & T(0x0F0F0F0F)) + ((x & T(0xF0F0F0F0)) >> 4);
            x = (x & T(0x00FF00FF)) + ((x & T(0xFF00FF00)) >> 8);
            return (x & T(0x0000FFFF)) + ((x & T(0xFFFF0000)) >> 16);
        }
};

// Support 64-bit processing natively if we can; otherwise we will fall
// back to the generic two-lots-of-32-bit implementation.
#if defined(NUMERIC_64_FOUND)
template <typename T>
class BitManipulatorBySize<T, 8> {
    public:
        enum {
            specialised = 1
        };

        inline static int bits(T x) {
            x = (x & T(0x5555555555555555)) +
                ((x & T(0xAAAAAAAAAAAAAAAA)) >> 1);
            x = (x & T(0x3333333333333333)) +
                ((x & T(0xCCCCCCCCCCCCCCCC)) >> 2);
            x = (x & T(0x0F0F0F0F0F0F0F0F)) +
                ((x & T(0xF0F0F0F0F0F0F0F0)) >> 4);
            x = (x & T(0x00FF00FF00FF00FF)) +
                ((x & T(0xFF00FF00FF00FF00)) >> 8);
            x = (x & T(0x0000FFFF0000FFFF)) +
                ((x & T(0xFFFF0000FFFF0000)) >> 16);
            return (x & T(0x00000000FFFFFFFF)) +
                ((x & T(0xFFFFFFFF00000000)) >> 32);
        }
};
#elif defined(NUMERIC_64_LL_FOUND)
template <typename T>
class BitManipulatorBySize<T, 8> {
    public:
        enum {
            specialised = 1
        };

        inline static int bits(T x) {
            x = (x & T(0x5555555555555555LL)) +
                ((x & T(0xAAAAAAAAAAAAAAAALL)) >> 1);
            x = (x & T(0x3333333333333333LL)) +
                ((x & T(0xCCCCCCCCCCCCCCCCLL)) >> 2);
            x = (x & T(0x0F0F0F0F0F0F0F0FLL)) +
                ((x & T(0xF0F0F0F0F0F0F0F0LL)) >> 4);
            x = (x & T(0x00FF00FF00FF00FFLL)) +
                ((x & T(0xFF00FF00FF00FF00LL)) >> 8);
            x = (x & T(0x0000FFFF0000FFFFLL)) +
                ((x & T(0xFFFF0000FFFF0000LL)) >> 16);
            return (x & T(0x00000000FFFFFFFFLL)) +
                ((x & T(0xFFFFFFFF00000000LL)) >> 32);
        }
};
#endif // NUMERIC_64_LL_FOUND
#endif // __DOXYGEN__

/**
 * An optimised class for bitwise analysis and manipulation of native
 * data types.
 *
 * The class BitManipulator<T> is used to manipulate an integer of type \a T
 * as a sequence of bits.  Here \a T must be an unsigned native integer
 * type such as unsigned char, unsigned int, or unsigned long long.
 *
 * Whilst BitManipulator has a generic implementation, all or most native types
 * \a T have template specialisations that are carefully optimised (precisely
 * what gets specialised depends upon properties of the compiler).
 *
 * \pre Type \a T is an unsigned integral numeric type whose size in
 * bits is a power of two.
 *
 * \ifacespython Not present.
 */
template <typename T>
class BitManipulator :
        public BitManipulatorByType<T>, public BitManipulatorBySize<T> {
    static_assert(sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4 ||
        sizeof(T) == 8 || sizeof(T) == 16 || sizeof(T) == 32 ||
        sizeof(T) == 64 || sizeof(T) == 128,
        "BitManipulator can only work with data types whose size is a "
        "power of two.");
    public:
        enum {
            /**
             * Indicates whether this class is a template specialisation
             * of BitManipulator with extra optimisations.
             *
             * This compile-time constant is set to 0 for the generic
             * implementation of BitManipulator, and 1 for all specialisations.
             */
            specialised = (BitManipulatorByType<T>::specialised |
                BitManipulatorBySize<T>::specialised)
        };

        /**
         * Returns the index of the first \c true bit in the given
         * integer, or -1 if the given integer is zero.
         * Bits are indexed from 0 upwards, starting at the least
         * significant bit.
         *
         * @param x the integer of type \a T to examine.
         * @return the position of the first \c true bit, or -1 if there
         * are no \c true bits.
         */
        inline static int firstBit(T x) {
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
         * @param x the integer of type \a T to examine.
         * @return the position of the last \c true bit, or -1 if there
         * are no \c true bits.
         */
        inline static int lastBit(T x) {
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
};

/*@}*/

} // namespace regina

#endif
