
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
#include <tuple>
#include <gmp.h>
#include "regina-core.h"
#include "utilities/exception.h"
#include "utilities/intutils.h"
#include "utilities/tightencoding.h"

/**
 * \hideinitializer
 *
 * An internal copy of the GMP signed comparison optimisations.
 * This macro should not be used outside this class.
 *
 * By making our own copy of such optimisation macros we can use
 * C++-style casts instead of C-style casts and avoid noisy compiler
 * warnings.  I'd love a better way of doing this.
 *
 * \ingroup maths
 */
#ifdef __GNUC__
    #define mpz_cmp_si_cpp(z, si) \
        (__builtin_constant_p(si) && (si) == 0 ? mpz_sgn(z) : \
        __builtin_constant_p(si) && (si) > 0 ? _mpz_cmp_ui(z, \
            static_cast<unsigned long>(si)) : \
        _mpz_cmp_si(z, si))
#else
    #define mpz_cmp_si_cpp(z, si) _mpz_cmp_si(z, si)
#endif

namespace regina {

template <int bytes>
class NativeInteger;

#ifdef __DOCSTRINGS
class Long; // Represents a Python arbitrary-precision integer.
#endif

/**
 * Internal base classes for use with IntegerBase, templated on whether we
 * should support infinity as an allowed value.
 *
 * See the IntegerBase class notes for details.
 *
 * \ingroup maths
 */
template <bool supportInfinity>
struct InfinityBase;

#ifndef __DOXYGEN
/**
 * An internal base class inherited by LargeInteger, which provides
 * support for infinity as an allowed value.
 *
 * End users should not use this class directly.
 *
 * \ingroup maths
 */
template <>
struct InfinityBase<true> {
    bool infinite_ = false;
        /**< Does this integer represent infinity? */
};

/**
 * An empty internal base class inherited by Integer, which does not
 * support infinity as an allowed value.
 *
 * \ingroup maths
 */
template <>
struct InfinityBase<false> {
};
#endif // __DOXYGEN

namespace detail {
    /**
     * Returns a raw GMP integer holding the given value.
     *
     * \nopython
     *
     * \param value the value to assign to the new GMP integer.
     * @return a corresponding newly created and initialised GMP integer.
     */
    mpz_ptr mpz_from_ll(long long value);

    /**
     * Returns a raw GMP integer holding the given value.
     *
     * \nopython
     *
     * \param value the value to assign to the new GMP integer.
     * @return a corresponding newly created and initialised GMP integer.
     */
    mpz_ptr mpz_from_ull(unsigned long long value);
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
 * This class takes a single boolean argument \a supportInfinity.
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
 * even when passing or returning objects by value.
 *
 * \headers Parts of this template class are implemented in a C++ source
 * file that is not available through the headers.  However, this should
 * not affect users since the calculation engine includes explicit
 * instantiations for all possible template parameters.
 *
 * \ifacespython Both variants of this template are available through Python.
 * For \a supportInfinity = \c false, use the name Integer.
 * For \a supportInfinity = \c true, use the name LargeInteger.
 *
 * \ingroup maths
 */
template <bool supportInfinity = false>
class IntegerBase : private InfinityBase<supportInfinity> {
    public:
        static const IntegerBase<supportInfinity> zero;
            /**< Globally available zero. */
        static const IntegerBase<supportInfinity> one;
            /**< Globally available one. */
        static const IntegerBase<supportInfinity> infinity;
            /**< Globally available infinity.
                 This is only defined if \a supportInfinity is \c true.
                 Any attempt to use it when \a supportInfinity is \c false
                 should generate a linker error. */

    private:
        long small_;
            /**< Contains the native representation of this integer, if
                 we are still using native representations (i.e., if
                 large_ is null).  If we are using GMP large integer
                 representations, or if this integer is infinity, then this
                 native integer is ignored (and may be set to anything). */
        mpz_ptr large_;
            /**< \c null if we are using native representations, or a pointer to
                 the full GMP large integer if we are now using these instead.
                 We require that, whenever this pointer is non-null, the
                 corresponding GMP large integer is initialised.
                 If this integer is infinity then large_ must be \c null. */

    public:
        /**
         * Initialises this integer to zero.
         */
        IntegerBase();
        /**
         * Initialises this integer to the given value.
         *
         * \nopython In Python, the only native-integer constructor
         * is IntegerBase(long).
         *
         * \param value the new value of this integer.
         */
        IntegerBase(int value);
        /**
         * Initialises this integer to the given value.
         *
         * \nopython In Python, the only native-integer constructor
         * is IntegerBase(long).
         *
         * \param value the new value of this integer.
         */
        IntegerBase(unsigned value);
        /**
         * Initialises this integer to the given value.
         *
         * \ifacespython In Python, this is the only native-integer
         * constructor available.
         *
         * \param value the new value of this integer.
         */
        IntegerBase(long value);
        /**
         * Initialises this integer to the given value.
         *
         * \nopython In Python, the only native-integer constructor
         * is IntegerBase(long).
         *
         * \param value the new value of this integer.
         */
        IntegerBase(unsigned long value);
        /**
         * Initialises this integer to the given value.
         *
         * \nopython In Python, the only native-integer constructor
         * is IntegerBase(long).
         *
         * \param value the new value of this integer.
         */
        IntegerBase(long long value);
        /**
         * Initialises this integer to the given value.
         *
         * \nopython In Python, the only native-integer constructor
         * is IntegerBase(long).
         *
         * \param value the new value of this integer.
         */
        IntegerBase(unsigned long long value);
        /**
         * Initialises this integer to the given value.
         *
         * \param value the new value of this integer.
         */
        IntegerBase(const IntegerBase<supportInfinity>& value);
        /**
         * Initialises this integer to the given value.
         *
         * \pre The given integer is not infinite.
         *
         * \param value the new value of this integer.
         */
        IntegerBase(const IntegerBase<! supportInfinity>& value);
        /**
         * Moves the given integer into this new integer.
         * This is a fast (constant time) operation.
         *
         * The integer that is passed (\a src) will no longer be usable.
         *
         * \param src the integer to move.
         */
        IntegerBase(IntegerBase<supportInfinity>&& src) noexcept;
        /**
         * Moves the given integer into this new integer.
         * This is a fast (constant time) operation.
         *
         * The integer that is passed (\a src) will no longer be usable.
         *
         * \pre The given integer is not infinite.
         *
         * \param src the integer to move.
         */
        IntegerBase(IntegerBase<! supportInfinity>&& src) noexcept;
        /**
         * Initialises this integer to the given value.
         *
         * \pre If \a bytes is larger than sizeof(long), then
         * \a bytes is a strict \e multiple of sizeof(long).  For
         * instance, if longs are 8 bytes then you can use \a bytes=16
         * but not \a bytes=12.  This restriction is enforced through a
         * compile-time assertion, but may be lifted in future versions
         * of Regina.
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
         * Initialises this to the given Python arbitrary-precision integer.
         *
         * The argument is of the Python type \c long, which Python uses
         * to store integers of arbitrary magnitude (much like Regina does
         * with its Integer and LargeInteger classes).
         *
         * \nocpp
         *
         * \param value the new value of this integer.
         */
        IntegerBase(Long value);
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
         * If the template argument \a supportInfinity is \c true, then
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
         * If the template argument \a supportInfinity is \c true, then
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
         * @return \c true if and only if we are still using a native
         * C/C++ long.
         */
        bool isNative() const;

        /**
         * Returns whether or not this integer is zero.
         *
         * This is micro-optimised to be faster than simply testing
         * whether (*this) == 0.
         *
         * @return \c true if and only if this integer is zero.
         */
        bool isZero() const;

        /**
         * Returns the sign of this integer.
         *
         * In this routine, infinity is considered to have sign +1.
         *
         * @return +1, -1 or 0 according to whether this integer is
         * positive, negative or zero.
         */
        int sign() const;

        /**
         * Returns whether this integer is infinity.
         *
         * @return \c true if and only if this integer is infinity.
         */
        bool isInfinite() const;

        /**
         * Sets this integer to be infinity.
         *
         * If the template parameter \a supportInfinity is \c false,
         * this routine safely does nothing.
         */
        inline void makeInfinite();

        /**
         * Returns the value of this integer as a long.
         *
         * It is the programmer's reponsibility to ensure that this integer
         * is within the required range.  If this integer is too large or
         * small to fit into a long, then the result will be undefined.
         *
         * Note that, assuming the value is within the required range,
         * this routine will give correct results regardless of whether the
         * underlying representation is a native or large integer.
         *
         * \pre This integer is not infinity.
         *
         * @return the value of this integer.
         */
        long longValue() const;
        /**
         * Returns the value of this integer as a long, or throws an
         * exception if this is not possible.
         *
         * If this integer is within the required range, regardless of
         * whether the underlying representation is a native or large integer,
         * this routine will return the correct result.
         *
         * \exception NoSolution This integer is too large or small to fit
         * into a long.
         *
         * @return the value of this integer.
         */
        long safeLongValue() const;
        /**
         * Returns the value of this integer as a native integer of some
         * fixed byte length.
         *
         * It is the programmer's reponsibility to ensure that this integer
         * is within the required range.  If this integer is too large or
         * small to fit into the return type, then the result will be undefined.
         *
         * Note that, assuming the value is within the required range,
         * this routine will give correct results regardless of whether the
         * underlying representation is a native or large integer.
         *
         * \pre If \a bytes is larger than sizeof(long), then
         * \a bytes is a strict \e multiple of sizeof(long).  For
         * instance, if longs are 8 bytes then you can use this routine
         * with \a bytes=4 or \a bytes=16 but not \a bytes=12.
         * This restriction is enforced through a compile-time assertion,
         * but may be lifted in future versions of Regina.
         *
         * \pre This integer is not infinity.
         *
         * \nopython Python users can use the non-templated longValue()
         * function instead.
         *
         * @return the value of this integer.
         */
        template <int bytes>
        typename IntOfSize<bytes>::type nativeValue() const;
        /**
         * Returns the value of this integer as a string in the given
         * base.  If not specified, the base defaults to 10.
         *
         * If this integer is infinity, the string returned will be
         * \c inf.
         *
         * \pre The given base is between 2 and 36 inclusive.
         *
         * @return the value of this integer as a string.
         */
        std::string stringValue(int base = 10) const;

        /**
         * Sets this integer to the given value.
         *
         * \param value the new value of this integer.
         * @return a reference to this integer with its new value.
         */
        IntegerBase& operator =(const IntegerBase& value);
        /**
         * Sets this integer to the given value.
         *
         * \pre The given integer is not infinite.
         *
         * \param value the new value of this integer.
         * @return a reference to this integer with its new value.
         */
        IntegerBase& operator = (const IntegerBase<! supportInfinity>& value);
        /**
         * Moves the given integer into this integer.
         * This is a fast (constant time) operation.
         *
         * The integer that is passed (\a src) will no longer be usable.
         *
         * \param src the integer to move.
         * @return a reference to this integer.
         */
        IntegerBase& operator = (IntegerBase&& src) noexcept;
        /**
         * Moves the given integer into this integer.
         * This is a fast (constant time) operation.
         *
         * The integer that is passed (\a src) will no longer be usable.
         *
         * \pre The given integer is not infinite.
         *
         * \param src the integer to move.
         * @return a reference to this integer.
         */
        IntegerBase& operator = (IntegerBase<! supportInfinity>&& src) noexcept;
        /**
         * Sets this integer to the given value.
         *
         * \param value the new value of this integer.
         * @return a reference to this integer with its new value.
         */
        IntegerBase& operator =(int value);
        /**
         * Sets this integer to the given value.
         *
         * \param value the new value of this integer.
         * @return a reference to this integer with its new value.
         */
        IntegerBase& operator =(unsigned value);
        /**
         * Sets this integer to the given value.
         *
         * \param value the new value of this integer.
         * @return a reference to this integer with its new value.
         */
        IntegerBase& operator =(long value);
        /**
         * Sets this integer to the given value.
         *
         * \param value the new value of this integer.
         * @return a reference to this integer with its new value.
         */
        IntegerBase& operator =(unsigned long value);
        /**
         * Sets this integer to the given value.
         *
         * \param value the new value of this integer.
         * @return a reference to this integer with its new value.
         */
        IntegerBase& operator =(long long value);
        /**
         * Sets this integer to the given value.
         *
         * \param value the new value of this integer.
         * @return a reference to this integer with its new value.
         */
        IntegerBase& operator =(unsigned long long value);
        /**
         * Sets this integer to the given value which is
         * represented as a string of digits in base 10.
         *
         * Whitespace may be present at the beginning or end of the given
         * string and will simply be ignored.
         *
         * If the template argument \a supportInfinity is \c true, then
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
         * @return a reference to this integer with its new value.
         */
        IntegerBase& operator =(const char* value);
        /**
         * Sets this integer to the given value which is
         * represented as a string of digits in base 10.
         *
         * Whitespace may be present at the beginning or end of the given
         * string and will simply be ignored.
         *
         * If the template argument \a supportInfinity is \c true, then
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
         * @return a reference to this integer with its new value.
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
         * @return \c true if and only if this and the given integer are
         * equal.
         */
        bool operator ==(const IntegerBase& rhs) const;
        /**
         * Determines if this is equal to the given integer.
         *
         * \param rhs the integer with which this will be compared.
         * @return \c true if and only if this and the given integer are
         * equal.
         */
        bool operator ==(const IntegerBase<! supportInfinity>& rhs) const;
        /**
         * Determines if this is equal to the given integer.
         *
         * \param rhs the integer with which this will be compared.
         * @return \c true if and only if this and the given integer are
         * equal.
         */
        bool operator ==(long rhs) const;
        /**
         * Determines if this is not equal to the given integer.
         *
         * \param rhs the integer with which this will be compared.
         * @return \c true if and only if this and the given integer are
         * not equal.
         */
        bool operator !=(const IntegerBase& rhs) const;
        /**
         * Determines if this is not equal to the given integer.
         *
         * \param rhs the integer with which this will be compared.
         * @return \c true if and only if this and the given integer are
         * not equal.
         */
        bool operator !=(const IntegerBase<! supportInfinity>& rhs) const;
        /**
         * Determines if this is not equal to the given integer.
         *
         * \param rhs the integer with which this will be compared.
         * @return \c true if and only if this and the given integer are
         * not equal.
         */
        bool operator !=(long rhs) const;
        /**
         * Determines if this is less than the given integer.
         *
         * \param rhs the integer with which this will be compared.
         * @return \c true if and only if this is less than the given
         * integer.
         */
        bool operator <(const IntegerBase& rhs) const;
        /**
         * Determines if this is less than the given integer.
         *
         * \param rhs the integer with which this will be compared.
         * @return \c true if and only if this is less than the given
         * integer.
         */
        bool operator <(long rhs) const;
        /**
         * Determines if this is greater than the given integer.
         *
         * \param rhs the integer with which this will be compared.
         * @return \c true if and only if this is greater than the given
         * integer.
         */
        bool operator >(const IntegerBase& rhs) const;
        /**
         * Determines if this is greater than the given integer.
         *
         * \param rhs the integer with which this will be compared.
         * @return \c true if and only if this is greater than the given
         * integer.
         */
        bool operator >(long rhs) const;
        /**
         * Determines if this is less than or equal to the given integer.
         *
         * \param rhs the integer with which this will be compared.
         * @return \c true if and only if this is less than or equal to
         * the given integer.
         */
        bool operator <=(const IntegerBase& rhs) const;
        /**
         * Determines if this is less than or equal to the given integer.
         *
         * \param rhs the integer with which this will be compared.
         * @return \c true if and only if this is less than or equal to
         * the given integer.
         */
        bool operator <=(long rhs) const;
        /**
         * Determines if this is greater than or equal to the given integer.
         *
         * \param rhs the integer with which this will be compared.
         * @return \c true if and only if this is greater than or equal
         * to the given integer.
         */
        bool operator >=(const IntegerBase& rhs) const;
        /**
         * Determines if this is greater than or equal to the given integer.
         *
         * \param rhs the integer with which this will be compared.
         * @return \c true if and only if this is greater than or equal
         * to the given integer.
         */
        bool operator >=(long rhs) const;

        /**
         * The preincrement operator.
         * This operator increments this integer by one, and returns a
         * reference to the integer \e after the increment.
         *
         * \nopython The postincrement operator is present in Python as the
         * member function inc().
         *
         * @return a reference to this integer after the increment.
         */
        IntegerBase& operator ++();

        /**
         * The postincrement operator.
         * This operator increments this integer by one, and returns a
         * copy of the integer \e before the increment.
         *
         * \ifacespython This routine is named inc() since python does not
         * support the increment operator.
         *
         * @return a copy of this integer before the increment took place.
         */
        IntegerBase operator ++(int);

        /**
         * The predecrement operator.
         * This operator decrements this integer by one, and returns a
         * reference to the integer \e after the decrement.
         *
         * \nopython The postdecrement operator is present in python as the
         * member function dec().
         *
         * @return a reference to this integer after the decrement.
         */
        IntegerBase& operator --();

        /**
         * The postdecrement operator.
         * This operator decrements this integer by one, and returns a
         * copy of the integer \e before the decrement.
         *
         * \ifacespython This routine is named dec() since python does not
         * support the decrement operator.
         *
         * @return a copy of this integer before the decrement took place.
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
         * @return the sum \a this plus \a other.
         */
        IntegerBase operator +(const IntegerBase& other) const;
        /**
         * Adds this to the given integer and returns the result.
         * This integer is not changed.
         *
         * If either term of the sum is infinite, the result will be
         * infinity.
         *
         * \param other the integer to add to this integer.
         * @return the sum \a this plus \a other.
         */
        IntegerBase operator +(long other) const;
        /**
         * Subtracts the given integer from this and returns the result.
         * This integer is not changed.
         *
         * If either term of the difference is infinite, the result will be
         * infinity.
         *
         * \param other the integer to subtract from this integer.
         * @return the difference \a this minus \a other.
         */
        IntegerBase operator -(const IntegerBase& other) const;
        /**
         * Subtracts the given integer from this and returns the result.
         * This integer is not changed.
         *
         * If either term of the difference is infinite, the result will be
         * infinity.
         *
         * \param other the integer to subtract from this integer.
         * @return the difference \a this minus \a other.
         */
        IntegerBase operator -(long other) const;
        /**
         * Multiplies this by the given integer and returns the
         * result.
         * This integer is not changed.
         *
         * If either factor of the product is infinite, the result will be
         * infinity.
         *
         * \param other the integer to multiply by this integer.
         * @return the product \a this times \a other.
         */
        IntegerBase operator *(const IntegerBase& other) const;
        /**
         * Multiplies this by the given integer and returns the
         * result.
         * This integer is not changed.
         *
         * If either factor of the product is infinite, the result will be
         * infinity.
         *
         * \param other the integer to multiply by this integer.
         * @return the product \a this times \a other.
         */
        IntegerBase operator *(long other) const;
        /**
         * Divides this by the given integer and returns the result.
         * The result will be truncated to an integer, i.e. rounded
         * towards zero.
         * This integer is not changed.
         *
         * If \a other is known to divide this integer exactly,
         * divExact() should be used instead.
         *
         * Infinity divided by anything will return infinity.
         * Anything finite divided by infinity will return zero.
         * Anything finite divided by zero will return infinity.
         *
         * For a division routine that always rounds down, see divisionAlg().
         *
         * \pre If this class does not support infinity, then
         * \a other must be non-zero.
         *
         * \param other the integer to divide this by.
         * @return the quotient \a this divided by \a other.
         */
        IntegerBase operator /(const IntegerBase& other) const;
        /**
         * Divides this by the given integer and returns the result.
         * The result will be truncated to an integer, i.e. rounded
         * towards zero.
         * This integer is not changed.
         *
         * If \a other is known to divide this integer exactly,
         * divExact() should be used instead.
         *
         * Infinity divided by anything will return infinity.
         * Anything finite divided by zero will return infinity.
         *
         * For a division routine that always rounds down, see divisionAlg().
         *
         * \pre If this class does not support infinity, then
         * \a other must be non-zero.
         *
         * \param other the integer to divide this by.
         * @return the quotient \a this divided by \a other.
         */
        IntegerBase operator /(long other) const;
        /**
         * Divides this by the given integer and returns the result.
         * This can only be used when the given integer divides into
         * this exactly, and for large integers can be much faster than
         * ordinary division.  This integer is not changed.
         *
         * \pre The given integer divides exactly into
         * this integer, i.e. \a this divided by \a other is an integer.
         * \pre \a other is not zero.
         * \pre Neither this nor \a other is infinite.
         *
         * \param other the integer to divide this by.
         * @return the quotient \a this divided by \a other.
         */
        IntegerBase divExact(const IntegerBase& other) const;
        /**
         * Divides this by the given integer and returns the result.
         * This can only be used when the given integer divides into
         * this exactly, and for large integers can be much faster than
         * ordinary division.  This integer is not changed.
         *
         * \pre The given integer divides exactly into
         * this integer, i.e. \a this divided by \a other is an integer.
         * \pre \a other is not zero.
         * \pre This integer is not infinite.
         *
         * \param other the integer to divide this by.
         * @return the quotient \a this divided by \a other.
         */
        IntegerBase divExact(long other) const;
        /**
         * Determines the remainder when this integer is divided by the
         * given integer.  If non-zero, the result will have the same sign
         * as this integer.
         * This integer is not changed.
         *
         * For a division routine that always returns a non-negative
         * remainder, see divisionAlg().
         *
         * \pre \a other is not zero.
         * \pre Neither this nor \a other is infinite.
         *
         * \param other the integer to divide this by.
         * @return the remainder \a this modulo \a other.
         */
        IntegerBase operator %(const IntegerBase& other) const;
        /**
         * Determines the remainder when this integer is divided by the
         * given integer.  If non-zero, the result will have the same sign
         * as this integer.
         * This integer is not changed.
         *
         * For a division routine that always returns a non-negative
         * remainder, see divisionAlg().
         *
         * \pre \a other is not zero.
         * \pre This integer is not infinite.
         *
         * \param other the integer to divide this by.
         * @return the remainder \a this modulo \a other.
         */
        IntegerBase operator %(long other) const;

        /**
         * Uses the division algorithm to obtain a quotient and
         * remainder when dividing by the given integer.
         *
         * Suppose this integer is \a n and we pass the divisor \a d.
         * The <em>division algorithm</em> describes the result of
         * dividing \a n by \a d; in particular, it expresses
         * <tt>n = qd + r</tt>, where \a q is the quotient and
         * \a r is the remainder.
         *
         * The division algorithm is precise about which values of \a q
         * and \a r are chosen; in particular it chooses the unique \a r
         * in the range <tt>0 ≤ r < |d|</tt>.
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
         * @return the pair (\a q, \a r), where \a q is the quotient and
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
         * @return the negative of this integer.
         */
        IntegerBase operator -() const;

        /**
         * Adds the given integer to this.
         * This integer is changed to reflect the result.
         *
         * If either term of the sum is infinite, the result will be
         * infinity.
         *
         * \param other the integer to add to this integer.
         * @return a reference to this integer with its new value.
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
         * @return a reference to this integer with its new value.
         */
        IntegerBase& operator +=(long other);
        /**
         * Subtracts the given integer from this.
         * This integer is changed to reflect the result.
         *
         * If either term of the difference is infinite, the result will be
         * infinity.
         *
         * \param other the integer to subtract from this integer.
         * @return a reference to this integer with its new value.
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
         * @return a reference to this integer with its new value.
         */
        IntegerBase& operator -=(long other);
        /**
         * Multiplies the given integer by this.
         * This integer is changed to reflect the result.
         *
         * If either factor of the product is infinite, the result will be
         * infinity.
         *
         * \param other the integer to multiply with this integer.
         * @return a reference to this integer with its new value.
         */
        IntegerBase& operator *=(const IntegerBase& other);
        /**
         * Multiplies the given integer by this.
         * This integer is changed to reflect the result.
         *
         * If either factor of the product is infinite, the result will be
         * infinity.
         *
         * \param other the integer to multiply with this integer.
         * @return a reference to this integer with its new value.
         */
        IntegerBase& operator *=(long other);
        /**
         * Divides this by the given integer.
         * The result will be truncated to an integer, i.e. rounded
         * towards zero.
         * This integer is changed to reflect the result.
         *
         * If \a other is known to divide this integer exactly,
         * divByExact() should be used instead.
         *
         * Infinity divided by anything will return infinity.
         * Anything finite divided by infinity will return zero.
         * Anything finite divided by zero will return infinity.
         *
         * For a division routine that always rounds down, see divisionAlg().
         *
         * \pre If this class does not support infinity, then
         * \a other must be non-zero.
         *
         * \param other the integer to divide this by.
         * @return a reference to this integer with its new value.
         */
        IntegerBase& operator /=(const IntegerBase& other);
        /**
         * Divides this by the given integer.
         * The result will be truncated to an integer, i.e. rounded
         * towards zero.
         * This integer is changed to reflect the result.
         *
         * If \a other is known to divide this integer exactly,
         * divByExact() should be used instead.
         *
         * Infinity divided by anything will return infinity.
         * Anything finite divided by zero will return infinity.
         *
         * For a division routine that always rounds down, see divisionAlg().
         *
         * \pre If this class does not support infinity, then
         * \a other must be non-zero.
         *
         * \param other the integer to divide this by.
         * @return a reference to this integer with its new value.
         */
        IntegerBase& operator /=(long other);
        /**
         * Divides this by the given integer.
         * This can only be used when the given integer divides into
         * this exactly, and for large integers this is much faster than
         * ordinary division.  This integer is changed to reflect the result.
         *
         * \pre The given integer divides exactly into
         * this integer, i.e. \a this divided by \a other is an integer.
         * \pre \a other is not zero.
         * \pre Neither this nor \a other is infinite.
         *
         * \param other the integer to divide this by.
         * @return a reference to this integer with its new value.
         */
        IntegerBase& divByExact(const IntegerBase& other);
        /**
         * Divides this by the given integer.
         * This can only be used when the given integer divides into
         * this exactly, and for large integers this is much faster than
         * ordinary division.  This integer is changed to reflect the result.
         *
         * \pre The given integer divides exactly into
         * this integer, i.e. \a this divided by \a other is an integer.
         * \pre \a other is not zero.
         * \pre This integer is not infinite.
         *
         * \param other the integer to divide this by.
         * @return a reference to this integer with its new value.
         */
        IntegerBase& divByExact(long other);
        /**
         * Reduces this integer modulo the given integer.
         * If non-zero, the result will have the same sign as the original
         * value of this integer.
         * This integer is changed to reflect the result.
         *
         * For a mod routine that always returns a non-negative
         * remainder, see divisionAlg().
         *
         * \pre \a other is not zero.
         * \pre Neither this nor \a other is infinite.
         *
         * \param other the integer modulo which this integer will be
         * reduced.
         * @return a reference to this integer with its new value.
         */
        IntegerBase& operator %=(const IntegerBase& other);
        /**
         * Reduces this integer modulo the given integer.
         * If non-zero, the result will have the same sign as the original
         * value of this integer.
         * This integer is changed to reflect the result.
         *
         * For a mod routine that always returns a non-negative
         * remainder, see divisionAlg().
         *
         * \pre \a other is not zero.
         * \pre This integer is not infinite.
         *
         * \param other the integer modulo which this integer will be
         * reduced.
         * @return a reference to this integer with its new value.
         */
        IntegerBase& operator %=(long other);
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
         * @return the absolute value of this integer.
         */
        IntegerBase abs() const;
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
         * @return the greatest common divisor of this and the given
         * integer.
         */
        IntegerBase gcd(const IntegerBase& other) const;
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
         * @return the lowest common multiple of this and the given
         * integer.
         */
        IntegerBase lcm(const IntegerBase& other) const;

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
         * - <tt>u*this + v*other = d</tt>;
         * - <tt>-abs(this)/d < v*sign(other) ≤ 0</tt>; and
         * - <tt>1 ≤ u*sign(this) ≤ abs(other)/d</tt>.
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
         * @return a tuple containing: the greatest common divisor of
         * \a this and \a other; the final coefficient of \a this; and
         * the final coefficient of \a other.
         */
        std::tuple<IntegerBase, IntegerBase, IntegerBase> gcdWithCoeffs(
            const IntegerBase<supportInfinity>& other) const;

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
         * - <tt>u*this + v*other = d</tt>;
         * - <tt>-abs(this)/d < v*sign(other) ≤ 0</tt>; and
         * - <tt>1 ≤ u*sign(this) ≤ abs(other)/d</tt>.
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
         * @return the greatest common divisor of \a this and \a other.
         */
        IntegerBase<supportInfinity> gcdWithCoeffs(
            const IntegerBase<supportInfinity>& other,
            IntegerBase<supportInfinity>& u,
            IntegerBase<supportInfinity>& v) const;

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
         * @return The Legendre symbol (0, 1 or -1) as described above.
         *
         * \author Ryan Budney
         */
        int legendre(const IntegerBase<supportInfinity>& p) const;

        /**
         * Generate a pseudo-random integer that is uniformly
         * distributed in the interval [0,*this).
         *
         * The random number generation here does \e not use Regina's
         * own RandomEngine class, but instead uses a separate random
         * number generator provided by GMP.
         *
         * \pre This integer is strictly positive.
         *
         * \warning Even if this integer is small, this routine is still
         * slow - it always goes through the GMP large integer routines
         * so that the random number generation algorithm is consistent.
         * If you need a fast random number generator and this integer
         * is small, consider using the standard rand() function instead.
         *
         * @return a pseudo-random integer.
         */
        IntegerBase<supportInfinity> randomBoundedByThis() const;

        /**
         * Generate a pseudo-random integer that is uniformly
         * distributed in the interval [0,2^n).
         *
         * The random number generation here does \e not use Regina's
         * own RandomEngine class, but instead uses a separate random
         * number generator provided by GMP.
         *
         * \param n the maximum number of bits in the pseudo-random
         * integer.
         * @return a pseudo-random integer.
         */
        static IntegerBase<supportInfinity> randomBinary(unsigned long n);

        /**
         * Generate a pseudo-random integer that is distributed in the
         * interval [0,2^n), with a tendency to have long strings of 0s
         * and 1s in its binary expansion.
         *
         * The random number generation here does \e not use Regina's
         * own RandomEngine class, but instead uses a separate random
         * number generator provided by GMP.
         *
         * \param n the maximum number of bits in the pseudo-random integer.
         * @return a pseudo-random integer.
         */
        static IntegerBase<supportInfinity> randomCornerBinary(
            unsigned long n);

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
         * @return the raw GMP data.
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
         * @return the raw GMP data.
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
         * \pre This integer is not infinite.
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
         * \pre This integer is not infinite.
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
         * @return the resulting encoded string.
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
         * supportInfinity is \c true.
         *
         * This routine is identical to calling the global template routine
         * regina::tightDecoding() with this type as the template argument.
         *
         * \exception InvalidArgument The given string is not a tight encoding
         * of an integer of this type.
         *
         * \param enc the tight encoding for an integer.
         * @return the integer represented by the given tight encoding.
         */
        static IntegerBase tightDecoding(const std::string& enc);

        /**
         * Reconstructs an integer from its given tight encoding.
         * See the page on \ref tight "tight encodings" for details.
         *
         * The tight encoding will be read from the given input stream.  If the
         * input stream contains leading whitespace then it will be treated as
         * an invalid encoding (i.e., this routine will throw an exception).
         * The input routine \e may contain further data: if this routine is
         * successful then the input stream will be left positioned immediately
         * after the encoding, without skipping any trailing whitespace.
         *
         * This routine does recognise infinity in the case where \a
         * supportInfinity is \c true.
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
         * @return the integer represented by the given tight encoding.
         */
        static IntegerBase tightDecode(std::istream& input);

    private:
        /**
         * Initialises this integer to infinity.
         * All parameters are ignored.
         *
         * This constructor is only defined if \a supportInfinity is \c true.
         * Any attempt to use it when \a supportInfinity is \c false
         * will generate a linker error.
         */
        IntegerBase(bool, bool);

        /**
         * Sets this integer to be finite.
         * Its new value will be determined by the current contents of
         * \a small_ which will not be touched.
         *
         * If the template parameter \a supportInfinity is \c false,
         * this routine safely does nothing.
         */
        inline void makeFinite();

        /**
         * Converts this integer from a native C/C++ long representation
         * into a GMP large integer representation.
         *
         * The contents of \a small will be copied into \a large.
         *
         * \pre \a large_ is null (i.e., we are indeed using a native
         * C/C++ long representation at present).
         * \pre This integer is not infinite.
         */
        void forceLarge();

        /**
         * Destroys the GMP large integer representation and reverts to
         * a native C/C++ long.
         *
         * The new value of this integer will be the current contents of
         * \a small_ (i.e., there is no attempt to "extract" a native long
         * from the contents of \a large_).
         *
         * \pre \a large_ is non-null (i.e., we are indeed using a large
         * integer reprentation at present).
         * \pre This integer is not infinite.
         */
        void clearLarge();

        /**
         * Converts this integer from a GMP large integer representation
         * into a native C/C++ long representation.
         *
         * The contents of \a large will be extracted and copied into \a small.
         *
         * \pre \a large_ is non-null, and the large integer that it
         * represents lies between LONG_MIN and LONG_MAX inclusive.
         * \pre This integer is not infinite.
         */
        void forceReduce();

    friend class IntegerBase<! supportInfinity>; // For conversions.

    template <int bytes>
    friend class NativeInteger; // For conversions.

    template <bool supportInfinity_>
    friend std::ostream& operator << (std::ostream& out,
        const IntegerBase<supportInfinity_>& large);
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
 * This global routine simply calls IntegerBase<supportInfinity>::swap();
 * it is provided so that IntegerBase<supportInfinity> meets the C++ Swappable
 * requirements.
 *
 * \param a the first integer whose contents should be swapped.
 * \param b the second integer whose contents should be swapped.
 *
 * \ingroup maths
 */
template <bool supportInfinity>
void swap(IntegerBase<supportInfinity>& a, IntegerBase<supportInfinity>& b)
    noexcept;

/**
 * Writes the given integer to the given output stream.
 *
 * \param out the output stream to which to write.
 * \param i the integer to write.
 * @return a reference to \a out.
 *
 * \ingroup maths
 */
template <bool supportInfinity>
std::ostream& operator << (std::ostream& out,
    const IntegerBase<supportInfinity>& i);

/**
 * Adds the given native integer to the given large integer.
 * If the large integer is infinite, the result will also be infinity.
 *
 * \param lhs the native integer to add.
 * \param rhs the large integer to add.
 * @return the sum \a lhs plus \a rhs.
 *
 * \ingroup maths
 */
template <bool supportInfinity>
IntegerBase<supportInfinity> operator + (long lhs,
    const IntegerBase<supportInfinity>& rhs);

/**
 * Multiplies the given native integer with the given large integer.
 * If the large integer is infinite, the result will also be infinity.
 *
 * \param lhs the native integer to multiply.
 * \param rhs the large integer to multiply.
 * @return the product \a lhs times \a rhs.
 *
 * \ingroup maths
 */
template <bool supportInfinity>
IntegerBase<supportInfinity> operator * (long lhs,
    const IntegerBase<supportInfinity>& rhs);

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
template <bool supportInfinity>
void tightEncode(std::ostream& out, IntegerBase<supportInfinity> value);

/**
 * Returns the tight encoding of the given arbitrary precision integer.
 * See the page on \ref tight "tight encodings" for details.
 *
 * This global function does the same thing as the member function
 * IntegerBase::tightEncoding().  However, this global function is more
 * efficient if the integer argument is an rvalue reference (since the const
 * member function induces an extra deep copy).
 *
 * \param value the integer to encode.
 * @return the resulting encoded string.
 *
 * \ingroup maths
 */
template <bool supportInfinity>
std::string tightEncoding(IntegerBase<supportInfinity> value);

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
 * \pre The system must support integers of the given size; in particular,
 * there must be an appropriate specialisation IntOfSize<bytes>.
 *
 * \nopython The purpose of NativeInteger is to be a highly optimised
 * drop-in replacement for Integer as a C++ template parameter.
 * Python users should just use regina.Integer if you need Regina's integer
 * interface, or Python's own integer type if you do not.
 *
 * \ingroup maths
 */
template <int bytes>
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
        NativeInteger();
        /**
         * Initialises this integer to the given value.
         *
         * \param value the new value of this integer.
         */
        NativeInteger(Native value);
        /**
         * Initialises this integer to the given value.
         *
         * \param value the new value of this integer.
         */
        NativeInteger(const NativeInteger<bytes>& value);
        /**
         * Initialises this integer to the given value.
         *
         * This constructor is marked as explicit in the hope of
         * avoiding accidental (and unintentional) mixing of integer classes.
         *
         * It is the programmer's reponsibility to ensure that the given value
         * fits within the required range.  If the given value is too large or
         * small to fit into this native type, then this new NativeInteger
         * will have an undefined initial value.
         *
         * \pre If \a bytes is larger than sizeof(long), then
         * \a bytes is a strict \e multiple of sizeof(long).  For
         * instance, if longs are 8 bytes then you can use this
         * routine with \a bytes=16 but not \a bytes=12.
         * This restriction is enforced through a compile-time assertion,
         * but may be lifted in future versions of Regina.
         *
         * \pre The given integer is not infinity.
         *
         * \param value the new value of this integer.
         */
        template <bool supportInfinity>
        explicit NativeInteger(const IntegerBase<supportInfinity>& value);

        /**
         * Returns whether or not this integer is zero.
         *
         * @return \c true if and only if this integer is zero.
         */
        bool isZero() const;

        /**
         * Returns the sign of this integer.
         *
         * @return +1, -1 or 0 according to whether this integer is
         * positive, negative or zero.
         */
        int sign() const;
        /**
         * Returns the value of this integer in its native type.
         *
         * @return the value of this integer.
         */
        Native nativeValue() const;

        /**
         * Sets this integer to the given value.
         *
         * \param value the new value of this integer.
         * @return a reference to this integer with its new value.
         */
        NativeInteger& operator =(const NativeInteger& value);
        /**
         * Sets this integer to the given value.
         *
         * \param value the new value of this integer.
         * @return a reference to this integer with its new value.
         */
        NativeInteger& operator =(Native value);
        /**
         * Swaps the values of this and the given integer.
         *
         * \param other the integer whose value will be swapped with this.
         */
        void swap(NativeInteger& other) noexcept;

        /**
         * Determines if this is equal to the given integer.
         *
         * \param rhs the integer with which this will be compared.
         * @return \c true if and only if this and the given integer are
         * equal.
         */
        bool operator ==(const NativeInteger& rhs) const;
        /**
         * Determines if this is equal to the given integer.
         *
         * \param rhs the integer with which this will be compared.
         * @return \c true if and only if this and the given integer are
         * equal.
         */
        bool operator ==(Native rhs) const;
        /**
         * Determines if this is not equal to the given integer.
         *
         * \param rhs the integer with which this will be compared.
         * @return \c true if and only if this and the given integer are
         * not equal.
         */
        bool operator !=(const NativeInteger& rhs) const;
        /**
         * Determines if this is not equal to the given integer.
         *
         * \param rhs the integer with which this will be compared.
         * @return \c true if and only if this and the given integer are
         * not equal.
         */
        bool operator !=(Native rhs) const;
        /**
         * Determines if this is less than the given integer.
         *
         * \param rhs the integer with which this will be compared.
         * @return \c true if and only if this is less than the given
         * integer.
         */
        bool operator <(const NativeInteger& rhs) const;
        /**
         * Determines if this is less than the given integer.
         *
         * \param rhs the integer with which this will be compared.
         * @return \c true if and only if this is less than the given
         * integer.
         */
        bool operator <(Native rhs) const;
        /**
         * Determines if this is greater than the given integer.
         *
         * \param rhs the integer with which this will be compared.
         * @return \c true if and only if this is greater than the given
         * integer.
         */
        bool operator >(const NativeInteger& rhs) const;
        /**
         * Determines if this is greater than the given integer.
         *
         * \param rhs the integer with which this will be compared.
         * @return \c true if and only if this is greater than the given
         * integer.
         */
        bool operator >(Native rhs) const;
        /**
         * Determines if this is less than or equal to the given integer.
         *
         * \param rhs the integer with which this will be compared.
         * @return \c true if and only if this is less than or equal to
         * the given integer.
         */
        bool operator <=(const NativeInteger& rhs) const;
        /**
         * Determines if this is less than or equal to the given integer.
         *
         * \param rhs the integer with which this will be compared.
         * @return \c true if and only if this is less than or equal to
         * the given integer.
         */
        bool operator <=(Native rhs) const;
        /**
         * Determines if this is greater than or equal to the given integer.
         *
         * \param rhs the integer with which this will be compared.
         * @return \c true if and only if this is greater than or equal
         * to the given integer.
         */
        bool operator >=(const NativeInteger& rhs) const;
        /**
         * Determines if this is greater than or equal to the given integer.
         *
         * \param rhs the integer with which this will be compared.
         * @return \c true if and only if this is greater than or equal
         * to the given integer.
         */
        bool operator >=(Native rhs) const;

        /**
         * The preincrement operator.
         * This operator increments this integer by one, and returns a
         * reference to the integer \e after the increment.
         *
         * @return a reference to this integer after the increment.
         */
        NativeInteger& operator ++();

        /**
         * The postincrement operator.
         * This operator increments this integer by one, and returns a
         * copy of the integer \e before the increment.
         *
         * @return a copy of this integer before the
         * increment took place.
         */
        NativeInteger operator ++(int);

        /**
         * The predecrement operator.
         * This operator decrements this integer by one, and returns a
         * reference to the integer \e after the decrement.
         *
         * @return a reference to this integer after the decrement.
         */
        NativeInteger& operator --();

        /**
         * The postdecrement operator.
         * This operator decrements this integer by one, and returns a
         * copy of the integer \e before the decrement.
         *
         * @return a copy of this integer before the
         * decrement took place.
         */
        NativeInteger operator --(int);

        /**
         * Adds this to the given integer and returns the result.
         * This integer is not changed.
         *
         * \param other the integer to add to this integer.
         * @return the sum \a this plus \a other.
         */
        NativeInteger operator +(const NativeInteger& other) const;
        /**
         * Adds this to the given integer and returns the result.
         * This integer is not changed.
         *
         * \param other the integer to add to this integer.
         * @return the sum \a this plus \a other.
         */
        NativeInteger operator +(Native other) const;
        /**
         * Subtracts the given integer from this and returns the result.
         * This integer is not changed.
         *
         * \param other the integer to subtract from this integer.
         * @return the difference \a this minus \a other.
         */
        NativeInteger operator -(const NativeInteger& other) const;
        /**
         * Subtracts the given integer from this and returns the result.
         * This integer is not changed.
         *
         * \param other the integer to subtract from this integer.
         * @return the difference \a this minus \a other.
         */
        NativeInteger operator -(Native other) const;
        /**
         * Multiplies this by the given integer and returns the
         * result.
         * This integer is not changed.
         *
         * \param other the integer to multiply by this integer.
         * @return the product \a this times \a other.
         */
        NativeInteger operator *(const NativeInteger& other) const;
        /**
         * Multiplies this by the given integer and returns the
         * result.
         * This integer is not changed.
         *
         * \param other the integer to multiply by this integer.
         * @return the product \a this times \a other.
         */
        NativeInteger operator *(Native other) const;
        /**
         * Divides this by the given integer and returns the result.
         * The result will be truncated to an integer, i.e. rounded
         * towards zero.
         * This integer is not changed.
         *
         * For a division routine that always rounds down, see divisionAlg().
         *
         * \pre \a other must be non-zero.
         *
         * \param other the integer to divide this by.
         * @return the quotient \a this divided by \a other.
         */
        NativeInteger operator /(const NativeInteger& other) const;
        /**
         * Divides this by the given integer and returns the result.
         * The result will be truncated to an integer, i.e. rounded
         * towards zero.
         * This integer is not changed.
         *
         * For a division routine that always rounds down, see divisionAlg().
         *
         * \pre \a other must be non-zero.
         *
         * \param other the integer to divide this by.
         * @return the quotient \a this divided by \a other.
         */
        NativeInteger operator /(Native other) const;
        /**
         * Divides this by the given integer and returns the result.
         * For native integers, this is identical to operator /.
         *
         * \pre \a other is not zero.
         *
         * \param other the integer to divide this by.
         * @return the quotient \a this divided by \a other.
         */
        NativeInteger divExact(const NativeInteger& other) const;
        /**
         * Divides this by the given integer and returns the result.
         * For native integers, this is identical to operator /.
         *
         * \pre \a other is not zero.
         *
         * \param other the integer to divide this by.
         * @return the quotient \a this divided by \a other.
         */
        NativeInteger divExact(Native other) const;
        /**
         * Determines the remainder when this integer is divided by the
         * given integer.  If non-zero, the result will have the same sign
         * as this integer.
         * This integer is not changed.
         *
         * For a division routine that always returns a non-negative
         * remainder, see divisionAlg().
         *
         * \pre \a other is not zero.
         *
         * \param other the integer to divide this by.
         * @return the remainder \a this modulo \a other.
         */
        NativeInteger operator %(const NativeInteger& other) const;
        /**
         * Determines the remainder when this integer is divided by the
         * given integer.  If non-zero, the result will have the same sign
         * as this integer.
         * This integer is not changed.
         *
         * For a division routine that always returns a non-negative
         * remainder, see divisionAlg().
         *
         * \pre \a other is not zero.
         *
         * \param other the integer to divide this by.
         * @return the remainder \a this modulo \a other.
         */
        NativeInteger operator %(Native other) const;

        /**
         * Uses the division algorithm to obtain a quotient and
         * remainder when dividing by the given integer.
         *
         * Suppose this integer is \a n and we pass the divisor \a d.
         * The <em>division algorithm</em> describes the result of
         * dividing \a n by \a d; in particular, it expresses
         * <tt>n = qd + r</tt>, where \a q is the quotient and
         * \a r is the remainder.
         *
         * The division algorithm is precise about which values of \a q
         * and \a r are chosen; in particular it chooses the unique \a r
         * in the range <tt>0 ≤ r < |d|</tt>.
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
         * @return the pair (\a q, \a r), where \a q is the quotient and
         * \a r is the remainder, as described above.
         */
        std::pair<NativeInteger, NativeInteger> divisionAlg(
            const NativeInteger& divisor) const;

        /**
         * Determines the negative of this integer.
         * This integer is not changed.
         *
         * @return the negative of this integer.
         */
        NativeInteger operator -() const;

        /**
         * Adds the given integer to this.
         * This integer is changed to reflect the result.
         *
         * \param other the integer to add to this integer.
         * @return a reference to this integer with its new value.
         */
        NativeInteger& operator +=(const NativeInteger& other);
        /**
         * Adds the given integer to this.
         * This integer is changed to reflect the result.
         *
         * \param other the integer to add to this integer.
         * @return a reference to this integer with its new value.
         */
        NativeInteger& operator +=(Native other);
        /**
         * Subtracts the given integer from this.
         * This integer is changed to reflect the result.
         *
         * \param other the integer to subtract from this integer.
         * @return a reference to this integer with its new value.
         */
        NativeInteger& operator -=(const NativeInteger& other);
        /**
         * Subtracts the given integer from this.
         * This integer is changed to reflect the result.
         *
         * \param other the integer to subtract from this integer.
         * @return a reference to this integer with its new value.
         */
        NativeInteger& operator -=(Native other);
        /**
         * Multiplies the given integer by this.
         * This integer is changed to reflect the result.
         *
         * \param other the integer to multiply with this integer.
         * @return a reference to this integer with its new value.
         */
        NativeInteger& operator *=(const NativeInteger& other);
        /**
         * Multiplies the given integer by this.
         * This integer is changed to reflect the result.
         *
         * \param other the integer to multiply with this integer.
         * @return a reference to this integer with its new value.
         */
        NativeInteger& operator *=(Native other);
        /**
         * Divides this by the given integer.
         * The result will be truncated to an integer, i.e. rounded
         * towards zero.
         * This integer is changed to reflect the result.
         *
         * For a division routine that always rounds down, see divisionAlg().
         *
         * \pre \a other must be non-zero.
         *
         * \param other the integer to divide this by.
         * @return a reference to this integer with its new value.
         */
        NativeInteger& operator /=(const NativeInteger& other);
        /**
         * Divides this by the given integer.
         * The result will be truncated to an integer, i.e. rounded
         * towards zero.
         * This integer is changed to reflect the result.
         *
         * For a division routine that always rounds down, see divisionAlg().
         *
         * \pre \a other must be non-zero.
         *
         * \param other the integer to divide this by.
         * @return a reference to this integer with its new value.
         */
        NativeInteger& operator /=(Native other);
        /**
         * Divides this by the given integer.
         * For native integers, this routine is identical to operator /=.
         *
         * \pre \a other is not zero.
         *
         * \param other the integer to divide this by.
         * @return a reference to this integer with its new value.
         */
        NativeInteger& divByExact(const NativeInteger& other);
        /**
         * Divides this by the given integer.
         * For native integers, this routine is identical to operator /=.
         *
         * \pre \a other is not zero.
         *
         * \param other the integer to divide this by.
         * @return a reference to this integer with its new value.
         */
        NativeInteger& divByExact(Native other);
        /**
         * Reduces this integer modulo the given integer.
         * If non-zero, the result will have the same sign as the original
         * value of this integer.
         * This integer is changed to reflect the result.
         *
         * For a mod routine that always returns a non-negative
         * remainder, see divisionAlg().
         *
         * \pre \a other is not zero.
         *
         * \param other the integer modulo which this integer will be
         * reduced.
         * @return a reference to this integer with its new value.
         */
        NativeInteger& operator %=(const NativeInteger& other);
        /**
         * Reduces this integer modulo the given integer.
         * If non-zero, the result will have the same sign as the original
         * value of this integer.
         * This integer is changed to reflect the result.
         *
         * For a mod routine that always returns a non-negative
         * remainder, see divisionAlg().
         *
         * \pre \a other is not zero.
         *
         * \param other the integer modulo which this integer will be
         * reduced.
         * @return a reference to this integer with its new value.
         */
        NativeInteger& operator %=(Native other);
        /**
         * Negates this integer.
         * This integer is changed to reflect the result.
         */
        void negate();
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
        void gcdWith(const NativeInteger& other);
        /**
         * Determines the greatest common divisor of this and the given
         * integer.  This integer is not changed.
         *
         * The result is guaranteed to be non-negative.  As a
         * special case, gcd(0,0) is considered to be zero.
         *
         * \param other the integer whose greatest common divisor with
         * this will be found.
         * @return the greatest common divisor of this and the given
         * integer.
         */
        NativeInteger gcd(const NativeInteger& other) const;

        /**
         * Returns whether this integer is infinity.
         *
         * Since NativeInteger cannot represent infinity, this routine will
         * always return \c false.  This routine is simply provided for
         * compatibility with LargeInteger (where infinity is allowed).
         *
         * @return \c false, since a NativeInteger can never be infinity.
         */
        bool isInfinite() const;
        /**
         * A do-nothing routine that ensures that this integer is using a
         * native C/C++ integer representation.
         *
         * Since the NativeInteger class always uses a native representation,
         * this routine does nothing at all.  This routine is simply provided
         * for compatibility with Regina's arbitrary-precision Integer and
         * LargeInteger classes.
         */
        void tryReduce();

    template <int bytes_>
    friend std::ostream& operator << (std::ostream& out,
        const NativeInteger<bytes_>& large);
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
void swap(NativeInteger<bytes>& a, NativeInteger<bytes>& b) noexcept;

/**
 * Writes the given integer to the given output stream.
 *
 * \param out the output stream to which to write.
 * \param i the integer to write.
 * @return a reference to \a out.
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

// Inline functions for IntegerBase

template <bool supportInfinity>
inline const IntegerBase<supportInfinity> IntegerBase<supportInfinity>::zero;

template <bool supportInfinity>
inline const IntegerBase<supportInfinity> IntegerBase<supportInfinity>::one = 1;

// We define infinity later, after the specialised infinity constructor.

template <bool supportInfinity>
inline IntegerBase<supportInfinity>::IntegerBase() :
        small_(0), large_(nullptr) {
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>::IntegerBase(int value) :
        small_(value), large_(nullptr) {
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>::IntegerBase(unsigned value) :
        small_(value) {
    // Detect overflow.
    if (small_ < 0) {
        large_ = new __mpz_struct[1];
        mpz_init_set_ui(large_, value);
    } else
        large_ = nullptr;
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>::IntegerBase(long value) :
        small_(value), large_(nullptr) {
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>::IntegerBase(unsigned long value) :
        small_(value) {
    // Detect overflow.
    if (small_ < 0) {
        large_ = new __mpz_struct[1];
        mpz_init_set_ui(large_, value);
    } else
        large_ = nullptr;
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>::IntegerBase(long long value) :
        small_(static_cast<long>(value)), large_(nullptr) {
    // Detect overflow.
    if constexpr (sizeof(long) < sizeof(long long))
        if (small_ != value)
            large_ = regina::detail::mpz_from_ll(value);
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>::IntegerBase(unsigned long long value) :
        small_(static_cast<long>(value)), large_(nullptr) {
    // Detect overflow.
    // This could occur even if long and long long have the same size,
    // due to the discrepancy between signed and unsigned ranges.
    if (small_ < 0 || static_cast<unsigned long long>(small_) != value)
        large_ = regina::detail::mpz_from_ull(value);
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>::IntegerBase(
        const IntegerBase<supportInfinity>& value) {
    if (value.isInfinite()) {
        large_ = nullptr;
        makeInfinite();
    } else if (value.large_) {
        large_ = new __mpz_struct[1];
        mpz_init_set(large_, value.large_);
    } else {
        small_ = value.small_;
        large_ = nullptr;
    }
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>::IntegerBase(
        const IntegerBase<! supportInfinity>& value) {
    // If value is infinite, we cannot make this infinite.
    // This is why we insist via preconditions that value is finite.
    if (value.large_) {
        large_ = new __mpz_struct[1];
        mpz_init_set(large_, value.large_);
    } else {
        small_ = value.small_;
        large_ = nullptr;
    }
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>::IntegerBase(
        IntegerBase<supportInfinity>&& src) noexcept :
        InfinityBase<supportInfinity>(src),
        small_(src.small_), large_(src.large_) {
    src.large_ = nullptr;
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>::IntegerBase(
        IntegerBase<! supportInfinity>&& src) noexcept :
        small_(src.small_), large_(src.large_) {
    // The default InfinityBase constructor makes the integer finite.
    src.large_ = nullptr;
}

template <bool supportInfinity>
template <int bytes>
inline IntegerBase<supportInfinity>::IntegerBase(
        const NativeInteger<bytes>& value) :
        // This cast may lose information, but we will fix this in a moment.
        small_(static_cast<long>(value.nativeValue())), large_(nullptr) {
    static_assert(bytes % sizeof(long) == 0,
        "IntegerBase native constructor: native integer must partition exactly into long integers.");
    if (sizeof(long) < bytes && value.nativeValue() != static_cast<typename IntOfSize<bytes>::type>(small_)) {
        // It didn't fit.  Take things one long at a time.
        unsigned blocks = bytes / sizeof(long);
        large_ = new __mpz_struct[1];
        mpz_init_set_si(large_, static_cast<long>(
            value.nativeValue() >> ((blocks - 1) * 8 * sizeof(long))));
        for (unsigned i = 2; i <= blocks; ++i) {
            mpz_mul_2exp(large_, large_, 8 * sizeof(long));
            mpz_add_ui(large_, large_, static_cast<unsigned long>(
                value.nativeValue() >> ((blocks - i) * 8 * sizeof(long))));
        }
    }
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>::IntegerBase(double value) :
        large_(nullptr) {
    // We start with a large representation, since we want to use GMP's
    // double-to-integer conversion.
    large_ = new __mpz_struct[1];
    mpz_init_set_d(large_, value);

    // Now switch to a small representation if we can.
    tryReduce();
}

template <bool supportInfinity>
template <int bytes>
typename IntOfSize<bytes>::type
        IntegerBase<supportInfinity>::nativeValue() const {
    using Native = typename IntOfSize<bytes>::type;
    using UNative = typename IntOfSize<bytes>::utype;

    if constexpr (bytes <= sizeof(long)) {
        // Since the requested type can fit inside a long, we can just
        // extract the long value and cast down.
        return static_cast<Native>(longValue());
    } else {
        // The requested type is too big to fit inside a long.
        if (! large_) {
            // We are holding a native long, and so this is already
            // safe to cast directly to the (larger) requested native type.
            return longValue();
        }

        // This is a GMP integer, and the requested native type is too
        // large for a long.  We will need to piece together the result
        // one long-sized chunk at a time.
        static_assert(bytes % sizeof(long) == 0,
            "IntegerBase::nativeValue(): native integer must partition "
            "exactly into long integers.");

        // We treat positive and negative numbers differently,
        // because mpz_get_ui() *ignores* sign.  Gulp.
        int sign = mpz_sgn(large_);
        if (sign == 0)
            return 0;

        Native ans = 0;
        unsigned blocks = bytes / sizeof(long);

        mpz_t tmp;
        mpz_init_set(tmp, large_);

        if (sign > 0) {
            // The positive case.
            for (unsigned i = 0; i < blocks - 1; ++i) {
                ans += (static_cast<UNative>(mpz_get_ui(tmp))
                    << (i * 8 * sizeof(long)));
                mpz_fdiv_q_2exp(tmp, tmp, 8 * sizeof(long));
            }
            ans += (static_cast<Native>(mpz_get_ui(tmp))
                << ((blocks - 1) * 8 * sizeof(long)));
        } else {
            // The negative case.
            for (unsigned i = 0; i < blocks - 1; ++i) {
                ans -= (static_cast<UNative>(mpz_get_ui(tmp))
                    << (i * 8 * sizeof(long)));
                mpz_tdiv_q_2exp(tmp, tmp, 8 * sizeof(long));
            }
            ans += (static_cast<Native>(mpz_get_si(tmp))
                << ((blocks - 1) * 8 * sizeof(long)));
        }

        mpz_clear(tmp);
        return ans;
    }
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>::IntegerBase(
        const std::string& value, int base) :
        IntegerBase(value.c_str(), base) {
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>::~IntegerBase() {
    if (large_) {
        mpz_clear(large_);
        delete[] large_;
    }
}

template <bool supportInfinity>
inline bool IntegerBase<supportInfinity>::isNative() const {
    return (! isInfinite()) && (! large_);
}

template <bool supportInfinity>
inline bool IntegerBase<supportInfinity>::isZero() const {
    return (! isInfinite()) &&
        (((! large_) && (! small_)) || (large_ && mpz_sgn(large_) == 0));
}

template <bool supportInfinity>
inline int IntegerBase<supportInfinity>::sign() const {
    return (isInfinite() ? 1 :
        large_ ? mpz_sgn(large_) :
        small_ > 0 ? 1 : small_ < 0 ? -1 : 0);
}

#ifndef __DOXYGEN // Doxygen gets confused by the specialisations.

template <>
inline bool IntegerBase<true>::isInfinite() const {
    return infinite_;
}

template <>
inline bool IntegerBase<false>::isInfinite() const {
    return false;
}

template <>
inline void IntegerBase<true>::makeInfinite() {
    infinite_ = true;
    if (large_)
        clearLarge();
}

template <>
inline void IntegerBase<false>::makeInfinite() {
}

#endif // __DOXYGEN

template <bool supportInfinity>
inline long IntegerBase<supportInfinity>::longValue() const {
    return (large_ ? mpz_get_si(large_) : small_);
}

template <bool supportInfinity>
inline long IntegerBase<supportInfinity>::safeLongValue() const {
    if constexpr (supportInfinity)
        if (isInfinite())
            throw NoSolution();

    if (large_) {
        if (mpz_cmp_si(large_, LONG_MAX) <= 0 &&
                mpz_cmp_si(large_, LONG_MIN) >= 0)
            return mpz_get_si(large_);
        else
            throw NoSolution();
    } else
        return small_;
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>&
        IntegerBase<supportInfinity>::operator =(
        const IntegerBase<supportInfinity>& value) {
    // We assume that mpz_set() is fine with self-assignment, since:
    // - the GMP docs state that output and input variables can be the same;
    // - the libgmpxx classes do not special-case self-assignment.
    // The C++ test suite tests self-assignment of Integers also.
    if (value.isInfinite()) {
        makeInfinite();
        return *this;
    }
    makeFinite();
    if (value.large_) {
        if (large_)
            mpz_set(large_, value.large_);
        else {
            large_ = new __mpz_struct[1];
            mpz_init_set(large_, value.large_);
        }
    } else {
        small_ = value.small_;
        if (large_)
            clearLarge();
    }
    return *this;
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>&
        IntegerBase<supportInfinity>::operator =(
        const IntegerBase<! supportInfinity>& value) {
    makeFinite(); // Either this or src does not support infinity.
    if (value.large_) {
        if (large_)
            mpz_set(large_, value.large_);
        else {
            large_ = new __mpz_struct[1];
            mpz_init_set(large_, value.large_);
        }
    } else {
        small_ = value.small_;
        if (large_)
            clearLarge();
    }
    return *this;
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>&
        IntegerBase<supportInfinity>::operator =(
        IntegerBase<supportInfinity>&& src) noexcept {
    if constexpr (supportInfinity)
        InfinityBase<true>::infinite_ = src.infinite_;
    small_ = src.small_;
    std::swap(large_, src.large_);
    // Let src dispose of the original large_, if it was non-null.
    return *this;
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>&
        IntegerBase<supportInfinity>::operator =(
        IntegerBase<! supportInfinity>&& src) noexcept {
    makeFinite(); // Either this or src does not support infinity.
    small_ = src.small_;
    std::swap(large_, src.large_);
    // Let src dispose of the original large_, if it was non-null.
    return *this;
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>&
        IntegerBase<supportInfinity>::operator =(int value) {
    makeFinite();
    small_ = value;
    if (large_)
        clearLarge();
    return *this;
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>&
        IntegerBase<supportInfinity>::operator =(unsigned value) {
    makeFinite();
    small_ = value;

    // Did we overflow?
    if (small_ < 0) {
        // Yes, it's an overflow: just a bit too large for a signed long
        // (literally).
        if (large_)
            mpz_set_ui(large_, value);
        else {
            large_ = new __mpz_struct[1];
            mpz_init_set_ui(large_, value);
        }
    } else if (large_) {
        // No overflow, but we must clear out any old large integer value.
        clearLarge();
    }
    return *this;
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>&
        IntegerBase<supportInfinity>::operator =(long value) {
    makeFinite();
    small_ = value;
    if (large_)
        clearLarge();
    return *this;
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>&
        IntegerBase<supportInfinity>::operator =(unsigned long value) {
    makeFinite();
    small_ = value;

    // Did we overflow?
    if (small_ < 0) {
        // Yes, it's an overflow: just a bit too large for a signed long
        // (literally).
        if (large_)
            mpz_set_ui(large_, value);
        else {
            large_ = new __mpz_struct[1];
            mpz_init_set_ui(large_, value);
        }
    } else if (large_) {
        // No overflow, but we must clear out any old large integer value.
        clearLarge();
    }
    return *this;
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>&
        IntegerBase<supportInfinity>::operator =(long long value) {
    makeFinite();
    if (large_)
        clearLarge();

    small_ = value;

    // Detect overflow.
    if constexpr (sizeof(long) < sizeof(long long))
        if (small_ != value)
            large_ = regina::detail::mpz_from_ll(value);

    return *this;
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>&
        IntegerBase<supportInfinity>::operator =(unsigned long long value) {
    makeFinite();
    if (large_)
        clearLarge();

    small_ = value;

    // Detect overflow.
    // This could occur even if long and long long have the same size,
    // due to the discrepancy between signed and unsigned ranges.
    if (small_ < 0 || static_cast<unsigned long long>(small_) != value)
        large_ = regina::detail::mpz_from_ull(value);

    return *this;
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>&
        IntegerBase<supportInfinity>::operator =(const std::string& value) {
    // NOLINTNEXTLINE(misc-unconventional-assign-operator)
    return (*this) = value.c_str();
}

template <bool supportInfinity>
inline void IntegerBase<supportInfinity>::swap(
        IntegerBase<supportInfinity>& other) noexcept {
    // This should just work, since large_ is a pointer.
    if constexpr (supportInfinity)
        std::swap(InfinityBase<true>::infinite_,
            other.InfinityBase<true>::infinite_);
    std::swap(small_, other.small_);
    std::swap(large_, other.large_);
}

template <bool supportInfinity>
inline bool IntegerBase<supportInfinity>::operator ==(
        const IntegerBase<supportInfinity>& rhs) const {
    if (isInfinite() && rhs.isInfinite())
        return true;
    else if (isInfinite() || rhs.isInfinite())
        return false;
    else if (large_) {
        if (rhs.large_)
            return (mpz_cmp(large_, rhs.large_) == 0);
        else
            return (mpz_cmp_si_cpp(large_, rhs.small_) == 0);
    } else {
        if (rhs.large_)
            return (mpz_cmp_si_cpp(rhs.large_, small_) == 0);
        else
            return (small_ == rhs.small_);
    }
}

template <bool supportInfinity>
inline bool IntegerBase<supportInfinity>::operator ==(
        const IntegerBase<! supportInfinity>& rhs) const {
    // The types are different, so both cannot be infinity.
    if (isInfinite() || rhs.isInfinite())
        return false;
    else if (large_) {
        if (rhs.large_)
            return (mpz_cmp(large_, rhs.large_) == 0);
        else
            return (mpz_cmp_si_cpp(large_, rhs.small_) == 0);
    } else {
        if (rhs.large_)
            return (mpz_cmp_si_cpp(rhs.large_, small_) == 0);
        else
            return (small_ == rhs.small_);
    }
}

template <bool supportInfinity>
inline bool IntegerBase<supportInfinity>::operator ==(long rhs) const {
    if (isInfinite())
        return false;
    else if (large_)
        return (mpz_cmp_si_cpp(large_, rhs) == 0);
    else
        return (small_ == rhs);
}

template <bool supportInfinity>
inline bool IntegerBase<supportInfinity>::operator !=(
        const IntegerBase<supportInfinity>& rhs) const {
    if (isInfinite() && rhs.isInfinite())
        return false;
    else if (isInfinite() || rhs.isInfinite())
        return true;
    else if (large_) {
        if (rhs.large_)
            return (mpz_cmp(large_, rhs.large_) != 0);
        else
            return (mpz_cmp_si_cpp(large_, rhs.small_) != 0);
    } else {
        if (rhs.large_)
            return (mpz_cmp_si_cpp(rhs.large_, small_) != 0);
        else
            return (small_ != rhs.small_);
    }
}

template <bool supportInfinity>
inline bool IntegerBase<supportInfinity>::operator !=(
        const IntegerBase<! supportInfinity>& rhs) const {
    // The types are different, so both cannot be infinity.
    if (isInfinite() || rhs.isInfinite())
        return true;
    else if (large_) {
        if (rhs.large_)
            return (mpz_cmp(large_, rhs.large_) != 0);
        else
            return (mpz_cmp_si_cpp(large_, rhs.small_) != 0);
    } else {
        if (rhs.large_)
            return (mpz_cmp_si_cpp(rhs.large_, small_) != 0);
        else
            return (small_ != rhs.small_);
    }
}

template <bool supportInfinity>
inline bool IntegerBase<supportInfinity>::operator !=(long rhs) const {
    if (isInfinite())
        return true;
    else if (large_)
        return (mpz_cmp_si_cpp(large_, rhs) != 0);
    else
        return (small_ != rhs);
}

template <bool supportInfinity>
inline bool IntegerBase<supportInfinity>::operator <(
        const IntegerBase<supportInfinity>& rhs) const {
    if (isInfinite())
        return false;
    else if (rhs.isInfinite())
        return true;
    else if (large_) {
        if (rhs.large_)
            return (mpz_cmp(large_, rhs.large_) < 0);
        else
            return (mpz_cmp_si_cpp(large_, rhs.small_) < 0);
    } else {
        if (rhs.large_)
            return (mpz_cmp_si_cpp(rhs.large_, small_) > 0); // back-to-front
        else
            return (small_ < rhs.small_);
    }
}

template <bool supportInfinity>
inline bool IntegerBase<supportInfinity>::operator <(long rhs) const {
    if (isInfinite())
        return false;
    else if (large_)
        return (mpz_cmp_si_cpp(large_, rhs) < 0);
    else
        return (small_ < rhs);
}

template <bool supportInfinity>
inline bool IntegerBase<supportInfinity>::operator >(
        const IntegerBase<supportInfinity>& rhs) const {
    if (rhs.isInfinite())
        return false;
    else if (isInfinite())
        return true;
    else if (large_) {
        if (rhs.large_)
            return (mpz_cmp(large_, rhs.large_) > 0);
        else
            return (mpz_cmp_si_cpp(large_, rhs.small_) > 0);
    } else {
        if (rhs.large_)
            return (mpz_cmp_si_cpp(rhs.large_, small_) < 0); // back-to-front
        else
            return (small_ > rhs.small_);
    }
}

template <bool supportInfinity>
inline bool IntegerBase<supportInfinity>::operator >(long rhs) const {
    if (isInfinite())
        return true;
    else if (large_)
        return (mpz_cmp_si_cpp(large_, rhs) > 0);
    else
        return (small_ > rhs);
}

template <bool supportInfinity>
inline bool IntegerBase<supportInfinity>::operator <=(
        const IntegerBase<supportInfinity>& rhs) const {
    if (rhs.isInfinite())
        return true;
    else if (isInfinite())
        return false;
    else if (large_) {
        if (rhs.large_)
            return (mpz_cmp(large_, rhs.large_) <= 0);
        else
            return (mpz_cmp_si_cpp(large_, rhs.small_) <= 0);
    } else {
        if (rhs.large_)
            return (mpz_cmp_si_cpp(rhs.large_, small_) >= 0); // back-to-front
        else
            return (small_ <= rhs.small_);
    }
}

template <bool supportInfinity>
inline bool IntegerBase<supportInfinity>::operator <=(long rhs) const {
    if (isInfinite())
        return false;
    else if (large_)
        return (mpz_cmp_si_cpp(large_, rhs) <= 0);
    else
        return (small_ <= rhs);
}

template <bool supportInfinity>
inline bool IntegerBase<supportInfinity>::operator >=(
        const IntegerBase<supportInfinity>& rhs) const {
    if (isInfinite())
        return true;
    else if (rhs.isInfinite())
        return false;
    else if (large_) {
        if (rhs.large_)
            return (mpz_cmp(large_, rhs.large_) >= 0);
        else
            return (mpz_cmp_si_cpp(large_, rhs.small_) >= 0);
    } else {
        if (rhs.large_)
            return (mpz_cmp_si_cpp(rhs.large_, small_) <= 0); // back-to-front
        else
            return (small_ >= rhs.small_);
    }
}

template <bool supportInfinity>
inline bool IntegerBase<supportInfinity>::operator >=(long rhs) const {
    if (isInfinite())
        return true;
    else if (large_)
        return (mpz_cmp_si_cpp(large_, rhs) >= 0);
    else
        return (small_ >= rhs);
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>&
        IntegerBase<supportInfinity>::operator ++() {
    if (! isInfinite()) {
        if (large_)
            mpz_add_ui(large_, large_, 1);
        else if (small_ != LONG_MAX)
            ++small_;
        else {
            // This is the point at which we overflow.
            forceLarge();
            mpz_add_ui(large_, large_, 1);
        }
    }
    return *this;
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>
        IntegerBase<supportInfinity>::operator ++(int) {
    if (isInfinite())
        return *this;

    // Hrmph, just do the standard thing for now.
    // It's not clear how much microoptimisation will help..?
    IntegerBase<supportInfinity> ans(*this);
    ++(*this);
    return ans;
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>&
        IntegerBase<supportInfinity>::operator --() {
    if (! isInfinite()) {
        if (large_)
            mpz_sub_ui(large_, large_, 1);
        else if (small_ != LONG_MIN)
            --small_;
        else {
            // This is the point at which we overflow.
            forceLarge();
            mpz_sub_ui(large_, large_, 1);
        }
    }
    return *this;
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>
        IntegerBase<supportInfinity>::operator --(int) {
    if (isInfinite())
        return *this;

    // Hrmph, just do the standard thing for now.
    // It's not clear how much microoptimisation will help..?
    IntegerBase<supportInfinity> ans(*this);
    --(*this);
    return ans;
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>
        IntegerBase<supportInfinity>::operator +(
        const IntegerBase<supportInfinity>& other) const {
    if (isInfinite())
        return *this;
    if (other.isInfinite())
        return other;

    // Do the standard thing for now.
    IntegerBase<supportInfinity> ans(*this);
    return ans += other;
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>
        IntegerBase<supportInfinity>::operator +(long other) const {
    if (isInfinite())
        return *this;

    // Do the standard thing for now.
    IntegerBase<supportInfinity> ans(*this);
    return ans += other;
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>
        IntegerBase<supportInfinity>::operator -(
        const IntegerBase<supportInfinity>& other) const {
    if (isInfinite())
        return *this;
    if (other.isInfinite())
        return other;

    // Do the standard thing for now.
    IntegerBase<supportInfinity> ans(*this);
    return ans -= other;
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>
        IntegerBase<supportInfinity>::operator -(long other) const {
    if (isInfinite())
        return *this;

    // Do the standard thing for now.
    IntegerBase<supportInfinity> ans(*this);
    return ans -= other;
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>
        IntegerBase<supportInfinity>::operator *(
        const IntegerBase<supportInfinity>& other) const {
    if (isInfinite())
        return *this;
    if (other.isInfinite())
        return other;

    // Do the standard thing for now.
    IntegerBase<supportInfinity> ans(*this);
    return ans *= other;
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>
        IntegerBase<supportInfinity>::operator *(long other) const {
    if (isInfinite())
        return *this;

    // Do the standard thing for now.
    IntegerBase<supportInfinity> ans(*this);
    return ans *= other;
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>
        IntegerBase<supportInfinity>::operator /(
        const IntegerBase<supportInfinity>& other) const {
    if (isInfinite())
        return *this;
    if (other.isInfinite())
        return (long)0;
    if (other.isZero()) {
        IntegerBase<supportInfinity> ans;
        ans.makeInfinite();
        return ans;
    }

    // Do the standard thing for now.
    IntegerBase<supportInfinity> ans(*this);
    return ans /= other;
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>
        IntegerBase<supportInfinity>::operator /(long other) const {
    if (isInfinite())
        return *this;
    if (other == 0) {
        IntegerBase<supportInfinity> ans;
        ans.makeInfinite();
        return ans;
    }

    // Do the standard thing for now.
    IntegerBase<supportInfinity> ans(*this);
    return ans /= other;
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>
        IntegerBase<supportInfinity>::divExact(
        const IntegerBase<supportInfinity>& other) const {
    // Do the standard thing for now.
    IntegerBase<supportInfinity> ans(*this);
    return ans.divByExact(other);
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>
        IntegerBase<supportInfinity>::divExact(long other) const {
    // Do the standard thing for now.
    IntegerBase<supportInfinity> ans(*this);
    return ans.divByExact(other);
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>
        IntegerBase<supportInfinity>::operator %(
        const IntegerBase<supportInfinity>& other) const {
    // Do the standard thing for now.
    IntegerBase<supportInfinity> ans(*this);
    return ans %= other;
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>
        IntegerBase<supportInfinity>::operator %(long other) const {
    // Do the standard thing for now.
    IntegerBase<supportInfinity> ans(*this);
    return ans %= other;
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>
        IntegerBase<supportInfinity>::operator -() const {
    if (isInfinite())
        return *this;
    if (large_) {
        IntegerBase<supportInfinity> ans;
        ans.large_ = new __mpz_struct[1];
        mpz_init(ans.large_);
        mpz_neg(ans.large_, large_);
        return ans;
    } else if (small_ == LONG_MIN) {
        // Overflow, just.
        IntegerBase<supportInfinity> ans;
        ans.large_ = new __mpz_struct[1];
        mpz_init_set_si(ans.large_, small_);
        mpz_neg(ans.large_, ans.large_);
        return ans;
    } else
        return IntegerBase<supportInfinity>(-small_);
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>&
        IntegerBase<supportInfinity>::operator +=(
        const IntegerBase<supportInfinity>& other) {
    if (isInfinite())
        return *this;
    else if (other.isInfinite()) {
        makeInfinite();
        return *this;
    }
    if (other.large_) {
        if (! large_)
            forceLarge();
        mpz_add(large_, large_, other.large_);
        return *this;
    } else
        return (*this) += other.small_;
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity>&
        IntegerBase<supportInfinity>::operator -=(
        const IntegerBase<supportInfinity>& other) {
    if (isInfinite())
        return *this;
    else if (other.isInfinite()) {
        makeInfinite();
        return *this;
    }
    if (other.large_) {
        if (! large_)
            forceLarge();
        mpz_sub(large_, large_, other.large_);
        return *this;
    } else
        return (*this) -= other.small_;
}

template <bool supportInfinity>
inline void IntegerBase<supportInfinity>::negate() {
    if (isInfinite())
        return;
    if (large_)
        mpz_neg(large_, large_);
    else if (small_ == LONG_MIN) {
        // Overflow, just.
        forceLarge();
        mpz_neg(large_, large_);
    } else
        small_ = -small_;
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity> IntegerBase<supportInfinity>::abs()
        const {
    if (isInfinite())
        return *this;
    if (large_) {
        IntegerBase<supportInfinity> ans;
        ans.large_ = new __mpz_struct[1];
        mpz_init_set(ans.large_, large_);
        mpz_abs(ans.large_, large_);
        return ans;
    } else if (small_ == LONG_MIN) {
        // Overflow, just.
        IntegerBase<supportInfinity> ans;
        ans.large_ = new __mpz_struct[1];
        mpz_init_set_si(ans.large_, small_);
        mpz_neg(ans.large_, ans.large_);
        return ans;
    } else
        return IntegerBase<supportInfinity>(small_ >= 0 ? small_ : - small_);
}

template <bool supportInfinity>
IntegerBase<supportInfinity> IntegerBase<supportInfinity>::gcd(
        const IntegerBase<supportInfinity>& other) const {
    // Do the standard thing for now.
    IntegerBase<supportInfinity> ans(*this);
    ans.gcdWith(other);
    return ans;
}

template <bool supportInfinity>
IntegerBase<supportInfinity> IntegerBase<supportInfinity>::lcm(
        const IntegerBase<supportInfinity>& other) const {
    // Do the standard thing for now.
    IntegerBase<supportInfinity> ans(*this);
    ans.lcmWith(other);
    return ans;
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity> operator +(long lhs,
        const IntegerBase<supportInfinity>& rhs) {
    return rhs + lhs;
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity> operator *(long lhs,
        const IntegerBase<supportInfinity>& rhs) {
    return rhs * lhs;
}

template <bool supportInfinity>
inline std::tuple<IntegerBase<supportInfinity>, IntegerBase<supportInfinity>,
        IntegerBase<supportInfinity>>
        IntegerBase<supportInfinity>::gcdWithCoeffs(
        const IntegerBase<supportInfinity>& other) const {
    // In the long term, this will eventually become the preferred
    // implementation.  For now though, just forward to the non-tuple variant.
    std::tuple<IntegerBase, IntegerBase, IntegerBase> ans;
    std::get<0>(ans) = gcdWithCoeffs(other, std::get<1>(ans), std::get<2>(ans));
    return ans;
}

template <bool supportInfinity>
inline void IntegerBase<supportInfinity>::setRaw(mpz_srcptr fromData) {
    makeFinite();
    if (! large_) {
        large_ = new __mpz_struct[1];
        mpz_init_set(large_, fromData);
    } else {
        mpz_set(large_, fromData);
    }
}

template <bool supportInfinity>
inline mpz_srcptr IntegerBase<supportInfinity>::rawData() const {
    // Cast away the const, since we are not changing the mathematical value.
    // We are, however, bulking up the representation.
    const_cast<IntegerBase<supportInfinity>&>(*this).makeLarge();
    return large_;
}

template <bool supportInfinity>
inline mpz_ptr IntegerBase<supportInfinity>::rawData() {
    makeLarge();
    return large_;
}

template <bool supportInfinity>
inline void IntegerBase<supportInfinity>::makeLarge() {
    if (! large_)
        forceLarge();
}

template <bool supportInfinity>
inline void IntegerBase<supportInfinity>::tryReduce() {
    if (large_ && mpz_cmp_si(large_, LONG_MAX) <= 0 &&
            mpz_cmp_si(large_, LONG_MIN) >= 0)
        forceReduce();
}

template <bool supportInfinity>
inline void IntegerBase<supportInfinity>::tightEncode(std::ostream& out) const {
    regina::detail::tightEncodeInteger(out, *this);
}

template <bool supportInfinity>
inline std::string IntegerBase<supportInfinity>::tightEncoding() const {
    std::ostringstream out;
    regina::detail::tightEncodeInteger(out, *this);
    return out.str();
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity> IntegerBase<supportInfinity>::tightDecoding(
        const std::string& enc) {
    return regina::detail::tightDecodeInteger<IntegerBase>(
        enc.begin(), enc.end(), true);
}

template <bool supportInfinity>
inline IntegerBase<supportInfinity> IntegerBase<supportInfinity>::tightDecode(
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

template <bool supportInfinity>
inline void IntegerBase<supportInfinity>::forceLarge() {
    large_ = new __mpz_struct[1];
    mpz_init_set_si(large_, small_);
}

template <bool supportInfinity>
inline void IntegerBase<supportInfinity>::clearLarge() {
    mpz_clear(large_);
    delete[] large_;
    large_ = nullptr;
}

template <bool supportInfinity>
inline void IntegerBase<supportInfinity>::forceReduce() {
    small_ = mpz_get_si(large_);
    mpz_clear(large_);
    delete[] large_;
    large_ = nullptr;
}

#ifndef __DOXYGEN // Doxygen gets confused by the specialisations.

template <>
inline void IntegerBase<true>::makeFinite() {
    infinite_ = false;
}

template <>
inline void IntegerBase<false>::makeFinite() {
}

// This definition must come *after* the definition of makeInfinite()
// to keep the compiler happy.
template <>
inline IntegerBase<true>::IntegerBase(bool, bool) : large_(nullptr) {
    // The infinity constructor.
    makeInfinite();
}

template <>
inline const IntegerBase<true> IntegerBase<true>::infinity(false, false);

#endif // __DOXYGEN

template <bool supportInfinity>
inline void swap(IntegerBase<supportInfinity>& a,
        IntegerBase<supportInfinity>& b) noexcept {
    a.swap(b);
}

template <bool supportInfinity>
void tightEncode(std::ostream& out, IntegerBase<supportInfinity> value) {
    regina::detail::tightEncodeInteger(out, std::move(value));
}

template <bool supportInfinity>
std::string tightEncoding(IntegerBase<supportInfinity> value) {
    std::ostringstream out;
    regina::detail::tightEncodeInteger(out, std::move(value));
    return out.str();
}

// Inline functions for NativeInteger

template <int bytes>
inline NativeInteger<bytes>::NativeInteger() : data_(0) {
}

template <int bytes>
inline NativeInteger<bytes>::NativeInteger(Native value) : data_(value) {
}

template <int bytes>
inline NativeInteger<bytes>::NativeInteger(
        const NativeInteger<bytes>& value) :
        data_(value.data_) {
}

template <int bytes>
template <bool supportInfinity>
inline NativeInteger<bytes>::NativeInteger(
        const IntegerBase<supportInfinity>& value) :
        data_(value.template nativeValue<bytes>()) {
}

template <int bytes>
inline bool NativeInteger<bytes>::isZero() const {
    return (data_ == 0);
}

template <int bytes>
inline int NativeInteger<bytes>::sign() const {
    return (data_ > 0 ? 1 : data_ < 0 ? -1 : 0);
}

template <int bytes>
inline typename NativeInteger<bytes>::Native NativeInteger<bytes>::
        nativeValue() const {
    return data_;
}

template <int bytes>
inline NativeInteger<bytes>& NativeInteger<bytes>::operator =(
        const NativeInteger<bytes>& value) {
    data_ = value.data_;
    return *this;
}

template <int bytes>
inline NativeInteger<bytes>& NativeInteger<bytes>::operator =(Native value) {
    data_ = value;
    return *this;
}

template <int bytes>
inline void NativeInteger<bytes>::swap(NativeInteger<bytes>& other) noexcept {
    std::swap(data_, other.data_);
}

template <int bytes>
inline bool NativeInteger<bytes>::operator ==(
        const NativeInteger<bytes>& rhs) const {
    return (data_ == rhs.data_);
}

template <int bytes>
inline bool NativeInteger<bytes>::operator ==(Native rhs) const {
    return (data_ == rhs);
}

template <int bytes>
inline bool NativeInteger<bytes>::operator !=(
        const NativeInteger<bytes>& rhs) const {
    return (data_ != rhs.data_);
}

template <int bytes>
inline bool NativeInteger<bytes>::operator !=(Native rhs) const {
    return (data_ != rhs);
}

template <int bytes>
inline bool NativeInteger<bytes>::operator <(
        const NativeInteger<bytes>& rhs) const {
    return (data_ < rhs.data_);
}

template <int bytes>
inline bool NativeInteger<bytes>::operator <(Native rhs) const {
    return (data_ < rhs);
}

template <int bytes>
inline bool NativeInteger<bytes>::operator >(
        const NativeInteger<bytes>& rhs) const {
    return (data_ > rhs.data_);
}

template <int bytes>
inline bool NativeInteger<bytes>::operator >(Native rhs) const {
    return (data_ > rhs);
}

template <int bytes>
inline bool NativeInteger<bytes>::operator <=(
        const NativeInteger<bytes>& rhs) const {
    return (data_ <= rhs.data_);
}

template <int bytes>
inline bool NativeInteger<bytes>::operator <=(Native rhs) const {
    return (data_ <= rhs);
}

template <int bytes>
inline bool NativeInteger<bytes>::operator >=(
        const NativeInteger<bytes>& rhs) const {
    return (data_ >= rhs.data_);
}

template <int bytes>
inline bool NativeInteger<bytes>::operator >=(Native rhs) const {
    return (data_ >= rhs);
}

template <int bytes>
inline NativeInteger<bytes>& NativeInteger<bytes>::operator ++() {
    ++data_;
    return *this;
}

template <int bytes>
inline NativeInteger<bytes> NativeInteger<bytes>::operator ++(int) {
    return NativeInteger<bytes>(data_++);
}

template <int bytes>
inline NativeInteger<bytes>& NativeInteger<bytes>::operator --() {
    --data_;
    return *this;
}

template <int bytes>
inline NativeInteger<bytes> NativeInteger<bytes>::operator --(int) {
    return NativeInteger<bytes>(data_--);
}

template <int bytes>
inline NativeInteger<bytes> NativeInteger<bytes>::operator +(
        const NativeInteger<bytes>& other) const {
    return NativeInteger<bytes>(data_ + other.data_);
}

template <int bytes>
inline NativeInteger<bytes> NativeInteger<bytes>::operator +(
        Native other) const {
    return NativeInteger<bytes>(data_ + other);
}

template <int bytes>
inline NativeInteger<bytes> NativeInteger<bytes>::operator -(
        const NativeInteger<bytes>& other) const {
    return NativeInteger<bytes>(data_ - other.data_);
}

template <int bytes>
inline NativeInteger<bytes> NativeInteger<bytes>::operator -(
        Native other) const {
    return NativeInteger<bytes>(data_ - other);
}

template <int bytes>
inline NativeInteger<bytes> NativeInteger<bytes>::operator *(
        const NativeInteger<bytes>& other) const {
    return NativeInteger<bytes>(data_ * other.data_);
}

template <int bytes>
inline NativeInteger<bytes> NativeInteger<bytes>::operator *(
        Native other) const {
    return NativeInteger<bytes>(data_ * other);
}

template <int bytes>
inline NativeInteger<bytes> NativeInteger<bytes>::operator /(
        const NativeInteger<bytes>& other) const {
    return NativeInteger<bytes>(data_ / other.data_);
}

template <int bytes>
inline NativeInteger<bytes> NativeInteger<bytes>::operator /(
        Native other) const {
    return NativeInteger<bytes>(data_ / other);
}

template <int bytes>
inline NativeInteger<bytes> NativeInteger<bytes>::divExact(
        const NativeInteger<bytes>& other) const {
    return NativeInteger<bytes>(data_ / other.data_);
}

template <int bytes>
inline NativeInteger<bytes> NativeInteger<bytes>::divExact(
        Native other) const {
    return NativeInteger<bytes>(data_ / other);
}

template <int bytes>
inline NativeInteger<bytes> NativeInteger<bytes>::operator %(
        const NativeInteger<bytes>& other) const {
    return NativeInteger<bytes>(data_ % other.data_);
}

template <int bytes>
inline NativeInteger<bytes> NativeInteger<bytes>::operator %(
        Native other) const {
    return NativeInteger<bytes>(data_ % other);
}

template <int bytes>
inline std::pair<NativeInteger<bytes>, NativeInteger<bytes>>
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
inline NativeInteger<bytes> NativeInteger<bytes>::operator -() const {
    return NativeInteger<bytes>(- data_);
}

template <int bytes>
inline NativeInteger<bytes>& NativeInteger<bytes>::operator += (
        const NativeInteger<bytes>& other) {
    data_ += other.data_;
    return *this;
}

template <int bytes>
inline NativeInteger<bytes>& NativeInteger<bytes>::operator += (
        Native other) {
    data_ += other;
    return *this;
}

template <int bytes>
inline NativeInteger<bytes>& NativeInteger<bytes>::operator -= (
        const NativeInteger<bytes>& other) {
    data_ -= other.data_;
    return *this;
}

template <int bytes>
inline NativeInteger<bytes>& NativeInteger<bytes>::operator -= (
        Native other) {
    data_ -= other;
    return *this;
}

template <int bytes>
inline NativeInteger<bytes>& NativeInteger<bytes>::operator *= (
        const NativeInteger<bytes>& other) {
    data_ *= other.data_;
    return *this;
}

template <int bytes>
inline NativeInteger<bytes>& NativeInteger<bytes>::operator *= (
        Native other) {
    data_ *= other;
    return *this;
}

template <int bytes>
inline NativeInteger<bytes>& NativeInteger<bytes>::operator /= (
        const NativeInteger<bytes>& other) {
    data_ /= other.data_;
    return *this;
}

template <int bytes>
inline NativeInteger<bytes>& NativeInteger<bytes>::operator /= (
        Native other) {
    data_ /= other;
    return *this;
}

template <int bytes>
inline NativeInteger<bytes>& NativeInteger<bytes>::divByExact(
        const NativeInteger<bytes>& other) {
    data_ /= other.data_;
    return *this;
}

template <int bytes>
inline NativeInteger<bytes>& NativeInteger<bytes>::divByExact(Native other) {
    data_ /= other;
    return *this;
}

template <int bytes>
inline NativeInteger<bytes>& NativeInteger<bytes>::operator %= (
        const NativeInteger<bytes>& other) {
    data_ %= other.data_;
    return *this;
}

template <int bytes>
inline NativeInteger<bytes>& NativeInteger<bytes>::operator %= (
        Native other) {
    data_ %= other;
    return *this;
}

template <int bytes>
inline void NativeInteger<bytes>::negate() {
    data_ = - data_;
}

template <int bytes>
void NativeInteger<bytes>::gcdWith(const NativeInteger<bytes>& other) {
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
inline NativeInteger<bytes> NativeInteger<bytes>::gcd(
        const NativeInteger<bytes>& other) const {
    NativeInteger<bytes> ans(data_);
    ans.gcdWith(other);
    return ans;
}

template <int bytes>
inline bool NativeInteger<bytes>::isInfinite() const {
    return false;
}

template <int bytes>
inline void NativeInteger<bytes>::tryReduce() {
}

template <int bytes>
inline std::ostream& operator << (std::ostream& out,
        const NativeInteger<bytes>& i) {
    return out << i.data_;
}

#ifndef __DOXYGEN // Doxygen gets confused by the specialisations.

#ifdef INT128_AVAILABLE
template <>
inline std::ostream& operator << (std::ostream& out,
        const NativeInteger<16>& i) {
    // The standard library does not support 128-bit integers (at least not
    // always).  Go through Integer and GMP instead (for now).
    return out << Integer(i);
}
#endif

#endif // __DOXYGEN

template <int bytes>
inline void swap(NativeInteger<bytes>& a, NativeInteger<bytes>& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

