
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#ifndef __NMPI_H
#ifndef __DOXYGEN
#define __NMPI_H
#endif

/*! \file nmpi.h
 *  \brief Deals with arbitrary precision integers.
 */

#include <gmp.h>
#include <iostream.h>

/**
 * Represents an arbitrary precision integer.
 * Calculations will be exact no matter how large the integers become.
 * Current algorithms and representations use the GNU multiple precision
 * arithmetic library.
 *
 * Infinity is catered for.  For comparison operations, infinity is
 * considered larger than any other integer but equal to itself.
 *
 * \ifaces Not present.
 */
class NLargeInteger {
    public:
        static const NLargeInteger zero;
            /**< Globally available zero. */
        static const NLargeInteger one;
            /**< Globally available one. */
        static const NLargeInteger infinity;
            /**< Globally available infinity. */

    private:
        mpz_t data;
            /**< Contains the arbitrarily large integer.  If this
             *   NLargeInteger represents infinity, this member variable
             *   is ignored. */
        bool infinite;
            /**< Does this NLargeInteger represent infinity? */

    public:
        /**
         * Initialises this integer to zero.
         */
        NLargeInteger();
        /**
         * Initialises this integer to the given value.
         *
         * @param value the new value of this integer.
         */
        NLargeInteger(long value);
        /**
         * Initialises this integer to the given value.
         *
         * @param value the new value of this integer.
         */
        NLargeInteger(const NLargeInteger& value);
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
         * Whitespace may be present in the given string and will simply
         * be ignored.
         *
         * \pre The given base is zero, or is between 2 and 36 inclusive.
         * \pre The given string represents a finite integer
         * in the given base, with optional whitespace added.
         *
         * @param the new value of this integer, represented as a string
         * of digits in base \a base.
         * @param base the base in which \a value is given.
         */
        NLargeInteger(const char* value, int base = 10);
        /**
         * Destroys this integer.
         */
        virtual ~NLargeInteger();

        /**
         * Determines if this large integer is infinity.
         *
         * @return \c true if and only if this large integer is infinity.
         */
        bool isInfinite() const;
        /**
         * Returns the value of this integer as a long.
         * If this integer is outside the range of a long, the result is
         * unpredictable.
         *
         * \pre This integer is not infinity.
         *
         * @return the value of this integer.
         */
        long longValue() const;
        /**
         * Returns the value of this integer as a string in the given
         * base.
         * The string will be newly allocated and it is the caller's
         * responsibility to deallocate it once finished with it.
         * Note that deallocation must be done using
         * <tt>delete[] str</tt> as opposed to simply
         * <tt>delete str</tt>.
         *
         * If not specified, the base will default to 10.
         *
         * If this integer is infinity, the string returned will be
         * \c inf.
         *
         * \pre The given base is between 2 and 36 inclusive.
         *
         * @return the value of this integer as a newly allocated
         * string.
         */
        char* stringValue(int base = 10) const;

        /**
         * Sets this integer to the given value.
         *
         * @param value the new value of this integer.
         * @return a reference to this integer with its new value.
         */
        NLargeInteger& operator =(const NLargeInteger& value);
        /**
         * Sets this integer to the given value.
         *
         * @param value the new value of this integer.
         * @return a reference to this integer with its new value.
         */
        NLargeInteger& operator =(long value);
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
         * @param the new value of this integer, represented as a string
         * of digits in base 10.
         * @return a reference to this integer with its new value.
         */
        NLargeInteger& operator =(const char* value);
        /**
         * Swaps the values of this and the given integer.
         *
         * @param other the integer whose value will be swapped with
         * this.
         */
        void swap(NLargeInteger& other);
        
        /**
         * Determines if this is equal to the given integer.
         *
         * @param compareTo the integer with which this will be compared.
         * @return \c true if and only if this and the given integer are
         * equal.
         */
        bool operator ==(const NLargeInteger& compareTo) const;
        /**
         * Determines if this is equal to the given integer.
         *
         * @param compareTo the integer with which this will be compared.
         * @return \c true if and only if this and the given integer are
         * equal.
         */
        bool operator ==(long compareTo) const;
        /**
         * Determines if this is not equal to the given integer.
         *
         * @param compareTo the integer with which this will be compared.
         * @return \c true if and only if this and the given integer are
         * not equal.
         */
        bool operator !=(const NLargeInteger& compareTo) const;
        /**
         * Determines if this is not equal to the given integer.
         *
         * @param compareTo the integer with which this will be compared.
         * @return \c true if and only if this and the given integer are
         * not equal.
         */
        bool operator !=(long compareTo) const;
        /**
         * Determines if this is less than the given integer.
         *
         * @param compareTo the integer with which this will be compared.
         * @return \c true if and only if this is less than the given
         * integer.
         */
        bool operator <(const NLargeInteger& compareTo) const;
        /**
         * Determines if this is less than the given integer.
         *
         * @param compareTo the integer with which this will be compared.
         * @return \c true if and only if this is less than the given
         * integer.
         */
        bool operator <(long compareTo) const;
        /**
         * Determines if this is greater than the given integer.
         *
         * @param compareTo the integer with which this will be compared.
         * @return \c true if and only if this is greater than the given
         * integer.
         */
        bool operator >(const NLargeInteger& compareTo) const;
        /**
         * Determines if this is greater than the given integer.
         *
         * @param compareTo the integer with which this will be compared.
         * @return \c true if and only if this is greater than the given
         * integer.
         */
        bool operator >(long compareTo) const;
        /**
         * Determines if this is less than or equal to the given integer.
         *
         * @param compareTo the integer with which this will be compared.
         * @return \c true if and only if this is less than or equal to
         * the given integer.
         */
        bool operator <=(const NLargeInteger& compareTo) const;
        /**
         * Determines if this is less than or equal to the given integer.
         *
         * @param compareTo the integer with which this will be compared.
         * @return \c true if and only if this is less than or equal to
         * the given integer.
         */
        bool operator <=(long compareTo) const;
        /**
         * Determines if this is greater than or equal to the given integer.
         *
         * @param compareTo the integer with which this will be compared.
         * @return \c true if and only if this is greater than or equal
         * to the given integer.
         */
        bool operator >=(const NLargeInteger& compareTo) const;
        /**
         * Determines if this is greater than or equal to the given integer.
         *
         * @param compareTo the integer with which this will be compared.
         * @return \c true if and only if this is greater than or equal
         * to the given integer.
         */
        bool operator >=(long compareTo) const;

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
        NLargeInteger operator +(const NLargeInteger& other) const;
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
        NLargeInteger operator -(const NLargeInteger& other) const;
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
        NLargeInteger operator *(const NLargeInteger& other) const;
        /**
         * Divides this by the given integer and returns the result.
         * The result will be truncated to an integer, i.e. rounded
         * towards zero.
         * This integer is not changed.
         * 
         * If \a other is known to divide this integer exactly,
         * divExact() should be used instead.
         *
         * Infinity divided by anything will return infinity; anything
         * finite divided by infinity will return zero; anything finite
         * divided by zero will return infinity.
         *
         * @param other the integer to divide this by.
         * @return the quotient \a this divided by \a other.
         */
        NLargeInteger operator /(const NLargeInteger& other) const;
        /**
         * Divides this by the given integer and returns the result.
         * This can only be used when the given integer divides into
         * this exactly, and is much faster than ordinary division.
         * This integer is not changed.
         *
         * \pre The given integer divides exactly into
         * this integer, i.e. \a this divided by \a other is an
         * integer.
         * \pre \a other is not zero.
         * \pre Neither this nor \a other is infinite.
         *
         * @param other the integer to divide this by.
         * @return the quotient \a this divided by \a other.
         */
        NLargeInteger divExact(const NLargeInteger& other) const;
        /**
         * Determines the remainder when this integer is divided by the
         * given integer.  If non-zero, the result will have the same sign
         * as this integer.
         * This integer is not changed.
         *
         * \pre \a other is not zero.
         * \pre Neither this nor \a other is infinite.
         *
         * @param other the integer to divide this by.
         * @return the remainder \a this modulo \a other.
         */
        NLargeInteger operator %(const NLargeInteger& other) const;
        /**
         * Determines the negative of this integer.
         * This integer is not changed.
         *
         * Negating infinity will return infinity.
         *
         * @return the negative of this integer.
         */
        NLargeInteger operator -() const;

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
        NLargeInteger& operator +=(const NLargeInteger& other);
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
        NLargeInteger& operator +=(long other);
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
        NLargeInteger& operator -=(const NLargeInteger& other);
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
        NLargeInteger& operator -=(long other);
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
        NLargeInteger& operator *=(const NLargeInteger& other);
        /**
         * Divides the given integer by this.
         * The result will be truncated to an integer, i.e. rounded
         * towards zero.
         * This integer is changed to reflect the result.
         * 
         * If \a other is known to divide this integer exactly,
         * divByExact() should be used instead.
         *
         * Infinity divided by anything will return infinity; anything
         * finite divided by infinity will return zero; anything finite
         * divided by zero will return infinity.
         *
         * @param other the integer to divide this by.
         * @return a reference to this integer with its new value.
         */
        NLargeInteger& operator /=(const NLargeInteger& other);
        /**
         * Divides this by the given integer.
         * This can only be used when the given integer divides into
         * this exactly, and is much faster than ordinary division.
         * This integer is changed to reflect the result.
         *
         * \pre The given integer divides exactly into
         * this integer, i.e. \a this divided by \a other is an
         * integer.
         * \pre \a other is not zero.
         * \pre Neither this nor \a other is infinite.
         *
         * @param other the integer to divide this by.
         * @return a reference to this integer with its new value.
         */
        NLargeInteger& divByExact(const NLargeInteger& other);
        /**
         * Reduces this integer modulo the given integer.
         * If non-zero, the result will have the same sign as the original
         * value of this integer.
         * This integer is changed to reflect the result.
         *
         * \pre \a other is not zero.
         * \pre Neither this nor \a other is infinite.
         *
         * @param other the integer modulo which this integer will be
         * reduced.
         * @return a reference to this integer with its new value.
         */
        NLargeInteger& operator %=(const NLargeInteger& other);
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
         * power of 0 will be 1 and infinity to the
         * power of anything else will be infinity.
         *
         * \pre The given exponent is non-negative.
         *
         * @param exp the power to which this integer will be raised.
         */
        void raiseToPower(unsigned long exp);

        /**
         * Determines the absolute value of this integer.
         * This integer is not changed.
         *
         * @return the absolute value of this integer.
         */
        NLargeInteger abs() const;
        /**
         * Determines the greatest common divisor of this and the given
         * integer.  This integer is not changed.
         * 
         * Note that the result might possibly be negative.
         *
         * \pre Neither this integer nor \a other is infinite.
         *
         * @param other the integer whose greatest common divisor with
         * this will be found.
         * @return the greatest common divisor of this and the given
         * integer.
         */
        NLargeInteger gcd(const NLargeInteger& other) const;
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
        NLargeInteger lcm(const NLargeInteger& other) const;

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
         * This equations are not satisfied when either of \a this or
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
         */
        NLargeInteger gcdWithCoeffs(const NLargeInteger& other,
            NLargeInteger& u, NLargeInteger& v) const;
    
    private:
        /**
         * Initialises this integer to infinity.
         * All parameters are ignored.
         */
        NLargeInteger(bool, bool);

    friend class NRational;
        /**< Allow access to private members. */
    friend ostream& operator << (ostream& out, const NLargeInteger& large);
};

/**
 * Writes the given integer to the given output stream.
 *
 * @param out the output stream to which to write.
 * @param large the integer to write.
 * @return a reference to \a out.
 */
ostream& operator << (ostream& out, const NLargeInteger& large);

// Inline functions for NLargeInteger

inline NLargeInteger::NLargeInteger() : infinite(false) {
    mpz_init(data);
}
inline NLargeInteger::NLargeInteger(long value) : infinite(false) {
    mpz_init_set_si(data, value);
}
inline NLargeInteger::NLargeInteger(const NLargeInteger& value) :
        infinite(value.infinite) {
    mpz_init_set(data, value.data);
}
inline NLargeInteger::NLargeInteger(const char* value, int base) :
        infinite(false) {
    mpz_init_set_str(data, value, base);
}
inline NLargeInteger::NLargeInteger(bool, bool) : infinite(true) {
    // Private constructor.
    mpz_init(data);
}
inline NLargeInteger::~NLargeInteger() {
    mpz_clear(data);
}

inline bool NLargeInteger::isInfinite() const {
    return infinite;
}
inline long NLargeInteger::longValue() const {
    return mpz_get_si(data);
}

inline NLargeInteger& NLargeInteger::operator =(const NLargeInteger& value) {
    infinite = value.infinite;
    mpz_set(data, value.data);
    return *this;
}
inline NLargeInteger& NLargeInteger::operator =(long value) {
    infinite = false;
    mpz_set_si(data, value);
    return *this;
}
inline NLargeInteger& NLargeInteger::operator =(const char* value) {
    infinite = false;
    mpz_set_str(data, value, 10);
    return *this;
}
inline void NLargeInteger::swap(NLargeInteger& other) {
    NLargeInteger tmp(other);
    other = *this;
    *this = tmp;
}

inline bool NLargeInteger::operator ==(const NLargeInteger& compareTo) const {
    return ((infinite && compareTo.infinite) ||
        ((! infinite) && (! compareTo.infinite) &&
        mpz_cmp(data, compareTo.data) == 0));
}
inline bool NLargeInteger::operator ==(long compareTo) const {
    return ((! infinite) && mpz_cmp_si(data, compareTo) == 0);
}
inline bool NLargeInteger::operator !=(const NLargeInteger& compareTo) const {
    return (((! infinite) || (! compareTo.infinite)) &&
        (infinite || compareTo.infinite ||
        mpz_cmp(data, compareTo.data) != 0));
}
inline bool NLargeInteger::operator !=(long compareTo) const {
    return (infinite || mpz_cmp_si(data, compareTo) != 0);
}
inline bool NLargeInteger::operator <(const NLargeInteger& compareTo) const {
    return ((! infinite) &&
        (compareTo.infinite || mpz_cmp(data, compareTo.data) < 0));
}
inline bool NLargeInteger::operator <(long compareTo) const {
    return ((! infinite) && mpz_cmp_si(data, compareTo) < 0);
}
inline bool NLargeInteger::operator >(const NLargeInteger& compareTo) const {
    return ((! compareTo.infinite) &&
        (infinite || mpz_cmp(data, compareTo.data) > 0));
}
inline bool NLargeInteger::operator >(long compareTo) const {
    return (infinite || mpz_cmp_si(data, compareTo) > 0);
}
inline bool NLargeInteger::operator <=(const NLargeInteger& compareTo) const {
    return (compareTo.infinite ||
        ((! infinite) && mpz_cmp(data, compareTo.data) <= 0));
}
inline bool NLargeInteger::operator <=(long compareTo) const {
    return ((! infinite) && mpz_cmp_si(data, compareTo) <= 0);
}
inline bool NLargeInteger::operator >=(const NLargeInteger& compareTo) const {
    return (infinite ||
        ((! compareTo.infinite) && mpz_cmp(data, compareTo.data) >= 0));
}
inline bool NLargeInteger::operator >=(long compareTo) const {
    return (infinite || mpz_cmp_si(data, compareTo) >= 0);
}

inline NLargeInteger NLargeInteger::operator +(const NLargeInteger& other)
        const {
    if (infinite || other.infinite)
        return infinity;
    NLargeInteger ans;
    mpz_add(ans.data, data, other.data);
    return ans;
}
inline NLargeInteger NLargeInteger::operator -(const NLargeInteger& other)
        const {
    if (infinite || other.infinite)
        return infinity;
    NLargeInteger ans;
    mpz_sub(ans.data, data, other.data);
    return ans;
}
inline NLargeInteger NLargeInteger::operator *(const NLargeInteger& other)
        const {
    if (infinite || other.infinite)
        return infinity;
    NLargeInteger ans;
    mpz_mul(ans.data, data, other.data);
    return ans;
}
inline NLargeInteger NLargeInteger::operator /(const NLargeInteger& other)
        const {
    if (infinite)
        return infinity;
    if (other.infinite)
        return zero;
    if (mpz_cmp_si(other.data, 0) == 0)
        return infinity;
    NLargeInteger ans;
    mpz_tdiv_q(ans.data, data, other.data);
    return ans;
}
inline NLargeInteger NLargeInteger::divExact(const NLargeInteger& other)
        const {
    NLargeInteger ans;
    mpz_divexact(ans.data, data, other.data);
    return ans;
}
inline NLargeInteger NLargeInteger::operator %(const NLargeInteger& other)
        const {
    NLargeInteger ans;
    mpz_tdiv_r(ans.data, data, other.data);
    return ans;
}
inline NLargeInteger NLargeInteger::operator -() const {
    if (infinite)
        return infinity;
    NLargeInteger ans;
    mpz_neg(ans.data, data);
    return ans;
}

inline NLargeInteger& NLargeInteger::operator +=(const NLargeInteger& other) {
    if (! infinite) {
        if (other.infinite)
            infinite = true;
        else
            mpz_add(data, data, other.data);
    }
    return *this;
}
inline NLargeInteger& NLargeInteger::operator +=(long other) {
    if (! infinite) {
        if (other >= 0)
            mpz_add_ui(data, data, other);
        else
            mpz_sub_ui(data, data, -other);
    }
    return *this;
}
inline NLargeInteger& NLargeInteger::operator -=(const NLargeInteger& other) {
    if (! infinite) {
        if (other.infinite)
            infinite = true;
        else
            mpz_sub(data, data, other.data);
    }
    return *this;
}
inline NLargeInteger& NLargeInteger::operator -=(long other) {
    if (! infinite) {
        if (other >= 0)
            mpz_sub_ui(data, data, other);
        else
            mpz_add_ui(data, data, -other);
    }
    return *this;
}
inline NLargeInteger& NLargeInteger::operator *=(const NLargeInteger& other) {
    if (! infinite) {
        if (other.infinite)
            infinite = true;
        else
            mpz_mul(data, data, other.data);
    }
    return *this;
}
inline NLargeInteger& NLargeInteger::operator /=(const NLargeInteger& other) {
    if (! infinite) {
        if (other.infinite)
            (*this) = zero;
        else if (mpz_cmp_si(other.data, 0) == 0)
            infinite = true;
        else
            mpz_tdiv_q(data, data, other.data);
    }
    return *this;
}
inline NLargeInteger& NLargeInteger::divByExact(const NLargeInteger& other) {
    mpz_divexact(data, data, other.data);
    return *this;
}
inline NLargeInteger& NLargeInteger::operator %=(const NLargeInteger& other) {
    mpz_tdiv_r(data, data, other.data);
    return *this;
}
inline void NLargeInteger::negate() {
    if (! infinite)
        mpz_neg(data, data);
}
inline void NLargeInteger::raiseToPower(unsigned long exp) {
    if (exp == 0)
        (*this) = one;
    else if (! infinite)
        mpz_pow_ui(data, data, exp);
}

inline NLargeInteger NLargeInteger::abs() const {
    if (infinite)
        return infinity;
    NLargeInteger ans;
    mpz_abs(ans.data, data);
    return ans;
}
inline NLargeInteger NLargeInteger::gcd(const NLargeInteger& other) const {
    NLargeInteger ans;
    mpz_gcd(ans.data, data, other.data);
    return ans;
}
inline NLargeInteger NLargeInteger::lcm(const NLargeInteger& other) const {
    NLargeInteger ans(*this);
    ans *= other;
    ans.divByExact(gcd(other));
    return ans;
}

#endif

