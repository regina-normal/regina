
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#ifndef __CYCLOTOMIC_H
#ifndef __DOXYGEN
#define __CYCLOTOMIC_H
#endif

/*! \file maths/cyclotomic.h
 *  \brief Implements exact arithmetic in cyclotomic fields.
 */

#include "regina-core.h"
#include "maths/polynomial.h"
#include "maths/rational.h"
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
 * This class stores field elements using such a polynomial representation,
 * and does \e not store complex numbers directly.  If you require the
 * complex value of a field element (as a floating point approximation),
 * you can call evaluate().
 *
 * Each object of this class stores both the value of the field element
 * and the order \a n of the underlying field.  This means that you can
 * freely work with elements of different fields simultaneously, though of
 * course most operations (such as addition, multplication and so on)
 * require all operands to belong to the same field.
 *
 * This class requires that the order \a n is strictly positive.
 *
 * Although this class makes use of global data in its implementation, all
 * of its methods are thread-safe.
 */
class REGINA_API Cyclotomic : public ShortOutput<Cyclotomic, true> {
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
        Rational* coeff_;
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
        Cyclotomic();
        /**
         * Creates the zero element of the given cyclotomic field.
         *
         * @param field the order of the underlying cyclotomic field;
         * this must be strictly positive.
         */
        explicit Cyclotomic(size_t field);
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
        Cyclotomic(size_t field, int value);
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
        Cyclotomic(size_t field, const Rational& value);
        /**
         * Creates a copy of the given field element, within the
         * same cyclotomic field.
         *
         * @param value the field element to copy.
         */
        Cyclotomic(const Cyclotomic& value);
        /**
         * Destroys this field element.
         *
         * This is safe even if the field element was never initialised.
         */
        ~Cyclotomic();
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
         * Returns the degree of the polynomial that defines the
         * underlying cyclotomic field.
         *
         * This is the degree of the cyclotomic polynomial <tt>Φ_n</tt>,
         * and also the value of Euler's totient function <tt>φ(n)</tt>,
         * where \a n is the order of the field as returned by field().
         *
         * A value of zero indicates that this field element has not yet
         * been initialised (for instance, it was created using the
         * default constructor).
         *
         * @return the degree of the polynomial that defines the
         * underlying field.
         */
        size_t degree() const;
        /**
         * Returns an individual rational coefficient of the
         * polynomial representation of this field element.
         *
         * The polynomial representation expresses this field element
         * as a member of <tt>ℚ[x]/Φ_n</tt>, using a rational polynomial
         * of degree strictly less than <tt>deg(Φ_n) = φ(n)</tt>;
         * that is, strictly less than the value returned by degree().
         * See the Cyclotomic class notes for further details.
         *
         * In particular, for a field element \a e, the operator
         * <tt>e[i]</tt> will return the coefficient of <tt>x^i</tt>
         * in this polynomial representation.
         *
         * This is a constant (read-only) routine; note that there is a
         * non-constant (read-write) variant of this routine also.
         *
         * @param exp indicates which coefficient to return; this must
         * be between 0 and degree()-1 inclusive.
         * @return a constant reference to the corresponding
         * rational coefficient.
         */
        const Rational& operator [] (size_t exp) const;
        /**
         * Offers access to an individual rational coefficient of the
         * polynomial representation of this field element.
         *
         * The polynomial representation expresses this field element
         * as a member of <tt>ℚ[x]/Φ_n</tt>, using a rational polynomial
         * of degree strictly less than <tt>deg(Φ_n) = φ(n)</tt>;
         * that is, strictly less than the value returned by degree().
         * See the Cyclotomic class notes for further details.
         *
         * In particular, for a field element \a e, the operator
         * <tt>e[i]</tt> will give access to the coefficient of <tt>x^i</tt>
         * in this polynomial representation.
         *
         * This routine returns a non-constant reference: you can use
         * this to directly edit the coefficients (and therefore the value of
         * the field element).  Note that there is also a constant (read-only)
         * variant of this routine.
         *
         * @param exp indicates which coefficient to access; this must
         * be between 0 and degree()-1 inclusive.
         * @return a reference to the corresponding rational coefficient.
         */
        Rational& operator [] (size_t exp);
        /**
         * Returns the full polynomial representation of this field element.
         *
         * The polynomial representation expresses this field element
         * as a member of <tt>ℚ[x]/Φ_n</tt>, using a rational polynomial
         * of degree strictly less than <tt>deg(Φ_n) = φ(n)</tt>;
         * that is, strictly less than the value returned by degree().
         * See the Cyclotomic class notes for further details.
         *
         * This routine returns the polynomial representation as a newly
         * allocated Polynomial<Rational> object.  The caller of this
         * routine is responsible for destroying this new polynomial.
         *
         * The new polynomial will become independent of this Cyclotomic field
         * element: if you subsequently change this field element then the
         * new Polynomial object will not change, and likewise if you
         * change the new Polynomial object then this Cyclotomic field
         * element will not change.
         *
         * \pre This field element has been initialised (either through
         * a non-default constructor, an assignment operator, or by
         * calling init()).
         *
         * @return a new polynomial giving the full polynomial
         * representation of this field element.
         */
        Polynomial<Rational>* polynomial() const;
        /**
         * Returns the value of this cyclotomic field element as a
         * complex number.
         *
         * The evaluation depends upon \e which primitive root of unity
         * is used to build the underlying cyclotomic field of order \a n.
         * This ambiguity is resolved as follows.
         *
         * Suppose the polynomial representation of this field element in
         * <tt>ℚ[x]/Φ_n</tt> (as described in the Cyclotomic class notes) is
         * <tt>f(x)</tt>.  Then the evaluation of this field element will be
         * <tt>f(ρ)</tt>, where \a ρ is the <tt>n</tt>th root of unity
         * <tt>ρ = exp(2πi × k/n)</tt>,
         * and where \a k is the argument \e whichRoot as passed to this
         * routine.
         *
         * \pre The argument \e whichRoot is coprime to \a n (the order of
         * the underlying cyclotomic field).
         *
         * \pre This field element has been initialised (either through
         * a non-default constructor, an assignment operator, or by
         * calling init()).
         *
         * \warning This routine uses floating point arithmetic, and so the
         * value that it returns is subject to the usual floating point error.
         *
         * @param whichRoot indicates which root of unity will be used
         * to convert the polynomial representation of this field
         * element into a complex number.
         * @return a floating-point approximation of this cyclotomic field
         * element as a complex number.
         */
        std::complex<double> evaluate(size_t whichRoot = 1) const;

        /**
         * Tests whether or not this and the given argument are the same
         * element of the same cyclotomic field.
         *
         * If this and \a rhs have different underlying fields then
         * this test will always return \c false, even if they take the
         * same numerical value when evaluated as complex numbers.
         *
         * If either this or \a rhs have not been initialised (typically
         * because they were created using the default constructor),
         * then this comparison will return \c false.  If \e both field
         * elements have not been initialised, then this comparison will
         * return \c true.
         *
         * @param rhs the value to compare with this.
         * @return \c true if and only if this and \a rhs are the same
         * element of the same cyclotomic field.
         */
        bool operator == (const Cyclotomic& rhs) const;

        /**
         * Tests whether or not this and the given argument are the same
         * element of the same cyclotomic field.
         *
         * If this and \a rhs have different underlying fields then
         * this test will always return \c true (indicating that the
         * elements are not equal), even if they take the same numerical
         * value when evaluated as complex numbers.
         *
         * If either this or \a rhs have not been initialised (typically
         * because they were created using the default constructor),
         * then this comparison will return \c true.  If \e both field
         * elements have not been initialised, then this comparison will
         * return \c false.
         *
         * @param rhs the value to compare with this.
         * @return \c false if this and \a rhs are the same element of the
         * same cyclotomic field, or \c true if they are not.
         */
        bool operator != (const Cyclotomic& rhs) const;

        /**
         * Sets this to a copy of the given field element.
         *
         * This assignment operator is safe even if this and \a value belong
         * to different cyclotomic fields, or if this and/or \a value has not
         * yet been initialised.  The underlying field for this element will
         * simply be changed to match the underlying field for \a value,
         * and all old information stored for this element (if any) will
         * be safely discarded.  If \a value is uninitialised then this
         * field element will become uninitialised also.
         *
         * @param value the new value to assign to this field element.
         * @return a reference to this field element.
         */
        Cyclotomic& operator = (const Cyclotomic& value);

        /**
         * Sets this field element to the given rational.
         * The underlying cyclotomic field will be left unchanged.
         *
         * The polynomial representation for this field element will
         * simply be a constant.
         *
         * \pre This field element has already been initialised (and so
         * it already has specified an underlying cyclotomic field).
         *
         * @param scalar the new rational value of this field element.
         * @return a reference to this field element.
         */
        Cyclotomic& operator = (const Rational& scalar);

        /**
         * Negates this field element.
         */
        void negate();

        /**
         * Inverts this field element.
         *
         * \pre This field element has already been initialised (and so
         * it already has specified an underlying cyclotomic field).
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
         * @param scalar the rational to multiply this by.
         * @return a reference to this field element.
         */
        Cyclotomic& operator *= (const Rational& scalar);

        /**
         * Divides this field element by the given rational.
         *
         * This has the effect of dividing the polynomial representation
         * by a scalar constant.
         *
         * \pre The given rational is non-zero.
         *
         * @param scalar the rational to divide this by.
         * @return a reference to this field element.
         */
        Cyclotomic& operator /= (const Rational& scalar);

        /**
         * Adds the given field element to this.
         *
         * \pre The argument \a other belongs to the same cyclotomic field
         * as this.
         *
         * @param other the field element to add to this.
         * @return a reference to this field element.
         */
        Cyclotomic& operator += (const Cyclotomic& other);

        /**
         * Subtracts the given field element from this.
         *
         * \pre The argument \a other belongs to the same cyclotomic field
         * as this.
         *
         * @param other the field element to subtract from this.
         * @return a reference to this field element.
         */
        Cyclotomic& operator -= (const Cyclotomic& other);

        /**
         * Multiplies this by the given field element.
         *
         * \pre The argument \a other belongs to the same cyclotomic field
         * as this.
         *
         * @param other the field element to multiply this by.
         * @return a reference to this field element.
         */
        Cyclotomic& operator *= (const Cyclotomic& other);

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
        Cyclotomic& operator /= (const Cyclotomic& other);

        /**
         * Returns the <i>n</i>th cyclotomic polynomial <tt>Φ_n</tt>.
         *
         * Cyclotomic polynomials are cached after they are computed, and
         * so after the first call to <tt>cyclotomic(n)</tt>, all subsequent
         * calls with the same value of \a n will be essentially instantaneous.
         *
         * Although it queries and manipulates a global cache, this routine
         * is thread-safe.
         *
         * \pre The given integer \a n must be strictly positive.
         *
         * \ifacespython This routine returns a newly allocated polynomial
         * (not a constant reference).  Moreover, since Python exposes the
         * class Polynomial<Rational> but not Polynomial<Integer>, this
         * routine returns an object of type Polynomial<Rational> instead.
         *
         * @param n indicates which cyclotomic polynomial to return.
         * @return the cyclotomic polynomial <tt>Φ_n</tt>.
         */
        static const Polynomial<Integer>& cyclotomic(size_t n);

        /**
         * Writes this field element to the given output stream, using the
         * given variable name instead of \c x.
         *
         * The field element will be written using its rational polynomial
         * representation.  The underlying field will \e not be indicated in the
         * output, since this is often already understood.  If required, it can
         * be accessed by calling <tt>c.field()</tt>.
         *
         * If \a utf8 is passed as \c true then unicode superscript characters
         * will be used for exponents; these will be encoded using UTF-8.
         * This will make the output nicer, but will require more complex
         * fonts to be available on the user's machine.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         * @param utf8 \c true if unicode superscript characters may be used.
         * @param variable the symbol to use for the polynomial variable.
         * This may be \c null, in which case the default variable \c x
         * will be used.
         * @return a reference to the given output stream.
         */
        void writeTextShort(std::ostream& out, bool utf8 = false,
            const char* variable = 0) const;

        /**
         * Returns this field element as a human-readable string, using the
         * given variable name instead of \c x.
         *
         * The field element will be written using its rational polynomial
         * representation.  The underlying field will \e not be indicated in the
         * output, since this is often already understood.  If required, it can
         * be accessed by calling <tt>c.field()</tt>.
         *
         * \note There is also the usual variant of str() which takes no
         * arguments; that variant is inherited from the Output class.
         *
         * @param variable the symbol to use for the polynomial variable.
         * This may be \c null, in which case the default variable \c x
         * will be used.
         * @return this field element as a human-readable string.
         */
        std::string str(const char* variable) const;

        /**
         * Returns this field element as a human-readable string using
         * unicode characters, using the given variable name instead of \c x.
         *
         * The field element will be written using its rational polynomial
         * representation.  The underlying field will \e not be indicated in the
         * output, since this is often already understood.  If required, it can
         * be accessed by calling <tt>c.field()</tt>.
         *
         * This is similar to the output from str(), except that it uses
         * unicode characters to make the output more pleasant to read.
         * In particular, it makes use of superscript digits for exponents.
         *
         * The string is encoded in UTF-8.
         *
         * \note There is also the usual variant of utf8() which takes no
         * arguments; that variant is inherited from the Output class.
         *
         * @param variable the symbol to use for the polynomial variable.
         * This may be \c null, in which case the default variable \c x
         * will be used.
         * @return this field element as a unicode-enabled human-readable
         * string.
         */
        std::string utf8(const char* variable) const;
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NCyclotomic has now been renamed to Cyclotomic.
 */
REGINA_DEPRECATED typedef Cyclotomic NCyclotomic;

/*@}*/

// Inline functions for Cyclotomic

inline Cyclotomic::Cyclotomic() : field_(0), degree_(0), coeff_(0) {
}

inline Cyclotomic::Cyclotomic(size_t field) :
        field_(field), degree_(cyclotomic(field).degree()),
        coeff_(new Rational[degree_]) {
    // Rational initialises to 0 by default.
}

inline Cyclotomic::Cyclotomic(size_t field, int value) :
        field_(field), degree_(cyclotomic(field).degree()),
        coeff_(new Rational[degree_]) {
    // Rational initialises to 0 by default.
    coeff_[0] = value;
}

inline Cyclotomic::Cyclotomic(size_t field, const Rational& value) :
        field_(field), degree_(cyclotomic(field).degree()),
        coeff_(new Rational[degree_]) {
    // Rational initialises to 0 by default.
    coeff_[0] = value;
}

inline Cyclotomic::Cyclotomic(const Cyclotomic& value) :
        field_(value.field_), degree_(value.degree_),
        coeff_(new Rational[value.degree_]) {
    for (size_t i = 0; i < degree_; ++i)
        coeff_[i] = value.coeff_[i];
}

inline Cyclotomic::~Cyclotomic() {
    delete[] coeff_;
}

inline void Cyclotomic::init(size_t field) {
    delete[] coeff_;
    field_ = field;
    degree_ = cyclotomic(field).degree();
    coeff_ = new Rational[degree_];
    // Rational initialises to 0 by default.
}

inline size_t Cyclotomic::field() const {
    return field_;
}

inline size_t Cyclotomic::degree() const {
    return degree_;
}

inline const Rational& Cyclotomic::operator [] (size_t exp) const {
    return coeff_[exp];
}

inline Rational& Cyclotomic::operator [] (size_t exp) {
    return coeff_[exp];
}

inline Polynomial<Rational>* Cyclotomic::polynomial() const {
    return new Polynomial<Rational>(coeff_, coeff_ + degree_);
}

inline bool Cyclotomic::operator == (const Cyclotomic& rhs) const {
    if (field_ != rhs.field_)
        return false;
    for (size_t i = 0; i < degree_; ++i)
        if (coeff_[i] != rhs.coeff_[i])
            return false;
    return true;
}

inline bool Cyclotomic::operator != (const Cyclotomic& rhs) const {
    if (field_ != rhs.field_)
        return true;
    for (size_t i = 0; i < degree_; ++i)
        if (coeff_[i] != rhs.coeff_[i])
            return true;
    return false;
}

inline Cyclotomic& Cyclotomic::operator = (const Cyclotomic& other) {
    if (degree_ < other.degree_) {
        delete[] coeff_;
        coeff_ = new Rational[other.degree_];
    }
    field_ = other.field_;
    degree_ = other.degree_;
    for (size_t i = 0; i < degree_; ++i)
        coeff_[i] = other.coeff_[i];
    return *this;
}

inline Cyclotomic& Cyclotomic::operator = (const Rational& scalar) {
    coeff_[0] = scalar;
    for (size_t i = 1; i < degree_; ++i)
        coeff_[i] = 0;
    return *this;
}

inline void Cyclotomic::negate() {
    for (size_t i = 0; i < degree_; ++i)
        coeff_[i].negate();
}

inline Cyclotomic& Cyclotomic::operator *= (const Rational& scalar) {
    for (size_t i = 0; i < degree_; ++i)
        coeff_[i] *= scalar;
    return *this;
}

inline Cyclotomic& Cyclotomic::operator /= (const Rational& scalar) {
    for (size_t i = 0; i < degree_; ++i)
        coeff_[i] /= scalar;
    return *this;
}

inline Cyclotomic& Cyclotomic::operator += (const Cyclotomic& other) {
    for (size_t i = 0; i < degree_; ++i)
        coeff_[i] += other.coeff_[i];
    return *this;
}

inline Cyclotomic& Cyclotomic::operator -= (const Cyclotomic& other) {
    for (size_t i = 0; i < degree_; ++i)
        coeff_[i] -= other.coeff_[i];
    return *this;
}

inline Cyclotomic& Cyclotomic::operator /= (const Cyclotomic& other) {
    Cyclotomic tmp(other);
    tmp.invert();
    return (*this) *= tmp;
}

inline std::string Cyclotomic::str(const char* variable) const {
    // Make sure that python will be able to find the inherited str().
    static_assert(std::is_same<typename OutputBase<Cyclotomic>::type,
        Output<Cyclotomic, true>>::value,
        "Cyclotomic is not identified as being inherited from Output<...>");

    std::ostringstream out;
    writeTextShort(out, false, variable);
    return out.str();
}

inline std::string Cyclotomic::utf8(const char* variable) const {
    std::ostringstream out;
    writeTextShort(out, true, variable);
    return out.str();
}

} // namespace regina

#endif
