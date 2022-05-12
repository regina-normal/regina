
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#ifndef __REGINA_POLYNOMIAL_H
#ifndef __DOXYGEN
#define __REGINA_POLYNOMIAL_H
#endif

/*! \file maths/polynomial.h
 *  \brief Implements single variable polynomials over arbitrary rings.
 */

#include "regina-core.h"
#include "utilities/stringutils.h"
#include "core/output.h"
#include <iostream>

namespace regina {

/**
 * Represents a single-variable polynomial with coefficients of type \a T.
 * All exponents in the polynomial must be non-negative (so you can
 * represent <tt>2+3x</tt> but not <tt>1+1/x</tt>).
 *
 * The type \a T must represent a ring with no zero divisors.
 * In particular, it must:
 *
 * - support basic arithmetic operations;
 * - support assignments of the form <tt>x = int</tt> and
 *   tests of the form <tt>x == int</tt> and <tt>x < int</tt>;
 * - have a default constructor that assigns an explicit value of zero.
 *
 * This means that Regina's numerical types such as Integer and Rational
 * are supported, but native data types such as int and long are not
 * (since they have no zero-initialising default constructor).
 *
 * The underlying storage method for this class is dense (i.e., all
 * coefficients are explicitly stored, including zero coefficients).
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \ifacespython In Python, the class Polynomial refers to the specific
 * template class Polynomial<Rational>.
 *
 * \ingroup maths
 */
template <typename T>
class Polynomial : public ShortOutput<Polynomial<T>, true> {
    public:
        using Coefficient = T;
            /**< The type of each coefficient of the polynomial. */

    private:
        size_t degree_;
            /**< The degree of the polynomial.  Here the zero polynomial
                 is considered to have degree zero. */
        T* coeff_;
            /**< An array of length (\a degree_ + 1), storing the
                 coefficients of the polynomial.  Specifically,
                 coeff_[i] stores the coefficient of <tt>x^i</tt>. */

    public:
        /**
         * Creates the zero polynomial.
         */
        Polynomial();

        /**
         * Creates the polynomial <tt>x^d</tt> for the given degree \a d.
         *
         * @param degree the degree of the new polynomial.
         */
        explicit Polynomial(size_t degree);

        /**
         * Creates a new copy of the given polynomial.
         *
         * This constructor induces a deep copy of \a value.
         *
         * A note for developers: even though this routine is identical to
         * the templated copy constructor, it must be declared and
         * implemented separately.  Otherwise the compiler might create
         * its own (incorrect) copy constructor automatically.
         *
         * @param value the polynomial to clone.
         */
        Polynomial(const Polynomial& value);

        /**
         * Creates a new copy of the given polynomial.
         *
         * This constructor induces a deep copy of \a value.
         *
         * \pre Objects of type \a T can be assigned values of type \a U.
         *
         * @param value the polynomial to clone.
         */
        template <typename U>
        Polynomial(const Polynomial<U>& value);

        /**
         * Moves the contents of the given polynomial to this new polynomial.
         * This is a fast (constant time) operation.
         *
         * The polynomial that was passed (\a value) will no longer be usable.
         *
         * @param value the polynomial to move.
         */
        Polynomial(Polynomial&& value) noexcept;

        /**
         * Creates a new polynomial from the given sequence of coefficients.
         * The coefficients should be given in order from the constant
         * coefficient to the leading coefficient.
         *
         * There is no problem if the leading coefficient (i.e., the
         * last coefficient in the sequence) is zero.
         * An empty sequence will be treated as the zero polynomial.
         *
         * This constructor induces a deep copy of the given range.
         *
         * \pre Objects of type \a T can be assigned values from
         * dereferenced iterators of type \a iterator.
         *
         * \ifacespython Instead of a pair of iterators, this routine
         * takes a python list of coefficients.
         *
         * @param begin the beginning of the sequence of coefficients.
         * @param end a past-the-end iterator indicating the end of the
         * sequence of coefficients.
         */
        template <typename iterator>
        Polynomial(iterator begin, iterator end);

        /**
         * Creates a new polynomial from a hard-coded sequence of coefficients.
         * The coefficients should be given in order from the constant
         * coefficient to the leading coefficient.
         *
         * There is no problem if the leading coefficient (i.e., the
         * last coefficient in the sequence) is zero.
         * An empty sequence will be treated as the zero polynomial.
         *
         * \ifacespython Not available, but there is a Python constructor
         * that takes a list of coefficients (which need not be constant).
         *
         * @param coefficients the full sequence of coefficients.
         */
        Polynomial(std::initializer_list<T> coefficients);

        /**
         * Destroys this polynomial.
         */
        ~Polynomial();

        /**
         * Sets this to become the zero polynomial.
         */
        void init();

        /**
         * Sets this to become the polynomial <tt>x^d</tt> for the
         * given degree \a d.
         *
         * @param degree the new degree of this polynomial.
         */
        void init(size_t degree);

        /**
         * Sets this to become the polynomial described by the given
         * sequence of coefficients.
         * The coefficients should appear in order from the constant
         * coefficient to the leading coefficient.
         *
         * There is no problem if the leading coefficient (i.e., the
         * last coefficient in the sequence) is zero.
         * An empty sequence will be treated as the zero polynomial.
         *
         * This routine induces a deep copy of the given range.
         *
         * \pre Objects of type \a T can be assigned values from
         * dereferenced iterators of type \a iterator.
         *
         * \ifacespython Instead of a pair of iterators, this routine
         * takes a python list of coefficients.
         *
         * @param begin the beginning of the sequence of coefficients.
         * @param end a past-the-end iterator indicating the end of the
         * sequence of coefficients.
         */
        template <typename iterator>
        void init(iterator begin, iterator end);

        /**
         * Returns the degree of this polynomial.
         * This is the largest exponent with a non-zero coefficient.
         *
         * For the purposes of this class, the zero polynomial is
         * considered to have degree zero.
         *
         * @return the degree of this polynomial.
         */
        size_t degree() const;

        /**
         * Returns whether this is the zero polynomial.
         *
         * @return \c true if and only if this is the zero polynomial.
         */
        bool isZero() const;

        /**
         * Returns whether this polynomial is monic.  A \e monic
         * polynomial is a non-zero polynomial whose leading coefficient
         * is one.
         *
         * @return \c true if and only if this is monic.
         */
        bool isMonic() const;

        /**
         * Returns the leading coefficient of this polynomial.
         * If this is the zero polynomial, then the leading coefficient
         * will be zero.
         *
         * \ifacespython This routine returns by value, not by reference,
         * since constness is important here and Python cannot enforce
         * it otherwise.
         *
         * @return the leading coefficient of this polynomial.
         */
        const T& leading() const;

        /**
         * Returns the given coefficient of this polynomial.
         *
         * \ifacespython Python users can also use this operator to \e set
         * cofficients; that is, you can write <tt>poly[exp] = value</tt>.
         * However, when \e getting a coefficient this operator will return
         * by value (to enforce constness), which means for example you
         * cannot write something like <tt>poly[exp].negate()</tt>.
         *
         * \ifacescpp C++ users must always set coefficients using the
         * separate routine set(), since this square bracket operator is const.
         *
         * @param exp the exponent of the term whose coefficient should
         * be returned.  This must be between 0 and degree() inclusive.
         * @return the coefficient of the given term.
         */
        const T& operator [] (size_t exp) const;

        /**
         * Changes the given coefficient of this polynomial.
         *
         * It is fine to set the leading coefficient to zero, though note
         * that degree() will now return a smaller value as a result.
         *
         * It is also fine to set a coefficient whose exponent is larger
         * than the current degree; this time degree() will now return a
         * larger value (unless the given coefficient is zero).  Such an
         * operation is expensive, however, since it will require
         * deallocating and reallocating the full list of coefficients.
         *
         * \ifacespython This set() routine is available, but you can
         * also set coefficients directly using syntax of the form
         * <tt>p[exp] = value</tt>.
         *
         * @param exp the exponent of the term whose coefficient should
         * be changed.
         * @param value the new value of this coefficient.
         */
        void set(size_t exp, const T& value);

        /**
         * Tests whether this and the given polynomial are equal.
         *
         * @param rhs the polynomial to compare with this.
         * @return \c true if and only if this and the given polynomial
         * are equal.
         */
        bool operator == (const Polynomial& rhs) const;

        /**
         * Tests whether this and the given polynomial are not equal.
         *
         * @param rhs the polynomial to compare with this.
         * @return \c true if and only if this and the given polynomial
         * are not equal.
         */
        bool operator != (const Polynomial& rhs) const;

        /**
         * Sets this to be a copy of the given polynomial.
         *
         * This and the given polynomial need not have the same degree
         * (but if they do not, then the degree of this polynomial will
         * of course change).
         *
         * This operator induces a deep copy of the given polynomial.
         *
         * A note to developers: although this is identical to the templated
         * assignment operator, it must be declared and implemented separately.
         * See the copy constructor for further details.
         *
         * @param value the polynomial to copy.
         * @return a reference to this polynomial.
         */
        Polynomial& operator = (const Polynomial& value);

        /**
         * Sets this to be a copy of the given polynomial.
         *
         * This and the given polynomial need not have the same degree
         * (but if they do not, then the degree of this polynomial will
         * of course change).
         *
         * This operator induces a deep copy of the given polynomial.
         *
         * @param value the polynomial to copy.
         * @return a reference to this polynomial.
         */
        template <typename U>
        Polynomial& operator = (const Polynomial<U>& value);

        /**
         * Moves the contents of the given polynomial to this polynomial.
         * This is a fast (constant time) operation.
         *
         * This and the given polynomial need not have the same degree
         * (but if they do not, then the degree of this polynomial will
         * of course change).
         *
         * The polynomial that was passed (\a value) will no longer be usable.
         *
         * @param value the polynomial to move.
         * @return a reference to this polynomial.
         */
        Polynomial& operator = (Polynomial&& value) noexcept;

        /**
         * Swaps the contents of this and the given polynomial.
         * This is a fast (constant time) operation.
         *
         * This and the given polynomial do not need to have the same degree.
         *
         * @param other the polynomial whose contents should be swapped
         * with this.
         */
        void swap(Polynomial& other) noexcept;

        /**
         * Negates this polynomial.
         * This field element is changed directly.
         */
        void negate();

        /**
         * Multiplies this polynomial by the given constant.
         *
         * @param scalar the scalar factor to multiply by.
         * @return a reference to this polynomial.
         */
        Polynomial& operator *= (const T& scalar);

        /**
         * Divides this polynomial by the given constant.
         *
         * This uses the division operator /= for the coefficient type \a T.
         *
         * \pre The argument \a scalar is non-zero.
         *
         * @param scalar the scalar factor to divide by.
         * @return a reference to this polynomial.
         */
        Polynomial& operator /= (const T& scalar);

        /**
         * Adds the given polynomial to this.
         *
         * The given polynomial need not have the same degree as this.
         * Note that the degree of this polynomial might change as a
         * result of this operation.
         *
         * \warning This routine may trigger a deep copy (currently this
         * happens when \a other has higher degree than this).  Consider using
         * the binary <tt>+</tt> operator instead, which is better able to
         * avoid this deep copy where possible.
         *
         * @param other the polynomial to add to this.
         * @return a reference to this polynomial.
         */
        Polynomial& operator += (const Polynomial& other);

        /**
         * Subtracts the given polynomial from this.
         *
         * The given polynomial need not have the same degree as this.
         * Note that the degree of this polynomial might change as a
         * result of this operation.
         *
         * @param other the polynomial to subtract from this.
         * @return a reference to this polynomial.
         */
        Polynomial& operator -= (const Polynomial& other);

        /**
         * Multiplies this by the given polynomial.
         *
         * @param other the polynomial to multiply this by.
         * @return a reference to this polynomial.
         */
        Polynomial& operator *= (const Polynomial& other);

        /**
         * Divides this by the given polynomial.
         *
         * More precisely: suppose there exist polynomials \a q and \a r with
         * coefficients of type \a T for which <tt>this = q.other + r</tt>,
         * and where \a r has smaller degree than \a other.  Then we call
         * \a q the \e quotient, and \a r the \e remainder.
         *
         * This routine replaces this polynomial with the quotient \a q,
         * and discards the remainder.  If you need to keep the remainder
         * also, then call divisionAlg() instead.
         *
         * Coefficients are divided using the operator /= on type \a T.
         *
         * If your coefficient type \a T is not a field (e.g., if \a T
         * is Integer), you must be sure to know in advance that the
         * quotient exists (see the precondition below).  Otherwise the
         * behaviour of this routine is undefined.
         *
         * \pre The given polynomial is not the zero polynomial.
         *
         * \pre The quotient as defined above exists.  If \a T is a field
         * type (e.g., if \a T is Rational) then this is true automatically.
         * If not (e.g., if \a T is Integer) then this requires some
         * prior knowledge about the arguments.
         *
         * @param other the polynomial to divide this by.
         * @return a reference to this polynomial.
         */
        Polynomial& operator /= (const Polynomial& other);

        /**
         * Divides this by the given divisor, and returns both the
         * quotient and the remainder.
         *
         * More precisely: suppose there exist polynomials \a q and \a r with
         * coefficients of type \a T for which <tt>this = q.divisor + r</tt>,
         * and where \a r has smaller degree than \a divisor.  Then this
         * routine returns the pair (\a q, \a r); that is, the \e quotient
         * and the \e remainder.
         *
         * If you do not need the remainder (e.g., if you know in
         * advance that \a divisor divides into this polynomial exactly),
         * then you can use the division operator /= instead, which will
         * be a little faster.
         *
         * If your coefficient type \a T is not a field (e.g., if \a T
         * is Integer), you must be sure to know in advance that the
         * quotient exists (see the precondition below).  Otherwise the
         * behaviour of this routine is undefined.
         *
         * Coefficients are divided using the operator /= on type \a T.
         *
         * \pre The given divisor is not the zero polynomial.
         *
         * \pre The quotient as defined above exists.  If \a T is a field
         * type (e.g., if \a T is Rational) then this is true automatically.
         * If not (e.g., if \a T is Integer) then this requires some
         * prior knowledge about the arguments.
         *
         * @param divisor the polynomial to divide this by.
         * @return a pair holding the quotient and remainder, as described
         * above.
         */
        std::pair<Polynomial, Polynomial> divisionAlg(
            const Polynomial& divisor) const;

        /**
         * Calculates the greatest common divisor of this and the given
         * polynomial, and finds a linear combination of these
         * polynomials that gives this gcd.
         *
         * The greatest common divisor will be a monic polynomial.
         * The polynomials returned in \a u and \a v will satisfy
         * <tt>u*this + v*other = gcd</tt>.
         *
         * As a special case, gcd(0,0) is considered to be zero.
         *
         * \pre The coefficient type \a T represents a field.  In particular,
         * Rational is supported but Integer is not.
         *
         * @param other the polynomial whose greatest common divisor with this
         * polynomial we should compute.
         * @param gcd a polynomial whose contents will be destroyed and
         * replaced with the greatest common divisor \a d, as described above.
         * @param u a polynomial whose contents will be destroyed and
         * replaced with \a u, as described above.
         * @param v a polynomial whose contents will be destroyed and
         * replaced with \a v, as described above.
         */
        template <typename U>
        void gcdWithCoeffs(const Polynomial<U>& other,
            Polynomial& gcd, Polynomial& u, Polynomial& v) const;

        /**
         * Writes this polynomial to the given output stream, using the
         * given variable name instead of \c x.
         *
         * If \a utf8 is passed as \c true then unicode superscript characters
         * will be used for exponents; these will be encoded using UTF-8.
         * This will make the output nicer, but will require more complex
         * fonts to be available on the user's machine.
         *
         * \ifacespython Not present; use str() or utf8() instead.
         *
         * @param out the output stream to which to write.
         * @param utf8 \c true if unicode superscript characters may be used.
         * @param variable the symbol to use for the variable in this
         * polynomial.  This may be \c null, in which case the default
         * variable \c x will be used.
         */
        void writeTextShort(std::ostream& out, bool utf8 = false,
            const char* variable = nullptr) const;

        /**
         * Returns this polynomial as a human-readable string, using the
         * given variable name instead of \c x.
         *
         * \note There is also the usual variant of str() which takes no
         * arguments; that variant is inherited from the Output class.
         *
         * @param variable the symbol to use for the variable in this
         * polynomial.  This may be \c null, in which case the default
         * variable \c x will be used.
         * @return this polynomial as a human-readable string.
         */
        std::string str(const char* variable) const;

        /**
         * Returns this polynomial as a human-readable string using
         * unicode characters, using the given variable name instead of \c x.
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
         * @param variable the symbol to use for the variable in this
         * polynomial.  This may be \c null, in which case the default
         * variable \c x will be used.
         * @return this polynomial as a unicode-enabled human-readable string.
         */
        std::string utf8(const char* variable) const;

    private:
        /**
         * Constructs a new polynomial with the given degree and coefficients.
         *
         * The data members \a degree_ and \a coeff_ will be set to the given
         * values; in particular, the new object will take ownership of the
         * coefficient array.
         */
        Polynomial(size_t degree, T* coeff);

        /**
         * Decreases \a degree_ to ensure that the leading coefficient
         * is non-zero.  If this is the zero polynomial then \a degree_
         * will be set to zero.
         */
        void fixDegree();

        /**
         * Replaces the contents of this polynomial with \a other - \a this.
         *
         * This is equivalent to calling the -= operator and then negating.
         *
         * The given polynomial need not have the same degree as this.
         * Note that the degree of this polynomial might change as a
         * result of this operation.
         *
         * @param other the polynomial to subtract this from.
         * @return a reference to this polynomial.
         */
        Polynomial& subtractFrom(const Polynomial& other);

    template <typename U>
    friend Polynomial<U> operator +(const Polynomial<U>&, const Polynomial<U>&);

    template <typename U>
    friend Polynomial<U> operator -(const Polynomial<U>&, const Polynomial<U>&);

    template <typename U>
    friend Polynomial<U> operator -(const Polynomial<U>&, Polynomial<U>&&);

    template <typename U>
    friend Polynomial<U> operator -(Polynomial<U>&&, Polynomial<U>&&);

    template <typename U>
    friend Polynomial<U> operator *(const Polynomial<U>&, const Polynomial<U>&);
};

/**
 * Swaps the contents of the given polynomials.
 *
 * This global routine simply calls Polynomial<T>::swap(); it is provided
 * so that Polynomial<T> meets the C++ Swappable requirements.
 *
 * @param a the first polynomial whose contents should be swapped.
 * @param b the second polynomial whose contents should be swapped.
 *
 * \ingroup maths
 */
template <typename T>
void swap(Polynomial<T>& a, Polynomial<T>& b) noexcept;

/**
 * Multiplies the given polynomial by the given scalar constant.
 *
 * The scalar is simply of type \a T; we use the identical type
 * Polynomial<T>::Coefficient here to assist with C++ template type matching.
 *
 * @param poly the polynomial to multiply by.
 * @param scalar the scalar to multiply by.
 * @return the product of the given polynomial and scalar.
 *
 * \ingroup maths
 */
template <typename T>
Polynomial<T> operator * (Polynomial<T> poly,
    const typename Polynomial<T>::Coefficient& scalar);

/**
 * Multiplies the given polynomial by the given scalar constant.
 *
 * The scalar is simply of type \a T; we use the identical type
 * Polynomial<T>::Coefficient here to assist with C++ template type matching.
 *
 * @param scalar the scalar to multiply by.
 * @param poly the polynomial to multiply by.
 * @return the product of the given polynomial and scalar.
 *
 * \ingroup maths
 */
template <typename T>
Polynomial<T> operator * (const typename Polynomial<T>::Coefficient& scalar,
    Polynomial<T> poly);

/**
 * Divides the given polynomial by the given scalar constant.
 *
 * This uses the division operator /= for the coefficient type \a T.
 *
 * The scalar is simply of type \a T; we use the identical type
 * Polynomial<T>::Coefficient here to assist with C++ template type matching.
 *
 * \pre The argument \a scalar is non-zero.
 *
 * @param poly the polynomial to divide by the given scalar.
 * @param scalar the scalar factor to divide by.
 * @return the quotient of the given polynomial by the given scalar.
 *
 * \ingroup maths
 */
template <typename T>
Polynomial<T> operator / (Polynomial<T> poly,
    const typename Polynomial<T>::Coefficient& scalar);

/**
 * Adds the two given polynomials.
 *
 * This operator <tt>+</tt> is sometimes faster than using <tt>+=</tt>,
 * since it has more flexibility to avoid an internal deep copy.
 *
 * @param lhs the first polynomial to add.
 * @param rhs the second polynomial to add.
 * @return the sum of both polynomials.
 *
 * \ingroup maths
 */
template <typename T>
Polynomial<T> operator + (const Polynomial<T>& lhs, const Polynomial<T>& rhs);

/**
 * Adds the two given polynomials.
 *
 * This operator <tt>+</tt> is sometimes faster than using <tt>+=</tt>,
 * since it has more flexibility to avoid an internal deep copy.
 *
 * @param lhs the first polynomial to add.
 * @param rhs the second polynomial to add.
 * @return the sum of both polynomials.
 *
 * \ingroup maths
 */
template <typename T>
Polynomial<T> operator + (Polynomial<T>&& lhs, const Polynomial<T>& rhs);

/**
 * Adds the two given polynomials.
 *
 * This operator <tt>+</tt> is sometimes faster than using <tt>+=</tt>,
 * since it has more flexibility to avoid an internal deep copy.
 *
 * @param lhs the first polynomial to add.
 * @param rhs the second polynomial to add.
 * @return the sum of both polynomials.
 *
 * \ingroup maths
 */
template <typename T>
Polynomial<T> operator + (const Polynomial<T>& lhs, Polynomial<T>&& rhs);

/**
 * Adds the two given polynomials.
 *
 * This operator <tt>+</tt> is sometimes faster than using <tt>+=</tt>,
 * since it has more flexibility to avoid an internal deep copy.
 *
 * @param lhs the first polynomial to add.
 * @param rhs the second polynomial to add.
 * @return the sum of both polynomials.
 *
 * \ingroup maths
 */
template <typename T>
Polynomial<T> operator + (Polynomial<T>&& lhs, Polynomial<T>&& rhs);

/**
 * Returns the negative of the given polynomial.
 *
 * @param arg the polynomial to negate.
 * @return the negative of \a arg.
 *
 * \ingroup maths
 */
template <typename T>
Polynomial<T> operator - (Polynomial<T> arg);

/**
 * Subtracts the two given polynomials.
 *
 * This operator <tt>-</tt> is sometimes faster than using <tt>-=</tt>,
 * since it has more flexibility to avoid an internal deep copy.
 *
 * @param lhs the polynomial to sutract \a rhs from.
 * @param rhs the polynomial to subtract from \a lhs.
 * @return the difference of the two given polynomials.
 *
 * \ingroup maths
 */
template <typename T>
Polynomial<T> operator - (const Polynomial<T>& lhs, const Polynomial<T>& rhs);

/**
 * Subtracts the two given polynomials.
 *
 * This operator <tt>-</tt> is sometimes faster than using <tt>-=</tt>,
 * since it has more flexibility to avoid an internal deep copy.
 *
 * @param lhs the polynomial to sutract \a rhs from.
 * @param rhs the polynomial to subtract from \a lhs.
 * @return the difference of the two given polynomials.
 *
 * \ingroup maths
 */
template <typename T>
Polynomial<T> operator - (Polynomial<T>&& lhs, const Polynomial<T>& rhs);

/**
 * Subtracts the two given polynomials.
 *
 * This operator <tt>-</tt> is sometimes faster than using <tt>-=</tt>,
 * since it has more flexibility to avoid an internal deep copy.
 *
 * @param lhs the polynomial to sutract \a rhs from.
 * @param rhs the polynomial to subtract from \a lhs.
 * @return the difference of the two given polynomials.
 *
 * \ingroup maths
 */
template <typename T>
Polynomial<T> operator - (const Polynomial<T>& lhs, Polynomial<T>&& rhs);

/**
 * Subtracts the two given polynomials.
 *
 * This operator <tt>-</tt> is sometimes faster than using <tt>-=</tt>,
 * since it has more flexibility to avoid an internal deep copy.
 *
 * @param lhs the polynomial to sutract \a rhs from.
 * @param rhs the polynomial to subtract from \a lhs.
 * @return the difference of the two given polynomials.
 *
 * \ingroup maths
 */
template <typename T>
Polynomial<T> operator - (Polynomial<T>&& lhs, Polynomial<T>&& rhs);

/**
 * Multiplies the two given polynomials.
 *
 * @param lhs the first polynomial to multiply.
 * @param rhs the second polynomial to multiply.
 * @return the product of both polynomials.
 *
 * \ingroup maths
 */
template <typename T>
Polynomial<T> operator * (const Polynomial<T>& lhs, const Polynomial<T>& rhs);

/**
 * Divides the two given polynomials.
 *
 * More precisely: suppose there exist polynomials \a q and \a r with
 * coefficients of type \a T for which <tt>lhs = q.rhs + r</tt>,
 * and where \a r has smaller degree than \a rhs.  Then we call
 * \a q the \e quotient, and \a r the \e remainder.
 *
 * This routine returns the quotient \a q, and discards the remainder.
 * If you need to keep the remainder also, then call Polynomial::divisionAlg()
 * instead.
 *
 * Coefficients are divided using the operator /= on type \a T.
 *
 * If your coefficient type \a T is not a field (e.g., if \a T
 * is Integer), you must be sure to know in advance that the
 * quotient exists (see the precondition below).  Otherwise the
 * behaviour of this routine is undefined.
 *
 * \pre The second polynomial \a rhs is non-zero.
 *
 * \pre The quotient as defined above exists.  If \a T is a field
 * type (e.g., if \a T is Rational) then this is true automatically.
 * If not (e.g., if \a T is Integer) then this requires some
 * prior knowledge about the arguments.
 *
 * @param lhs the polynomial to divide by \a rhs.
 * @param rhs the polynomial that we will divide \a lhs by.
 * @return the quotient, as described above.
 *
 * \ingroup maths
 */
template <typename T>
Polynomial<T> operator / (Polynomial<T> lhs, const Polynomial<T>& rhs);

template <typename T>
inline Polynomial<T>::Polynomial() : degree_(0), coeff_(new T[1]) {
    // The default constructor for T already initialises coeff_[0] to zero.
}

template <typename T>
inline Polynomial<T>::Polynomial(size_t degree) :
        degree_(degree), coeff_(new T[degree + 1]) {
    coeff_[degree] = 1;
}

template <typename T>
template <typename iterator>
inline Polynomial<T>::Polynomial(iterator begin, iterator end) :
        coeff_(nullptr) {
    init(begin, end);
}

template <typename T>
inline Polynomial<T>::Polynomial(std::initializer_list<T> coefficients) :
        coeff_(nullptr) {
    init(coefficients.begin(), coefficients.end());
}

template <typename T>
inline Polynomial<T>::Polynomial(const Polynomial<T>& value) :
        degree_(value.degree()), coeff_(new T[value.degree() + 1]) {
    // std::cerr << "Polynomial: deep copy (init)" << std::endl;
    for (size_t i = 0; i <= degree_; ++i)
        coeff_[i] = value[i];
}

template <typename T>
template <typename U>
inline Polynomial<T>::Polynomial(const Polynomial<U>& value) :
        degree_(value.degree()), coeff_(new T[value.degree() + 1]) {
    // std::cerr << "Polynomial: deep copy (init)" << std::endl;
    for (size_t i = 0; i <= degree_; ++i)
        coeff_[i] = value[i];
}

template <typename T>
inline Polynomial<T>::Polynomial(Polynomial<T>&& value) noexcept :
        degree_(value.degree_), coeff_(value.coeff_) {
    value.coeff_ = nullptr;
}

template <typename T>
inline Polynomial<T>::~Polynomial() {
    delete[] coeff_;
}

template <typename T>
inline void Polynomial<T>::init() {
    delete[] coeff_;
    degree_ = 0;
    coeff_ = new T[1];
    // coeff_[0] is initialised to 0 automatically.
}

template <typename T>
inline void Polynomial<T>::init(size_t degree) {
    delete[] coeff_;
    degree_ = degree;
    coeff_ = new T[degree + 1];
    coeff_[degree] = 1;
}

template <typename T>
template <typename iterator>
void Polynomial<T>::init(iterator begin, iterator end) {
    delete[] coeff_;

    if (begin == end) {
        degree_ = 0;
        coeff_ = new T[1];
        return;
    }

    degree_ = end - begin - 1;
    coeff_ = new T[degree_ + 1];

    size_t i = 0;
    while (begin != end)
        coeff_[i++] = *begin++;

    // The leading coefficient might be zero.
    fixDegree();
}

template <typename T>
inline size_t Polynomial<T>::degree() const {
    return degree_;
}

template <typename T>
inline bool Polynomial<T>::isZero() const {
    return (degree_ == 0 && coeff_[0] == 0);
}

template <typename T>
inline bool Polynomial<T>::isMonic() const {
    return (coeff_[degree_] == 1);
}

template <typename T>
inline const T& Polynomial<T>::leading() const {
    return coeff_[degree_];
}

template <typename T>
inline const T& Polynomial<T>::operator [] (size_t exp) const {
    return coeff_[exp];
}

template <typename T>
void Polynomial<T>::set(size_t exp, const T& value) {
    if (exp < degree_) {
        coeff_[exp] = value;
    } else if (exp == degree_) {
        coeff_[exp] = value;
        if (value == 0) {
            if (degree_ > 0) {
                --degree_;
                fixDegree();
            }
        }
    } else if (! (value == 0)) {
        // The degree will increase.
        T* newCoeff = new T[exp + 1];
        size_t i;
        for (i = 0; i <= degree_; ++i)
            newCoeff[i] = coeff_[i];
        newCoeff[exp] = value;

        delete[] coeff_;
        coeff_ = newCoeff;
        degree_ = exp;
    }
}

template <typename T>
inline bool Polynomial<T>::operator == (const Polynomial<T>& rhs) const {
    if (degree_ != rhs.degree_)
        return false;
    for (size_t i = 0; i <= degree_; ++i)
        if (coeff_[i] != rhs.coeff_[i])
            return false;
    return true;
}

template <typename T>
inline bool Polynomial<T>::operator != (const Polynomial<T>& rhs) const {
    if (degree_ != rhs.degree_)
        return true;
    for (size_t i = 0; i <= degree_; ++i)
        if (coeff_[i] != rhs.coeff_[i])
            return true;
    return false;
}

template <typename T>
// NOLINTNEXTLINE(bugprone-unhandled-self-assignment)
Polynomial<T>& Polynomial<T>::operator = (const Polynomial<T>& value) {
    // This works even if &value == this, assuming T itself can handle
    // self-assignment, since we don't reallocate if the degrees are equal.
    if (degree_ < value.degree_) {
        delete[] coeff_;
        coeff_ = new T[value.degree_ + 1];
    }
    // std::cerr << "Polynomial: deep copy (=)" << std::endl;
    degree_ = value.degree_;
    for (size_t i = 0; i <= degree_; ++i)
        coeff_[i] = value.coeff_[i];
    return *this;
}

template <typename T>
template <typename U>
Polynomial<T>& Polynomial<T>::operator = (const Polynomial<U>& value) {
    // This works even if &value == this, since we don't reallocate if
    // the degrees are equal.
    if (degree_ < value.degree()) {
        delete[] coeff_;
        coeff_ = new T[value.degree() + 1];
    }
    // std::cerr << "Polynomial: deep copy (=)" << std::endl;
    degree_ = value.degree();
    for (size_t i = 0; i <= degree_; ++i)
        coeff_[i] = value[i];
    return *this;
}

template <typename T>
inline Polynomial<T>& Polynomial<T>::operator = (Polynomial<T>&& value)
        noexcept {
    degree_ = value.degree_;
    std::swap(coeff_, value.coeff_);
    // Let value dispose of the original coefficients in its own destructor.
    return *this;
}

template <typename T>
inline void Polynomial<T>::swap(Polynomial<T>& other) noexcept {
    std::swap(degree_, other.degree_);
    std::swap(coeff_, other.coeff_);
}

template <typename T>
inline void Polynomial<T>::negate() {
    for (size_t i = 0; i <= degree_; ++i)
        if (coeff_[i] != 0)
            coeff_[i] = - coeff_[i];
}

template <typename T>
Polynomial<T>& Polynomial<T>::operator *= (const T& scalar) {
    if (scalar == 0)
        init();
    else {
        for (size_t i = 0; i <= degree_; ++i)
            coeff_[i] *= scalar;
    }
    return *this;
}

template <typename T>
inline Polynomial<T>& Polynomial<T>::operator /= (const T& scalar) {
    for (size_t i = 0; i <= degree_; ++i)
        coeff_[i] /= scalar;

    // For integer division, we could have zeroed out some coefficients.
    fixDegree();

    return *this;
}

template <typename T>
Polynomial<T>& Polynomial<T>::operator += (const Polynomial<T>& other) {
    // This works even if &other == this, since we don't reallocate if
    // the degrees are equal.
    size_t i;
    if (degree_ < other.degree_) {
        // std::cerr << "Polynomial: deep copy (+=)" << std::endl;
        T* copy = new T[other.degree_ + 1];
        for (i = 0; i <= degree_; ++i)
            copy[i] = coeff_[i];
        delete[] coeff_;
        coeff_ = copy;
        degree_ = other.degree_;
    }
    for (i = 0; i <= other.degree_; ++i)
        coeff_[i] += other.coeff_[i];

    // We might have zeroed out the leading coefficient.
    fixDegree();

    return *this;
}

template <typename T>
Polynomial<T>& Polynomial<T>::operator -= (const Polynomial<T>& other) {
    // This works even if &other == this, since we don't reallocate if
    // the degrees are equal.
    size_t i;
    if (degree_ < other.degree_) {
        // std::cerr << "Polynomial: deep copy (-=)" << std::endl;
        T* copy = new T[other.degree_ + 1];
        for (i = 0; i <= degree_; ++i)
            copy[i] = coeff_[i];
        delete[] coeff_;
        coeff_ = copy;
        degree_ = other.degree_;
    }
    for (i = 0; i <= other.degree_; ++i)
        coeff_[i] -= other.coeff_[i];

    // We might have zeroed out the leading coefficient.
    fixDegree();

    return *this;
}

template <typename T>
Polynomial<T>& Polynomial<T>::operator *= (const Polynomial<T>& other) {
    if (isZero())
        return *this;
    if (other.isZero()) {
        init();
        return *this;
    }

    // The following code works even if &other == this, since we construct the
    // coefficients of the product in a separate section of memory.
    // std::cerr << "Polynomial: deep copy (*=)" << std::endl;
    size_t i, j;
    T* ans = new T[degree_ + other.degree_ + 1];
    for (i = 0; i <= degree_; ++i)
        for (j = 0; j <= other.degree_; ++j)
            ans[i + j] += (coeff_[i] * other.coeff_[j]);

    delete[] coeff_;
    coeff_ = ans;
    degree_ += other.degree_;

    // Both leading coefficients are non-zero, so the degree is correct.
    return *this;
}

template <typename T>
Polynomial<T>& Polynomial<T>::operator /= (const Polynomial<T>& other) {
    // The code below breaks if other and *this are the same object, so
    // treat this case specially.
    if (&other == this) {
        init(0);
        return *this;
    }

    size_t i, j;
    if (other.degree_ == 0) {
        for (i = 0; i <= degree_; ++i)
            coeff_[i] /= other.coeff_[0];
        return *this;
    }

    // The divisor has positive degree.
    if (degree_ < other.degree_) {
        // The quotient is zero.
        init();
        return *this;
    }

    // We now have 0 < deg(other) <= deg(this).
    // std::cerr << "Polynomial: deep copy (/=)" << std::endl;
    T* remainder = coeff_;
    coeff_ = new T[degree_ - other.degree_ + 1];
    for (i = degree_; i >= other.degree_; --i) {
        coeff_[i - other.degree_] =
            remainder[i] /= other.coeff_[other.degree_];
        for (j = 0; j <= other.degree_; ++j)
            remainder[j + i - other.degree_] -=
                (coeff_[i - other.degree_] * other.coeff_[j]);
    }
    delete[] remainder;
    degree_ -= other.degree_;

    return *this;
}

template <typename T>
std::pair<Polynomial<T>, Polynomial<T>> Polynomial<T>::divisionAlg(
        const Polynomial<T>& divisor) const {
    // The code below breaks if divisor and *this are the same object, so
    // treat this case specially.
    if (&divisor == this)
        return { {1}, {} }; // q = 1, r = 0

    if (divisor.degree_ > degree_)
        return { {}, *this }; // q = 0, r = this

    if (divisor.degree_ == 0) {
        // q = this / divisor[0], r = 0
        std::pair<Polynomial<T>, Polynomial<T>> ans(*this, {});
        for (size_t i = 0; i <= ans.first.degree_; ++i)
            ans.first.coeff_[i] /= divisor.coeff_[0];
        return ans;
    }

    // From here we have: 0 < deg(divisor) <= deg(this).
    // In particular, both this and divisor have strictly positive degree.

    // We initialise the quotient to be x^k where k is the correct degree;
    // this is just so the constructor corretly allocated the right number of
    // coefficients.  We will overwrite the unwanted leading coefficient later.
    //
    // We initialise the remainer to be a copy of this.

    std::pair<Polynomial<T>, Polynomial<T>> ans(
        degree_ - divisor.degree_, *this);

    for (size_t i = degree_; i >= divisor.degree_; --i) {
        ans.first.coeff_[i - divisor.degree_] = ans.second.coeff_[i];
        ans.first.coeff_[i - divisor.degree_] /=
            divisor.coeff_[divisor.degree_];
        for (size_t j = 0; j <= divisor.degree_; ++j)
            ans.second.coeff_[j + i - divisor.degree_] -=
                (ans.first.coeff_[i - divisor.degree_] * divisor.coeff_[j]);
    }

    // Although the degree of the quotient is correct, the remainder
    // might have zero coefficients at any (or all) positions.
    ans.second.degree_ = divisor.degree_ - 1;
    while (ans.second.degree_ > 0 && ans.second.coeff_[ans.second.degree_] == 0)
        --ans.second.degree_;

    return ans;
}

template <typename T>
template <typename U>
void Polynomial<T>::gcdWithCoeffs(const Polynomial<U>& other,
        Polynomial<T>& gcd, Polynomial<T>& u, Polynomial<T>& v) const {
    // Special-case situations where one or both polynomials are zero.
    if (other.isZero()) {
        if (isZero()) {
            // gcd(0, 0) = 0.
            gcd.init();
            u.init();
            v.init();
            return;
        }

        // gcd(this, 0) = this / this.leading()
        gcd = *this;
        gcd /= coeff_[degree_];
        u.init(0);
        u.coeff_[0] /= coeff_[degree_];
        v.init();
        return;
    }
    if (isZero()) {
        // gcd(0, other) = other / other.leading()
        gcd = other;
        gcd /= other[other.degree()];
        u.init();
        v.init(0);
        v.coeff_[0] /= other[other.degree()];
        return;
    }

    // We use Euclid's algorithm to find gcd(this, other).
    //
    // At each stage we maintain the invariants:
    //
    //   u * this + v * other = x
    //   uu * this + vv * other = y
    //   deg(x) >= deg(y)
    //
    // We begin with (x, y, u, v, uu, vv) = (this, other, 1, 0, 0, 1).
    // The iteration step, assuming x = q * y + r, is then:
    //
    //   (x, y, u, v, uu, vv) -> (y, r, uu, vv, u-q*uu, v-q*vv)
    //
    // We finish with (x, y) = (gcd, 0).
    //
    // In the code below we use the given polyomial gcd to store x throughout
    // the algorithm.

    gcd = *this;
    Polynomial<T> y(other);
    u.init(0);
    v.init();
    Polynomial<T> uu;
    Polynomial<T> vv(0);

    if (gcd.degree() < y.degree()) {
        gcd.swap(y);
        u.swap(uu);
        v.swap(vv);
    }

    while (! y.isZero()) {
        auto [q, r] = gcd.divisionAlg(y);

        u -= (q * uu);
        v -= (q * vv);

        u.swap(uu);
        v.swap(vv);
        gcd.swap(y);
        y.swap(r);
    }

    // Make the gcd monic.
    // Note that gcd != 0 at this point, so this is indeed possible.
    if (! gcd.isMonic()) {
        T leading(gcd[gcd.degree_]);
        gcd /= leading;
        u /= leading;
        v /= leading;
    }
}

template <typename T>
void Polynomial<T>::writeTextShort(std::ostream& out, bool utf8,
        const char* variable) const {
    if (degree_ == 0) {
        out << coeff_[0];
        return;
    }

    size_t i = degree_;
    while (true) {
        if (i == 0) {
            // This is the constant term.
            // If non-zero, it is not the first term being output.
            if (coeff_[i] < 0)
                out << " - " << (-coeff_[i]);
            else if (coeff_[i] != 0)
                out << " + " << coeff_[i];
            break;
        }
        if (coeff_[i] != 0) {
            // This is non-zero, and not the constant term.
            if (i == degree_) {
                // This is the first term being output.
                if (coeff_[i] == -1)
                    out << "- ";
                else if (coeff_[i] != 1)
                    out << coeff_[i] << ' ';
            } else {
                // This is not the first term being output.
                if (coeff_[i] == -1)
                    out << " - ";
                else if (coeff_[i] < 0)
                    out << " - " << (-coeff_[i]) << ' ';
                else if (coeff_[i] == 1)
                    out << " + ";
                else
                    out << " + " << coeff_[i] << ' ';
            }
            if (variable)
                out << variable;
            else
                out << 'x';
            if (i != 1) {
                if (utf8)
                    out << regina::superscript(i);
                else
                    out << '^' << i;
            }
        }
        --i;
    }
}

template <typename T>
inline std::string Polynomial<T>::str(const char* variable) const {
    // Make sure that python will be able to find the inherited str().
    static_assert(std::is_same<typename OutputBase<Polynomial<T>>::type,
        Output<Polynomial<T>, true>>::value,
        "Polynomial<T> is not identified as being inherited from Output<...>");

    std::ostringstream out;
    writeTextShort(out, false, variable);
    return out.str();
}

template <typename T>
inline std::string Polynomial<T>::utf8(const char* variable) const {
    std::ostringstream out;
    writeTextShort(out, true, variable);
    return out.str();
}

template <typename T>
inline Polynomial<T>::Polynomial(size_t degree, T* coeff) :
        degree_(degree), coeff_(coeff) {
}

template <typename T>
inline void Polynomial<T>::fixDegree() {
    // The leading coefficient might be zero.
    while (degree_ > 0 && coeff_[degree_] == 0)
        --degree_;
}

template <typename T>
inline Polynomial<T>& Polynomial<T>::subtractFrom(const Polynomial<T>& other) {
    // This works even if &other == this, since we don't reallocate if
    // the degrees are equal.
    size_t i;
    if (degree_ < other.degree_) {
        // std::cerr << "Polynomial: deep copy (subtractFrom)" << std::endl;
        T* copy = new T[other.degree_ + 1];
        for (i = 0; i <= degree_; ++i)
            if (coeff_[i] == 0)
                copy[i] = other.coeff_[i];
            else
                copy[i] = other.coeff_[i] - coeff_[i];
        for ( ; i <= other.degree_; ++i)
            copy[i] = other.coeff_[i];
        delete[] coeff_;
        coeff_ = copy;
        degree_ = other.degree_;
    } else {
        // No need to reallocate.
        for (i = 0; i <= other.degree_; ++i)
            if (coeff_[i] == 0)
                coeff_[i] = other.coeff_[i];
            else
                coeff_[i] = other.coeff_[i] - coeff_[i];
        for ( ; i <= degree_; ++i)
            if (coeff_[i] != 0)
                coeff_[i] = -coeff_[i];
    }

    // We might have zeroed out the leading coefficient.
    fixDegree();

    return *this;
}

template <typename T>
inline void swap(Polynomial<T>& a, Polynomial<T>& b) noexcept {
    a.swap(b);
}

template <typename T>
inline Polynomial<T> operator * (Polynomial<T> poly,
        const typename Polynomial<T>::Coefficient& scalar) {
    // When the argument poly is an lvalue reference, we perform a deep copy
    // due to pass-by-value.  If scalar == 0 then we don't need this deep copy,
    // since the argument can be ignored.  This special-case optimisation
    // would require two different lvalue/rvalue implementations of *, and
    // so we leave it for now.
    poly *= scalar;
    return poly;
}

template <typename T>
inline Polynomial<T> operator * (
        const typename Polynomial<T>::Coefficient& scalar, Polynomial<T> poly) {
    // See the notes above on a possible optimisation for scalar == 0.
    poly *= scalar;
    return poly;
}

template <typename T>
inline Polynomial<T> operator / (Polynomial<T> poly,
        const typename Polynomial<T>::Coefficient& scalar) {
    poly /= scalar;
    return poly;
}

template <typename T>
Polynomial<T> operator + (const Polynomial<T>& lhs, const Polynomial<T>& rhs) {
    // std::cerr << "Polynomial: deep copy (const +)" << std::endl;
    if (lhs.degree_ >= rhs.degree_) {
        T* coeff = new T[lhs.degree_ + 1];

        for (size_t i = 0 ; i <= rhs.degree_; ++i)
            coeff[i] = lhs.coeff_[i] + rhs.coeff_[i];
        std::copy(lhs.coeff_ + rhs.degree_ + 1, lhs.coeff_ + lhs.degree_ + 1,
            coeff + rhs.degree_ + 1);

        // We might have zeroed out the leading coefficient.
        size_t degree = lhs.degree_;
        while (degree > 0 && coeff[degree] == 0)
            --degree;
        return Polynomial<T>(degree, coeff);
    } else {
        T* coeff = new T[rhs.degree_ + 1];

        for (size_t i = 0 ; i <= lhs.degree_; ++i)
            coeff[i] = lhs.coeff_[i] + rhs.coeff_[i];
        std::copy(rhs.coeff_ + lhs.degree_ + 1, rhs.coeff_ + rhs.degree_ + 1,
            coeff + lhs.degree_ + 1);

        return Polynomial<T>(rhs.degree_, coeff);
    }
}

template <typename T>
inline Polynomial<T> operator + (Polynomial<T>&& lhs,
        const Polynomial<T>& rhs) {
    // If deg(lhs) < deg(rhs) then a deep copy is unavoidable.
    return std::move(lhs += rhs);
}

template <typename T>
inline Polynomial<T> operator + (const Polynomial<T>& lhs,
        Polynomial<T>&& rhs) {
    // If deg(rhs) < deg(lhs) then a deep copy is unavoidable.
    return std::move(rhs += lhs);
}

template <typename T>
inline Polynomial<T> operator + (Polynomial<T>&& lhs, Polynomial<T>&& rhs) {
    // Add in whichever diretion avoids the deep copy in +=.
    if (lhs.degree() >= rhs.degree())
        return std::move(lhs += rhs);
    else
        return std::move(rhs += lhs);
}

template <typename T>
inline Polynomial<T> operator - (Polynomial<T> arg) {
    arg.negate();
    return arg;
}

template <typename T>
Polynomial<T> operator - (const Polynomial<T>& lhs, const Polynomial<T>& rhs) {
    // std::cerr << "Polynomial: deep copy (const -)" << std::endl;
    if (lhs.degree_ >= rhs.degree_) {
        T* coeff = new T[lhs.degree_ + 1];

        for (size_t i = 0 ; i <= rhs.degree_; ++i)
            if (rhs.coeff_[i] == 0)
                coeff[i] = lhs.coeff_[i];
            else
                coeff[i] = lhs.coeff_[i] - rhs.coeff_[i];
        std::copy(lhs.coeff_ + rhs.degree_ + 1, lhs.coeff_ + lhs.degree_ + 1,
            coeff + rhs.degree_ + 1);

        // We might have zeroed out the leading coefficient.
        size_t degree = lhs.degree_;
        while (degree > 0 && coeff[degree] == 0)
            --degree;
        return Polynomial<T>(degree, coeff);
    } else {
        T* coeff = new T[rhs.degree_ + 1];

        size_t i;
        for (i = 0 ; i <= lhs.degree_; ++i)
            if (rhs.coeff_[i] == 0)
                coeff[i] = lhs.coeff_[i];
            else
                coeff[i] = lhs.coeff_[i] - rhs.coeff_[i];
        for ( ; i <= rhs.degree_; ++i)
            if (rhs.coeff_[i] != 0)
                coeff[i] = - rhs.coeff_[i];

        return Polynomial<T>(rhs.degree_, coeff);
    }
}

template <typename T>
Polynomial<T> operator - (Polynomial<T>&& lhs, const Polynomial<T>& rhs) {
    return std::move(lhs -= rhs);
}

template <typename T>
Polynomial<T> operator - (const Polynomial<T>& lhs, Polynomial<T>&& rhs) {
    return std::move(rhs.subtractFrom(lhs));
}

template <typename T>
Polynomial<T> operator - (Polynomial<T>&& lhs, Polynomial<T>&& rhs) {
    // Choose a direction for the subtraction that avoids a
    // deep copy within -= / subtractFrom.
    if (lhs.degree_ < rhs.degree_)
        return std::move(rhs.subtractFrom(lhs));
    else
        return std::move(lhs -= rhs);
}

template <typename T>
Polynomial<T> operator * (const Polynomial<T>& lhs, const Polynomial<T>& rhs) {
    if (lhs.isZero() || rhs.isZero())
        return Polynomial<T>();

    // std::cerr << "Polynomial: deep copy (const *)" << std::endl;
    size_t i, j;
    T* coeff = new T[lhs.degree_ + rhs.degree_ + 1];
    for (i = 0; i <= lhs.degree_; ++i)
        for (j = 0; j <= rhs.degree_; ++j)
            coeff[i + j] += (lhs.coeff_[i] * rhs.coeff_[j]);

    // Both leading coefficients are non-zero, so the degree is correct.
    return Polynomial<T>(lhs.degree_ + rhs.degree_, coeff);
}

template <typename T>
inline Polynomial<T> operator / (Polynomial<T> lhs, const Polynomial<T>& rhs) {
    return std::move(lhs /= rhs);
}

} // namespace regina

#endif
