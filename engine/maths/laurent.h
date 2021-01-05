
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

#ifndef __LAURENT_H
#ifndef __DOXYGEN
#define __LAURENT_H
#endif

/*! \file maths/laurent.h
 *  \brief Implements single variable Laurent polynomials over arbitrary rings.
 */

#include "utilities/stringutils.h"
#include "core/output.h"
#include <iostream>
#include <sstream>

namespace regina {

/**
 * \weakgroup maths
 * @{
 */

/**
 * Represents a single-variable Laurent polynomial with coefficients of
 * type \a T.  A Laurent polynomial differs from an ordinary polynomial
 * in that it allows negative exponents (so, unlike the Polynomial class,
 * you can represent both <tt>2+3x</tt> and <tt>1+1/x</tt>).
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
 * This class is designed to avoid deep copies wherever possible.
 * In particular, it supports C++11 move constructors and move assignment.
 * Functions that take or return objects by value are designed to be just as
 * efficient as working with references or pointers (any variations to
 * this are noted in the method documentation), and long chains of operators
 * such as <tt>a = b * c + d</tt> do not make unwanted deep copies.
 *
 * The underlying storage method for this class is dense (i.e., all
 * coefficients are explicitly stored, including zero coefficients).
 *
 * See also the class Laurent2, which describes Laurent polynomials in
 * two variables.
 *
 * \ifacespython In Python, the class Laurent refers to the specific
 * template class Laurent<Integer>.
 */
template <typename T>
class Laurent : public ShortOutput<Laurent<T>, true> {
    public:
        typedef T Coefficient;
            /**< The type of each coefficient of the polynomial. */

    private:
        long minExp_;
            /**< The minimum exponent that appears in the polynomial,
                 or 0 if this is the zero polynomial. */
        long maxExp_;
            /**< The maximum exponent that appears in the polynomial,
                 or 0 if this is the zero polynomial. */
        long base_;
            /**< The exponent represented by the coefficient coeff_[0].
                 This is less than or equal to \a minExp_ (and in many
                 scenarios they will be the same). */
        T* coeff_;
            /**< The coefficients of the polynomial.  Specifically, coeff_[i]
                 stores the coefficient of <tt>x^(base_ + i)</tt>.
                 This array has length at least (\a maxExp_ - \a base_ + 1). */

        static const T zero_;
            /**< A zero coefficient that we can safely make references to. */

    public:
        /**
         * Creates the zero polynomial.
         */
        Laurent();

        /**
         * Creates the polynomial <tt>x^d</tt> for the given exponent \a d.
         *
         * @param exponent the exponent to use for the new polynomial.
         */
        explicit Laurent(long exponent);

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
        Laurent(const Laurent<T>& value);

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
        Laurent(const Laurent<U>& value);

        /**
         * Moves the contents of the given polynomial to this new polynomial.
         * This is a fast (constant time) operation.
         *
         * The polynomial that was passed (\a value) will no longer be usable.
         *
         * @param value the polynomial to move.
         */
        Laurent(Laurent<T>&& value) noexcept;

        /**
         * Creates a new polynomial from the given sequence of coefficients.
         *
         * The coefficients should appear in order from the smallest
         * exponent term to the largest.  The first coefficient in the
         * sequence will be associated with the exponent \a minExp.
         *
         * There is no problem if the first and/or last coefficient in
         * the sequence is zero.
         * An empty sequence will be treated as the zero polynomial.
         *
         * This constructor induces a deep copy of the given range.
         *
         * \pre Objects of type \a T can be assigned values from
         * dereferenced iterators of type \a iterator.
         *
         * \ifacespython Instead of the iterators \a begin and \a end,
         * this routine takes a python list of coefficients.
         *
         * @param minExp the exponent corresponding to the first
         * coefficient in the sequence.
         * @param begin the beginning of the sequence of coefficients.
         * @param end a past-the-end iterator indicating the end of the
         * sequence of coefficients.
         */
        template <typename iterator>
        Laurent(long minExp, iterator begin, iterator end);

        /**
         * Creates a new polynomial from a hard-coded sequence of coefficients.
         *
         * This constructor takes a C++11 initialiser list, which should
         * contain the coefficients in order from the smallest exponent
         * term to the largest.  The first coefficient in the sequence
         * will be associated with the exponent \a minExp.
         *
         * There is no problem if the first and/or last coefficient in
         * the sequence is zero.
         * An empty sequence will be treated as the zero polynomial.
         *
         * \ifacespython Not available, but there is a Python constructor
         * that takes a list of coefficients (which need not be constant).
         *
         * @param minExp the exponent corresponding to the first
         * coefficient in the sequence.
         * @param coefficients the full sequence of coefficients.
         */
        Laurent(long minExp, std::initializer_list<T> coefficients);

        /**
         * Destroys this polynomial.
         */
        ~Laurent();

        /**
         * Sets this to become the zero polynomial.
         */
        void init();

        /**
         * Sets this to become the polynomial <tt>x^d</tt> for the
         * given exponent \a d.
         *
         * @param exponent the new exponent to use for this polynomial.
         */
        void init(long exponent);

        /**
         * Sets this to become the polynomial described by the given
         * sequence of coefficients.
         *
         * The coefficients should appear in order from the smallest
         * exponent term to the largest.  The first coefficient in the
         * sequence will be associated with the exponent \a minExp.
         *
         * There is no problem if the first and/or last coefficient in
         * the sequence is zero.
         * An empty sequence will be treated as the zero polynomial.
         *
         * This routine induces a deep copy of the given range.
         *
         * \pre Objects of type \a T can be assigned values from
         * dereferenced iterators of type \a iterator.
         *
         * \ifacespython Instead of the iterators \a begin and \a end,
         * this routine takes a python list of coefficients.
         *
         * @param minExp the exponent corresponding to the first
         * coefficient in the sequence.
         * @param begin the beginning of the sequence of coefficients.
         * @param end a past-the-end iterator indicating the end of the
         * sequence of coefficients.
         */
        template <typename iterator>
        void init(long minExp, iterator begin, iterator end);

        /**
         * Returns the smallest exponent that appears in this polynomial
         * with a non-zero coefficient.
         * If this is the zero polynomial, then this routine returns 0.
         *
         * @return the smallest exponent.
         */
        long minExp() const;

        /**
         * Returns the largest exponent that appears in this polynomial
         * with a non-zero coefficient.
         * If this is the zero polynomial, then this routine returns 0.
         *
         * @return the largest exponent.
         */
        long maxExp() const;

        /**
         * Returns whether this is the zero polynomial.
         *
         * @return \c true if and only if this is the zero polynomial.
         */
        bool isZero() const;

        /**
         * Returns the given coefficient of this polynomial.
         * There are no restrictions on the exponent \a exp.
         *
         * \ifacespython Python users can also use this operator to \e set
         * cofficients; that is, you can write <tt>poly[exp] = value</tt>.
         * In contrast, C++ users must use the separate routine set(), due to
         * the fact that in C++ this square bracket operator is const.
         *
         * @param exp the exponent of the term whose coefficient should
         * be returned.
         * @return the coefficient of the given term.
         */
        const T& operator [] (long exp) const;

        /**
         * Changes the given coefficient of this polynomial.
         *
         * There are no restrictions on the exponent \a exp, and the
         * new coefficient \a value may be zero.
         *
         * Note, however, that it is expensive to set a non-zero coefficient
         * whose exponent is larger than maxExp() or smaller than minExp(),
         * since this will typically require deallocating and reallocating
         * the full list of coefficients.
         *
         * In contrast, setting a zero coefficient for the exponent
         * maxExp() or minExp() is cheap, even though the range of
         * non-zero coefficients changes as a result.
         *
         * \ifacespython This set() routine is available, but you can
         * also set coefficients directly using syntax of the form
         * <tt>p[exp] = value</tt>.
         *
         * @param exp the exponent of the term whose coefficient should
         * be changed.
         * @param value the new value of this coefficient.
         */
        void set(long exp, const T& value);

        /**
         * Tests whether this and the given polynomial are equal.
         *
         * @param rhs the polynomial to compare with this.
         * @return \c true if and only if this and the given polynomial
         * are equal.
         */
        bool operator == (const Laurent<T>& rhs) const;

        /**
         * Tests whether this and the given polynomial are not equal.
         *
         * @param rhs the polynomial to compare with this.
         * @return \c true if and only if this and the given polynomial
         * are not equal.
         */
        bool operator != (const Laurent<T>& rhs) const;

        /**
         * Sets this to be a copy of the given polynomial.
         *
         * This and the given polynomial need not have the same minimum
         * and/or maximum exponents.
         *
         * This operator induces a deep copy of \a value.
         *
         * A note to developers: although this is identical to the templated
         * assignment operator, it must be declared and implemented separately.
         * See the copy constructor for further details.
         *
         * @param value the polynomial to copy.
         * @return a reference to this polynomial.
         */
        Laurent& operator = (const Laurent<T>& value);

        /**
         * Sets this to be a copy of the given polynomial.
         *
         * This and the given polynomial need not have the same minimum
         * and/or maximum exponents.
         *
         * This operator induces a deep copy of \a value.
         *
         * @param value the polynomial to copy.
         * @return a reference to this polynomial.
         */
        template <typename U>
        Laurent& operator = (const Laurent<U>& value);

        /**
         * Moves the contents of the given polynomial to this polynomial.
         * This is a fast (constant time) operation.
         *
         * This and the given polynomial need not have the same minimum
         * and/or maximum exponents.
         *
         * The polynomial that was passed (\a value) will no longer be usable.
         *
         * @param value the polynomial to move.
         * @return a reference to this polynomial.
         */
        Laurent& operator = (Laurent<T>&& value) noexcept;

        /**
         * Swaps the contents of this and the given polynomial.
         * This is a fast (constant time) operation.
         *
         * This and the given polynomial do not need to have the same
         * minimum and/or maximum exponents.
         *
         * @param other the polynomial whose contents should be swapped
         * with this.
         */
        void swap(Laurent<T>& other);

        /**
         * Multiplies this polynomial by <tt>x^s</tt> for some integer \a s.
         *
         * @param s the power of \a x to multiply by.
         */
        void shift(long s);

        /**
         * Multiplies all exponents in this polynomial by \a k for some
         * integer \a k.  This is equivalent to replacing the variable
         * \a x of the polynomial with <i>x</i><sup><i>k</i></sup>.
         *
         * Both positive and negative scaling factors \a k are allowed.
         *
         * \pre \a k is non-zero.
         *
         * @param k the scaling factor to multiply exponents by.
         */
        void scaleUp(long k);

        /**
         * Divides all exponents in this polynomial by \a k for some
         * integer \a k.  This is equivalent to replacing the variable
         * \a x of the polynomial with <i>x</i><sup>1/<i>k</i></sup>.
         *
         * Both positive and negative scaling factors \a k are allowed.
         *
         * \pre \a k is non-zero.
         * \pre All exponents in this polynomial with non-zero coefficients
         * are multiples of \a k.
         *
         * @param k the scaling factor to divide exponents by.
         */
        void scaleDown(long k);

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
        Laurent& operator *= (const T& scalar);

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
        Laurent& operator /= (const T& scalar);

        /**
         * Adds the given polynomial to this.
         *
         * The given polynomial need not have the same minimum and/or
         * maximum exponents as this.
         *
         * \warning This routine may trigger a deep copy (depending upon
         * the range of exponents used in \a other).  Consider using
         * the binary <tt>+</tt> operator instead, which is better able to
         * avoid this deep copy where possible.
         *
         * @param other the polynomial to add to this.
         * @return a reference to this polynomial.
         */
        Laurent& operator += (const Laurent<T>& other);

        /**
         * Subtracts the given polynomial from this.
         *
         * The given polynomial need not have the same minimum and/or
         * maximum exponents as this.
         *
         * @param other the polynomial to subtract from this.
         * @return a reference to this polynomial.
         */
        Laurent& operator -= (const Laurent<T>& other);

        /**
         * Multiplies this by the given polynomial.
         *
         * The given polynomial need not have the same minimum and/or
         * maximum exponents as this.
         *
         * @param other the polynomial to multiply this by.
         * @return a reference to this polynomial.
         */
        Laurent& operator *= (const Laurent<T>& other);

        /**
         * Writes this polynomial to the given output stream, using the
         * given variable name instead of \c x.
         *
         * If \a utf8 is passed as \c true then unicode superscript characters
         * will be used for exponents and the minus sign; these will be encoded
         * using UTF-8.  This will make the output nicer, but will require more
         * complex fonts to be available on the user's machine.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         * @param utf8 \c true if unicode characters may be used.
         * @param variable the symbol to use for the variable in this
         * polynomial.  This may be \c null, in which case the default
         * variable \c x will be used.
         * @return a reference to the given output stream.
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
         * In particular, it makes use of superscript digits for exponents
         * and a wider minus sign.
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
         * Constructs a new polynomial with the given exponent range and
         * coefficients.  It is assumed that the coefficient array starts
         * at exponent \a minExp.
         *
         * The data members \a minExp_, \a maxExp_ and \a coeff_ will be
         * set to the given values, and \a base_ will be set to \a minExp.
         * In particular, the new object will take ownership of the
         * coefficient array.
         *
         * The coefficient array may have leading or trailing zeroes,
         * but if this is a possibility then you \e must pass
         * \a checkZeroes as \c true.
         */
        Laurent(long minExp, long maxExp, T* coeff, bool checkZeroes = false);

        /**
         * Expands the array of coefficients if necessary so that
         * minExp_ <= newMin_ and maxExp_ >= newMax.
         *
         * The value of \a minExp_ might decrease, but it will not increase.
         * The value of \a maxExp_ might increase, but it will not decrease.
         * All new coefficients that appear in this expanded range will
         * be set to zero.
         *
         * Note that the resulting polynomial might have zero
         * coefficients at the exponents \a minExp_ and/or \a maxExp_.
         *
         * This routine is used (for example) in the implementations
         * of += and -=.
         */
        void reallocateForRange(long newMin, long newMax);

        /**
         * Increases \a minExp_ and/or decreases \a maxExp_ to ensure
         * that both exponents \a minExp_ and \a maxExp_ have non-zero
         * coefficients.  If this is the zero polynomial then all of
         * \a minExp_, \a maxExp_ and \a base_ will be set to zero.
         */
        void fixDegrees();

        /**
         * Replaces the contents of this polynomial with \a other - \a this.
         *
         * This is equivalent to calling the -= operator and then negating.
         *
         * @param other the polynomial to subtract this from.
         * @return a reference to this polynomial.
         */
        Laurent<T>& subtractFrom(const Laurent<T>& other);

    template <typename U>
    friend Laurent<U> operator + (const Laurent<U>&, const Laurent<U>&);

    template <typename U>
    friend Laurent<U> operator + (Laurent<U>&&, Laurent<U>&&);

    template <typename U>
    friend Laurent<U> operator - (const Laurent<U>&, const Laurent<U>&);

    template <typename U>
    friend Laurent<U> operator - (const Laurent<U>&, Laurent<U>&&);

    template <typename U>
    friend Laurent<U> operator - (Laurent<U>&&, Laurent<U>&&);

    template <typename U>
    friend Laurent<U> operator * (const Laurent<U>&, const Laurent<U>&);
};

/**
 * Multiplies the given polynomial by the given scalar constant.
 *
 * The scalar is simply of type \a T; we use the identical type
 * Laurent<T>::Coefficient here to assist with C++ template type matching.
 *
 * @param poly the polynomial to multiply by.
 * @param scalar the scalar to multiply by.
 * @return the product of the given polynomial and scalar.
 */
template <typename T>
Laurent<T> operator * (Laurent<T> poly,
    const typename Laurent<T>::Coefficient& scalar);

/**
 * Multiplies the given polynomial by the given scalar constant.
 *
 * The scalar is simply of type \a T; we use the identical type
 * Laurent<T>::Coefficient here to assist with C++ template type matching.
 *
 * @param scalar the scalar to multiply by.
 * @param poly the polynomial to multiply by.
 * @return the product of the given polynomial and scalar.
 */
template <typename T>
Laurent<T> operator * (const typename Laurent<T>::Coefficient& scalar,
    Laurent<T> poly);

/**
 * Divides the given polynomial by the given scalar constant.
 *
 * This uses the division operator /= for the coefficient type \a T.
 *
 * The scalar is simply of type \a T; we use the identical type
 * Laurent<T>::Coefficient here to assist with C++ template type matching.
 *
 * \pre The argument \a scalar is non-zero.
 *
 * @param poly the polynomial to divide by the given scalar.
 * @param scalar the scalar factor to divide by.
 * @return the quotient of the given polynomial by the given scalar.
 */
template <typename T>
Laurent<T> operator / (Laurent<T> poly,
    const typename Laurent<T>::Coefficient& scalar);

/**
 * Adds the two given polynomials.
 *
 * This operator <tt>+</tt> is sometimes faster than using <tt>+=</tt>,
 * since it has more flexibility to avoid an internal deep copy.
 *
 * @param lhs the first polynomial to add.
 * @param rhs the second polynomial to add.
 * @return the sum of both polynomials.
 */
template <typename T>
Laurent<T> operator + (const Laurent<T>& lhs, const Laurent<T>& rhs);

/**
 * Adds the two given polynomials.
 *
 * This operator <tt>+</tt> is sometimes faster than using <tt>+=</tt>,
 * since it has more flexibility to avoid an internal deep copy.
 *
 * @param lhs the first polynomial to add.
 * @param rhs the second polynomial to add.
 * @return the sum of both polynomials.
 */
template <typename T>
Laurent<T> operator + (Laurent<T>&& lhs, const Laurent<T>& rhs);

/**
 * Adds the two given polynomials.
 *
 * This operator <tt>+</tt> is sometimes faster than using <tt>+=</tt>,
 * since it has more flexibility to avoid an internal deep copy.
 *
 * @param lhs the first polynomial to add.
 * @param rhs the second polynomial to add.
 * @return the sum of both polynomials.
 */
template <typename T>
Laurent<T> operator + (const Laurent<T>& lhs, Laurent<T>&& rhs);

/**
 * Adds the two given polynomials.
 *
 * This operator <tt>+</tt> is sometimes faster than using <tt>+=</tt>,
 * since it has more flexibility to avoid an internal deep copy.
 *
 * @param lhs the first polynomial to add.
 * @param rhs the second polynomial to add.
 * @return the sum of both polynomials.
 */
template <typename T>
Laurent<T> operator + (Laurent<T>&& lhs, Laurent<T>&& rhs);

/**
 * Returns the negative of the given polynomial.
 *
 * @param arg the polynomial to negate.
 * @return the negative of \a arg.
 */
template <typename T>
Laurent<T> operator - (Laurent<T> arg);

/**
 * Subtracts the two given polynomials.
 *
 * This operator <tt>-</tt> is sometimes faster than using <tt>-=</tt>,
 * since it has more flexibility to avoid an internal deep copy.
 *
 * @param lhs the polynomial to sutract \a rhs from.
 * @param rhs the polynomial to subtract from \a lhs.
 * @return the difference of the two given polynomials.
 */
template <typename T>
Laurent<T> operator - (const Laurent<T>& lhs, const Laurent<T>& rhs);

/**
 * Subtracts the two given polynomials.
 *
 * This operator <tt>-</tt> is sometimes faster than using <tt>-=</tt>,
 * since it has more flexibility to avoid an internal deep copy.
 *
 * @param lhs the polynomial to sutract \a rhs from.
 * @param rhs the polynomial to subtract from \a lhs.
 * @return the difference of the two given polynomials.
 */
template <typename T>
Laurent<T> operator - (Laurent<T>&& lhs, const Laurent<T>& rhs);

/**
 * Subtracts the two given polynomials.
 *
 * This operator <tt>-</tt> is sometimes faster than using <tt>-=</tt>,
 * since it has more flexibility to avoid an internal deep copy.
 *
 * @param lhs the polynomial to sutract \a rhs from.
 * @param rhs the polynomial to subtract from \a lhs.
 * @return the difference of the two given polynomials.
 */
template <typename T>
Laurent<T> operator - (const Laurent<T>& lhs, Laurent<T>&& rhs);

/**
 * Subtracts the two given polynomials.
 *
 * This operator <tt>-</tt> is sometimes faster than using <tt>-=</tt>,
 * since it has more flexibility to avoid an internal deep copy.
 *
 * @param lhs the polynomial to sutract \a rhs from.
 * @param rhs the polynomial to subtract from \a lhs.
 * @return the difference of the two given polynomials.
 */
template <typename T>
Laurent<T> operator - (Laurent<T>&& lhs, Laurent<T>&& rhs);

/**
 * Multiplies the two given polynomials.
 *
 * @param lhs the first polynomial to multiply.
 * @param rhs the second polynomial to multiply.
 * @return the product of both polynomials.
 */
template <typename T>
Laurent<T> operator * (const Laurent<T>& lhs, const Laurent<T>& rhs);

/*@}*/

template <typename T>
const T Laurent<T>::zero_(0);

template <typename T>
inline Laurent<T>::Laurent() : minExp_(0), maxExp_(0), base_(0),
        coeff_(new T[1]) {
    // The default constructor for T already initialises coeff_[0] to zero.
}

template <typename T>
inline Laurent<T>::Laurent(long exp) :
        minExp_(exp), maxExp_(exp), base_(exp), coeff_(new T[1]) {
    coeff_[0] = 1;
}

template <typename T>
template <typename iterator>
inline Laurent<T>::Laurent(long minExp, iterator begin, iterator end) :
        coeff_(nullptr) {
    init(minExp, begin, end);
}

template <typename T>
inline Laurent<T>::Laurent(long minExp, std::initializer_list<T> coefficients) :
        coeff_(nullptr) {
    init(minExp, coefficients.begin(), coefficients.end());
}

template <typename T>
inline Laurent<T>::Laurent(const Laurent<T>& value) :
        minExp_(value.minExp_), maxExp_(value.maxExp_), base_(value.minExp_),
        coeff_(new T[value.maxExp_ - value.minExp_ + 1]) {
    // std::cerr << "Laurent: deep copy (init)" << std::endl;
    for (size_t i = 0; i <= maxExp_ - minExp_; ++i)
        coeff_[i] = value.coeff_[i + value.minExp_ - value.base_];
}

template <typename T>
template <typename U>
inline Laurent<T>::Laurent(const Laurent<U>& value) :
        minExp_(value.minExp_), maxExp_(value.maxExp_), base_(value.minExp_),
        coeff_(new T[value.maxExp_ - value.minExp_ + 1]) {
    // std::cerr << "Laurent: deep copy (init)" << std::endl;
    for (size_t i = 0; i <= maxExp_ - minExp_; ++i)
        coeff_[i] = value.coeff_[i + value.minExp_ - value.base_];
}

template <typename T>
inline Laurent<T>::Laurent(Laurent<T>&& value) noexcept :
        minExp_(value.minExp_), maxExp_(value.maxExp_), base_(value.base_),
        coeff_(value.coeff_) {
    value.coeff_ = nullptr;
}

template <typename T>
inline Laurent<T>::Laurent(long minExp, long maxExp, T* coeff,
        bool checkZeroes) :
        minExp_(minExp), maxExp_(maxExp), base_(minExp), coeff_(coeff) {
    if (checkZeroes)
        fixDegrees();
}

template <typename T>
inline Laurent<T>::~Laurent() {
    delete[] coeff_;
}

template <typename T>
inline void Laurent<T>::init() {
    delete[] coeff_;
    minExp_ = maxExp_ = base_ = 0;
    coeff_ = new T[1];
    // coeff_[0] is initialised to 0 automatically.
}

template <typename T>
inline void Laurent<T>::init(long exp) {
    delete[] coeff_;
    minExp_ = maxExp_ = base_ = exp;
    coeff_ = new T[1];
    coeff_[0] = 1;
}

template <typename T>
template <typename iterator>
void Laurent<T>::init(long minExp, iterator begin, iterator end) {
    delete[] coeff_;

    // Skip through any initial zero terms.
    while (begin != end && *begin == 0) {
        ++begin;
        ++minExp;
    }

    if (begin == end) {
        minExp_ = maxExp_ = base_ = 0;
        coeff_ = new T[1];
        return;
    }

    minExp_ = base_ = minExp;
    maxExp_ = minExp + (end - begin) - 1;
    coeff_ = new T[maxExp_ - minExp_ + 1];

    size_t i = 0;
    while (begin != end)
        coeff_[i++] = *begin++;

    // The final coefficient(s) might be zero - fix maxExp_ accordingly.
    // Note that it is guaranteed here that the first coefficient is non-zero.
    while (maxExp_ > minExp_ && coeff_[maxExp_ - minExp_] == 0)
        --maxExp_;
}

template <typename T>
inline long Laurent<T>::minExp() const {
    return minExp_;
}

template <typename T>
inline long Laurent<T>::maxExp() const {
    return maxExp_;
}

template <typename T>
inline bool Laurent<T>::isZero() const {
    return (minExp_ == maxExp_ && coeff_[minExp_ - base_] == 0);
}

template <typename T>
inline const T& Laurent<T>::operator [] (long exp) const {
    if (exp < minExp_ || exp > maxExp_)
        return zero_;
    else
        return coeff_[exp - base_];
}

template <typename T>
void Laurent<T>::set(long exp, const T& value) {
    if (value == 0) {
        if (exp == maxExp_) {
            if (minExp_ == maxExp_) {
                // This becomes the zero polynomial.
                coeff_[0] = 0;
                minExp_ = maxExp_ = base_ = 0;
            } else {
                --maxExp_;
                // Note: the lowest-exponent coefficient must be non-zero.
                while (maxExp_ > minExp_ && coeff_[maxExp_ - base_] == 0)
                    --maxExp_;
            }
        } else if (exp == minExp_) {
            // Here we know the highest-exponent coefficient is non-zero.
            ++minExp_;
            while (coeff_[minExp_ - base_] == 0)
                ++minExp_;
        } else if (exp > minExp_ && exp < maxExp_) {
            coeff_[exp - base_] = 0;
        }
        return;
    }

    // From here, value is non-zero.
    if (exp >= minExp_ && exp <= maxExp_) {
        coeff_[exp - base_] = value;
    } else if (isZero()) {
        // Both min and max exponents change.
        minExp_ = maxExp_ = base_ = exp;
        coeff_[0] = value;
    } else if (exp < base_) {
        // The minimum exponent decreases, and we must reallocate.
        T* newCoeff = new T[maxExp_ - exp + 1];
        std::copy(coeff_ + (minExp_ - base_), coeff_ + (maxExp_ - base_) + 1,
            newCoeff + (minExp_ - exp));
        newCoeff[0] = value;

        minExp_ = base_ = exp;
        delete[] coeff_;
        coeff_ = newCoeff;
    } else if (exp < minExp_) {
        // The minimum exponent decreases, but we need not reallocate.
        coeff_[exp - base_] = value;
        for (++exp; exp < minExp_; ++exp)
            coeff_[exp - base_] = 0;
        minExp_ = exp;
    } else {
        // The maximum exponent increases, and we must reallocate.
        T* newCoeff = new T[exp - minExp_ + 1];
        std::copy(coeff_ + (minExp_ - base_), coeff_ + (maxExp_ - base_) + 1,
            newCoeff);
        newCoeff[exp - minExp_] = value;

        base_ = minExp_;
        maxExp_ = exp;
        delete[] coeff_;
        coeff_ = newCoeff;
    }
}

template <typename T>
inline bool Laurent<T>::operator == (const Laurent<T>& rhs) const {
    if (minExp_ != rhs.minExp_ || maxExp_ != rhs.maxExp_)
        return false;
    for (long i = minExp_; i <= maxExp_; ++i)
        if ((*this)[i] != rhs[i])
            return false;
    return true;
}

template <typename T>
inline bool Laurent<T>::operator != (const Laurent<T>& rhs) const {
    if (minExp_ != rhs.minExp_ || maxExp_ != rhs.maxExp_)
        return true;
    for (long i = minExp_; i <= maxExp_; ++i)
        if ((*this)[i] != rhs[i])
            return true;
    return false;
}

template <typename T>
Laurent<T>& Laurent<T>::operator = (const Laurent<T>& other) {
    // Treat x = x separately, since otherwise we break things when we
    // reset base_ = other.minExp_.
    if (&other == this)
        return *this;

    // std::cerr << "Laurent: deep copy (=)" << std::endl;
    if (maxExp_ - base_ < other.maxExp_ - other.minExp_) {
        delete[] coeff_;
        coeff_ = new T[other.maxExp_ - other.minExp_ + 1];
    }
    base_ = minExp_ = other.minExp_;
    maxExp_ = other.maxExp_;
    for (long exp = minExp_; exp <= maxExp_; ++exp)
        coeff_[exp - base_] = other.coeff_[exp - other.base_];
    return *this;
}

template <typename T>
template <typename U>
Laurent<T>& Laurent<T>::operator = (const Laurent<U>& other) {
    // Treat x = x separately, since otherwise we break things when we
    // reset base_ = other.minExp_.
    if (&other == this)
        return *this;

    // std::cerr << "Laurent: deep copy (=)" << std::endl;
    if (maxExp_ - base_ < other.maxExp_ - other.minExp_) {
        delete[] coeff_;
        coeff_ = new T[other.maxExp_ - other.minExp_ + 1];
    }
    base_ = minExp_ = other.minExp_;
    maxExp_ = other.maxExp_;
    for (long exp = minExp_; exp <= maxExp_; ++exp)
        coeff_[exp - base_] = other.coeff_[exp - other.base_];
    return *this;
}

template <typename T>
inline Laurent<T>& Laurent<T>::operator = (Laurent<T>&& other) noexcept {
    // Strictly speaking we could just assign these integers.
    std::swap(minExp_, other.minExp_);
    std::swap(maxExp_, other.maxExp_);
    std::swap(base_, other.base_);
    // Let other dispose of the original contents in its own destructor.
    std::swap(coeff_, other.coeff_);
    return *this;
}

template <typename T>
inline void Laurent<T>::swap(Laurent<T>& other) {
    std::swap(minExp_, other.minExp_);
    std::swap(maxExp_, other.maxExp_);
    std::swap(base_, other.base_);
    std::swap(coeff_, other.coeff_);
}

template <typename T>
inline void Laurent<T>::shift(long s) {
    base_ += s;
    minExp_ += s;
    maxExp_ += s;
}

template <typename T>
void Laurent<T>::scaleUp(long k) {
    if (k == 1)
        return;

    if (minExp_ == maxExp_ && base_ == minExp_) {
        minExp_ *= k;
        maxExp_ *= k;
        base_ *= k;
        return;
    }

    T* newCoeff;
    if (k > 0) {
        newCoeff = new T[k * (maxExp_ - minExp_) + 1];
        for (long i = 0; i <= maxExp_ - minExp_; ++i)
            newCoeff[k * i] = coeff_[minExp_ - base_ + i];
        minExp_ *= k;
        maxExp_ *= k;
    } else {
        newCoeff = new T[k * (minExp_ - maxExp_) + 1];
        for (long i = maxExp_ - minExp_; i >= 0; --i)
            newCoeff[(-k) * i] = coeff_[maxExp_ - base_ - i];
        minExp_ *= k;
        maxExp_ *= k;
        std::swap(minExp_, maxExp_);
    }

    base_ = minExp_;
    delete[] coeff_;
    coeff_ = newCoeff;
}

template <typename T>
void Laurent<T>::scaleDown(long k) {
    if (k == 1)
        return;

    if (minExp_ == maxExp_ && base_ == minExp_) {
        minExp_ /= k;
        maxExp_ /= k;
        base_ /= k;
        return;
    }

    T* newCoeff;
    if (k > 0) {
        newCoeff = new T[(maxExp_ - minExp_) / k + 1];
        for (long i = 0; i <= (maxExp_ - minExp_) / k; ++i)
            newCoeff[i] = coeff_[minExp_ - base_ + k * i];
        minExp_ /= k;
        maxExp_ /= k;
    } else {
        newCoeff = new T[(minExp_ - maxExp_) / k + 1];
        for (long i = (minExp_ - maxExp_) / k; i >= 0; --i)
            newCoeff[i] = coeff_[maxExp_ - base_ + k * i];
        minExp_ /= k;
        maxExp_ /= k;
        std::swap(minExp_, maxExp_);
    }

    base_ = minExp_;
    delete[] coeff_;
    coeff_ = newCoeff;
}

template <typename T>
inline void Laurent<T>::negate() {
    for (long exp = minExp_; exp <= maxExp_; ++exp)
        if (coeff_[exp - base_] != 0)
            coeff_[exp - base_] = -coeff_[exp - base_];
}

template <typename T>
Laurent<T>& Laurent<T>::operator *= (const T& scalar) {
    if (scalar == 0)
        init();
    else {
        for (long exp = minExp_; exp <= maxExp_; ++exp)
            coeff_[exp - base_] *= scalar;
    }
    return *this;
}

template <typename T>
inline Laurent<T>& Laurent<T>::operator /= (const T& scalar) {
    for (long exp = minExp_; exp <= maxExp_; ++exp)
        coeff_[exp - base_] /= scalar;

    // For integer division, we could have zeroed out some coefficients.
    fixDegrees();
    return *this;
}

template <typename T>
inline Laurent<T>& Laurent<T>::operator += (const Laurent<T>& other) {
    // This routine works even if &other == this, since in this case we do not
    // reallocate.

    // Handle zero separately, since we do not care about ranges in this case.
    if (other.isZero())
        return *this;

    reallocateForRange(other.minExp_, other.maxExp_);

    for (long exp = other.minExp_; exp <= other.maxExp_; ++exp)
        coeff_[exp - base_] += other.coeff_[exp - other.base_];

    // We might have zeroed out some coefficients.
    fixDegrees();
    return *this;
}

template <typename T>
inline Laurent<T>& Laurent<T>::operator -= (const Laurent<T>& other) {
    // This routine works even if &other == this, since in this case we do not
    // reallocate.

    // Handle zero separately, since we do not care about ranges in this case.
    if (other.isZero())
        return *this;

    reallocateForRange(other.minExp_, other.maxExp_);

    for (long exp = other.minExp_; exp <= other.maxExp_; ++exp)
        coeff_[exp - base_] -= other.coeff_[exp - other.base_];

    // We might have zeroed out some coefficients.
    fixDegrees();
    return *this;
}

template <typename T>
Laurent<T>& Laurent<T>::operator *= (const Laurent<T>& other) {
    if (isZero())
        return *this;
    if (other.isZero()) {
        init();
        return *this;
    }

    // The following code works even if &other == this, since we construct the
    // coefficients of the product in a separate section of memory.
    // std::cerr << "Laurent: deep copy (*=)" << std::endl;
    long i, j;
    T* ans = new T[maxExp_ - minExp_ + other.maxExp_ - other.minExp_ + 1];
    for (i = minExp_; i <= maxExp_; ++i)
        for (j = other.minExp_; j <= other.maxExp_; ++j)
            ans[i + j - minExp_ - other.minExp_] +=
                (coeff_[i - base_] * other.coeff_[j - other.base_]);

    delete[] coeff_;
    coeff_ = ans;
    minExp_ += other.minExp_;
    maxExp_ += other.maxExp_;
    base_ = minExp_;

    // Both leading coefficients are non-zero, so the degree is correct.
    return *this;
}

template <typename T>
void Laurent<T>::writeTextShort(std::ostream& out, bool utf8,
        const char* variable) const {
    if (isZero()) {
        out << '0';
        return;
    }

    // Both minExp_ and maxExp_ have non-zero coefficients (though
    // minExp_ and maxExp_ might be the same exponent).
    for (long exp = maxExp(); exp >= minExp(); --exp) {
        if ((*this)[exp] == 0)
            continue;

        T writeCoeff = (*this)[exp];
        if (exp == maxExp()) {
            // This is the first term being output.
            if (writeCoeff < 0) {
                if (utf8)
                    out << "\u2212";
                else
                    out << '-';
                writeCoeff = -writeCoeff;
            }
        } else {
            if (writeCoeff < 0) {
                if (utf8)
                    out << " \u2212 ";
                else
                    out << " - ";
                writeCoeff = -writeCoeff;
            } else
                out << " + ";
        }

        // From here, it is guaranteed that writeCoeff > 0.
        if (exp == 0)
            out << writeCoeff;
        else {
            if (writeCoeff != 1)
                out << writeCoeff << ' ';
            if (variable)
                out << variable;
            else
                out << 'x';
            if (exp != 1) {
                if (utf8)
                    out << regina::superscript(exp);
                else
                    out << '^' << exp;
            }
        }
    }
}

template <typename T>
inline std::string Laurent<T>::str(const char* variable) const {
    // Make sure that python will be able to find the inherited str().
    static_assert(std::is_same<typename OutputBase<Laurent<T>>::type,
        Output<Laurent<T>, true>>::value,
        "Laurent<T> is not identified as being inherited from Output<...>");

    std::ostringstream out;
    writeTextShort(out, false, variable);
    return out.str();
}

template <typename T>
inline std::string Laurent<T>::utf8(const char* variable) const {
    std::ostringstream out;
    writeTextShort(out, true, variable);
    return out.str();
}

template <typename T>
void Laurent<T>::reallocateForRange(long newMin, long newMax) {
    long exp;
    if (base_ > newMin) {
        // We must reallocate.
        // std::cerr << "Laurent: deep copy (reallocate)" << std::endl;
        if (maxExp_ < newMax) {
            // newMin < base_ <= minExp_ <= maxExp_ < newMax
            T* newCoeff = new T[newMax - newMin + 1];
            for (exp = minExp_; exp <= maxExp_; ++exp)
                newCoeff[exp - newMin] = coeff_[exp - base_];
            delete[] coeff_;
            coeff_ = newCoeff;

            minExp_ = base_ = newMin;
            maxExp_ = newMax;
        } else {
            // newMin < base_ <= minExp_  &&  newMax <= maxExp_
            T* newCoeff = new T[maxExp_ - newMin + 1];
            for (exp = minExp_; exp <= maxExp_; ++exp)
                newCoeff[exp - newMin] = coeff_[exp - base_];
            delete[] coeff_;
            coeff_ = newCoeff;

            minExp_ = base_ = newMin;
        }
    } else if (maxExp_ < newMax) {
        // Still, we must reallocate.
        // std::cerr << "Laurent: deep copy (reallocate)" << std::endl;
        // (Actually, if base_ is far enough below minExp_ then we might not
        // have to reallocate, but we'll do it for now anyway.)
        if (minExp_ <= newMin) {
            // base_ <= minExp_ <= newMin  &&  maxExp_ < newMax
            T* newCoeff = new T[newMax - minExp_ + 1];
            for (exp = minExp_; exp <= maxExp_; ++exp)
                newCoeff[exp - minExp_] = coeff_[exp - base_];
            delete[] coeff_;
            coeff_ = newCoeff;

            base_ = minExp_;
            maxExp_ = newMax;
        } else {
            // base_ <= newMin < minExp_ <= maxExp_ < newMax
            T* newCoeff = new T[newMax - newMin + 1];
            for (exp = minExp_; exp <= maxExp_; ++exp)
                newCoeff[exp - newMin] = coeff_[exp - base_];
            delete[] coeff_;
            coeff_ = newCoeff;

            minExp_ = base_ = newMin;
            maxExp_ = newMax;
        }
    } else if (minExp_ > newMin) {
        // base_ <= newMin  &&  newMax <= maxExp_
        // We don't need to reallocate, but minExp_ will drop - we must
        // zero out everything from newMin to minExp_-1.
        for (exp = newMin; exp < minExp_; ++exp)
            coeff_[exp - base_] = 0;

        minExp_ = newMin;
    }
}

template <typename T>
void Laurent<T>::fixDegrees() {
    while (maxExp_ > minExp_ && coeff_[maxExp_ - base_] == 0)
        --maxExp_;
    while (minExp_ < maxExp_ && coeff_[minExp_ - base_] == 0)
        ++minExp_;
    if (minExp_ == maxExp_ && coeff_[minExp_ - base_] == 0) {
        // We have the zero polynomial now.
        // Note however that our zero coefficient might not be in the first
        // slot of the coeff_ array, since we might have base_ < minExp_.
        base_ -= minExp_;
        minExp_ = maxExp_ = 0;
    }
}

template <typename T>
inline Laurent<T>& Laurent<T>::subtractFrom(const Laurent<T>& other) {
    // This routine works even if &other == this, since in this case we do not
    // reallocate.

    // Handle zero separately, since we do not care about ranges in this case.
    if (other.isZero()) {
        negate();
        return *this;
    }

    reallocateForRange(other.minExp_, other.maxExp_);

    long exp = (minExp_ < other.minExp_ ? minExp_ : other.minExp_);
    for ( ; exp < other.minExp_; ++exp)
        if (coeff_[exp - base_] != 0)
            coeff_[exp - base_] = -coeff_[exp - base_];
    for ( ; exp <= other.maxExp_; ++exp)
        if (coeff_[exp - base_] != 0)
            coeff_[exp - base_] = other.coeff_[exp - other.base_]
                - coeff_[exp - base_];
        else
            coeff_[exp - base_] = other.coeff_[exp - other.base_];
    for ( ; exp <= maxExp_; ++exp)
        if (coeff_[exp - base_] != 0)
            coeff_[exp - base_] = -coeff_[exp - base_];

    // We might have zeroed out some coefficients.
    fixDegrees();
    return *this;
}

template <typename T>
inline Laurent<T> operator * (Laurent<T> poly,
        const typename Laurent<T>::Coefficient& scalar) {
    // When the argument poly is an lvalue reference, we perform a deep copy
    // due to pass-by-value.  If scalar == 0 then we don't need this deep copy,
    // since the argument can be ignored.  This special-case optimisation
    // would require two different lvalue/rvalue implementations of *, and
    // so we leave it for now.
    poly *= scalar;
    return poly;
}

template <typename T>
inline Laurent<T> operator * (const typename Laurent<T>::Coefficient& scalar,
        Laurent<T> poly) {
    // See the notes above on a possible optimisation for scalar == 0.
    poly *= scalar;
    return poly;
}

template <typename T>
inline Laurent<T> operator / (Laurent<T> poly,
        const typename Laurent<T>::Coefficient& scalar) {
    poly /= scalar;
    return poly;
}

template <typename T>
Laurent<T> operator + (const Laurent<T>& lhs, const Laurent<T>& rhs) {
    // Handle zero polynomials separately, since their ranges do not
    // actually contain any coefficients and so should be ignored.
    if (lhs.isZero())
        return rhs;
    if (rhs.isZero())
        return lhs;

    // If the two ranges do not overlap, just copy them separately;
    // this avoids having to worry about jumping over the gap between
    // them in the code that follows.
    if (lhs.maxExp_ < rhs.minExp_) {
        T* coeff = new T[rhs.maxExp_ - lhs.minExp_ + 1];
        std::copy(lhs.coeff_ + lhs.minExp_ - lhs.base_,
            lhs.coeff_ + lhs.maxExp_ + 1 - lhs.base_,
            coeff);
        std::copy(rhs.coeff_ + rhs.minExp_ - rhs.base_,
            rhs.coeff_ + rhs.maxExp_ + 1 - rhs.base_,
            coeff + rhs.minExp_ - lhs.minExp_);
        return Laurent<T>(lhs.minExp_, rhs.maxExp_, coeff);
    } else if (rhs.maxExp_ < lhs.minExp_) {
        T* coeff = new T[lhs.maxExp_ - rhs.minExp_ + 1];
        std::copy(rhs.coeff_ + rhs.minExp_ - rhs.base_,
            rhs.coeff_ + rhs.maxExp_ + 1 - rhs.base_,
            coeff);
        std::copy(lhs.coeff_ + lhs.minExp_ - lhs.base_,
            lhs.coeff_ + lhs.maxExp_ + 1 - lhs.base_,
            coeff + lhs.minExp_ - rhs.minExp_);
        return Laurent<T>(rhs.minExp_, lhs.maxExp_, coeff);
    }

    // We are guaranteed from here on that the two ranges overlap.
    long minExp = std::min(lhs.minExp_, rhs.minExp_);
    long maxExp = std::max(lhs.maxExp_, rhs.maxExp_);
    // std::cerr << "Laurent: deep copy (const +)" << std::endl;
    T* coeff = new T[maxExp - minExp + 1];

    long exp /* next exponent */, idx /* next index into coeff */;

    if (lhs.minExp_ < rhs.minExp_) {
        std::copy(lhs.coeff_ + lhs.minExp_ - lhs.base_,
            lhs.coeff_ + rhs.minExp_ - lhs.base_, coeff);
        exp = rhs.minExp_;
        idx = rhs.minExp_ - lhs.minExp_;
    } else if (rhs.minExp_ < lhs.minExp_) {
        std::copy(rhs.coeff_ + rhs.minExp_ - rhs.base_,
            rhs.coeff_ + lhs.minExp_ - rhs.base_, coeff);
        exp = lhs.minExp_;
        idx = lhs.minExp_ - rhs.minExp_;
    } else {
        exp = lhs.minExp_;
        idx = 0;
    }

    for ( ; exp <= lhs.maxExp_ && exp <= rhs.maxExp_; ++idx, ++exp)
        coeff[idx] = lhs.coeff_[exp - lhs.base_] + rhs.coeff_[exp - rhs.base_];

    // exp is now (lhs.maxExp_ + 1) or (rhs.maxExp_ + 1).
    if (exp <= lhs.maxExp_) {
        std::copy(lhs.coeff_ + exp - lhs.base_,
            lhs.coeff_ + lhs.maxExp_ + 1 - lhs.base_, coeff + idx);
    } else if (exp <= rhs.maxExp_) {
        std::copy(rhs.coeff_ + exp - rhs.base_,
            rhs.coeff_ + rhs.maxExp_ + 1 - rhs.base_, coeff + idx);
    }

    return Laurent<T>(minExp, maxExp, coeff, true);
}

template <typename T>
inline Laurent<T> operator + (Laurent<T>&& lhs, const Laurent<T>& rhs) {
    return std::move(lhs += rhs);
}

template <typename T>
inline Laurent<T> operator + (const Laurent<T>& lhs, Laurent<T>&& rhs) {
    return std::move(rhs += lhs);
}

template <typename T>
inline Laurent<T> operator + (Laurent<T>&& lhs, Laurent<T>&& rhs) {
    // If we can, choose a direction for the addition that avoids a
    // deep copy within +=.
    if (lhs.base_ <= rhs.minExp_ && rhs.maxExp_ <= lhs.maxExp_) {
        // We can avoid the deep copy if we start with LHS.
        return std::move(lhs += rhs);
    } else {
        // Either we can avoid the deep copy if we start with RHS,
        // or else we cannot avoid the deep copy at all.
        return std::move(rhs += lhs);
    }
}

template <typename T>
inline Laurent<T> operator - (Laurent<T> arg) {
    arg.negate();
    return arg;
}

template <typename T>
Laurent<T> operator - (const Laurent<T>& lhs, const Laurent<T>& rhs) {
    // Handle zero polynomials separately, since their ranges do not
    // actually contain any coefficients and so should be ignored.
    if (rhs.isZero())
        return lhs;
    if (lhs.isZero())
        return -rhs;

    // If the two ranges do not overlap, just copy them separately;
    // this avoids having to worry about jumping over the gap between
    // them in the code that follows.
    if (lhs.maxExp_ < rhs.minExp_) {
        T* coeff = new T[rhs.maxExp_ - lhs.minExp_ + 1];
        std::copy(lhs.coeff_ + lhs.minExp_ - lhs.base_,
            lhs.coeff_ + lhs.maxExp_ + 1 - lhs.base_,
            coeff);
        T *rit, *cit;
        for (rit = rhs.coeff_ + rhs.minExp_ - rhs.base_,
                    cit = coeff + rhs.minExp_ - lhs.minExp_;
                rit != rhs.coeff_ + rhs.maxExp_ + 1 - rhs.base_;
                ++rit, ++cit)
            if (*rit != 0)
                *cit = -(*rit);
        return Laurent<T>(lhs.minExp_, rhs.maxExp_, coeff);
    } else if (rhs.maxExp_ < lhs.minExp_) {
        T* coeff = new T[lhs.maxExp_ - rhs.minExp_ + 1];
        T *rit, *cit;
        for (rit = rhs.coeff_ + rhs.minExp_ - rhs.base_, cit = coeff;
                rit != rhs.coeff_ + rhs.maxExp_ + 1 - rhs.base_;
                ++rit, ++cit)
            if (*rit != 0)
                *cit = -(*rit);
        std::copy(lhs.coeff_ + lhs.minExp_ - lhs.base_,
            lhs.coeff_ + lhs.maxExp_ + 1 - lhs.base_,
            coeff + lhs.minExp_ - rhs.minExp_);
        return Laurent<T>(rhs.minExp_, lhs.maxExp_, coeff);
    }

    // We are guaranteed from here on that the two ranges overlap.
    long minExp = std::min(lhs.minExp_, rhs.minExp_);
    long maxExp = std::max(lhs.maxExp_, rhs.maxExp_);
    // std::cerr << "Laurent: deep copy (const -)" << std::endl;
    T* coeff = new T[maxExp - minExp + 1];

    long exp /* next exponent */, idx /* next index into coeff */;

    if (lhs.minExp_ < rhs.minExp_) {
        std::copy(lhs.coeff_ + lhs.minExp_ - lhs.base_,
            lhs.coeff_ + rhs.minExp_ - lhs.base_, coeff);
        exp = rhs.minExp_;
        idx = rhs.minExp_ - lhs.minExp_;
    } else if (rhs.minExp_ < lhs.minExp_) {
        T *rit, *cit;
        for (rit = rhs.coeff_ + rhs.minExp_ - rhs.base_, cit = coeff;
                rit != rhs.coeff_ + lhs.minExp_ - rhs.base_;
                ++rit, ++cit)
            if (*rit != 0)
                *cit = -(*rit);
        exp = lhs.minExp_;
        idx = lhs.minExp_ - rhs.minExp_;
    } else {
        exp = lhs.minExp_;
        idx = 0;
    }

    for ( ; exp <= lhs.maxExp_ && exp <= rhs.maxExp_; ++idx, ++exp)
        coeff[idx] = lhs.coeff_[exp - lhs.base_] - rhs.coeff_[exp - rhs.base_];

    // exp is now (lhs.maxExp_ + 1) or (rhs.maxExp_ + 1).
    if (exp <= lhs.maxExp_) {
        std::copy(lhs.coeff_ + exp - lhs.base_,
            lhs.coeff_ + lhs.maxExp_ + 1 - lhs.base_, coeff + idx);
    } else if (exp <= rhs.maxExp_) {
        T *rit, *cit;
        for (rit = rhs.coeff_ + exp - rhs.base_, cit = coeff + idx;
                rit != rhs.coeff_ + rhs.maxExp_ + 1 - rhs.base_;
                ++rit, ++cit)
            if (*rit != 0)
                *cit = -(*rit);
    }

    return Laurent<T>(minExp, maxExp, coeff, true);
}

template <typename T>
Laurent<T> operator - (Laurent<T>&& lhs, const Laurent<T>& rhs) {
    return std::move(lhs -= rhs);
}

template <typename T>
Laurent<T> operator - (const Laurent<T>& lhs, Laurent<T>&& rhs) {
    return std::move(rhs.subtractFrom(lhs));
}

template <typename T>
Laurent<T> operator - (Laurent<T>&& lhs, Laurent<T>&& rhs) {
    // If we can, choose a direction for the subtraction that avoids a
    // deep copy within -= / subtractFrom.
    // Prefer the -= operator if we can't avoid the deep copy.
    if (rhs.base_ <= lhs.minExp_ && lhs.maxExp_ <= rhs.maxExp_)
        return std::move(rhs.subtractFrom(lhs));
    else
        return std::move(lhs -= rhs);
}

template <typename T>
Laurent<T> operator * (const Laurent<T>& lhs, const Laurent<T>& rhs) {
    if (lhs.isZero() || rhs.isZero())
        return Laurent<T>(); // zero

    // std::cerr << "Laurent: deep copy (const *)" << std::endl;
    long i, j;
    T* coeff = new T[lhs.maxExp_ - lhs.minExp_ + rhs.maxExp_ - rhs.minExp_ + 1];
    for (i = lhs.minExp_; i <= lhs.maxExp_; ++i)
        for (j = rhs.minExp_; j <= rhs.maxExp_; ++j)
            coeff[i + j - lhs.minExp_ - rhs.minExp_] +=
                (lhs.coeff_[i - lhs.base_] * rhs.coeff_[j - rhs.base_]);

    // Note: the final minExp/maxExp coefficients will both be non-zero,
    // since the same is true of both lhs and rhs.
    return Laurent<T>(lhs.minExp_ + rhs.minExp_, lhs.maxExp_ + rhs.maxExp_,
        coeff);
}

} // namespace regina

#endif
