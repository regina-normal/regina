
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

#ifndef __NINTEGER_H
#ifndef __DOXYGEN
#define __NINTEGER_H
#endif

/*! \file maths/ninteger.h
 *  \brief Deals with arbitrary precision integers.
 */

#include <climits>
#include <gmp.h>
#include <iostream>
#include <string>
#include "regina-core.h"

/**
 * \hideinitializer
 *
 * An internal copy of the GMP signed comparison optimisations.
 * This macro should not be used outside this class.
 *
 * By making our own copy of such optimisation macros we can use
 * C++-style casts instead of C-style casts and avoid noisy compiler
 * warnings.  I'd love a better way of doing this.
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

/**
 * \weakgroup maths
 * @{
 */

/**
 * Policy classes for use with NInteger, indicating whether we
 * should support infinity as an allowed value.
 *
 * See the NInteger class notes for details.
 */
template <bool supportInfinity>
class REGINA_API InfinityPolicy;

template <>
class REGINA_API InfinityPolicy<true> {
    private:
        bool infinite;
            /**< Does this integer represent infinity? */

    public:
        /**
         * Default constructor.
         * Sets this integer to be finite.
         */
        inline InfinityPolicy() : infinite(false) {
        }

        /**
         * Returns whether this integer is infinity.
         *
         * @return \c true if and only if this integer is infinity.
         */
        inline bool isInfinite() const {
            return infinite;
        };

    protected:
        /**
         * Sets this integer to be finite.
         */
        inline void makeFinite() {
            infinite = false;
        }

        /**
         * Sets this integer to be infinite.
         */
        inline void makeInfinite() {
            infinite = true;
        }

        /**
         * Swaps the data held by this policy class between
         * this and the given object.
         *
         * @param other the objects whose data is to be swapped with this.
         */
        inline void swap(InfinityPolicy<true>& other) {
            std::swap(infinite, other.infinite);
        }
};

template <>
class REGINA_API InfinityPolicy<false> {
    public:
        /**
         * Returns whether this integer is infinity.
         * For IntegerWithoutInfinity, this routine always returns \c false.
         *
         * @return \c true if and only if this integer is infinity.
         */
        inline bool isInfinite() const {
            return false;
        };

    protected:
        /**
         * Sets this integer to be finite.
         * For IntegerWithoutInfinity, this routine does nothing.
         */
        inline void makeFinite() {
        }

        /**
         * Sets this integer to be infinite.
         * For IntegerWithoutInfinity, this routine does nothing
         * (and should never be called, since infinity is not supported).
         */
        inline void makeInfinite() {
        }

        /**
         * Swaps the data held by this policy class between
         * this and the given object.
         * For IntegerWithoutInfinity, this routine does nothing.
         *
         * @param other the objects whose data is to be swapped with this.
         */
        inline void swap(InfinityPolicy<false>& other) {
        }
};

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
 * \ifacespython Both variants of this template are available through Python.
 * For \a supportInfinity = \c false (the default), simply use NInteger.
 * For \a supportInfinity = \c true, use NLargeInteger.
 *
 * \testpart
 */
template <bool supportInfinity = false>
class REGINA_API NInteger : public InfinityPolicy<supportInfinity> {
    public:
        static const NInteger<supportInfinity> zero;
            /**< Globally available zero. */
        static const NInteger<supportInfinity> one;
            /**< Globally available one. */
        static const NInteger<supportInfinity> infinity;
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
            /**< 0 if we are using native representations, or a pointer to the
                 full GMP large integer if we are now using these instead.
                 We require that, whenever this pointer is non-null, the
                 corresponding GMP large integer is initialised.
                 If this integer is infinity then large_ must be null. */
                 // TODO: Do that.

    public:
        /**
         * Initialises this integer to zero.
         */
        NInteger();
        /**
         * Initialises this integer to the given value.
         *
         * \ifacespython In Python, the only native-integer constructor
         * is NInteger(long).
         *
         * @param value the new value of this integer.
         */
        NInteger(int value);
        /**
         * Initialises this integer to the given value.
         *
         * \ifacespython In Python, the only native-integer constructor
         * is NInteger(long).
         *
         * @param value the new value of this integer.
         */
        NInteger(unsigned value);
        /**
         * Initialises this integer to the given value.
         *
         * \ifacespython In Python, this is the only native-integer
         * constructor available.
         *
         * @param value the new value of this integer.
         */
        NInteger(long value);
        /**
         * Initialises this integer to the given value.
         *
         * \ifacespython In Python, the only native-integer constructor
         * is NInteger(long).
         *
         * @param value the new value of this integer.
         */
        NInteger(unsigned long value);
        /**
         * Initialises this integer to the given value.
         *
         * @param value the new value of this integer.
         */
        NInteger(const NInteger<supportInfinity>& value);
        /**
         * Initialises this integer to the given value.
         *
         * This constructor is marked as explicit in the hope of
         * avoiding accidental (and unintentional) mixing of template
         * parameters.
         *
         * \pre If this class does not support infinity, then \a value
         * must not be infinite.
         *
         * TODO: Implement.
         *
         * @param value the new value of this integer.
         */
        template <bool rhsSupportInfinity>
        explicit NInteger(const NInteger<rhsSupportInfinity>& value);
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
         * TODO: Test 0X.
         *
         * Whitespace may be present at the beginning of the given string,
         * and will simply be ignored.
         *
         * Error detection is possible by passing a non-null boolean
         * pointer as the third parameter to this constructor.
         *
         * For finer details on how the string parsing works, see
         * strtol() from the standard C library (on which this method
         * is based).
         *
         * \pre The given base is zero, or is between 2 and 36 inclusive.
         * \pre The given string represents a finite integer
         * in the given base, with optional whitespace beforehand.
         * TODO: Test the validity of whitespace before and after.
         *
         * \ifacespython The final parameter \a valid is not present.
         *
         * @param value the new value of this integer, represented as a string
         * of digits in base \a base.
         * @param base the base in which \a value is given.
         * @param valid if this pointer is not null, the boolean referenced
         * will be set to \c true if the entire given string was a valid
         * large integer representation and \c false otherwise.
         */
        NInteger(const char* value, int base = 10, bool* valid = 0);
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
         * TODO: Text 0X.
         *
         * Whitespace may be present at the beginning of the given string,
         * and will simply be ignored.
         *
         * Error detection is possible by passing a non-null boolean
         * pointer as the third parameter to this constructor.
         *
         * For finer details on how the string parsing works, see
         * strtol() from the standard C library (on which this method
         * is based).
         *
         * \pre The given base is zero, or is between 2 and 36 inclusive.
         * \pre The given string represents an integer
         * in the given base, with optional whitespace beforehand.
         * TODO: Test the validity of whitespace before and after.
         *
         * \ifacespython The final parameter \a valid is not present.
         *
         * @param value the new value of this integer, represented as a string
         * of digits in base \a base.
         * @param base the base in which \a value is given.
         * @param valid if this pointer is not null, the boolean referenced
         * will be set to \c true if the entire given string was a valid
         * large integer representation and \c false otherwise.
         */
        NInteger(const std::string& value, int base = 10,
                bool* valid = 0);
        /**
         * Destroys this integer.
         */
        ~NInteger();

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
         * Returns whether this integer is infinity.
         *
         * @return \c true if and only if this integer is infinity.
         */
#ifdef __DOXYGEN
        bool isInfinite() const;
#else
        using InfinityPolicy<supportInfinity>::isInfinite;
#endif

        /**
         * Returns the value of this integer as a long.
         *
         * If this integer is outside the range of a long, the result is
         * unpredictable.  If it is within the range of a long however,
         * it will give correct results regardless of whether the
         * underlying representation is a native or large integer.
         * In particular, it guarantees to give correct results if
         * isNative() returns \c true.
         *
         * \pre This integer is not infinity.
         *
         * @return the value of this integer.
         */
        long longValue() const;
        /**
         * Returns the value of this integer as a string in the given
         * base.  If not specified, the base defaults to 10.
         *
         * If this integer is infinity, the string returned will be
         * \c inf.
         *
         * \pre The given base is between 2 and 36 inclusive.
         *
         * @return the value of this integer as a newly allocated
         * string.
         */
        std::string stringValue(int base = 10) const;

        /**
         * Sets this integer to the given value.
         *
         * @param value the new value of this integer.
         * @return a reference to this integer with its new value.
         */
        NInteger& operator =(const NInteger& value);
        /**
         * Sets this integer to the given value.
         *
         * \pre If this class does not support infinity, then \a value
         * must not be infinite.
         *
         * TODO: Implement.
         *
         * @param value the new value of this integer.
         * @return a reference to this integer with its new value.
         */
        template <bool rhsSupportInfinity>
        NInteger& operator = (
            const NInteger<rhsSupportInfinity>& value);
        /**
         * Sets this integer to the given value.
         *
         * @param value the new value of this integer.
         * @return a reference to this integer with its new value.
         */
        NInteger& operator =(int value);
        /**
         * Sets this integer to the given value.
         *
         * @param value the new value of this integer.
         * @return a reference to this integer with its new value.
         */
        NInteger& operator =(unsigned value);
        /**
         * Sets this integer to the given value.
         *
         * @param value the new value of this integer.
         * @return a reference to this integer with its new value.
         */
        NInteger& operator =(long value);
        /**
         * Sets this integer to the given value.
         *
         * @param value the new value of this integer.
         * @return a reference to this integer with its new value.
         */
        NInteger& operator =(unsigned long value);
        /**
         * Sets this integer to the given value which is
         * represented as a string of digits in base 10.
         *
         * Whitespace may be present in the given string and will simply
         * be ignored.
         *
         * \pre The given string represents an integer
         * in base 10, with optional whitespace added.
         *
         * @param value the new value of this integer, represented as a string
         * of digits in base 10.
         * @return a reference to this integer with its new value.
         */
        NInteger& operator =(const char* value);
        /**
         * Sets this integer to the given value which is
         * represented as a string of digits in base 10.
         *
         * Whitespace may be present in the given string and will simply
         * be ignored.
         *
         * \pre The given string represents an integer
         * in base 10, with optional whitespace added.
         * TODO: Test whitespace.
         *
         * @param value the new value of this integer, represented as a string
         * of digits in base 10.
         * @return a reference to this integer with its new value.
         */
        NInteger& operator =(const std::string& value);
        /**
         * Swaps the values of this and the given integer.
         *
         * @param other the integer whose value will be swapped with
         * this.
         */
        void swap(NInteger& other);

        /**
         * Determines if this is equal to the given integer.
         *
         * @param rhs the integer with which this will be compared.
         * @return \c true if and only if this and the given integer are
         * equal.
         */
        bool operator ==(const NInteger& rhs) const;
        /**
         * Determines if this is equal to the given integer.
         *
         * @param rhs the integer with which this will be compared.
         * @return \c true if and only if this and the given integer are
         * equal.
         */
        bool operator ==(long rhs) const;
        /**
         * Determines if this is not equal to the given integer.
         *
         * @param rhs the integer with which this will be compared.
         * @return \c true if and only if this and the given integer are
         * not equal.
         */
        bool operator !=(const NInteger& rhs) const;
        /**
         * Determines if this is not equal to the given integer.
         *
         * @param rhs the integer with which this will be compared.
         * @return \c true if and only if this and the given integer are
         * not equal.
         */
        bool operator !=(long rhs) const;
        /**
         * Determines if this is less than the given integer.
         *
         * @param rhs the integer with which this will be compared.
         * @return \c true if and only if this is less than the given
         * integer.
         */
        bool operator <(const NInteger& rhs) const;
        /**
         * Determines if this is less than the given integer.
         *
         * @param rhs the integer with which this will be compared.
         * @return \c true if and only if this is less than the given
         * integer.
         */
        bool operator <(long rhs) const;
        /**
         * Determines if this is greater than the given integer.
         *
         * @param rhs the integer with which this will be compared.
         * @return \c true if and only if this is greater than the given
         * integer.
         */
        bool operator >(const NInteger& rhs) const;
        /**
         * Determines if this is greater than the given integer.
         *
         * @param rhs the integer with which this will be compared.
         * @return \c true if and only if this is greater than the given
         * integer.
         */
        bool operator >(long rhs) const;
        /**
         * Determines if this is less than or equal to the given integer.
         *
         * @param rhs the integer with which this will be compared.
         * @return \c true if and only if this is less than or equal to
         * the given integer.
         */
        bool operator <=(const NInteger& rhs) const;
        /**
         * Determines if this is less than or equal to the given integer.
         *
         * @param rhs the integer with which this will be compared.
         * @return \c true if and only if this is less than or equal to
         * the given integer.
         */
        bool operator <=(long rhs) const;
        /**
         * Determines if this is greater than or equal to the given integer.
         *
         * @param rhs the integer with which this will be compared.
         * @return \c true if and only if this is greater than or equal
         * to the given integer.
         */
        bool operator >=(const NInteger& rhs) const;
        /**
         * Determines if this is greater than or equal to the given integer.
         *
         * @param rhs the integer with which this will be compared.
         * @return \c true if and only if this is greater than or equal
         * to the given integer.
         */
        bool operator >=(long rhs) const;

        /**
         * The preincrement operator.
         * This operator increments this integer by one, and returns a
         * reference to the integer \e after the increment.
         *
         * \ifacespython Not available.
         *
         * @return a reference to this integer after the increment.
         */
        NInteger& operator ++();

        /**
         * The postincrement operator.
         * This operator increments this integer by one, and returns a
         * copy of the integer \e before the increment.
         *
         * \ifacespython Not available.
         *
         * @return a copy of this integer before the
         * increment took place.
         */
        NInteger operator ++(int);

        /**
         * The predecrement operator.
         * This operator decrements this integer by one, and returns a
         * reference to the integer \e after the decrement.
         *
         * \ifacespython Not available.
         *
         * @return a reference to this integer after the decrement.
         */
        NInteger& operator --();

        /**
         * The postdecrement operator.
         * This operator decrements this integer by one, and returns a
         * copy of the integer \e before the decrement.
         *
         * \ifacespython Not available.
         *
         * @return a copy of this integer before the
         * decrement took place.
         */
        NInteger operator --(int);

        /**
         * Adds this to the given integer and returns the result.
         * This integer is not changed.
         *
         * If either term of the sum is infinite, the result will be
         * infinity.
         *
         * @param other the integer to add to this integer.
         * @return the sum \a this plus \a other.
         */
        NInteger operator +(const NInteger& other) const;
        /**
         * Adds this to the given integer and returns the result.
         * This integer is not changed.
         *
         * If either term of the sum is infinite, the result will be
         * infinity.
         *
         * @param other the integer to add to this integer.
         * @return the sum \a this plus \a other.
         */
        NInteger operator +(long other) const;
        /**
         * Subtracts the given integer from this and returns the result.
         * This integer is not changed.
         *
         * If either term of the difference is infinite, the result will be
         * infinity.
         *
         * @param other the integer to subtract from this integer.
         * @return the difference \a this minus \a other.
         */
        NInteger operator -(const NInteger& other) const;
        /**
         * Subtracts the given integer from this and returns the result.
         * This integer is not changed.
         *
         * If either term of the difference is infinite, the result will be
         * infinity.
         *
         * @param other the integer to subtract from this integer.
         * @return the difference \a this minus \a other.
         */
        NInteger operator -(long other) const;
        /**
         * Multiplies this by the given integer and returns the
         * result.
         * This integer is not changed.
         *
         * If either factor of the product is infinite, the result will be
         * infinity.
         *
         * @param other the integer to multiply by this integer.
         * @return the product \a this times \a other.
         */
        NInteger operator *(const NInteger& other) const;
        /**
         * Multiplies this by the given integer and returns the
         * result.
         * This integer is not changed.
         *
         * If either factor of the product is infinite, the result will be
         * infinity.
         *
         * @param other the integer to multiply by this integer.
         * @return the product \a this times \a other.
         */
        NInteger operator *(long other) const;
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
         * TODO: Test rounding direction, for all variants of division.
         *
         * \pre If this class does not support infinity, then
         * \a other must be non-zero.
         *
         * \warning As I understand it, the direction of rounding for
         * native C/C++ integer division was fixed in the C++11
         * specification, but left to the compiler implementation in
         * earlier versions of the specification; however, any modern
         * hardware should satisfy the C++11 rounding rule as described above.
         *
         * @param other the integer to divide this by.
         * @return the quotient \a this divided by \a other.
         */
        NInteger operator /(const NInteger& other) const;
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
         * \warning As I understand it, the direction of rounding for
         * native C/C++ integer division was fixed in the C++11
         * specification, but left to the compiler implementation in
         * earlier versions of the specification; however, any modern
         * hardware should satisfy the C++11 rounding rule as described above.
         *
         * @param other the integer to divide this by.
         * @return the quotient \a this divided by \a other.
         */
        NInteger operator /(long other) const;
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
         * @param other the integer to divide this by.
         * @return the quotient \a this divided by \a other.
         */
        NInteger divExact(const NInteger& other) const;
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
         * @param other the integer to divide this by.
         * @return the quotient \a this divided by \a other.
         */
        NInteger divExact(long other) const;
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
         * \warning As I understand it, the sign of the result under
         * native C/C++ integer division when the second operand is
         * negative was fixed in the C++11 specification, but left to the
         * compiler implementation in earlier versions of the specification;
         * however, any modern hardware should satisfy the C++11 sign rule
         * as described above.
         *
         * @param other the integer to divide this by.
         * @return the remainder \a this modulo \a other.
         */
        NInteger operator %(const NInteger& other) const;
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
         * \warning As I understand it, the sign of the result under
         * native C/C++ integer division when the second operand is
         * negative was fixed in the C++11 specification, but left to the
         * compiler implementation in earlier versions of the specification;
         * however, any modern hardware should satisfy the C++11 sign rule
         * as described above.
         *
         * @param other the integer to divide this by.
         * @return the remainder \a this modulo \a other.
         */
        NInteger operator %(long other) const;

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
         * in the range <tt>0 <= r < |d|</tt>.
         *
         * Note that this differs from other division routines in this
         * class, in that it always rounds to give a non-negative remainder.
         * Thus NInteger(-7).divisionAlg(3) gives quotient -3 and
         * remainder 2, whereas (-7)/3 gives quotient -2 and (-7)\%3 gives
         * remainder -1.
         *
         * The two results are passed back to the caller as follows:
         * The quotient \a q is passed back as the return value of the
         * function, and the remainder \a r is stored in the reference
         * argument \a r.
         *
         * In the special case where the given divisor is 0 (not
         * allowed by the usual division algorithm), this routine selects
         * quotient 0 and remainder \a n.
         *
         * \pre Neither this nor the divisor are infinite.
         *
         * \ifacespython The argument \a remainder is missing; instead both
         * the quotient and remainder are passed back through the return
         * value of the function.  Specifically, this function returns a
         * (\a quotient, \a remainder) pair.
         *
         * @param divisor the divisor \a d.
         * @param remainder used to store the remainder \a r when the
         * functon returns.  The initial value of this argument is ignored.
         * @return the quotient \a q.
         *
         * @author Ryan Budney, B.B.
         * TODO: Implement.
         */
        NInteger<supportInfinity> divisionAlg(
                const NInteger<supportInfinity>& divisor,
                NInteger<supportInfinity>& remainder) const;

        /**
         * Determines the negative of this integer.
         * This integer is not changed.
         *
         * Negative infinity will return infinity.
         *
         * @return the negative of this integer.
         */
        NInteger operator -() const;

        /**
         * Adds the given integer to this.
         * This integer is changed to reflect the result.
         *
         * If either term of the sum is infinite, the result will be
         * infinity.
         *
         * @param other the integer to add to this integer.
         * @return a reference to this integer with its new value.
         */
        NInteger& operator +=(const NInteger& other);
        /**
         * Adds the given integer to this.
         * This integer is changed to reflect the result.
         *
         * If either term of the sum is infinite, the result will be
         * infinity.
         *
         * @param other the integer to add to this integer.
         * @return a reference to this integer with its new value.
         */
        NInteger& operator +=(long other);
        /**
         * Subtracts the given integer from this.
         * This integer is changed to reflect the result.
         *
         * If either term of the difference is infinite, the result will be
         * infinity.
         *
         * @param other the integer to subtract from this integer.
         * @return a reference to this integer with its new value.
         */
        NInteger& operator -=(const NInteger& other);
        /**
         * Subtracts the given integer from this.
         * This integer is changed to reflect the result.
         *
         * If either term of the difference is infinite, the result will be
         * infinity.
         *
         * @param other the integer to subtract from this integer.
         * @return a reference to this integer with its new value.
         */
        NInteger& operator -=(long other);
        /**
         * Multiplies the given integer by this.
         * This integer is changed to reflect the result.
         *
         * If either factor of the product is infinite, the result will be
         * infinity.
         *
         * @param other the integer to multiply with this integer.
         * @return a reference to this integer with its new value.
         */
        NInteger& operator *=(const NInteger& other);
        /**
         * Multiplies the given integer by this.
         * This integer is changed to reflect the result.
         *
         * If either factor of the product is infinite, the result will be
         * infinity.
         *
         * @param other the integer to multiply with this integer.
         * @return a reference to this integer with its new value.
         */
        NInteger& operator *=(long other);
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
         * \warning As I understand it, the direction of rounding for
         * native C/C++ integer division was fixed in the C++11
         * specification, but left to the compiler implementation in
         * earlier versions of the specification; however, any modern
         * hardware should satisfy the C++11 rounding rule as described above.
         *
         * @param other the integer to divide this by.
         * @return a reference to this integer with its new value.
         */
        NInteger& operator /=(const NInteger& other);
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
         * \warning As I understand it, the direction of rounding for
         * native C/C++ integer division was fixed in the C++11
         * specification, but left to the compiler implementation in
         * earlier versions of the specification; however, any modern
         * hardware should satisfy the C++11 rounding rule as described above.
         *
         * @param other the integer to divide this by.
         * @return a reference to this integer with its new value.
         */
        NInteger& operator /=(long other);
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
         * @param other the integer to divide this by.
         * @return a reference to this integer with its new value.
         */
        NInteger& divByExact(const NInteger& other);
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
         * @param other the integer to divide this by.
         * @return a reference to this integer with its new value.
         */
        NInteger& divByExact(long other);
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
         * \warning As I understand it, the sign of the result under
         * native C/C++ integer division when the second operand is
         * negative was fixed in the C++11 specification, but left to the
         * compiler implementation in earlier versions of the specification;
         * however, any modern hardware should satisfy the C++11 sign rule
         * as described above.
         *
         * @param other the integer modulo which this integer will be
         * reduced.
         * @return a reference to this integer with its new value.
         */
        NInteger& operator %=(const NInteger& other);
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
         * \warning As I understand it, the sign of the result under
         * native C/C++ integer division when the second operand is
         * negative was fixed in the C++11 specification, but left to the
         * compiler implementation in earlier versions of the specification;
         * however, any modern hardware should satisfy the C++11 sign rule
         * as described above.
         *
         * @param other the integer modulo which this integer will be
         * reduced.
         * @return a reference to this integer with its new value.
         */
        NInteger& operator %=(long other);
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
         * @param exp the power to which this integer will be raised.
         * TODO: Implement.
         */
        void raiseToPower(unsigned long exp);

        /**
         * Determines the absolute value of this integer.
         * This integer is not changed.
         *
         * @return the absolute value of this integer.
         */
        NInteger abs() const;
        /**
         * Determines the greatest common divisor of this and the given
         * integer.  This integer is not changed.
         *
         * The result is guaranteed to be non-negative.  As a
         * special case, gcd(0,0) is considered to be zero.
         *
         * \pre Neither this integer nor \a other is infinite.
         *
         * @param other the integer whose greatest common divisor with
         * this will be found.
         * @return the greatest common divisor of this and the given
         * integer.
         */
        NInteger gcd(const NInteger& other) const;
        /**
         * Determines the lowest common multiple of this and the given
         * integer.  This integer is not changed.
         *
         * Note that the result might possibly be negative.
         *
         * \pre Neither this integer nor \a other is infinite.
         *
         * @param other the integer whose lowest common multiple with
         * this will be found.
         * @return the lowest common multiple of this and the given
         * integer.
         */
        NInteger lcm(const NInteger& other) const;

        /**
         * Determines the greatest common divisor of this and the given
         * integer and finds the smallest coefficients with which these
         * integers combine to give their gcd.
         *
         * Note that the given integers need not be non-negative.
         * However, the gcd returned is guaranteed to be non-negative.
         *
         * If \a d is the gcd of \a this and \a other, the values placed
         * into \a u and \a v will be those for which
         * <tt>u*this + v*other = d</tt>,
         * <tt>-abs(this)/d < v*sign(other) <= 0</tt> and
         * <tt>1 <= u*sign(this) <= abs(other)/d</tt>.
         * These equations are not satisfied when either of \a this or
         * \a other are zero, but in this case \a u and \a v are both
         * 0, 1 or -1, using as many zeros as possible.
         *
         * \pre Neither this integer nor \a other is infinite.
         *
         * @param other the integer whose greatest common divisor with
         * this will be found.
         * @param u a variable into which the final coefficient of
         * \a this will be placed.
         * @param v a variable into which the final coefficient of
         * \a other will be placed.
         * @return the greatest common divisor of \a this and \a other.
         * TODO: Implement.
         */
        NInteger<supportInfinity> gcdWithCoeffs(
            const NInteger<supportInfinity>& other,
            NInteger<supportInfinity>& u, NInteger<supportInfinity>& v) const;

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
         * @param p the given odd prime.
         * @return The Legendre symbol (0, 1 or -1) as described above.
         *
         * @author Ryan Budney
         * TODO: implement
         */
        int legendre(const NInteger<supportInfinity>& p) const;

        // TODO: random number routines.

        /**
         * Set this to a copy of the given raw GMP integer.
         *
         * This routine allows NInteger to interact directly with
         * libgmp and libgmpxx if necessary.
         *
         * \ifacespython Not available.
         *
         * @param fromData the raw GMP integer to clone.
         * TODO: Implement.
         */
        void setRaw(mpz_srcptr fromData);

        /**
         * Returns the raw GMP data that describes this integer.
         *
         * This routine allows NInteger to interact directly with
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
         * \ifacespython Not available.
         *
         * @return the raw GMP data.
         */
        mpz_srcptr rawData() const;

        /**
         * Returns the raw GMP data that describes this integer.
         *
         * This routine allows NInteger to interact directly with
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
         * \ifacespython Not available.
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

    private:
        /**
         * Initialises this integer to infinity.
         * All parameters are ignored.
         *
         * This constructor is only defined if \a supportInfinity is \c true.
         * Any attempt to use it when \a supportInfinity is \c false
         * will generate a linker error.
         */
        NInteger(bool, bool);

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

    friend class NInteger<! supportInfinity>; // For conversions.

    template <bool supportInfinity_>
    friend std::ostream& operator << (std::ostream& out,
        const NInteger<supportInfinity_>& large);
};

/**
 * A legacy typedef provided for backward compatibility.
 *
 * \deprecated In C++, the old class NLargeInteger has been
 * replaced with the template class NInteger (which is significantly faster).
 * NLargeInteger is now a typedef for NInteger<true>, which offers the
 * same functionality as the old NLargeInteger.
 *
 * \ifacespython This typedef is available in Python, and indeed Python
 * users will need to use it to access integers with infinity (since the
 * Python class NInteger resolves to the C++ class NInteger<false>,
 * which does not support infinity).
 */
typedef NInteger<true> NLargeInteger;

/**
 * Writes the given integer to the given output stream.
 *
 * @param out the output stream to which to write.
 * @param large the integer to write.
 * @return a reference to \a out.
 */
template <bool supportInfinity>
REGINA_API std::ostream& operator << (std::ostream& out,
    const NInteger<supportInfinity>& i);

/**
 * Adds the given native integer to the given large integer.
 * If the large integer is infinite, the result will also be infinity.
 *
 * \ifacespython Not available.
 *
 * @param lhs the native integer to add.
 * @param rhs the large integer to add.
 * @return the sum \a lhs plus \a rhs.
 */
template <bool supportInfinity>
REGINA_API NInteger<supportInfinity> operator + (long lhs,
    const NInteger<supportInfinity>& rhs);

/**
 * Multiplies the given native integer with the given large integer.
 * If the large integer is infinite, the result will also be infinity.
 *
 * \ifacespython Not available.
 *
 * @param lhs the native integer to multiply.
 * @param rhs the large integer to multiply.
 * @return the product \a lhs times \a rhs.
 */
template <bool supportInfinity>
REGINA_API NInteger<supportInfinity> operator * (long lhs,
    const NInteger<supportInfinity>& rhs);

/*@}*/

} // namespace regina

#ifndef __DOXYGEN
namespace libnormaliz {
/**
 * Explicit integer cast functions, for compatibility with Normaliz.
 *
 * We define functions separately for each variant of \a supportInfinity
 * to avoid partial template specialisation.
 *
 * @param a an instance of some arbitrary integer type.
 * @return the given integer, cast as a native long.
 */
template <typename Integer>
long explicit_cast_to_long(const Integer& a);

template <>
inline long explicit_cast_to_long<regina::NInteger<true> >(
        const regina::NInteger<true>& a) {
    return a.longValue();
}
template <>
inline long explicit_cast_to_long<regina::NInteger<false> >(
        const regina::NInteger<false>& a) {
    return a.longValue();
}
} //namespace libnormaliz
#endif

namespace regina {

// Inline functions for NInteger

template <bool supportInfinity>
inline NInteger<supportInfinity>::NInteger() :
        small_(0), large_(0) {
}

template <bool supportInfinity>
inline NInteger<supportInfinity>::NInteger(int value) :
        small_(value), large_(0) {
}

template <bool supportInfinity>
inline NInteger<supportInfinity>::NInteger(unsigned value) :
        small_(value) {
    // Detect overflow.
    if (small_ < 0) {
        large_ = new mpz_t;
        mpz_init_set_ui(large_, value);
    } else
        large_ = 0;
}

template <bool supportInfinity>
inline NInteger<supportInfinity>::NInteger(long value) :
        small_(value), large_(0) {
}

template <bool supportInfinity>
inline NInteger<supportInfinity>::NInteger(unsigned long value) :
        small_(value) {
    // Detect overflow.
    if (small_ < 0) {
        large_ = new mpz_t;
        mpz_init_set_ui(large_, value);
    } else
        large_ = 0;
}

template <bool supportInfinity>
inline NInteger<supportInfinity>::NInteger(
        const NInteger<supportInfinity>& value) {
    if (value.isInfinite()) {
        InfinityPolicy<supportInfinity>::makeInfinite();
        large_ = 0;
    } else if (value.large_) {
        large_ = new mpz_t;
        mpz_init_set(large_, value.large_);
    } else {
        small_ = value.small_;
        large_ = 0;
    }
}

template <bool supportInfinity>
template <bool rhsSupportInfinity>
inline NInteger<supportInfinity>::NInteger(
        const NInteger<rhsSupportInfinity>& value) {
    if (value.isInfinite()) {
        InfinityPolicy<supportInfinity>::makeInfinite();
        large_ = 0;
    } else if (value.large_) {
        large_ = new mpz_t;
        mpz_init_set(large_, value.large_);
    } else {
        small_ = value.small_;
        large_ = 0;
    }
}

template <>
inline NInteger<true>::NInteger(bool, bool) : large_(0) {
    // The infinity constructor.
    InfinityPolicy<true>::makeInfinite();
}

template <bool supportInfinity>
inline NInteger<supportInfinity>::~NInteger() {
    if (large_) {
        mpz_clear(large_);
        delete large_;
    }
}

template <bool supportInfinity>
inline bool NInteger<supportInfinity>::isNative() const {
    return (! isInfinite()) && (! large_);
}

template <bool supportInfinity>
inline bool NInteger<supportInfinity>::isZero() const {
    return (! isInfinite()) &&
        (((! large_) && (! small_)) || (large_ && mpz_sgn(large_) == 0));
}

template <bool supportInfinity>
inline long NInteger<supportInfinity>::longValue() const {
    return (large_ ? mpz_get_si(large_) : small_);
}

template <bool supportInfinity>
inline NInteger<supportInfinity>&
        NInteger<supportInfinity>::operator =(
        const NInteger<supportInfinity>& value) {
    if (value.isInfinite()) {
        InfinityPolicy<supportInfinity>::makeInfinite();
        return *this;
    }
    InfinityPolicy<supportInfinity>::makeFinite();
    if (value.large_) {
        if (large_)
            mpz_set(large_, value.large_);
        else {
            large_ = new mpz_t;
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
template <bool rhsSupportInfinity>
inline NInteger<supportInfinity>&
        NInteger<supportInfinity>::operator =(
        const NInteger<rhsSupportInfinity>& value) {
    if (value.isInfinite()) {
        InfinityPolicy<supportInfinity>::makeInfinite();
        return *this;
    }
    InfinityPolicy<supportInfinity>::makeFinite();
    if (value.large_) {
        if (large_)
            mpz_set(large_, value.large_);
        else {
            large_ = new mpz_t;
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
inline NInteger<supportInfinity>&
        NInteger<supportInfinity>::operator =(int value) {
    InfinityPolicy<supportInfinity>::makeFinite();
    small_ = value;
    if (large_)
        clearLarge();
    return *this;
}

template <bool supportInfinity>
inline NInteger<supportInfinity>&
        NInteger<supportInfinity>::operator =(unsigned value) {
    InfinityPolicy<supportInfinity>::makeFinite();
    small_ = value;

    // Did we overflow?
    if (small_ < 0) {
        // Yes, it's an overflow: just a bit too large for a signed long
        // (literally).
        if (large_)
            mpz_set_ui(large_, value);
        else {
            large_ = new mpz_t;
            mpz_init_set_ui(large_, value);
        }
    } else if (large_) {
        // No overflow, but we must clear out any old large integer value.
        clearLarge();
    }
    return *this;
}

template <bool supportInfinity>
inline NInteger<supportInfinity>&
        NInteger<supportInfinity>::operator =(long value) {
    InfinityPolicy<supportInfinity>::makeFinite();
    small_ = value;
    if (large_)
        clearLarge();
    return *this;
}

template <bool supportInfinity>
inline NInteger<supportInfinity>&
        NInteger<supportInfinity>::operator =(unsigned long value) {
    InfinityPolicy<supportInfinity>::makeFinite();
    small_ = value;

    // Did we overflow?
    if (small_ < 0) {
        // Yes, it's an overflow: just a bit too large for a signed long
        // (literally).
        if (large_)
            mpz_set_ui(large_, value);
        else {
            large_ = new mpz_t;
            mpz_init_set_ui(large_, value);
        }
    } else if (large_) {
        // No overflow, but we must clear out any old large integer value.
        clearLarge();
    }
    return *this;
}

template <bool supportInfinity>
inline void NInteger<supportInfinity>::swap(
        NInteger<supportInfinity>& other) {
    // This should just work, since large_ is a pointer.
    InfinityPolicy<supportInfinity>::swap(other);
    std::swap(small_, other.small_);
    std::swap(large_, other.large_);
}

template <bool supportInfinity>
inline bool NInteger<supportInfinity>::operator ==(
        const NInteger<supportInfinity>& rhs) const {
    if (isInfinite() && rhs.isInfinite())
        return true;
    if (isInfinite() || rhs.isInfinite())
        return false;
    if (large_) {
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
inline bool NInteger<supportInfinity>::operator ==(long rhs) const {
    if (isInfinite())
        return false;
    if (large_)
        return (mpz_cmp_si_cpp(large_, rhs) == 0);
    else
        return (small_ == rhs);
}

template <bool supportInfinity>
inline bool NInteger<supportInfinity>::operator !=(
        const NInteger<supportInfinity>& rhs) const {
    if (isInfinite() && rhs.isInfinite())
        return false;
    if (isInfinite() || rhs.isInfinite())
        return true;
    if (large_) {
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
inline bool NInteger<supportInfinity>::operator !=(long rhs) const {
    if (isInfinite())
        return true;
    if (large_)
        return (mpz_cmp_si_cpp(large_, rhs) != 0);
    else
        return (small_ != rhs);
}

template <bool supportInfinity>
inline bool NInteger<supportInfinity>::operator <(
        const NInteger<supportInfinity>& rhs) const {
    if (isInfinite())
        return false;
    if (rhs.isInfinite())
        return true;
    if (large_) {
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
inline bool NInteger<supportInfinity>::operator <(long rhs) const {
    if (isInfinite())
        return false;
    if (large_)
        return (mpz_cmp_si_cpp(large_, rhs) < 0);
    else
        return (small_ < rhs);
}

template <bool supportInfinity>
inline bool NInteger<supportInfinity>::operator >(
        const NInteger<supportInfinity>& rhs) const {
    if (rhs.isInfinite())
        return false;
    if (isInfinite())
        return true;
    if (large_) {
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
inline bool NInteger<supportInfinity>::operator >(long rhs) const {
    if (isInfinite())
        return true;
    if (large_)
        return (mpz_cmp_si_cpp(large_, rhs) > 0);
    else
        return (small_ > rhs);
}

template <bool supportInfinity>
inline bool NInteger<supportInfinity>::operator <=(
        const NInteger<supportInfinity>& rhs) const {
    if (rhs.isInfinite())
        return true;
    if (isInfinite())
        return false;
    if (large_) {
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
inline bool NInteger<supportInfinity>::operator <=(long rhs) const {
    if (isInfinite())
        return false;
    if (large_)
        return (mpz_cmp_si_cpp(large_, rhs) <= 0);
    else
        return (small_ <= rhs);
}

template <bool supportInfinity>
inline bool NInteger<supportInfinity>::operator >=(
        const NInteger<supportInfinity>& rhs) const {
    if (isInfinite())
        return true;
    if (rhs.isInfinite())
        return false;
    if (large_) {
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
inline bool NInteger<supportInfinity>::operator >=(long rhs) const {
    if (isInfinite())
        return true;
    if (large_)
        return (mpz_cmp_si_cpp(large_, rhs) >= 0);
    else
        return (small_ >= rhs);
}

template <bool supportInfinity>
inline NInteger<supportInfinity>&
        NInteger<supportInfinity>::operator ++() {
    if (isInfinite())
        return *this;

    if (large_)
        mpz_add_ui(large_, large_, 1);
    else if (small_ != LONG_MAX)
        ++small_;
    else {
        // This is the point at which we overflow.
        forceLarge();
        mpz_add_ui(large_, large_, 1);
    }
    return *this;
}

template <bool supportInfinity>
inline NInteger<supportInfinity>
        NInteger<supportInfinity>::operator ++(int) {
    if (isInfinite())
        return *this;

    // Hrmph, just do the standard thing for now.
    // It's not clear how much microoptimisation will help..?
    NInteger<supportInfinity> ans(*this);
    ++(*this);
    return ans;
}

template <bool supportInfinity>
inline NInteger<supportInfinity>&
        NInteger<supportInfinity>::operator --() {
    if (isInfinite())
        return *this;

    if (large_)
        mpz_sub_ui(large_, large_, 1);
    else if (small_ != LONG_MIN)
        --small_;
    else {
        // This is the point at which we overflow.
        forceLarge();
        mpz_sub_ui(large_, large_, 1);
    }
    return *this;
}

template <bool supportInfinity>
inline NInteger<supportInfinity>
        NInteger<supportInfinity>::operator --(int) {
    if (isInfinite())
        return *this;

    // Hrmph, just do the standard thing for now.
    // It's not clear how much microoptimisation will help..?
    NInteger<supportInfinity> ans(*this);
    --(*this);
    return ans;
}

template <bool supportInfinity>
inline NInteger<supportInfinity>
        NInteger<supportInfinity>::operator +(
        const NInteger<supportInfinity>& other) const {
    if (isInfinite())
        return *this;
    if (other.isInfinite())
        return other;

    if (! other.large_)
        return (*this) + other.small_;

    NInteger<supportInfinity> ans;
    ans.large_ = new mpz_t;
    if (large_) {
        mpz_init(ans.large_);
        mpz_add(ans.large_, large_, other.large_);
    } else {
        mpz_init_set_si(ans.large_, small_);
        mpz_add(ans.large_, ans.large_, other.large_);
    }

    return ans;
}

template <bool supportInfinity>
inline NInteger<supportInfinity>
        NInteger<supportInfinity>::operator +(long other) const {
    if (isInfinite())
        return *this;

    // See operator += for an explanation of why all of this code works.
    if (! large_) {
        long ans = small_ + other;
        if (! ((((ans ^ small_) & (ans ^ other))) & LONG_MIN))
            return NInteger<supportInfinity>(ans); // No overflow.
    }

    // We have to use large integer arithmetic.
    NInteger<supportInfinity> ans;
    ans.large_ = new mpz_t;
    if (large_)
        mpz_init_set(ans.large_, large_);
    else
        mpz_init_set_si(ans.large_, small_);

    if (other >= 0)
        mpz_add_ui(ans.large_, ans.large_, other);
    else
        mpz_sub_ui(ans.large_, ans.large_, -other);

    return ans;
}

template <bool supportInfinity>
inline NInteger<supportInfinity>
        NInteger<supportInfinity>::operator -(
        const NInteger<supportInfinity>& other) const {
    if (isInfinite())
        return *this;
    if (other.isInfinite())
        return other;

    if (! other.large_)
        return (*this) - other.small_;

    NInteger<supportInfinity> ans;
    ans.large_ = new mpz_t;
    if (large_) {
        mpz_init(ans.large_);
        mpz_sub(ans.large_, large_, other.large_);
    } else {
        mpz_init_set_si(ans.large_, small_);
        mpz_sub(ans.large_, ans.large_, other.large_);
    }

    return ans;
}

template <bool supportInfinity>
inline NInteger<supportInfinity>
        NInteger<supportInfinity>::operator -(long other) const {
    if (isInfinite())
        return *this;

    // See operator -= for an explanation of why all of this code works.
    if (! large_) {
        long ans = small_ - other;
        if (! ((((small_ ^ other) & (ans ^ small_))) & LONG_MIN))
            return NInteger<supportInfinity>(ans); // No overflow.
    }

    // We have to use large integer arithmetic.
    NInteger<supportInfinity> ans;
    ans.large_ = new mpz_t;
    if (large_)
        mpz_init_set(ans.large_, large_);
    else
        mpz_init_set_si(ans.large_, small_);

    if (other >= 0)
        mpz_sub_ui(ans.large_, ans.large_, other);
    else
        mpz_add_ui(ans.large_, ans.large_, -other);

    return ans;
}

template <bool supportInfinity>
inline NInteger<supportInfinity>
        NInteger<supportInfinity>::operator *(
        const NInteger<supportInfinity>& other) const {
    if (isInfinite())
        return *this;
    if (other.isInfinite())
        return other;

    // Do the standard thing for now.
    NInteger<supportInfinity> ans(*this);
    return ans *= other;
}

template <bool supportInfinity>
inline NInteger<supportInfinity>
        NInteger<supportInfinity>::operator *(long other) const {
    if (isInfinite())
        return *this;

    // Do the standard thing for now.
    NInteger<supportInfinity> ans(*this);
    return ans *= other;
}

template <bool supportInfinity>
inline NInteger<supportInfinity>
        NInteger<supportInfinity>::operator /(
        const NInteger<supportInfinity>& other) const {
    if (isInfinite())
        return *this;
    if (other.isInfinite())
        return (long)0;
    if (other.isZero()) {
        NInteger<supportInfinity> ans;
        ans.InfinityPolicy<supportInfinity>::makeInfinite();
        return ans;
    }

    // Do the standard thing for now.
    NInteger<supportInfinity> ans(*this);
    return ans /= other;
}

template <bool supportInfinity>
inline NInteger<supportInfinity>
        NInteger<supportInfinity>::operator /(long other) const {
    if (isInfinite())
        return *this;
    if (other == 0) {
        NInteger<supportInfinity> ans;
        ans.InfinityPolicy<supportInfinity>::makeInfinite();
        return ans;
    }

    // Do the standard thing for now.
    NInteger<supportInfinity> ans(*this);
    return ans /= other;
}

template <bool supportInfinity>
inline NInteger<supportInfinity>
        NInteger<supportInfinity>::divExact(
        const NInteger<supportInfinity>& other) const {
    // Do the standard thing for now.
    NInteger<supportInfinity> ans(*this);
    return ans.divExact(other);
}

template <bool supportInfinity>
inline NInteger<supportInfinity>
        NInteger<supportInfinity>::divExact(long other) const {
    // Do the standard thing for now.
    NInteger<supportInfinity> ans(*this);
    return ans.divExact(other);
}

template <bool supportInfinity>
inline NInteger<supportInfinity>
        NInteger<supportInfinity>::operator %(
        const NInteger<supportInfinity>& other) const {
    // Do the standard thing for now.
    NInteger<supportInfinity> ans(*this);
    return ans %= other;
}

template <bool supportInfinity>
inline NInteger<supportInfinity>
        NInteger<supportInfinity>::operator %(long other) const {
    // Do the standard thing for now.
    NInteger<supportInfinity> ans(*this);
    return ans %= other;
}

template <bool supportInfinity>
inline NInteger<supportInfinity>
        NInteger<supportInfinity>::operator -() const {
    if (isInfinite())
        return *this;
    if (large_) {
        NInteger<supportInfinity> ans;
        ans.large_ = new mpz_t;
        mpz_init(ans.large_);
        mpz_neg(ans.large_, large_);
        return ans;
    } else if (small_ == LONG_MIN) {
        // Overflow, just.
        NInteger<supportInfinity> ans;
        ans.large_ = new mpz_t;
        mpz_init_set_si(ans.large_, small_);
        mpz_neg(ans.large_, ans.large_);
        return ans;
    } else
        return NInteger<supportInfinity>(-small_);
}

template <bool supportInfinity>
inline NInteger<supportInfinity>&
        NInteger<supportInfinity>::operator +=(
        const NInteger<supportInfinity>& other) {
    if (isInfinite())
        return *this;
    else if (other.isInfinite()) {
        InfinityPolicy<supportInfinity>::makeInfinite();
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
inline NInteger<supportInfinity>&
        NInteger<supportInfinity>::operator +=(long other) {
    if (isInfinite())
        return *this;
    if (! large_) {
        // Use native arithmetic if we can.
        // The following overflow test is taken from Hackers' Delight,
        // sec. 2-21.
        // Here we use (... & LONG_MIN) to extract the sign bit.
        long ans = small_ + other;
        if ((((ans ^ small_) & (ans ^ other))) & LONG_MIN) {
            // Boom.  It's an overflow.
            // Fall back to large integer arithmetic in the next block.
            forceLarge();
        } else {
            // All good: we're done.
            small_ = ans;
            return *this;
        }
    }

    // And now we're down to large integer arithmetic.
    // The following code should work even if other == LONG_MIN (in which case
    // -other == LONG_MIN also), since passing -other to mpz_sub_ui casts it
    // to an unsigned long (and gives it the correct positive value).
    if (other >= 0)
        mpz_add_ui(large_, large_, other);
    else
        mpz_sub_ui(large_, large_, -other);

    return *this;
}

template <bool supportInfinity>
inline NInteger<supportInfinity>&
        NInteger<supportInfinity>::operator -=(
        const NInteger<supportInfinity>& other) {
    if (isInfinite())
        return *this;
    else if (other.isInfinite()) {
        InfinityPolicy<supportInfinity>::makeInfinite();
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
inline NInteger<supportInfinity>&
        NInteger<supportInfinity>::operator -=(long other) {
    if (isInfinite())
        return *this;
    if (! large_) {
        // Use native arithmetic if we can.
        // The following overflow test is taken from Hackers' Delight,
        // sec. 2-21.
        // Here we use (... & LONG_MIN) to extract the sign bit.
        long ans = small_ - other;
        if ((((small_ ^ other) & (ans ^ small_))) & LONG_MIN) {
            // Boom.  It's an overflow.
            // Fall back to large integer arithmetic in the next block.
            forceLarge();
        } else {
            // All good: we're done.
            small_ = ans;
            return *this;
        }
    }

    // And now we're down to large integer arithmetic.
    // The following code should work even if other == LONG_MIN (in which case
    // -other == LONG_MIN also), since passing -other to mpz_add_ui casts it
    // to an unsigned long (and gives it the correct positive value).
    if (other >= 0)
        mpz_sub_ui(large_, large_, other);
    else
        mpz_add_ui(large_, large_, -other);

    return *this;
}

template <bool supportInfinity>
inline NInteger<supportInfinity>&
        NInteger<supportInfinity>::operator *=(
        const NInteger<supportInfinity>& other) {
    if (isInfinite())
        return *this;
    else if (other.isInfinite()) {
        InfinityPolicy<supportInfinity>::makeInfinite();
        return *this;
    }
    if (large_) {
        if (other.large_)
            mpz_mul(large_, large_, other.large_);
        else
            mpz_mul_si(large_, large_, other.small_);
    } else if (other.large_) {
        large_ = new mpz_t;
        mpz_init(large_);
        mpz_mul_si(large_, other.large_, small_);
    } else {
        long ans = small_ * other.small_;
        // From Hacker's Delight, sec. 2-12:
        // Overflow iff the following conditions hold:
        if ((other.small_ < 0 && small_ == LONG_MIN) ||
                (other.small_ != 0 && ans / other.small_ != small_)) {
            // Overflow.
            large_ = new mpz_t;
            mpz_init_set_si(large_, small_);
            mpz_mul_si(large_, large_, other.small_);
        } else
            small_ = ans;
    }
    return *this;
}

template <bool supportInfinity>
inline NInteger<supportInfinity>&
        NInteger<supportInfinity>::operator *=(long other) {
    if (isInfinite())
        return *this;
    if (large_)
        mpz_mul_si(large_, large_, other);
    else {
        long ans = small_ * other;
        // From Hacker's Delight, sec. 2-12:
        // Overflow iff the following conditions hold:
        if ((other < 0 && small_ == LONG_MIN) ||
                (other != 0 && ans / other != small_)) {
            // Overflow.
            large_ = new mpz_t;
            mpz_init_set_si(large_, small_);
            mpz_mul_si(large_, large_, other);
        } else
            small_ = ans;
    }
    return *this;
}

template <bool supportInfinity>
inline NInteger<supportInfinity>&
        NInteger<supportInfinity>::operator /=(
        const NInteger<supportInfinity>& other) {
    if (isInfinite())
        return *this;
    if (other.isInfinite())
        return (*this = 0);
    if (other.isZero()) {
        InfinityPolicy<supportInfinity>::makeInfinite();
        return *this;
    }
    if (other.large_) {
        if (large_) {
            mpz_tdiv_q(large_, large_, other.large_);
            return *this;
        }
        // This is a native C/C++ long.
        // One of four things must happen:
        // (i) |other| > |this|, in which case the result = 0;
        // (ii) this = LONG_MIN and OTHER = -1, in which case the result
        // is the large integer -LONG_MIN;
        // (iii) this = LONG_MIN and OTHER is the large integer -LONG_MIN,
        // in which case the result = -1;
        // (iv) other can be converted to a native long, and the result
        // is a native long also.
        //
        // Deal with the problematic LONG_MIN case first.
        if (small_ == LONG_MIN) {
            if (! mpz_cmp_ui(other.large_,
                    LONG_MIN /* casting to unsigned makes this -LONG_MIN */)) {
                small_ = -1;
                return *this;
            }
            if (! mpz_cmp_si(other.large_, -1)) {
                // The result is -LONG_MIN, which requires large integers.
                // Reduce other while we're at it.
                const_cast<NInteger<supportInfinity>&>(other).forceReduce();
                large_ = new mpz_t;
                mpz_init_set_si(large_, LONG_MIN);
                mpz_neg(large_, large_);
                return *this;
            }
            if (mpz_cmp_ui(other.large_,
                    LONG_MIN /* cast to ui makes this -LONG_MIN */) > 0 ||
                    mpz_cmp_si(other.large_, LONG_MIN) < 0) {
                small_ = 0;
                return *this;
            }
            // other is in [ LONG_MIN, -LONG_MIN ) \ {-1}.
            // Reduce it and use native arithmetic.
            const_cast<NInteger<supportInfinity>&>(other).forceReduce();
            small_ /= other.small_;
            return *this;
        }

        // From here we have this in ( LONG_MIN, -LONG_MIN ).
        if (small_ >= 0) {
            if (mpz_cmp_si(other.large_, small_) > 0 ||
                    mpz_cmp_si(other.large_, -small_) < 0) {
                small_ = 0;
                return *this;
            }
        } else {
            // We can negate, since small_ != LONG_MIN.
            if (mpz_cmp_si(other.large_, -small_) > 0 ||
                    mpz_cmp_si(other.large_, small_) < 0) {
                small_ = 0;
                return *this;
            }
        }

        // We can do this all in native longs from here.
        // Opportunistically reduce other, since we know we can.
        const_cast<NInteger<supportInfinity>&>(other).forceReduce();
        small_ /= other.small_;
        return *this;
    } else
        return (*this) /= other.small_;
}

template <bool supportInfinity>
inline NInteger<supportInfinity>&
        NInteger<supportInfinity>::operator /=(long other) {
    if (isInfinite())
        return *this;
    if (other == 0) {
        InfinityPolicy<supportInfinity>::makeInfinite();
        return *this;
    }
    if (large_) {
        if (other >= 0)
            mpz_tdiv_q_ui(large_, large_, other);
        else {
            // The cast to (unsigned long) makes this correct even if
            // other = LONG_MIN.
            mpz_tdiv_q_ui(large_, large_, - other);
            mpz_neg(large_, large_);
        }
    } else if (small_ == LONG_MIN && other == -1) {
        // This is the special case where we must switch from native to
        // large integers.
        large_ = new mpz_t;
        mpz_init_set_si(large_, LONG_MIN);
        mpz_neg(large_, large_);
    } else {
        // We can do this entirely in native arithmetic.
        small_ /= other;
    }
    return *this;
}

template <bool supportInfinity>
inline NInteger<supportInfinity>&
        NInteger<supportInfinity>::divByExact(
        const NInteger<supportInfinity>& other) {
    if (other.large_) {
        if (large_) {
            mpz_divexact(large_, large_, other.large_);
            return *this;
        }
        // This is a native C/C++ long.
        // Because we are guaranteed other | this, it follows that
        // other must likewise fit within a native long, or else
        // this == LONG_MIN and other == -LONG_MIN.
        // It also follows that the result must fit within a native long,
        // or else this == LONG_MIN and other == -1.
        if (small_ == LONG_MIN) {
            if (! mpz_cmp_ui(other.large_,
                    LONG_MIN /* casting to unsigned makes this -LONG_MIN */)) {
                // The result is -1, since we have LONG_MIN / -LONG_MIN.
                small_ = -1;
                return *this;
            }

            // At this point we know that other fits within a native long.
            // Opportunistically reduce its representation.
            const_cast<NInteger<supportInfinity>&>(other).forceReduce();

            if (other.small_ == -1) {
                // The result is -LONG_MIN, which requires large integers.
                large_ = new mpz_t;
                mpz_init_set_si(large_, LONG_MIN);
                mpz_neg(large_, large_);
            } else {
                // The result will fit within a native long also.
                small_ /= other.small_;
            }
            return *this;
        }

        // Here we know that other always fits within a native long,
        // and so does the result.
        // Opportunisticaly reduce the representation of other, since
        // we know we can.
        const_cast<NInteger<supportInfinity>&>(other).forceReduce();
        small_ /= other.small_;
        return *this;
    } else {
        // other is already a native int.
        // Use the native version of this routine instead.
        return divByExact(other.small_);
    }
}

template <bool supportInfinity>
inline NInteger<supportInfinity>&
        NInteger<supportInfinity>::divByExact(long other) {
    if (large_) {
        if (other >= 0)
            mpz_divexact_ui(large_, large_, other);
        else {
            // The cast to (unsigned long) makes this correct even if
            // other = LONG_MIN.
            mpz_divexact_ui(large_, large_, - other);
            mpz_neg(large_, large_);
        }
    } else if (small_ == LONG_MIN && other == -1) {
        // This is the special case where we must switch from native to
        // large integers.
        large_ = new mpz_t;
        mpz_init_set_si(large_, LONG_MIN);
        mpz_neg(large_, large_);
    } else {
        // We can do this entirely in native arithmetic.
        small_ /= other;
    }
    return *this;
}

template <bool supportInfinity>
inline NInteger<supportInfinity>&
        NInteger<supportInfinity>::operator %=(
        const NInteger<supportInfinity>& other) {
    if (other.large_) {
        if (large_) {
            mpz_tdiv_r(large_, large_, other.large_);
            return *this;
        }

        // We fit into a native long.  Either:
        // (i) |other| > |this|, in which case the result is just this;
        // (ii) |other| == |this|, in which case the result is 0;
        // (iii) |other| < |this|, in which case we can convert
        // everything to native C/C++ integer arithmetic.
        int res = (small_ >= 0 ?
            mpz_cmp_si(other.large_, small_) :
            mpz_cmp_ui(other.large_, - small_) /* ui cast makes this work
                                                 even if small_ = LONG_MIN */);
        if (res > 0)
            return *this;
        if (res == 0) {
            small_ = 0;
            return *this;
        }

        res = (small_ >= 0 ?
            mpz_cmp_si(other.large_, - small_) :
            mpz_cmp_ui(other.large_, small_));

        if (res < 0)
            return *this;
        if (res == 0) {
            small_ = 0;
            return *this;
        }

        // Everything can be made native integer arithmetic.
        // Opportunistically reduce other while we're at it.
        const_cast<NInteger<supportInfinity>&>(other).forceReduce();
        small_ %= other.small_;
        return *this;
    } else
        return (*this) %= other.small_;
}

template <bool supportInfinity>
inline NInteger<supportInfinity>&
        NInteger<supportInfinity>::operator %=(long other) {
    // Since |result| < |other|, whatever happens we can fit the result
    // into a native C/C++ long.
    if (large_) {
        // We can safely cast other to an unsigned long, because the rounding
        // rules imply that (this % LONG_MIN) == (this % -LONG_MIN).
        mpz_tdiv_r_ui(large_, large_, other >= 0 ? other : -other);
        forceReduce();
    } else {
        // All native arithmetic from here.
        small_ %= other;
    }
    return *this;
}

template <bool supportInfinity>
inline void NInteger<supportInfinity>::negate() {
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
inline NInteger<supportInfinity> NInteger<supportInfinity>::abs()
        const {
    if (isInfinite())
        return *this;
    if (large_) {
        NInteger<supportInfinity> ans;
        ans.large_ = new mpz_t;
        mpz_init_set(ans.large_, large_);
        mpz_abs(ans.large_, large_);
        return ans;
    } else if (small_ == LONG_MIN) {
        // Overflow, just.
        NInteger<supportInfinity> ans;
        ans.large_ = new mpz_t;
        mpz_init_set_si(ans.large_, small_);
        mpz_neg(ans.large_, ans.large_);
        return ans;
    } else
        return NInteger<supportInfinity>(small_ >= 0 ? small_ : - small_);
}

template <bool supportInfinity>
inline NInteger<supportInfinity> operator +(long lhs,
        const NInteger<supportInfinity>& rhs) {
    return rhs + lhs;
}

template <bool supportInfinity>
inline NInteger<supportInfinity> operator *(long lhs,
        const NInteger<supportInfinity>& rhs) {
    return rhs * lhs;
}

template <bool supportInfinity>
inline void NInteger<supportInfinity>::setRaw(mpz_srcptr fromData) {
    InfinityPolicy<supportInfinity>::makeFinite();
    if (! large_) {
        large_ = new mpz_t;
        mpz_init_set(large_, fromData);
    } else {
        mpz_set(large_, fromData);
    }
}

template <bool supportInfinity>
inline mpz_srcptr NInteger<supportInfinity>::rawData() const {
    // Cast away the const, since we are not changing the mathematical value.
    // We are, however, bulking up the representation.
    const_cast<NInteger<supportInfinity>&>(*this).makeLarge();
    return large_;
}

template <bool supportInfinity>
inline mpz_ptr NInteger<supportInfinity>::rawData() {
    makeLarge();
    return large_;
}

template <bool supportInfinity>
inline void NInteger<supportInfinity>::makeLarge() {
    if (! large_)
        forceLarge();
}

template <bool supportInfinity>
inline void NInteger<supportInfinity>::tryReduce() {
    if (large_ && mpz_cmp_si(large_, LONG_MAX) <= 0 &&
            mpz_cmp_si(large_, LONG_MIN) >= 0)
        forceReduce();
}

template <bool supportInfinity>
inline void NInteger<supportInfinity>::forceLarge() {
    large_ = new mpz_t;
    mpz_init_set_si(large_, small_);
}

template <bool supportInfinity>
inline void NInteger<supportInfinity>::clearLarge() {
    mpz_clear(large_);
    delete large_;
    large_ = 0;
}

template <bool supportInfinity>
inline void NInteger<supportInfinity>::forceReduce() {
    small_ = mpz_get_si(large_);
    mpz_clear(large_);
    delete large_;
    large_ = 0;
}

} // namespace regina

#endif

