
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

#ifndef __NLAZYINTEGER_H
#ifndef __DOXYGEN
#define __NLAZYINTEGER_H
#endif

/*! \file maths/nlazyinteger.h
 *  \brief A fast arbitrary precision integer class that uses native integer
 *  arithmetic until otherwise necessary.
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
 * A fast class that represents arbitrary precision integers, but uses
 * fast native integer arithmetic wherever possible.  Specifically, all
 * operations will use the native C/C++ long type until a potential
 * overflow is detected, at which point they will switch to using the
 * GNU multiple precision arithmetic library instead.
 *
 * Calculations are always guaranteed to be exact.
 *
 * Unlike the NLargeInteger class (which never uses native integers),
 * this class does not support infinity.
 *
 * All routines in this class are thread-safe.
 *
 * This class was inspired by the (much more complex and powerful) lazy exact
 * arithmetic in CGAL.  Thanks to Menelaos Karavelas for encouraging me to
 * take another look at these ideas.
 *
 * \testpart
 */
class REGINA_API NLazyInteger {
    public:
        static const NLazyInteger zero;
            /**< Globally available zero. */
        static const NLazyInteger one;
            /**< Globally available one. */

    private:
        long small_;
            /**< Contains the native representation of this integer, if
                 we are still using native representations (i.e., if
                 large_ is null).  If we are using GMP large integer
                 representations, this native integer is ignored (and
                 may be set to anything). */
        mpz_ptr large_;
            /**< 0 if we are using native representations, or a pointer to the
                 full GMP large integer if we are now using these instead.
                 We require that, whenever this pointer is non-null, the
                 corresponding GMP large integer is initialised. */

    public:
        /**
         * Initialises this integer to zero.
         */
        NLazyInteger();
        /**
         * Initialises this integer to the given value.
         *
         * \ifacespython In Python, the only native-integer constructor
         * is NLazyInteger(long).
         *
         * @param value the new value of this integer.
         */
        NLazyInteger(int value);
        /**
         * Initialises this integer to the given value.
         *
         * \ifacespython In Python, the only native-integer constructor
         * is NLazyInteger(long).
         *
         * @param value the new value of this integer.
         */
        NLazyInteger(unsigned value);
        /**
         * Initialises this integer to the given value.
         *
         * \ifacespython In Python, this is the only native-integer
         * constructor available.
         *
         * @param value the new value of this integer.
         */
        NLazyInteger(long value);
        /**
         * Initialises this integer to the given value.
         *
         * \ifacespython In Python, the only native-integer constructor
         * is NLazyInteger(long).
         *
         * @param value the new value of this integer.
         */
        NLazyInteger(unsigned long value);
        /**
         * Initialises this integer to the given value.
         *
         * @param value the new value of this integer.
         */
        NLazyInteger(const NLazyInteger& value);
        /**
         * Initialises this integer to the given value which is
         * represented as a string of digits in a given base.
         *
         * If not specified, the base defaults to 10.
         * If the given base is zero, the base will be automatically
         * determined.  If the given string begins with \c 0x,
         * the base will be assumed to be 16.  Otherwise, if the string
         * begins with \c 0, the base will be assumed to be 8.
         * Otherwise it will be taken as base 10.
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
        NLazyInteger(const char* value, int base = 10, bool* valid = 0);
        /**
         * Initialises this integer to the given value which is
         * represented as a string of digits in a given base.
         *
         * If not specified, the base defaults to 10.
         * If the given base is zero, the base will be automatically
         * determined.  If the given string begins with \c 0x,
         * the base will be assumed to be 16.  Otherwise, if the string
         * begins with \c 0, the base will be assumed to be 8.
         * Otherwise it will be taken as base 10.
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
        NLazyInteger(const std::string& value, int base = 10,
                bool* valid = 0);
        /**
         * Destroys this integer.
         */
        ~NLazyInteger();

        /**
         * Returns whether we are currently working with a native C/C++
         * long, or whether we have switched to GMP large integer arithmetic
         * for this integer.
         *
         * @return \c true if and only if we are stil using a native C/C++ long.
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
         * Returns the value of this integer as a long.
         *
         * If this integer is outside the range of a long, the result is
         * unpredictable.  If it is within the range of a long however,
         * it will give correct results regardless of whether the
         * underlying representation is a native or large integer.
         *
         * @return the value of this integer.
         */
        long longValue() const;
        /**
         * Returns the value of this integer as a string in the given
         * base.  If not specified, the base defaults to 10.
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
        NLazyInteger& operator =(const NLazyInteger& value);
        /**
         * Sets this integer to the given value.
         *
         * @param value the new value of this integer.
         * @return a reference to this integer with its new value.
         */
        NLazyInteger& operator =(int value);
        /**
         * Sets this integer to the given value.
         *
         * @param value the new value of this integer.
         * @return a reference to this integer with its new value.
         */
        NLazyInteger& operator =(unsigned value);
        /**
         * Sets this integer to the given value.
         *
         * @param value the new value of this integer.
         * @return a reference to this integer with its new value.
         */
        NLazyInteger& operator =(long value);
        /**
         * Sets this integer to the given value.
         *
         * @param value the new value of this integer.
         * @return a reference to this integer with its new value.
         */
        NLazyInteger& operator =(unsigned long value);
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
        NLazyInteger& operator =(const char* value);
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
        NLazyInteger& operator =(const std::string& value);
        /**
         * Swaps the values of this and the given integer.
         *
         * @param other the integer whose value will be swapped with
         * this.
         */
        void swap(NLazyInteger& other);

        /**
         * Determines if this is equal to the given integer.
         *
         * @param rhs the integer with which this will be compared.
         * @return \c true if and only if this and the given integer are
         * equal.
         */
        bool operator ==(const NLazyInteger& rhs) const;
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
        bool operator !=(const NLazyInteger& rhs) const;
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
        bool operator <(const NLazyInteger& rhs) const;
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
        bool operator >(const NLazyInteger& rhs) const;
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
        bool operator <=(const NLazyInteger& rhs) const;
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
        bool operator >=(const NLazyInteger& rhs) const;
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
        NLazyInteger& operator ++();

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
        NLazyInteger operator ++(int);

        /**
         * The predecrement operator.
         * This operator decrements this integer by one, and returns a
         * reference to the integer \e after the decrement.
         *
         * \ifacespython Not available.
         *
         * @return a reference to this integer after the decrement.
         */
        NLazyInteger& operator --();

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
        NLazyInteger operator --(int);

        /**
         * Adds this to the given integer and returns the result.
         * This integer is not changed.
         *
         * @param other the integer to add to this integer.
         * @return the sum \a this plus \a other.
         */
        NLazyInteger operator +(const NLazyInteger& other) const;
        /**
         * Adds this to the given integer and returns the result.
         * This integer is not changed.
         *
         * @param other the integer to add to this integer.
         * @return the sum \a this plus \a other.
         */
        NLazyInteger operator +(long other) const;
        /**
         * Subtracts the given integer from this and returns the result.
         * This integer is not changed.
         *
         * @param other the integer to subtract from this integer.
         * @return the difference \a this minus \a other.
         */
        NLazyInteger operator -(const NLazyInteger& other) const;
        /**
         * Subtracts the given integer from this and returns the result.
         * This integer is not changed.
         *
         * @param other the integer to subtract from this integer.
         * @return the difference \a this minus \a other.
         */
        NLazyInteger operator -(long other) const;
        /**
         * Multiplies this by the given integer and returns the
         * result.
         * This integer is not changed.
         *
         * @param other the integer to multiply by this integer.
         * @return the product \a this times \a other.
         */
        NLazyInteger operator *(const NLazyInteger& other) const;
        /**
         * Multiplies this by the given integer and returns the
         * result.
         * This integer is not changed.
         *
         * @param other the integer to multiply by this integer.
         * @return the product \a this times \a other.
         */
        NLazyInteger operator *(long other) const;
        /**
         * Divides this by the given integer and returns the result.
         * The result will be truncated to an integer, i.e. rounded
         * towards zero.
         * This integer is not changed.
         *
         * If \a other is known to divide this integer exactly,
         * divExact() should be used instead.
         *
         * \pre \a other is non-zero.
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
        NLazyInteger operator /(const NLazyInteger& other) const;
        /**
         * Divides this by the given integer and returns the result.
         * The result will be truncated to an integer, i.e. rounded
         * towards zero.
         * This integer is not changed.
         *
         * If \a other is known to divide this integer exactly,
         * divExact() should be used instead.
         *
         * \pre \a other is non-zero.
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
        NLazyInteger operator /(long other) const;
        /**
         * Divides this by the given integer and returns the result.
         * This can only be used when the given integer divides into
         * this exactly, and for large integers can be much faster than
         * ordinary division.  This integer is not changed.
         *
         * \pre The given integer divides exactly into
         * this integer, i.e. \a this divided by \a other is an integer.
         * \pre \a other is not zero.
         *
         * @param other the integer to divide this by.
         * @return the quotient \a this divided by \a other.
         */
        NLazyInteger divExact(const NLazyInteger& other) const;
        /**
         * Divides this by the given integer and returns the result.
         * This can only be used when the given integer divides into
         * this exactly, and for large integers can be much faster than
         * ordinary division.  This integer is not changed.
         *
         * \pre The given integer divides exactly into
         * this integer, i.e. \a this divided by \a other is an integer.
         * \pre \a other is not zero.
         *
         * @param other the integer to divide this by.
         * @return the quotient \a this divided by \a other.
         */
        NLazyInteger divExact(long other) const;
        /**
         * Determines the remainder when this integer is divided by the
         * given integer.  If non-zero, the result will have the same sign
         * as this integer.
         * This integer is not changed.
         *
         * \pre \a other is not zero.
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
        NLazyInteger operator %(const NLazyInteger& other) const;
        /**
         * Determines the remainder when this integer is divided by the
         * given integer.  If non-zero, the result will have the same sign
         * as this integer.
         * This integer is not changed.
         *
         * \pre \a other is not zero.
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
        NLazyInteger operator %(long other) const;
        /**
         * Determines the negative of this integer.
         * This integer is not changed.
         *
         * @return the negative of this integer.
         */
        NLazyInteger operator -() const;

        /**
         * Adds the given integer to this.
         * This integer is changed to reflect the result.
         *
         * @param other the integer to add to this integer.
         * @return a reference to this integer with its new value.
         */
        NLazyInteger& operator +=(const NLazyInteger& other);
        /**
         * Adds the given integer to this.
         * This integer is changed to reflect the result.
         *
         * @param other the integer to add to this integer.
         * @return a reference to this integer with its new value.
         */
        NLazyInteger& operator +=(long other);
        /**
         * Subtracts the given integer from this.
         * This integer is changed to reflect the result.
         *
         * @param other the integer to subtract from this integer.
         * @return a reference to this integer with its new value.
         */
        NLazyInteger& operator -=(const NLazyInteger& other);
        /**
         * Subtracts the given integer from this.
         * This integer is changed to reflect the result.
         *
         * @param other the integer to subtract from this integer.
         * @return a reference to this integer with its new value.
         */
        NLazyInteger& operator -=(long other);
        /**
         * Multiplies the given integer by this.
         * This integer is changed to reflect the result.
         *
         * @param other the integer to multiply with this integer.
         * @return a reference to this integer with its new value.
         */
        NLazyInteger& operator *=(const NLazyInteger& other);
        /**
         * Multiplies the given integer by this.
         * This integer is changed to reflect the result.
         *
         * @param other the integer to multiply with this integer.
         * @return a reference to this integer with its new value.
         */
        NLazyInteger& operator *=(long other);
        /**
         * Divides this by the given integer.
         * The result will be truncated to an integer, i.e. rounded
         * towards zero.
         * This integer is changed to reflect the result.
         *
         * If \a other is known to divide this integer exactly,
         * divByExact() should be used instead.
         *
         * \pre \a other is non-zero.
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
        NLazyInteger& operator /=(const NLazyInteger& other);
        /**
         * Divides this by the given integer.
         * The result will be truncated to an integer, i.e. rounded
         * towards zero.
         * This integer is changed to reflect the result.
         *
         * If \a other is known to divide this integer exactly,
         * divByExact() should be used instead.
         *
         * \pre \a other is non-zero.
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
        NLazyInteger& operator /=(long other);
        /**
         * Divides this by the given integer.
         * This can only be used when the given integer divides into
         * this exactly, and for large integers this is much faster than
         * ordinary division.  This integer is changed to reflect the result.
         *
         * \pre The given integer divides exactly into
         * this integer, i.e. \a this divided by \a other is an integer.
         * \pre \a other is not zero.
         *
         * @param other the integer to divide this by.
         * @return a reference to this integer with its new value.
         */
        NLazyInteger& divByExact(const NLazyInteger& other);
        /**
         * Divides this by the given integer.
         * This can only be used when the given integer divides into
         * this exactly, and for large integers this is much faster than
         * ordinary division.  This integer is changed to reflect the result.
         *
         * \pre The given integer divides exactly into
         * this integer, i.e. \a this divided by \a other is an integer.
         * \pre \a other is not zero.
         *
         * @param other the integer to divide this by.
         * @return a reference to this integer with its new value.
         */
        NLazyInteger& divByExact(long other);
        /**
         * Reduces this integer modulo the given integer.
         * If non-zero, the result will have the same sign as the original
         * value of this integer.
         * This integer is changed to reflect the result.
         *
         * \pre \a other is not zero.
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
        NLazyInteger& operator %=(const NLazyInteger& other);
        /**
         * Reduces this integer modulo the given integer.
         * If non-zero, the result will have the same sign as the original
         * value of this integer.
         * This integer is changed to reflect the result.
         *
         * \pre \a other is not zero.
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
        NLazyInteger& operator %=(long other);
        /**
         * Negates this integer.
         * This integer is changed to reflect the result.
         */
        void negate();

        /**
         * Determines the absolute value of this integer.
         * This integer is not changed.
         *
         * @return the absolute value of this integer.
         */
        NLazyInteger abs() const;
        /**
         * Determines the greatest common divisor of this and the given
         * integer.  This integer is not changed.
         *
         * The result is guaranteed to be non-negative.  As a
         * special case, gcd(0,0) is considered to be zero.
         *
         * @param other the integer whose greatest common divisor with
         * this will be found.
         * @return the greatest common divisor of this and the given
         * integer.
         */
        NLazyInteger gcd(const NLazyInteger& other) const;
        /**
         * Determines the lowest common multiple of this and the given
         * integer.  This integer is not changed.
         *
         * Note that the result might possibly be negative.
         *
         * @param other the integer whose lowest common multiple with
         * this will be found.
         * @return the lowest common multiple of this and the given
         * integer.
         */
        NLazyInteger lcm(const NLazyInteger& other) const;

        /**
         * Converts this integer to use a GMP large integer representation,
         * regardless of whether this is actually necessary.  The contents
         * of this integer will be preserved.
         *
         * It does not matter which kind of representation this integer
         * is currently using.
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
         */
        void tryReduce();

    private:
        /**
         * Converts this integer from a native C/C++ long representation
         * into a GMP large integer representation.
         *
         * The contents of \a small will be copied into \a large.
         *
         * \pre \a large_ is null (i.e., we are indeed using a native
         * C/C++ long representation at present).
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
         */
        void forceReduce();

    friend std::ostream& operator << (std::ostream& out,
        const NLazyInteger& large);

    friend class NLargeInteger;
};

/**
 * Writes the given integer to the given output stream.
 *
 * @param out the output stream to which to write.
 * @param large the integer to write.
 * @return a reference to \a out.
 */
REGINA_API std::ostream& operator << (std::ostream& out, const NLazyInteger& i);

/**
 * Adds the given native integer to the given large integer.
 *
 * \ifacespython Not available.
 *
 * @param lhs the native integer to add.
 * @param rhs the large integer to add.
 * @return the sum \a lhs plus \a rhs.
 */
REGINA_API NLazyInteger operator + (long lhs, const NLazyInteger& rhs);

/**
 * Multiplies the given native integer with the given large integer.
 *
 * \ifacespython Not available.
 *
 * @param lhs the native integer to multiply.
 * @param rhs the large integer to multiply.
 * @return the product \a lhs times \a rhs.
 */
REGINA_API NLazyInteger operator * (long lhs, const NLazyInteger& rhs);

/*@}*/

} // namespace regina

#ifndef __DOXYGEN
namespace libnormaliz {
/**
 * An explicit integer cast function, for compatibility with Normaliz.
 *
 * @param a an instance of some arbitrary integer type.
 * @return the given integer, cast as a native long.
 */
template<typename Integer>
long explicit_cast_to_long(const Integer& a);

template<>
inline long explicit_cast_to_long<regina::NLazyInteger>(
        const regina::NLazyInteger& a) {
	return a.longValue();
}
} //namespace libnormaliz
#endif

namespace regina {

// Inline functions for NLazyInteger

inline NLazyInteger::NLazyInteger() : small_(0), large_(0) {
}

inline NLazyInteger::NLazyInteger(int value) : small_(value), large_(0) {
}

inline NLazyInteger::NLazyInteger(unsigned value) : small_(value) {
    // Detect overflow.
    if (small_ < 0) {
        large_ = new mpz_t;
        mpz_init_set_ui(large_, value);
    } else
        large_ = 0;
}

inline NLazyInteger::NLazyInteger(long value) : small_(value), large_(0) {
}

inline NLazyInteger::NLazyInteger(unsigned long value) : small_(value) {
    // Detect overflow.
    if (small_ < 0) {
        large_ = new mpz_t;
        mpz_init_set_ui(large_, value);
    } else
        large_ = 0;
}

inline NLazyInteger::NLazyInteger(const NLazyInteger& value) {
    if (value.large_) {
        large_ = new mpz_t;
        mpz_init_set(large_, value.large_);
    } else {
        small_ = value.small_;
        large_ = 0;
    }
}

inline NLazyInteger::~NLazyInteger() {
    if (large_) {
        mpz_clear(large_);
        delete large_;
    }
}

inline bool NLazyInteger::isNative() const {
    return (! large_);
}

inline bool NLazyInteger::isZero() const {
    return ((! large_) && (! small_)) || (large_ && mpz_sgn(large_) == 0);
}

inline long NLazyInteger::longValue() const {
    return (large_ ? mpz_get_si(large_) : small_);
}

inline NLazyInteger& NLazyInteger::operator =(const NLazyInteger& value) {
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

inline NLazyInteger& NLazyInteger::operator =(int value) {
    small_ = value;
    if (large_)
        clearLarge();
    return *this;
}

inline NLazyInteger& NLazyInteger::operator =(unsigned value) {
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

inline NLazyInteger& NLazyInteger::operator =(long value) {
    small_ = value;
    if (large_)
        clearLarge();
    return *this;
}

inline NLazyInteger& NLazyInteger::operator =(unsigned long value) {
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

inline void NLazyInteger::swap(NLazyInteger& other) {
    // This should just work, since large_ is a pointer.
    std::swap(small_, other.small_);
    std::swap(large_, other.large_);
}

inline bool NLazyInteger::operator ==(const NLazyInteger& rhs) const {
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

inline bool NLazyInteger::operator ==(long rhs) const {
    if (large_)
        return (mpz_cmp_si_cpp(large_, rhs) == 0);
    else
        return (small_ == rhs);
}

inline bool NLazyInteger::operator !=(const NLazyInteger& rhs) const {
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

inline bool NLazyInteger::operator !=(long rhs) const {
    if (large_)
        return (mpz_cmp_si_cpp(large_, rhs) != 0);
    else
        return (small_ != rhs);
}

inline bool NLazyInteger::operator <(const NLazyInteger& rhs) const {
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

inline bool NLazyInteger::operator <(long rhs) const {
    if (large_)
        return (mpz_cmp_si_cpp(large_, rhs) < 0);
    else
        return (small_ < rhs);
}

inline bool NLazyInteger::operator >(const NLazyInteger& rhs) const {
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

inline bool NLazyInteger::operator >(long rhs) const {
    if (large_)
        return (mpz_cmp_si_cpp(large_, rhs) > 0);
    else
        return (small_ > rhs);
}

inline bool NLazyInteger::operator <=(const NLazyInteger& rhs) const {
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

inline bool NLazyInteger::operator <=(long rhs) const {
    if (large_)
        return (mpz_cmp_si_cpp(large_, rhs) <= 0);
    else
        return (small_ <= rhs);
}

inline bool NLazyInteger::operator >=(const NLazyInteger& rhs) const {
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

inline bool NLazyInteger::operator >=(long rhs) const {
    if (large_)
        return (mpz_cmp_si_cpp(large_, rhs) >= 0);
    else
        return (small_ >= rhs);
}

inline NLazyInteger& NLazyInteger::operator ++() {
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

inline NLazyInteger NLazyInteger::operator ++(int) {
    // Hrmph, just do the standard thing for now.
    // It's not clear how much microoptimisation will help..?
    NLazyInteger ans(*this);
    ++(*this);
    return ans;
}

inline NLazyInteger& NLazyInteger::operator --() {
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

inline NLazyInteger NLazyInteger::operator --(int) {
    // Hrmph, just do the standard thing for now.
    // It's not clear how much microoptimisation will help..?
    NLazyInteger ans(*this);
    --(*this);
    return ans;
}

inline NLazyInteger NLazyInteger::operator +(const NLazyInteger& other)
        const {
    if (! other.large_)
        return (*this) + other.small_;

    NLazyInteger ans;
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

inline NLazyInteger NLazyInteger::operator +(long other) const {
    // See operator += for an explanation of why all of this code works.
    if (! large_) {
        long ans = small_ + other;
        if (! ((((ans ^ small_) & (ans ^ other))) & LONG_MIN))
            return NLazyInteger(ans); // No overflow.
    }

    // We have to use large integer arithmetic.
    NLazyInteger ans;
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

inline NLazyInteger NLazyInteger::operator -(const NLazyInteger& other)
        const {
    if (! other.large_)
        return (*this) - other.small_;

    NLazyInteger ans;
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

inline NLazyInteger NLazyInteger::operator -(long other) const {
    // See operator -= for an explanation of why all of this code works.
    if (! large_) {
        long ans = small_ - other;
        if (! ((((small_ ^ other) & (ans ^ small_))) & LONG_MIN))
            return NLazyInteger(ans); // No overflow.
    }

    // We have to use large integer arithmetic.
    NLazyInteger ans;
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

inline NLazyInteger NLazyInteger::operator *(const NLazyInteger& other)
        const {
    // Do the standard thing for now.
    NLazyInteger ans(*this);
    return ans *= other;
}

inline NLazyInteger NLazyInteger::operator *(long other) const {
    // Do the standard thing for now.
    NLazyInteger ans(*this);
    return ans *= other;
}

inline NLazyInteger NLazyInteger::operator /(const NLazyInteger& other)
        const {
    // Do the standard thing for now.
    NLazyInteger ans(*this);
    return ans /= other;
}

inline NLazyInteger NLazyInteger::operator /(long other) const {
    // Do the standard thing for now.
    NLazyInteger ans(*this);
    return ans /= other;
}

inline NLazyInteger NLazyInteger::divExact(const NLazyInteger& other)
        const {
    // Do the standard thing for now.
    NLazyInteger ans(*this);
    return ans.divExact(other);
}

inline NLazyInteger NLazyInteger::divExact(long other) const {
    // Do the standard thing for now.
    NLazyInteger ans(*this);
    return ans.divExact(other);
}

inline NLazyInteger NLazyInteger::operator %(const NLazyInteger& other)
        const {
    // Do the standard thing for now.
    NLazyInteger ans(*this);
    return ans %= other;
}

inline NLazyInteger NLazyInteger::operator %(long other) const {
    // Do the standard thing for now.
    NLazyInteger ans(*this);
    return ans %= other;
}

inline NLazyInteger NLazyInteger::operator -() const {
    if (large_) {
        NLazyInteger ans;
        ans.large_ = new mpz_t;
        mpz_init(ans.large_);
        mpz_neg(ans.large_, large_);
        return ans;
    } else if (small_ == LONG_MIN) {
        // Overflow, just.
        NLazyInteger ans;
        ans.large_ = new mpz_t;
        mpz_init_set_si(ans.large_, small_);
        mpz_neg(ans.large_, ans.large_);
        return ans;
    } else
        return NLazyInteger(-small_);
}

inline NLazyInteger& NLazyInteger::operator +=(const NLazyInteger& other) {
    if (other.large_) {
        if (! large_)
            forceLarge();
        mpz_add(large_, large_, other.large_);
        return *this;
    } else
        return (*this) += other.small_;
}

inline NLazyInteger& NLazyInteger::operator +=(long other) {
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

inline NLazyInteger& NLazyInteger::operator -=(const NLazyInteger& other) {
    if (other.large_) {
        if (! large_)
            forceLarge();
        mpz_sub(large_, large_, other.large_);
        return *this;
    } else
        return (*this) -= other.small_;
}

inline NLazyInteger& NLazyInteger::operator -=(long other) {
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

inline NLazyInteger& NLazyInteger::operator *=(const NLazyInteger& other) {
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

inline NLazyInteger& NLazyInteger::operator *=(long other) {
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

inline NLazyInteger& NLazyInteger::operator /=(const NLazyInteger& other) {
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
                const_cast<NLazyInteger&>(other).forceReduce();
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
            const_cast<NLazyInteger&>(other).forceReduce();
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
        const_cast<NLazyInteger&>(other).forceReduce();
        small_ /= other.small_;
        return *this;
    } else
        return (*this) /= other.small_;
}

inline NLazyInteger& NLazyInteger::operator /=(long other) {
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

inline NLazyInteger& NLazyInteger::divByExact(const NLazyInteger& other) {
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
            const_cast<NLazyInteger&>(other).forceReduce();

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
        const_cast<NLazyInteger&>(other).forceReduce();
        small_ /= other.small_;
        return *this;
    } else {
        // other is already a native int.
        // Use the native version of this routine instead.
        return divByExact(other.small_);
    }
}

inline NLazyInteger& NLazyInteger::divByExact(long other) {
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

inline NLazyInteger& NLazyInteger::operator %=(const NLazyInteger& other) {
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
        const_cast<NLazyInteger&>(other).forceReduce();
        small_ %= other.small_;
        return *this;
    } else
        return (*this) %= other.small_;
}

inline NLazyInteger& NLazyInteger::operator %=(long other) {
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

inline void NLazyInteger::negate() {
    if (large_)
        mpz_neg(large_, large_);
    else if (small_ == LONG_MIN) {
        // Overflow, just.
        forceLarge();
        mpz_neg(large_, large_);
    } else
        small_ = -small_;
}

inline NLazyInteger NLazyInteger::abs() const {
    if (large_) {
        NLazyInteger ans;
        ans.large_ = new mpz_t;
        mpz_init_set(ans.large_, large_);
        mpz_abs(ans.large_, large_);
        return ans;
    } else if (small_ == LONG_MIN) {
        // Overflow, just.
        NLazyInteger ans;
        ans.large_ = new mpz_t;
        mpz_init_set_si(ans.large_, small_);
        mpz_neg(ans.large_, ans.large_);
        return ans;
    } else
        return NLazyInteger(small_ >= 0 ? small_ : - small_);
}

inline NLazyInteger operator +(long lhs, const NLazyInteger& rhs) {
    return rhs + lhs;
}

inline NLazyInteger operator *(long lhs, const NLazyInteger& rhs) {
    return rhs * lhs;
}

inline void NLazyInteger::makeLarge() {
    if (! large_)
        forceLarge();
}

inline void NLazyInteger::tryReduce() {
    if (large_ && mpz_cmp_si(large_, LONG_MAX) <= 0 &&
            mpz_cmp_si(large_, LONG_MIN) >= 0)
        forceReduce();
}

inline void NLazyInteger::forceLarge() {
    large_ = new mpz_t;
    mpz_init_set_si(large_, small_);
}

inline void NLazyInteger::clearLarge() {
    mpz_clear(large_);
    delete large_;
    large_ = 0;
}

inline void NLazyInteger::forceReduce() {
    small_ = mpz_get_si(large_);
    mpz_clear(large_);
    delete large_;
    large_ = 0;
}

} // namespace regina

#endif

