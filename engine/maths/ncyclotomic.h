
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

#ifndef __NCYCLOTOMIC_H
#ifndef __DOXYGEN
#define __NCYCLOTOMIC_H
#endif

/*! \file maths/ncyclotomic.h
 *  \brief Implements exact arithmetic in cyclotomic fields.
 */

#include "regina-core.h"
#include "maths/npolynomial.h"
#include "maths/nrational.h"
#include <complex>

namespace regina {

/**
 * \weakgroup maths
 * @{
 */

/**
 * Represents an element of a cyclotomic field.
 *
 * The cyclotomic field of order \a n extends the rationals with a
 * primitive <i>n</i>th root of unity.  This is isomorphic to the
 * polynomial field <tt>ℚ[x]/Φ_n</tt>, where <tt>Φ_n</tt> is the <i>n</i>th
 * cyclotomic polynomial.
 *
 * Using this isomorphism, each element of the cyclotomic field can be
 * uniquely represented as a rational polynomial of degree strictly less than
 * <tt>deg(Φ_n) = φ(n)</tt>, where <tt>φ</tt> denotes Euler's totient function.
 *
 * Each object of this class stores both the value of the field element
 * and the order \a n of the underlying field.  This means that you can
 * freely work with elements of different fields simultaneously, though of
 * course most operations (such as addition, multplication and so on)
 * require all operands to belong to the same field.
 *
 * This class requires that the order \a n is strictly positive.
 */
class REGINA_API NCyclotomic {
    private:
        size_t field_;
            /**< The order \a n of the underlying cyclotomic field.
                 This is strictly positive if the element has initialised,
                 or zero if not. */
        size_t degree_;
            /**< The degree of the underlying cyclotomic polynomial,
                 which is equal to <tt>φ(field_)</tt>.
                 This is strictly positive if the element has been
                 initialised, or zero if not. */
        NRational* coeff_;
            /**< An array of size \a degree_ that stores the coefficients of
                 the polynomial representation of this field element.
                 If this element has not been initialised then this will
                 be the null pointer. */

    public:
        /**
         * Creates an uninitialised field element.
         *
         * This element must be initialised using either init() or the
         * assignment operator before it can be used.
         *
         * The underlying cyclotomic field is not yet known; this will also
         * be specified during the call to init() or the assignment operator.
         */
        NCyclotomic();
        /**
         * Creates the zero element of the given cyclotomic field.
         *
         * @param field the order of the underlying cyclotomic field;
         * this must be strictly positive.
         */
        explicit NCyclotomic(size_t field);
        /**
         * Creates the given integer element within the given cyclotomic field.
         *
         * The polynomial representation of this element will simply be an
         * integer constant.
         *
         * @param field the order of the underlying cyclotomic field;
         * this must be strictly positive.
         * @param value the value of this element; that is, the integer
         * constant.
         */
        NCyclotomic(size_t field, int value);
        /**
         * Creates the given rational element within the given cyclotomic field.
         *
         * The polynomial representation of this element will simply be a
         * rational constant.
         *
         * @param field the order of the underlying cyclotomic field;
         * this must be strictly positive.
         * @param value the value of this element; that is, the rational
         * constant.
         */
        NCyclotomic(size_t field, const NRational& value);
        /**
         * Creates a copy of the given field element, within the
         * same cyclotomic field.
         *
         * @param value the field element to copy.
         */
        NCyclotomic(const NCyclotomic& value);
        /**
         * Destroys this field element.
         *
         * This is safe even if the field element was never initialised.
         */
        ~NCyclotomic();
        /**
         * Initialises this to be the zero element of the given
         * cyclotomic field.
         *
         * This is safe even if this element was previously initialised
         * as an element of a \e different field - all prior information
         * about this field element will be safely discarded.
         *
         * @param field the order of the cyclotomic field to which this
         * field element will now belong; this must be strictly positive.
         */
        void init(size_t field);
        /**
         * Returns the order \a n of the underlying cyclotomic field to
         * which this element belongs.
         *
         * A value of zero indicates that this field element has not yet
         * been initialised (for instance, it was created using the
         * default constructor).
         *
         * @return the order of the underlying cyclotomic field.
         */
        size_t field() const;
        /**
         * TODO
         */
        size_t degree() const;
        /**
         * TODO
         */
        const NRational& operator [] (size_t exp) const;
        /**
         * TODO
         */
        NRational& operator [] (size_t exp);
        /**
         * TODO
         */
        NPolynomial<NRational>* polynomial() const;
        /**
         * TODO
         */
        std::complex<double> evaluate(size_t whichRoot) const;

        /**
         * Tests whether or not this and the given argument are the same
         * element of the same cyclotomic field.
         *
         * If this and \a rhs have different underlying fields then
         * this test will always return \c false, even if they take the
         * same numerical value when evaluated as complex numbers.
         *
         * @param rhs the value to compare with this.
         * @return \c true if and only if this and \a rhs are the same
         * element of the same cyclotomic field.
         */
        bool operator == (const NCyclotomic& rhs) const;

        /**
         * Tests whether or not this and the given argument are the same
         * element of the same cyclotomic field.
         *
         * If this and \a rhs have different underlying fields then
         * this test will always return \c true (indicating that the
         * elements are not equal), even if they take the same numerical
         * value when evaluated as complex numbers.
         *
         * @param rhs the value to compare with this.
         * @return \c false if this and \a rhs are the same element of the
         * same cyclotomic field, or \c true if they are not.
         */
        bool operator != (const NCyclotomic& rhs) const;

        /**
         * Sets this to a copy of the given field element.
         *
         * If this and \a value belong to different cyclotomic fields,
         * then the underlying field for this element will be changed to
         * match the field for \a value.
         *
         * @param value the new value to assign to this field element.
         * @return a reference to this field element.
         */
        NCyclotomic& operator = (const NCyclotomic& value);

        /**
         * Sets this field element to the given rational.
         * The underlying cyclotomic field will be left unchanged.
         *
         * The polynomial representation for this field element will
         * simply be a constant.
         *
         * @param scalar the new rational value of this field element.
         * @return a reference to this field element.
         */
        NCyclotomic& operator = (const NRational& scalar);

        /**
         * Negates this field element.
         */
        void negate();

        /**
         * Inverts this field element.
         *
         * \pre This field element is non-zero.
         */
        void invert();

        /**
         * Multiplies this field element by the given rational.
         *
         * This has the effect of multiplying the polynomial representation
         * by a scalar constant.
         *
         * @param the rational to multiply this by.
         * @return a reference to this field element.
         */
        NCyclotomic& operator *= (const NRational& scalar);

        /**
         * Divides this field element by the given rational.
         *
         * This has the effect of dividing the polynomial representation
         * by a scalar constant.
         *
         * \pre The given rational is non-zero.
         *
         * @param the rational to divide this by.
         * @return a reference to this field element.
         */
        NCyclotomic& operator /= (const NRational& scalar);

        /**
         * Adds the given field element to this.
         *
         * \pre The argument \a other belongs to the same cyclotomic field
         * as this.
         *
         * @param other the field element to add to this.
         * @return a reference to this field element.
         */
        NCyclotomic& operator += (const NCyclotomic& other);

        /**
         * Subtracts the given field element from this.
         *
         * \pre The argument \a other belongs to the same cyclotomic field
         * as this.
         *
         * @param other the field element to subtract from this.
         * @return a reference to this field element.
         */
        NCyclotomic& operator -= (const NCyclotomic& other);

        /**
         * Multiplies this by the given field element.
         *
         * \pre The argument \a other belongs to the same cyclotomic field
         * as this.
         *
         * @param other the field element to multiply this by.
         * @return a reference to this field element.
         */
        NCyclotomic& operator *= (const NCyclotomic& other);

        /**
         * Divides this by the given field element.
         *
         * \pre The argument \a other is non-zero.
         * \pre The argument \a other belongs to the same cyclotomic field
         * as this.
         *
         * @param other the field element to divide this by.
         * @return a reference to this field element.
         */
        NCyclotomic& operator /= (const NCyclotomic& other);

        /**
         * Returns the <i>n</i>th cyclotomic polynomial <tt>Φ_n</tt>.
         *
         * Cyclotomic polynomials are cached after they are computed, and
         * so after the first call to <tt>cyclotomic(n)</tt>, all subsequent
         * calls with the same value of \a n will be essentially instantaneous.
         *
         * \pre The given integer \a n must be strictly positive.
         *
         * \ifacespython This routine returns a newly allocated polynomial
         * (not a constant reference).  Moreover, since Python exposes the
         * class NPolynomial<NRational> but not NPolynomial<NInteger>, this
         * routine returns an object of type NPolynomial<NRational> instead.
         *
         * @param n indicates which cyclotomic polynomial to return.
         * @return the cyclotomic polynomial <tt>Φ_n</tt>.
         */
        static const NPolynomial<NInteger>& cyclotomic(size_t n);
};

/**
 * Writes the given field element to the given output stream in
 * human-readable form.
 *
 * The field element will be written using its rational polynomial
 * representation.  The underlying field will \e not be indicated in the
 * output, since this is often already understood.  If required, it can be
 * accessed by calling <tt>c.field()</tt>.
 *
 * @param out the output stream to which to write.
 * @param c the field element to write.
 * @return a reference to the given output stream.
 */
REGINA_API std::ostream& operator << (std::ostream& out, const NCyclotomic& c);

/*@}*/

// Inline functions for NCyclotomic

inline NCyclotomic::NCyclotomic() : field_(0), degree_(0), coeff_(0) {
}

inline NCyclotomic::NCyclotomic(size_t field) :
        field_(field), degree_(cyclotomic(field).degree()),
        coeff_(new NRational[degree_]) {
    // NRational initialises to 0 by default.
}

inline NCyclotomic::NCyclotomic(size_t field, int value) :
        field_(field), degree_(cyclotomic(field).degree()),
        coeff_(new NRational[degree_]) {
    // NRational initialises to 0 by default.
    coeff_[0] = value;
}

inline NCyclotomic::NCyclotomic(size_t field, const NRational& value) :
        field_(field), degree_(cyclotomic(field).degree()),
        coeff_(new NRational[degree_]) {
    // NRational initialises to 0 by default.
    coeff_[0] = value;
}

inline NCyclotomic::NCyclotomic(const NCyclotomic& value) :
        field_(value.field_), degree_(value.degree_),
        coeff_(new NRational[value.degree_]) {
    for (size_t i = 0; i < degree_; ++i)
        coeff_[i] = value.coeff_[i];
}

inline NCyclotomic::~NCyclotomic() {
    delete[] coeff_;
}

inline void NCyclotomic::init(size_t field) {
    delete[] coeff_;
    field_ = field;
    degree_ = cyclotomic(field).degree();
    coeff_ = new NRational[degree_];
    // NRational initialises to 0 by default.
}

inline size_t NCyclotomic::field() const {
    return field_;
}

inline size_t NCyclotomic::degree() const {
    return degree_;
}

inline const NRational& NCyclotomic::operator [] (size_t exp) const {
    return coeff_[exp];
}

inline NRational& NCyclotomic::operator [] (size_t exp) {
    return coeff_[exp];
}

inline NPolynomial<NRational>* NCyclotomic::polynomial() const {
    return new NPolynomial<NRational>(coeff_, coeff_ + degree_);
}

inline bool NCyclotomic::operator == (const NCyclotomic& rhs) const {
    if (field_ != rhs.field_)
        return false;
    for (size_t i = 0; i < degree_; ++i)
        if (coeff_[i] != rhs.coeff_[i])
            return false;
    return true;
}

inline bool NCyclotomic::operator != (const NCyclotomic& rhs) const {
    if (field_ != rhs.field_)
        return true;
    for (size_t i = 0; i < degree_; ++i)
        if (coeff_[i] != rhs.coeff_[i])
            return true;
    return false;
}

inline NCyclotomic& NCyclotomic::operator = (const NCyclotomic& other) {
    if (degree_ < other.degree_) {
        delete[] coeff_;
        coeff_ = new NRational[other.degree_];
    }
    field_ = other.field_;
    degree_ = other.degree_;
    for (size_t i = 0; i < degree_; ++i)
        coeff_[i] = other.coeff_[i];
    return *this;
}

inline NCyclotomic& NCyclotomic::operator = (const NRational& scalar) {
    coeff_[0] = scalar;
    for (size_t i = 1; i < degree_; ++i)
        coeff_[i] = 0;
    return *this;
}

inline void NCyclotomic::negate() {
    for (size_t i = 0; i < degree_; ++i)
        coeff_[i].negate();
}

inline NCyclotomic& NCyclotomic::operator *= (const NRational& scalar) {
    for (size_t i = 0; i < degree_; ++i)
        coeff_[i] *= scalar;
    return *this;
}

inline NCyclotomic& NCyclotomic::operator /= (const NRational& scalar) {
    for (size_t i = 0; i < degree_; ++i)
        coeff_[i] /= scalar;
    return *this;
}

inline NCyclotomic& NCyclotomic::operator += (const NCyclotomic& other) {
    for (size_t i = 0; i < degree_; ++i)
        coeff_[i] += other.coeff_[i];
    return *this;
}

inline NCyclotomic& NCyclotomic::operator -= (const NCyclotomic& other) {
    for (size_t i = 0; i < degree_; ++i)
        coeff_[i] -= other.coeff_[i];
    return *this;
}

inline NCyclotomic& NCyclotomic::operator /= (const NCyclotomic& other) {
    NCyclotomic tmp(other);
    tmp.invert();
    return (*this) *= tmp;
}

} // namespace regina

#endif
