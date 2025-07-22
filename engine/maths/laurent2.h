
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#ifndef __REGINA_LAURENT2_H
#ifndef __DOXYGEN
#define __REGINA_LAURENT2_H
#endif

/*! \file maths/laurent2.h
 *  \brief Implements Laurent polynomials in two variables over arbitrary rings.
 */

#include "utilities/stringutils.h"
#include "utilities/tightencoding.h"
#include "core/output.h"
#include <iostream>
#include <map>

namespace regina {

/**
 * Represents a Laurent polynomial in the two variables \e x, \e y with
 * coefficients of type \a T.  A Laurent polynomial differs from an ordinary
 * polynomial in that it allows negative exponents (so, for example, you can
 * represent a polynomial such as `2 + 3x² + y/x - 1/y³`).
 *
 * The type \a T must represent a ring with no zero divisors.
 * In particular, it must:
 *
 * - support basic arithmetic operations;
 * - support assignments of the form `x = int` and
 *   tests of the form `x == int` and `x < int`;
 * - have a default constructor that assigns an explicit value of zero.
 *
 * This means that Regina's numerical types such as Integer and Rational
 * are supported, but native data types such as int and long are not
 * (since they have no zero-initialising default constructor).
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * The underlying storage method for this class is sparse: only the
 * non-zero coefficients are stored.
 *
 * See also the class Laurent, which describes Laurent polynomials in
 * just one variable.
 *
 * \python In Python, the class Laurent2 refers to the specific
 * template class Laurent2<Integer>.
 *
 * \ingroup maths
 */
template <typename T>
class Laurent2 :
        public ShortOutput<Laurent2<T>, true>,
        public TightEncodable<Laurent2<T>> {
    static_assert(! std::is_integral_v<T>,
        "Laurent2<T> requires the type T to have a default constructor that "
        "assigns a value of zero.");

    public:
        using Coefficient = T;
            /**< The type of each coefficient of the polynomial. */

        // Make sure the compiler can see the zero-argument string output
        // routines, since we declare alternative versions of these below.
        using ShortOutput<Laurent2<T>, true>::str;
        using ShortOutput<Laurent2<T>, true>::utf8;

    private:
        using Exponents = std::pair<long, long>;

        std::map<Exponents, T> coeff_;
            /**< Stores all non-zero coefficients of the polynomial.
                 Specifically, coeff_[(i,j)] stores the coefficient of
                 `x^i y^j`. */

        static const T zero_;
            /**< A zero coefficient that we can safely make references to. */

    public:
        /**
         * Creates the zero polynomial.
         */
        Laurent2() = default;

        /**
         * Deprecated constructor that creates the polynomial `x^d y^e` for
         * the given exponents \a d and \a e.
         *
         * \deprecated This will be removed in a future version of Regina
         * for consistency with the single-variable polynomial classes
         * Laurent and Polynomial, since for those classes it is too easy for
         * a casual reader to misread what such an "exponent-based constructor"
         * actually does.  You can still create `x^d y^e` by calling
         * `initExp(d, e)` instead.
         *
         * \param xExp the exponent \a d, which is attached to \a x.
         * \param yExp the exponent \a e, which is attached to \a y.
         */
        [[deprecated]] explicit Laurent2(long xExp, long yExp);

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
         * \param value the polynomial to clone.
         */
        Laurent2(const Laurent2<T>& value);

        /**
         * Moves the contents of the given polynomial to this new polynomial.
         * This is a fast (constant time) operation.
         *
         * The polynomial that was passed (\a value) will no longer be usable.
         *
         * \param value the polynomial to move.
         */
        Laurent2(Laurent2<T>&& value) noexcept = default;

        /**
         * Creates a copy of the given polynomial with all terms
         * multiplied by `x^d y^e` for some integers \a d and \a e.
         *
         * This constructor induces a deep (and modified) copy of \a value.
         *
         * \param toShift the polynomial to clone and shift.
         * \param xShift the integer \a d, which will be added to all
         * exponents for \a x.
         * \param yShift the integer \a e, which will be added to all
         * exponents for \a y.
         */
        Laurent2(const Laurent2<T>& toShift, long xShift, long yShift);

        /**
         * Creates a new copy of the given polynomial.
         *
         * This constructor induces a deep copy of \a value.
         *
         * \pre Objects of type \a T can be assigned values of type \a U.
         *
         * \nopython Python only supports Laurent polynomials with one type of
         * coefficient (the case where \a T is Integer).  Therefore
         * Python users can use the non-templated copy constructor.
         *
         * \param value the polynomial to clone.
         */
        template <typename U>
        Laurent2(const Laurent2<U>& value);

        /**
         * Creates a new polynomial from the given collection of coefficients.
         *
         * The coefficients should be presented as a collection of tuples of
         * the form (\a d, \a e, \a v), each representing a term of the form
         * `v x^d y^e`.
         *
         * The tuples may be given in any order.
         * An empty sequence will be treated as the zero polynomial.
         *
         * Unlike the std::initializer_list constructor, zero coefficients are
         * allowed (these will be silently ignored), and multiple coefficients
         * with the same exponents are also allowed (these will be aggregated
         * using the += operator).
         *
         * \python Instead of the iterators \a begin and \a end,
         * this routine takes a python list of tuples.
         *
         * \tparam iterator an iterator type which, when dereferenced, gives a
         * std::tuple of the form (\a d, \a e, \a v), where \a d and \a e can
         * be assigned to long integers, and where \a v can be assigned to
         * type \a T.
         *
         * \tparam deref a dummy argument that should be ignored.  This is
         * present to ensure that \a iterator can be dereferenced, so that
         * a call such as Laurent2(int, int) falls through to the (long, long)
         * constructor, and not this iterator-based constructor instead.
         * Once we support a greater subset of C++20, this will be enforced
         * through concepts instead.
         *
         * \param begin the beginning of the set of coefficients, as outlined
         * above.
         * \param end a past-the-end iterator indicating the end of the set of
         * coefficients.
         */
        template <typename iterator, typename deref = decltype(*iterator())>
        Laurent2(iterator begin, iterator end);

        /**
         * Creates a new polynomial from a hard-coded collection of
         * non-zero coefficients.
         *
         * The coefficients should be presented as a collection of tuples of
         * the form (\a d, \a e, \a v) each representing a term of the form
         * `v x^d y^e`.
         *
         * The tuples may be given in any order.
         * An empty sequence will be treated as the zero polynomial.
         *
         * In practice, this means you can create a hard-coded
         * polynomial using syntax such as:
         *
         * \code
         * Laurent2<Integer> p = { { 0, 0, 3 }, { 1, -1, 2 } };
         * \endcode
         *
         * \pre Each tuple has a non-zero value \a v, and no two tuples
         * share the same pair of exponents (\a d, \a e).
         *
         * \nopython Instead, use the Python constructor that takes a list
         * of coefficients (which need not be constant).
         *
         * \exception InvalidArgument Two of the given tuples share the same
         * pair of exponents (\a d, \a e), and/or one of the given tuples has
         * its value \a v equal to zero.
         *
         * \param coefficients the set of all non-zero coefficients, as
         * outlined above.
         */
        Laurent2(std::initializer_list<std::tuple<long, long, T>> coefficients);

        /**
         * Sets this to become the zero polynomial.
         */
        void init();

        /**
         * Sets this to become the polynomial `x^d y^e` for the
         * given exponents \a d and \a e.
         *
         * \param xExp the new exponent \a d, which is attached to \a x.
         * \param yExp the new exponent \a e, which is attached to \a y.
         */
        void initExp(long xExp, long yExp);

        /**
         * Deprecated function that sets this to become the polynomial `x^d y^e`
         * for the given exponents \a d and \a e.
         *
         * \deprecated This has been renamed to initExp() for consistency with
         * the single-variable polynomial classes Laurent and Polynomial,
         * since for those classes it is too easy for a casual reader to
         * misread what such an "exponent-based initialisation" actually does.
         *
         * \param xExp the new exponent \a d, which is attached to \a x.
         * \param yExp the new exponent \a e, which is attached to \a y.
         */
        [[deprecated]] void init(long xExp, long yExp);

        /**
         * Returns whether this is the zero polynomial.
         *
         * \return \c true if and only if this is the zero polynomial.
         */
        bool isZero() const;

        /**
         * Returns the given coefficient of this polynomial.
         * There are no restrictions on the exponents \a xExp and \a yExp.
         *
         * \python In Python, this is the square bracket operator,
         * not the round bracket operator; that is, Python users can access
         * coefficients through the syntax `poly[xExp, yExp]`.
         * Moreover, this operator can also _set_ cofficients; that is,
         * you can write `poly[xExp, yExp] = value`.
         * However, when _getting_ a coefficient this operator will return
         * by value (to enforce constness), which means for example you
         * cannot write something like `poly[xExp, yExp].negate()`.
         *
         * \cpp For C++ users, this operator is read-only.  To _set_
         * coefficients, you must use the separate routine set().
         *
         * \param xExp the exponent attached to \a x.
         * \param yExp the exponent attached to \a y.
         * \return the coefficient of the term with the given exponents.
         */
        const T& operator () (long xExp, long yExp) const;

        /**
         * Changes the given coefficient of this polynomial.
         *
         * There are no restrictions on the exponents \a xExp and \a yExp,
         * and the new coefficient \a value may be zero.
         *
         * Moreover, the underlying data structures ensure that this
         * operation is cheap regardless of the exponents involved.
         *
         * \python This set() routine is available, but you can
         * also set coefficients directly using syntax of the form
         * `p[xExp, yExp] = value`.
         *
         * \param xExp the exponent attached to \a x.
         * \param yExp the exponent attached to \a y.
         * \param value the new value of the corresponding coefficient.
         */
        void set(long xExp, long yExp, const T& value);

        /**
         * Tests whether this and the given polynomial are equal.
         *
         * \param rhs the polynomial to compare with this.
         * \return \c true if and only if this and the given polynomial
         * are equal.
         */
        bool operator == (const Laurent2<T>& rhs) const;

        /**
         * Compares this against the given polynomial under a total
         * ordering of all two-variable Laurent polynomials.
         *
         * The particular total order that Regina uses is not important,
         * and may change between Regina releases (though such changes
         * should be very infrequent).  The main purpose of this routine
         * is to support algorithms that require a "canonical" choice of
         * polynomial from amongst many alternatives.
         *
         * This routine generates all of the usual comparison operators,
         * including `<`, `<=`, `>`, and `>=`.
         *
         * \python This spaceship operator `x <=> y` is not available, but the
         * other comparison operators that it generates _are_ available.
         *
         * \param rhs the polynomial to compare with this.
         * \return The result of the comparison between this
         * and the given polynomial.
         */
        std::strong_ordering operator <=> (const Laurent2<T>& rhs) const;

        /**
         * Sets this to be a copy of the given polynomial.
         *
         * This and the given polynomial need not have the same range of
         * non-zero coefficients.
         *
         * This operator induces a deep copy of \a value.
         *
         * A note to developers: although this is identical to the templated
         * assignment operator, it must be declared and implemented separately.
         * See the copy constructor for further details.
         *
         * \param value the polynomial to copy.
         * \return a reference to this polynomial.
         */
        Laurent2& operator = (const Laurent2<T>& value);

        /**
         * Sets this to be a copy of the given polynomial.
         *
         * This and the given polynomial need not have the same range of
         * non-zero coefficients.
         *
         * This operator induces a deep copy of \a value.
         *
         * \param value the polynomial to copy.
         * \return a reference to this polynomial.
         */
        template <typename U>
        Laurent2& operator = (const Laurent2<U>& value);

        /**
         * Moves the contents of the given polynomial to this polynomial.
         * This is a fast (constant time) operation.
         *
         * This and the given polynomial need not have the same range of
         * non-zero coefficients.
         *
         * The polynomial that was passed (\a value) will no longer be usable.
         *
         * \param value the polynomial to move.
         * \return a reference to this polynomial.
         */
        Laurent2& operator = (Laurent2<T>&& value) noexcept = default;

        /**
         * Swaps the contents of this and the given polynomial.
         * This is a fast (constant time) operation.
         *
         * This and the given polynomial need not have the same range of
         * non-zero coefficients.
         *
         * \param other the polynomial whose contents should be swapped
         * with this.
         */
        void swap(Laurent2<T>& other) noexcept;

        /**
         * Negates this polynomial.
         * This polynomial is changed directly.
         */
        void negate();

        /**
         * Replaces `x` with `x^-1` in this polynomial.
         * This polynomial is changed directly.
         */
        void invertX();

        /**
         * Replaces `y` with `y^-1` in this polynomial.
         * This polynomial is changed directly.
         */
        void invertY();

        /**
         * Multiplies this polynomial by the given constant.
         *
         * \param scalar the scalar factor to multiply by.
         * \return a reference to this polynomial.
         */
        Laurent2& operator *= (const T& scalar);

        /**
         * Divides this polynomial by the given constant.
         *
         * This uses the division operator /= for the coefficient type \a T.
         *
         * \pre The argument \a scalar is non-zero.
         *
         * \param scalar the scalar factor to divide by.
         * \return a reference to this polynomial.
         */
        Laurent2& operator /= (const T& scalar);

        /**
         * Adds the given polynomial to this.
         *
         * This and the given polynomial need not have the same range of
         * non-zero coefficients.
         *
         * \param other the polynomial to add to this.
         * \return a reference to this polynomial.
         */
        Laurent2& operator += (const Laurent2<T>& other);

        /**
         * Subtracts the given polynomial from this.
         *
         * This and the given polynomial need not have the same range of
         * non-zero coefficients.
         *
         * \param other the polynomial to subtract from this.
         * \return a reference to this polynomial.
         */
        Laurent2& operator -= (const Laurent2<T>& other);

        /**
         * Multiplies this by the given polynomial.
         *
         * This and the given polynomial need not have the same range of
         * non-zero coefficients.
         *
         * \param other the polynomial to multiply this by.
         * \return a reference to this polynomial.
         */
        Laurent2& operator *= (const Laurent2<T>& other);

        /**
         * Writes this polynomial to the given output stream, using the
         * given variable names instead of \c x and \c y.
         *
         * If \a utf8 is passed as \c true then unicode superscript characters
         * will be used for exponents and the minus sign; these will be encoded
         * using UTF-8.  This will make the output nicer, but will require more
         * complex fonts to be available on the user's machine.
         *
         * \nopython Use str() or utf8() instead.
         *
         * \param out the output stream to which to write.
         * \param utf8 \c true if unicode characters may be used.
         * \param varX the symbol to use for the variable \a x.  This may be
         * \c null, in which case the default symbol `x` will be used.
         * \param varY the symbol to use for the variable \a y.  This may be
         * \c null, in which case the default symbol `y` will be used.
         */
        void writeTextShort(std::ostream& out, bool utf8 = false,
            const char* varX = nullptr, const char* varY = nullptr) const;

        /**
         * Returns this polynomial as a human-readable string, using the
         * given variable names instead of \c x and \c y.
         *
         * \note There is also the usual variant of str() which takes no
         * arguments; that variant is inherited from the Output class.
         *
         * \param varX the symbol to use for the variable \a x.  This may be
         * \c null, in which case the default symbol `x` will be used.
         * \param varY the symbol to use for the variable \a y.  This may be
         * \c null, in which case the default symbol `y` will be used.
         * \return this polynomial as a human-readable string.
         */
        std::string str(const char* varX, const char* varY = nullptr) const;

        /**
         * Returns this polynomial as a human-readable string using unicode
         * characters, using the given variable names instead of \c x and \c y.
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
         * \param varX the symbol to use for the variable \a x.  This may be
         * \c null, in which case the default symbol `x` will be used.
         * \param varY the symbol to use for the variable \a y.  This may be
         * \c null, in which case the default symbol `y` will be used.
         * \return this polynomial as a unicode-enabled human-readable string.
         */
        std::string utf8(const char* varX, const char* varY = nullptr) const;

        /**
         * Writes the tight encoding of this polynomial to the given output
         * stream.  See the page on \ref tight "tight encodings" for details.
         *
         * \pre The coefficient type \a T must have a corresponding
         * tightEncode() function.  This is true for Regina's arbitrary
         * precision integer types (Integer and LargeInteger).
         *
         * \nopython Use tightEncoding() instead, which returns a string.
         *
         * \param out the output stream to which the encoded string will
         * be written.
         */
        void tightEncode(std::ostream& out) const;

        /**
         * Reconstructs a polynomial from its given tight encoding.
         * See the page on \ref tight "tight encodings" for details.
         *
         * The tight encoding will be read from the given input stream.
         * If the input stream contains leading whitespace then it will be
         * treated as an invalid encoding (i.e., this routine will throw an
         * exception).  The input stream _may_ contain further data: if this
         * routine is successful then the input stream will be left positioned
         * immediately after the encoding, without skipping any trailing
         * whitespace.
         *
         * \pre The coefficient type \a T must have a corresponding static
         * tightDecode() function.  This is true for Regina's arbitrary
         * precision integer types (Integer and LargeInteger).
         *
         * \exception InvalidInput The given input stream does not begin with
         * a tight encoding of a two-variable Laurent polynomial.
         *
         * \nopython Use tightDecoding() instead, which takes a string as
         * its argument.
         *
         * \param input an input stream that begins with the tight encoding
         * for a two-variable Laurent polynomial.
         * \return the polynomial represented by the given tight encoding.
         */
        static Laurent2 tightDecode(std::istream& input);

    private:
        /**
         * Removes all entries from coeff_ whose coefficients are zero.
         */
        void removeZeroes();

    template <typename U>
    friend Laurent2<U> operator * (const Laurent2<U>&, const Laurent2<U>&);

    // For the time being, allow Link::homflyAZtoLM() to do low-level
    // operations on these polynomials.
    friend class Link;
};

#ifndef __DOXYGEN
// Don't confuse doxygen with specialisations.
template <typename T>
struct RingTraits<Laurent2<T>> {
    inline static const Laurent2<T> zero;
    inline static const Laurent2<T> one { { 0, 0, 1 } };
};
#endif // __DOXYGEN

/**
 * Swaps the contents of the given polynomials.
 *
 * This global routine simply calls Laurent2<T>::swap(); it is provided
 * so that Laurent2<T> meets the C++ Swappable requirements.
 *
 * \param a the first polynomial whose contents should be swapped.
 * \param b the second polynomial whose contents should be swapped.
 *
 * \ingroup maths
 */
template <typename T>
void swap(Laurent2<T>& a, Laurent2<T>& b) noexcept;

/**
 * Multiplies the given polynomial by the given scalar constant.
 *
 * The scalar is simply of type \a T; we use the identical type
 * Laurent2<T>::Coefficient here to assist with C++ template type matching.
 *
 * \param poly the polynomial to multiply by.
 * \param scalar the scalar to multiply by.
 * \return the product of the given polynomial and scalar.
 *
 * \ingroup maths
 */
template <typename T>
Laurent2<T> operator * (Laurent2<T> poly,
    const typename Laurent2<T>::Coefficient& scalar);

/**
 * Multiplies the given polynomial by the given scalar constant.
 *
 * The scalar is simply of type \a T; we use the identical type
 * Laurent2<T>::Coefficient here to assist with C++ template type matching.
 *
 * \param scalar the scalar to multiply by.
 * \param poly the polynomial to multiply by.
 * \return the product of the given polynomial and scalar.
 *
 * \ingroup maths
 */
template <typename T>
Laurent2<T> operator * (const typename Laurent2<T>::Coefficient& scalar,
    Laurent2<T> poly);

/**
 * Divides the given polynomial by the given scalar constant.
 *
 * This uses the division operator /= for the coefficient type \a T.
 *
 * The scalar is simply of type \a T; we use the identical type
 * Laurent2<T>::Coefficient here to assist with C++ template type matching.
 *
 * \pre The argument \a scalar is non-zero.
 *
 * \param poly the polynomial to divide by the given scalar.
 * \param scalar the scalar factor to divide by.
 * \return the quotient of the given polynomial by the given scalar.
 *
 * \ingroup maths
 */
template <typename T>
Laurent2<T> operator / (Laurent2<T> poly,
    const typename Laurent2<T>::Coefficient& scalar);

/**
 * Adds the two given polynomials.
 *
 * The two polynomials need not have the same range of non-zero coefficients.
 *
 * \param lhs the first polynomial to add.
 * \param rhs the second polynomial to add.
 * \return the sum of both polynomials.
 *
 * \ingroup maths
 */
template <typename T>
Laurent2<T> operator + (const Laurent2<T>& lhs, const Laurent2<T>& rhs);

/**
 * Adds the two given polynomials.
 *
 * The two polynomials need not have the same range of non-zero coefficients.
 *
 * \param lhs the first polynomial to add.
 * \param rhs the second polynomial to add.
 * \return the sum of both polynomials.
 *
 * \ingroup maths
 */
template <typename T>
Laurent2<T> operator + (Laurent2<T>&& lhs, const Laurent2<T>& rhs);

/**
 * Adds the two given polynomials.
 *
 * The two polynomials need not have the same range of non-zero coefficients.
 *
 * \param lhs the first polynomial to add.
 * \param rhs the second polynomial to add.
 * \return the sum of both polynomials.
 *
 * \ingroup maths
 */
template <typename T>
Laurent2<T> operator + (const Laurent2<T>& lhs, Laurent2<T>&& rhs);

/**
 * Adds the two given polynomials.
 *
 * The two polynomials need not have the same range of non-zero coefficients.
 *
 * \param lhs the first polynomial to add.
 * \param rhs the second polynomial to add.
 * \return the sum of both polynomials.
 *
 * \ingroup maths
 */
template <typename T>
Laurent2<T> operator + (Laurent2<T>&& lhs, Laurent2<T>&& rhs);

/**
 * Returns the negative of the given polynomial.
 *
 * \param arg the polynomial to negate.
 * \return the negative of \a arg.
 *
 * \ingroup maths
 */
template <typename T>
Laurent2<T> operator - (Laurent2<T> arg);

/**
 * Subtracts the two given polynomials.
 *
 * The two polynomials need not have the same range of non-zero coefficients.
 *
 * \param lhs the polynomial to subtract from.
 * \param rhs the polynomial to subtract.
 * \return the first polynomial minus the second.
 *
 * \ingroup maths
 */
template <typename T>
Laurent2<T> operator - (const Laurent2<T>& lhs, const Laurent2<T>& rhs);

/**
 * Subtracts the two given polynomials.
 *
 * The two polynomials need not have the same range of non-zero coefficients.
 *
 * \param lhs the polynomial to subtract from.
 * \param rhs the polynomial to subtract.
 * \return the first polynomial minus the second.
 *
 * \ingroup maths
 */
template <typename T>
Laurent2<T> operator - (Laurent2<T>&& lhs, const Laurent2<T>& rhs);

/**
 * Subtracts the two given polynomials.
 *
 * The two polynomials need not have the same range of non-zero coefficients.
 *
 * \param lhs the polynomial to subtract from.
 * \param rhs the polynomial to subtract.
 * \return the first polynomial minus the second.
 *
 * \ingroup maths
 */
template <typename T>
Laurent2<T> operator - (const Laurent2<T>& lhs, Laurent2<T>&& rhs);

/**
 * Subtracts the two given polynomials.
 *
 * The two polynomials need not have the same range of non-zero coefficients.
 *
 * \param lhs the polynomial to subtract from.
 * \param rhs the polynomial to subtract.
 * \return the first polynomial minus the second.
 *
 * \ingroup maths
 */
template <typename T>
Laurent2<T> operator - (Laurent2<T>&& lhs, Laurent2<T>&& rhs);

/**
 * Multiplies the two given polynomials.
 *
 * The two polynomials need not have the same range of non-zero coefficients.
 *
 * \param lhs the first polynomial to multiply.
 * \param rhs the second polynomial to multiply.
 * \return the product of both polynomials.
 *
 * \ingroup maths
 */
template <typename T>
Laurent2<T> operator * (const Laurent2<T>& lhs, const Laurent2<T>& rhs);

template <typename T>
const T Laurent2<T>::zero_(0);

template <typename T>
inline Laurent2<T>::Laurent2(long xExp, long yExp) {
    coeff_.emplace(Exponents(xExp, yExp), 1);
}

template <typename T>
inline Laurent2<T>::Laurent2(const Laurent2<T>& value) :
        coeff_(value.coeff_) {
    // TODO: Use default implementation.
    // std::cerr << "Laurent2: deep copy (init)" << std::endl;
}

template <typename T>
Laurent2<T>::Laurent2(const Laurent2<T>& toShift, long xShift, long yShift) {
    for (const auto& entry : toShift.coeff_)
        coeff_.emplace_hint(coeff_.end(),
            std::make_pair(entry.first.first + xShift,
                entry.first.second + yShift), entry.second);
}

template <typename T>
template <typename U>
inline Laurent2<T>::Laurent2(const Laurent2<U>& value) :
        coeff_(value.coeff_) {
    // std::cerr << "Laurent2: deep copy (init)" << std::endl;
}

template <typename T>
template <typename iterator, typename deref>
inline Laurent2<T>::Laurent2(iterator begin, iterator end) {
    for (auto it = begin; it != end; ++it) {
        if (std::get<2>(*it) == 0)
            continue;

        auto res = coeff_.emplace(Exponents(std::get<0>(*it), std::get<1>(*it)),
            std::get<2>(*it));
        if (! res.second) {
            // This pair of exponents is already present.
            // Accumulate, and erase if the resulting coefficient is zero.
            if ((res.first->second += std::get<2>(*it)) == 0)
                coeff_.erase(res.first);
        }
    }
}

template <typename T>
inline Laurent2<T>::Laurent2(
        std::initializer_list<std::tuple<long, long, T>> coefficients) {
    for (const auto& c : coefficients) {
        if (std::get<2>(c) == 0)
            throw InvalidArgument("One of the given tuples has a value of "
                "zero");
        if (! coeff_.emplace(Exponents(std::get<0>(c), std::get<1>(c)),
                std::get<2>(c)).second)
            throw InvalidArgument("Two of the given tuples share the "
                "same pair of exponents");
    }
}

template <typename T>
inline void Laurent2<T>::init() {
    coeff_.clear();
}

template <typename T>
inline void Laurent2<T>::initExp(long xExp, long yExp) {
    coeff_.clear();
    coeff_.emplace(Exponents(xExp, yExp), 1);
}

template <typename T>
inline void Laurent2<T>::init(long xExp, long yExp) {
    initExp(xExp, yExp);
}

template <typename T>
inline bool Laurent2<T>::isZero() const {
    return coeff_.empty();
}

template <typename T>
inline const T& Laurent2<T>::operator () (long xExp, long yExp) const {
    auto it = coeff_.find(Exponents(xExp, yExp));
    if (it == coeff_.end())
        return zero_;
    else
        return it->second;
}

template <typename T>
void Laurent2<T>::set(long xExp, long yExp, const T& value) {
    if (value == 0) {
        coeff_.erase(Exponents(xExp, yExp));
    } else {
        auto result = coeff_.emplace(Exponents(xExp, yExp), value);
        if (! result.second) {
            // A coefficient was already present.  Change it.
            result.first->second = value;
        }
    }
}

template <typename T>
inline bool Laurent2<T>::operator == (const Laurent2<T>& rhs) const {
    return coeff_ == rhs.coeff_;
}

template <typename T>
inline std::strong_ordering Laurent2<T>::operator <=> (const Laurent2<T>& rhs)
        const {
    return coeff_ <=> rhs.coeff_;
}

template <typename T>
inline Laurent2<T>& Laurent2<T>::operator = (const Laurent2<T>& other) {
    // TODO: Use default implementation.
    // std::cerr << "Laurent2: deep copy (=)" << std::endl;
    coeff_ = other.coeff_;
    return *this;
}

#ifndef __DOXYGEN
// Doxygen does not match this to the documented declaration.  I think the
// issue is that the return type "looks" different due to the explicit <T>.
template <typename T>
template <typename U>
inline Laurent2<T>& Laurent2<T>::operator = (const Laurent2<U>& other) {
    // std::cerr << "Laurent2: deep copy (=)" << std::endl;
    coeff_ = other.coeff_;
    return *this;
}
#endif // __DOXYGEN

template <typename T>
inline void Laurent2<T>::swap(Laurent2<T>& other) noexcept {
    std::swap(coeff_, other.coeff_);
}

template <typename T>
inline void Laurent2<T>::negate() {
    for (auto& c : coeff_)
        c.second = -c.second;
}

template <typename T>
inline void Laurent2<T>::invertX() {
    std::map<Exponents, T> newCoeff;
    for (const auto& c : coeff_)
        newCoeff.insert(std::make_pair(
            std::make_pair(- c.first.first, c.first.second), c.second));
    coeff_ = std::move(newCoeff);
}

template <typename T>
inline void Laurent2<T>::invertY() {
    std::map<Exponents, T> newCoeff;
    for (const auto& c : coeff_)
        newCoeff.insert(std::make_pair(
            std::make_pair(c.first.first, - c.first.second), c.second));
    coeff_ = std::move(newCoeff);
}

template <typename T>
inline Laurent2<T>& Laurent2<T>::operator *= (const T& scalar) {
    if (scalar == 0) {
        // All coefficients become zero.
        coeff_.clear();
    } else {
        // No coefficients become zero that were not zero already.
        for (auto& c : coeff_)
            c.second *= scalar;
    }
    return *this;
}

template <typename T>
inline Laurent2<T>& Laurent2<T>::operator /= (const T& scalar) {
    for (auto& c : coeff_)
        c.second /= scalar;

    // For integer division, we could have zeroed out some coefficients.
    removeZeroes();
    return *this;
}

template <typename T>
Laurent2<T>& Laurent2<T>::operator += (const Laurent2<T>& other) {
    // This works even if &other == this, since in this case there are
    // no insertions or deletions.
    for (const auto& entry : other.coeff_) {
        auto result = coeff_.emplace(entry);
        if (! result.second)
            result.first->second += entry.second;
    }

    // We might have zeroed out some coefficients.
    removeZeroes();
    return *this;
}

template <typename T>
Laurent2<T>& Laurent2<T>::operator -= (const Laurent2<T>& other) {
    // This works even if &other == this, since in this case there are
    // no insertions or deletions.
    for (auto entry : other.coeff_) {
        entry.second = - entry.second;
        auto result = coeff_.emplace(entry);
        if (! result.second)
            result.first->second += entry.second;
    }

    // We might have zeroed out some coefficients.
    removeZeroes();
    return *this;
}

template <typename T>
Laurent2<T>& Laurent2<T>::operator *= (const Laurent2<T>& other) {
    if (isZero())
        return *this;
    if (other.isZero()) {
        init();
        return *this;
    }

    // The following code works even if &other == this, since we construct the
    // coefficients of the product in a separate section of memory.
    std::map<Exponents, T> ans;

    for (const auto& left : coeff_)
        for (const auto& right : other.coeff_) {
            Exponents e(left.first.first + right.first.first,
                left.first.second + right.first.second);
            T term = left.second * right.second;
            auto result = ans.emplace(e, term);
            if (! result.second)
                result.first->second += term;
        }

    coeff_.clear();
    std::swap(ans, coeff_);

    // We might have zeroed out some coefficients.
    removeZeroes();
    return *this;
}

template <typename T>
void Laurent2<T>::writeTextShort(std::ostream& out, bool utf8,
        const char* varX, const char* varY) const {
    if (isZero()) {
        out << '0';
        return;
    }

    for (auto it = coeff_.rbegin(); it != coeff_.rend(); ++it) {
        T writeCoeff = it->second;

        if (it == coeff_.rbegin()) {
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
        if (it->first.first == 0 && it->first.second == 0) {
            out << writeCoeff;
            continue;
        }
        if (writeCoeff != 1)
            out << writeCoeff << ' ';
        if (it->first.first != 0) {
            if (varX)
                out << varX;
            else
                out << 'x';
            if (it->first.first != 1) {
                if (utf8)
                    out << regina::superscript(it->first.first);
                else
                    out << '^' << it->first.first;
            }
            if (it->first.second != 0)
                out << ' ';
        }
        if (it->first.second != 0) {
            if (varY)
                out << varY;
            else
                out << 'y';
            if (it->first.second != 1) {
                if (utf8)
                    out << regina::superscript(it->first.second);
                else
                    out << '^' << it->first.second;
            }
        }
    }
}

template <typename T>
inline std::string Laurent2<T>::str(const char* varX, const char* varY)
        const {
    // Make sure that python will be able to find the inherited str().
    static_assert(std::is_same_v<typename OutputBase<Laurent2<T>>::type,
        Output<Laurent2<T>, true>>,
        "Laurent2<T> is not identified as being inherited from Output<...>");

    std::ostringstream out;
    writeTextShort(out, false, varX, varY);
    return out.str();
}

template <typename T>
inline std::string Laurent2<T>::utf8(const char* varX, const char* varY)
        const {
    std::ostringstream out;
    writeTextShort(out, true, varX, varY);
    return out.str();
}

template <typename T>
void Laurent2<T>::removeZeroes() {
    auto it = coeff_.begin();
    while (it != coeff_.end())
        if (it->second == 0)
            it = coeff_.erase(it); // C++11: returns next element.
        else
            ++it;
}

template <typename T>
inline void swap(Laurent2<T>& a, Laurent2<T>& b) noexcept {
    a.swap(b);
}

template <typename T>
inline Laurent2<T> operator * (Laurent2<T> poly,
        const typename Laurent2<T>::Coefficient& scalar) {
    // When the argument poly is an lvalue reference, we perform a deep copy
    // due to pass-by-value.  If scalar == 0 then we don't need this deep copy,
    // since the argument can be ignored.  This special-case optimisation
    // would require two different lvalue/rvalue implementations of *, and
    // so we leave it for now.
    poly *= scalar;
    return poly;
}

template <typename T>
inline Laurent2<T> operator * (const typename Laurent2<T>::Coefficient& scalar,
        Laurent2<T> poly) {
    // See the notes above on a possible optimisation for scalar == 0.
    poly *= scalar;
    return poly;
}

template <typename T>
inline Laurent2<T> operator / (Laurent2<T> poly,
        const typename Laurent2<T>::Coefficient& scalar) {
    poly /= scalar;
    return poly;
}

template <typename T>
inline Laurent2<T> operator + (const Laurent2<T>& lhs, const Laurent2<T>& rhs) {
    // We have to make a deep copy since both arguments are read-only.
    return std::move(Laurent2<T>(lhs) += rhs);
}

template <typename T>
inline Laurent2<T> operator + (Laurent2<T>&& lhs, const Laurent2<T>& rhs) {
    return std::move(lhs += rhs);
}

template <typename T>
inline Laurent2<T> operator + (const Laurent2<T>& lhs, Laurent2<T>&& rhs) {
    return std::move(rhs += lhs);
}

template <typename T>
inline Laurent2<T> operator + (Laurent2<T>&& lhs, Laurent2<T>&& rhs) {
    return std::move(lhs += rhs);
}

template <typename T>
inline Laurent2<T> operator - (Laurent2<T> arg) {
    arg.negate();
    return arg;
}

template <typename T>
inline Laurent2<T> operator - (const Laurent2<T>& lhs, const Laurent2<T>& rhs) {
    // We have to make a deep copy since both arguments are read-only.
    return std::move(Laurent2<T>(lhs) -= rhs);
}

template <typename T>
inline Laurent2<T> operator - (Laurent2<T>&& lhs, const Laurent2<T>& rhs) {
    return std::move(lhs -= rhs);
}

template <typename T>
inline Laurent2<T> operator - (const Laurent2<T>& lhs, Laurent2<T>&& rhs) {
    rhs.negate();
    return std::move(rhs += lhs);
}

template <typename T>
inline Laurent2<T> operator - (Laurent2<T>&& lhs, Laurent2<T>&& rhs) {
    return std::move(lhs -= rhs);
}

template <typename T>
inline Laurent2<T> operator * (const Laurent2<T>& lhs, const Laurent2<T>& rhs) {
    if (lhs.isZero() || rhs.isZero())
        return Laurent2<T>(); // zero

    Laurent2<T> ans;

    for (const auto& left : lhs.coeff_)
        for (const auto& right : rhs.coeff_) {
            typename Laurent2<T>::Exponents e(
                left.first.first + right.first.first,
                left.first.second + right.first.second);
            T term = left.second * right.second;
            auto result = ans.coeff_.emplace(e, term);
            if (! result.second)
                result.first->second += term;
        }

    // We might have zeroed out some coefficients.
    ans.removeZeroes();
    return ans;
}

template <typename T>
inline void Laurent2<T>::tightEncode(std::ostream& out) const {
    for (const auto& c : coeff_) {
        // Write the coefficient (which must be non-zero) before the exponents.
        // This way we can use tightEncode(0) as an unambiguous terminator.
        c.second.tightEncode(out);
        regina::tightEncode(out, c.first.first);
        regina::tightEncode(out, c.first.second);
    }
    T().tightEncode(out); // The zero terminator
}

template <typename T>
inline Laurent2<T> Laurent2<T>::tightDecode(std::istream& input) {
    Laurent2 ans;

    while (true) {
        T coeff = T::tightDecode(input);
        if (coeff == 0)
            return ans;

        long x = regina::tightDecode<long>(input);
        long y = regina::tightDecode<long>(input);
        if (! ans.coeff_.emplace(Exponents(x, y), std::move(coeff)).second)
            throw InvalidInput("The tight encoding has a repeated "
                "pair of exponents");
    }
}

} // namespace regina

#endif
