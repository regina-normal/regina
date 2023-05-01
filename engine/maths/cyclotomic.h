
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

#ifndef __REGINA_CYCLOTOMIC_H
#ifndef __DOXYGEN
#define __REGINA_CYCLOTOMIC_H
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
 * Represents an element of a cyclotomic field.
 *
 * The cyclotomic field of order \a n extends the rationals with a
 * primitive <i>n</i>th root of unity.  This is isomorphic to the
 * polynomial field `ℚ[x]/Φ_n`, where `Φ_n` is the <i>n</i>th
 * cyclotomic polynomial.
 *
 * Using this isomorphism, each element of the cyclotomic field can be
 * uniquely represented as a rational polynomial of degree strictly less than
 * `deg(Φ_n) = φ(n)`, where `φ` denotes Euler's totient function.
 * This class stores field elements using such a polynomial representation,
 * and does _not_ store complex numbers directly.  If you require the
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
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * Although this class makes use of global data in its implementation, all
 * of its methods are thread-safe.
 *
 * \ingroup maths
 */
class Cyclotomic : public ShortOutput<Cyclotomic, true> {
    public:
        using Coefficient = Rational;
            /**< The type of each coefficient of the polynomial that is
                 used to store a field element. */

    private:
        size_t field_;
            /**< The order \a n of the underlying cyclotomic field.
                 This is strictly positive if the element has initialised,
                 or zero if not. */
        size_t degree_;
            /**< The degree of the underlying cyclotomic polynomial,
                 which is equal to `φ(field_)`.
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
         * \param field the order of the underlying cyclotomic field;
         * this must be strictly positive.
         */
        explicit Cyclotomic(size_t field);
        /**
         * Creates the given integer element within the given cyclotomic field.
         *
         * The polynomial representation of this element will simply be an
         * integer constant.
         *
         * \param field the order of the underlying cyclotomic field;
         * this must be strictly positive.
         * \param value the value of this element; that is, the integer
         * constant.
         */
        Cyclotomic(size_t field, int value);
        /**
         * Creates the given rational element within the given cyclotomic field.
         *
         * The polynomial representation of this element will simply be a
         * rational constant.
         *
         * \param field the order of the underlying cyclotomic field;
         * this must be strictly positive.
         * \param value the value of this element; that is, the rational
         * constant.
         */
        Cyclotomic(size_t field, const Rational& value);
        /**
         * Creates a copy of the given field element, within the
         * same cyclotomic field.
         *
         * This constructor induces a deep copy of \a value.
         *
         * \param value the field element to copy.
         */
        Cyclotomic(const Cyclotomic& value);
        /**
         * Moves the contents of the given field element to this new
         * field element.  This is a fast (constant time) operation.
         *
         * The element that was passed (\a value) will no longer be usable.
         *
         * \param value the field element to move.
         */
        Cyclotomic(Cyclotomic&& value) noexcept;
        /**
         * Creates a new field element from the given sequence of coefficients.
         * The coefficients should describe the field element's polynomial
         * representation, and should be given in order from the constant
         * coefficient upwards.  See operator[] for details on what this
         * polynomial representation means.
         *
         * There should be at most `deg(Φ_n) = φ(n)` coefficients in
         * the list, where \a n is the given order of the underlying field;
         * any missing coefficients are assumed to be zero.  In particular,
         * an empty sequence is allowed (and represents the zero field element).
         *
         * \pre Rationals can be assigned values from dereferenced iterators
         * of type \a iterator.
         *
         * \python Instead of a pair of iterators, this routine
         * takes a python list of coefficients.
         *
         * \param field the order of the underlying cyclotomic field;
         * this must be strictly positive.
         * \param begin the beginning of a sequence of at most `φ(n)`
         * coefficients, as described above.
         * \param end a past-the-end iterator indicating the end of the
         * sequence of coefficients.
         */
        template <typename iterator>
        Cyclotomic(size_t field, iterator begin, iterator end);
        /**
         * Creates a new field element from a hard-coded sequence of
         * coefficients.  The coefficients should describe the field element's
         * polynomial representation, and should be given in order from the
         * constant coefficient upwards.  See operator[] for details on what
         * this polynomial representation means.
         *
         * There should be at most `deg(Φ_n) = φ(n)` coefficients in
         * the list, where \a n is the given order of the underlying field;
         * any missing coefficients are assumed to be zero.  In particular,
         * an empty sequence is allowed (and represents the zero field element).
         *
         * \nopython Instead, use the Python constructor that takes a list
         * of coefficients (which need not be constant).
         *
         * \param field the order of the underlying cyclotomic field;
         * this must be strictly positive.
         * \param coefficients a sequence of at most `φ(n)`
         * coefficients, as described above.
         */
        Cyclotomic(size_t field, std::initializer_list<Rational> coefficients);
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
         * as an element of a _different_ field - all prior information
         * about this field element will be safely discarded.
         *
         * \param field the order of the cyclotomic field to which this
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
         * \return the order of the underlying cyclotomic field.
         */
        size_t field() const;
        /**
         * Returns the degree of the polynomial that defines the
         * underlying cyclotomic field.
         *
         * This is the degree of the cyclotomic polynomial `Φ_n`,
         * and also the value of Euler's totient function `φ(n)`,
         * where \a n is the order of the field as returned by field().
         *
         * A value of zero indicates that this field element has not yet
         * been initialised (for instance, it was created using the
         * default constructor).
         *
         * \python This is also used to implement the Python special
         * method __len__().
         *
         * \return the degree of the polynomial that defines the
         * underlying field.
         */
        size_t degree() const;
        /**
         * Returns an individual rational coefficient of the
         * polynomial representation of this field element.
         *
         * The polynomial representation expresses this field element
         * as a member of `ℚ[x]/Φ_n`, using a rational polynomial
         * of degree strictly less than `deg(Φ_n) = φ(n)`;
         * that is, strictly less than the value returned by degree().
         * See the Cyclotomic class notes for further details.
         *
         * In particular, for a field element \a e, the operator
         * `e[i]` will return the coefficient of `x^i`
         * in this polynomial representation.
         *
         * This is a constant (read-only) routine; note that there is a
         * non-constant (read-write) variant of this routine also.
         *
         * \param exp indicates which coefficient to return; this must
         * be between 0 and degree()-1 inclusive.
         * \return a constant reference to the corresponding
         * rational coefficient.
         */
        const Rational& operator [] (size_t exp) const;
        /**
         * Offers access to an individual rational coefficient of the
         * polynomial representation of this field element.
         *
         * The polynomial representation expresses this field element
         * as a member of `ℚ[x]/Φ_n`, using a rational polynomial
         * of degree strictly less than `deg(Φ_n) = φ(n)`;
         * that is, strictly less than the value returned by degree().
         * See the Cyclotomic class notes for further details.
         *
         * In particular, for a field element \a e, the operator
         * `e[i]` will give access to the coefficient of `x^i`
         * in this polynomial representation.
         *
         * This routine returns a non-constant reference: you can use
         * this to directly edit the coefficients (and therefore the value of
         * the field element).  Note that there is also a constant (read-only)
         * variant of this routine.
         *
         * \param exp indicates which coefficient to access; this must
         * be between 0 and degree()-1 inclusive.
         * \return a reference to the corresponding rational coefficient.
         */
        Rational& operator [] (size_t exp);
        /**
         * Returns the full polynomial representation of this field element.
         *
         * The polynomial representation expresses this field element
         * as a member of `ℚ[x]/Φ_n`, using a rational polynomial
         * of degree strictly less than `deg(Φ_n) = φ(n)`;
         * that is, strictly less than the value returned by degree().
         * See the Cyclotomic class notes for further details.
         *
         * \pre This field element has been initialised (either through
         * a non-default constructor, an assignment operator, or by
         * calling init()).
         *
         * \return the full polynomial representation of this field element.
         */
        Polynomial<Rational> polynomial() const;
        /**
         * Returns the value of this cyclotomic field element as a
         * complex number.
         *
         * The evaluation depends upon _which_ primitive root of unity
         * is used to build the underlying cyclotomic field of order \a n.
         * This ambiguity is resolved as follows.
         *
         * Suppose the polynomial representation of this field element in
         * `ℚ[x]/Φ_n` (as described in the Cyclotomic class notes) is
         * `f(x)`.  Then the evaluation of this field element will be
         * `f(ρ)`, where \a ρ is the `n`th root of unity
         * `ρ = exp(2πi × k/n)`,
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
         * \param whichRoot indicates which root of unity will be used
         * to convert the polynomial representation of this field
         * element into a complex number.
         * \return a floating-point approximation of this cyclotomic field
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
         * then this comparison will return \c false.  If _both_ field
         * elements have not been initialised, then this comparison will
         * return \c true.
         *
         * \param rhs the value to compare with this.
         * \return \c true if and only if this and \a rhs are the same
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
         * then this comparison will return \c true.  If _both_ field
         * elements have not been initialised, then this comparison will
         * return \c false.
         *
         * \param rhs the value to compare with this.
         * \return \c false if this and \a rhs are the same element of the
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
         * This operator induces a deep copy of \a value.
         *
         * \param value the new value to assign to this field element.
         * \return a reference to this field element.
         */
        Cyclotomic& operator = (const Cyclotomic& value);

        /**
         * Moves the contents of the given field element to this
         * field element.  This is a fast (constant time) operation.
         *
         * This assignment operator is safe even if this and \a value belong
         * to different cyclotomic fields, or if this and/or \a value has not
         * yet been initialised.  The underlying field for this element will
         * simply be changed to match the underlying field for \a value,
         * and all old information stored for this element (if any) will
         * be safely discarded.  If \a value is uninitialised then this
         * field element will become uninitialised also.
         *
         * The element that was passed (\a value) will no longer be usable.
         *
         * \param value the field element to move.
         * \return a reference to this field element.
         */
        Cyclotomic& operator = (Cyclotomic&& value) noexcept;

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
         * \param scalar the new rational value of this field element.
         * \return a reference to this field element.
         */
        Cyclotomic& operator = (const Rational& scalar);

        /**
         * Swaps the contents of this and the given field element.
         * This is a fast (constant time) operation.
         *
         * This and the given field element do not need to belong to the
         * same cyclotomic field, and indeed one or both of them may be
         * uninitialised.  The underlying fields (if different) will be
         * swapped accordingly.
         *
         * \param other the field element whose contents should be swapped
         * with this.
         */
        void swap(Cyclotomic& other) noexcept;

        /**
         * Negates this field element.
         * This field element is changed directly.
         */
        void negate();

        /**
         * Inverts this field element.
         * This field element is changed directly.
         *
         * \pre This field element has already been initialised (and so
         * it already has specified an underlying cyclotomic field).
         *
         * \pre This field element is non-zero.
         */
        void invert();

        /**
         * Returns the inverse of this field element.
         * This field element is not changed.
         *
         * \pre This field element has already been initialised (and so
         * it already has specified an underlying cyclotomic field).
         *
         * \pre This field element is non-zero.
         */
        Cyclotomic inverse() const;

        /**
         * Multiplies this field element by the given rational.
         *
         * This has the effect of multiplying the polynomial representation
         * by a scalar constant.
         *
         * \param scalar the rational to multiply this by.
         * \return a reference to this field element.
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
         * \param scalar the rational to divide this by.
         * \return a reference to this field element.
         */
        Cyclotomic& operator /= (const Rational& scalar);

        /**
         * Adds the given field element to this.
         *
         * \pre The argument \a other belongs to the same cyclotomic field
         * as this.
         *
         * \param other the field element to add to this.
         * \return a reference to this field element.
         */
        Cyclotomic& operator += (const Cyclotomic& other);

        /**
         * Subtracts the given field element from this.
         *
         * \pre The argument \a other belongs to the same cyclotomic field
         * as this.
         *
         * \param other the field element to subtract from this.
         * \return a reference to this field element.
         */
        Cyclotomic& operator -= (const Cyclotomic& other);

        /**
         * Multiplies this by the given field element.
         *
         * \pre The argument \a other belongs to the same cyclotomic field
         * as this.
         *
         * \param other the field element to multiply this by.
         * \return a reference to this field element.
         */
        Cyclotomic& operator *= (const Cyclotomic& other);

        /**
         * Divides this by the given field element.
         *
         * \pre The argument \a other is non-zero.
         * \pre The argument \a other belongs to the same cyclotomic field
         * as this.
         *
         * \param other the field element to divide this by.
         * \return a reference to this field element.
         */
        Cyclotomic& operator /= (const Cyclotomic& other);

        /**
         * Returns the <i>n</i>th cyclotomic polynomial `Φ_n`.
         *
         * Cyclotomic polynomials are cached after they are computed, and
         * so after the first call to `cyclotomic(n)`, all subsequent
         * calls with the same value of \a n will be essentially instantaneous.
         *
         * Although it queries and manipulates a global cache, this routine
         * is thread-safe.
         *
         * \pre The given integer \a n must be strictly positive.
         *
         * \python Since Python exposes the class Polynomial<Rational>
         * but not Polynomial<Integer>, this routine will convert the result
         * to a Polynomial<Rational> (and will therefore return by value,
         * not by reference).
         *
         * \param n indicates which cyclotomic polynomial to return.
         * \return the cyclotomic polynomial `Φ_n`.
         */
        static const Polynomial<Integer>& cyclotomic(size_t n);

        /**
         * Writes this field element to the given output stream, using the
         * given variable name instead of \c x.
         *
         * The field element will be written using its rational polynomial
         * representation.  The underlying field will _not_ be indicated in the
         * output, since this is often already understood.  If required, it can
         * be accessed by calling `c.field()`.
         *
         * If \a utf8 is passed as \c true then unicode superscript characters
         * will be used for exponents; these will be encoded using UTF-8.
         * This will make the output nicer, but will require more complex
         * fonts to be available on the user's machine.
         *
         * \nopython Use str() or utf8() instead.
         *
         * \param out the output stream to which to write.
         * \param utf8 \c true if unicode superscript characters may be used.
         * \param variable the symbol to use for the polynomial variable.
         * This may be \c null, in which case the default variable \c x
         * will be used.
         */
        void writeTextShort(std::ostream& out, bool utf8 = false,
            const char* variable = nullptr) const;

        /**
         * Returns this field element as a human-readable string, using the
         * given variable name instead of \c x.
         *
         * The field element will be written using its rational polynomial
         * representation.  The underlying field will _not_ be indicated in the
         * output, since this is often already understood.  If required, it can
         * be accessed by calling `c.field()`.
         *
         * \note There is also the usual variant of str() which takes no
         * arguments; that variant is inherited from the Output class.
         *
         * \param variable the symbol to use for the polynomial variable.
         * This may be \c null, in which case the default variable \c x
         * will be used.
         * \return this field element as a human-readable string.
         */
        std::string str(const char* variable) const;

        /**
         * Returns this field element as a human-readable string using
         * unicode characters, using the given variable name instead of \c x.
         *
         * The field element will be written using its rational polynomial
         * representation.  The underlying field will _not_ be indicated in the
         * output, since this is often already understood.  If required, it can
         * be accessed by calling `c.field()`.
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
         * \param variable the symbol to use for the polynomial variable.
         * This may be \c null, in which case the default variable \c x
         * will be used.
         * \return this field element as a unicode-enabled human-readable
         * string.
         */
        std::string utf8(const char* variable) const;

    private:
        /**
         * Constructs a new field element with the given data.
         *
         * The data members \a field_, \a degree_ and \a coeff_ will be
         * set to the given values; in particular, the new object will
         * take ownership of the coefficient array.
         */
        Cyclotomic(size_t field, size_t degree, Rational* coeff);

    friend Cyclotomic operator + (const Cyclotomic& lhs, const Cyclotomic& rhs);
    friend Cyclotomic operator - (const Cyclotomic& lhs, const Cyclotomic& rhs);
    friend Cyclotomic operator * (const Cyclotomic& lhs, const Cyclotomic& rhs);
};

/**
 * Swaps the contents of the given field elements.
 *
 * This global routine simply calls Cyclotomic::swap(); it is provided
 * so that Cyclotomic meets the C++ Swappable requirements.
 *
 * \param a the first field element whose contents should be swapped.
 * \param b the second field element whose contents should be swapped.
 *
 * \ingroup maths
 */
void swap(Cyclotomic& a, Cyclotomic& b) noexcept;

/**
 * Multiplies the given field element by the given rational.
 *
 * \param elt the field element to multiply by.
 * \param scalar the rational to multiply by.
 * \return the product of the given field element and rational.
 *
 * \ingroup maths
 */
Cyclotomic operator * (Cyclotomic elt, const Rational& scalar);

/**
 * Multiplies the given field element by the given rational.
 *
 * \param scalar the rational to multiply by.
 * \param elt the field element to multiply by.
 * \return the product of the given field element and rational.
 *
 * \ingroup maths
 */
Cyclotomic operator * (const Rational& scalar, Cyclotomic elt);

/**
 * Divides the given field element by the given rational.
 *
 * \pre The argument \a scalar is non-zero.
 *
 * \param elt the field element to divide by the given rational.
 * \param scalar the rational to divide by.
 * \return the quotient of the given field element by the given rational.
 *
 * \ingroup maths
 */
Cyclotomic operator / (Cyclotomic elt, const Rational& scalar);

/**
 * Adds the two given cyclotomic field elements.
 *
 * \pre Both arguments belong to the same cyclotomic field.
 *
 * \param lhs the first field element to add.
 * \param rhs the second field element to add.
 * \return the sum of both field elements.
 *
 * \ingroup maths
 */
Cyclotomic operator + (const Cyclotomic& lhs, const Cyclotomic& rhs);

/**
 * Adds the two given cyclotomic field elements.
 *
 * \pre Both arguments belong to the same cyclotomic field.
 *
 * \param lhs the first field element to add.
 * \param rhs the second field element to add.
 * \return the sum of both field elements.
 *
 * \ingroup maths
 */
Cyclotomic operator + (Cyclotomic&& lhs, const Cyclotomic& rhs);

/**
 * Adds the two given cyclotomic field elements.
 *
 * \pre Both arguments belong to the same cyclotomic field.
 *
 * \param lhs the first field element to add.
 * \param rhs the second field element to add.
 * \return the sum of both field elements.
 *
 * \ingroup maths
 */
Cyclotomic operator + (const Cyclotomic& lhs, Cyclotomic&& rhs);

/**
 * Adds the two given cyclotomic field elements.
 *
 * \pre Both arguments belong to the same cyclotomic field.
 *
 * \param lhs the first field element to add.
 * \param rhs the second field element to add.
 * \return the sum of both field elements.
 *
 * \ingroup maths
 */
Cyclotomic operator + (Cyclotomic&& lhs, Cyclotomic&& rhs);

/**
 * Returns the negative of the given field element.
 *
 * \param arg the field element to negate.
 * \return the negative of \a arg.
 *
 * \ingroup maths
 */
Cyclotomic operator - (Cyclotomic arg);

/**
 * Subtracts the two given cyclotomic field elements.
 *
 * \pre Both arguments belong to the same cyclotomic field.
 *
 * \param lhs the field element to subtract from.
 * \param rhs the field element to subtract.
 * \return the first field element minus the second.
 *
 * \ingroup maths
 */
Cyclotomic operator - (const Cyclotomic& lhs, const Cyclotomic& rhs);

/**
 * Subtracts the two given cyclotomic field elements.
 *
 * \pre Both arguments belong to the same cyclotomic field.
 *
 * \param lhs the field element to subtract from.
 * \param rhs the field element to subtract.
 * \return the first field element minus the second.
 *
 * \ingroup maths
 */
Cyclotomic operator - (Cyclotomic&& lhs, const Cyclotomic& rhs);

/**
 * Subtracts the two given cyclotomic field elements.
 *
 * \pre Both arguments belong to the same cyclotomic field.
 *
 * \param lhs the field element to subtract from.
 * \param rhs the field element to subtract.
 * \return the first field element minus the second.
 *
 * \ingroup maths
 */
Cyclotomic operator - (const Cyclotomic& lhs, Cyclotomic&& rhs);

/**
 * Subtracts the two given cyclotomic field elements.
 *
 * \pre Both arguments belong to the same cyclotomic field.
 *
 * \param lhs the field element to subtract from.
 * \param rhs the field element to subtract.
 * \return the first field element minus the second.
 *
 * \ingroup maths
 */
Cyclotomic operator - (Cyclotomic&& lhs, Cyclotomic&& rhs);

/**
 * Multiplies the two given cyclotomic field elements.
 *
 * \pre Both arguments belong to the same cyclotomic field.
 *
 * \param lhs the first field element to multiply.
 * \param rhs the second field element to multiply.
 * \return the product of both field elements.
 *
 * \ingroup maths
 */
Cyclotomic operator * (const Cyclotomic& lhs, const Cyclotomic& rhs);

/**
 * Divides the two given cyclotomic field elements.
 *
 * \pre The second argument \a rhs is non-zero.
 * \pre Both arguments belong to the same cyclotomic field.
 *
 * \param lhs the field element to divide by \a rhs.
 * \param rhs the field element to divide \a lhs by.
 * \return the result of dividing \a lhs by \a rhs.
 *
 * \ingroup maths
 */
Cyclotomic operator / (const Cyclotomic& lhs, const Cyclotomic& rhs);

// Inline functions for Cyclotomic

inline Cyclotomic::Cyclotomic() : field_(0), degree_(0), coeff_(nullptr) {
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
    // std::cerr << "Cyclotomic: deep copy (init)" << std::endl;
    for (size_t i = 0; i < degree_; ++i)
        coeff_[i] = value.coeff_[i];
}

inline Cyclotomic::Cyclotomic(Cyclotomic&& value) noexcept :
        field_(value.field_), degree_(value.degree_), coeff_(value.coeff_) {
    value.coeff_ = nullptr;
}

inline Cyclotomic::Cyclotomic(size_t field, size_t degree, Rational* coeff) :
        field_(field), degree_(degree), coeff_(coeff) {
}

template <typename iterator>
inline Cyclotomic::Cyclotomic(size_t field, iterator begin, iterator end) :
        field_(field), degree_(cyclotomic(field).degree()),
        coeff_(new Rational[degree_]) {
    // Rationals initialise to 0, so a shorter list of coefficients is ok.
    std::copy(begin, end, coeff_);
}

inline Cyclotomic::Cyclotomic(size_t field,
        std::initializer_list<Rational> coefficients) :
        field_(field), degree_(cyclotomic(field).degree()),
        coeff_(new Rational[degree_]) {
    // Rationals initialise to 0, so a shorter list of coefficients is ok.
    std::copy(coefficients.begin(), coefficients.end(), coeff_);
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

inline Polynomial<Rational> Cyclotomic::polynomial() const {
    return Polynomial<Rational>(coeff_, coeff_ + degree_);
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

// Self-assignment works, assuming Rational's self-assignment works.
// NOLINTNEXTLINE(bugprone-unhandled-self-assignment)
inline Cyclotomic& Cyclotomic::operator = (const Cyclotomic& other) {
    // std::cerr << "Cyclotomic: deep copy (=)" << std::endl;
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

inline Cyclotomic& Cyclotomic::operator = (Cyclotomic&& other) noexcept {
    field_ = other.field_;
    degree_ = other.degree_;
    // Let other dispose of the original coefficients in its own destructor.
    std::swap(coeff_, other.coeff_);
    return *this;
}

inline Cyclotomic& Cyclotomic::operator = (const Rational& scalar) {
    coeff_[0] = scalar;
    for (size_t i = 1; i < degree_; ++i)
        coeff_[i] = 0;
    return *this;
}

inline void Cyclotomic::swap(Cyclotomic& other) noexcept {
    std::swap(field_, other.field_);
    std::swap(degree_, other.degree_);
    std::swap(coeff_, other.coeff_);
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
    return (*this) *= other.inverse();
}

inline std::string Cyclotomic::str(const char* variable) const {
    // Make sure that python will be able to find the inherited str().
    static_assert(std::is_same_v<typename OutputBase<Cyclotomic>::type,
        Output<Cyclotomic, true>>,
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

inline void swap(Cyclotomic& a, Cyclotomic& b) noexcept {
    a.swap(b);
}

inline Cyclotomic operator * (Cyclotomic elt, const Rational& scalar) {
    // When the argument elt is an lvalue reference, we perform a deep copy
    // due to pass-by-value.  If scalar == 0 then we don't need this deep copy,
    // since the argument can be ignored.  This special-case optimisation
    // would require two different lvalue/rvalue implementations of *, and
    // so we leave it for now.
    elt *= scalar;
    return elt;
}

inline Cyclotomic operator * (const Rational& scalar, Cyclotomic elt) {
    // See the notes above on a possible optimisation for scalar == 0.
    elt *= scalar;
    return elt;
}

inline Cyclotomic operator / (Cyclotomic elt, const Rational& scalar) {
    elt /= scalar;
    return elt;
}

inline Cyclotomic operator + (const Cyclotomic& lhs, const Cyclotomic& rhs) {
    // std::cerr << "Cyclotomic: deep copy (const +)" << std::endl;
    auto* coeff = new Rational[lhs.degree_];
    for (size_t i = 0; i < lhs.degree_; ++i)
        coeff[i] = lhs.coeff_[i] + rhs.coeff_[i];
    return Cyclotomic(lhs.field_, lhs.degree_, coeff);
}

inline Cyclotomic operator + (Cyclotomic&& lhs, const Cyclotomic& rhs) {
    return std::move(lhs += rhs);
}

inline Cyclotomic operator + (const Cyclotomic& lhs, Cyclotomic&& rhs) {
    return std::move(rhs += lhs);
}

inline Cyclotomic operator + (Cyclotomic&& lhs, Cyclotomic&& rhs) {
    return std::move(lhs += rhs);
}

inline Cyclotomic operator - (Cyclotomic arg) {
    arg.negate();
    return arg;
}

inline Cyclotomic operator - (const Cyclotomic& lhs, const Cyclotomic& rhs) {
    // std::cerr << "Cyclotomic: deep copy (const -)" << std::endl;
    auto* coeff = new Rational[lhs.degree_];
    for (size_t i = 0; i < lhs.degree_; ++i)
        coeff[i] = lhs.coeff_[i] - rhs.coeff_[i];
    return Cyclotomic(lhs.field_, lhs.degree_, coeff);
}

inline Cyclotomic operator - (Cyclotomic&& lhs, const Cyclotomic& rhs) {
    return std::move(lhs -= rhs);
}

inline Cyclotomic operator - (const Cyclotomic& lhs, Cyclotomic&& rhs) {
    rhs.negate();
    return std::move(rhs += lhs);
}

inline Cyclotomic operator - (Cyclotomic&& lhs, Cyclotomic&& rhs) {
    return std::move(lhs -= rhs);
}

inline Cyclotomic operator / (const Cyclotomic& lhs, const Cyclotomic& rhs) {
    return lhs * rhs.inverse();
}

} // namespace regina

#endif
