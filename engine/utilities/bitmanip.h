
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
 * A generic class for bitwise analysis and manipulation of native data types.
 *
 * This class is not optimised for common data types; its
 * sole functionality is to provide default implementations for some
 * operations that are not yet optimised (or do not need to be).
 *
 * End users should use BitManipulator<T, size> instead, which is better
 * optimised and which has all the functionality of this class.
 *
 * See BitManipulator for further information.
 *
 * \pre Type \a T is an unsigned integral numeric type.
 * \pre The argument \a size is a power of two, and is at most sizeof(\a T).
 *
 * \ifacespython Not present.
 */
template <typename T, unsigned size = sizeof(T)>
class GenericBitManipulator {
    public:
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
            unsigned chunkSize = size << 3; // 8 * size
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

            // This binary search will break if size is not a power of two.
            unsigned chunkSize = size << 3; // 8 * size
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

/**
 * An optimised class for bitwise analysis and manipulation of native
 * data types.
 *
 * The class BitManipulator<T, size> is used to manipulate the lowest
 * \a size bytes of type \a T, which must be an unsigned native integer
 * type such as unsigned char, unsigned int, or unsigned long long.
 *
 * The generic implementation of BitManipulator is here for completeness.
 * All or most native types \a T have template specialisations
 * that are carefully optimised (precisely what gets specialised depends
 * upon properties of the compiler).
 *
 * Like this class, all specialisations either inherit or override the
 * default implementations from the base class GenericBitManipulator.
 *
 * \pre Type \a T is an unsigned integral numeric type.
 * \pre The argument \a size is a power of two, and is at most sizeof(\a T).
 *
 * \ifacespython Not present.
 */
template <typename T, unsigned size = sizeof(T)>
class BitManipulator : public GenericBitManipulator<T, size> {
    public:
        enum {
            /**
             * Indicates whether this class is a template specialisation
             * of BitManipulator with extra optimisations.
             *
             * This compile-time constant is set to 0 for the generic
             * implementation of BitManipulator, and 1 for all specialisations.
             */
            specialised = 0
        };

        /**
         * Returns the number of bits that are set in the given integer.
         *
         * Specifically, this routine returns the number of bits set to 1
         * from amongst the lowest \a size bytes of \a x.
         *
         * @param x the integer of type \a T to examine.
         * @return the number of bits that are set.
         */
        inline static int bits(T x) {
            return BitManipulator<T, (size >> 1)>::bits(x) +
                BitManipulator<T, (size >> 1)>::bits(x >> (4 * size));
        }
};

// Specialisations for individual type sizes.
// Hide these from doxygen.

#ifndef __DOXYGEN

template <typename T>
class BitManipulator<T, 1> : public GenericBitManipulator<T, 1> {
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
class BitManipulator<T, 2> : public GenericBitManipulator<T, 2> {
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
class BitManipulator<T, 4> : public GenericBitManipulator<T, 4> {
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
class BitManipulator<T, 8> : public GenericBitManipulator<T, 8> {
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
class BitManipulator<T, 8> : public GenericBitManipulator<T, 8> {
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
#endif

// End the hide-from-doxygen block.
#endif

/*@}*/

} // namespace regina

#endif
