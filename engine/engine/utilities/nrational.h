
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2004, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#ifndef __NRATIONAL_H
#ifndef __DOXYGEN
#define __NRATIONAL_H
#endif

/*! \file nrational.h
 *  \brief Deals with artibrary precision rational numbers.
 */

#include "utilities/nmpi.h"

namespace regina {

/**
 * \weakgroup utilities
 * @{
 */

/**
 * Represents an arbitrary precision rational number.
 * Calculations with NRational objects will be exact.
 *
 * Infinity (1/0) and undefined (0/0) are catered for.  (-1/0) is considered
 * the same as (1/0), and is represented as (1/0).
 * Any operation involving (0/0) will return (0/0).
 *
 * Since infinity is the same as negative infinity, both infinity plus
 * infinity and infinity minus infinity will return infinity.  Infinity
 * divided by infinity returns undefined, as does infinity times zero.
 *
 * For the purposes of ordering, undefined is the smallest rational and
 * infinity is the largest.  Undefined is always equal to itself, and
 * infinity is always equal to itself.
 *
 * Rationals will always be stored in lowest terms with non-negative
 * denominator.
 */
class NRational {
    public:
        static const NRational zero;
            /**< Globally available zero. */
        static const NRational one;
            /**< Globally available one. */
        static const NRational infinity;
            /**< Globally available infinity.  Note that both 1/0 and
             *   -1/0 evaluate to this same rational.  When queried,
             *   the representation 1/0 will be returned. */
        static const NRational undefined;
            /**< Globally available undefined.  This is represented as
             *   0/0. */
    private:
        /**
         * Represents the available flavours of rational number.
         */
        enum flavourType {
            f_infinity,
                /**< Infinity; there is only one rational of this type. */
            f_undefined,
                /**< Undefined; there is only one rational of this type. */
            f_normal
                /**< An ordinary rational (the denominator is non-zero). */
        };
        flavourType flavour;
            /**< Stores whether this rational is infinity, undefined or
             *   normal (non-zero denominator). */
        mpq_t data;
            /**< Contains the arbitrary precision rational data for normal
             *   (non-zero denominator) rationals. */

    public:
        /**
         * Initialises to 0/1.
         */
        NRational();
        /**
         * Initialises to the given rational value.
         *
         * @param value the new rational value of this rational.
         */
        NRational(const NRational& value);
        /**
         * Initialises to the given integer value.
         *
         * @param value the new integer value of this rational.
         */
        NRational(const NLargeInteger& value);
        /**
         * Initialises to the given integer value.
         *
         * @param value the new integer value of this rational.
         */
        NRational(long value);
        /**
         * Initialises to <i>newNum</i>/<i>newDen</i>.
         *
         * \pre gcd(<i>newNum</i>, <i>newDen</i>) = 1 or <i>newDen</i> = 0.
         * \pre \a newDen is non-negative.
         *
         * @param newNum the new numerator.
         * @param newDen the new denominator.
         */
        NRational(const NLargeInteger& newNum, const NLargeInteger& newDen);
        /**
         * Initialises to <i>newNum</i>/<i>newDen</i>.
         *
         * \pre gcd(<i>newNum</i>, <i>newDen</i>) = 1 or <i>newDen</i> = 0.
         * \pre \a newDen is non-negative.
         *
         * @param newNum the new numerator.
         * @param newDen the new denominator.
         */
        NRational(long newNum, unsigned long newDen);
        /**
         * Destroys this rational.
         */
        virtual ~NRational();

        /**
         * Sets this rational to the given rational value.
         *
         * @param value the new value of this rational.
         * @return a reference to this rational with its new value.
         */
        NRational& operator = (const NRational& value);
        /**
         * Sets this rational to the given integer value.
         *
         * @param value the new value of this rational.
         * @return a reference to this rational with its new value.
         */
        NRational& operator = (const NLargeInteger& value);
        /**
         * Sets this rational to the given integer value.
         *
         * @param value the new value of this rational.
         * @return a reference to this rational with its new value.
         */
        NRational& operator = (long value);

        /**
         * Returns the numerator of this rational.
         * Note that rationals are always stored in lowest terms with
         * non-negative denominator.  Infinity will be stored as 1/0.
         *
         * @return the numerator.
         */
        NLargeInteger getNumerator() const;
        /**
         * Returns the denominator of this rational.
         * Note that rationals are always stored in lowest terms with
         * non-negative denominator.
         *
         * @return the denominator.
         */
        NLargeInteger getDenominator() const;
        
        /**
         * Calculates the product of two rationals.
         * This rational is not changed.
         *
         * @param r the rational with which to multiply this.
         * @return the product \a this * \a r.
         */
        NRational operator *(const NRational& r) const;
        /**
         * Calculates the ratio of two rationals.
         * This rational is not changed.
         *
         * @param r the rational to divide this by.
         * @return the ratio \a this / \a r.
         */
        NRational operator /(const NRational& r) const;
        /**
         * Calculates the sum of two rationals.
         * This rational is not changed.
         *
         * @param r the rational to add to this.
         * @return the sum \a this + \a r.
         */
        NRational operator +(const NRational& r) const;
        /**
         * Calculates the difference of two rationals.
         * This rational is not changed.
         *
         * @param r the rational to subtract from this.
         * @return the difference \a this - \a r.
         */
        NRational operator -(const NRational& r) const;
        /**
         * Determines the negative of this rational.
         * This rational is not changed.
         *
         * @return the negative of this rational.
         */
        NRational operator - () const;
        /**
         * Calculates the inverse of this rational.
         * This rational is not changed.
         *
         * @return the inverse 1 / \a this.
         */
        NRational inverse() const;

        /**
         * Adds the given rational to this.
         * This rational is changed to reflect the result.
         *
         * @param other the rational to add to this.
         * @return a reference to this rational with its new value.
         */
        NRational& operator += (const NRational& other);
        /**
         * Subtracts the given rational from this.
         * This rational is changed to reflect the result.
         *
         * @param other the rational to subtract from this.
         * @return a reference to this rational with its new value.
         */
        NRational& operator -= (const NRational& other);
        /**
         * Multiplies the given rational by this.
         * This rational is changed to reflect the result.
         *
         * @param other the rational to multiply by this.
         * @return a reference to this rational with its new value.
         */
        NRational& operator *= (const NRational& other);
        /**
         * Divides this by the given rational.
         * This rational is changed to reflect the result.
         *
         * @param other the rational to divide this by.
         * @return a reference to this rational with its new value.
         */
        NRational& operator /= (const NRational& other);
        /**
         * Negates this rational.
         * This rational is changed to reflect the result.
         */
        void negate();
        /**
         * Inverts this rational.
         * This rational is changed to reflect the result.
         */
        void invert();

        /**
         * Determines if this is equal to the given rational.
         *
         * @param compare the rational with which this will be compared.
         * @return \c true if and only if this rational is equal to
         * \a compare.
         */
        bool operator == (const NRational& compare) const;
        /**
         * Determines if this is not equal to the given rational.
         *
         * @param compare the rational with which this will be compared.
         * @return \c true if and only if this rational is not equal to
         * \a compare.
         */
        bool operator != (const NRational& compare) const;
        /**
         * Determines if this is less than the given rational.
         *
         * @param compare the rational with which this will be compared.
         * @return \c true if and only if this rational is less than
         * \a compare.
         */
        bool operator < (const NRational& compare) const;
        /**
         * Determines if this is greater than the given rational.
         *
         * @param compare the rational with which this will be compared.
         * @return \c true if and only if this rational is greater than
         * \a compare.
         */
        bool operator > (const NRational& compare) const;
        /**
         * Determines if this is less than or equal to the given rational.
         *
         * @param compare the rational with which this will be compared.
         * @return \c true if and only if this rational is less than or
         * equal to \a compare.
         */
        bool operator <= (const NRational& compare) const;
        /**
         * Determines if this is greater than or equal to the given rational.
         *
         * @param compare the rational with which this will be compared.
         * @return \c true if and only if this rational is greater than
         * or equal to \a compare.
         */
        bool operator >= (const NRational& compare) const;

    friend std::ostream& operator << (std::ostream& out, const NRational& rat);
};

/**
 * Writes the given rational to the given output stream.
 * Infinity will be written as <tt>Inf</tt>.  Undefined will be written
 * as <tt>Undef</tt>.  A rational with denominator one will be written
 * as a single integer.  All other rationals will be written in the form
 * <tt>r/s</tt>.
 *
 * @param out the output stream to which to write.
 * @param rat the rational to write.
 * @return a reference to \a out.
 */
std::ostream& operator << (std::ostream& out, const NRational& rat);

/*@}*/

// Inline functions for NRational

inline NRational::NRational() : flavour(f_normal) {
    mpq_init(data);
}
inline NRational::NRational(const NRational& value) : flavour(value.flavour) {
    mpq_init(data);
    if (flavour == f_normal)
        mpq_set(data, value.data);
}
inline NRational::NRational(const NLargeInteger& value) : flavour(f_normal) {
    mpq_init(data);
    mpq_set_z(data, value.data);
}
inline NRational::NRational(long value) : flavour(f_normal) {
    mpq_init(data);
    mpq_set_si(data, value, 1);
}
inline NRational::~NRational() {
    mpq_clear(data);
}

inline NRational& NRational::operator = (const NRational& value) {
    flavour = value.flavour;
    if (flavour == f_normal)
        mpq_set(data, value.data);
    return *this;
}
inline NRational& NRational::operator = (const NLargeInteger& value) {
    flavour = f_normal;
    mpq_set_z(data, value.data);
    return *this;
}
inline NRational& NRational::operator = (long value) {
    flavour = f_normal;
    mpq_set_si(data, value, 1);
    return *this;
}

inline void NRational::negate() {
    if (flavour == f_normal)
        mpq_neg(data, data);
}

inline bool NRational::operator <= (const NRational& compare) const {
    return ! (*this > compare);
}
inline bool NRational::operator >= (const NRational& compare) const {
    return ! (*this < compare);
}
inline bool NRational::operator != (const NRational& compare) const {
    return ! (*this == compare);
}

} // namespace regina

#endif
