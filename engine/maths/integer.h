
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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

#ifndef __REGINA_INTEGER_H
#ifndef __DOXYGEN
#define __REGINA_INTEGER_H
#endif

/*! \file maths/integer.h
 *  \brief Provides arbitrary-precision and fixed-precision integer types.
 */

#include <climits>
#include <cstdint> // MPIR (and thus SAGE) needs this *before* gmp.h.
#include <cstddef> // OSX needs this before gmp.h to avoid a ::ptrdiff_t error.
#include <limits>
#include <tuple>
#include <gmp.h>
#include "regina-core.h"
#include "concepts/core.h"
#include "maths/ring.h"
#include "utilities/exception.h"
#include "utilities/stringutils.h"
#include "utilities/tightencoding.h"

ENSURE_ESSENTIAL_REGINA_HEADERS

// Regina assumes in many places that a byte contains exactly 8 bits.
// I believe this mandated for POSIX systems.  I'm not sure if anyone has ever
// tried to build regina on a platform where this assumption fails, but the
// C++ standard allows other byte sizes and so we should enforce it somewhere.
static_assert(CHAR_BIT == 8, "Regina works under the assumption that a byte "
    "is precisely 8 bits, which is not true on your platform.  Please contact "
    "the Regina developers.");

namespace regina {

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
class NativeInteger;

#ifdef __DOCSTRINGS
class python_int; // Represents a Python arbitrary-precision integer.
#endif

namespace detail {
    /**
     * An empty type used to indicate that we are working with infinity.
     */
    struct InfiniteTag {};

    /**
     * An empty type used to indicate that we are working with GMP large
     * integer representations.
     */
    struct GMPTag {};

    /**
     * A compile-time constant equal to `|LONG_MIN|`.
     */
    static constexpr unsigned long absLongMin =
        static_cast<unsigned long>(LONG_MAX) + 1;

    /**
     * Holds the internal data for one of Regina's arbitrary precision integers.
     *
     * The enclosing integer class is responsible for tracking which of the
     * various representations is currently in use.
     *
     * The only reason this IntegerData type is declared externally (as opposed
     * to being a private inner type within IntegerBase) is so that Integer and
     * LargeInteger can swap data (e.g., during move operations).
     */
    union IntegerData {
        long native_;
            /**< A native C++ integer representation. */
        mpz_t gmp_;
            /**< A GMP large integer representation. */

        /**
         * Leaves this data uninitialised.
         */
        IntegerData() = default;
        /**
         * Initialises the member `.native_` to the given native integer value.
         */
        IntegerData(long native) : native_(native) {}
    };
}

/**
 * Represents an arbitrary precision integer.
 * Calculations are always guaranteed to be exact, regardless of how
 * large the integers become.
 *
 * The current implementation uses fast native integer arithmetic wherever
 * possible, whilst always testing for potential overflow.  If a potential
 * overflow is detected, this class switches to using the GNU multiple
 * precision arithmetic library (libgmp) instead.
 *
 * This class takes a single boolean argument \a withInfinity.
 * If this is \c true, then this class will support infinity as an allowed
 * value.  If this is \c false (the default), then infinity is not supported,
 * and any attempt to work with infinity will lead to undefined behaviour.
 * Supporting infinity is more flexible, but also comes with a slight
 * performance cost (very roughly estimated at around 10%-20%).
 *
 * For the purposes of comparison, infinity is
 * considered larger than any other integer but equal to itself.
 *
 * All routines in this class, including random number generation, are
 * thread-safe.
 *
 * The opportunistic use of native arithmetic where possible was inspired by
 * the (much more complex and powerful) lazy exact arithmetic in CGAL.
 * Thanks to Menelaos Karavelas for encouraging me to take another look at
 * these ideas.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.  Moving from an arbitrary
 * precision integer will leave the source object in a valid but undefined
 * state.
 *
 * \headers Parts of this template class are implemented in a C++ source
 * file that is not available through the headers.  However, this should
 * not affect users since the calculation engine includes explicit
 * instantiations for all possible template parameters.
 *
 * \python Both variants of this template are available through Python.
 * For \a withInfinity = \c false, use the name Integer.
 * For \a withInfinity = \c true, use the name LargeInteger.
 *
 * \ingroup maths
 */
template <bool withInfinity = false>
class IntegerBase {
    public:
        /**
         * A compile-time constant indicating whether this integer type
         * supports infinity.  This is provided to help with generic code.
         */
        static constexpr bool supportsInfinity = withInfinity;

        static const IntegerBase zero;
            /**< Globally available zero. */
        static const IntegerBase one;
            /**< Globally available one. */
        static const IntegerBase infinity;
            /**< Globally available infinity.
                 This is only defined if \a withInfinity is \c true.
                 Any attempt to use it when \a withInfinity is \c false
                 should generate a linker error. */

    private:
        /**
         * Possible values for \a rep_.  We deliberately use an unscoped
         * enumeration because we really just want a suite of private
         * compile-time integer constants.
         */
        enum {
            REP_NATIVE = 0,
                /**< Indicates that this integer uses a native representation
                     stored in `d_.native_`. */
            REP_GMP = 1,
                /**< Indicates that this integer uses a GMP large integer
                     representation stored in `d_.gmp_`. */
            REP_INFINITE = 2
                /**< Indicates that this integer is infinite.  This is only
                     allowed if \a withInfinity is `true`. */
        };

        /**
         * Indicates the internal representation of this integer.
         * This must be one of the `REP_...` constants; moreover, the
         * value *REP_INFINITE* is allowed only if \a withInfinity
         * is `true`.  A common pattern is to test \a rep_ as a boolean,
         * which is enough to distinguish between a native (`false`)
         * versus non-native (`true`) representation.
         */
        int rep_;

        /**
         * Holds the internal representation of this integer (whatever
         * representation that is).
         *
         * We require at all times that `d_.gmp_` is GMP-initialised
         * if and only if `rep_ == REP_GMP`.
         */
        detail::IntegerData d_;

    public:
        /**
         * Initialises this integer to zero.
         */
        IntegerBase();
        /**
         * Initialises this integer to the given native C++ value.
         *
         * \python It is assumed that the type \a IntType is \c long.
         *
         * \param value the new value of this integer.
         */
        template <CppInteger IntType>
        IntegerBase(IntType value);
        /**
         * Initialises this integer to the given value.
         *
         * \param value the new value of this integer.
         */
        IntegerBase(const IntegerBase& value);
        /**
         * Initialises this integer to the given value.
         *
         * \exception InvalidArgument This class does not support infinity,
         * but the given integer is infinite.
         *
         * \param value the new value of this integer.
         */
        IntegerBase(const IntegerBase<! withInfinity>& value);
        /**
         * Moves the given integer into this new integer.
         * This is a fast (constant time) operation.
         *
         * \param src the integer to move.
         */
        IntegerBase(IntegerBase&& src) noexcept;
        /**
         * Moves the given integer into this new integer.
         * This is a fast (constant time) operation.
         *
         * \exception InvalidArgument This class does not support infinity,
         * but the given integer is infinite.
         *
         * \param src the integer to move.
         */
        IntegerBase(IntegerBase<! withInfinity>&& src);
        /**
         * Initialises this integer to the given value.
         *
         * \nopython This is because the NativeInteger classes are not
         * available to Python users.
         *
         * \param value the new value of this integer.
         */
        template <int bytes>
        IntegerBase(const NativeInteger<bytes>& value);
#ifdef __APIDOCS
        /**
         * Initialises this to the given native Python integer.
         *
         * The argument is of the Python \c int type, which Python uses
         * to store integers of arbitrary magnitude (much like Regina does
         * with its Integer and LargeInteger classes).
         *
         * \nocpp
         *
         * \param value the new value of this integer.
         */
        IntegerBase(python_int value);
#endif

        /**
         * Initialises this integer to the truncation of the given
         * real number.
         *
         * \param value the real number to be truncated.
         */
        IntegerBase(double value);
        /**
         * Initialises this integer to the given value which is
         * represented as a string of digits in a given base.
         *
         * If not specified, the base defaults to 10.
         * If the given base is zero, the base will be automatically
         * determined.  If the given string begins with \c 0x or \c 0X,
         * the base will be assumed to be 16.  Otherwise, if the string
         * begins with \c 0, the base will be assumed to be 8.
         * Otherwise it will be taken as base 10.
         *
         * If the template argument \a withInfinity is \c true, then
         * any string beginning with "inf" (after any initial whitesapce)
         * will be interpreted as infinity.
         *
         * Whitespace may be present at the beginning or the end
         * of the given string, and will simply be ignored.
         *
         * For finer details on how the string parsing works, see
         * strtol() from the standard C library (on which this method
         * is based).
         *
         * \pre The given base is zero, or is between 2 and 36 inclusive.
         * \pre The given string represents an integer
         * in the given base, with optional whitespace beforehand.
         *
         * \exception InvalidArgument The given string was not a valid
         * large integer representation.
         *
         * \param value the new value of this integer, represented as a string
         * of digits in base \a base.
         * \param base the base in which \a value is given.
         */
        IntegerBase(const char* value, int base = 10);
        /**
         * Initialises this integer to the given value which is
         * represented as a string of digits in a given base.
         *
         * If not specified, the base defaults to 10.
         * If the given base is zero, the base will be automatically
         * determined.  If the given string begins with \c 0x or \c 0X,
         * the base will be assumed to be 16.  Otherwise, if the string
         * begins with \c 0, the base will be assumed to be 8.
         * Otherwise it will be taken as base 10.
         *
         * If the template argument \a withInfinity is \c true, then
         * any string beginning with "inf" (after any initial whitesapce)
         * will be interpreted as infinity.
         *
         * Whitespace may be present at the beginning or the end
         * of the given string, and will simply be ignored.
         *
         * For finer details on how the string parsing works, see
         * strtol() from the standard C library (on which this method
         * is based).
         *
         * \pre The given base is zero, or is between 2 and 36 inclusive.
         * \pre The given string represents an integer
         * in the given base, with optional whitespace beforehand.
         *
         * \exception InvalidArgument The given string was not a valid
         * large integer representation.
         *
         * \param value the new value of this integer, represented as a string
         * of digits in base \a base.
         * \param base the base in which \a value is given.
         */
        IntegerBase(const std::string& value, int base = 10);
        /**
         * Destroys this integer.
         */
        ~IntegerBase();

        /**
         * Returns whether we are currently working with a native C/C++
         * long, or whether we have switched to GMP large integer arithmetic
         * for this integer.
         *
         * If this integer is infinite, this routine will return \c false.
         *
         * \return \c true if and only if we are still using a native
         * C/C++ long.
         */
        bool isNative() const;

        /**
         * Returns whether or not this integer is zero.
         *
         * This is micro-optimised to be faster than simply testing
         * whether (*this) == 0.
         *
         * \return \c true if and only if this integer is zero.
         */
        bool isZero() const;

        /**
         * Returns the sign of this integer.
         *
         * In this routine, infinity is considered to have sign +1.
         *
         * \return +1, -1 or 0 according to whether this integer is
         * positive, negative or zero.
         */
        int sign() const;

        /**
         * Returns whether this integer is infinity.
         *
         * \return \c true if and only if this integer is infinity.
         */
        constexpr bool isInfinite() const {
            if constexpr (withInfinity)
                return rep_ == REP_INFINITE;
            else
                return false;
        }

        /**
         * Sets this integer to be infinity.
         */
        inline void makeInfinite() requires (withInfinity) {
            if (rep_ == REP_GMP)
                mpz_clear(d_.gmp_);
            rep_ = REP_INFINITE;
        }

        /**
         * Returns the value of this integer as a native C++ integer of the
         * given type, or throws an exception if this is not possible.
         *
         * If this integer is within the required range, regardless of
         * whether the underlying representation is a native or large integer,
         * this routine will return the correct result.
         *
         * Note that both signed and unsigned native integer types are
         * supported here.
         *
         * \python It is assumed that the type \a IntType is \c long.
         *
         * \exception IntegerOverflow This integer does not fit into the range
         * of the given native C++ integer type.
         *
         * \return the value of this integer.
         */
        template <CppInteger IntType>
        IntType safeValue() const;
        /**
         * Returns the value of this integer as a native C++ integer of the
         * given type, leaving the programmer responsible for range checking.
         *
         * Note that both signed and unsigned native integer types are
         * supported here.
         *
         * \pre This integer is within the required range for the type
         * \a IntType.  It does not matter whether the underlying
         * representation for this integer is a native or large integer.
         *
         * \warning You should _only_ use this routine if you know in advance
         * that this integer is within the required range.  Otherwise you
         * should call safeValue() instead, which performs range checking and
         * throws an exception if this fails.
         *
         * \python It is assumed that the type \a IntType is \c long.
         *
         * \return the value of this integer.
         */
        template <CppInteger IntType>
        IntType unsafeValue() const;
        /**
         * Deprecated routine that returns the value of this integer as a
         * native C++ \c long, or throws an exception if this is not possible.
         *
         * If this integer is within the required range, regardless of
         * whether the underlying representation is a native or large integer,
         * this routine will return the correct result.
         *
         * \deprecated C++ users should now call `safeValue<long>()` (with a
         * different template parameter if you wish to work with a different
         * native C++ integer type instead of \c long).  Python users should
         * just call `safeValue()`.
         *
         * \exception IntegerOverflow This integer is too large or small to fit
         * into a \c long.
         *
         * \return the value of this integer.
         */
        [[deprecated]] long safeLongValue() const;
        /**
         * Deprecated routine that returns the value of this integer as a
         * native C++ \c long, with no range checking.
         *
         * \deprecated C++ users should now call `unsafeValue<long>()` (with a
         * different template parameter if you wish to work with a different
         * native C++ integer type instead of \c long).  Python users should
         * just call `unsafeValue()`.
         *
         * \pre This integer is within the required range for a native C++
         * \c long.  It does not matter whether the underlying representation
         * for this integer is a native or large integer.
         *
         * \return the value of this integer.
         */
        [[deprecated]] long longValue() const;
        /**
         * Deprecated function that returns the value of this integer as a
         * native integer of some fixed byte length, with no range checking.
         *
         * \deprecated If you want range checking, use safeValue().  If you
         * know the integer will be within range then you can use unsafeValue()
         * (which makes your responsibilities more obvious to a casual reader).
         * Note that both safeValue() and unsafeValue() take a C++ integer type
         * as their template parameter, not the number of bytes.
         *
         * \pre This integer is within the required range for the type
         * \a IntType.  It does not matter whether the underlying
         * representation for this integer is a native or large integer.
         *
         * \nopython Python does not have the diversity of integer types that
         * C++ does, and so this function is not so important.  Python users
         * can simply call safeValue(), unsafeValue(), or pythonValue() instead.
         *
         * \tparam bytes the exact number of bytes in the native C++ integer
         * type to return.
         *
         * \return the value of this integer.
         */
        template <int bytes>
        requires (supportsNativeIntegerSize(bytes))
        [[deprecated]] typename IntOfSize<bytes>::type nativeValue() const;
        /**
         * Returns the value of this integer as a string in the given
         * base.  If not specified, the base defaults to 10.
         *
         * If this integer is infinity, the string returned will be `inf`.
         *
         * \pre The given base is between 2 and 36 inclusive.
         *
         * \return the value of this integer as a string.
         */
        std::string stringValue(int base = 10) const;
        /**
         * Returns the value of this integer as a string in base 10.
         *
         * Calling str() is identical to calling stringValue() (though,
         * unlike stringValue(), str() has no option to change the base).
         * This alias str() is provided for consistency with the many other
         * classes in Regina that provide a str() function.
         *
         * If this integer is infinity, the string returned will be `inf`.
         *
         * \return the value of this integer as a string in base 10.
         */
        std::string str() const;
#ifdef __APIDOCS
        /**
         * Returns the value of this integer as a native Python integer.
         *
         * The return value will be of the Python \c int type, which Python
         * uses to store integers of arbitrary magnitude (much like Regina
         * does with its Integer and LargeInteger classes).
         *
         * \pre This integer is not infinity.
         *
         * \nocpp
         *
         * \return the value of this integer as a Python integer.
         */
        python_int pythonValue() const;
#endif

        /**
         * Sets this integer to the given value.
         *
         * \param value the new value of this integer.
         * \return a reference to this integer with its new value.
         */
        IntegerBase& operator =(const IntegerBase& value);
        /**
         * Sets this integer to the given value.
         *
         * \exception InvalidArgument This class does not support infinity,
         * but the given integer is infinite.
         *
         * \param value the new value of this integer.
         * \return a reference to this integer with its new value.
         */
        IntegerBase& operator = (const IntegerBase<! withInfinity>& value);
        /**
         * Moves the given integer into this integer.
         * This is a fast (constant time) operation.
         *
         * \param src the integer to move.
         * \return a reference to this integer.
         */
        IntegerBase& operator = (IntegerBase&& src) noexcept;
        /**
         * Moves the given integer into this integer.
         * This is a fast (constant time) operation.
         *
         * \exception InvalidArgument This class does not support infinity,
         * but the given integer is infinite.
         *
         * \param src the integer to move.
         * \return a reference to this integer.
         */
        IntegerBase& operator = (IntegerBase<! withInfinity>&& src);
        /**
         * Sets this integer to the given native C++ value.
         *
         * \param value the new value of this integer.
         * \return a reference to this integer with its new value.
         */
        template <CppInteger IntType>
        IntegerBase& operator =(IntType value);
        /**
         * Sets this integer to the given value.
         *
         * \param value the new value of this integer.
         * \return a reference to this integer with its new value.
         */
        template <int bytes>
        IntegerBase& operator =(const NativeInteger<bytes>& value);
        /**
         * Sets this integer to the given value which is
         * represented as a string of digits in base 10.
         *
         * Whitespace may be present at the beginning or end of the given
         * string and will simply be ignored.
         *
         * If the template argument \a withInfinity is \c true, then
         * any string beginning with "inf" (after any initial whitesapce)
         * will be interpreted as infinity.
         *
         * \pre The given string represents an integer
         * in base 10, with optional whitespace added.
         *
         * \exception InvalidArgument The given string was not a valid
         * large integer representation.
         *
         * \param value the new value of this integer, represented as a string
         * of digits in base 10.
         * \return a reference to this integer with its new value.
         */
        IntegerBase& operator =(const char* value);
        /**
         * Sets this integer to the given value which is
         * represented as a string of digits in base 10.
         *
         * Whitespace may be present at the beginning or end of the given
         * string and will simply be ignored.
         *
         * If the template argument \a withInfinity is \c true, then
         * any string beginning with "inf" (after any initial whitesapce)
         * will be interpreted as infinity.
         *
         * \pre The given string represents an integer
         * in base 10, with optional whitespace added.
         *
         * \exception InvalidArgument The given string was not a valid
         * large integer representation.
         *
         * \param value the new value of this integer, represented as a string
         * of digits in base 10.
         * \return a reference to this integer with its new value.
         */
        IntegerBase& operator =(const std::string& value);
        /**
         * Swaps the values of this and the given integer.
         *
         * \param other the integer whose value will be swapped with this.
         */
        void swap(IntegerBase& other) noexcept;

        /**
         * Determines if this is equal to the given integer.
         *
         * \param rhs the integer with which this will be compared.
         * \return \c true if and only if this and the given integer are
         * equal.
         */
        template <bool rhsWithInfinity>
        bool operator ==(const IntegerBase<rhsWithInfinity>& rhs) const;
        /**
         * Determines if this is equal to the given native C++ integer.
         *
         * \python It is assumed that the type \a IntType is \c long.
         *
         * \param rhs the integer with which this will be compared.
         * \return \c true if and only if this and the given integer are
         * equal.
         */
        template <CppInteger IntType>
        bool operator ==(IntType rhs) const;
        /**
         * Determines if this is equal to the given integer.
         *
         * \nopython This is because the NativeInteger classes are not
         * available to Python users.
         *
         * \param rhs the integer with which this will be compared.
         * \return \c true if and only if this and the given integer are
         * equal.
         */
        template <int bytes>
        bool operator ==(const NativeInteger<bytes>& rhs) const;
        /**
         * Compares this to the given integer.
         *
         * This is a numerical comparison; that is, it uses the usual ordering
         * of the integers. Infinity is considered greater than any finite
         * integer.
         *
         * This generates all of the usual comparison operators, including
         * `<`, `<=`, `>`, and `>=`.
         *
         * \python This spaceship operator `x <=> y` is not available, but the
         * other comparison operators that it generates _are_ available.
         *
         * \param rhs the integer with which this will be compared.
         * \return the result of the numerical comparison between this and
         * the given integer.
         */
        template <bool rhsWithInfinity>
        std::strong_ordering operator <=> (
            const IntegerBase<rhsWithInfinity>& rhs) const;
        /**
         * Compares this to the given native C++ integer.
         *
         * This is a numerical comparison; that is, it uses the usual ordering
         * of the integers. Infinity is considered greater than any integer.
         *
         * This generates all of the usual comparison operators, including
         * `<`, `<=`, `>`, and `>=`.
         *
         * \python This spaceship operator `x <=> y` is not available, but the
         * other comparison operators that it generates _are_ available.
         * It is assumed that the type \a IntType is \c long.
         *
         * \param rhs the integer with which this will be compared.
         * \return the result of the numerical comparison between this and
         * the given integer.
         */
        template <CppInteger IntType>
        std::strong_ordering operator <=> (IntType rhs) const;
        /**
         * Compares this to the given integer.
         *
         * This is a numerical comparison; that is, it uses the usual ordering
         * of the integers. Infinity is considered greater than any integer.
         *
         * This generates all of the usual comparison operators, including
         * `<`, `<=`, `>`, and `>=`.
         *
         * \nopython This is because the NativeInteger classes are not
         * available to Python users.
         *
         * \param rhs the integer with which this will be compared.
         * \return the result of the numerical comparison between this and
         * the given integer.
         */
        template <int bytes>
        std::strong_ordering operator <=> (const NativeInteger<bytes>& rhs)
            const;

        /**
         * The preincrement operator.
         * This operator increments this integer by one, and returns a
         * reference to the integer _after_ the increment.
         *
         * \nopython The postincrement operator is present in Python as the
         * member function inc().
         *
         * \return a reference to this integer after the increment.
         */
        IntegerBase& operator ++();

        /**
         * The postincrement operator.
         * This operator increments this integer by one, and returns a
         * copy of the integer _before_ the increment.
         *
         * \python This routine is named inc() since Python does not
         * support the increment operator.
         *
         * \return a copy of this integer before the increment took place.
         */
        IntegerBase operator ++(int);

        /**
         * The predecrement operator.
         * This operator decrements this integer by one, and returns a
         * reference to the integer _after_ the decrement.
         *
         * \nopython The postdecrement operator is present in Python as the
         * member function dec().
         *
         * \return a reference to this integer after the decrement.
         */
        IntegerBase& operator --();

        /**
         * The postdecrement operator.
         * This operator decrements this integer by one, and returns a
         * copy of the integer _before_ the decrement.
         *
         * \python This routine is named dec() since Python does not
         * support the decrement operator.
         *
         * \return a copy of this integer before the decrement took place.
         */
        IntegerBase operator --(int);

        /**
         * Adds this to the given integer and returns the result.
         * This integer is not changed.
         *
         * If either term of the sum is infinite, the result will be
         * infinity.
         *
         * \param other the integer to add to this integer.
         * \return the sum \a this plus \a other.
         */
        IntegerBase operator +(const IntegerBase& other) const&;
        /**
         * Adds this to the given integer and returns the result.
         * This integer is not changed.
         *
         * If either term of the sum is infinite, the result will be
         * infinity.
         *
         * \param other the integer to add to this integer.
         * \return the sum \a this plus \a other.
         */
        IntegerBase operator +(IntegerBase&& other) const&;
        /**
         * Adds this to the given integer and returns the result.
         *
         * If either term of the sum is infinite, the result will be
         * infinity.
         *
         * \param other the integer to add to this integer.
         * \return the sum \a this plus \a other.
         */
        IntegerBase operator +(const IntegerBase& other) &&;
        /**
         * Adds this to the given integer and returns the result.
         *
         * If either term of the sum is infinite, the result will be
         * infinity.
         *
         * \param other the integer to add to this integer.
         * \return the sum \a this plus \a other.
         */
        IntegerBase operator +(IntegerBase&& other) &&;
        /**
         * Adds this to the given native C++ integer and returns the result.
         * This integer is not changed.
         *
         * If either term of the sum is infinite, the result will be
         * infinity.
         *
         * \python It is assumed that the type \a IntType is \c long.
         *
         * \param other the integer to add to this integer.
         * \return the sum \a this plus \a other.
         */
        template <CppInteger IntType>
        IntegerBase operator +(IntType other) const&;
        /**
         * Adds this to the given native C++ integer and returns the result.
         *
         * If either term of the sum is infinite, the result will be
         * infinity.
         *
         * \param other the integer to add to this integer.
         * \return the sum \a this plus \a other.
         */
        template <CppInteger IntType>
        IntegerBase operator +(IntType other) &&;
        /**
         * Subtracts the given integer from this and returns the result.
         * This integer is not changed.
         *
         * If either term of the difference is infinite, the result will be
         * infinity.
         *
         * \param other the integer to subtract from this integer.
         * \return the difference \a this minus \a other.
         */
        IntegerBase operator -(const IntegerBase& other) const&;
        /**
         * Subtracts the given integer from this and returns the result.
         * This integer is not changed.
         *
         * If either term of the difference is infinite, the result will be
         * infinity.
         *
         * \param other the integer to subtract from this integer.
         * \return the difference \a this minus \a other.
         */
        IntegerBase operator -(IntegerBase&& other) const&;
        /**
         * Subtracts the given integer from this and returns the result.
         *
         * If either term of the difference is infinite, the result will be
         * infinity.
         *
         * \param other the integer to subtract from this integer.
         * \return the difference \a this minus \a other.
         */
        IntegerBase operator -(const IntegerBase& other) &&;
        /**
         * Subtracts the given integer from this and returns the result.
         *
         * If either term of the difference is infinite, the result will be
         * infinity.
         *
         * \param other the integer to subtract from this integer.
         * \return the difference \a this minus \a other.
         */
        IntegerBase operator -(IntegerBase&& other) &&;
        /**
         * Subtracts the given native C++ integer from this and returns
         * the result.
         * This integer is not changed.
         *
         * If either term of the difference is infinite, the result will be
         * infinity.
         *
         * \python It is assumed that the type \a IntType is \c long.
         *
         * \param other the integer to subtract from this integer.
         * \return the difference \a this minus \a other.
         */
        template <CppInteger IntType>
        IntegerBase operator -(IntType other) const&;
        /**
         * Subtracts the given native C++ integer from this and returns
         * the result.
         *
         * If either term of the difference is infinite, the result will be
         * infinity.
         *
         * \param other the integer to subtract from this integer.
         * \return the difference \a this minus \a other.
         */
        template <CppInteger IntType>
        IntegerBase operator -(IntType other) &&;
        /**
         * Multiplies this by the given integer and returns the result.
         * This integer is not changed.
         *
         * If either factor of the product is infinite, the result will be
         * infinity.
         *
         * \param other the integer to multiply by this integer.
         * \return the product \a this times \a other.
         */
        IntegerBase operator *(const IntegerBase& other) const&;
        /**
         * Multiplies this by the given integer and returns the result.
         * This integer is not changed.
         *
         * If either factor of the product is infinite, the result will be
         * infinity.
         *
         * \param other the integer to multiply by this integer.
         * \return the product \a this times \a other.
         */
        IntegerBase operator *(IntegerBase&& other) const&;
        /**
         * Multiplies this by the given integer and returns the result.
         *
         * If either factor of the product is infinite, the result will be
         * infinity.
         *
         * \param other the integer to multiply by this integer.
         * \return the product \a this times \a other.
         */
        IntegerBase operator *(const IntegerBase& other) &&;
        /**
         * Multiplies this by the given integer and returns the result.
         *
         * If either factor of the product is infinite, the result will be
         * infinity.
         *
         * \param other the integer to multiply by this integer.
         * \return the product \a this times \a other.
         */
        IntegerBase operator *(IntegerBase&& other) &&;
        /**
         * Multiplies this by the given native C++ integer and returns the
         * result.
         * This integer is not changed.
         *
         * If either factor of the product is infinite, the result will be
         * infinity.
         *
         * \python It is assumed that the type \a IntType is \c long.
         *
         * \param other the integer to multiply by this integer.
         * \return the product \a this times \a other.
         */
        template <CppInteger IntType>
        IntegerBase operator *(IntType other) const&;
        /**
         * Multiplies this by the given native C++ integer and returns the
         * result.
         *
         * If either factor of the product is infinite, the result will be
         * infinity.
         *
         * \param other the integer to multiply by this integer.
         * \return the product \a this times \a other.
         */
        template <CppInteger IntType>
        IntegerBase operator *(IntType other) &&;
        /**
         * Divides this by the given integer and returns the result.
         * The result will be truncated to an integer, i.e., rounded
         * towards zero.
         * This integer is not changed.
         *
         * If \a other is known to divide this integer exactly,
         * divExact() should be used instead.
         *
         * Regarding special cases:
         *
         * - infinity divided by anything will return infinity;
         * - anything divided by zero will likewise return infinity;
         * - anything finite divided by infinity will return zero.
         *
         * For a division routine that always rounds down, see divisionAlg().
         *
         * \exception DivisionByZero The argument \a other is zero, but this
         * class does not support infinity.
         *
         * \param other the integer to divide this by.
         * \return the quotient \a this divided by \a other.
         */
        IntegerBase operator /(const IntegerBase& other) const&;
        /**
         * Divides this by the given integer and returns the result.
         * The result will be truncated to an integer, i.e., rounded
         * towards zero.
         *
         * If \a other is known to divide this integer exactly,
         * divExact() should be used instead.
         *
         * Regarding special cases:
         *
         * - infinity divided by anything will return infinity;
         * - anything divided by zero will likewise return infinity;
         * - anything finite divided by infinity will return zero.
         *
         * For a division routine that always rounds down, see divisionAlg().
         *
         * \exception DivisionByZero The argument \a other is zero, but this
         * class does not support infinity.
         *
         * \param other the integer to divide this by.
         * \return the quotient \a this divided by \a other.
         */
        IntegerBase operator /(const IntegerBase& other) &&;
        /**
         * Divides this by the given native C++ integer and returns the result.
         * The result will be truncated to an integer, i.e., rounded
         * towards zero.
         * This integer is not changed.
         *
         * If \a other is known to divide this integer exactly,
         * divExact() should be used instead.
         *
         * Regarding special cases:
         *
         * - infinity divided by anything will return infinity;
         * - anything divided by zero will likewise return infinity.
         *
         * For a division routine that always rounds down, see divisionAlg().
         *
         * \python It is assumed that the type \a IntType is \c long.
         *
         * \exception DivisionByZero The argument \a other is zero, but this
         * class does not support infinity.
         *
         * \param other the integer to divide this by.
         * \return the quotient \a this divided by \a other.
         */
        template <CppInteger IntType>
        IntegerBase operator /(IntType other) const&;
        /**
         * Divides this by the given native C++ integer and returns the result.
         * The result will be truncated to an integer, i.e., rounded
         * towards zero.
         *
         * If \a other is known to divide this integer exactly,
         * divExact() should be used instead.
         *
         * Regarding special cases:
         *
         * - infinity divided by anything will return infinity;
         * - anything divided by zero will likewise return infinity.
         *
         * For a division routine that always rounds down, see divisionAlg().
         *
         * \exception DivisionByZero The argument \a other is zero, but this
         * class does not support infinity.
         *
         * \param other the integer to divide this by.
         * \return the quotient \a this divided by \a other.
         */
        template <CppInteger IntType>
        IntegerBase operator /(IntType other) &&;
        /**
         * Divides this by the given integer and returns the result.
         * This can only be used when the given integer divides into
         * this exactly, and for large integers can be much faster than
         * ordinary division.  This integer is not changed.
         *
         * \pre The given integer divides exactly into this integer,
         * i.e., \a this divided by \a other is an integer.
         * \pre \a other is not zero.
         * \pre Neither this nor \a other is infinite.
         *
         * \param other the integer to divide this by.
         * \return the quotient \a this divided by \a other.
         */
        IntegerBase divExact(const IntegerBase& other) const&;
        /**
         * Divides this by the given integer and returns the result.
         * This can only be used when the given integer divides into
         * this exactly, and for large integers can be much faster than
         * ordinary division.
         *
         * \pre The given integer divides exactly into this integer,
         * i.e., \a this divided by \a other is an integer.
         * \pre \a other is not zero.
         * \pre Neither this nor \a other is infinite.
         *
         * \param other the integer to divide this by.
         * \return the quotient \a this divided by \a other.
         */
        IntegerBase divExact(const IntegerBase& other) &&;
        /**
         * Divides this by the given native C++ integer and returns the result.
         * This can only be used when the given integer divides into
         * this exactly, and for large integers can be much faster than
         * ordinary division.  This integer is not changed.
         *
         * \pre The given integer divides exactly into this integer,
         * i.e., \a this divided by \a other is an integer.
         * \pre \a other is not zero.
         * \pre This integer is not infinite.
         *
         * \python It is assumed that the type \a IntType is \c long.
         *
         * \param other the integer to divide this by.
         * \return the quotient \a this divided by \a other.
         */
        template <CppInteger IntType>
        IntegerBase divExact(IntType other) const&;
        /**
         * Divides this by the given native C++ integer and returns the result.
         * This can only be used when the given integer divides into
         * this exactly, and for large integers can be much faster than
         * ordinary division.
         *
         * \pre The given integer divides exactly into this integer,
         * i.e., \a this divided by \a other is an integer.
         * \pre \a other is not zero.
         * \pre This integer is not infinite.
         *
         * \param other the integer to divide this by.
         * \return the quotient \a this divided by \a other.
         */
        template <CppInteger IntType>
        IntegerBase divExact(IntType other) &&;
        /**
         * Determines the remainder when this integer is divided by the
         * given integer.  If non-zero, the result will have the same sign
         * as this integer.
         * This integer is not changed.
         *
         * Regarding special cases:
         *
         * - any finite \a x modulo infinity will return \a x;
         * - infinity modulo anything non-zero will return zero.
         *
         * For a division/modulo routine that always returns a non-negative
         * remainder, see divisionAlg().
         *
         * \exception DivisionByZero The argument \a other is zero.
         * Note that, unlike the division operators, this exception will be
         # thrown even if this class supports infinity.
         *
         * \param other the integer to divide this by.
         * \return the remainder \a this modulo \a other.
         */
        IntegerBase operator %(const IntegerBase& other) const&;
        /**
         * Determines the remainder when this integer is divided by the
         * given integer.  If non-zero, the result will have the same sign
         * as this integer.
         *
         * Regarding special cases:
         *
         * - any finite \a x modulo infinity will return \a x;
         * - infinity modulo anything non-zero will return zero.
         *
         * For a division/modulo routine that always returns a non-negative
         * remainder, see divisionAlg().
         *
         * \exception DivisionByZero The argument \a other is zero.
         * Note that, unlike the division operators, this exception will be
         # thrown even if this class supports infinity.
         *
         * \param other the integer to divide this by.
         * \return the remainder \a this modulo \a other.
         */
        IntegerBase operator %(const IntegerBase& other) &&;
        /**
         * Determines the remainder when this integer is divided by the given
         * native C++ integer.  If non-zero, the result will have the same sign
         * as this integer.
         * This integer is not changed.
         *
         * Regarding special cases:
         *
         * - infinity modulo anything non-zero will return zero.
         *
         * For a division/modulo routine that always returns a non-negative
         * remainder, see divisionAlg().
         *
         * \python It is assumed that the type \a IntType is \c long.
         *
         * \exception DivisionByZero The argument \a other is zero.
         * Note that, unlike the division operators, this exception will be
         # thrown even if this class supports infinity.
         *
         * \param other the integer to divide this by.
         * \return the remainder \a this modulo \a other.
         */
        template <CppInteger IntType>
        IntegerBase operator %(IntType other) const&;
        /**
         * Determines the remainder when this integer is divided by the given
         * native C++ integer.  If non-zero, the result will have the same sign
         * as this integer.
         *
         * Regarding special cases:
         *
         * - infinity modulo anything non-zero will return zero.
         *
         * For a division/modulo routine that always returns a non-negative
         * remainder, see divisionAlg().
         *
         * \exception DivisionByZero The argument \a other is zero.
         * Note that, unlike the division operators, this exception will be
         # thrown even if this class supports infinity.
         *
         * \param other the integer to divide this by.
         * \return the remainder \a this modulo \a other.
         */
        template <CppInteger IntType>
        IntegerBase operator %(IntType other) &&;

        /**
         * Uses the division algorithm to obtain a quotient and
         * remainder when dividing by the given integer.
         *
         * Suppose this integer is \a n and we pass the divisor \a d.
         * The _division algorithm_ describes the result of
         * dividing \a n by \a d; in particular, it expresses
         * `n = qd + r`, where \a q is the quotient and
         * \a r is the remainder.
         *
         * The division algorithm is precise about which values of \a q
         * and \a r are chosen; in particular it chooses the unique \a r
         * in the range `0 ≤ r < |d|`.
         *
         * Note that this differs from other division routines in this
         * class, in that it always rounds to give a non-negative remainder.
         * Thus (-7).divisionAlg(3) gives quotient -3 and remainder 2,
         * whereas (-7)/3 gives quotient -2 and (-7)\%3 gives remainder -1.
         *
         * In the special case where the given divisor is 0 (not
         * allowed by the usual division algorithm), this routine selects
         * quotient 0 and remainder \a n.
         *
         * \pre Neither this nor the divisor are infinite.
         *
         * \param divisor the divisor \a d.
         * \return the pair (\a q, \a r), where \a q is the quotient and
         * \a r is the remainder, as described above.
         */
        std::pair<IntegerBase, IntegerBase> divisionAlg(
            const IntegerBase& divisor) const;

        /**
         * Determines the negative of this integer.
         * This integer is not changed.
         *
         * Negative infinity will return infinity.
         *
         * \return the negative of this integer.
         */
        IntegerBase operator -() const&;
        /**
         * Determines the negative of this integer.
         *
         * Negative infinity will return infinity.
         *
         * \return the negative of this integer.
         */
        IntegerBase operator -() &&;

        /**
         * Adds the given integer to this.
         * This integer is changed to reflect the result.
         *
         * If either term of the sum is infinite, the result will be
         * infinity.
         *
         * \param other the integer to add to this integer.
         * \return a reference to this integer with its new value.
         */
        IntegerBase& operator +=(const IntegerBase& other);
        /**
         * Adds the given integer to this.
         * This integer is changed to reflect the result.
         *
         * If either term of the sum is infinite, the result will be
         * infinity.
         *
         * \param other the integer to add to this integer.
         * \return a reference to this integer with its new value.
         */
        IntegerBase& operator +=(IntegerBase&& other);
        /**
         * Adds the given integer to this.
         * This integer is changed to reflect the result.
         *
         * If either term of the sum is infinite, the result will be
         * infinity.
         *
         * \python It is assumed that the type \a IntType is \c long.
         *
         * \param other the integer to add to this integer.
         * \return a reference to this integer with its new value.
         */
        template <CppInteger IntType>
        IntegerBase& operator +=(IntType other);
        /**
         * Subtracts the given integer from this.
         * This integer is changed to reflect the result.
         *
         * If either term of the difference is infinite, the result will be
         * infinity.
         *
         * \param other the integer to subtract from this integer.
         * \return a reference to this integer with its new value.
         */
        IntegerBase& operator -=(const IntegerBase& other);
        /**
         * Subtracts the given integer from this.
         * This integer is changed to reflect the result.
         *
         * If either term of the difference is infinite, the result will be
         * infinity.
         *
         * \param other the integer to subtract from this integer.
         * \return a reference to this integer with its new value.
         */
        IntegerBase& operator -=(IntegerBase&& other);
        /**
         * Subtracts the given integer from this.
         * This integer is changed to reflect the result.
         *
         * If either term of the difference is infinite, the result will be
         * infinity.
         *
         * \python It is assumed that the type \a IntType is \c long.
         *
         * \param other the integer to subtract from this integer.
         * \return a reference to this integer with its new value.
         */
        template <CppInteger IntType>
        IntegerBase& operator -=(IntType other);
        /**
         * Multiplies this by the given integer.
         * This integer is changed to reflect the result.
         *
         * If either factor of the product is infinite, the result will be
         * infinity.
         *
         * \param other the integer to multiply with this integer.
         * \return a reference to this integer with its new value.
         */
        IntegerBase& operator *=(const IntegerBase& other);
        /**
         * Multiplies this by the given native C++ integer.
         * This integer is changed to reflect the result.
         *
         * If either factor of the product is infinite, the result will be
         * infinity.
         *
         * \python It is assumed that the type \a IntType is \c long.
         *
         * \param other the integer to multiply with this integer.
         * \return a reference to this integer with its new value.
         */
        template <CppInteger IntType>
        IntegerBase& operator *=(IntType other);
        /**
         * Divides this by the given integer.
         * The result will be truncated to an integer, i.e., rounded
         * towards zero.
         * This integer is changed to reflect the result.
         *
         * If \a other is known to divide this integer exactly,
         * divByExact() should be used instead.
         *
         * Regarding special cases:
         *
         * - infinity divided by anything will return infinity;
         * - anything divided by zero will likewise return infinity;
         * - anything finite divided by infinity will return zero.
         *
         * For a division routine that always rounds down, see divisionAlg().
         *
         * \exception DivisionByZero The argument \a other is zero, but this
         * class does not support infinity.
         *
         * \param other the integer to divide this by.
         * \return a reference to this integer with its new value.
         */
        IntegerBase& operator /=(const IntegerBase& other);
        /**
         * Divides this by the given native C++ integer.
         * The result will be truncated to an integer, i.e., rounded
         * towards zero.
         * This integer is changed to reflect the result.
         *
         * If \a other is known to divide this integer exactly,
         * divByExact() should be used instead.
         *
         * Regarding special cases:
         *
         * - infinity divided by anything will return infinity;
         * - anything divided by zero will likewise return infinity.
         *
         * For a division routine that always rounds down, see divisionAlg().
         *
         * \python It is assumed that the type \a IntType is \c long.
         *
         * \exception DivisionByZero The argument \a other is zero, but this
         * class does not support infinity.
         *
         * \param other the integer to divide this by.
         * \return a reference to this integer with its new value.
         */
        template <CppInteger IntType>
        IntegerBase& operator /=(IntType other);
        /**
         * Divides this by the given integer.
         * This can only be used when the given integer divides into
         * this exactly, and for large integers this is much faster than
         * ordinary division.  This integer is changed to reflect the result.
         *
         * \pre The given integer divides exactly into this integer,
         * i.e., \a this divided by \a other is an integer.
         * \pre \a other is not zero.
         * \pre Neither this nor \a other is infinite.
         *
         * \param other the integer to divide this by.
         * \return a reference to this integer with its new value.
         */
        IntegerBase& divByExact(const IntegerBase& other);
        /**
         * Divides this by the given integer.
         * This can only be used when the given integer divides into
         * this exactly, and for large integers this is much faster than
         * ordinary division.  This integer is changed to reflect the result.
         *
         * \pre The given integer divides exactly into this integer,
         * i.e., \a this divided by \a other is an integer.
         * \pre \a other is not zero.
         * \pre This integer is not infinite.
         *
         * \python It is assumed that the type \a IntType is \c long.
         *
         * \param other the integer to divide this by.
         * \return a reference to this integer with its new value.
         */
        template <CppInteger IntType>
        IntegerBase& divByExact(IntType other);
        /**
         * Reduces this integer modulo the given integer.
         * If non-zero, the result will have the same sign as the original
         * value of this integer.
         * This integer is changed to reflect the result.
         *
         * Regarding special cases:
         *
         * - any finite \a x modulo infinity will return \a x;
         * - infinity modulo anything non-zero will return zero.
         *
         * For a division/modulo routine that always returns a non-negative
         * remainder, see divisionAlg().
         *
         * \exception DivisionByZero The argument \a other is zero.
         * Note that, unlike the division operators, this exception will be
         # thrown even if this class supports infinity.
         *
         * \param other the integer modulo which this integer will be
         * reduced.
         * \return a reference to this integer with its new value.
         */
        IntegerBase& operator %=(const IntegerBase& other);
        /**
         * Reduces this integer modulo the given native C++ integer.
         * If non-zero, the result will have the same sign as the original
         * value of this integer.
         * This integer is changed to reflect the result.
         *
         * Regarding special cases:
         *
         * - infinity modulo anything non-zero will return zero.
         *
         * For a division/modulo routine that always returns a non-negative
         * remainder, see divisionAlg().
         *
         * \python It is assumed that the type \a IntType is \c long.
         *
         * \exception DivisionByZero The argument \a other is zero.
         * Note that, unlike the division operators, this exception will be
         # thrown even if this class supports infinity.
         *
         * \param other the integer modulo which this integer will be reduced.
         * \return a reference to this integer with its new value.
         */
        template <CppInteger IntType>
        IntegerBase& operator %=(IntType other);
        /**
         * Adds the product of the two given integers to this integer.
         * This is a common operation in (for example) inner products
         * and matrix multiplication.
         *
         * Calling `x.addProduct(y, z)` is equivalent to, but sometimes
         * faster than, calling `x += y * z`.
         *
         * \param x the first integer in the product to add to this.
         * \param y the second integer in the product to add to this.
         */
        void addProduct(const IntegerBase& x, const IntegerBase& y);
        /**
         * Negates this integer.
         * This integer is changed to reflect the result.
         *
         * Negating infinity will result in infinity.
         */
        void negate();
        /**
         * Raises this integer to the power of the given exponent.
         * This integer is changed to reflect the result.
         *
         * Note that 0 to the power of 0 will be 1, infinity to the
         * power of 0 will be 1, and infinity to the power of anything
         * else will be infinity.
         *
         * \pre The given exponent is non-negative.
         *
         * \param exp the power to which this integer will be raised.
         */
        void raiseToPower(unsigned long exp);

        /**
         * Determines the absolute value of this integer.
         * This integer is not changed.
         *
         * \return the absolute value of this integer.
         */
        IntegerBase abs() const&;
        /**
         * Determines the absolute value of this integer.
         *
         * \return the absolute value of this integer.
         */
        IntegerBase abs() &&;
        /**
         * Sets this integer to be the greatest common divisor of this
         * and the given integer.
         *
         * The result is guaranteed to be non-negative.  As a
         * special case, gcd(0,0) is considered to be zero.
         *
         * \pre Neither this integer nor \a other is infinite.
         *
         * \param other the integer whose greatest common divisor with
         * this will be found.
         */
        void gcdWith(const IntegerBase& other);
        /**
         * Determines the greatest common divisor of this and the given
         * integer.  This integer is not changed.
         *
         * The result is guaranteed to be non-negative.  As a
         * special case, gcd(0,0) is considered to be zero.
         *
         * \pre Neither this integer nor \a other is infinite.
         *
         * \param other the integer whose greatest common divisor with
         * this will be found.
         * \return the greatest common divisor of this and the given
         * integer.
         */
        IntegerBase gcd(const IntegerBase& other) const&;
        /**
         * Determines the greatest common divisor of this and the given
         * integer.  This integer is not changed.
         *
         * The result is guaranteed to be non-negative.  As a
         * special case, gcd(0,0) is considered to be zero.
         *
         * \pre Neither this integer nor \a other is infinite.
         *
         * \param other the integer whose greatest common divisor with
         * this will be found.
         * \return the greatest common divisor of this and the given
         * integer.
         */
        IntegerBase gcd(IntegerBase&& other) const&;
        /**
         * Determines the greatest common divisor of this and the given
         * integer.
         *
         * The result is guaranteed to be non-negative.  As a
         * special case, gcd(0,0) is considered to be zero.
         *
         * \pre Neither this integer nor \a other is infinite.
         *
         * \param other the integer whose greatest common divisor with
         * this will be found.
         * \return the greatest common divisor of this and the given
         * integer.
         */
        IntegerBase gcd(const IntegerBase& other) &&;
        /**
         * Determines the greatest common divisor of this and the given
         * integer.
         *
         * The result is guaranteed to be non-negative.  As a
         * special case, gcd(0,0) is considered to be zero.
         *
         * \pre Neither this integer nor \a other is infinite.
         *
         * \param other the integer whose greatest common divisor with
         * this will be found.
         * \return the greatest common divisor of this and the given
         * integer.
         */
        IntegerBase gcd(IntegerBase&& other) &&;
        /**
         * Sets this integer to be the lowest common multiple of this
         * and the given integer.
         *
         * Note that the result might possibly be negative.
         *
         * \pre Neither this integer nor \a other is infinite.
         *
         * \param other the integer whose lowest common multiple with
         * this will be found.
         */
        void lcmWith(const IntegerBase& other);
        /**
         * Determines the lowest common multiple of this and the given
         * integer.  This integer is not changed.
         *
         * Note that the result might possibly be negative.
         *
         * \pre Neither this integer nor \a other is infinite.
         *
         * \param other the integer whose lowest common multiple with
         * this will be found.
         * \return the lowest common multiple of this and the given
         * integer.
         */
        IntegerBase lcm(const IntegerBase& other) const&;
        /**
         * Determines the lowest common multiple of this and the given
         * integer.  This integer is not changed.
         *
         * Note that the result might possibly be negative.
         *
         * \pre Neither this integer nor \a other is infinite.
         *
         * \param other the integer whose lowest common multiple with
         * this will be found.
         * \return the lowest common multiple of this and the given
         * integer.
         */
        IntegerBase lcm(IntegerBase&& other) const&;
        /**
         * Determines the lowest common multiple of this and the given
         * integer.
         *
         * Note that the result might possibly be negative.
         *
         * \pre Neither this integer nor \a other is infinite.
         *
         * \param other the integer whose lowest common multiple with
         * this will be found.
         * \return the lowest common multiple of this and the given
         * integer.
         */
        IntegerBase lcm(const IntegerBase& other) &&;
        /**
         * Determines the lowest common multiple of this and the given
         * integer.
         *
         * Note that the result might possibly be negative.
         *
         * \pre Neither this integer nor \a other is infinite.
         *
         * \param other the integer whose lowest common multiple with
         * this will be found.
         * \return the lowest common multiple of this and the given
         * integer.
         */
        IntegerBase lcm(IntegerBase&& other) &&;

        /**
         * Determines the greatest common divisor of this and the given
         * integer and finds the smallest coefficients with which these
         * integers combine to give their gcd.
         *
         * Note that the given integers need not be non-negative.
         * However, the gcd returned is guaranteed to be non-negative.
         *
         * If \a d is the gcd of \a this and \a other, then this routine
         * returns the tuple (\a d, \a u, \a v), where \a u and \a v are
         * coefficients for which:
         *
         * - `u⋅this + v⋅other = d`;
         * - `-abs(this)/d < v⋅sign(other) ≤ 0`; and
         * - `1 ≤ u⋅sign(this) ≤ abs(other)/d`.
         *
         * These equations are not satisfied when either of \a this or
         * \a other are zero, but in this case \a u and \a v will both be
         * 0, 1 or -1, using as many zeros as possible.
         *
         * \pre Neither this integer nor \a other is infinite.
         *
         * \note There are two variants of this routine: one returns the
         * coefficients \a u and \a v as part of a tuple, and one returns
         * them via reference arguments.  For now both versions remain
         * supported, but there is a long-term plan to eventually phase out
         * the reference argument variant (i.e., not this variant).
         *
         * \param other the integer whose greatest common divisor with
         * this will be found.
         * \return a tuple containing: the greatest common divisor of
         * \a this and \a other; the final coefficient of \a this; and
         * the final coefficient of \a other.
         */
        std::tuple<IntegerBase, IntegerBase, IntegerBase> gcdWithCoeffs(
            const IntegerBase& other) const;

        /**
         * Determines the greatest common divisor of this and the given
         * integer and finds the smallest coefficients with which these
         * integers combine to give their gcd.
         *
         * Note that the given integers need not be non-negative.
         * However, the gcd returned is guaranteed to be non-negative.
         *
         * If \a d is the gcd of \a this and \a other, the values placed
         * into \a u and \a v will be coefficients for which:
         *
         * - `u⋅this + v⋅other = d`;
         * - `-abs(this)/d < v⋅sign(other) ≤ 0`; and
         * - `1 ≤ u⋅sign(this) ≤ abs(other)/d`.
         *
         * These equations are not satisfied when either of \a this or
         * \a other are zero, but in this case \a u and \a v will both be
         * 0, 1 or -1, using as many zeros as possible.
         *
         * \pre Neither this integer nor \a other is infinite.
         *
         * \note There are two variants of this routine: one returns the
         * coefficients \a u and \a v as part of a tuple, and one returns
         * them via reference arguments.  For now both versions remain
         * supported, but there is a long-term plan to eventually phase out
         * the reference argument variant (i.e., this variant).
         *
         * \param other the integer whose greatest common divisor with
         * this will be found.
         * \param u a variable into which the final coefficient of \a this
         * will be placed.  Any existing contents of \a u will be overwritten.
         * \param v a variable into which the final coefficient of \a other
         * will be placed.  Any existing contents of \a v will be overwritten.
         * \return the greatest common divisor of \a this and \a other.
         */
        IntegerBase gcdWithCoeffs(const IntegerBase& other, IntegerBase& u,
            IntegerBase& v) const;

        /**
         * Returns the Legendre symbol (\a a/\a p), where
         * \a a is this integer and \a p is an odd prime.
         *
         * The Legendre symbol is equal to 0 if this integer
         * is divisible by \a p, 1 if this integer is congruent
         * to a square mod \a p (but not divisible by \a p),
         * and -1 otherwise.
         *
         * \pre The given integer \a p is an odd positive prime.
         * \pre This integer is not infinite.
         *
         * \param p the given odd prime.
         * \return the Legendre symbol (0, 1 or -1) as described above.
         *
         * \author Ryan Budney
         */
        int legendre(const IntegerBase& p) const;

        /**
         * Generate a pseudo-random integer that is uniformly
         * distributed in the interval [0,*this).
         *
         * The random number generation here does _not_ use Regina's
         * own RandomEngine class, but instead uses a separate random
         * number generator provided by GMP.
         *
         * \pre This integer is strictly positive and not infinite.
         *
         * \warning Even if this integer is small, this routine is still
         * slow - it always goes through the GMP large integer routines
         * so that the random number generation algorithm is consistent.
         * If you need a fast random number generator and this integer
         * is small, consider using the standard rand() function instead.
         *
         * \return a pseudo-random integer.
         */
        IntegerBase randomBoundedByThis() const;

        /**
         * Generate a pseudo-random integer that is uniformly
         * distributed in the interval [0,2^n).
         *
         * The random number generation here does _not_ use Regina's
         * own RandomEngine class, but instead uses a separate random
         * number generator provided by GMP.
         *
         * \param n the maximum number of bits in the pseudo-random
         * integer.
         * \return a pseudo-random integer.
         */
        static IntegerBase randomBinary(unsigned long n);

        /**
         * Generate a pseudo-random integer that is distributed in the
         * interval [0,2^n), with a tendency to have long strings of 0s
         * and 1s in its binary expansion.
         *
         * The random number generation here does _not_ use Regina's
         * own RandomEngine class, but instead uses a separate random
         * number generator provided by GMP.
         *
         * \param n the maximum number of bits in the pseudo-random integer.
         * \return a pseudo-random integer.
         */
        static IntegerBase randomCornerBinary(unsigned long n);

        /**
         * Set this to a copy of the given raw GMP integer.
         *
         * This routine allows IntegerBase to interact directly with
         * libgmp and libgmpxx if necessary.
         *
         * \nopython
         *
         * \param fromData the raw GMP integer to clone.
         */
        void setRaw(mpz_srcptr fromData);

        /**
         * Returns the raw GMP data that describes this integer.
         *
         * This routine allows IntegerBase to interact directly with
         * libgmp and libgmpxx if necessary.
         *
         * \warning This routine will have the side-effect of converting
         * this integer to a (bulkier and slower) GMP representation,
         * regardless of whether it is small enough to fit within a native
         * integer.  Excessive use of this routine could lead to a significant
         * performance loss.  It is best to use this only when isNative() is
         * already known to return \c false.
         *
         * \pre This integer is not infinite.
         *
         * \nopython
         *
         * \return the raw GMP data.
         */
        mpz_srcptr rawData() const;

        /**
         * Returns the raw GMP data that describes this integer.
         *
         * This routine allows IntegerBase to interact directly with
         * libgmp and libgmpxx if necessary.
         *
         * \warning This routine will have the side-effect of converting
         * this integer to a (bulkier and slower) GMP representation,
         * regardless of whether it is small enough to fit within a native
         * integer.  Excessive use of this routine could lead to a significant
         * performance loss.  It is best to use this only when isNative() is
         * already known to return \c false.
         *
         * \pre This integer is not infinite.
         *
         * \nopython
         *
         * \return the raw GMP data.
         */
        mpz_ptr rawData();

        /**
         * Converts this integer to use a GMP large integer representation,
         * regardless of whether this is actually necessary.  The contents
         * of this integer will be preserved.
         *
         * It does not matter which kind of representation this integer
         * is currently using.
         *
         * If this integer is infinite then this routine will do nothing.
         */
        void makeLarge();

        /**
         * Converts this integer to use a native C/C++ long representation,
         * if this is possible.  However, if this integer is outside the range
         * of a C/C++ long, then it will remain as a GMP large integer instead
         * (i.e., nothing will change).  Whatever happens, the contents of this
         * integer will be preserved.
         *
         * It does not matter which kind of representation this integer
         * is currently using.
         *
         * If this integer is infinite then this routine will do nothing.
         */
        void tryReduce();

        /**
         * Writes the tight encoding of this integer to the given output
         * stream.  See the page on \ref tight "tight encodings" for details.
         *
         * There is also a corresponding global regina::tightEncode()
         * function, for better compatibility with native C++ integer types.
         * The global function is more efficient if the integer argument is an
         * rvalue reference (since this const member function induces an extra
         * deep copy).
         *
         * \nopython Use tightEncoding() instead, which returns a string.
         *
         * \param out the output stream to which the encoded string will
         * be written.
         */
        void tightEncode(std::ostream& out) const;

        /**
         * Returns the tight encoding of this integer.
         * See the page on \ref tight "tight encodings" for details.
         *
         * There is also a corresponding global regina::tightEncoding()
         * function, for better compatibility with native C++ integer types.
         * The global function is more efficient if the integer argument is an
         * rvalue reference (since this const member function induces an extra
         * deep copy).
         *
         * \return the resulting encoded string.
         */
        std::string tightEncoding() const;

        /**
         * Reconstructs an integer from its given tight encoding.
         * See the page on \ref tight "tight encodings" for details.
         *
         * The tight encoding will be given as a string.  If this string
         * contains leading whitespace or any trailing characters at all
         * (including trailing whitespace), then it will be treated as an
         * invalid encoding (i.e., this routine will throw an exception).
         *
         * This routine does recognise infinity in the case where \a
         * withInfinity is \c true.
         *
         * This routine is identical to calling the global template routine
         * regina::tightDecoding() with this type as the template argument.
         *
         * \exception InvalidArgument The given string is not a tight encoding
         * of an integer of this type.
         *
         * \param enc the tight encoding for an integer.
         * \return the integer represented by the given tight encoding.
         */
        static IntegerBase tightDecoding(const std::string& enc);

        /**
         * Reconstructs an integer from its given tight encoding.
         * See the page on \ref tight "tight encodings" for details.
         *
         * The tight encoding will be read from the given input stream.  If the
         * input stream contains leading whitespace then it will be treated as
         * an invalid encoding (i.e., this routine will throw an exception).
         * The input stream _may_ contain further data: if this routine is
         * successful then the input stream will be left positioned immediately
         * after the encoding, without skipping any trailing whitespace.
         *
         * This routine does recognise infinity in the case where \a
         * withInfinity is \c true.
         *
         * This routine is identical to calling the global template routine
         * regina::tightDecode() with this type as the template argument.
         *
         * \exception InvalidInput The given input stream does not begin with
         * a tight encoding of an integer of this type.
         *
         * \nopython Use tightDecoding() instead, which takes a string as
         * its argument.
         *
         * \param input an input stream that begins with the tight encoding
         * for an integer.
         * \return the integer represented by the given tight encoding.
         */
        static IntegerBase tightDecode(std::istream& input);

        /**
         * Hashes this arbitrary-precision integer to a \c size_t, allowing
         * it to be used for keys in hash tables.
         *
         * The implementation here is fairly simple (but it is a little more
         * intelligent than just casting the integer down to a \c size_t).
         * The specific implementation (and therefore the hash values
         * obtained) is subject to change in future versions of Regina.
         *
         * \python For Python users, this function uses the standard Python
         * name __hash__().  This allows Regina's arbitrary-precision integers
         * to be used as keys in Python dictionaries and sets.
         *
         * \return the hash of this arbitrary-precision integer.
         */
        size_t hash() const;

        /**
         * A diagnostic routine that ensures that the internal representation
         * of this integer is valid.
         *
         * If Regina is working correctly, this routine should do nothing.
         * If the internal representation of this integer is _not_ valid,
         * this routine will throw an exception.
         *
         * This routine is provided for use within Regina's various test suites.
         * End users should not need to call it at all.
         *
         * \exception ImpossibleScenario The internal state of this integer is
         * invalid.
         */
        void validate() const;

    private:
        /**
         * Initialises this integer to infinity.
         */
        constexpr IntegerBase(detail::InfiniteTag) requires (withInfinity) :
                rep_(REP_INFINITE) {
        }

        /**
         * Initialises this integer to zero, using a GMP large integer
         * representation.
         */
        constexpr IntegerBase(detail::GMPTag) : rep_(REP_GMP) {
            mpz_init(d_.gmp_);
        }

        /**
         * Initialises this integer to the given value, using a GMP large
         * integer representation.
         */
        constexpr IntegerBase(long value, detail::GMPTag) : rep_(REP_GMP) {
            mpz_init_set_si(d_.gmp_, value);
        }

        /**
         * Initialises this integer to the given value, using a GMP large
         * integer representation.
         */
        constexpr IntegerBase(unsigned long value, detail::GMPTag) :
                rep_(REP_GMP) {
            mpz_init_set_ui(d_.gmp_, value);
        }

        /**
         * Converts this integer from a native C/C++ long representation
         * into a GMP large integer representation.
         *
         * The contents of `d_.native_` will be copied into `d_.gmp_`.
         *
         * \pre \a rep_ is *REP_NATIVE*.
         */
        void forceLarge();

        /**
         * Converts this integer from a GMP large integer representation
         * into a native C/C++ long representation.
         *
         * The contents of `d_.gmp_` will be extracted and copied into
         * `d_.native_`.
         *
         * \pre \a rep_ is *REP_GMP*, and the large integer that `d_.gmp_`
         * represents lies between `LONG_MIN` and `LONG_MAX` inclusive.
         */
        void forceReduce();

    friend class IntegerBase<! withInfinity>; // For conversions.

    template <int bytes>
    requires (supportsNativeIntegerSize(bytes))
    friend class NativeInteger; // For conversions.

    template <bool withInfinity_>
    friend std::ostream& operator << (std::ostream& out,
        const IntegerBase<withInfinity_>& large);
};

/**
 * LargeInteger is a type alias for IntegerBase<true>, which offers
 * arbitrary precision integers with support for infinity.
 *
 * \ingroup maths
 */
using LargeInteger = IntegerBase<true>;

/**
 * Integer is a type alias for IntegerBase<false>, which offers
 * arbitrary precision integers without support for infinity.
 *
 * \ingroup maths
 */
using Integer = IntegerBase<false>;

/**
 * Swaps the contents of the given integers.
 *
 * This global routine simply calls IntegerBase<withInfinity>::swap();
 * it is provided so that IntegerBase meets the C++ Swappable requirements.
 *
 * \param a the first integer whose contents should be swapped.
 * \param b the second integer whose contents should be swapped.
 *
 * \ingroup maths
 */
template <bool withInfinity>
void swap(IntegerBase<withInfinity>& a, IntegerBase<withInfinity>& b) noexcept;

/**
 * Writes the given integer to the given output stream.
 *
 * \param out the output stream to which to write.
 * \param i the integer to write.
 * \return a reference to \a out.
 *
 * \ingroup maths
 */
template <bool withInfinity>
std::ostream& operator << (std::ostream& out,
    const IntegerBase<withInfinity>& i);

/**
 * Adds the given native C++ integer to the given large integer.
 * If the large integer is infinite, the result will also be infinity.
 *
 * \python It is assumed that the type \a IntType is \c long.
 *
 * \param lhs the native integer to add.
 * \param rhs the large integer to add.
 * \return the sum \a lhs plus \a rhs.
 *
 * \ingroup maths
 */
template <bool withInfinity, CppInteger IntType>
IntegerBase<withInfinity> operator + (IntType lhs,
    const IntegerBase<withInfinity>& rhs);

/**
 * Adds the given native C++ integer to the given large integer.
 * If the large integer is infinite, the result will also be infinity.
 *
 * \param lhs the native integer to add.
 * \param rhs the large integer to add.
 * \return the sum \a lhs plus \a rhs.
 *
 * \ingroup maths
 */
template <bool withInfinity, CppInteger IntType>
IntegerBase<withInfinity> operator + (IntType lhs,
    IntegerBase<withInfinity>&& rhs);

/**
 * Multiplies the given native C++ integer with the given large integer.
 * If the large integer is infinite, the result will also be infinity.
 *
 * \python It is assumed that the type \a IntType is \c long.
 *
 * \param lhs the native integer to multiply.
 * \param rhs the large integer to multiply.
 * \return the product \a lhs times \a rhs.
 *
 * \ingroup maths
 */
template <bool withInfinity, CppInteger IntType>
IntegerBase<withInfinity> operator * (IntType lhs,
    const IntegerBase<withInfinity>& rhs);

/**
 * Multiplies the given native C++ integer with the given large integer.
 * If the large integer is infinite, the result will also be infinity.
 *
 * \param lhs the native integer to multiply.
 * \param rhs the large integer to multiply.
 * \return the product \a lhs times \a rhs.
 *
 * \ingroup maths
 */
template <bool withInfinity, CppInteger IntType>
IntegerBase<withInfinity> operator * (IntType lhs,
    IntegerBase<withInfinity>&& rhs);

/**
 * Writes the tight encoding of the given arbitrary precision integer to the
 * given output stream.  See the page on \ref tight "tight encodings" for
 * details.
 *
 * This global function does the same thing as the member function
 * IntegerBase::tightEncode().  However, this global function is more efficient
 * if the integer argument is an rvalue reference (since the const member
 * function induces an extra deep copy).
 *
 * \nopython Use tightEncoding() instead.
 *
 * \param out the output stream to which the encoded string will be written.
 * \param value the integer to encode.
 *
 * \ingroup maths
 */
template <bool withInfinity>
void tightEncode(std::ostream& out, IntegerBase<withInfinity> value);

/**
 * Returns the tight encoding of the given arbitrary precision integer.
 * See the page on \ref tight "tight encodings" for details.
 *
 * This global function does the same thing as the member function
 * IntegerBase::tightEncoding().  However, this global function is more
 * efficient if the integer argument is an rvalue reference (since the const
 * member function induces an extra deep copy).
 *
 * \pydocname{tightEncoding_ReginaInteger}
 *
 * \param value the integer to encode.
 * \return the resulting encoded string.
 *
 * \ingroup maths
 */
template <bool withInfinity>
std::string tightEncoding(IntegerBase<withInfinity> value);

#ifndef __DOXYGEN
// Don't confuse doxygen with specialisations.

template <bool withInfinity>
struct RingTraits<IntegerBase<withInfinity>> {
    inline static const IntegerBase<withInfinity> zero;
    inline static const IntegerBase<withInfinity> one { 1 };
    static constexpr bool commutative = true;
    static constexpr bool zeroInitialised = true;
    static constexpr bool zeroDivisors = false;
    static constexpr bool inverses = false;
};

// We need to specify the common type to use when working with both Integer and
// LargeInteger, since conversions exist in both directions.

} // namespace regina
template <bool inf1, bool inf2>
struct std::common_type<regina::IntegerBase<inf1>, regina::IntegerBase<inf2>> {
    using type = regina::IntegerBase<inf1 || inf2>;
};
namespace regina {

#endif // __DOXYGEN

/**
 * A wrapper class for a native, fixed-precision integer type of the
 * given size.
 *
 * This class behaves just like native integer arithmetic, where the
 * underlying integer type is signed and stores the given number of bytes.
 * There is no overflow testing, and it is up to the user to ensure that
 * overflows do not occur.  On the other hand, this class is almost as
 * fast as native integer arithmetic (i.e., there is very little overhead).
 *
 * The reason for using this class, instead of working directly in a native
 * integer type, is that this class offers an interface that is compatible with
 * Integer.  Only some of the Integer member functions are offered here;
 * however, those that are offered behave just like their Integer
 * counterparts (with the single exception that all arithmetic in
 * NativeInteger is subject to overflow).  Developers can therefore
 * switch between integer types easily with minimal changes to
 * their code, or support both Integer and NativeInteger types as
 * template arguments.
 *
 * This class supports copying but does not implement separate move operations,
 * since its internal data is so small that copying is just as efficient.
 * It implements the C++ Swappable requirement via its own member and global
 * swap() functions, for consistency with the Integer and LargeInteger classes.
 *
 * \tparam bytes the exact number of bytes in the native C++ integer type
 * that is being wrapped.
 *
 * \nopython The purpose of NativeInteger is to be a highly optimised
 * drop-in replacement for Integer as a C++ template parameter.
 * Python users should just use regina.Integer if you need Regina's integer
 * interface, or Python's own integer type if you do not.
 *
 * \ingroup maths
 */
template <int bytes>
requires (supportsNativeIntegerSize(bytes))
class NativeInteger {
    public:
        using Native = typename IntOfSize<bytes>::type;
            /**< The native data type used to store this integer. */

    private:
        Native data_;
            /**< The value of this integer. */

    public:
        /**
         * Initialises this integer to zero.
         */
        constexpr NativeInteger();
        /**
         * Initialises this integer to the given value.
         *
         * \param value the new value of this integer.
         */
        constexpr NativeInteger(Native value);
        /**
         * Initialises this integer to the given value.
         *
         * \param value the new value of this integer.
         */
        constexpr NativeInteger(const NativeInteger<bytes>& value);
        /**
         * Deprecated constructor that initialises this to the given arbitrary
         * precision integer, with no range checking.
         *
         * This constructor is marked as explicit in the hope of
         * avoiding accidental (and unintentional) mixing of integer classes.
         *
         * It is the programmer's reponsibility to ensure that the given value
         * fits within the required range.  If the given value is too large or
         * small to fit into this native type, then this new NativeInteger
         * will have an undefined initial value.
         *
         * \deprecated You should explicitly specify how you plan to deal with
         * range checking.  If (as with this constructor) you know that the
         * given integer is within range, you should initialise this as
         * `NativeInteger(value.unsafeValue<Native>)`.  If you want Regina to
         * look after range checking for you (and throw an exception if this
         * fails), you should use `NativeInteger(value.safeValue<Native>)`.
         *
         * \pre The given integer is within the required range; that is, it
         * can fit within a signed integer of the chosen byte length.
         *
         * \param value the new value of this integer.
         */
        template <bool withInfinity>
        [[deprecated]] explicit NativeInteger(
            const IntegerBase<withInfinity>& value);

        /**
         * Returns whether or not this integer is zero.
         *
         * \return \c true if and only if this integer is zero.
         */
        constexpr bool isZero() const;

        /**
         * Returns the sign of this integer.
         *
         * \return +1, -1 or 0 according to whether this integer is
         * positive, negative or zero.
         */
        constexpr int sign() const;
        /**
         * Returns the value of this integer in its native type.
         *
         * \return the value of this integer.
         */
        constexpr Native nativeValue() const;
        /**
         * Returns the string representation of this integer in base 10.
         *
         * \return the string representation of this integer.
         */
        std::string str() const;

        /**
         * Sets this integer to the given value.
         *
         * \param value the new value of this integer.
         * \return a reference to this integer with its new value.
         */
        constexpr NativeInteger& operator =(const NativeInteger& value);
        /**
         * Sets this integer to the given value.
         *
         * \param value the new value of this integer.
         * \return a reference to this integer with its new value.
         */
        constexpr NativeInteger& operator =(Native value);
        /**
         * Swaps the values of this and the given integer.
         *
         * \param other the integer whose value will be swapped with this.
         */
        constexpr void swap(NativeInteger& other) noexcept;

        /**
         * Determines if this is equal to the given integer.
         *
         * \param rhs the integer with which this will be compared.
         * \return \c true if and only if this and the given integer are
         * equal.
         */
        constexpr bool operator ==(const NativeInteger& rhs) const;
        /**
         * Determines if this is equal to the given integer.
         *
         * \param rhs the integer with which this will be compared.
         * \return \c true if and only if this and the given integer are
         * equal.
         */
        constexpr bool operator ==(Native rhs) const;
        /**
         * Compares this to the given integer.
         *
         * This is a numerical comparison; that is, it uses the usual ordering
         * of the integers.
         *
         * This generates all of the usual comparison operators, including
         * `<`, `<=`, `>`, and `>=`.
         *
         * \python This spaceship operator `x <=> y` is not available, but the
         * other comparison operators that it generates _are_ available.
         *
         * \param rhs the integer with which this will be compared.
         * \return the result of the numerical comparison between this and
         * the given integer.
         */
        constexpr std::strong_ordering operator <=> (const NativeInteger& rhs)
            const;
        /**
         * Compares this to the given integer.
         *
         * This is a numerical comparison; that is, it uses the usual ordering
         * of the integers.
         *
         * This generates all of the usual comparison operators, including
         * `<`, `<=`, `>`, and `>=`.
         *
         * \python This spaceship operator `x <=> y` is not available, but the
         * other comparison operators that it generates _are_ available.
         *
         * \param rhs the integer with which this will be compared.
         * \return the result of the numerical comparison between this and
         * the given integer.
         */
        constexpr std::strong_ordering operator <=> (Native rhs) const;

        /**
         * The preincrement operator.
         * This operator increments this integer by one, and returns a
         * reference to the integer _after_ the increment.
         *
         * \return a reference to this integer after the increment.
         */
        constexpr NativeInteger& operator ++();

        /**
         * The postincrement operator.
         * This operator increments this integer by one, and returns a
         * copy of the integer _before_ the increment.
         *
         * \return a copy of this integer before the
         * increment took place.
         */
        constexpr NativeInteger operator ++(int);

        /**
         * The predecrement operator.
         * This operator decrements this integer by one, and returns a
         * reference to the integer _after_ the decrement.
         *
         * \return a reference to this integer after the decrement.
         */
        constexpr NativeInteger& operator --();

        /**
         * The postdecrement operator.
         * This operator decrements this integer by one, and returns a
         * copy of the integer _before_ the decrement.
         *
         * \return a copy of this integer before the
         * decrement took place.
         */
        constexpr NativeInteger operator --(int);

        /**
         * Adds this to the given integer and returns the result.
         * This integer is not changed.
         *
         * \param other the integer to add to this integer.
         * \return the sum \a this plus \a other.
         */
        constexpr NativeInteger operator +(const NativeInteger& other) const;
        /**
         * Adds this to the given integer and returns the result.
         * This integer is not changed.
         *
         * \param other the integer to add to this integer.
         * \return the sum \a this plus \a other.
         */
        constexpr NativeInteger operator +(Native other) const;
        /**
         * Subtracts the given integer from this and returns the result.
         * This integer is not changed.
         *
         * \param other the integer to subtract from this integer.
         * \return the difference \a this minus \a other.
         */
        constexpr NativeInteger operator -(const NativeInteger& other) const;
        /**
         * Subtracts the given integer from this and returns the result.
         * This integer is not changed.
         *
         * \param other the integer to subtract from this integer.
         * \return the difference \a this minus \a other.
         */
        constexpr NativeInteger operator -(Native other) const;
        /**
         * Multiplies this by the given integer and returns the
         * result.
         * This integer is not changed.
         *
         * \param other the integer to multiply by this integer.
         * \return the product \a this times \a other.
         */
        constexpr NativeInteger operator *(const NativeInteger& other) const;
        /**
         * Multiplies this by the given integer and returns the
         * result.
         * This integer is not changed.
         *
         * \param other the integer to multiply by this integer.
         * \return the product \a this times \a other.
         */
        constexpr NativeInteger operator *(Native other) const;
        /**
         * Divides this by the given integer and returns the result.
         * The result will be truncated to an integer, i.e., rounded
         * towards zero.
         * This integer is not changed.
         *
         * For a division routine that always rounds down, see divisionAlg().
         *
         * \exception DivisionByZero The argument \a other is zero.
         *
         * \param other the integer to divide this by.
         * \return the quotient \a this divided by \a other.
         */
        constexpr NativeInteger operator /(const NativeInteger& other) const;
        /**
         * Divides this by the given integer and returns the result.
         * The result will be truncated to an integer, i.e., rounded
         * towards zero.
         * This integer is not changed.
         *
         * For a division routine that always rounds down, see divisionAlg().
         *
         * \exception DivisionByZero The argument \a other is zero.
         *
         * \param other the integer to divide this by.
         * \return the quotient \a this divided by \a other.
         */
        constexpr NativeInteger operator /(Native other) const;
        /**
         * Divides this by the given integer and returns the result.
         * This integer is not changed.
         *
         * This function is provided for consistency with Integer::divExact()
         * and LargeInteger::divExact(), and in the spirit of those functions,
         * it should only be used when the given integer divides into this
         * exactly.  However, the implementation for NativeInteger just uses
         * the native operator `/`, and so (unlike for arbitrary precision
         * integers) there is no real performance gain in using `x.divExact(y)`
         * instead of `x / y`.
         *
         * \pre The given integer divides exactly into this integer,
         * i.e., \a this divided by \a other is an integer.
         * \pre \a other is not zero.
         *
         * \param other the integer to divide this by.
         * \return the quotient \a this divided by \a other.
         */
        constexpr NativeInteger divExact(const NativeInteger& other) const;
        /**
         * Divides this by the given integer and returns the result.
         * This integer is not changed.
         *
         * This function is provided for consistency with Integer::divExact()
         * and LargeInteger::divExact(), and in the spirit of those functions,
         * it should only be used when the given integer divides into this
         * exactly.  However, the implementation for NativeInteger just uses
         * the native operator `/`, and so (unlike for arbitrary precision
         * integers) there is no real performance gain in using `x.divExact(y)`
         * instead of `x / y`.
         *
         * \pre The given integer divides exactly into this integer,
         * i.e., \a this divided by \a other is an integer.
         * \pre \a other is not zero.
         *
         * \param other the integer to divide this by.
         * \return the quotient \a this divided by \a other.
         */
        constexpr NativeInteger divExact(Native other) const;
        /**
         * Determines the remainder when this integer is divided by the
         * given integer.  If non-zero, the result will have the same sign
         * as this integer.
         * This integer is not changed.
         *
         * For a division/modulo routine that always returns a non-negative
         * remainder, see divisionAlg().
         *
         * \exception DivisionByZero The argument \a other is zero.
         *
         * \param other the integer to divide this by.
         * \return the remainder \a this modulo \a other.
         */
        constexpr NativeInteger operator %(const NativeInteger& other) const;
        /**
         * Determines the remainder when this integer is divided by the
         * given integer.  If non-zero, the result will have the same sign
         * as this integer.
         * This integer is not changed.
         *
         * For a division/modulo routine that always returns a non-negative
         * remainder, see divisionAlg().
         *
         * \exception DivisionByZero The argument \a other is zero.
         *
         * \param other the integer to divide this by.
         * \return the remainder \a this modulo \a other.
         */
        constexpr NativeInteger operator %(Native other) const;

        /**
         * Uses the division algorithm to obtain a quotient and
         * remainder when dividing by the given integer.
         *
         * Suppose this integer is \a n and we pass the divisor \a d.
         * The _division algorithm_ describes the result of
         * dividing \a n by \a d; in particular, it expresses
         * `n = qd + r`, where \a q is the quotient and
         * \a r is the remainder.
         *
         * The division algorithm is precise about which values of \a q
         * and \a r are chosen; in particular it chooses the unique \a r
         * in the range `0 ≤ r < |d|`.
         *
         * Note that this differs from other division routines in this
         * class, in that it always rounds to give a non-negative remainder.
         * Thus (-7).divisionAlg(3) gives quotient -3 and remainder 2,
         * whereas (-7)/3 gives quotient -2 and (-7)\%3 gives remainder -1.
         *
         * In the special case where the given divisor is 0 (not
         * allowed by the usual division algorithm), this routine selects
         * quotient 0 and remainder \a n.
         *
         * \param divisor the divisor \a d.
         * \return the pair (\a q, \a r), where \a q is the quotient and
         * \a r is the remainder, as described above.
         */
        constexpr std::pair<NativeInteger, NativeInteger> divisionAlg(
            const NativeInteger& divisor) const;

        /**
         * Determines the negative of this integer.
         * This integer is not changed.
         *
         * \return the negative of this integer.
         */
        constexpr NativeInteger operator -() const;

        /**
         * Adds the given integer to this.
         * This integer is changed to reflect the result.
         *
         * \param other the integer to add to this integer.
         * \return a reference to this integer with its new value.
         */
        constexpr NativeInteger& operator +=(const NativeInteger& other);
        /**
         * Adds the given integer to this.
         * This integer is changed to reflect the result.
         *
         * \param other the integer to add to this integer.
         * \return a reference to this integer with its new value.
         */
        constexpr NativeInteger& operator +=(Native other);
        /**
         * Subtracts the given integer from this.
         * This integer is changed to reflect the result.
         *
         * \param other the integer to subtract from this integer.
         * \return a reference to this integer with its new value.
         */
        constexpr NativeInteger& operator -=(const NativeInteger& other);
        /**
         * Subtracts the given integer from this.
         * This integer is changed to reflect the result.
         *
         * \param other the integer to subtract from this integer.
         * \return a reference to this integer with its new value.
         */
        constexpr NativeInteger& operator -=(Native other);
        /**
         * Multiplies the given integer by this.
         * This integer is changed to reflect the result.
         *
         * \param other the integer to multiply with this integer.
         * \return a reference to this integer with its new value.
         */
        constexpr NativeInteger& operator *=(const NativeInteger& other);
        /**
         * Multiplies the given integer by this.
         * This integer is changed to reflect the result.
         *
         * \param other the integer to multiply with this integer.
         * \return a reference to this integer with its new value.
         */
        constexpr NativeInteger& operator *=(Native other);
        /**
         * Divides this by the given integer.
         * The result will be truncated to an integer, i.e., rounded
         * towards zero.
         * This integer is changed to reflect the result.
         *
         * For a division routine that always rounds down, see divisionAlg().
         *
         * \exception DivisionByZero The argument \a other is zero.
         *
         * \param other the integer to divide this by.
         * \return a reference to this integer with its new value.
         */
        constexpr NativeInteger& operator /=(const NativeInteger& other);
        /**
         * Divides this by the given integer.
         * The result will be truncated to an integer, i.e., rounded
         * towards zero.
         * This integer is changed to reflect the result.
         *
         * For a division routine that always rounds down, see divisionAlg().
         *
         * \exception DivisionByZero The argument \a other is zero.
         *
         * \param other the integer to divide this by.
         * \return a reference to this integer with its new value.
         */
        constexpr NativeInteger& operator /=(Native other);
        /**
         * Divides this by the given integer.
         * This integer is changed to reflect the result.
         *
         * This function is provided for consistency with Integer::divByExact()
         * and LargeInteger::divByExact(), and in the spirit of those functions,
         * it should only be used when the given integer divides into this
         * exactly.  However, the implementation for NativeInteger just uses
         * the native operator `/=`, and so (unlike for arbitrary precision
         * integers) there is no real performance gain in using
         * `x.divByExact(y)` instead of `x /= y`.
         *
         * \pre The given integer divides exactly into this integer,
         * i.e., \a this divided by \a other is an integer.
         * \pre \a other is not zero.
         *
         * \param other the integer to divide this by.
         * \return a reference to this integer with its new value.
         */
        constexpr NativeInteger& divByExact(const NativeInteger& other);
        /**
         * Divides this by the given integer.
         * This integer is changed to reflect the result.
         *
         * This function is provided for consistency with Integer::divByExact()
         * and LargeInteger::divByExact(), and in the spirit of those functions,
         * it should only be used when the given integer divides into this
         * exactly.  However, the implementation for NativeInteger just uses
         * the native operator `/=`, and so (unlike for arbitrary precision
         * integers) there is no real performance gain in using
         * `x.divByExact(y)` instead of `x /= y`.
         *
         * \pre The given integer divides exactly into this integer,
         * i.e., \a this divided by \a other is an integer.
         * \pre \a other is not zero.
         *
         * \param other the integer to divide this by.
         * \return a reference to this integer with its new value.
         */
        constexpr NativeInteger& divByExact(Native other);
        /**
         * Reduces this integer modulo the given integer.
         * If non-zero, the result will have the same sign as the original
         * value of this integer.
         * This integer is changed to reflect the result.
         *
         * For a division/modulo routine that always returns a non-negative
         * remainder, see divisionAlg().
         *
         * \exception DivisionByZero The argument \a other is zero.
         *
         * \param other the integer modulo which this integer will be
         * reduced.
         * \return a reference to this integer with its new value.
         */
        constexpr NativeInteger& operator %=(const NativeInteger& other);
        /**
         * Reduces this integer modulo the given integer.
         * If non-zero, the result will have the same sign as the original
         * value of this integer.
         * This integer is changed to reflect the result.
         *
         * For a division/modulo routine that always returns a non-negative
         * remainder, see divisionAlg().
         *
         * \exception DivisionByZero The argument \a other is zero.
         *
         * \param other the integer modulo which this integer will be
         * reduced.
         * \return a reference to this integer with its new value.
         */
        constexpr NativeInteger& operator %=(Native other);
        /**
         * Negates this integer.
         * This integer is changed to reflect the result.
         */
        constexpr void negate();
        /**
         * Sets this integer to be the greatest common divisor of this
         * and the given integer.
         *
         * The result is guaranteed to be non-negative.  As a
         * special case, gcd(0,0) is considered to be zero.
         *
         * \param other the integer whose greatest common divisor with
         * this will be found.
         */
        constexpr void gcdWith(const NativeInteger& other);
        /**
         * Determines the greatest common divisor of this and the given
         * integer.  This integer is not changed.
         *
         * The result is guaranteed to be non-negative.  As a
         * special case, gcd(0,0) is considered to be zero.
         *
         * \param other the integer whose greatest common divisor with
         * this will be found.
         * \return the greatest common divisor of this and the given
         * integer.
         */
        constexpr NativeInteger gcd(const NativeInteger& other) const;

        /**
         * Returns whether this integer is infinity.
         *
         * Since NativeInteger cannot represent infinity, this routine will
         * always return \c false.  This routine is simply provided for
         * compatibility with LargeInteger (where infinity is allowed).
         *
         * \return \c false, since a NativeInteger can never be infinity.
         */
        constexpr bool isInfinite() const {
            return false;
        }
        /**
         * A do-nothing routine that ensures that this integer is using a
         * native C/C++ integer representation.
         *
         * Since the NativeInteger class always uses a native representation,
         * this routine does nothing at all.  This routine is simply provided
         * for compatibility with Regina's arbitrary-precision Integer and
         * LargeInteger classes.
         */
        constexpr void tryReduce();

#ifndef __DOXYGEN
    // Doxygen is not able to match this up to the documented version below.
    template <int bytes_>
    friend std::ostream& operator << (std::ostream& out,
        const NativeInteger<bytes_>& large);
#endif
};

/**
 * Swaps the contents of the given integers.
 *
 * This global routine simply calls NativeInteger<bytes>::swap(); it is
 * provided so that NativeInteger<bytes> meets the C++ Swappable requirements.
 *
 * \nopython The NativeInteger classes are not available to Python users.
 *
 * \param a the first integer whose contents should be swapped.
 * \param b the second integer whose contents should be swapped.
 *
 * \ingroup maths
 */
template <int bytes>
constexpr void swap(NativeInteger<bytes>& a, NativeInteger<bytes>& b) noexcept;

/**
 * Writes the given integer to the given output stream.
 *
 * The output will always be numerical, even if the underlying native
 * C++ type is equivalent to `signed char` (i.e., `bytes == 1`).
 *
 * \param out the output stream to which to write.
 * \param i the integer to write.
 * \return a reference to \a out.
 *
 * \ingroup maths
 */
template <int bytes>
std::ostream& operator << (std::ostream& out, const NativeInteger<bytes>& i);

/**
 * NativeLong is a type alias for the NativeInteger template class whose
 * underlying integer type is a native long.
 *
 * \nopython The NativeInteger classes are not available to Python users.
 *
 * \ingroup maths
 */
using NativeLong = NativeInteger<sizeof(long)>;

#ifndef __DOXYGEN
// Don't confuse doxygen with specialisations.
template <int bytes>
struct RingTraits<NativeInteger<bytes>> {
    static constexpr NativeInteger<bytes> zero { };
    static constexpr NativeInteger<bytes> one { 1 };
    static constexpr bool commutative = true;
    static constexpr bool zeroInitialised = true;
    static constexpr bool zeroDivisors = true;
    static constexpr bool inverses = false;
};
#endif // __DOXYGEN

// Implementation details

namespace detail {

/**
 * Negates the given signed native C++ integer, and returns the result as an
 * unsigned native C++ integer of the same size.  The result should always be
 * correct (i.e., there should never be an overflow condition).
 *
 * \pre The result will be non-negative; that is, `x ≤ 0`.
 *
 * \param x the signed integer to negate.
 * \return a corresponding unsigned representation of `-x`.
 *
 * \ingroup detail
 */
template <SignedCppInteger IntType>
inline MakeUnsigned<IntType> negateToUnsignedType(IntType x) {
    // C++20 mandates a two's complement representation.
    if (x == std::numeric_limits<IntType>::min()) {
        // Negating x would be a signed overflow, which the C++ standard
        // says is undefined behaviour.  However, casting x directly as the
        // unsigned type will do the right thing.
        return static_cast<MakeUnsigned<IntType>>(x);
    } else {
        return static_cast<MakeUnsigned<IntType>>(-x);
    }
}

/**
 * Returns the difference between two signed native C++ integers, and returns
 * the result as an unsigned native C++ integer of the same size.  The result
 * should always be correct (i.e., there should never be an overflow condition).
 *
 * \pre The result will be non-negative; that is, `x ≥ y` (as signed types).
 *
 * \param x the first signed integer to use in the subtraction.
 * \param y the second signed integer to use in the subtraction.
 * \return a corresponding unsigned representation of `x - y`.
 *
 * \ingroup detail
 */
template <SignedCppInteger IntType>
inline MakeUnsigned<IntType> differenceAsUnsigned(IntType x, IntType y) {
    // C++20 mandates a two's complement representation.
    // The C++ standard says both unsigned overflow and casting to unsigned
    // types always do the right thing (arithmetic modulo 2^bits), whereas
    // signed overflow is undefined.
    // So: we can just do everything in the unsigned type.  All errors will be
    // modulo 2^bits, and we know that the answer is in the range [0, 2^bits).
    return static_cast<MakeUnsigned<IntType>>(x) -
        static_cast<MakeUnsigned<IntType>>(y);
}

} // namespace detail

// Inline functions for IntegerBase

template <bool withInfinity>
inline const IntegerBase<withInfinity> IntegerBase<withInfinity>::zero;

template <bool withInfinity>
inline const IntegerBase<withInfinity> IntegerBase<withInfinity>::one = 1;

// We define infinity later, after the specialised infinity constructor.

template <bool withInfinity>
inline IntegerBase<withInfinity>::IntegerBase() : rep_(REP_NATIVE), d_(0) {
}

template <bool withInfinity>
template <CppInteger IntType>
inline IntegerBase<withInfinity>::IntegerBase(IntType value) :
        rep_(REP_NATIVE), d_(value) {
    // If sizeof(IntType) < sizeof(long), or if IntType == long, then we are
    // already finished.
    if constexpr (sizeof(IntType) == sizeof(long) &&
            UnsignedCppInteger<IntType>) {
        // Detect overflow.
        if (d_.native_ < 0) {
            rep_ = REP_GMP;
            mpz_init_set_ui(d_.gmp_, value);
        }
    } else if constexpr (sizeof(IntType) > sizeof(long)) {
        if constexpr (SignedCppInteger<IntType>) {
            // Detect overflow.
            if (d_.native_ != value) {
                rep_ = REP_GMP;
                mpz_init(d_.gmp_);
                if (value >= 0) {
                    mpz_import(d_.gmp_, 1, 1 /* word order */, sizeof(IntType),
                        0 /* native endianness */, 0 /* full words */, &value);
                } else {
                    // mpz_import assumes an unsigned type.
                    // C++20 mandates a two's complement representation, and
                    // we use that here.
                    if (value != std::numeric_limits<IntType>::min())
                        value = -value;
                    // In all cases - including min() where we did not negate -
                    // if we treat the type as unsigned we get |original value|.
                    mpz_import(d_.gmp_, 1, 1 /* word order */, sizeof(IntType),
                        0 /* native endianness */, 0 /* full words */, &value);
                    mpz_neg(d_.gmp_, d_.gmp_);
                }
            }
        } else {
            // Detect overflow.  Here we need to be careful about comparisons
            // between signed and unsigned.
            if (d_.native_ < 0 || static_cast<IntType>(d_.native_) != value) {
                rep_ = REP_GMP;
                mpz_init(d_.gmp_);
                mpz_import(d_.gmp_, 1, 1 /* word order */, sizeof(IntType),
                    0 /* native endianness */, 0 /* full words */, &value);
            }
        }
    }
}

template <bool withInfinity>
inline IntegerBase<withInfinity>::IntegerBase(const IntegerBase& value) :
        rep_(value.rep_) {
    if constexpr (withInfinity)
        if (rep_ == REP_INFINITE)
            return;

    if (rep_)
        mpz_init_set(d_.gmp_, value.d_.gmp_);
    else
        d_.native_ = value.d_.native_;
}

template <bool withInfinity>
inline IntegerBase<withInfinity>::IntegerBase(
        const IntegerBase<! withInfinity>& value) : rep_(value.rep_) {
    if constexpr (! withInfinity)
        if (rep_ == REP_INFINITE)
            throw InvalidArgument(
                "Cannot initialise Integer as LargeInteger::infinity");

    if (rep_)
        mpz_init_set(d_.gmp_, value.d_.gmp_);
    else
        d_.native_ = value.d_.native_;
}

template <bool withInfinity>
inline IntegerBase<withInfinity>::IntegerBase(IntegerBase&& src) noexcept :
        rep_(src.rep_) {
    if constexpr (supportsInfinity)
        if (rep_ == REP_INFINITE)
            return;

    if (rep_) {
        *d_.gmp_ = *src.d_.gmp_;
        src.rep_ = REP_NATIVE; // to release src's hold on the GMP int
    } else
        d_.native_ = src.d_.native_;
}

template <bool withInfinity>
inline IntegerBase<withInfinity>::IntegerBase(
        IntegerBase<! withInfinity>&& src) : rep_(src.rep_) {
    if constexpr (! withInfinity)
        if (rep_ == REP_INFINITE)
            throw InvalidArgument(
                "Cannot initialise Integer as LargeInteger::infinity");

    if (rep_) {
        *d_.gmp_ = *src.d_.gmp_;
        src.rep_ = REP_NATIVE; // to release src's hold on the GMP int
    } else
        d_.native_ = src.d_.native_;
}

template <bool withInfinity>
template <int bytes>
inline IntegerBase<withInfinity>::IntegerBase(
        const NativeInteger<bytes>& value) : IntegerBase(value.nativeValue()) {
}

template <bool withInfinity>
inline IntegerBase<withInfinity>::IntegerBase(double value) : rep_(REP_GMP) {
    // We start with a large representation, since we want to use GMP's
    // double-to-integer conversion.
    mpz_init_set_d(d_.gmp_, value);

    // Now switch to a small representation if we can.
    tryReduce();
}

template <bool withInfinity>
inline IntegerBase<withInfinity>::IntegerBase(
        const std::string& value, int base) :
        IntegerBase(value.c_str(), base) {
}

template <bool withInfinity>
inline IntegerBase<withInfinity>::~IntegerBase() {
    if constexpr (supportsInfinity) {
        if (rep_ == REP_GMP)
            mpz_clear(d_.gmp_);
    } else {
        if (rep_)
            mpz_clear(d_.gmp_);
    }
}

template <bool withInfinity>
inline bool IntegerBase<withInfinity>::isNative() const {
    if constexpr (withInfinity)
        return rep_ == REP_NATIVE;
    else
        return ! rep_;
}

template <bool withInfinity>
inline bool IntegerBase<withInfinity>::isZero() const {
    if constexpr (withInfinity)
        if (rep_ == REP_INFINITE)
            return false;

    return (rep_ ? mpz_sgn(d_.gmp_) == 0 : ! d_.native_);
}

template <bool withInfinity>
inline int IntegerBase<withInfinity>::sign() const {
    if constexpr (withInfinity)
        if (rep_ == REP_INFINITE)
            return 1;

    return (rep_ ? mpz_sgn(d_.gmp_) :
        d_.native_ > 0 ? 1 : d_.native_ < 0 ? -1 : 0);
}

template <bool withInfinity>
inline std::string IntegerBase<withInfinity>::str() const {
    return stringValue();
}

template <bool withInfinity>
template <CppInteger IntType>
IntType IntegerBase<withInfinity>::safeValue() const {
    if constexpr (withInfinity)
        if (rep_ == REP_INFINITE)
            throw IntegerOverflow();

    using limits = std::numeric_limits<IntType>;

    if (rep_) {
        // We have a GMP integer.
        if constexpr (sizeof(IntType) <= sizeof(long)) {
            // Optimise for small native types.
            if constexpr (UnsignedCppInteger<IntType>) {
                if (mpz_sgn(d_.gmp_) >= 0 &&
                        mpz_cmp_ui(d_.gmp_, limits::max()) <= 0)
                    return static_cast<IntType>(mpz_get_ui(d_.gmp_));
                else
                    throw IntegerOverflow();
            } else {
                if (mpz_cmp_si(d_.gmp_, limits::max()) <= 0 &&
                        mpz_cmp_si(d_.gmp_, limits::min()) >= 0)
                    return static_cast<IntType>(mpz_get_si(d_.gmp_));
                else
                    throw IntegerOverflow();
            }
        } else {
            int sign = mpz_sgn(d_.gmp_);
            if (sign == 0)
                return 0;

            if constexpr (UnsignedCppInteger<IntType>) {
                if (sign < 0)
                    throw IntegerOverflow();

                // We have a strictly positive GMP integer.
                size_t count;
                auto* result = mpz_export(nullptr, &count, 1 /* word order */,
                    sizeof(IntType), 0 /* native endianness */,
                    0 /* full words */, d_.gmp_);
                // We should have count > 0.
                if (count == 1) {
                    IntType ans = *static_cast<IntType*>(result);
                    free(result);
                    return ans;
                } else {
                    free(result);
                    throw IntegerOverflow();
                }
            } else {
                // Fetch the absolute value of our GMP integer, which we know to
                // be non-zero.
                size_t count;
                auto* result = mpz_export(nullptr, &count, 1 /* word order */,
                    sizeof(IntType), 0 /* native endianness */,
                    0 /* full words */, d_.gmp_);
                // We should have count > 0.
                if (count == 1) {
                    IntType absVal = *static_cast<IntType*>(result);
                    free(result);

                    // Note that IntType is signed, and so absVal will in fact
                    // appear negative if and only if its highest bit is set.
                    if (absVal >= 0) {
                        // The highest bit is not set.
                        // There will be no overflow.
                        return (sign > 0 ? absVal : -absVal);
                    } else {
                        // The highest bit is set.
                        // There will be overflow - however, this overflow will
                        // actually wrap around to give the correct result if
                        // (and only if) our integer is the minimum possible
                        // IntVal (-100..0 in binary).
                        if (sign < 0 && absVal == limits::min())
                            return absVal;
                        else
                            throw IntegerOverflow();
                    }
                } else {
                    free(result);
                    throw IntegerOverflow();
                }
            }
        }
    } else {
        // We have a native long integer.
        if constexpr (UnsignedCppInteger<IntType>) {
            if (d_.native_ < 0)
                throw IntegerOverflow();

            // We have a _non-negative_ native long integer.
            if constexpr (sizeof(long) <= sizeof(IntType)) {
                // Any non-negative long can fit inside IntType.
                return static_cast<IntType>(d_.native_);
            } else {
                // We need to test for overflow.
                // The following test is fine, since in this scenario the
                // maximum IntType can be happily represented as a signed long.
                if (d_.native_ > limits::max())
                    throw IntegerOverflow();
                return static_cast<IntType>(d_.native_);
            }
        } else {
            if constexpr (sizeof(long) <= sizeof(IntType)) {
                // Our native long can fit inside IntType.
                return static_cast<IntType>(d_.native_);
            } else {
                // We need to test for overflow.
                // The following test is fine, since in this scenario the
                // upper and lower bounds on IntType can both be happily
                // represented as a signed long.
                if (d_.native_ < limits::min() || d_.native_ > limits::max())
                    throw IntegerOverflow();
                return static_cast<IntType>(d_.native_);
            }
        }
    }
}

template <bool withInfinity>
template <CppInteger IntType>
IntType IntegerBase<withInfinity>::unsafeValue() const {
    // Here we follow the logic for unsafeValue(), but without the bounds
    // checking.
    if constexpr (withInfinity)
        if (rep_ == REP_INFINITE) {
            // Avoid accessing a non-existent GMP integer in the code below.
            return 0;
        }

    if (rep_) {
        // We have a GMP integer.
        if constexpr (sizeof(IntType) <= sizeof(long)) {
            // Optimise for small native types.
            if constexpr (UnsignedCppInteger<IntType>)
                return static_cast<IntType>(mpz_get_ui(d_.gmp_));
            else
                return static_cast<IntType>(mpz_get_si(d_.gmp_));
        } else {
            int sign = mpz_sgn(d_.gmp_);
            if (sign == 0)
                return 0;

            // Fetch the absolute value of our GMP integer.
            auto* result = mpz_export(nullptr, nullptr, 1 /* word order */,
                sizeof(IntType), 0 /* native endianness */, 0 /* full words */,
                d_.gmp_);
            // We should have result != null, since our GMP integer is non-zero.
            IntType absVal = *static_cast<IntType*>(result);
            free(result);

            if constexpr (UnsignedCppInteger<IntType>) {
                return absVal;
            } else {
                if (absVal >= 0) {
                    return (sign > 0 ? absVal : -absVal);
                } else {
                    // C++20 guarantees a two's complement representation for
                    // signed integers.  Therefore the only legitimate situation
                    // in which absVal appears negative is if our integer is the
                    // minimum possible IntType, i.e., -2^(bits-1).
                    // In this case, absVal is already the correct answer.
                    return absVal;
                }
            }
        }
    } else {
        // We have a native long integer.
        return static_cast<IntType>(d_.native_);
    }
}

template <bool withInfinity>
inline long IntegerBase<withInfinity>::safeLongValue() const {
    return safeValue<long>();
}

template <bool withInfinity>
inline long IntegerBase<withInfinity>::longValue() const {
    return unsafeValue<long>();
}

template <bool withInfinity>
template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline typename IntOfSize<bytes>::type IntegerBase<withInfinity>::nativeValue()
        const {
    return unsafeValue<typename IntOfSize<bytes>::type>();
}

template <bool withInfinity>
inline IntegerBase<withInfinity>& IntegerBase<withInfinity>::operator =(
        const IntegerBase& value) {
    if constexpr (withInfinity) {
        if (value.rep_ == REP_INFINITE) {
            makeInfinite();
            return *this;
        } else if (rep_ == REP_INFINITE)
            rep_ = REP_NATIVE;
    }

    // From here on, both this and value are finite.
    //
    // We assume that mpz_set() is fine with self-assignment, since:
    // - the GMP docs state that output and input variables can be the same;
    // - the libgmpxx classes do not special-case self-assignment.
    // The C++ test suite tests self-assignment of Integers also.
    if (value.rep_) {
        if (rep_)
            mpz_set(d_.gmp_, value.d_.gmp_);
        else {
            rep_ = REP_GMP;
            mpz_init_set(d_.gmp_, value.d_.gmp_);
        }
    } else {
        if (rep_) {
            mpz_clear(d_.gmp_);
            rep_ = REP_NATIVE;
        }
        d_.native_ = value.d_.native_; // overwrites d_.gmp_
    }
    return *this;
}

template <bool withInfinity>
inline IntegerBase<withInfinity>& IntegerBase<withInfinity>::operator =(
        const IntegerBase<! withInfinity>& value) {
    if constexpr (withInfinity) {
        // The given value cannot be infinity.
        if (rep_ == REP_INFINITE)
            rep_ = REP_NATIVE;
    } else if (value.rep_ == REP_INFINITE)
        throw InvalidArgument(
            "Cannot set Integer to LargeInteger::infinity");

    // From here on, both this and value are finite.
    // We follow the same logic as for the copy constructor above.
    if (value.rep_) {
        if (rep_)
            mpz_set(d_.gmp_, value.d_.gmp_);
        else {
            rep_ = REP_GMP;
            mpz_init_set(d_.gmp_, value.d_.gmp_);
        }
    } else {
        if (rep_) {
            mpz_clear(d_.gmp_);
            rep_ = REP_NATIVE;
        }
        d_.native_ = value.d_.native_; // overwrites d_.gmp_
    }
    return *this;
}

template <bool withInfinity>
inline IntegerBase<withInfinity>& IntegerBase<withInfinity>::operator =(
        IntegerBase&& src) noexcept {
    // We leave *src* to dispose of our GMP integer, if there is one.
    std::swap(rep_, src.rep_);
    std::swap(d_, src.d_);
    return *this;
}

template <bool withInfinity>
inline IntegerBase<withInfinity>& IntegerBase<withInfinity>::operator =(
        IntegerBase<! withInfinity>&& src) {
    if constexpr (withInfinity) {
        if (rep_ == REP_INFINITE) {
            // We cannot swap an infinite value into src.
            // Give ourselves a finite (but undefined) value.
            rep_ = REP_NATIVE;
        }
    } else {
        if (src.rep_ == REP_INFINITE)
            throw InvalidArgument(
                "Cannot set Integer to LargeInteger::infinity");
    }

    // From here on, both this and *src* are finite.
    // We leave *src* to dispose of our GMP integer, if there is one.
    std::swap(rep_, src.rep_);
    std::swap(d_, src.d_);
    return *this;
}

template <bool withInfinity>
template <CppInteger IntType>
inline IntegerBase<withInfinity>& IntegerBase<withInfinity>::operator =(
        IntType value) {
    if constexpr (withInfinity)
        if (rep_ == REP_INFINITE)
            rep_ = REP_NATIVE; // makes this finite and valid (but undefined)

    // From here on, we know that we have either a native or GMP representation.
    if constexpr (sizeof(IntType) == sizeof(long) &&
            UnsignedCppInteger<IntType>) {
        // We are trying to faithfully convert an unsigned long to a long.
        if (value > static_cast<IntType>(LONG_MAX)) {
            // We cannot do it - switch to a GMP representation.
            if (rep_)
                mpz_set_ui(d_.gmp_, value);
            else {
                rep_ = REP_GMP;
                mpz_init_set_ui(d_.gmp_, value);
            }
        } else {
            // The value can fit.
            if (rep_) {
                mpz_clear(d_.gmp_);
                rep_ = REP_NATIVE;
            }
            d_.native_ = static_cast<long>(value); // overwrites d_.gmp_
        }
    } else if constexpr (sizeof(IntType) > sizeof(long)) {
        // Here we can overflow in either direction, and if we do then
        // importing to a GMP representation is a bit more work.
        if constexpr (SignedCppInteger<IntType>) {
            if (value < LONG_MIN || value > LONG_MAX) {
                if (! rep_) {
                    rep_ = REP_GMP;
                    mpz_init(d_.gmp_);
                }
                if (value >= 0) {
                    mpz_import(d_.gmp_, 1, 1 /* word order */, sizeof(IntType),
                        0 /* native endianness */, 0 /* full words */, &value);
                } else {
                    // mpz_import assumes an unsigned type.
                    // C++20 mandates a two's complement representation, and
                    // we use that here.
                    if (value != std::numeric_limits<IntType>::min())
                        value = -value;
                    // In all cases - including min() where we did not negate -
                    // if we treat the type as unsigned we get |original value|.
                    mpz_import(d_.gmp_, 1, 1 /* word order */, sizeof(IntType),
                        0 /* native endianness */, 0 /* full words */, &value);
                    mpz_neg(d_.gmp_, d_.gmp_);
                }
            } else {
                // The value can fit.
                if (rep_) {
                    mpz_clear(d_.gmp_);
                    rep_ = REP_NATIVE;
                }
                d_.native_ = static_cast<long>(value); // overwrites d_.gmp_
            }
        } else {
            // Be careful about comparisons between signed and unsigned.
            if (value > static_cast<IntType>(LONG_MAX)) {
                if (! rep_) {
                    rep_ = REP_GMP;
                    mpz_init(d_.gmp_);
                }
                mpz_import(d_.gmp_, 1, 1 /* word order */, sizeof(IntType),
                    0 /* native endianness */, 0 /* full words */, &value);
            } else {
                // The value can fit.
                if (rep_) {
                    mpz_clear(d_.gmp_);
                    rep_ = REP_NATIVE;
                }
                d_.native_ = static_cast<long>(value); // overwrites d_.gmp_
            }
        }
    } else {
        // IntType is small enough that overflow is impossible.
        if (rep_) {
            mpz_clear(d_.gmp_);
            rep_ = REP_NATIVE;
        }
        d_.native_ = value; // overwrites d_.gmp_
    }

    return *this;
}

template <bool withInfinity>
template <int bytes>
inline IntegerBase<withInfinity>& IntegerBase<withInfinity>::operator =(
        const NativeInteger<bytes>& value) {
    return (*this) = value.nativeValue();
}

template <bool withInfinity>
inline IntegerBase<withInfinity>& IntegerBase<withInfinity>::operator =(
        const std::string& value) {
    // NOLINTNEXTLINE(misc-unconventional-assign-operator)
    return (*this) = value.c_str();
}

template <bool withInfinity>
inline void IntegerBase<withInfinity>::swap(IntegerBase& other) noexcept {
    std::swap(rep_, other.rep_);
    std::swap(d_, other.d_);
}

template <bool withInfinity>
template <bool rhsWithInfinity>
inline bool IntegerBase<withInfinity>::operator ==(
        const IntegerBase<rhsWithInfinity>& rhs) const {
    if constexpr (withInfinity && rhsWithInfinity)
        if (rep_ == REP_INFINITE && rhs.rep_ == REP_INFINITE)
            return true;
    if constexpr (withInfinity || rhsWithInfinity)
        if (rep_ == REP_INFINITE || rhs.rep_ == REP_INFINITE)
            return false;

    // From here on, all integers are finite.
    if (rep_) {
        if (rhs.rep_)
            return (mpz_cmp(d_.gmp_, rhs.d_.gmp_) == 0);
        else
            return (mpz_cmp_si(d_.gmp_, rhs.d_.native_) == 0);
    } else {
        if (rhs.rep_)
            return (mpz_cmp_si(rhs.d_.gmp_, d_.native_) == 0);
        else
            return (d_.native_ == rhs.d_.native_);
    }
}

template <bool withInfinity>
template <CppInteger IntType>
inline bool IntegerBase<withInfinity>::operator ==(IntType rhs) const {
    if constexpr (withInfinity)
        if (rep_ == REP_INFINITE)
            return false;

    // From here on, all integers are finite.
    if (rep_) {
        if constexpr (sizeof(IntType) <= sizeof(long)) {
            if constexpr (SignedCppInteger<IntType>) {
                return (mpz_cmp_si(d_.gmp_, rhs) == 0);
            } else {
                return (mpz_cmp_ui(d_.gmp_, rhs) == 0);
            }
        } else {
            // We will need to use GMP-to-GMP comparison.
            return *this == IntegerBase(rhs);
        }
    } else {
        if constexpr (SignedCppInteger<IntType>) {
            return (d_.native_ == rhs);
        } else {
            // Be careful: d_.native_ is signed, but rhs is unsigned.
            // Testing d_.native_ == rhs might convert d_.native_ to unsigned
            // before the comparison.
            return (d_.native_ >= 0 && d_.native_ == rhs);
        }
    }
}

template <bool withInfinity>
template <int bytes>
inline bool IntegerBase<withInfinity>::operator ==(
        const NativeInteger<bytes>& rhs) const {
    return (*this) == rhs.nativeValue();
}

template <bool withInfinity>
template <bool rhsWithInfinity>
inline std::strong_ordering IntegerBase<withInfinity>::operator <=> (
        const IntegerBase<rhsWithInfinity>& rhs) const {
    if constexpr (withInfinity) {
        if (rep_ == REP_INFINITE) {
            if constexpr (rhsWithInfinity)
                if (rhs.rep_ == REP_INFINITE)
                    return std::strong_ordering::equal;
            return std::strong_ordering::greater;
        }
    }
    if constexpr (rhsWithInfinity) {
        if (rhs.rep_ == REP_INFINITE)
            return std::strong_ordering::less;
    }

    // From here on, all integers are finite.
    if (rep_) {
        if (rhs.rep_)
            return (mpz_cmp(d_.gmp_, rhs.d_.gmp_) <=> 0);
        else
            return (mpz_cmp_si(d_.gmp_, rhs.d_.native_) <=> 0);
    } else {
        if (rhs.rep_)
            return (0 <=> mpz_cmp_si(rhs.d_.gmp_, d_.native_)); // back-to-front
        else
            return (d_.native_ <=> rhs.d_.native_);
    }
}

template <bool withInfinity>
template <CppInteger IntType>
inline std::strong_ordering IntegerBase<withInfinity>::operator <=> (
        IntType rhs) const {
    if constexpr (withInfinity)
        if (rep_ == REP_INFINITE)
            return std::strong_ordering::greater;

    // From here on, all integers are finite.
    if (rep_) {
        if constexpr (sizeof(IntType) <= sizeof(long)) {
            if constexpr (SignedCppInteger<IntType>) {
                return (mpz_cmp_si(d_.gmp_, rhs) <=> 0);
            } else {
                return (mpz_cmp_ui(d_.gmp_, rhs) <=> 0);
            }
        } else {
            // We will need to use GMP-to-GMP comparison.
            return *this <=> IntegerBase(rhs);
        }
    } else {
        if constexpr (SignedCppInteger<IntType>) {
            // Both d_.native_ and rhs are signed.
            return (d_.native_ <=> rhs);
        } else {
            // Be careful: d_.native_ is signed, but rhs is unsigned.
            if (d_.native_ < 0)
                return std::strong_ordering::less;
            else {
                // Cast d_.native_ to an unsigned type that can hold its value.
                return (static_cast<unsigned long>(d_.native_) <=> rhs);
            }
        }
    }
}

template <bool withInfinity>
template <int bytes>
inline std::strong_ordering IntegerBase<withInfinity>::operator <=> (
        const NativeInteger<bytes>& rhs) const {
    return (*this) <=> rhs.nativeValue();
}

template <bool withInfinity>
inline IntegerBase<withInfinity>& IntegerBase<withInfinity>::operator ++() {
    if constexpr (withInfinity)
        if (rep_ == REP_INFINITE)
            return *this;

    if (rep_)
        mpz_add_ui(d_.gmp_, d_.gmp_, 1);
    else if (d_.native_ != LONG_MAX)
        ++d_.native_;
    else {
        // This is the point at which we overflow.
        forceLarge();
        mpz_add_ui(d_.gmp_, d_.gmp_, 1);
    }
    return *this;
}

template <bool withInfinity>
inline IntegerBase<withInfinity> IntegerBase<withInfinity>::operator ++(int) {
    if constexpr (withInfinity)
        if (rep_ == REP_INFINITE)
            return { detail::InfiniteTag() };

    // Hrmph, just do the standard thing for now.
    // It's not clear how much microoptimisation will help..?
    IntegerBase ans(*this);
    ++(*this);
    return ans;
}

template <bool withInfinity>
inline IntegerBase<withInfinity>& IntegerBase<withInfinity>::operator --() {
    if constexpr (withInfinity)
        if (rep_ == REP_INFINITE)
            return *this;

    if (rep_)
        mpz_sub_ui(d_.gmp_, d_.gmp_, 1);
    else if (d_.native_ != LONG_MIN)
        --d_.native_;
    else {
        // This is the point at which we overflow.
        forceLarge();
        mpz_sub_ui(d_.gmp_, d_.gmp_, 1);
    }
    return *this;
}

template <bool withInfinity>
inline IntegerBase<withInfinity> IntegerBase<withInfinity>::operator --(int) {
    if constexpr (withInfinity)
        if (rep_ == REP_INFINITE)
            return { detail::InfiniteTag() };

    // Hrmph, just do the standard thing for now.
    // It's not clear how much microoptimisation will help..?
    IntegerBase ans(*this);
    --(*this);
    return ans;
}

template <bool withInfinity>
inline IntegerBase<withInfinity> IntegerBase<withInfinity>::operator +(
        const IntegerBase& other) const& {
    // Do the standard thing for now.
    IntegerBase ans(*this);
    ans += other;
    return ans;
}

template <bool withInfinity>
inline IntegerBase<withInfinity> IntegerBase<withInfinity>::operator +(
        IntegerBase&& other) const& {
    // Do the standard thing for now.
    return std::move(other += *this);
}

template <bool withInfinity>
inline IntegerBase<withInfinity> IntegerBase<withInfinity>::operator +(
        const IntegerBase& other) && {
    // Do the standard thing for now.
    return std::move((*this) += other);
}

template <bool withInfinity>
inline IntegerBase<withInfinity> IntegerBase<withInfinity>::operator +(
        IntegerBase&& other) && {
    // Do the standard thing for now.
    return std::move((*this) += std::move(other));
}

template <bool withInfinity>
template <CppInteger IntType>
inline IntegerBase<withInfinity> IntegerBase<withInfinity>::operator +(
        IntType other) const& {
    // Do the standard thing for now.
    IntegerBase ans(*this);
    ans += other;
    return ans;
}

template <bool withInfinity>
template <CppInteger IntType>
inline IntegerBase<withInfinity> IntegerBase<withInfinity>::operator +(
        IntType other) && {
    // Do the standard thing for now.
    return std::move((*this) += other);
}

template <bool withInfinity>
inline IntegerBase<withInfinity> IntegerBase<withInfinity>::operator -(
        const IntegerBase& other) const& {
    // Do the standard thing for now.
    IntegerBase ans(*this);
    ans -= other;
    return ans;
}

template <bool withInfinity>
inline IntegerBase<withInfinity> IntegerBase<withInfinity>::operator -(
        IntegerBase&& other) const& {
    // Do the standard thing for now.
    other.negate();
    return std::move(other += *this);
}

template <bool withInfinity>
inline IntegerBase<withInfinity> IntegerBase<withInfinity>::operator -(
        const IntegerBase& other) && {
    // Do the standard thing for now.
    return std::move((*this) -= other);
}

template <bool withInfinity>
inline IntegerBase<withInfinity> IntegerBase<withInfinity>::operator -(
        IntegerBase&& other) && {
    // Do the standard thing for now.
    return std::move((*this) -= std::move(other));
}

template <bool withInfinity>
template <CppInteger IntType>
inline IntegerBase<withInfinity> IntegerBase<withInfinity>::operator -(
        IntType other) const& {
    // Do the standard thing for now.
    IntegerBase ans(*this);
    ans -= other;
    return ans;
}

template <bool withInfinity>
template <CppInteger IntType>
inline IntegerBase<withInfinity> IntegerBase<withInfinity>::operator -(
        IntType other) && {
    // Do the standard thing for now.
    return std::move((*this) -= other);
}

// The const-const variant of multiplication is implemented separately in
// integer.cpp, since GMP prefers out-of-place multiplication.

template <bool withInfinity>
inline IntegerBase<withInfinity> IntegerBase<withInfinity>::operator *(
        IntegerBase&& other) const& {
    // Do the standard thing for now.
    return std::move(other *= (*this));
}

template <bool withInfinity>
inline IntegerBase<withInfinity> IntegerBase<withInfinity>::operator *(
        const IntegerBase& other) && {
    // Do the standard thing for now.
    return std::move((*this) *= other);
}

template <bool withInfinity>
inline IntegerBase<withInfinity> IntegerBase<withInfinity>::operator *(
        IntegerBase&& other) && {
    // Do the standard thing for now.
    return std::move((*this) *= std::move(other));
}

template <bool withInfinity>
template <CppInteger IntType>
inline IntegerBase<withInfinity> IntegerBase<withInfinity>::operator *(
        IntType other) const& {
    // Do the standard thing for now.
    IntegerBase ans(*this);
    ans *= other;
    return ans;
}

template <bool withInfinity>
template <CppInteger IntType>
inline IntegerBase<withInfinity> IntegerBase<withInfinity>::operator *(
        IntType other) && {
    // Do the standard thing for now.
    return std::move((*this) *= other);
}

template <bool withInfinity>
inline IntegerBase<withInfinity> IntegerBase<withInfinity>::operator /(
        const IntegerBase& other) const& {
    // Do the standard thing for now.
    IntegerBase ans(*this);
    ans /= other;
    return ans;
}

template <bool withInfinity>
inline IntegerBase<withInfinity> IntegerBase<withInfinity>::operator /(
        const IntegerBase& other) && {
    // Do the standard thing for now.
    return std::move((*this) /= other);
}

template <bool withInfinity>
template <CppInteger IntType>
inline IntegerBase<withInfinity> IntegerBase<withInfinity>::operator /(
        IntType other) const& {
    // Do the standard thing for now.
    IntegerBase ans(*this);
    ans /= other;
    return ans;
}

template <bool withInfinity>
template <CppInteger IntType>
inline IntegerBase<withInfinity> IntegerBase<withInfinity>::operator /(
        IntType other) && {
    // Do the standard thing for now.
    return std::move((*this) /= other);
}

template <bool withInfinity>
inline IntegerBase<withInfinity> IntegerBase<withInfinity>::divExact(
        const IntegerBase& other) const& {
    // Do the standard thing for now.
    IntegerBase ans(*this);
    ans.divByExact(other);
    return ans;
}

template <bool withInfinity>
inline IntegerBase<withInfinity> IntegerBase<withInfinity>::divExact(
        const IntegerBase& other) && {
    // Do the standard thing for now.
    return std::move(divByExact(other));
}

template <bool withInfinity>
template <CppInteger IntType>
inline IntegerBase<withInfinity> IntegerBase<withInfinity>::divExact(
        IntType other) const& {
    // Do the standard thing for now.
    IntegerBase ans(*this);
    ans.divByExact(other);
    return ans;
}

template <bool withInfinity>
template <CppInteger IntType>
inline IntegerBase<withInfinity> IntegerBase<withInfinity>::divExact(
        IntType other) && {
    // Do the standard thing for now.
    return std::move(divByExact(other));
}

template <bool withInfinity>
inline IntegerBase<withInfinity> IntegerBase<withInfinity>::operator %(
        const IntegerBase& other) const& {
    // Do the standard thing for now.
    IntegerBase ans(*this);
    ans %= other;
    return ans;
}

template <bool withInfinity>
inline IntegerBase<withInfinity> IntegerBase<withInfinity>::operator %(
        const IntegerBase& other) && {
    // Do the standard thing for now.
    return std::move((*this) %= other);
}

template <bool withInfinity>
template <CppInteger IntType>
inline IntegerBase<withInfinity> IntegerBase<withInfinity>::operator %(
        IntType other) const& {
    // Do the standard thing for now.
    IntegerBase ans(*this);
    ans %= other;
    return ans;
}

template <bool withInfinity>
template <CppInteger IntType>
inline IntegerBase<withInfinity> IntegerBase<withInfinity>::operator %(
        IntType other) && {
    // Do the standard thing for now.
    return std::move((*this) %= other);
}

template <bool withInfinity>
inline IntegerBase<withInfinity> IntegerBase<withInfinity>::operator -()
        const& {
    if constexpr (withInfinity)
        if (rep_ == REP_INFINITE)
            return { detail::InfiniteTag() };

    // From here on, all integers are finite.
    if (rep_) {
        IntegerBase ans(detail::GMPTag{});
        mpz_neg(ans.d_.gmp_, d_.gmp_);
        return ans;
    } else if (d_.native_ == LONG_MIN) {
        // Overflow, just.
        return { detail::absLongMin, detail::GMPTag() };
    } else
        return -d_.native_;
}

template <bool withInfinity>
inline IntegerBase<withInfinity> IntegerBase<withInfinity>::operator -() && {
    if constexpr (withInfinity)
        if (rep_ == REP_INFINITE)
            return { detail::InfiniteTag() };

    // From here on, all integers are finite.
    if (rep_) {
        // This operation is very cheap, which is the main reason we want a
        // move variant of negation.
        mpz_neg(d_.gmp_, d_.gmp_);
        return std::move(*this);
    } else if (d_.native_ == LONG_MIN) {
        // Overflow, just.
        return { detail::absLongMin, detail::GMPTag() };
    } else
        return -d_.native_; // also cheap
}

template <bool withInfinity>
IntegerBase<withInfinity>& IntegerBase<withInfinity>::operator +=(
        const IntegerBase& other) {
    if constexpr (withInfinity) {
        if (rep_ == REP_INFINITE)
            return *this;
        else if (other.rep_ == REP_INFINITE) {
            makeInfinite();
            return *this;
        }
    }

    // From here on, all integers are finite.
    if (other.rep_) {
        if (! rep_)
            forceLarge();
        mpz_add(d_.gmp_, d_.gmp_, other.d_.gmp_);
        return *this;
    }

    // At this point we need to add the long integer other.d_.native_.
    // What follows is a stripped-down copy of the operator += (long).
    if (other.d_.native_ >= 0) {
        if (! rep_) {
            // We have long += long.  Stay native as long as we won't overflow.
            if (d_.native_ <= (LONG_MAX - other.d_.native_)) {
                d_.native_ += other.d_.native_;
            } else {
                // It will overflow.  Fall back to large integer arithmetic.
                // Beware: we could have other and this as the same object, so
                // we need to back up the RHS before converting this to GMP.
                long summand = other.d_.native_;
                forceLarge();
                mpz_add_ui(d_.gmp_, d_.gmp_, summand);
            }
        } else {
            mpz_add_ui(d_.gmp_, d_.gmp_, other.d_.native_);
        }
    } else {
        if (! rep_) {
            // We have long += long.  Stay native as long as we won't overflow.
            if (d_.native_ >= (LONG_MIN - other.d_.native_)) {
                d_.native_ += other.d_.native_;
            } else {
                // It will overflow.  Fall back to large integer arithmetic.
                // Again: we could have other and this as the same object, so
                // we need to back up the RHS before converting this to GMP.
                long summand = detail::negateToUnsignedType(other.d_.native_);
                forceLarge();
                mpz_sub_ui(d_.gmp_, d_.gmp_, summand);
            }
        } else {
            mpz_sub_ui(d_.gmp_, d_.gmp_,
                detail::negateToUnsignedType(other.d_.native_));
        }
    }
    return *this;
}

template <bool withInfinity>
IntegerBase<withInfinity>& IntegerBase<withInfinity>::operator +=(
        IntegerBase&& other) {
    if constexpr (withInfinity) {
        if (rep_ == REP_INFINITE)
            return *this;
        else if (other.rep_ == REP_INFINITE) {
            makeInfinite();
            return *this;
        }
    }

    // From here on, all integers are finite.
    if (rep_) {
        if (other.rep_) {
            mpz_add(d_.gmp_, d_.gmp_, other.d_.gmp_);
        } else if (other.d_.native_ >= 0) {
            mpz_add_ui(d_.gmp_, d_.gmp_, other.d_.native_);
        } else {
            mpz_sub_ui(d_.gmp_, d_.gmp_,
                detail::negateToUnsignedType(other.d_.native_));
        }
    } else if (other.rep_) {
        if (d_.native_ >= 0) {
            mpz_add_ui(other.d_.gmp_, other.d_.gmp_, d_.native_);
        } else {
            mpz_sub_ui(other.d_.gmp_, other.d_.gmp_,
                detail::negateToUnsignedType(d_.native_));
        }
        rep_ = REP_GMP;
        *d_.gmp_ = *other.d_.gmp_;
        other.rep_ = REP_NATIVE; // becomes valid but undefined
    } else {
        // Both integers are stored as native longs.
        // What follows is a stripped-down copy of the operator += (long).
        if (other.d_.native_ >= 0) {
            if (d_.native_ <= (LONG_MAX - other.d_.native_)) {
                d_.native_ += other.d_.native_;
            } else {
                // It will overflow.  Fall back to large integer arithmetic.
                forceLarge();
                mpz_add_ui(d_.gmp_, d_.gmp_, other.d_.native_);
            }
        } else {
            if (d_.native_ >= (LONG_MIN - other.d_.native_)) {
                d_.native_ += other.d_.native_;
            } else {
                // It will overflow.  Fall back to large integer arithmetic.
                forceLarge();
                mpz_sub_ui(d_.gmp_, d_.gmp_,
                    detail::negateToUnsignedType(other.d_.native_));
            }
        }
    }
    return *this;
}

template <bool withInfinity>
template <CppInteger IntType>
IntegerBase<withInfinity>& IntegerBase<withInfinity>::operator +=(
        IntType other) {
    if constexpr (withInfinity)
        if (rep_ == REP_INFINITE)
            return *this;

    // From here on, all integers are finite.
    if (! rep_) {
        // Use native arithmetic if we can.
        // Note: both signed and unsigned integer _conversion_ are guaranteed
        // to be correct modulo 2^bits (as of C++20); however, signed integer
        // _arithmetic_ has undefined overflow behaviour.  Be careful.
        if constexpr (UnsignedCppInteger<IntType>) {
            if (other <= detail::differenceAsUnsigned(LONG_MAX, d_.native_)) {
                // A consequence: 0 ≤ other ≤ ULONG_MAX.
                // If sizeof(IntType) < sizeof(long) then I understand the
                // operation takes place via long, and this is fine since
                // other will fit into a long.
                // If sizeof(IntType) ≥ sizeof(long) then I understand the
                // operation takes place via IntType (which is unsigned,
                // and therefore overflow behaviour is well-defined).
                // Casting d_.native_ up, performing the addition and then
                // casting the result down could all introduce errors; however,
                // I understand these errors are all guaranteed to be
                // ± 2^long_bits and/or ± 2^IntType_bits, which means the
                // final result should still be correct.
                d_.native_ += other;
                return *this;
            }
        } else {
            if ((other >= 0 && d_.native_ <= (LONG_MAX - other)) ||
                    (other < 0 && d_.native_ >= (LONG_MIN - other))) {
                // A consequence: -ULONG_MAX ≤ other ≤ ULONG_MAX.
                // If other does not fit into a long, then we must have
                // sizeof(IntType) > sizeof(long).  I understand this means the
                // operation takes place via IntType (correctly) and then gets
                // cast down to long (again correctly).
                d_.native_ += other;
                return *this;
            }
        }
        // It will overflow.
        // Fall back to the large integer arithmetic in the next block.
        forceLarge();
    }

    // And now we're down to GMP arithmetic.
    if constexpr (sizeof(IntType) <= sizeof(long)) {
        if constexpr (UnsignedCppInteger<IntType>) {
            mpz_add_ui(d_.gmp_, d_.gmp_, other);
        } else if (other >= 0) {
            mpz_add_ui(d_.gmp_, d_.gmp_, other);
        } else {
            mpz_sub_ui(d_.gmp_, d_.gmp_, detail::negateToUnsignedType(other));
        }
    } else {
        // TODO: Improve this (the case where IntType is wider than long).
        return (*this) += IntegerBase(other);
    }

    return *this;
}

template <bool withInfinity>
IntegerBase<withInfinity>& IntegerBase<withInfinity>::operator -=(
        const IntegerBase& other) {
    if constexpr (withInfinity) {
        if (rep_ == REP_INFINITE)
            return *this;
        else if (other.rep_ == REP_INFINITE) {
            makeInfinite();
            return *this;
        }
    }

    // From here on, all integers are finite.
    if (other.rep_) {
        if (! rep_)
            forceLarge();
        mpz_sub(d_.gmp_, d_.gmp_, other.d_.gmp_);
        return *this;
    }

    // At this point we need to subtract the long integer other.d_.native_.
    // What follows is a stripped-down copy of the operator -= (long).
    if (other.d_.native_ >= 0) {
        if (! rep_) {
            // We have long -= long.  Stay native as long as we won't overflow.
            if (d_.native_ >= other.d_.native_ + LONG_MIN) {
                d_.native_ -= other.d_.native_;
            } else {
                // It will overflow.  Fall back to large integer arithmetic.
                // Beware: we could have other and this as the same object, so
                // we need to back up the RHS before converting this to GMP.
                long summand = other.d_.native_;
                forceLarge();
                mpz_sub_ui(d_.gmp_, d_.gmp_, summand);
            }
        } else {
            mpz_sub_ui(d_.gmp_, d_.gmp_, other.d_.native_);
        }
    } else {
        if (! rep_) {
            // We have long -= long.  Stay native as long as we won't overflow.
            if (other < 0 && d_.native_ <= other + LONG_MAX) {
                d_.native_ -= other.d_.native_;
            } else {
                // It will overflow.  Fall back to large integer arithmetic.
                // Again: we could have other and this as the same object, so
                // we need to back up the RHS before converting this to GMP.
                long summand = detail::negateToUnsignedType(other.d_.native_);
                forceLarge();
                mpz_add_ui(d_.gmp_, d_.gmp_, summand);
            }
        } else {
            mpz_add_ui(d_.gmp_, d_.gmp_,
                detail::negateToUnsignedType(other.d_.native_));
        }
    }
    return *this;
}

template <bool withInfinity>
IntegerBase<withInfinity>& IntegerBase<withInfinity>::operator -=(
        IntegerBase&& other) {
    if constexpr (withInfinity) {
        if (rep_ == REP_INFINITE)
            return *this;
        else if (other.rep_ == REP_INFINITE) {
            makeInfinite();
            return *this;
        }
    }

    // From here on, all integers are finite.
    if (rep_) {
        if (other.rep_) {
            mpz_sub(d_.gmp_, d_.gmp_, other.d_.gmp_);
        } else if (other.d_.native_ >= 0) {
            mpz_sub_ui(d_.gmp_, d_.gmp_, other.d_.native_);
        } else {
            mpz_add_ui(d_.gmp_, d_.gmp_,
                detail::negateToUnsignedType(other.d_.native_));
        }
    } else if (other.rep_) {
        if (d_.native_ >= 0) {
            mpz_sub_ui(other.d_.gmp_, other.d_.gmp_, d_.native_);
        } else {
            mpz_add_ui(other.d_.gmp_, other.d_.gmp_,
                detail::negateToUnsignedType(d_.native_));
        }
        rep_ = REP_GMP;
        *d_.gmp_ = *other.d_.gmp_;
        mpz_neg(d_.gmp_, d_.gmp_);
        other.rep_ = REP_NATIVE; // becomes valid but undefined
    } else {
        // Both integers are stored as native longs.
        // What follows is a stripped-down copy of the operator -= (long).
        if (other.d_.native_ >= 0) {
            if (d_.native_ >= other.d_.native_ + LONG_MIN) {
                d_.native_ -= other.d_.native_;
            } else {
                // It will overflow.  Fall back to large integer arithmetic.
                forceLarge();
                mpz_sub_ui(d_.gmp_, d_.gmp_, other.d_.native_);
            }
        } else {
            if (d_.native_ <= other.d_.native_ + LONG_MAX) {
                d_.native_ -= other.d_.native_;
            } else {
                // It will overflow.  Fall back to large integer arithmetic.
                forceLarge();
                mpz_add_ui(d_.gmp_, d_.gmp_,
                    detail::negateToUnsignedType(other.d_.native_));
            }
        }
    }
    return *this;
}

template <bool withInfinity>
template <CppInteger IntType>
IntegerBase<withInfinity>& IntegerBase<withInfinity>::operator -=(
        IntType other) {
    if constexpr (withInfinity)
        if (rep_ == REP_INFINITE)
            return *this;

    // From here on, all integers are finite.
    if (! rep_) {
        // Use native arithmetic if we can.
        // Note: both signed and unsigned integer _conversion_ are guaranteed
        // to be correct modulo 2^bits (as of C++20); however, signed integer
        // _arithmetic_ has undefined overflow behaviour.  Be careful.
        if constexpr (UnsignedCppInteger<IntType>) {
            if (other <= detail::differenceAsUnsigned(d_.native_, LONG_MIN)) {
                // A consequence: 0 ≤ other ≤ ULONG_MAX.
                // If sizeof(IntType) < sizeof(long) then I understand the
                // operation takes place via long, and this is fine since
                // other will fit into a long.
                // If sizeof(IntType) ≥ sizeof(long) then I understand the
                // operation takes place via IntType (which is unsigned,
                // and therefore overflow behaviour is well-defined).
                // Casting d_.native_ up, performing the subtraction and then
                // casting the result down could all introduce errors; however,
                // I understand these errors are all guaranteed to be
                // ± 2^long_bits and/or ± 2^IntType_bits, which means the
                // final result should still be correct.
                d_.native_ -= other;
                return *this;
            }
        } else {
            if ((other >= 0 && d_.native_ >= other + LONG_MIN) ||
                    (other < 0 && d_.native_ <= other + LONG_MAX)) {
                // A consequence: -ULONG_MAX ≤ other ≤ ULONG_MAX.
                // If other does not fit into a long, then we must have
                // sizeof(IntType) > sizeof(long).  I understand this means the
                // operation takes place via IntType (correctly) and then gets
                // cast down to long (again correctly).
                d_.native_ -= other;
                return *this;
            }
        }
        // It will overflow.
        // Fall back to the large integer arithmetic in the next block.
        forceLarge();
    }

    // And now we're down to large integer arithmetic (large != null).
    if constexpr (sizeof(IntType) <= sizeof(long)) {
        if constexpr (UnsignedCppInteger<IntType>) {
            mpz_sub_ui(d_.gmp_, d_.gmp_, other);
        } else if (other >= 0) {
            mpz_sub_ui(d_.gmp_, d_.gmp_, other);
        } else {
            mpz_add_ui(d_.gmp_, d_.gmp_, detail::negateToUnsignedType(other));
        }
    } else {
        // TODO: Improve this (the case where IntType is wider than long).
        return (*this) -= IntegerBase(other);
    }

    return *this;
}

template <bool withInfinity>
template <CppInteger IntType>
IntegerBase<withInfinity>& IntegerBase<withInfinity>::operator *=(
        IntType other) {
    if constexpr (withInfinity)
        if (rep_ == REP_INFINITE)
            return *this;

    // From here on, all integers are finite.
    if constexpr (sizeof(IntType) <= sizeof(long)) {
        if (rep_) {
            if (other == 0) {
                mpz_clear(d_.gmp_);
                rep_ = REP_NATIVE;
                d_.native_ = 0; // overwrites d_.gmp_
            } else if constexpr (SignedCppInteger<IntType>) {
                mpz_mul_si(d_.gmp_, d_.gmp_, other);
            } else {
                mpz_mul_ui(d_.gmp_, d_.gmp_, other);
            }
        } else {
            // Note: even if other is unsigned, casting it to DoubleLong will do
            // the cast correctly, and the multiplication should not overflow.
            // Moreover, the multiplication cannot reach the minimum possible
            // DoubleLong, which means we can safely negate the result.
            DoubleLong ans = static_cast<DoubleLong>(d_.native_) *
                static_cast<DoubleLong>(other);
            if (ans > LONG_MAX || ans < LONG_MIN) {
                // Overflow.
                rep_ = REP_GMP;
                mpz_init(d_.gmp_);
                if (ans >= 0) {
                    mpz_import(d_.gmp_, 1 /* word count */, 1 /* word order */,
                        sizeof(DoubleLong) /* word size */,
                        0 /* native endianness */, 0 /* full words */, &ans);
                } else {
                    // mpz_import assumes an unsigned type.
                    // C++20 mandates a two's complement representation, and
                    // we use that here.
                    ans = -ans;
                    mpz_import(d_.gmp_, 1 /* word count */, 1 /* word order */,
                        sizeof(DoubleLong) /* word size */,
                        0 /* native endianness */, 0 /* full words */, &ans);
                    mpz_neg(d_.gmp_, d_.gmp_);
                }
            } else
                d_.native_ = static_cast<long>(ans);
        }
        return *this;
    } else {
        // Before we pull out the heavy machinery, look for more easy solutions.
        // (The test below does not capture _all_ representations of zero, but
        // it _does_ capture the ones that are trivial to test.)
        if ((! rep_) && d_.native_ == 0)
            return *this;

        // TODO: Improve this (the case where IntType is wider than long).
        return (*this) *= IntegerBase(other);
    }
}

template <bool withInfinity>
template <CppInteger IntType>
IntegerBase<withInfinity>& IntegerBase<withInfinity>::operator /=(
        IntType other) {
    if constexpr (withInfinity) {
        if (rep_ == REP_INFINITE)
            return *this;
        if (other == 0) {
            makeInfinite();
            return *this;
        }
    } else {
        if (other == 0)
            throw DivisionByZero();
    }

    // From here on, all integers (including the result) are finite.
    if (rep_) {
        if constexpr (sizeof(IntType) <= sizeof(long)) {
            if constexpr (UnsignedCppInteger<IntType>) {
                mpz_tdiv_q_ui(d_.gmp_, d_.gmp_, other);
            } else if (other >= 0) {
                mpz_tdiv_q_ui(d_.gmp_, d_.gmp_, other);
            } else {
                mpz_tdiv_q_ui(d_.gmp_, d_.gmp_,
                    detail::negateToUnsignedType(other));
                mpz_neg(d_.gmp_, d_.gmp_);
            }
        } else {
            // TODO: Improve this (the case where IntType is wider than long).
            return (*this) /= IntegerBase(other);
        }
    } else {
        if constexpr (UnsignedCppInteger<IntType>) {
            // We can do this all in native arithmetic.
            if constexpr (sizeof(IntType) < sizeof(long))
                d_.native_ /= static_cast<long>(other);
            else if (other <= static_cast<unsigned long>(LONG_MAX))
                d_.native_ /= static_cast<long>(other);
            else if (other == detail::absLongMin)
                d_.native_ = (d_.native_ == LONG_MIN ? -1 : 0);
            else
                d_.native_ = 0;
        } else if (d_.native_ == LONG_MIN && other == -1) {
            // This is the special case where we must switch from native to
            // large integers.
            rep_ = REP_GMP;
            mpz_init_set_ui(d_.gmp_, detail::absLongMin);
        } else {
            // We can do this all in native arithmetic.
            if constexpr (sizeof(IntType) <= sizeof(long))
                d_.native_ /= other;
            else if (other >= LONG_MIN && other <= LONG_MAX)
                d_.native_ /= static_cast<long>(other);
            else if (other == static_cast<IntType>(LONG_MAX) + 1)
                d_.native_ = (d_.native_ == LONG_MIN ? -1 : 0);
            else
                d_.native_ = 0; // since |other| > |small|
        }
    }
    return *this;
}

template <bool withInfinity>
template <CppInteger IntType>
IntegerBase<withInfinity>& IntegerBase<withInfinity>::divByExact(
        IntType other) {
    // Preconditions: this is finite; other ≠ 0; (this / other) is an integer.
    if constexpr (sizeof(IntType) <= sizeof(long)) {
        if constexpr (UnsignedCppInteger<IntType>) {
            if (rep_) {
                mpz_divexact_ui(d_.gmp_, d_.gmp_, other);
            } else {
                // We can do this entirely in native arithmetic.
                // Our precondition implies: other ≤ |d_.native_|, or
                // d_.native_ == 0 (and in the latter case this is a no-op).
                if (d_.native_ != 0) {
                    // We have other ≤ |d_.native_|.  The only case where other
                    // _cannot_ fit into a signed long is if other == |LONG_MIN|
                    // (and therefore d_.native_ == LONG_MIN).
                    if (other == static_cast<unsigned long>(LONG_MIN))
                        d_.native_ = -1;
                    else
                        d_.native_ /= static_cast<long>(other);
                }
            }
        } else {
            if (rep_) {
                if (other >= 0)
                    mpz_divexact_ui(d_.gmp_, d_.gmp_, other);
                else {
                    mpz_divexact_ui(d_.gmp_, d_.gmp_,
                        detail::negateToUnsignedType(other));
                    mpz_neg(d_.gmp_, d_.gmp_);
                }
            } else if (d_.native_ == LONG_MIN && other == -1) {
                // This is the special case where we must switch from native to
                // large integers.
                rep_ = REP_GMP;
                mpz_init_set_ui(d_.gmp_, detail::absLongMin);
            } else {
                // We can do this entirely in signed native arithmetic.
                d_.native_ /= other;
            }
        }
        return *this;
    } else {
        // TODO: Improve this (the case where IntType is wider than long).
        return divByExact(IntegerBase(other));
    }
}

template <bool withInfinity>
template <CppInteger IntType>
IntegerBase<withInfinity>& IntegerBase<withInfinity>::operator %=(
        IntType other) {
    if (other == 0)
        throw DivisionByZero();
    if constexpr (withInfinity)
        if (rep_ == REP_INFINITE) {
            rep_ = REP_NATIVE;
            d_.native_ = 0;
            return *this;
        }

    // Now we have this != infinity, and other != 0.
    // From here on, all integers are finite and no exceptions are thrown.
    if (rep_) {
        if constexpr (sizeof(IntType) <= sizeof(long)) {
            if constexpr (UnsignedCppInteger<IntType>) {
                mpz_tdiv_r_ui(d_.gmp_, d_.gmp_, other);
            } else if (other >= 0) {
                mpz_tdiv_r_ui(d_.gmp_, d_.gmp_, other);
            } else {
                // We use the fact that (this % other) == (this % |other|).
                mpz_tdiv_r_ui(d_.gmp_, d_.gmp_,
                    detail::negateToUnsignedType(other));
            }
            if constexpr (sizeof(IntType) < sizeof(long))
                forceReduce();
        } else {
            // TODO: Improve this (the case where IntType is wider than long).
            return (*this) %= IntegerBase(other);
        }
    } else {
        // We can do this all in native arithmetic.
        // Note: some compilers crash on LONG_MIN % -1.
        if constexpr (UnsignedCppInteger<IntType>) {
            if constexpr (sizeof(IntType) < sizeof(long))
                d_.native_ %= static_cast<long>(other);
            else if (other <= static_cast<unsigned long>(LONG_MAX))
                d_.native_ %= static_cast<long>(other);
            else if (other == detail::absLongMin && d_.native_ == LONG_MIN)
                d_.native_ = 0;
            // Otherwise we have |other| > |d_.native_|, so d_.native_
            // remains fixed.
        } else if (other == -1) {
            d_.native_ = 0;
        } else {
            if constexpr (sizeof(IntType) <= sizeof(long))
                d_.native_ %= other;
            else if (other >= LONG_MIN && other <= LONG_MAX)
                d_.native_ %= static_cast<long>(other);
            else if (other == static_cast<IntType>(LONG_MAX) + 1 &&
                    d_.native_ == LONG_MIN)
                d_.native_ = 0;
            // Otherwise we have |other| > |d_.native_|, so d_.native_
            // remains fixed.
        }
    }
    return *this;
}

template <bool withInfinity>
void IntegerBase<withInfinity>::addProduct(
        const IntegerBase& x, const IntegerBase& y) {
    if constexpr (withInfinity) {
        if (rep_ == REP_INFINITE)
            return;
        if (x.rep_ == REP_INFINITE || y.rep_ == REP_INFINITE) {
            makeInfinite();
            return;
        }
    }
    if (x.isZero() || y.isZero())
        return;

    // From here on, all integers are finite and the arguments are non-zero
    // (so there is actually something to do).
    //
    // Note: GMP functions explicitly allow the input and output
    // variables to be the same (so x.addProduct(x, x) is fine, for example).
    if (rep_) {
        if (x.rep_) {
            if (y.rep_)
                mpz_addmul(d_.gmp_, x.d_.gmp_, y.d_.gmp_);
            else if (y.d_.native_ > 0)
                mpz_addmul_ui(d_.gmp_, x.d_.gmp_, y.d_.native_);
            else
                mpz_submul_ui(d_.gmp_, x.d_.gmp_,
                    detail::negateToUnsignedType(y.d_.native_));
        } else if (y.rep_) {
            if (x.d_.native_ > 0)
                mpz_addmul_ui(d_.gmp_, y.d_.gmp_, x.d_.native_);
            else
                mpz_submul_ui(d_.gmp_, y.d_.gmp_,
                    detail::negateToUnsignedType(x.d_.native_));
        } else {
            // GMP += native * native
            *this += x * y; // TODO: Do we want to do anything special here?
        }
    } else {
        if (x.rep_ || y.rep_) {
            // Native += { GMP * native | native * GMP | GMP * GMP }
            *this += x * y; // TODO: Do we want to do anything special here?
        } else {
            // Native += native * native
            // We can streamline the implementation a little here, since the
            // result and its negation will both never overflow a DoubleLong.
            DoubleLong ans = (static_cast<DoubleLong>(x.d_.native_) *
                y.d_.native_) + d_.native_;
            if (ans > LONG_MAX || ans < LONG_MIN) {
                // Overflows a long.
                rep_ = REP_GMP;
                mpz_init(d_.gmp_);
                if (ans >= 0) {
                    mpz_import(d_.gmp_, 1 /* word count */, 1 /* word order */,
                        sizeof(DoubleLong) /* word size */,
                        0 /* native endianness */, 0 /* full words */, &ans);
                } else {
                    // mpz_import assumes an unsigned type.
                    ans = -ans;
                    mpz_import(d_.gmp_, 1 /* word count */, 1 /* word order */,
                        sizeof(DoubleLong) /* word size */,
                        0 /* native endianness */, 0 /* full words */, &ans);
                    mpz_neg(d_.gmp_, d_.gmp_);
                }
            } else
                d_.native_ = static_cast<long>(ans);
        }
    }
}

template <bool withInfinity>
inline void IntegerBase<withInfinity>::negate() {
    if constexpr (withInfinity)
        if (rep_ == REP_INFINITE)
            return;

    // From here on, all integers are finite.
    if (rep_)
        mpz_neg(d_.gmp_, d_.gmp_);
    else if (d_.native_ == LONG_MIN) {
        // Overflow, just.
        rep_ = REP_GMP;
        mpz_init_set_ui(d_.gmp_, detail::absLongMin);
    } else
        d_.native_ = -d_.native_;
}

template <bool withInfinity>
inline IntegerBase<withInfinity> IntegerBase<withInfinity>::abs() const& {
    if constexpr (withInfinity)
        if (rep_ == REP_INFINITE)
            return { detail::InfiniteTag() };

    // From here on, all integers are finite.
    if (rep_) {
        IntegerBase ans(detail::GMPTag{});
        mpz_abs(ans.d_.gmp_, d_.gmp_);
        return ans;
    } else if (d_.native_ == LONG_MIN) {
        // Overflow, just.
        return { detail::absLongMin, detail::GMPTag() };
    } else
        return (d_.native_ >= 0 ? d_.native_ : - d_.native_);
}

template <bool withInfinity>
inline IntegerBase<withInfinity> IntegerBase<withInfinity>::abs() && {
    if constexpr (withInfinity)
        if (rep_ == REP_INFINITE)
            return { detail::InfiniteTag() };

    if (rep_) {
        // This operation is very cheap, which is the main reason we want a
        // move variant of absolute value.
        mpz_abs(d_.gmp_, d_.gmp_);
        return std::move(*this);
    } else if (d_.native_ == LONG_MIN) {
        // Overflow, just.
        return { detail::absLongMin, detail::GMPTag() };
    } else
        return (d_.native_ >= 0 ? d_.native_ : - d_.native_); // also cheap
}

template <bool withInfinity>
IntegerBase<withInfinity> IntegerBase<withInfinity>::gcd(
        const IntegerBase& other) const& {
    // Do the standard thing for now.
    IntegerBase ans(*this);
    ans.gcdWith(other);
    return ans;
}

template <bool withInfinity>
IntegerBase<withInfinity> IntegerBase<withInfinity>::gcd(
        IntegerBase&& other) const& {
    // Do the standard thing for now.
    other.gcdWith(*this);
    return std::move(other);
}

template <bool withInfinity>
IntegerBase<withInfinity> IntegerBase<withInfinity>::gcd(
        const IntegerBase& other) && {
    // Do the standard thing for now.
    gcdWith(other);
    return std::move(*this);
}

template <bool withInfinity>
IntegerBase<withInfinity> IntegerBase<withInfinity>::gcd(
        IntegerBase&& other) && {
    // Do the standard thing for now.
    gcdWith(std::move(other));
    return std::move(*this);
}

template <bool withInfinity>
IntegerBase<withInfinity> IntegerBase<withInfinity>::lcm(
        const IntegerBase& other) const& {
    // Do the standard thing for now.
    IntegerBase ans(*this);
    ans.lcmWith(other);
    return ans;
}

template <bool withInfinity>
IntegerBase<withInfinity> IntegerBase<withInfinity>::lcm(
        IntegerBase&& other) const& {
    // Do the standard thing for now.
    other.lcmWith(*this);
    return std::move(other);
}

template <bool withInfinity>
IntegerBase<withInfinity> IntegerBase<withInfinity>::lcm(
        const IntegerBase& other) && {
    // Do the standard thing for now.
    lcmWith(other);
    return std::move(*this);
}

template <bool withInfinity>
IntegerBase<withInfinity> IntegerBase<withInfinity>::lcm(
        IntegerBase&& other) && {
    // Do the standard thing for now.
    lcmWith(std::move(other));
    return std::move(*this);
}

template <bool withInfinity, CppInteger IntType>
inline IntegerBase<withInfinity> operator +(IntType lhs,
        const IntegerBase<withInfinity>& rhs) {
    return rhs + lhs;
}

template <bool withInfinity, CppInteger IntType>
inline IntegerBase<withInfinity> operator +(IntType lhs,
        IntegerBase<withInfinity>&& rhs) {
    return std::move(rhs) + lhs;
}

template <bool withInfinity, CppInteger IntType>
inline IntegerBase<withInfinity> operator *(IntType lhs,
        const IntegerBase<withInfinity>& rhs) {
    return rhs * lhs;
}

template <bool withInfinity, CppInteger IntType>
inline IntegerBase<withInfinity> operator *(IntType lhs,
        IntegerBase<withInfinity>&& rhs) {
    return std::move(rhs) * lhs;
}

template <bool withInfinity>
inline std::tuple<IntegerBase<withInfinity>, IntegerBase<withInfinity>,
        IntegerBase<withInfinity>> IntegerBase<withInfinity>::gcdWithCoeffs(
        const IntegerBase& other) const {
    // In the long term, this will eventually become the preferred
    // implementation.  For now though, just forward to the non-tuple variant.
    std::tuple<IntegerBase, IntegerBase, IntegerBase> ans;
    std::get<0>(ans) = gcdWithCoeffs(other, std::get<1>(ans), std::get<2>(ans));
    return ans;
}

template <bool withInfinity>
inline void IntegerBase<withInfinity>::setRaw(mpz_srcptr fromData) {
    if constexpr (withInfinity) {
        if (rep_ == REP_GMP) {
            mpz_set(d_.gmp_, fromData);
        } else {
            rep_ = REP_GMP;
            mpz_init_set(d_.gmp_, fromData);
        }
    } else {
        if (rep_) {
            mpz_set(d_.gmp_, fromData);
        } else {
            rep_ = REP_GMP;
            mpz_init_set(d_.gmp_, fromData);
        }
    }
}

template <bool withInfinity>
inline mpz_srcptr IntegerBase<withInfinity>::rawData() const {
    // Precondition: this integer is finite.
    // Cast away the const, since we are not changing the mathematical value.
    // We are, however, bulking up the representation.
    const_cast<IntegerBase&>(*this).makeLarge();
    return d_.gmp_;
}

template <bool withInfinity>
inline mpz_ptr IntegerBase<withInfinity>::rawData() {
    // Precondition: this integer is finite.
    makeLarge();
    return d_.gmp_;
}

template <bool withInfinity>
inline void IntegerBase<withInfinity>::makeLarge() {
    // Note: this does the right thing if this integer is infinite.
    if (! rep_)
        forceLarge();
}

template <bool withInfinity>
inline void IntegerBase<withInfinity>::tryReduce() {
    // Note: this does the right thing if this integer is infinite.
    if constexpr (withInfinity) {
        if (rep_ == REP_GMP && mpz_cmp_si(d_.gmp_, LONG_MAX) <= 0 &&
                mpz_cmp_si(d_.gmp_, LONG_MIN) >= 0)
            forceReduce();
    } else {
        if (rep_ && mpz_cmp_si(d_.gmp_, LONG_MAX) <= 0 &&
                mpz_cmp_si(d_.gmp_, LONG_MIN) >= 0)
            forceReduce();
    }
}

template <bool withInfinity>
inline void IntegerBase<withInfinity>::tightEncode(std::ostream& out) const {
    regina::detail::tightEncodeInteger(out, *this);
}

template <bool withInfinity>
inline std::string IntegerBase<withInfinity>::tightEncoding() const {
    std::ostringstream out;
    regina::detail::tightEncodeInteger(out, *this);
    return std::move(out).str();
}

template <bool withInfinity>
inline IntegerBase<withInfinity> IntegerBase<withInfinity>::tightDecoding(
        const std::string& enc) {
    return regina::detail::tightDecodeInteger<IntegerBase>(
        enc.begin(), enc.end(), true);
}

template <bool withInfinity>
inline IntegerBase<withInfinity> IntegerBase<withInfinity>::tightDecode(
        std::istream& input) {
    try {
        return regina::detail::tightDecodeInteger<IntegerBase>(
            std::istreambuf_iterator<char>(input),
            std::istreambuf_iterator<char>(), false);
    } catch (const InvalidArgument& exc) {
        // For input streams we use a different exception type.
        throw InvalidInput(exc.what());
    }
}

template <bool withInfinity>
inline size_t IntegerBase<withInfinity>::hash() const {
    if constexpr (withInfinity) {
        // For infinity, just return an arbitrary hard-coded constant.
        if (rep_ == REP_INFINITE)
            return 33651164;
    }

    // From here on, all integers are finite.

    // We should ensure that hash(k) != hash(-k).
    //
    // By casting a GMP int to a signed long (if we are using GMP) and then
    // casting a signed long directly to size_t, we get for k > 0:
    //
    // - hash(k) = k;
    // - hash(-k) = 2^n - k (where n is the bitsize of size_t).
    //
    // This is enough distinguishing power for the time being.
    if (rep_)
        return static_cast<size_t>(mpz_get_si(d_.gmp_));
    else
        return static_cast<size_t>(d_.native_);
}

template <bool withInfinity>
inline void IntegerBase<withInfinity>::validate() const {
    // Under the current internal represention, all states are valid.
    // However, we keep this function around for the test suite in case the
    // internal representation should ever change.
}

template <bool withInfinity>
inline void IntegerBase<withInfinity>::forceLarge() {
    // Precondition: rep_ == REP_NATIVE.
    rep_ = REP_GMP;
    // This call should copy d_.native_ before overwriting it as d_.gmp_.
    mpz_init_set_si(d_.gmp_, d_.native_);
}

template <bool withInfinity>
inline void IntegerBase<withInfinity>::forceReduce() {
    // Precondition: rep_ == REP_GMP, and the value fits into a long.
    long extracted = mpz_get_si(d_.gmp_);
    mpz_clear(d_.gmp_);
    rep_ = REP_NATIVE;
    d_.native_ = extracted; // overwrites d_.gmp_
}

#ifndef __DOXYGEN // Doxygen gets confused by the specialisations.

template <>
inline const IntegerBase<true> IntegerBase<true>::infinity(
    detail::InfiniteTag{});

#endif // __DOXYGEN

template <bool withInfinity>
inline void swap(IntegerBase<withInfinity>& a, IntegerBase<withInfinity>& b)
        noexcept {
    a.swap(b);
}

template <bool withInfinity>
void tightEncode(std::ostream& out, IntegerBase<withInfinity> value) {
    regina::detail::tightEncodeInteger(out, std::move(value));
}

template <bool withInfinity>
std::string tightEncoding(IntegerBase<withInfinity> value) {
    std::ostringstream out;
    regina::detail::tightEncodeInteger(out, std::move(value));
    return std::move(out).str();
}

// Inline functions for NativeInteger

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr NativeInteger<bytes>::NativeInteger() : data_(0) {
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr NativeInteger<bytes>::NativeInteger(Native value) : data_(
        value) {
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr NativeInteger<bytes>::NativeInteger(
        const NativeInteger<bytes>& value) : data_(value.data_) {
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
template <bool withInfinity>
inline NativeInteger<bytes>::NativeInteger(
        const IntegerBase<withInfinity>& value) :
        data_(value.template unsafeValue<Native>()) {
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr bool NativeInteger<bytes>::isZero() const {
    return (data_ == 0);
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr int NativeInteger<bytes>::sign() const {
    return (data_ > 0 ? 1 : data_ < 0 ? -1 : 0);
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr typename NativeInteger<bytes>::Native NativeInteger<bytes>::
        nativeValue() const {
    return data_;
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline std::string NativeInteger<bytes>::str() const {
    if constexpr (StandardStringifiable<Native>) {
        return std::to_string(data_);
    } else {
        return regina::toString(data_);
    }
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr NativeInteger<bytes>& NativeInteger<bytes>::operator =(
        const NativeInteger<bytes>& value) {
    data_ = value.data_;
    return *this;
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr NativeInteger<bytes>& NativeInteger<bytes>::operator =(
        Native value) {
    data_ = value;
    return *this;
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr void NativeInteger<bytes>::swap(NativeInteger<bytes>& other)
        noexcept {
    std::swap(data_, other.data_);
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr bool NativeInteger<bytes>::operator ==(
        const NativeInteger<bytes>& rhs) const {
    return (data_ == rhs.data_);
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr bool NativeInteger<bytes>::operator ==(Native rhs) const {
    return (data_ == rhs);
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr std::strong_ordering NativeInteger<bytes>::operator <=> (
        const NativeInteger& rhs) const {
    return data_ <=> rhs.data_;
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr std::strong_ordering NativeInteger<bytes>::operator <=> (
        Native rhs) const {
    return data_ <=> rhs;
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr NativeInteger<bytes>& NativeInteger<bytes>::operator ++() {
    ++data_;
    return *this;
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr NativeInteger<bytes> NativeInteger<bytes>::operator ++(int) {
    return NativeInteger<bytes>(data_++);
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr NativeInteger<bytes>& NativeInteger<bytes>::operator --() {
    --data_;
    return *this;
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr NativeInteger<bytes> NativeInteger<bytes>::operator --(int) {
    return NativeInteger<bytes>(data_--);
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr NativeInteger<bytes> NativeInteger<bytes>::operator +(
        const NativeInteger<bytes>& other) const {
    return NativeInteger<bytes>(data_ + other.data_);
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr NativeInteger<bytes> NativeInteger<bytes>::operator +(
        Native other) const {
    return NativeInteger<bytes>(data_ + other);
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr NativeInteger<bytes> NativeInteger<bytes>::operator -(
        const NativeInteger<bytes>& other) const {
    return NativeInteger<bytes>(data_ - other.data_);
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr NativeInteger<bytes> NativeInteger<bytes>::operator -(
        Native other) const {
    return NativeInteger<bytes>(data_ - other);
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr NativeInteger<bytes> NativeInteger<bytes>::operator *(
        const NativeInteger<bytes>& other) const {
    return NativeInteger<bytes>(data_ * other.data_);
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr NativeInteger<bytes> NativeInteger<bytes>::operator *(
        Native other) const {
    return NativeInteger<bytes>(data_ * other);
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr NativeInteger<bytes> NativeInteger<bytes>::operator /(
        const NativeInteger<bytes>& other) const {
    if (other.data_ == 0)
        throw DivisionByZero();
    return NativeInteger<bytes>(data_ / other.data_);
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr NativeInteger<bytes> NativeInteger<bytes>::operator /(
        Native other) const {
    if (other == 0)
        throw DivisionByZero();
    return NativeInteger<bytes>(data_ / other);
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr NativeInteger<bytes> NativeInteger<bytes>::divExact(
        const NativeInteger<bytes>& other) const {
    return NativeInteger<bytes>(data_ / other.data_);
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr NativeInteger<bytes> NativeInteger<bytes>::divExact(
        Native other) const {
    return NativeInteger<bytes>(data_ / other);
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr NativeInteger<bytes> NativeInteger<bytes>::operator %(
        const NativeInteger<bytes>& other) const {
    if (other.data_ == 0)
        throw DivisionByZero();
    return NativeInteger<bytes>(data_ % other.data_);
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr NativeInteger<bytes> NativeInteger<bytes>::operator %(
        Native other) const {
    if (other == 0)
        throw DivisionByZero();
    return NativeInteger<bytes>(data_ % other);
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr std::pair<NativeInteger<bytes>, NativeInteger<bytes>>
        NativeInteger<bytes>::divisionAlg(const NativeInteger& divisor) const {
    if (divisor == 0)
        return { 0, data_ };

    std::pair<NativeInteger, NativeInteger> ans;

    // Native integer division could leave a negative remainder
    // regardless of the sign of the divisor (I think the standard
    // indicates that the decision is based on the sign of *this?).
    ans.first = data_ / divisor.data_;
    ans.second = data_ - (ans.first.data_ * divisor.data_);
    if (ans.second.data_ < 0) {
        if (divisor.data_ > 0) {
            ans.second.data_ += divisor.data_;
            --ans.first.data_;
        } else {
            ans.second.data_ -= divisor.data_;
            ++ans.first.data_;
        }
    }

    return ans;
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr NativeInteger<bytes> NativeInteger<bytes>::operator -() const {
    return NativeInteger<bytes>(- data_);
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr NativeInteger<bytes>& NativeInteger<bytes>::operator += (
        const NativeInteger<bytes>& other) {
    data_ += other.data_;
    return *this;
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr NativeInteger<bytes>& NativeInteger<bytes>::operator += (
        Native other) {
    data_ += other;
    return *this;
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr NativeInteger<bytes>& NativeInteger<bytes>::operator -= (
        const NativeInteger<bytes>& other) {
    data_ -= other.data_;
    return *this;
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr NativeInteger<bytes>& NativeInteger<bytes>::operator -= (
        Native other) {
    data_ -= other;
    return *this;
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr NativeInteger<bytes>& NativeInteger<bytes>::operator *= (
        const NativeInteger<bytes>& other) {
    data_ *= other.data_;
    return *this;
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr NativeInteger<bytes>& NativeInteger<bytes>::operator *= (
        Native other) {
    data_ *= other;
    return *this;
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr NativeInteger<bytes>& NativeInteger<bytes>::operator /= (
        const NativeInteger<bytes>& other) {
    if (other.data_ == 0)
        throw DivisionByZero();
    data_ /= other.data_;
    return *this;
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr NativeInteger<bytes>& NativeInteger<bytes>::operator /= (
        Native other) {
    if (other == 0)
        throw DivisionByZero();
    data_ /= other;
    return *this;
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr NativeInteger<bytes>& NativeInteger<bytes>::divByExact(
        const NativeInteger<bytes>& other) {
    data_ /= other.data_;
    return *this;
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr NativeInteger<bytes>& NativeInteger<bytes>::divByExact(
        Native other) {
    data_ /= other;
    return *this;
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr NativeInteger<bytes>& NativeInteger<bytes>::operator %= (
        const NativeInteger<bytes>& other) {
    if (other.data_ == 0)
        throw DivisionByZero();
    data_ %= other.data_;
    return *this;
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr NativeInteger<bytes>& NativeInteger<bytes>::operator %= (
        Native other) {
    if (other == 0)
        throw DivisionByZero();
    data_ %= other;
    return *this;
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr void NativeInteger<bytes>::negate() {
    data_ = - data_;
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
constexpr void NativeInteger<bytes>::gcdWith(const NativeInteger<bytes>& other)
        {
    Native a = data_;
    Native b = other.data_;

    if (a < 0) a = -a;
    if (b < 0) b = -b;

    /**
     * Now everything is non-negative.
     * The following code is based on Stein's binary GCD algorithm.
     */
    if (! a) {
        data_ = b;
        return;
    }
    if (! b) {
        data_ = a;
        return;
    }

    // Compute the largest common power of 2.
    int pow2;
    for (pow2 = 0; ! ((a | b) & 1); ++pow2) {
        a >>= 1;
        b >>= 1;
    }

    // Strip out all remaining powers of 2 from a and b.
    while (! (a & 1))
        a >>= 1;
    while (! (b & 1))
        b >>= 1;

    while (a != b) {
        // INV: a and b are both odd and non-zero.
        if (a < b) {
            b -= a;
            do
                b >>= 1;
            while (! (b & 1));
        } else {
            a -= b;
            do
                a >>= 1;
            while (! (a & 1));
        }
    }
    // On arm64 with 128-bit integers I am seeing some very strange behaviour
    // when pow2=0.  For instance, (3 << 0) = (0x0000000300000003).  Aaaugh.
    // Things seem fine for non-zero shifts, so just test for pow2=0 explicitly.
    // (Note: things are working correctly on x86_64 - it's just arm64 that is
    // showing these errors.)
    if (pow2)
        data_ = (a << pow2);
    else
        data_ = a;
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr NativeInteger<bytes> NativeInteger<bytes>::gcd(
        const NativeInteger<bytes>& other) const {
    NativeInteger<bytes> ans(data_);
    ans.gcdWith(other);
    return ans;
}

template <int bytes>
requires (supportsNativeIntegerSize(bytes))
inline constexpr void NativeInteger<bytes>::tryReduce() {
}

template <int bytes>
inline std::ostream& operator << (std::ostream& out,
        const NativeInteger<bytes>& i) {
    if constexpr (bytes <= sizeof(char)) {
        // Make sure the ostream operator write this as an int, not a char.
        return out << int(i.data_);
    } else if constexpr (Writeable<typename NativeInteger<bytes>::Native>) {
        return out << i.data_;
    } else {
        // Presumably this is a 128-bit integer, for which std::to_string()
        // and/or std::ostream output do not exist on some platforms.
        static_assert(bytes >= 16,
            "std::ostream output should be available for all native C++ "
            "integer types with < 128 bits.  Please report this to the "
            "Regina developers.");

        // We still try to use native numerical ostream output where possible.
        if (i.data_ <= LLONG_MAX && i.data_ >= LLONG_MIN) {
            return out << static_cast<long long>(i.data_);
        } else {
            // Fall back to string output.
            return out << i.str();
        }
    }
}

template <int bytes>
inline constexpr void swap(NativeInteger<bytes>& a, NativeInteger<bytes>& b)
        noexcept {
    a.swap(b);
}

} // namespace regina

#endif

