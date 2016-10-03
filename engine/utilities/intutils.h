
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

/*! \file utilities/intutils.h
 *  \brief Miscellaneous utility classes for analysing the built-in
 *  integer types at compile time.
 */

#ifndef __INTUTILS_H
#ifndef __DOXYGEN
#define __INTUTILS_H
#endif

#include "regina-core.h"
#include "regina-config.h"
#include <stdint.h>

#if defined(USE_BOOST_INT128)
#include <boost/config.hpp>
#endif

namespace regina {

/**
 * Returns the number of bits required to store integers in the range
 * 0,...,<i>n</i>-1.
 * This is simply the number of bits in the binary expansion of <i>n</i>-1.
 *
 * If \a n is non-positive then this function will return 0.
 *
 * \ifacespython In Python, this routine fixes the integer type
 * \a IntType to be \c long.
 *
 * \param n any integer.
 * \return the number of bits required to store 0,...,<i>n</i>-1.
 *
 * \tparam IntType any integer type, such as \c int, \c long, and so on.
 */
template <typename IntType>
constexpr int bitsRequired(IntType n) {
    return (n <= 1 ? 0 : (bitsRequired((n + 1) / 2)) + 1);
}

/**
 * Returns the smallest integer power of two that is greater than or equal to
 * the given argument \a n.
 *
 * If \a n is non-positive then this function will return 1.
 *
 * \ifacespython In Python, this routine fixes the integer type
 * \a IntType to be \c long.
 *
 * \warning Even though the return value is the same type as the
 * argument \a n, this routine may still overflow.  For example, if
 * \a IntType is a signed char then nextPowerOfTwo(127) will return -128, and
 * if \a IntType is an unsigned char then nextPowerOfTwo(255) will return 0.
 * Be sure that \a IntType is large enough for your requirements.
 *
 * \param n any integer.
 * \return the smallest integer power of two that is &ge; \a n.
 *
 * \tparam IntType any integer type, such as \c int, \c long, and so on.
 */
template <typename IntType>
constexpr IntType nextPowerOfTwo(IntType n) {
    return (n <= 1 ? 1 : (nextPowerOfTwo((n + 1) / 2)) << 1);
}

/**
 * Gives access to native integer types that hold \e exactly \a k bytes,
 * where \a k may be any compile-time constant.
 *
 * \tparam bytes the exact number of bytes in the native integer types
 * (i.e., the integer \a k described above).
 *
 * \ifacespython Not present.
 *
 * @see IntOfMinSize
 */
template <int bytes>
struct IntOfSize {
    /**
     * A native signed integer type with exactly \a k bytes, where \a k is the
     * template parameter.
     *
     * The default is \c void, which indicates that Regina does not know
     * how to access an integer type of the requested size.
     */
    typedef void type;

    /**
     * A native unsigned integer type with exactly \a k bytes, where \a k is the
     * template parameter.
     *
     * The default is \c void, which indicates that Regina does not know
     * how to access an integer type of the requested size.
     */
    typedef void utype;
};

/**
 * Gives access to native integer types that hold <em>at least</em> \a k bytes,
 * where \a k may be any compile-time constant.
 *
 * \tparam bytes the minimum number of bytes in the native integer types
 * (i.e., the integer \a k described above).
 *
 * \ifacespython Not present.
 *
 * @see IntOfSize
 */
template <int bytes>
struct IntOfMinSize {
    /**
     * A native signed integer type with at least \a k bytes, where \a k is
     * the template parameter.
     *
     * The default is \c void, which indicates that Regina does not know
     * how to access an integer type of the requested size.
     */
    typedef typename IntOfSize<nextPowerOfTwo(bytes)>::type type;

    /**
     * A native unsigned integer type with at least \a k bytes, where \a k is
     * the template parameter.
     *
     * The default is \c void, which indicates that Regina does not know
     * how to access an integer type of the requested size.
     */
    typedef typename IntOfSize<nextPowerOfTwo(bytes)>::utype utype;
};

#ifdef __DOXYGEN
    /**
     * Defined if and only if native 128-bit arithmetic is available on
     * this platform.
     *
     * If this macro is defined, then you can access native signed and
     * unsigned 128-bit integers through the types IntOfSize<16>::type
     * and IntOfSize<16>::utype respectively.
     *
     * If this macro is not defined, then the types IntOfSize<16>::type and
     * IntOfSize<16>::utype will both be \c void.
     */
    #define INT128_AVAILABLE
#else
template <>
struct IntOfSize<1> {
    typedef int8_t type;
    typedef uint8_t utype;
};

template <>
struct IntOfSize<2> {
    typedef int16_t type;
    typedef uint16_t utype;
};

template <>
struct IntOfSize<4> {
    typedef int32_t type;
    typedef uint32_t utype;
};

template <>
struct IntOfSize<8> {
    typedef int64_t type;
    typedef uint64_t utype;
};

#if defined(USE_BOOST_INT128)
    #if defined(BOOST_HAS_INT128)
        #define INT128_AVAILABLE
        template <>
        struct IntOfSize<16> {
            typedef boost::int128_type type;
            typedef boost::uint128_type utype;
        };
    #else
        #undef INT128_AVAILABLE
        template <>
        struct IntOfSize<16> {
            typedef void type;
            typedef void utype;
        };
    #endif
#elif defined(INTERNAL___INT128_T_FOUND)
    #define INT128_AVAILABLE
    template <>
    struct IntOfSize<16> {
        typedef __int128_t type;
        typedef __uint128_t utype;
    };
#elif defined(INTERNAL_INT128_T_FOUND)
    #define INT128_AVAILABLE
    template <>
    struct IntOfSize<16> {
        typedef int128_t type;
        typedef uint128_t utype;
    };
#else
    #undef INT128_AVAILABLE
    template <>
    struct IntOfSize<16> {
        typedef void type;
        typedef void utype;
    };
#endif

#endif // __DOXYGEN

} // namespace regina

#endif

