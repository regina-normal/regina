
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

/*! \file utilities/intutils.h
 *  \brief Miscellaneous utility classes for analysing the built-in
 *  integer types at compile time.
 */

#ifndef __REGINA_INTUTILS_H
#ifndef __DOXYGEN
#define __REGINA_INTUTILS_H
#endif

#include "regina-core.h"
#include "regina-config.h"
#include <cstdint>
#include <limits>
#include <type_traits>

namespace regina {

template <bool> class IntegerBase;
template <int> class NativeInteger;

/**
 * A compile-time boolean constant that indicates whether the type \a T is a
 * native C++ integer type, allowing for 128-bit integers also.
 *
 * This is true precisely when either `std::is_integral_v<T>` is true and/or
 * \a T is a native 128-bit integer.
 *
 * The only reason for using this constant (as opposed to
 * `std::is_integral_v<T>`) is because the C++ standard constants treat
 * 128-bit integers differently under different compilers.
 *
 * \nopython
 *
 * \ingroup utilities
 */
template <typename T>
#if defined(INTERNAL___INT128_FOUND)
    constexpr bool is_cpp_integer_v = std::is_integral_v<T> ||
        std::is_same_v<T, __int128> || std::is_same_v<T, __uint128>;
#elif defined(INTERNAL___INT128_T_FOUND)
    constexpr bool is_cpp_integer_v = std::is_integral_v<T> ||
        std::is_same_v<T, __int128_t> || std::is_same_v<T, __uint128_t>;
#elif defined(INTERNAL_INT128_T_FOUND)
    constexpr bool is_cpp_integer_v = std::is_integral_v<T> ||
        std::is_same_v<T, int128_t> || std::is_same_v<T, uint128_t>;
#else
    constexpr bool is_cpp_integer_v = std::is_integral_v<T>;
#endif

/**
 * A compile-time boolean constant that indicates whether the type \a T is an
 * unsigned native C++ integer type, allowing for 128-bit integers also.
 *
 * This is true precisely when (i) either `std::is_integral_v<T>` is true
 * and/or \a T is a native 128-bit integer, and (ii) \a T is an unsigned type.
 *
 * The only reason for using this constant (as opposed to
 * `std::is_integral_v<T>` and `std::is_unsigned_v<T>`)
 * is because the C++ standard constants treat 128-bit integers differently
 * under different compilers.
 *
 * \nopython
 *
 * \ingroup utilities
 */
template <typename T>
#if defined(INTERNAL___INT128_FOUND)
    constexpr bool is_unsigned_cpp_integer_v =
        (std::is_integral_v<T> && std::is_unsigned_v<T>) ||
        std::is_same_v<T, __uint128>;
#elif defined(INTERNAL___INT128_T_FOUND)
    constexpr bool is_unsigned_cpp_integer_v =
        (std::is_integral_v<T> && std::is_unsigned_v<T>) ||
        std::is_same_v<T, __uint128_t>;
#elif defined(INTERNAL_INT128_T_FOUND)
    constexpr bool is_unsigned_cpp_integer_v =
        (std::is_integral_v<T> && std::is_unsigned_v<T>) ||
        std::is_same_v<T, uint128_t>;
#else
    constexpr bool is_unsigned_cpp_integer_v =
        (std::is_integral_v<T> && std::is_unsigned_v<T>);
#endif

/**
 * Determines if the type \a T is one of Regina's own integer types
 * (either arbitrary precision or fixed size).
 *
 * This is true precisely when \a T is one of the classes Integer,
 * LargeInteger, or NativeInteger<...>.
 *
 * The result will be available through the compile-time boolean constant
 * IsReginaInteger<T>::value.
 *
 * \nopython
 *
 * \ingroup utilities
 */
template <typename T>
struct IsReginaInteger : public std::false_type {};

#ifndef __DOXYGEN
template <bool withInfinity>
struct IsReginaInteger<IntegerBase<withInfinity>> : public std::true_type {};

template <int bytes>
struct IsReginaInteger<NativeInteger<bytes>> : public std::true_type {};
#endif // __DOXYGEN

/**
 * Determines if the type \a T is one of Regina's arbitrary precision
 * integer types.
 *
 * This is true only when \a T is one of the classes Integer or LargeInteger.
 *
 * The result will be available through the compile-time boolean constant
 * IsReginaArbitraryPrecisionInteger<T>::value.
 *
 * \nopython
 *
 * \ingroup utilities
 */
template <typename T>
struct IsReginaArbitraryPrecisionInteger : public std::false_type {};

#ifndef __DOXYGEN
template <bool withInfinity>
struct IsReginaArbitraryPrecisionInteger<IntegerBase<withInfinity>> : public std::true_type {};
#endif // __DOXYGEN

#ifndef __DOCSTRINGS
/**
 * Conditionally enables a member function for a template class only
 * when the type \a T is one of Regina's own integer classes.
 *
 * This macro should be used as the return type for such a member function.
 * If \a T is one of Regina's own integer classes (Integer, LargeInteger
 * or NativeInteger), then the actual return type for the member function
 * will be the argument \a returnType.  Otherwise the member function will
 * be disabled, will not appear in the class at all, and will not generate
 * compile errors if it uses operations that \a T does not support.
 *
 * The implementation uses SFINAE to remove the member function without
 * compile errors.  A side-effect of this is that the member function will
 * now be a _template_ member function.  The user should never specify their
 * own template arguments, and indeed the template parameter pack \a Args in
 * the implementation is there precisely to stop users from doing this.
 *
 * \deprecated This macro is no longer used within Regina, since it makes code
 * unnecessarily difficult to read (especially by automated documentation
 * tools).  Regina's approach now is simply to implement the member function
 * in the natural way and use a static_assert to ensure it is only
 * instantiated with appropriate types.
 *
 * \pre The member function this macro is applied to is _not_ a
 * template member function (though, as noted above, this macro will
 * silently make it one).
 *
 * \apinotfinal
 *
 * \ingroup utilities
 */
#define ENABLE_MEMBER_FOR_REGINA_INTEGER(T, returnType) \
    template <typename... Args, typename Return = returnType> \
    std::enable_if_t<IsReginaInteger<T>::value, Return>
#else
// When generating docstrings, we want docs for all member functions.
#define ENABLE_MEMBER_FOR_REGINA_INTEGER(T, returnType) returnType
#endif

/**
 * Returns the number of bits required to store integers in the range
 * 0,...,<i>n</i>-1.
 * This is simply the number of bits in the binary expansion of <i>n</i>-1.
 *
 * If \a n is non-positive then this function will return 0.
 *
 * \python In Python, this routine fixes the integer type
 * \a IntType to be \c long.
 *
 * \tparam IntType any integer type, such as \c int, \c long, and so on.
 *
 * \param n any integer.
 * \return the number of bits required to store 0,...,<i>n</i>-1.
 *
 * \ingroup utilities
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
 * \python In Python, this routine fixes the integer type
 * \a IntType to be \c long.
 *
 * \warning Even though the return value is the same type as the
 * argument \a n, this routine may still overflow.  For example, if
 * \a IntType is a signed char then nextPowerOfTwo(127) will return -128, and
 * if \a IntType is an unsigned char then nextPowerOfTwo(255) will return 0.
 * Be sure that \a IntType is large enough for your requirements.
 *
 * \tparam IntType any integer type, such as \c int, \c long, and so on.
 *
 * \param n any integer.
 * \return the smallest integer power of two that is ≥ \a n.
 *
 * \ingroup utilities
 */
template <typename IntType>
constexpr IntType nextPowerOfTwo(IntType n) {
    return (n <= 1 ? 1 : (nextPowerOfTwo((n + 1) / 2)) << 1);
}

/**
 * The largest integer of the given type that can be multiplied by \a coeff
 * without overflowing.
 *
 * The template parameter \a IntType may be any native C++ integer type, such
 * as \c int, \c long, and so on.  This type may be either signed or unsigned,
 * but it must be supported by std::numeric_limits.
 *
 * The template parameter \a coeff can be any positive integer.
 *
 * \nopython This is because Python does not support templates.
 *
 * \ingroup utilities
 */
template <typename IntType, IntType coeff>
inline constexpr IntType maxSafeFactor =
    std::numeric_limits<IntType>::max() / coeff;

/**
 * The largest integer of the given type that can be multiplied by \a coeff
 * without overflowing.
 *
 * The template parameter \a IntType may be any native C++ integer type, such
 * as \c int, \c long, and so on.  This type may be either signed or unsigned,
 * but it must be supported by std::numeric_limits.
 *
 * The template parameter \a coeff can be any positive integer.
 *
 * \nopython This is because Python does not support templates.
 *
 * \ingroup utilities
 */
template <typename IntType, IntType coeff>
inline constexpr IntType minSafeFactor =
    std::numeric_limits<IntType>::min() / coeff;

/**
 * Gives access to native integer types that hold _exactly_ \a k bytes,
 * where \a k may be any compile-time constant.
 *
 * \tparam bytes the exact number of bytes in the native integer types
 * (i.e., the integer \a k described above).
 *
 * \nopython
 *
 * \see IntOfMinSize
 *
 * \ingroup utilities
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
    using type = void;

    /**
     * A native unsigned integer type with exactly \a k bytes, where \a k is the
     * template parameter.
     *
     * The default is \c void, which indicates that Regina does not know
     * how to access an integer type of the requested size.
     */
    using utype = void;
};

/**
 * Gives access to native integer types that hold _at least_ \a k bytes,
 * where \a k may be any compile-time constant.
 *
 * \tparam bytes the minimum number of bytes in the native integer types
 * (i.e., the integer \a k described above).
 *
 * \nopython
 *
 * \see IntOfSize
 *
 * \ingroup utilities
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
    using type = typename IntOfSize<nextPowerOfTwo(bytes)>::type;

    /**
     * A native unsigned integer type with at least \a k bytes, where \a k is
     * the template parameter.
     *
     * The default is \c void, which indicates that Regina does not know
     * how to access an integer type of the requested size.
     */
    using utype = typename IntOfSize<nextPowerOfTwo(bytes)>::utype;
};

/**
 * Gives access to native integer types that hold _at least_ \a k bits,
 * where \a k may be any compile-time constant.
 *
 * \tparam bits the minimum number of bits in the native integer types
 * (i.e., the integer \a k described above).
 *
 * \nopython
 *
 * \see IntOfSize
 *
 * \ingroup utilities
 */
template <int bits>
using IntOfMinBits = IntOfMinSize<(bits + 7) / 8>;

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
    using type = int8_t;
    using utype = uint8_t;
};

template <>
struct IntOfSize<2> {
    using type = int16_t;
    using utype = uint16_t;
};

template <>
struct IntOfSize<4> {
    using type = int32_t;
    using utype = uint32_t;
};

template <>
struct IntOfSize<8> {
    using type = int64_t;
    using utype = uint64_t;
};

#if defined(INTERNAL___INT128_FOUND)
    #define INT128_AVAILABLE
    template <>
    struct IntOfSize<16> {
        using type = __int128;
        using utype = __uint128;
    };
#elif defined(INTERNAL___INT128_T_FOUND)
    #define INT128_AVAILABLE
    template <>
    struct IntOfSize<16> {
        using type = __int128_t;
        using utype = __uint128_t;
    };
#elif defined(INTERNAL_INT128_T_FOUND)
    #define INT128_AVAILABLE
    template <>
    struct IntOfSize<16> {
        using type = int128_t;
        using utype = uint128_t;
    };
#else
    #undef INT128_AVAILABLE
    template <>
    struct IntOfSize<16> {
        using type = void;
        using utype = void;
    };
#endif

#endif // __DOXYGEN

/**
 * Determines if an integer of type \a From can always be assigned to an
 * integer of type \a To with no loss of information.
 *
 * The result will be available through the compile-time boolean constant
 * FaithfulAssignment<From, To>::value.
 *
 * Currently this is only implemented for Regina's own integer types
 * (Integer, LargeInteger and NativeInteger).  If you attempt to use this
 * with other types (e.g., int or long), this struct will be undefined.
 *
 * \nopython
 *
 * \ingroup utilities
 */
template <typename From, typename To>
struct FaithfulAssignment;

#ifndef __DOXYGEN
template <int a, int b>
struct FaithfulAssignment<NativeInteger<a>, NativeInteger<b>> :
    public std::integral_constant<bool, (a <= b)> {};

template <bool a, bool b>
struct FaithfulAssignment<IntegerBase<a>, IntegerBase<b>> :
    public std::integral_constant<bool, (b || ! a)> {};

template <int bytes, bool withInfinity>
struct FaithfulAssignment<NativeInteger<bytes>, IntegerBase<withInfinity>> :
    public std::true_type {};

template <int bytes, bool withInfinity>
struct FaithfulAssignment<IntegerBase<withInfinity>, NativeInteger<bytes>> :
    public std::false_type {};

#endif // __DOXYGEN

} // namespace regina

#endif

