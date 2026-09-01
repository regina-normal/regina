
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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
#include "concepts/io.h"
#include "core/output.h"
#include <iostream>
#include <iterator>
#include <map>

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

template <CoefficientDomain T> class Laurent;

/**
 * Represents a Laurent polynomial in the two variables \e x, \e y with
 * coefficients of type \a T.  A Laurent polynomial differs from an ordinary
 * polynomial in that it allows negative exponents (so, for example, you can
 * represent a polynomial such as `2 + 3x² + y/x - 1/y³`).
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
 * template class `Laurent2<Integer>`.
 *
 * \tparam T the coefficient type.  A typical coefficient type would be
 * Integer or Rational.  Note that native C++ integer types are _not_
 * supported (since they have no zero-initialising default constructor).
 *
 * \ingroup maths
 */
template <CoefficientDomain T>
class Laurent2 :
        public ShortOutput<Laurent2<T>, true>,
        public TightEncodable<Laurent2<T>> {
    private:
        /**
         * A pair of exponents that appear in a single term of a two-variable
         * Laurent polynomial.  The first and second elements of the pair are
         * the exponents of \a x and \a y respectively.
         */
        using Exponents = std::pair<long, long>;

    public:
        /**
         * The type of each coefficient of the polynomial.
         */
        using Coefficient = T;

        /**
         * A read-only iterator type for iterating over non-zero polynomial
         * coefficients and their corresponding exponent pairs.  This is
         * identical to \a const_iterator, since iterators over coefficients
         * are always read-only.
         */
        using iterator = std::map<Exponents, T>::const_iterator;

        /**
         * A read-only iterator type for iterating over non-zero polynomial
         * coefficients and their corresponding exponent pairs.
         */
        using const_iterator = std::map<Exponents, T>::const_iterator;

        // Make sure the compiler can see the zero-argument string output
        // routines, since we declare alternative versions of these below.
        using ShortOutput<Laurent2<T>, true>::str;
        using ShortOutput<Laurent2<T>, true>::utf8;

    private:
        std::map<Exponents, T> coeff_;
            /**< Stores all non-zero coefficients of the polynomial.
                 Specifically, `coeff_[(i,j)]` stores the coefficient of
                 `x^i y^j`. */

        static const T zero_;
            /**< A zero coefficient that we can safely make references to. */

    public:
        /**
         * Creates the zero polynomial.
         */
        Laurent2() = default;

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
        Laurent2(const Laurent2<T>& value) = default;

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
        Laurent2(const Laurent2<T>& toShift, long xShift, long yShift) {
            for (const auto& entry : toShift.coeff_)
                coeff_.emplace_hint(coeff_.end(),
                    std::make_pair(entry.first.first + xShift,
                        entry.first.second + yShift), entry.second);
        }

        /**
         * Creates a new copy of the given polynomial.
         *
         * This constructor induces a deep copy of \a value.
         *
         * \nopython Python only supports Laurent polynomials with one type of
         * coefficient (the case where \a T is Integer).  Therefore
         * Python users can use the non-templated copy constructor.
         *
         * \param value the polynomial to clone.
         */
        template <CoefficientDomain U>
        requires std::assignable_from<T&, U>
        Laurent2(const Laurent2<U>& value) : coeff_(value.coeff_) {
        }

        /**
         * Creates a new polynomial from the given collection of coefficients.
         *
         * The coefficients should be presented as a collection of tuples of
         * the form `(d, e, v)`, each representing a term of the form
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
         * this routine takes a Python list of tuples.
         *
         * \param begin the beginning of the set of coefficients, as outlined
         * above.
         * \param end a past-the-end iterator indicating the end of the set of
         * coefficients.
         */
        template <std::input_iterator Iterator>
        requires requires(Iterator it) {
            { std::get<0>(*it) } -> std::convertible_to<long>;
            { std::get<1>(*it) } -> std::convertible_to<long>;
            { std::get<2>(*it) } -> std::convertible_to<T>;
        }
        Laurent2(Iterator begin, Iterator end) {
            for (auto it = begin; it != end; ++it) {
                if (std::get<2>(*it) == 0)
                    continue;

                auto res = coeff_.emplace(Exponents(
                    std::get<0>(*it), std::get<1>(*it)), std::get<2>(*it));
                if (! res.second) {
                    // This pair of exponents is already present.
                    // Accumulate, and erase if the coefficient becomes zero.
                    if ((res.first->second += std::get<2>(*it)) == 0)
                        coeff_.erase(res.first);
                }
            }
        }

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
        Laurent2(std::initializer_list<std::tuple<long, long, T>>
                coefficients) {
            for (const auto& c : coefficients) {
                if (std::get<2>(c) == 0)
                    throw InvalidArgument("One of the given tuples has a "
                        "value of zero");
                if (! coeff_.emplace(Exponents(std::get<0>(c), std::get<1>(c)),
                        std::get<2>(c)).second)
                    throw InvalidArgument("Two of the given tuples share the "
                        "same pair of exponents");
            }
        }

        /**
         * Creates a new two-variable Laurent polynomial from a one-variable
         * Laurent polynomial.
         *
         * Specifically, this will become the two-variable polynomial
         * `poly(x^xExp y^yExp)`.
         *
         * \param poly the one-variable polynomial into which we substitute
         * `x^xExp y^yExp`.
         * \param xExp the power of \a x to substitute into \a poly.
         * \param yExp the power of \a y to substitute into \a poly.
         */
        Laurent2(const Laurent<T>& poly, long xExp, long yExp) {
            if (xExp == 0 && yExp == 0) {
                // Create a single constant that sums all coefficients of poly.
                T sum; // zero-initialised
                for (long exp = poly.minExp(); exp <= poly.maxExp(); ++exp)
                    sum += poly[exp];
                if (sum != zero_)
                    coeff_.emplace(Exponents(0, 0), std::move(sum));
            } else {
                for (long exp = poly.minExp(); exp <= poly.maxExp(); ++exp) {
                    const T& coeff = poly[exp];
                    if (coeff != zero_)
                        coeff_.emplace(Exponents(xExp * exp, yExp * exp),
                            coeff);
                }
            }
        }

        /**
         * Sets this to become the zero polynomial.
         */
        void init() {
            coeff_.clear();
        }

        /**
         * Sets this to become the polynomial `x^d y^e` for the
         * given exponents \a d and \a e.
         *
         * \param xExp the new exponent \a d, which is attached to \a x.
         * \param yExp the new exponent \a e, which is attached to \a y.
         */
        void initExp(long xExp, long yExp) {
            coeff_.clear();
            coeff_.emplace(Exponents(xExp, yExp), 1);
        }

        /**
         * Returns whether this is the zero polynomial.
         *
         * \return \c true if and only if this is the zero polynomial.
         */
        bool isZero() const {
            return coeff_.empty();
        }

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
        const T& operator () (long xExp, long yExp) const {
            auto it = coeff_.find(Exponents(xExp, yExp));
            if (it == coeff_.end())
                return zero_;
            else
                return it->second;
        }

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
        void set(long xExp, long yExp, const T& value) {
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

        /**
         * Returns a C++ iterator pointing to the beginning of the list of
         * all non-zero coefficients and their corresponding exponent pairs.
         * Such iterators provide read-only access to the coefficients:
         * to modify coefficients you will need to call different routines,
         * such as set().
         *
         * Iterators in Laurent2 work differently from single-variable
         * polynomial classes such as Polynomial and Laurent:
         *
         * - they run through only the _non-zero_ coefficients of the
         *   polynomial, ignoring any zero coefficients;
         *
         * - dereferencing an iterator gives access to not only a coefficent,
         *   but also its corresponding exponent pair.
         *
         * Specifically, dereferencing an iterator will give a result of the
         * form `((s, t), c)`, where round brackets indicate pairs (stored via
         * `std::pair`).  This result describes a single term in the polynomial
         * of the form `c x^s y^t`.
         *
         * The order of iteration will be lexicographic in the pair `(s, t)`
         * above; that is, by increasing \a x exponent and then (in the case
         * of ties) by increasing \a y exponent.
         *
         * If this is the zero polynomial then the iterator range from
         * begin() to end() will be empty.
         *
         * \nopython For Python users, Laurent2 implements the Python iterable
         * interface.  You can iterate through coefficients in the same way
         * that you would iterate over any native Python container.
         *
         * \return an iterator pointing to the first non-zero coefficient and
         * corresponding exponent pair.
         */
        iterator begin() const {
            return coeff_.begin();
        }

        /**
         * Returns a C++ iterator pointing beyond the end of the list of
         * all non-zero coefficients and their corresponding exponent pairs.
         * Such iterators provide read-only access to the coefficients:
         * to modify coefficients you will need to call different routines,
         * such as set().
         *
         * See begin() for full details on how these iterators work.
         * In particular, they work differently from single-variable polynomial
         * classes such as Polynomial and Laurent: for instance, they only run
         * through _non-zero_ coefficients, and they also give access to the
         * corresponding exponent pairs.
         *
         * \nopython For Python users, Laurent2 implements the Python iterable
         * interface.  You can iterate through coefficients in the same way
         * that you would iterate over any native Python container.
         *
         * \return an iterator pointing beyond the last non-zero coefficient and
         * corresponding exponent pair.
         */
        iterator end() const {
            return coeff_.end();
        }

#ifdef __APIDOCS
        /**
         * Returns a Python iterator that provides read-only access to all
         * non-zero coefficients and their corresponding exponent pairs.
         * If this is the zero polynomial then this iterator range will be
         * empty.
         *
         * To enforce read-only access, Python iterators will return
         * coefficients and exponents by value, not by reference.  If you wish
         * to modify coefficients or exponents then you will need to call
         * different routines, such as set().
         *
         * Iterators in Laurent2 work differently from single-variable
         * polynomial classes such as Polynomial and Laurent:
         *
         * - they run through only the _non-zero_ coefficients of the
         *   polynomial, ignoring any zero coefficients;
         *
         * - each value returned by an iterator includes not only a coefficent,
         *   but also its corresponding exponent pair.
         *
         * Specifically, each value returned by an iterator will be of the
         * form `((s, t), c)`, where round brackets indicate pairs (stored via
         * Python tuples).  This value describes a single term in the polynomial
         * of the form `c x^s y^t`.
         *
         * The order of iteration will be lexicographic in the pair `(s, t)`
         * above; that is, by increasing \a x exponent and then (in the case
         * of ties) by increasing \a y exponent.
         *
         * \nocpp For C++ users, Laurent2 provides the usual begin() and end()
         * functions instead.  In particular, you can iterate over coefficients
         * and exponent pairs in the usual way using a range-based `for` loop.
         *
         * \return an iterator over all coefficients and their corresponding
         * exponent pairs.
         */
        auto __iter__() const;
#endif

        /**
         * Tests whether this and the given polynomial are equal.
         *
         * \param rhs the polynomial to compare with this.
         * \return \c true if and only if this and the given polynomial
         * are equal.
         */
        bool operator == (const Laurent2<T>& rhs) const {
            return coeff_ == rhs.coeff_;
        }

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
         * \return the result of the comparison between this
         * and the given polynomial.
         */
        std::strong_ordering operator <=> (const Laurent2<T>& rhs) const {
            return coeff_ <=> rhs.coeff_;
        }

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
        Laurent2& operator = (const Laurent2<T>& value) = default;

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
        template <CoefficientDomain U>
        requires std::assignable_from<T&, U>
        Laurent2& operator = (const Laurent2<U>& value) {
            coeff_ = value.coeff_;
            return *this;
        }

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
        void swap(Laurent2<T>& other) noexcept {
            coeff_.swap(other.coeff_);
        }

        /**
         * Multiplies this polynomial by `x^s y^t` for some integers \a s and
         * \a t.  This polynomial will be changed directly.
         *
         * \param s the power of \a x to multiply by.
         * \param t the power of \a y to multiply by.
         */
        void shift(long s, long t) {
            // It is difficult to change all the keys in a map without just
            // building a complete new map.
            Laurent2 ans = shifted(s, t);
            coeff_.swap(ans.coeff_);
        }

        /**
         * Returns the product of this polynomial with `x^s y^t` for some
         * integers \a s and \a t.  This polynomial will not be changed.
         *
         * \param s the power of \a x to multiply by.
         * \param t the power of \a y to multiply by.
         * \return the product of this with `x^s y^t`.
         */
        Laurent2 shifted(long s, long t) const {
            Laurent2 ans;
            for (auto& c : coeff_)
                ans.coeff_.emplace(
                    Exponents(c.first.first + s, c.first.second + t), c.second);
            return ans;
        }

        /**
         * Negates this polynomial.
         * This polynomial is changed directly.
         */
        void negate() {
            // TODO: negatable
            for (auto& c : coeff_)
                c.second = -c.second;
        }

        /**
         * Replaces `x` with `x^-1` in this polynomial.
         * This polynomial is changed directly.
         */
        void invertX() {
            std::map<Exponents, T> newCoeff;
            for (const auto& c : coeff_)
                newCoeff.insert(std::make_pair(
                    std::make_pair(- c.first.first, c.first.second), c.second));
            coeff_ = std::move(newCoeff);
        }

        /**
         * Replaces `y` with `y^-1` in this polynomial.
         * This polynomial is changed directly.
         */
        void invertY() {
            std::map<Exponents, T> newCoeff;
            for (const auto& c : coeff_)
                newCoeff.insert(std::make_pair(
                    std::make_pair(c.first.first, - c.first.second), c.second));
            coeff_ = std::move(newCoeff);
        }

        /**
         * Multiplies this polynomial by the given constant.
         *
         * \param scalar the scalar factor to multiply by.
         * \return a reference to this polynomial.
         */
        Laurent2& operator *= (const T& scalar) {
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

        /**
         * Divides this polynomial by the given constant.
         *
         * This uses the division operator `/=` for the coefficient type \a T.
         *
         * \pre The argument \a scalar is non-zero.
         *
         * \param scalar the scalar factor to divide by.
         * \return a reference to this polynomial.
         */
        Laurent2& operator /= (const T& scalar) {
            for (auto& c : coeff_)
                c.second /= scalar;

            // For integer division, we could have zeroed out some coefficients.
            removeZeroes();
            return *this;
        }

        /**
         * Adds the given polynomial to this.
         *
         * This and the given polynomial need not have the same range of
         * non-zero coefficients.
         *
         * \param other the polynomial to add to this.
         * \return a reference to this polynomial.
         */
        Laurent2& operator += (const Laurent2<T>& other) {
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

        /**
         * Subtracts the given polynomial from this.
         *
         * This and the given polynomial need not have the same range of
         * non-zero coefficients.
         *
         * \param other the polynomial to subtract from this.
         * \return a reference to this polynomial.
         */
        Laurent2& operator -= (const Laurent2<T>& other) {
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

        /**
         * Multiplies this by the given polynomial.
         *
         * This and the given polynomial need not have the same range of
         * non-zero coefficients.
         *
         * \param other the polynomial to multiply this by.
         * \return a reference to this polynomial.
         */
        Laurent2& operator *= (const Laurent2<T>& other) {
            if (isZero())
                return *this;
            if (other.isZero()) {
                init();
                return *this;
            }

            // The following code works even if &other == this, since we build
            // the coefficients of the product in a separate section of memory.
            std::map<Exponents, T> ans;

            for (const auto& left : coeff_)
                for (const auto& right : other.coeff_) {
                    Exponents e(left.first.first + right.first.first,
                        left.first.second + right.first.second);
                    T term = left.second * right.second;
                    auto result = ans.emplace(e, term);
                    if (! result.second)
                        result.first->second += std::move(term);
                }

            coeff_.clear();
            ans.swap(coeff_);

            // We might have zeroed out some coefficients.
            removeZeroes();
            return *this;
        }

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
                const char* varX = nullptr, const char* varY = nullptr) const {
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
        std::string str(const char* varX, const char* varY = nullptr) const {
            std::ostringstream out;
            writeTextShort(out, false, varX, varY);
            return std::move(out).str();
        }

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
        std::string utf8(const char* varX, const char* varY = nullptr) const {
            std::ostringstream out;
            writeTextShort(out, true, varX, varY);
            return std::move(out).str();
        }

        /**
         * Writes the tight encoding of this polynomial to the given output
         * stream.  See the page on \ref tight "tight encodings" for details.
         *
         * \nopython Use tightEncoding() instead, which returns a string.
         *
         * \param out the output stream to which the encoded string will
         * be written.
         */
        void tightEncode(std::ostream& out) const
                requires InherentlyTightEncodable<T> {
            for (const auto& c : coeff_) {
                // Write the (non-zero) coefficient before the exponents.
                // This way we can use tightEncode(0) as an unambiguous
                // terminator.
                c.second.tightEncode(out);
                regina::tightEncode(out, c.first.first);
                regina::tightEncode(out, c.first.second);
            }
            T().tightEncode(out); // The zero terminator
        }

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
        static Laurent2 tightDecode(std::istream& input)
                requires InherentlyTightEncodable<T> {
            Laurent2 ans;

            while (true) {
                T coeff = T::tightDecode(input);
                if (coeff == 0)
                    return ans;

                long x = regina::tightDecode<long>(input);
                long y = regina::tightDecode<long>(input);
                if (! ans.coeff_.emplace(Exponents(x, y),
                        std::move(coeff)).second)
                    throw InvalidInput("The tight encoding has a repeated "
                        "pair of exponents");
            }
        }

    private:
        /**
         * Removes all entries from coeff_ whose coefficients are zero.
         */
        void removeZeroes() {
            auto it = coeff_.begin();
            while (it != coeff_.end())
                if (it->second == 0)
                    it = coeff_.erase(it); // C++11: returns next element.
                else
                    ++it;
        }

    template <CoefficientDomain U>
    friend Laurent2<U> operator * (const Laurent2<U>&, const Laurent2<U>&);

    // For the time being, allow Link::homflyAZtoLM() to do low-level
    // operations on these polynomials.
    friend class Link;
};

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
template <CoefficientDomain T>
void swap(Laurent2<T>& a, Laurent2<T>& b) noexcept {
    a.swap(b);
}

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
template <CoefficientDomain T>
Laurent2<T> operator * (Laurent2<T> poly,
        const typename Laurent2<T>::Coefficient& scalar) {
    // When the argument poly is an lvalue reference, we perform a deep copy
    // due to pass-by-value.  If scalar == 0 then we don't need this deep copy,
    // since the argument can be ignored.  This special-case optimisation
    // would require two different lvalue/rvalue implementations of *, and
    // so we leave it for now.
    poly *= scalar;
    return poly;
}

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
template <CoefficientDomain T>
Laurent2<T> operator * (const typename Laurent2<T>::Coefficient& scalar,
        Laurent2<T> poly) {
    // See the notes above on a possible optimisation for scalar == 0.
    poly *= scalar;
    return poly;
}

/**
 * Divides the given polynomial by the given scalar constant.
 *
 * This uses the division operator `/=` for the coefficient type \a T.
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
template <CoefficientDomain T>
Laurent2<T> operator / (Laurent2<T> poly,
        const typename Laurent2<T>::Coefficient& scalar) {
    poly /= scalar;
    return poly;
}

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
template <CoefficientDomain T>
Laurent2<T> operator + (const Laurent2<T>& lhs, const Laurent2<T>& rhs) {
    // We have to make a deep copy since both arguments are read-only.
    return std::move(Laurent2<T>(lhs) += rhs);
}

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
template <CoefficientDomain T>
Laurent2<T> operator + (Laurent2<T>&& lhs, const Laurent2<T>& rhs) {
    return std::move(lhs += rhs);
}

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
template <CoefficientDomain T>
Laurent2<T> operator + (const Laurent2<T>& lhs, Laurent2<T>&& rhs) {
    return std::move(rhs += lhs);
}

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
template <CoefficientDomain T>
Laurent2<T> operator + (Laurent2<T>&& lhs, Laurent2<T>&& rhs) {
    return std::move(lhs += rhs);
}

/**
 * Returns the negative of the given polynomial.
 *
 * \param arg the polynomial to negate.
 * \return the negative of \a arg.
 *
 * \ingroup maths
 */
template <CoefficientDomain T>
Laurent2<T> operator - (Laurent2<T> arg) {
    arg.negate();
    return arg;
}

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
template <CoefficientDomain T>
Laurent2<T> operator - (const Laurent2<T>& lhs, const Laurent2<T>& rhs) {
    // We have to make a deep copy since both arguments are read-only.
    return std::move(Laurent2<T>(lhs) -= rhs);
}

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
template <CoefficientDomain T>
Laurent2<T> operator - (Laurent2<T>&& lhs, const Laurent2<T>& rhs) {
    return std::move(lhs -= rhs);
}

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
template <CoefficientDomain T>
Laurent2<T> operator - (const Laurent2<T>& lhs, Laurent2<T>&& rhs) {
    rhs.negate();
    return std::move(rhs += lhs);
}

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
template <CoefficientDomain T>
Laurent2<T> operator - (Laurent2<T>&& lhs, Laurent2<T>&& rhs) {
    return std::move(lhs -= rhs);
}

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
template <CoefficientDomain T>
Laurent2<T> operator * (const Laurent2<T>& lhs, const Laurent2<T>& rhs) {
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
                result.first->second += std::move(term);
        }

    // We might have zeroed out some coefficients.
    ans.removeZeroes();
    return ans;
}

#ifndef __DOXYGEN
// Don't confuse doxygen with specialisations.
template <CoefficientDomain T>
struct RingTraits<Laurent2<T>> {
    inline static const Laurent2<T> zero;
    inline static const Laurent2<T> one { { 0, 0, 1 } };
    static constexpr bool commutative = RingTraits<T>::commutative;
    static constexpr bool zeroInitialised = true;
    static constexpr bool zeroDivisors = false; // since T is a domain
    static constexpr bool inverses = false;
};
#endif // __DOXYGEN

// Inline constants for Laurent2:

template <CoefficientDomain T>
const T Laurent2<T>::zero_(0);

} // namespace regina

#endif
