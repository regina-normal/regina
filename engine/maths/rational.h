
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

#ifndef __REGINA_RATIONAL_H
#ifndef __DOXYGEN
#define __REGINA_RATIONAL_H
#endif

/*! \file maths/rational.h
 *  \brief Deals with artibrary precision rational numbers.
 */

#include "regina-core.h"
#include "maths/integer.h"

namespace regina {

/**
 * Represents an arbitrary precision rational number.
 * Calculations with Rational objects will be exact.
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
 * When performing computations on rationals, the results will always be
 * stored in lowest terms (i.e., with relatively prime numerator and
 * denominator), and with a non-negative denominator.
 * However, when constructing a rational number from scratch (e.g., by
 * supplying the numerator and denominator separately), it is your
 * responsibility to ensure that the rational is in lowest terms.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \ingroup maths
 */
class Rational {
    public:
        static const Rational zero;
            /**< Globally available zero. */
        static const Rational one;
            /**< Globally available one. */
        static const Rational infinity;
            /**< Globally available infinity.  Note that both 1/0 and
             *   -1/0 evaluate to this same rational.  When queried,
             *   the representation 1/0 will be returned. */
        static const Rational undefined;
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
             *   (non-zero denominator) rationals.
             *   This is initialised even if the rational is infinite. */

        static const Rational maxDouble;
            /**< The largest positive rational number that can be converted
             *   to a finite double.  This begins as undefined, and is set
             *   to its correct value on the first call to doubleApprox(). */
        static const Rational minDouble;
            /**< The smallest positive rational number that can be converted
             *   to a non-zero double.  This begins as undefined, and is set
             *   to its correct value on the first call to doubleApprox(). */

    public:
        /**
         * Initialises to 0/1.
         */
        Rational();
        /**
         * Initialises to the given rational value.
         *
         * \param value the new rational value of this rational.
         */
        Rational(const Rational& value);
        /**
         * Moves the given rational into this new rational.
         * This is a fast (constant time) operation.
         *
         * The rational that is passed (\a src) will no longer be usable.
         *
         * \param src the rational to move.
         */
        Rational(Rational&& src) noexcept;
        /**
         * Initialises to the given integer value.
         * The given integer may be infinite.
         *
         * \param value the new integer value of this rational.
         */
        template <bool withInfinity>
        Rational(const IntegerBase<withInfinity>& value);
        /**
         * Initialises to the given integer value.
         *
         * \param value the new integer value of this rational.
         */
        Rational(long value);
        /**
         * Initialises to <i>num</i>/<i>den</i>.
         *
         * \pre gcd(<i>num</i>, <i>den</i>) = 1 or <i>den</i> = 0.
         * \pre \a den is non-negative.
         *
         * \pre Neither of the given integers is infinite.
         *
         * \warning Failing to meet the preconditions above can result
         * in misleading or even undefined behaviour.  As an example,
         * Rational(4,4) (which breaks the gcd requirement) is
         * considered different from Rational(1,1) (a valid rational),
         * which is different again from Rational(-1,-1) (which breaks
         * the non-negativity requirement).
         *
         * \param num the new numerator.
         * \param den the new denominator.
         */
        template <bool withInfinity>
        Rational(const IntegerBase<withInfinity>& num,
                  const IntegerBase<withInfinity>& den);
        /**
         * Initialises to <i>num</i>/<i>den</i>.
         *
         * \pre gcd(<i>num</i>, <i>den</i>) = 1 or <i>den</i> = 0.
         * \pre \a den is non-negative.
         *
         * \warning Failing to meet the preconditions above can result
         * in misleading or even undefined behaviour.  As an example,
         * Rational(4,4) (which breaks the gcd requirement) is
         * considered different from Rational(1,1) (a valid rational),
         * which is different again from Rational(-1,-1) (which breaks
         * the non-negativity requirement).
         *
         * \param num the new numerator.
         * \param den the new denominator.
         */
        Rational(long num, unsigned long den);
        /**
         * Destroys this rational.
         */
        ~Rational();

        /**
         * Sets this rational to the given rational value.
         *
         * \param value the new value of this rational.
         * \return a reference to this rational with its new value.
         */
        Rational& operator = (const Rational& value);
        /**
         * Sets this rational to the given integer value.
         * The given integer may be infinite.
         *
         * \param value the new value of this rational.
         * \return a reference to this rational with its new value.
         */
        template <bool withInfinity>
        Rational& operator = (const IntegerBase<withInfinity>& value);
        /**
         * Sets this rational to the given integer value.
         *
         * \param value the new value of this rational.
         * \return a reference to this rational with its new value.
         */
        Rational& operator = (long value);
        /**
         * Moves the given rational into this rational.
         * This is a fast (constant time) operation.
         *
         * The rational that is passed (\a src) will no longer be usable.
         *
         * \param src the rational to move.
         * \return a reference to this rational.
         */
        Rational& operator = (Rational&& src) noexcept;
        /**
         * Swaps the values of this and the given rational.
         *
         * \param other the rational whose value will be swapped with this.
         */
        void swap(Rational& other) noexcept;

        /**
         * Returns the numerator of this rational.
         * Note that rationals are always stored in lowest terms with
         * non-negative denominator.  Infinity will be stored as 1/0.
         *
         * \return the numerator.
         */
        Integer numerator() const;
        /**
         * Returns the denominator of this rational.
         * Note that rationals are always stored in lowest terms with
         * non-negative denominator.  Infinity will be stored as 1/0.
         *
         * \return the denominator.
         */
        Integer denominator() const;

        /**
         * Calculates the product of two rationals.
         * This rational is not changed.
         *
         * \param r the rational with which to multiply this.
         * \return the product \a this * \a r.
         */
        Rational operator *(const Rational& r) const;
        /**
         * Calculates the ratio of two rationals.
         * This rational is not changed.
         *
         * \param r the rational to divide this by.
         * \return the ratio \a this / \a r.
         */
        Rational operator /(const Rational& r) const;
        /**
         * Calculates the sum of two rationals.
         * This rational is not changed.
         *
         * \param r the rational to add to this.
         * \return the sum \a this + \a r.
         */
        Rational operator +(const Rational& r) const;
        /**
         * Calculates the difference of two rationals.
         * This rational is not changed.
         *
         * \param r the rational to subtract from this.
         * \return the difference \a this - \a r.
         */
        Rational operator -(const Rational& r) const;
        /**
         * Determines the negative of this rational.
         * This rational is not changed.
         *
         * \return the negative of this rational.
         */
        Rational operator - () const;
        /**
         * Calculates the inverse of this rational.
         * This rational is not changed.
         *
         * \return the inverse 1 / \a this.
         */
        Rational inverse() const;
        /**
         * Determines the absolute value of this rational.
         * This rational is not changed.
         *
         * \return the absolute value of this rational.
         */
        Rational abs() const;

        /**
         * Adds the given rational to this.
         * This rational is changed to reflect the result.
         *
         * \param other the rational to add to this.
         * \return a reference to this rational with its new value.
         */
        Rational& operator += (const Rational& other);
        /**
         * Subtracts the given rational from this.
         * This rational is changed to reflect the result.
         *
         * \param other the rational to subtract from this.
         * \return a reference to this rational with its new value.
         */
        Rational& operator -= (const Rational& other);
        /**
         * Multiplies the given rational by this.
         * This rational is changed to reflect the result.
         *
         * \param other the rational to multiply by this.
         * \return a reference to this rational with its new value.
         */
        Rational& operator *= (const Rational& other);
        /**
         * Divides this by the given rational.
         * This rational is changed to reflect the result.
         *
         * \param other the rational to divide this by.
         * \return a reference to this rational with its new value.
         */
        Rational& operator /= (const Rational& other);
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
         * \param compare the rational with which this will be compared.
         * \return \c true if and only if this rational is equal to
         * \a compare.
         */
        bool operator == (const Rational& compare) const;
        /**
         * Determines if this is not equal to the given rational.
         *
         * \param compare the rational with which this will be compared.
         * \return \c true if and only if this rational is not equal to
         * \a compare.
         */
        bool operator != (const Rational& compare) const;
        /**
         * Determines if this is less than the given rational.
         *
         * \param compare the rational with which this will be compared.
         * \return \c true if and only if this rational is less than
         * \a compare.
         */
        bool operator < (const Rational& compare) const;
        /**
         * Determines if this is greater than the given rational.
         *
         * \param compare the rational with which this will be compared.
         * \return \c true if and only if this rational is greater than
         * \a compare.
         */
        bool operator > (const Rational& compare) const;
        /**
         * Determines if this is less than or equal to the given rational.
         *
         * \param compare the rational with which this will be compared.
         * \return \c true if and only if this rational is less than or
         * equal to \a compare.
         */
        bool operator <= (const Rational& compare) const;
        /**
         * Determines if this is greater than or equal to the given rational.
         *
         * \param compare the rational with which this will be compared.
         * \return \c true if and only if this rational is greater than
         * or equal to \a compare.
         */
        bool operator >= (const Rational& compare) const;

        /**
         * Attempts to convert this rational to a real number.
         *
         * If this rational can be approximated by a double
         * (specifically, if it lies within double's allowable range)
         * then a such an approximation is returned.  Otherwise this
         * routine will throw an exception.
         *
         * Note that "lies with double's allowable range" is
         * machine-dependent, and may vary between different installations.
         * Infinity and undefined are always considered out of range.
         * Otherwise a rational is out of range if its absolute value is
         * finite but too large (e.g., 10^10000) or non-zero but too small
         * (e.g., 10^-10000).
         *
         * \exception UnsolvedCase This rational lies outside double's
         * allowable range.
         *
         * \return the double approximation to this rational.
         *
         * \author Ryan Budney, B.B.
         */
        double doubleApprox() const;

        /**
         * Returns a string representation of this rational.
         *
         * For infinity and undefined rationals, the result will be `Inf` or
         * `Undef` respectively.  If this rational has denominator one then
         * the result will simply be the string representation of the numerator.
         * Otherwise the string returned will be in the form `r/s`.
         *
         * \return a string representation of this rational.
         */
        std::string str() const;

        /**
         * Returns this rational as written using TeX formatting.
         * No leading or trailing dollar signs will be included.
         *
         * \return this rational as written using TeX formatting.
         *
         * \author Ryan Budney
         */
        std::string tex() const;

        /**
         * Writes this rational in TeX format to the given output stream.
         * No leading or trailing dollar signs will be included.
         *
         * \nopython Instead use the variant tex()
         * that takes no arguments and returns a string.
         *
         * \param out the output stream to which to write.
         * \return a reference to the given output stream.
         *
         * \author Ryan Budney
         */
        std::ostream& writeTeX(std::ostream& out) const;

    private:
        /**
         * Initialises the class constants \a maxDouble and \a minDouble.
         * These constants are used by doubleApprox(), and so this routine
         * is called the first time that doubleApprox() is run.
         */
        static void initDoubleBounds();

    friend std::ostream& operator << (std::ostream& out, const Rational& rat);
};

/**
 * Swaps the contents of the given rationals.
 *
 * This global routine simply calls Rational::swap(); it is provided
 * so that Rational meets the C++ Swappable requirements.
 *
 * \param a the first rational whose contents should be swapped.
 * \param b the second rational whose contents should be swapped.
 *
 * \ingroup maths
 */
void swap(Rational& a, Rational& b) noexcept;

/**
 * Writes the given rational to the given output stream.
 * Infinity will be written as `Inf`.  Undefined will be written
 * as `Undef`.  A rational with denominator one will be written
 * as a single integer.  All other rationals will be written in the form
 * `r/s`.
 *
 * \param out the output stream to which to write.
 * \param rat the rational to write.
 * \return a reference to \a out.
 *
 * \ingroup maths
 */
std::ostream& operator << (std::ostream& out, const Rational& rat);

// Inline functions for Rational

inline Rational::Rational() : flavour(f_normal) {
    mpq_init(data);
}
inline Rational::Rational(const Rational& value) : flavour(value.flavour) {
    mpq_init(data);
    if (flavour == f_normal)
        mpq_set(data, value.data);
}
inline Rational::Rational(Rational&& src) noexcept : flavour(src.flavour) {
    mpq_init(data); // we need to leave something for src to destroy.
    mpq_swap(data, src.data);
}
template <bool withInfinity>
inline Rational::Rational(const IntegerBase<withInfinity>& value) :
        flavour(f_normal) {
    mpq_init(data);
    if (value.isInfinite())
        flavour = f_infinity;
    else if (value.isNative())
        mpq_set_si(data, value.longValue(), 1);
    else
        mpq_set_z(data, value.rawData());
}
inline Rational::Rational(long value) : flavour(f_normal) {
    mpq_init(data);
    mpq_set_si(data, value, 1);
}
template <bool withInfinity>
Rational::Rational(const IntegerBase<withInfinity>& num,
        const IntegerBase<withInfinity>& den) {
    mpq_init(data);
    if (den.isZero()) {
        if (num.isZero())
            flavour = f_undefined;
        else
            flavour = f_infinity;
    } else {
        flavour = f_normal;
        if (num.isNative() && den.isNative())
            mpq_set_si(data, num.longValue(), den.longValue());
        else if (num.isNative()) {
            // Avoid bloating num with a GMP representation.
            IntegerBase<withInfinity> tmp(num);
            mpz_set(mpq_numref(data), tmp.rawData());
            mpz_set(mpq_denref(data), den.rawData());
        } else if (den.isNative()) {
            // Avoid bloating den with a GMP representation.
            IntegerBase<withInfinity> tmp(den);
            mpz_set(mpq_numref(data), num.rawData());
            mpz_set(mpq_denref(data), tmp.rawData());
        } else {
            mpz_set(mpq_numref(data), num.rawData());
            mpz_set(mpq_denref(data), den.rawData());
        }
    }
}
inline Rational::~Rational() {
    mpq_clear(data);
}

inline Rational& Rational::operator = (const Rational& value) {
    // We assume that mpq_set() is fine with self-assignment, since:
    // - the GMP docs state that output and input variables can be the same;
    // - the libgmpxx classes do not special-case self-assignment.
    // The C++ test suite tests self-assignment of Rationals also.
    flavour = value.flavour;
    if (flavour == f_normal)
        mpq_set(data, value.data);
    return *this;
}
template <bool withInfinity>
inline Rational& Rational::operator = (const IntegerBase<withInfinity>& value) {
    if (value.isInfinite())
        flavour = f_infinity;
    else if (value.isNative()) {
        flavour = f_normal;
        mpq_set_si(data, value.longValue(), 1);
    } else {
        flavour = f_normal;
        mpq_set_z(data, value.rawData());
    }
    return *this;
}
inline Rational& Rational::operator = (long value) {
    flavour = f_normal;
    mpq_set_si(data, value, 1);
    return *this;
}

inline Rational& Rational::operator = (Rational&& src) noexcept {
    flavour = src.flavour;
    mpq_swap(data, src.data);
    // Let src dispose of the original mpq data.
    return *this;
}

inline void Rational::swap(Rational& other) noexcept {
    std::swap(flavour, other.flavour);
    // Note: GMP marks mpq_swap() as __GMP_NOTHROW.
    mpq_swap(data, other.data);
}

inline void Rational::negate() {
    if (flavour == f_normal)
        mpq_neg(data, data);
}

inline bool Rational::operator <= (const Rational& compare) const {
    return ! (*this > compare);
}
inline bool Rational::operator >= (const Rational& compare) const {
    return ! (*this < compare);
}
inline bool Rational::operator != (const Rational& compare) const {
    return ! (*this == compare);
}

inline void swap(Rational& a, Rational& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif
