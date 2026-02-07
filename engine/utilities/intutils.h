
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
#include <concepts>
#include <cstdint>
#include <limits>
#include <type_traits>

namespace regina {

template <bool> class IntegerBase;
template <int> class NativeInteger;

/**
 * A compile-time boolean constant that indicates whether the type \a T is a
 * native C++ integer type, allowing for 128-bit integers also but excluding
 * booleans.
 *
 * Except for booleans, this is true precisely when either
 * `std::is_integral_v<T>` is true and/or \a T is a native 128-bit integer.
 *
 * Regina treats booleans differently: `is_cpp_integer_v<bool>` is false,
 * even though C++ `std::is_integral_v<bool>` is true, since Regina's
 * functions aim to identify native types that _behave_ like integers
 * arithmetically.
 *
 * The main reason for using this constant (as opposed to
 * `std::is_integral_v<T>`) is because the C++ standard constants treat
 * 128-bit integers differently under different compilers.
 *
 * \nopython
 *
 * \ingroup utilities
 */
template <typename T>
#ifdef INT128_AVAILABLE
    constexpr bool is_cpp_integer_v = (std::is_integral_v<T> ||
        std::is_same_v<T, Int128> || std::is_same_v<T, UInt128>) &&
        ! std::is_same_v<T, bool>;
#else
    constexpr bool is_cpp_integer_v = std::is_integral_v<T> &&
        ! std::is_same_v<T, bool>;
#endif

/**
 * A compile-time boolean constant that indicates whether the type \a T is a
 * signed native C++ integer type, allowing for 128-bit integers also but
 * excluding booleans.
 *
 * Except for booleans, this is true precisely when (i) either
 * `std::is_integral_v<T>` is true and/or \a T is a native 128-bit integer,
 * and (ii) \a T is a signed type.
 *
 * Regina treats booleans differently: `is_signed_cpp_integer_v<bool>` is
 * false, even though C++ `std::is_integral_v<bool>` is true, since Regina's
 * functions aim to identify native types that _behave_ like integers
 * arithmetically.
 *
 * The main reason for using this constant (as opposed to
 * `std::is_integral_v<T>` and `std::is_signed_v<T>`) is because the
 * C++ standard constants treat 128-bit integers differently under
 * different compilers.
 *
 * \nopython
 *
 * \ingroup utilities
 */
template <typename T>
#ifdef INT128_AVAILABLE
    constexpr bool is_signed_cpp_integer_v =
        ((std::is_integral_v<T> && std::is_signed_v<T>) ||
        std::is_same_v<T, Int128>) && ! std::is_same_v<T, bool>;
#else
    constexpr bool is_signed_cpp_integer_v =
        (std::is_integral_v<T> && std::is_signed_v<T>) &&
        ! std::is_same_v<T, bool>;
#endif

/**
 * A compile-time boolean constant that indicates whether the type \a T is an
 * unsigned native C++ integer type, allowing for 128-bit integers also but
 * excluding booleans.
 *
 * Except for booleans, this is true precisely when (i) either
 * `std::is_integral_v<T>` is true and/or \a T is a native 128-bit integer,
 * and (ii) \a T is an unsigned type.
 *
 * Regina treats booleans differently: `is_unsigned_cpp_integer_v<bool>` is
 * false, even though C++ `std::is_integral_v<bool>` and
 * `std::is_unsigned_v<bool>` are both true, since Regina's functions aim to
 * identify native types that _behave_ like integers arithmetically.
 *
 * The main reason for using this constant (as opposed to
 * `std::is_integral_v<T>` and `std::is_unsigned_v<T>`)
 * is because the C++ standard constants treat 128-bit integers differently
 * under different compilers.
 *
 * \nopython
 *
 * \ingroup utilities
 */
template <typename T>
#ifdef INT128_AVAILABLE
    constexpr bool is_unsigned_cpp_integer_v =
        ((std::is_integral_v<T> && std::is_unsigned_v<T>) ||
        std::is_same_v<T, UInt128>) && ! std::is_same_v<T, bool>;
#else
    constexpr bool is_unsigned_cpp_integer_v =
        (std::is_integral_v<T> && std::is_unsigned_v<T>) &&
        ! std::is_same_v<T, bool>;
#endif

/**
 * One of the standard non-boolean C++ integer types, without making any
 * special accommodations for 128-bit integer compiler extensions.
 *
 * This concept is exactly like `std::integral` but with `bool` excluded.
 *
 * Note that 128-bit integers (which are not standard C++) might or might not
 * pass this test, depending on your compiler.
 *
 * \ingroup utilities
 */
template <typename T>
concept StandardCppInteger = std::integral<T> && ! std::same_as<T, bool>;

/**
 * A native non-boolean C++ integer type, allowing for 128-bit integers also
 * if these are supported by the compiler.
 *
 * See the constant regina::is_cpp_integer_v for further details.
 *
 * \ingroup utilities
 */
template <typename T>
concept CppInteger = is_cpp_integer_v<T>;

/**
 * A signed native non-boolean C++ integer type, allowing for 128-bit integers
 * also if these are supported by the compiler.
 *
 * See the constant regina::is_signed_cpp_integer_v for further details.
 *
 * \ingroup utilities
 */
template <typename T>
concept SignedCppInteger = is_signed_cpp_integer_v<T>;

/**
 * An unsigned native non-boolean C++ integer type, allowing for 128-bit
 * integers also if these are supported by the compiler.
 *
 * See the constant regina::is_unsigned_cpp_integer_v for further details.
 *
 * \ingroup utilities
 */
template <typename T>
concept UnsignedCppInteger = is_unsigned_cpp_integer_v<T>;

/**
 * One of Regina's arbitrary precision integer types (Integer or LargeInteger).
 *
 * \ingroup utilities
 */
template <typename T>
concept ArbitraryPrecisionInteger =
    std::is_same_v<IntegerBase<true>, T> ||
    std::is_same_v<IntegerBase<false>, T>;

/**
 * One of Regina's own integer types (Integer, LargeInteger, or NativeInteger).
 *
 * An important feature of all of Regina's integer types is that their default
 * constructors initialise the integers to zero.
 *
 * \ingroup utilities
 */
template <typename T>
concept ReginaInteger =
    ArbitraryPrecisionInteger<T> ||
    requires(T x) { { NativeInteger(x) } -> std::same_as<T>; };

/**
 * Either any standard non-boolean C++ integer type or any of Regina's own
 * integer types.
 *
 * This concept excludes `bool`, and does not make any special accommodations
 * for 128-bit integer compiler extensions.
 *
 * \ingroup utilities
 */
template <typename T>
concept AnyInteger = StandardCppInteger<T> || ReginaInteger<T>;

/**
 * A type that supports very basic interoperability with native C++ integer
 * values, via construction, assignment, and equality/inequality testing.
 *
 * \ingroup utilities
 */
template <typename T>
concept IntegerCompatible =
    std::constructible_from<T, int> &&
    std::assignable_from<T&, int> &&
    std::equality_comparable_with<T, int>;

/**
 * A type that supports interoperability with native C++ integer values via
 * construction, assignment, equality/inequality testing, and comparisons.
 * The comparisons must yield a total order.
 *
 * \ingroup utilities
 */
template <typename T>
concept IntegerComparable =
    IntegerCompatible<T> &&
    std::totally_ordered_with<T, int>;

/**
 * Deprecated traits class to determine if the type \a T is one of Regina's
 * own integer types (either arbitrary precision or fixed size).
 *
 * This is true precisely when \a T is one of the classes Integer,
 * LargeInteger, or NativeInteger<...>.
 *
 * The result will be available through the compile-time boolean constant
 * IsReginaInteger<T>::value.
 *
 * \deprecated Instead use the concept `ReginaInteger<T>`.
 *
 * \nopython
 *
 * \ingroup utilities
 */
template <typename T>
struct [[deprecated]] IsReginaInteger : public std::false_type {};

#ifndef __DOXYGEN
template <bool withInfinity>
struct IsReginaInteger<IntegerBase<withInfinity>> : public std::true_type {};

template <int bytes>
struct IsReginaInteger<NativeInteger<bytes>> : public std::true_type {};
#endif // __DOXYGEN

/**
 * Deprecated traits class to determine if the type \a T is one of Regina's
 * arbitrary precision integer types.
 *
 * This is true only when \a T is one of the classes Integer or LargeInteger.
 *
 * The result will be available through the compile-time boolean constant
 * IsReginaArbitraryPrecisionInteger<T>::value.
 *
 * \deprecated Instead use the concept `ArbitraryPrecisionInteger<T>`.
 *
 * \nopython
 *
 * \ingroup utilities
 */
template <typename T>
struct [[deprecated]] IsReginaArbitraryPrecisionInteger : public std::false_type {};

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
 * tools).  Regina now uses C++20 concepts instead to constraint access to
 * member functions.
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
    std::enable_if_t<ReginaInteger<T>, Return>
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
 * \nopython This is because Python does not support templates.
 *
 * \ingroup utilities
 */
template <CppInteger IntType, IntType coeff> requires (coeff > 0)
inline constexpr IntType maxSafeFactor =
    std::numeric_limits<IntType>::max() / coeff;

/**
 * The largest integer of the given type that can be multiplied by \a coeff
 * without overflowing.
 *
 * \nopython This is because Python does not support templates.
 *
 * \ingroup utilities
 */
template <CppInteger IntType, IntType coeff> requires (coeff > 0)
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

#ifndef __DOXYGEN
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

#ifdef INT128_AVAILABLE
template <>
struct IntOfSize<16> {
    using type = Int128;
    using utype = UInt128;
};
#endif

#endif // __DOXYGEN

/**
 * Converts the given native C++ integer type into a signed integer type of
 * the same size, allowing for 128-bit integers also.
 *
 * This is like `std::make_signed_t<T>` but with explicit support for 128-bit
 * integers on those platforms that support them.  (In contrast some compilers
 * support 128-bit integers but do not implement `std::make_signed_t for them.)
 *
 * \nopython
 *
 * \ingroup utilities
 */
template <CppInteger T>
using make_signed_cpp_t = typename IntOfSize<sizeof(T)>::type;

/**
 * Converts the given native C++ integer type into an unsigned integer type of
 * the same size, allowing for 128-bit integers also.
 *
 * This is like `std::make_unsigned_t<T>` but with explicit support for 128-bit
 * integers on those platforms that support them.  (In contrast some compilers
 * support 128-bit integers but do not implement `std::make_unsigned_t for
 * them.)
 *
 * \nopython
 *
 * \ingroup utilities
 */
template <CppInteger T>
using make_unsigned_cpp_t = typename IntOfSize<sizeof(T)>::utype;

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

