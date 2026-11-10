
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

#ifndef __REGINA_LAURENT_H
#ifndef __DOXYGEN
#define __REGINA_LAURENT_H
#endif

/*! \file maths/laurent.h
 *  \brief Implements Laurent polynomials over arbitrary rings.
 */

#include "concepts/io.h"
#include "concepts/iterator.h"
#include "core/output.h"
#include "maths/forward.h"
#include "maths/polyops.h"
#include "utilities/stringutils.h"
#include "utilities/tightencoding.h"
#include "utilities/typeutils.h"
#include <array>
#include <functional>
#include <iostream>
#include <iterator>
#include <ranges>

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

namespace detail {
    /**
     * Provides supporting types for use in the implementation and testing of
     * Laurent polynomials.
     *
     * \nopython
     *
     * \ingroup detail
     */
    template <CoefficientDomain T, int vars>
    requires (vars > 0 && vars <= 3)
    struct LaurentHelper {
        // The base case vars == 1 is specialised below.
        static_assert(vars > 1);

        /**
         * The data to store for a single term in a (`var`)-variable Laurent
         * polynomial \a L.
         *
         * If `vars > 1`, this is a smaller Laurent polynomial in the last
         * `vars - 1` variables of \a L, and is attached to an individual
         * power of the first variable of \a L.
         *
         * If `vars == 1`, this is just a single coefficient of type \a T.
         */
        using Term = Laurent<T, vars - 1>;
    };

    #ifndef __DOXYGEN
    // Don't confuse doxygen with specialisations.
    template <CoefficientDomain T>
    struct LaurentHelper<T, 1> {
        using Term = T;
    };
    #endif
} // namespace detail

/**
 * A bidirectional iterator that steps through the non-zero coefficients of a
 * Laurent polynomial.
 *
 * This class works with `Laurent<T, vars>`.  It behaves like a bidirectional
 * iterator over the non-zero coefficients of a (possibly multivariate)
 * polynomial, but also offers extra member functions for identifying which
 * monomial the current cofficient is attached to (i.e., for identifying the
 * current exponents of the polynomial variables).
 *
 * For a Laurent polynomial \a p, you begin iteration by calling either
 * `p.first()` or `p.last()` (for forward or backward iteration respectively),
 * which returns a LaurentCoefficient \a c.  You can then dereference \a c to
 * extract the coefficient, call `c.exp()` or `c.exponents()` to identify the
 * current exponents of the polynomial variables, and/or move the iterator
 * forwards or backwards using the usual operators `++c` and `--c`.
 * To test whether iteration has finished (i.e., whether \a c is no longer
 * dereferenceable), you can test whether \a c evaluates to `false` in a
 * boolean context, or you can compare \a c to a default-constructed
 * `LaurentCoefficient()` (which behaves like an end iterator).
 *
 * As an example, you could print all of the non-zero terms of a single-variable
 * Laurent polynomial using the following loop:
 *
 * ```cpp
 * for (auto c = poly.first(); c; ++c) {
 *     std::cout << "Term: " << *c << " x^" << c.exp<0>() << std::endl;
 * }
 * ```
 *
 * The order of forward iteration is lexicographic in the sequences of
 * exponents.  That is, the coefficient attached to the monomial `x^a y^b …`
 * will appear before the coefficient attached to the monomial `x^p y^q …`
 * if and only if the sequence `(a,b,…)` is lexicographically smaller than
 * `(p,q,…)`.  Here `x,y,…` denote the polynomial variables.
 *
 * LaurentCoefficient objects are small enough to pass by value and swap with
 * std::swap(), with no need for any specialised move operations or swap
 * functions.
 *
 * \python In Python, the classes LaurentCoefficient, LaurentCoefficient2, and
 * LaurentCoefficient3 refer to the specific template classes
 * `LaurentCoefficient<Integer, 1>`, `LaurentCoefficient<Integer, 2>`, and
 * `LaurentCoefficient<Integer, 3>` respectively.
 *
 * \tparam T the coefficient type of the corresponding Laurent polynomial.
 * \tparam vars the number of variables in the corresponding Laurent polynomial.
 *
 * \ingroup maths
 */
template <CoefficientDomain T, int vars>
requires (vars > 0 && vars <= 3)
class LaurentCoefficient {
    private:
        /**
         * A pointer to a term of the underlying Laurent polynomial.
         */
        using Pos = const typename Laurent<T, vars>::Term*;
        /**
         * An iterator pointing to a non-zero coefficient within some term of
         * the underlying Laurent polynomial.  If `vars == 1` then this is
         * just an empty struct, since each term already identifies a single
         * coefficient.
         *
         * The ternary operator is present so the compiler does not attempt to
         * instantiate the invalid type `Laurent<vars, 0>` in the case where
         * `vars == 1` (where the conditional evaluates to `Empty` anyway).
         */
        using Inner = std::conditional_t<vars != 1,
            LaurentCoefficient<T, vars == 1 ? 1 : vars - 1>, Empty>;

        Pos first_ { nullptr };
            /**< The first non-zero term of the underlying Laurent
                 polynomial \a p, or `null` if \a p is the zero polynomial. */
        Pos last_ { nullptr };
            /**< The last non-zero term of the underlying Laurent
                 polynomial \a p, or `null` if \a p is the zero polynomial. */
        long firstExp_;
            /**< The exponent of the first variable of the underlying Laurent
                 polynomial \a p that corresponds to the term \a first_.
                 If \a p is the zero polynomial then this is undefined. */

        Pos pos_ { nullptr };
            /**< The current term of the underlying Laurent polynomial, or
                 `null` if this iterator is not dereferenceable. */
        [[no_unique_address]] Inner inner_;
            /**< Points to the current non-zero coefficient within the current
                 term \a pos_.  If `vars == 1` then this is an empty data member
                 and is ignored.  If `vars > 1` then \a inner_ must be
                 dereferenceable if and only if \a pos_ is non-null. */

    public:
        /**
         * Creates an iterator that is not dereferenceable.
         *
         * This will compare as equal to any other non-dereferenceable iterator,
         * and so it can be used as an end iterator in a begin/end pair.
         */
        LaurentCoefficient() = default;
        /**
         * Creates a copy of the given iterator.
         */
        LaurentCoefficient(const LaurentCoefficient&) = default;
        /**
         * Sets this to be a copy of the given iterator.
         *
         * \return a reference to this iterator.
         */
        LaurentCoefficient& operator = (const LaurentCoefficient&) = default;

        /**
         * Tests whether this and the given iterator point to the same
         * non-zero coefficient of the same Laurent polynomial.
         *
         * Iterators that point to coefficients attached to different monomials
         * within the same polynomial will compare as different, even if both
         * coefficients take the same numerical value of type \a T.
         *
         * A non-dereferenceable iterator will compare as equal to any other
         * non-dereferenceable iterator (even if they came from different
         * Laurent polynomials, or if they were default constructed and
         * therefore have no associated polynomial at all).
         *
         * \param rhs the iterator to compare to this.
         * \return `true` if and only if the iterators are equal as described
         * above.
         */
        bool operator == (const LaurentCoefficient& rhs) const {
            if constexpr (vars == 1)
                return pos_ == rhs.pos_;
            else
                return pos_ == rhs.pos_ && inner_ == rhs.inner_;
        }

        /**
         * Tests whether this iterator is dereferenceable.
         *
         * \return `true` if and only if this iterator is dereferenceable.
         */
        operator bool() const {
            return pos_;
        }

        /**
         * Preincrement operator.  This iterator will step forward to point to
         * the next non-zero coefficient of the underlying Laurent polynomial,
         * or will become non-dereferenceable if there are no more non-zero
         * coefficients.
         *
         * \pre This iterator is dereferenceable.
         *
         * \nopython The postincrement operator is present in Python as the
         * member function inc().
         *
         * \return a reference to this iterator.
         */
        LaurentCoefficient& operator ++() {
            if constexpr (vars == 1) {
                while (true) {
                    // Advance to the next non-zero coefficient.
                    if (pos_ == last_) {
                        pos_ = nullptr;
                        break;
                    }
                    ++pos_;
                    if (*pos_ != 0)
                        break;
                }
            } else {
                if (! ++inner_) {
                    while (true) {
                        // Advance to the next non-zero term.
                        if (pos_ == last_) {
                            pos_ = nullptr;
                            break;
                        }
                        ++pos_;
                        if (! pos_->isZero()) {
                            inner_ = pos_->first();
                            break;
                        }
                    }
                }
            }
            return *this;
        }

        /**
         * Postincrement operator.  This iterator will step forward to point to
         * the next non-zero coefficient of the underlying Laurent polynomial,
         * or will become non-dereferenceable if there are no more non-zero
         * coefficients.
         *
         * \pre This iterator is dereferenceable.
         *
         * \python This routine is named inc() since Python does not support
         * the increment operator.
         *
         * \return a copy of this iterator before it was incremented.
         */
        LaurentCoefficient operator ++(int) {
            LaurentCoefficient ans(*this);
            ++(*this);
            return ans;
        }

        /**
         * Predecrement operator.  This iterator will step backward to point to
         * the previous non-zero coefficient of the underlying Laurent
         * polynomial, or will become non-dereferenceable if there are no prior
         * non-zero coefficients.
         *
         * \pre This iterator is dereferenceable.
         *
         * \nopython The postdecrement operator is present in Python as the
         * member function dec().
         *
         * \return a reference to this iterator.
         */
        LaurentCoefficient& operator --() {
            if constexpr (vars == 1) {
                while (true) {
                    // Move back to the previous non-zero coefficient.
                    if (pos_ == first_) {
                        pos_ = nullptr;
                        break;
                    }
                    --pos_;
                    if (*pos_ != 0)
                        break;
                }
            } else {
                if (! --inner_) {
                    while (true) {
                        // Move back to the previous non-zero term.
                        if (pos_ == first_) {
                            pos_ = nullptr;
                            break;
                        }
                        --pos_;
                        if (! pos_->isZero()) {
                            inner_ = pos_->last();
                            break;
                        }
                    }
                }
            }
            return *this;
        }

        /**
         * Postdecrement operator.  This iterator will step backward to point to
         * the previous non-zero coefficient of the underlying Laurent
         * polynomial, or will become non-dereferenceable if there are no prior
         * non-zero coefficients.
         *
         * \pre This iterator is dereferenceable.
         *
         * \python This routine is named dec() since Python does not support
         * the decrement operator.
         *
         * \return a copy of this iterator before it was decremented.
         */
        LaurentCoefficient operator --(int) {
            LaurentCoefficient ans(*this);
            --(*this);
            return ans;
        }

#ifdef __APIDOCS
        /**
         * Returns details of the current non-zero coefficient and increments
         * this iterator.
         *
         * Specifically, this function will return a pair `(exp, coeff)`, where
         * \a coeff is a deep copy of the (non-zero) coefficient of type \a T,
         * and \a exp indicates which monomial the coefficient is attached to.
         * If `vars == 1` then \a exp will simply be a Python integer indicating
         * the corresponding exponent of the polynomial variable; if `vars > 1`
         * then \a exp will be a tuple of \a vars Python integers indicating
         * the corresponding exponent of each polynomial variable in turn.
         *
         * \nocpp For C++ users, LaurentCoefficient provides the usual iterator
         * preincrement and postincrement operators (`++`), and gives access
         * to details of the current coefficient via the deference operator
         * `(*)` and the functions exp() and exponents().
         *
         * \exception StopIteration This iterator is not dereferenceable.
         *
         * \return details of the coefficient that this iterator is pointing to
         * (as described above), before the increment takes place.
         */
        auto __next__();
#endif

        /**
         * Returns a read-only reference to the current non-zero coefficient
         * of the underlying Laurent polynomial.
         *
         * \pre This iterator is dereferenceable.
         *
         * \python This routine is named coeff() since Python does not support
         * the dereference operator.  Moreover, in order to enforce constness,
         * this routine returns by value (not by reference).
         *
         * \return the current non-zero coefficient.
         */
        const T& operator * () const {
            if constexpr (vars == 1)
                return *pos_;
            else
                return *inner_;
        }

        /**
         * Gives read-only member access to the current non-zero coefficient
         * of the underlying Laurent polynomial.
         *
         * \pre This iterator is dereferenceable.
         *
         * \return member access to the current non-zero coefficient.
         */
        const T* operator -> () const {
            if constexpr (vars == 1)
                return pos_;
            else
                return inner_.operator->();
        }

        /**
         * Identifies which exponent of the given polynomial variable
         * corresponds to the current non-zero coefficient.  This allows you
         * to determine which monomial the current coefficient is attached to.
         *
         * This template function is the fastest way to obtain the current
         * exponent if you know \a var at compile time.
         *
         * \pre This iterator is dereferenceable.
         *
         * \nopython Instead use the variant `exp(var)`, where \a var is
         * passed at runtime.
         *
         * \tparam var indicates which variable of the polynomial you wish to
         * query.
         *
         * \return the current exponent of the given polynomial variable.
         */
        template <int var> requires (var >= 0 && var < vars)
        long exp() const {
            if constexpr (var == 0)
                return (pos_ - first_) + firstExp_;
            else
                return inner_.template exp<var - 1>();
        }

        /**
         * Identifies which exponent of the given polynomial variable
         * corresponds to the current non-zero coefficient, where the variable
         * in question does not need to be known until runtime.  This routine
         * allows you to determine which monomial the current coefficient is
         * attached to.
         *
         * For C++ programmers who know \a var at compile time, you are better
         * off using the template function `exp<var>()` instead, which is
         * slightly faster.
         *
         * \pre This iterator is dereferenceable.
         *
         * \exception InvalidArgument The argument \a var is not within the
         * allowed range (from 0 to `vars-1` inclusive).
         *
         * \param var indicates which variable of the polynomial you wish to
         * query; this must be between 0 and `vars-1` inclusive.
         * \return the current exponent of the given polynomial variable.
         */
        long exp(int var) const {
            if (var < 0 || var >= vars)
                throw InvalidArgument("Unsupported variable index");

            return select_constexpr<0, vars, long>(var, [this](auto k) {
                return exp<k>();
            });
        }

        /**
         * Returns the full sequence of exponents corresponding to the current
         * non-zero coefficient.  Like exp(), this routine allows you to
         * determine which monomial the current coefficient is attached to.
         *
         * Calling `exponents()` is equivalent to building the array
         * `{exp(0), …, exp(vars - 1)}`.
         *
         * \pre This iterator is dereferenceable.
         *
         * \return the full sequence of exponents.
         */
        std::array<long, vars> exponents() const {
            std::array<long, vars> ans;
            for_constexpr<0, vars>([this, &ans](auto k) {
                ans[k] = exp<k>();
            });
            return ans;
        }

        /**
         * Identifies whether the current non-zero coefficient is in fact the
         * constant term of the underlying Laurent polynomial.
         *
         * This is true if and only if `exp<i>() == 0` for all \a i.
         *
         * \pre This iterator is dereferenceable.
         *
         * \return `true` if and only if this iterator is currently pointing to
         * the constant coefficient.
         */
        bool constantTerm() const {
            if constexpr (vars == 1)
                return exp<0>() == 0;
            else
                return exp<0>() == 0 && inner_.constantTerm();
        }

        /**
         * Returns a string representation of this iterator.
         *
         * If this iterator is dereferenceable, a suitable string will still
         * be returned.
         *
         * \return a string representation of this iterator.
         */
        std::string str() const {
            if (pos_) {
                std::ostringstream s;
                if constexpr (vars == 1) {
                    s << exp<0>() << " -> " << *pos_;
                } else if constexpr (vars == 2) {
                    s << exp<0>() << ',' << exp<1>() << " -> " << *inner_;
                } else { // vars == 3
                    s << exp<0>() << ',' << exp<1>() << ',' << exp<2>()
                        << " -> " << *inner_;
                }
                return s.str();
            } else
                return "(null)";
        }

    private:
        /**
         * Creates a start iterator suitable for iteration over the non-zero
         * coefficients of a single-variable Laurent polynomial.
         *
         * \pre The underlying Laurent polynomial is non-zero.
         *
         * \param first the first non-zero term of the underlying Laurent
         * polynomial.  This must be non-null.
         * \param last the last non-zero term of the underlying Laurent
         * polynomial  This must be non-null..
         * \param firstExp the exponent of the first variable of the underlying
         * Laurent polynomial that corresponds to the term \a first.
         * \param forward `true` if this should be a start iterator for
         * forward iteration, or `false` if it should be a start iterator for
         * backward iteration.
         */
        LaurentCoefficient(Pos first, Pos last, long firstExp, bool forward)
                requires (vars == 1) :
                first_(first), last_(last), firstExp_(firstExp),
                pos_(forward ? first : last) {
        }

        /**
         * Creates a start iterator suitable for iteration over the non-zero
         * coefficients of a multivariate Laurent polynomial.
         *
         * \pre The underlying Laurent polynomial is non-zero.
         *
         * \param first the first non-zero term of the underlying Laurent
         * polynomial.  This must be non-null.
         * \param last the last non-zero term of the underlying Laurent
         * polynomial  This must be non-null..
         * \param firstExp the exponent of the first variable of the underlying
         * Laurent polynomial that corresponds to the term \a first.
         * \param forward `true` if this should be a start iterator for
         * forward iteration, or `false` if it should be a start iterator for
         * backward iteration.
         */
        LaurentCoefficient(Pos first, Pos last, long firstExp, bool forward)
                requires (vars > 1) :
                first_(first), last_(last), firstExp_(firstExp),
                pos_(forward ? first : last),
                inner_(forward ? pos_->first() : pos_->last()) {
        }

    friend class Laurent<T, vars>;
};

#ifndef __APIDOCS
} namespace std {
    template <regina::CoefficientDomain T, int vars>
    requires (vars > 0 && vars <= 3)
    struct iterator_traits<regina::LaurentCoefficient<T, vars>> {
        using value_type = T;
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;
    };
} namespace regina {
#endif

/**
 * Represents a Laurent polynomial with coefficients of type \a T.  A Laurent
 * polynomial differs from an ordinary polynomial in that it allows negative
 * exponents (so, unlike the Polynomial class, you can represent both `2+3x`
 * and `1+1/x`).
 *
 * As of Regina 8.0, this class can be configured to represent either
 * single-variable or multivariate Laurent polynomials, via the optional
 * template parameter \a vars.  In particular, `Laurent<T, 2>` replaces the
 * old type `Laurent2<T>` from Regina 7.x, which represents Laurent polynomials
 * in two variables.  In the documentation for this class, we frequently refer
 * to the individual polynomial variables as `x,y,…`.
 *
 * This class uses _dense_ storage: coefficients are stored in contiguous
 * ranges, including zero coefficients.  Like `std::vector`, Laurent polynomials
 * allocate additional memory to accommodate future growth, which means that
 * (for example) appending a term to a single-variable polynomial with exponent
 * `maxExp() + 1` or `minExp() - 1` takes amortised constant time.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.  If a Laurent object is
 * moved from, it can later be reused by assigning it a new value or by
 * calling one of the initialisation functions init() or initExp().
 *
 * \pre The coefficient type \a T has the property that, if an object is moved
 * from, it can later be reused by assigning it a new value.  Examples of types
 * with this property include C++ standard library types (see C++ 20 standard,
 * section 16.5.5.16), and Regina's Integer, LargeInteger and Rational classes.
 *
 * \python In Python, the classes Laurent, Laurent2, and Laurent3 refer to the
 * specific template classes `Laurent<Integer>`, `Laurent<Integer, 2>`, and
 * `Laurent<Integer, 3>` respectively.
 *
 * \tparam T the coefficient type.  A typical coefficient type would be
 * Integer or Rational.  Note that native C++ integer types are _not_
 * supported (since they have no zero-initialising default constructor).
 *
 * \tparam vars the number of variables in this type of Laurent polynomial.
 * This is optional, and defaults to 1.  Currently it is limited to a maximum
 * of 3 (this is because a handful of member functions are implemented
 * separately for each value of \a vars).
 *
 * \ingroup maths
 */
template <CoefficientDomain T, int vars> // default vars=1 is in maths/forward.h
requires (vars > 0 && vars <= 3)
class Laurent :
        public ShortOutput<Laurent<T, vars>, true>,
        public TightEncodable<Laurent<T, vars>> {
    public:
        /**
         * The type of each coefficient of the polynomial.
         */
        using Coefficient = T;

        /**
         * A read-only iterator type for iterating over polynomial coefficients.
         * This is identical to \a const_iterator, since iterators over
         * coefficients are always read-only.
         *
         * Iteration is currently only supported for single-variable
         * polynomials, and so if `vars > 1` this type will be `void`.
         */
        using iterator = std::conditional_t<vars == 1, const T*, void>;

        /**
         * A read-only iterator type for iterating over polynomial coefficients.
         *
         * Iteration is currently only supported for single-variable
         * polynomials, and so if `vars > 1` this type will be `void`.
         */
        using const_iterator = std::conditional_t<vars == 1, const T*, void>;

        // Make sure the compiler can see the zero-argument string output
        // routines, since we declare alternative versions of these below.
        using ShortOutput<Laurent<T, vars>, true>::str;
        using ShortOutput<Laurent<T, vars>, true>::utf8;

    private:
        /**
         * The data to store for each term in the internal array of terms.
         *
         * If `vars > 1`, this is a smaller Laurent polynomial in the last
         * `vars - 1` variables of this polynomial, and is attached to an
         * individual power of the first variable of this polynomial.
         *
         * If `vars == 1`, this is just a single coefficient of type \a T.
         */
        using Term = detail::LaurentHelper<T, vars>::Term;

        long minExp_;
            /**< The minimum exponent for the first variable that appears in
                 the polynomial.  For the zero polynomial, this may take any
                 value strictly greater than \a maxExp_.  For a polynomial that
                 has been moved out of and is in an invalid (but re-assignable)
                 state, this may take any value at all. */
        long maxExp_;
            /**< The maximum exponent for the first variable that appears in
                 the polynomial.  For the zero polynomial, this may take any
                 value strictly less than \a minExp_.  For a polynomial that
                 has been moved out of and is in an invalid (but re-assignable)
                 state, this may take any value at all.  */
        long base_;
            /**< The exponent for the first variable corresponding to term_[0].
                 This may take any value if this is the zero polynomial.
                 For a non-zero polynomial, this is less than or equal to
                 \a minExp_.  For a polynomial that has been moved out of and
                 is in an invalid (but re-assignable) state, this may take
                 any value at all. */
        size_t capacity_;
            /**< The total allocated size of the \a term_ array.  If \a term_
                 is `null` (which is possible only for the zero polynomial),
                 this may take any value.  If \a term_ is non-null, then this
                 must be strictly positive; moreover, for a non-zero polynomial
                 it must be at least `maxExp_ - base_ + 1`.  For a polynomial
                 that has been moved out of and is in an invalid (but
                 re-assignable) state, \a capacity_ must still accurately
                 reflect the (strictly positive) allocated size of the
                 \a term_ array, or must be 0 if \a term_ is `null`. */
        Term* term_;
            /**< An array holding the terms of the polynomial.  Here `term_[i]`
                 is attached to `x^(base_ + i)` (where \a x is the first
                 polynomial variable), and describes a Laurent polynomial in the
                 remaining `vars - 1` variables.  For the zero polynomial, the
                 \a term_ array may optionally be `null`; if it is non-null,
                 then the array must have positive allocated size.
                 For a polynomial that has been moved out of and is in an
                 invalid (but re-assignable) state, \a term_ must still
                 be either an array of strictly positive size \a capacity_,
                 or `null`. */

        static const T zero_;
            /**< A zero coefficient that we can safely make references to. */

        /**
         * Returns the extra capacity to add when growing the array of terms.
         * This should always be positive; moreover, it should be asymptotically
         * `Θ(capacity_)` to ensure amortised constant time when pushing new
         * terms onto either end of the polynomial.
         *
         * \pre \a term_ is non-null (i.e., \a capacity_ genuinely describes
         * the allocated size of the \a term_ array).
         *
         * \return the extra capacity to add.
         */
        constexpr size_t growth() const {
            return (capacity_ >> 1) + 1;
        }

    public:
        /**
         * Creates the zero polynomial.
         */
        Laurent() : term_(nullptr), minExp_(0), maxExp_(-1) {
            // Note: if we ever change the implementation so that term_ is NOT
            // null under default construction, we will need to update the
            // implementation of diagonalise (which relies on this behaviour).
        }

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
        Laurent(const Laurent& value) {
            if (value.isZero()) {
                #if 1
                term_ = nullptr;
                #else
                // Allocate some space even though we don't need to.
                // This code is only for diagnostic/testing purposes, and
                // should not be used in production.
                // Using this will break the LaurentTest.reserveRange test,
                // but it should not break any of the other tests.
                capacity_ = 1;
                term_ = new Term[1];
                #endif
                minExp_ = 0;
                maxExp_ = -1;
            } else {
                #if 1
                minExp_ = base_ = value.minExp_;
                maxExp_ = value.maxExp_;
                capacity_ = value.maxExp_ - value.minExp_ + 1;
                term_ = new Term[capacity_];
                std::copy(
                    value.term_ + value.minExp_ - value.base_,
                    value.term_ + value.maxExp_ - value.base_ + 1, term_);
                #else
                // Allocate more space than we need to (at both ends).
                // This code is only for diagnostic/testing purposes, and
                // should not be used in production.
                // Using this will break the LaurentTest.reserveRange test,
                // but it should not break any of the other tests.
                minExp_ = value.minExp_;
                maxExp_ = value.maxExp_;
                base_ = minExp_ - 1;
                capacity_ = value.maxExp_ - value.minExp_ + 3;
                term_ = new Term[capacity_];
                std::copy(
                    value.term_ + value.minExp_ - value.base_,
                    value.term_ + value.maxExp_ - value.base_ + 1, term_ + 1);
                #endif
            }
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
        Laurent(const Laurent<U, vars>& value) {
            if (value.isZero()) {
                term_ = nullptr;
                minExp_ = 0;
                maxExp_ = -1;
            } else {
                minExp_ = base_ = value.minExp_;
                maxExp_ = value.maxExp_;
                capacity_ = value.maxExp_ - value.minExp_ + 1;
                term_ = new Term[capacity_];
                std::copy(
                    value.term_ + value.minExp_ - value.base_,
                    value.term_ + value.maxExp_ - value.base_ + 1, term_);
            }
        }

        /**
         * Moves the contents of the given polynomial to this new polynomial.
         * This is a fast (constant time) operation.
         *
         * \param value the polynomial to move.
         */
        Laurent(Laurent&& value) noexcept :
                minExp_(value.minExp_), maxExp_(value.maxExp_),
                base_(value.base_), capacity_(value.capacity_),
                term_(value.term_) {
            value.term_ = nullptr;

            // This leaves value in an invalid state (since we might not have
            // value.minExp_ > value.maxExp_); however, it will become valid
            // again if we assign it a new value or call one of the
            // initialisation member functions.
        }

        /**
         * Creates a new single-variable Laurent polynomial from a contiguous
         * sequence of coefficients.
         *
         * The coefficients should be given in order from the smallest
         * exponent term to the largest.  The first coefficient in the
         * sequence will be associated with the exponent \a minExp.
         *
         * There is no problem if the first and/or last coefficient in
         * the sequence is zero.
         * An empty sequence will be treated as the zero polynomial.
         *
         * This constructor induces a deep copy of the given range.
         *
         * The iterator type must be random access because this allows the
         * implementation to compute the sequence length in constant time.
         *
         * \python Instead of the iterators \a begin and \a end,
         * this routine takes a Python list of coefficients.
         *
         * \param minExp the exponent corresponding to the first
         * coefficient in the sequence.
         * \param begin the beginning of the sequence of coefficients.
         * \param end a past-the-end iterator indicating the end of the
         * sequence of coefficients.
         */
        template <RandomAccessIteratorFor<T> Iterator>
        Laurent(long minExp, Iterator begin, Iterator end)
                requires (vars == 1) {
            // Skip through any initial zero terms.
            while (begin != end && *begin == 0) {
                ++begin;
                ++minExp;
            }
            if (begin == end) {
                // We have the zero polynomial.
                term_ = nullptr;
                minExp_ = 0;
                maxExp_ = -1;
                return;
            } else {
                // We have a non-zero polynomial.
                capacity_ = end - begin;
                term_ = new Term[capacity_];
                minExp_ = base_ = minExp;
                maxExp_ = minExp + capacity_ - 1;

                Term* it = term_;
                while (begin != end)
                    *it++ = *begin++;

                // The final coefficient(s) might be zero: fix maxExp_.
                // We _do_ already know that the first coefficient is non-zero.
                for (--it; *it == 0; --it, --maxExp_)
                    ;
            }
        }

        /**
         * Creates a new single-variable Laurent polynomial from a hard-coded
         * contiguous sequence of coefficients.
         *
         * The coefficients should be given in order from the smallest
         * exponent term to the largest.  The first coefficient in the
         * sequence will be associated with the exponent \a minExp.
         *
         * There is no problem if the first and/or last coefficient in
         * the sequence is zero.
         * An empty sequence will be treated as the zero polynomial.
         *
         * \nopython Instead, use the Python constructor that takes a list
         * of coefficients (which need not be constant).
         *
         * \param minExp the exponent corresponding to the first
         * coefficient in the sequence.
         * \param coefficients the full sequence of coefficients.
         */
        Laurent(long minExp, std::initializer_list<T> coefficients)
                requires (vars == 1) :
                Laurent(minExp, coefficients.begin(), coefficients.end()) {
        }

        /**
         * Creates a new multivariate Laurent polynomial from a sparse
         * collection of terms.
         *
         * The terms should be presented as a collection of pairs
         * `{exp, coeff}`, where \a exp is an sequence of integer exponents
         * `{a,b,…}`, and \a coeff is the corresponding coefficient of type \a T
         * attached to the monomial `x^a y^b …`.  Here `x,y,…` denote the
         * polynomial variables.
         *
         * As an example, you can create the two-variable polynomial
         * `(x - x^-1) / y` by passing the two terms `([1,-1],1)` and
         * `([-1,-1],-1)`.
         *
         * The terms _must_ use distinct exponent sequences, and they may be
         * given in any order.  Passing coefficients of zero is allowed (any
         * such terms will be ignored completely).  An empty sequence will be
         * treated as the zero polynomial.
         *
         * Regarding the C++ iterator type:
         *
         * - The exponent/coefficient pairs should support C++ tuple-like
         *   access via `std::get()`.  This means in particular that you can
         *   use `std::pair` or `std::tuple`.
         *
         * - The exponent sequence should support iteration and size queries
         *   (via the standard C++ ranges interface).  This means that standard
         *   container types such as `std::array`, `std::tuple` and
         *   `std::vector` are all fine to use.
         *
         * - As a complete example, you could perhaps store each term using the
         *   type `std::pair<std::array<long, vars>, T>`, collect _all_ of
         *   these terms in a `std::vector` \a v, and then pass `v.begin()` and
         *   `v.end()` as the arguments to this constructor.
         *
         * \python Instead of the iterators \a begin and \a end, this routine
         * takes a Python sequence of pairs, such as
         * `Laurent2([ ((1,-1),1), ((-1,-1),-1) ])`.  It does not matter
         * whether you use Python tuples or Python lists or a mixture of both.
         *
         * \exception InvalidArgument The same exponent sequence is reused in
         * more than one term, and/or some exponent sequence has a length
         * different from \a vars.
         *
         * \param begin the beginning of the collection of terms, as described
         * above.
         * \param end a past-the-end iterator indicating the end of the
         * collection of terms.
         */
        template <std::input_iterator Iterator>
        requires requires(Iterator it) {
            { std::get<0>(*it) } -> std::ranges::input_range;
            { std::get<0>(*it) } -> std::ranges::sized_range;
            { *std::ranges::begin(std::get<0>(*it)) } ->
                std::convertible_to<long>;
            { std::get<1>(*it) } -> std::convertible_to<T>;
        } && (vars > 1)
        Laurent(Iterator begin, Iterator end) : Laurent() {
            std::array<long, vars> exp;
            for (auto it = begin; it != end; ++it) {
                if (std::ranges::size(std::get<0>(*it)) != vars)
                    throw InvalidArgument(
                        "Exponent sequence has the wrong length");

                // We quietly accept reuse of exponent sequences when at most
                // one appearance assigns a non-zero coefficient.  (Otherwise
                // detecting reuse becomes expensive.)
                if (std::get<1>(*it) == 0)
                    continue;

                auto res = std::ranges::copy(
                    std::ranges::begin(std::get<0>(*it)),
                    std::ranges::end(std::get<0>(*it)),
                    exp.begin());

                if (isZero() || exp[0] < minExp_ || exp[0] > maxExp_) {
                    // No reuse here.  Pass through to set(), which might or
                    // might not need to reallocate.
                    if constexpr (vars == 2)
                        set(exp[0], exp[1], std::get<1>(*it));
                    else // vars == 3
                        set(exp[0], exp[1], exp[2], std::get<1>(*it));
                } else {
                    // We might be reusing an exponent sequence... but we do
                    // know that exp[0] is already within our allocated range
                    // and this assignment will not change minExp_ or maxExp_.
                    // Pass through to the inner term and do this all again.
                    auto& t1 = term_[exp[0] - base_];
                    if (t1.isZero() || exp[1] < t1.minExp_ ||
                            exp[1] > t1.maxExp_) {
                        if constexpr (vars == 2)
                            t1.set(exp[1], std::get<1>(*it));
                        else // vars == 3
                            t1.set(exp[1], exp[2], std::get<1>(*it));
                    } else {
                        if constexpr (vars == 2) {
                            // Both exp[0] and exp[1] are within range:
                            // we can go ahead and make the assignment.
                            T& t2 = t1.term_[exp[1] - t1.base_];
                            if (t2 == 0)
                                t2 = std::get<1>(*it);
                            else
                                throw InvalidArgument("Exponent sequence "
                                    "reused in multiple terms");
                        } else { // vars == 3
                            // One more time around.
                            auto& t2 = t1.term_[exp[1] - t1.base_];
                            if (t2.isZero() || exp[2] < t2.minExp_ ||
                                    exp[2] > t2.maxExp_) {
                                t2.set(exp[2], std::get<1>(*it));
                            } else {
                                T& t3 = t2.term_[exp[2] - t2.base_];
                                if (t3 == 0)
                                    t3 = std::get<1>(*it);
                                else
                                    throw InvalidArgument("Exponent sequence "
                                        "reused in multiple terms");
                            }
                        }
                    }
                }
            }
        }

        /**
         * Creates a new multivariate Laurent polynomial from a hard-coded
         * sparse collection of terms.
         *
         * The terms should be presented as a collection of pairs
         * `{exp, coeff}`, where \a exp is an sequence of integer exponents
         * `{a,b,…}`, and \a coeff is the corresponding coefficient of type \a T
         * attached to the monomial `x^a y^b …`.  Here `x,y,…` denote the
         * polynomial variables.
         *
         * As an example, you can create the two-variable polynomial
         * `(x - x^-1) / y` (which has two terms) via the following C++ code:
         *
         * ```cpp
         * Laurent<Integer, 2> poly({ {{1,-1},1}, {{-1,-1},-1} });
         * ```
         *
         * The terms _must_ use distinct exponent sequences, and they may be
         * given in any order.  Passing coefficients of zero is allowed (any
         * such terms will be ignored completely).  An empty sequence will be
         * treated as the zero polynomial.
         *
         * \nopython Instead, use the Python constructor that takes a sequence
         * of pairs, such as `Laurent2([ ((1,-1),1), ((-1,-1),-1) ])`.  It does
         * not matter whether you use Python tuples or Python lists or a mixture
         * of both.
         *
         * \exception InvalidArgument The same exponent sequence is reused in
         * more than one term.
         *
         * \param data the list of terms, as described above.
         */
        Laurent(std::initializer_list<std::pair<std::array<long, vars>, T>>
                data) requires (vars > 1) : Laurent(data.begin(), data.end()) {
        }

        /**
         * Creates a new polynomial with a single constant term, equal to the
         * given value.
         *
         * There is no problem if the given value is zero.
         *
         * \param constant the value of this new constant polynomial.
         */
        Laurent(const T& constant) {
            if (constant == 0) {
                term_ = nullptr;
                minExp_ = 0;
                maxExp_ = -1;
            } else {
                minExp_ = maxExp_ = base_ = 0;
                capacity_ = 1;
                term_ = new Term[1];
                *term_ = constant;
            }
        }

        /**
         * Creates a new polynomial with a single constant term, whose
         * contents will be moved out of the given value.
         *
         * There is no problem if the given value is zero.
         *
         * \param constant the value of this new constant polynomial.
         */
        Laurent(T&& constant) {
            if (constant == 0) {
                term_ = nullptr;
                minExp_ = 0;
                maxExp_ = -1;
            } else {
                minExp_ = maxExp_ = base_ = 0;
                capacity_ = 1;
                term_ = new Term[1];
                *term_ = std::move(constant);
            }
        }

        /**
         * Creates a new polynomial with a single integer constant term.
         *
         * There is no problem if the given constant is zero (though you can
         * also construct the zero polynomial by passing no arguments at all).
         *
         * The extent to which this constructor can handle large native
         * integer types without overflow will depend on how well such integer
         * types are supported by the coefficient type \a T.
         *
         * \python It is assumed that the type \a IntType is \c long.
         *
         * \param constant the value of this new polynomial.
         */
        template <CppInteger IntType>
        Laurent(IntType constant) {
            if (constant == 0) {
                term_ = nullptr;
                minExp_ = 0;
                maxExp_ = -1;
            } else {
                minExp_ = maxExp_ = base_ = 0;
                capacity_ = 1;
                term_ = new Term[1];
                *term_ = constant;
            }
        }

        /**
         * Creates the product of two Laurent polynomials that use independent
         * sets of variables.  Specifically, this creates a `(v + w)`-variable
         * polynomial that is the product `p(…) q(…)`, where \a p uses the first
         * \a v variables of the resulting polynomial, and \a q uses the last
         * \a w variables of the resulting polynomial.
         *
         * For example, if \a p is the single-variable polynomial `x^3 + 2 x`
         * and \a q is the two-variable polynomial `x y - y`, then
         * `Laurent<Integer, 3>(p, q)` will be the three-variable product
         * `(x^3 + 2 x)(y z - z)`.
         *
         * \param p the first of the two polynomials to multiply; this will
         * use the first \a v of the available \a vars variables in the
         * resulting polynomial.
         * \param q the second of the two polynomials to multiply; this will
         * use the last \a w of the available \a vars variables in the
         * resulting polynomial.
         */
        template <int v, int w> requires (v + w == vars)
        Laurent(const Laurent<T, v>& p, const Laurent<T, w>& q) {
            if (p.isZero() || q.isZero()) {
                term_ = nullptr;
                minExp_ = 0;
                maxExp_ = -1;
            } else {
                minExp_ = base_ = p.minExp_;
                maxExp_ = p.maxExp_;
                capacity_ = p.maxExp_ - p.minExp_ + 1;
                term_ = new Term[capacity_];

                Term* dest = term_;
                for (const auto* src = p.term_ + p.minExp_ - p.base_;
                        src != p.term_ + p.maxExp_ - p.base_ + 1;
                        ++src, ++dest) {
                    if constexpr (v == 1) {
                        if (*src != 0)
                            *dest = *src * q;
                    } else {
                        if (! src->isZero())
                            *dest = Term(*src, q);
                    }
                }
            }
        }

        /**
         * Creates the product of two Laurent polynomials that use independent
         * sets of variables.  Specifically, this creates a `(v + w)`-variable
         * polynomial that is the product `p(…) q(…)`, where \a p uses the first
         * \a v variables of the resulting polynomial, and \a q uses the last
         * \a w variables of the resulting polynomial.
         *
         * See the version of this constructor with const arguments for more
         * information and examples.
         *
         * \param p the first of the two polynomials to multiply; this will
         * use the first \a v of the available \a vars variables in the
         * resulting polynomial.
         * \param q the second of the two polynomials to multiply; this will
         * use the last \a w of the available \a vars variables in the
         * resulting polynomial.
         */
        template <int v, int w> requires (v + w == vars)
        Laurent(Laurent<T, v>&& p, const Laurent<T, w>& q) {
            if (p.isZero() || q.isZero()) {
                term_ = nullptr;
                minExp_ = 0;
                maxExp_ = -1;
            } else {
                minExp_ = base_ = p.minExp_;
                maxExp_ = p.maxExp_;
                capacity_ = p.maxExp_ - p.minExp_ + 1;
                term_ = new Term[capacity_];

                Term* dest = term_;
                for (T* src = p.term_ + p.minExp_ - p.base_;
                        src != p.term_ + p.maxExp_ - p.base_ + 1;
                        ++src, ++dest) {
                    if constexpr (v == 1) {
                        if (*src != 0)
                            *dest = std::move(*src) * q;
                    } else {
                        if (! src->isZero())
                            *dest = Term(std::move(*src), q);
                    }
                }
            }
        }

        /**
         * Destroys this polynomial.
         */
        ~Laurent() {
            delete[] term_;
        }

        /**
         * Sets this to become the zero polynomial.
         */
        void init() {
            // Leave term_ and capacity_ untouched: we might want to use this
            // space for terms again later.
            minExp_ = 0;
            maxExp_ = -1;
        }

        /**
         * Sets this to become the monomial `x^a y^b …` for the given sequence
         * of exponents `a,b,…`.  Here `x,y,…` represent the polynomial
         * variables.
         *
         * The arguments are separated into \a firstExp vs \a otherExp for
         * implementation reasons only - to create the monomial `x^a y^b …`
         * you would simply call `initExp(a, b, …)`.
         *
         * \param firstExp the exponent of the first polynomial variable \a x.
         * \param otherExp the exponents of the remaining `(vars - 1)`
         * polynomial variables `y,…`.
         */
        template <std::convertible_to<long>... Exponents>
        requires (sizeof...(Exponents) == vars - 1)
        void initExp(long firstExp, Exponents&&... otherExp) {
            // Reuse any pre-allocated term_ array, if we can.
            if (term_) {
                size_t gap = capacity_ >> 1; // 0 ≤ gap < capacity_
                base_ = firstExp - gap;
                if constexpr (vars == 1)
                    term_[gap] = 1;
                else
                    term_[gap].initExp(std::forward<Exponents>(otherExp)...);
            } else {
                capacity_ = 1;
                term_ = new Term[1];
                base_ = firstExp;
                if constexpr (vars == 1)
                    *term_ = 1;
                else
                    term_->initExp(std::forward<Exponents>(otherExp)...);
            }
            minExp_ = maxExp_ = firstExp;
        }

        /**
         * Returns the monomial `x^a y^b …` for the given sequence of exponents
         * `a,b,…`.  Here `x,y,…` represent the polynomial variables.
         *
         * The arguments are separated into \a firstExp vs \a otherExp for
         * implementation reasons only - to create the monomial `x^a y^b …`
         * you would simply call `monomial(a, b, …)`.
         *
         * \param firstExp the exponent of the first polynomial variable \a x.
         * \param otherExp the exponents of the remaining `(vars - 1)`
         * polynomial variables `y,…`.
         * \return the requested monomial.
         */
        template <std::convertible_to<long>... Exponents>
        requires (sizeof...(Exponents) == vars - 1)
        static Laurent monomial(long firstExp, Exponents&&... otherExp) {
            Term* c = new Term[1];
            if constexpr (vars == 1)
                *c = 1;
            else
                c->initExp(std::forward<Exponents>(otherExp)...);
            return Laurent(firstExp, firstExp, c);
        }

        /**
         * Converts a single-variable polynomial `p(⋅)` into a multivariate
         * polynomial `p(v)` for a given variable \a v.  The other `(vars - 1)`
         * variables do not appear in the resulting polynomial at all.
         *
         * For example, if \a p is the single-variable polynomial `x^3 + 2 x`,
         * then `Laurent<Integer, 2>::embed<0>(p)` will be `x^3 + 2 x`, and
         * `Laurent<Integer, 2>::embed<1>(p)` will be `y^3 + 2 y`, where `x,y`
         * denote the two variables of the resulting multivariate polynomials.
         *
         * This template function (and its rvalue counterpart) are the fastest
         * ways to perform such a conversion if you know \a var at compile time.
         *
         * \nopython Instead use the variant `embed(p, var)`, where \a var is
         * passed at runtime.
         *
         * \tparam var indicates which variable of the resulting multivariate
         * polynomial should be substituted into \a p.
         *
         * \param p the single-variable polynomial `p(⋅)` to convert.
         * \result the resulting multivariate polynomial `p(v)`.
         */
        template <int var> requires (0 <= var && var < vars)
        static Laurent embed(const Laurent<T, 1>& p) requires (vars > 1) {
            if (p.isZero())
                return {};

            if constexpr (var == 0) {
                // Mirror the copy constructor.
                Term* terms = new Term[p.maxExp_ - p.minExp_ + 1];
                std::copy(
                    p.term_ + p.minExp_ - p.base_,
                    p.term_ + p.maxExp_ - p.base_ + 1, terms);
                return { p.minExp_, p.maxExp_, terms };
            } else {
                // Mirror the single-element constructor.
                Term* terms = new Term[1];
                if constexpr (vars == 2)
                    *terms = p;
                else
                    *terms = Term::template embed<var - 1>(p);
                return { 0, 0, terms };
            }
        }

        /**
         * Converts a single-variable polynomial `p(⋅)` into a multivariate
         * polynomial `p(v)` for a given variable \a v.  The other `(vars - 1)`
         * variables do not appear in the resulting polynomial at all.
         *
         * See the const version of this function for more information and
         * examples.
         *
         * \tparam var indicates which variable of the resulting multivariate
         * polynomial should be substituted into \a p.
         *
         * \param p the single-variable polynomial `p(⋅)` to convert.
         * \result the resulting multivariate polynomial `p(v)`.
         */
        template <int var> requires (0 <= var && var < vars)
        static Laurent embed(Laurent<T, 1>&& p) requires (vars > 1) {
            if (p.isZero())
                return {};

            if constexpr (var == 0) {
                // Mirror the copy constructor.
                Term* terms = new Term[p.maxExp_ - p.minExp_ + 1];
                std::move(
                    p.term_ + p.minExp_ - p.base_,
                    p.term_ + p.maxExp_ - p.base_ + 1, terms);
                return { p.minExp_, p.maxExp_, terms };
            } else {
                // Mirror the single-element constructor.
                Term* terms = new Term[1];
                if constexpr (vars == 2)
                    *terms = std::move(p);
                else
                    *terms = Term::embed<var - 1>(std::move(p));
                return { 0, 0, terms };
            }
        }

        /**
         * Converts a single-variable polynomial `p(⋅)` into a multivariate
         * polynomial `p(v)` for a given variable \a v, where the variable \a v
         * does not need to be known until runtime.  The other `(vars - 1)`
         * variables do not appear in the resulting polynomial at all.
         *
         * For example, if \a p is the single-variable polynomial `x^3 + 2 x`,
         * then `Laurent<Integer, 2>::embed(p, 0)` will be `x^3 + 2 x`, and
         * `Laurent<Integer, 2>::embed(p, 1)` will be `y^3 + 2 y`, where `x,y`
         * denote the two variables of the resulting multivariate polynomials.
         *
         * For C++ programmers who know the argument \a var at compile time,
         * you are better off using the template function `embed<var>()`
         * instead, which is slightly faster (and which also offers an rvalue
         * variant).
         *
         * \exception InvalidArgument The argument \a var is not within the
         * allowed range (from 0 to `vars-1` inclusive).
         *
         * \param p the single-variable polynomial `p(⋅)` to convert.
         * \param var indicates which variable of the resulting multivariate
         * polynomial should be substituted into \a p; this must be between
         * 0 and `vars-1` inclusive.
         * \result the resulting multivariate polynomial `p(v)`.
         */
        static Laurent embed(const Laurent<T, 1>& p, int var)
                requires (vars > 1) {
            if (var < 0 || var >= vars)
                throw InvalidArgument("Unsupported variable index");

            return select_constexpr<0, vars, Laurent>(var, [&p](auto k) {
                return embed<k>(p);
            });
        }

        /**
         * Converts a single-variable polynomial `p(⋅)` into the multivariate
         * polynomial `p(x y …)`, where `x,y,…` are the \a vars variables of
         * the resulting multivariate polynomial.
         *
         * For example, if \a p is the single-variable polynomial `x^3 + 2 x`,
         * then `Laurent<Integer, 2>::diagonalise(p)` will be the two-variable
         * polynomial `x^3 y^3 + 2 x y`.
         *
         * \param p the single-variable polynomial `p(⋅)` to convert.
         * \result the resulting polynomial `p(x y …)`.
         */
        static Laurent diagonalise(const Laurent<T, 1>& p) requires (vars > 1) {
            if (p.isZero())
                return {};

            Term* terms = new Term[p.maxExp_ - p.minExp_ + 1];

            long exp = p.minExp_;
            const T* src = p.term_ + p.minExp_ - p.base_;
            Term* dest = terms;
            for ( ; exp <= p.maxExp_; ++exp, ++src, ++dest)
                if (*src != 0) {
                    // At this point, we know that *dest has been default
                    // constructed - this means its own term_ array is null.
                    dest->minExp_ = dest->maxExp_ = dest->base_ = exp;
                    dest->capacity_ = 1;
                    dest->term_ = new Term::Term[1];

                    if constexpr (vars == 2) {
                        *dest->term_ = *src;
                    } else { // vars == 3
                        typename Term::Term* next = dest->term_;
                        next->minExp_ = next->maxExp_ = next->base_ = exp;
                        next->capacity_ = 1;
                        next->term_ = new T[1];
                        *next->term_ = *src;
                    }
                }

            return { p.minExp_, p.maxExp_, terms };
        }

        /**
         * Converts a single-variable polynomial `p(⋅)` into the multivariate
         * polynomial `p(x y …)`, where `x,y,…` are the \a vars variables of
         * the resulting multivariate polynomial.
         *
         * See the const version of this function for more information and
         * examples.
         *
         * \param p the single-variable polynomial `p(⋅)` to convert.
         * \result the resulting polynomial `p(x y …)`.
         */
        static Laurent diagonalise(Laurent<T, 1>&& p) requires (vars > 1) {
            if (p.isZero())
                return {};

            Term* terms = new Term[p.maxExp_ - p.minExp_ + 1];

            long exp = p.minExp_;
            T* src = p.term_ + p.minExp_ - p.base_;
            Term* dest = terms;
            for ( ; exp <= p.maxExp_; ++exp, ++src, ++dest)
                if (*src != 0) {
                    // At this point, we know that *dest has been default
                    // constructed - this means its own term_ array is null.
                    dest->minExp_ = dest->maxExp_ = dest->base_ = exp;
                    dest->capacity_ = 1;
                    dest->term_ = new Term::Term[1];

                    if constexpr (vars == 2) {
                        *dest->term_ = std::move(*src);
                    } else { // vars == 3
                        T* next = dest->term_;
                        next->minExp_ = next->maxExp_ = next->base_ = exp;
                        next->capacity_ = 1;
                        next->term_ = new Term[1];
                        *next->term_ = std::move(*src);
                    }
                }

            return { p.minExp_, p.maxExp_, terms };
        }

        /**
         * Returns the smallest exponent that appears in this polynomial
         * with a non-zero coefficient.  This is only relevant for
         * single-variable Laurent polynomials.
         *
         * If this is the zero polynomial, then this routine returns 0.
         *
         * \return the smallest exponent.
         */
        long minExp() const requires (vars == 1) {
            return isZero() ? 0 : minExp_;
        }

        /**
         * Returns the largest exponent that appears in this polynomial
         * with a non-zero coefficient.  This is only relevant for
         * single-variable Laurent polynomials.
         *
         * If this is the zero polynomial, then this routine returns 0.
         *
         * \return the largest exponent.
         */
        long maxExp() const requires (vars == 1) {
            return isZero() ? 0 : maxExp_;
        }

        /**
         * Returns whether this is the zero polynomial.
         *
         * \return \c true if and only if this is the zero polynomial.
         */
        bool isZero() const {
            return minExp_ > maxExp_;
        }

        /**
         * Indicates the range of terms for which memory is currently allocated.
         *
         * This is mainly provided for diagnostics and performance analysis;
         * end users will typically not need to use this routine.
         *
         * This routine returns a pair `(base, capacity)`, where \a base
         * indicates the smallest exponent of the first polynomial variable
         * for which memory is allocated, and \a capacity indicates the total
         * number of exponents of the first polynomial variable for which memory
         * is allocated.  This means that the _largest_ exponent of the first
         * variable for which memory is allocated will be `base + capacity - 1`.
         *
         * In the special case of the zero polynomial, it is possible (but not
         * necessarily true) that no memory is allocated at all.  If no memory
         * is allocated, then this routine will return `(0, 0)`.  If a zero
         * polynomial _does_ have memory allocated, then this memory will not
         * be holding any meaningful terms, and so this routine will return a
         * positive capacity but an arbitrary base.
         *
         * \return a pair `(base, capacity)` as described above, or `(0, 0)`
         * if this is the zero polynomial _and_ no memory is allocated at all.
         */
        std::pair<long, size_t> allocation() const {
            if (term_)
                return { base_, capacity_ };
            else
                return { 0, 0 };
        }

        /**
         * Reserves enough memory in the array of terms to span the given
         * range of exponents in the first polynomial variable.  This routine
         * merely affects the internal storage; it does not change the value of
         * this polynomial.
         *
         * This routine is intended for use with single-variable Laurent
         * polynomials, but it is made available for multivariate polynomials
         * also.
         *
         * In particular, for single-variable polynomials, after calling
         * `reserveRange(fromExp, toExp)` you can add non-zero coefficients
         * for any `x^i` where `fromExp ≤ i ≤ toExp` without needing to
         * reallocate memory and/or shuffle data around.
         *
         * Note that the range `[fromExp, toExp]` is inclusive at both ends
         * (unlike iterator ranges, for example).
         *
         * This routine is safe to call even if `fromExp > toExp` (in which
         * case it will do nothing at all).
         *
         * \param fromExp the smallest exponent for the first polynomial
         * variable whose coefficient will have storage reserved.
         * \param toExp the largest exponent for the first polynomial
         * variable whose coefficient will have storage reserved.
         */
        void reserveRange(long fromExp, long toExp) {
            if (fromExp > toExp) {
                return;
            } else if (! term_) {
                base_ = fromExp;
                capacity_ = toExp - fromExp + 1;
                term_ = new Term[capacity_];
            } else {
                // Use reallocateForRange(), which will preserve our existing
                // coefficient data, but which may also change minExp_ and
                // maxExp_ (which means we need to change them back).
                //
                // This approach is a bit heavyweight: the biggest unnecessary
                // cost is zeroing out coefficients in the ranges
                // [fromExp, minexp_) and/or (maxExp_, toExp] in the case where
                // we don't need to reallocate.  We will live with this for now.
                long oldMin = minExp_;
                long oldMax = maxExp_;
                reallocateForRange(fromExp, toExp);
                minExp_ = oldMin;
                maxExp_ = oldMax;
            }
        }

        /**
         * Returns the given coefficient of a single-variable Laurent
         * polynomial.
         *
         * Specifically, for a single-variable polynomial \a p, calling `p[a]`
         * will return the coefficient of `x^a`, where \a x is the polynomial
         * variable.  There are no restrictions on the exponent \a a.
         *
         * For multivariate polynomials you should use the round bracket (call)
         * operator instead, since that operator supports multiple arguments
         * (i.e., a sequence of exponents).
         *
         * \python Python users should use this square bracket (subscript)
         * operator even for multivariate polynomials.  Moreover, Python users
         * can also use this operator to _set_ cofficients; that is, you can
         * write `poly[exp] = value`.  However, when _getting_ a coefficient
         * this operator will return by value (to enforce constness), which
         * means for example you cannot write something like
         * `poly[exp].negate()`.
         *
         * \cpp For C++ users, this operator is read-only.  To _set_
         * coefficients, you must use the separate routine set().
         *
         * \param exp the exponent of the term whose coefficient should
         * be returned (i.e., the integer \a a in the explanation above).
         * \return the requested coefficient.
         */
        const T& operator [] (long exp) const requires (vars == 1) {
            if (exp >= minExp_ && exp <= maxExp_) // implies term_ is non-null
                return term_[exp - base_];
            else
                return zero_;
        }

        /**
         * Returns the given coefficient of a multivariate Laurent polynomial.
         *
         * Specifically, for a multivariate polynomial \a p, calling `p(a,b,…)`
         * will return the coefficient of `x^a y^b …`, where `x,y,…` are the
         * polynomial variables.  There are no restrictions on the exponents
         * `a,b,…`.
         *
         * \python In Python, this is the square bracket (subscript) operator,
         * not the round bracket (call) operator; that is, Python users can
         * access coefficients through the syntax `poly[a,b,…]`.  Moreover,
         * this operator can also _set_ cofficients; that is, you can write
         * `poly[a,b,…] = value`.  However, when _getting_ a coefficient this
         * operator will return by value (to enforce constness), which means
         * for example you cannot write something like `poly[a,b,…].negate()`.
         *
         * \cpp For C++ users, this operator is read-only.  To _set_
         * coefficients, you must use the separate routine set().
         *
         * \param firstExp the exponent of the first polynomial variable \a x
         * (i.e., the integer \a a in the explanation above).
         * \param otherExp the exponents of the remaining `(vars - 1)`
         * polynomial variables `y,…` (i.e., the integers `b,…` in the
         * explanation above).
         * \return the requested coefficient.
         */
        template <std::convertible_to<long>... Exponents>
        requires (vars > 1) && (sizeof...(Exponents) == vars - 1)
        const T& operator () (long firstExp, Exponents&&... otherExp) const {
            // TODO: Once we move to C++23, we can use the subscript operator.
            if (firstExp >= minExp_ && firstExp <= maxExp_) {
                // Here we are guaranteed that term_ is non-null.
                if constexpr (vars > 2) {
                    return term_[firstExp - base_](
                        std::forward<Exponents>(otherExp)...);
                } else {
                    return term_[firstExp - base_].operator[](
                        std::forward<Exponents>(otherExp)...);
                }
            } else
                return zero_;
        }

        /**
         * Changes the given coefficient of a single-variable Laurent
         * polynomial.
         *
         * Specifically, for a polynomial \a p, calling `p.set(a, value)`
         * will set the coefficient of `x^a` to \a value, where `x` is
         * the polynomial variable.  There are no restrictions on the
         * exponent `a`, and the new coefficient \a value may be zero.
         *
         * Note that it may be expensive to set a non-zero coefficient outside
         * the current span of exponents, since this may require allocating
         * more space and/or moving coefficients around in memory.  The Laurent
         * class does, however, use an exponential growth strategy that attempts
         * to minimise the cost of this.
         *
         * \python This set() routine is available, but you can also set
         * coefficients directly using syntax of the form `p[a] = value`.
         *
         * \param exp the exponent of the polynomial variable for the
         * coefficient that we should set.
         * \param value the new value to assign to the given coefficient.
         */
        inline void set(long exp, const T& value) requires (vars == 1) {
            setTo(value, exp); // use the unified implementation
        }

        /**
         * Changes the given coefficient of a two-variable Laurent polynomial.
         *
         * Specifically, for a polynomial \a p, calling `p.set(a, b, value)`
         * will set the coefficient of `x^a y^b` to \a value, where `x,y`
         * are the polynomial variables.  There are no restrictions on the
         * exponents `a,b`, and the new coefficient \a value may be zero.
         *
         * Note that it may be expensive to set a non-zero coefficient outside
         * the current span of exponents, since this may require allocating
         * more space and/or moving coefficients around in memory.  The Laurent
         * class does, however, use an exponential growth strategy that attempts
         * to minimise the cost of this.
         *
         * \python This set() routine is available, but you can also set
         * coefficients directly using syntax of the form `p[a, b] = value`.
         *
         * \param exp0 the exponent of the first polynomial variable for the
         * coefficient that we should set.
         * \param exp1 the exponent of the second polynomial variable for the
         * coefficient that we should set.
         * \param value the new value to assign to the given coefficient.
         */
        inline void set(long exp0, long exp1, const T& value)
                requires (vars == 2) {
            setTo(value, exp0, exp1); // use the unified implementation
        }

        /**
         * Changes the given coefficient of a three-variable Laurent polynomial.
         *
         * Specifically, for a polynomial \a p, calling `p.set(a, b, c, value)`
         * will set the coefficient of `x^a y^b z^c` to \a value, where `x,y,z`
         * are the polynomial variables.  There are no restrictions on the
         * exponents `a,b,c`, and the new coefficient \a value may be zero.
         *
         * Note that it may be expensive to set a non-zero coefficient outside
         * the current span of exponents, since this may require allocating
         * more space and/or moving coefficients around in memory.  The Laurent
         * class does, however, use an exponential growth strategy that attempts
         * to minimise the cost of this.
         *
         * \python This set() routine is available, but you can also set
         * coefficients directly using syntax of the form `p[a, b, c] = value`.
         *
         * \param exp0 the exponent of the first polynomial variable for the
         * coefficient that we should set.
         * \param exp1 the exponent of the second polynomial variable for the
         * coefficient that we should set.
         * \param exp2 the exponent of the third polynomial variable for the
         * coefficient that we should set.
         * \param value the new value to assign to the given coefficient.
         */
        inline void set(long exp0, long exp1, long exp2, const T& value)
                requires (vars == 3) {
            setTo(value, exp0, exp1, exp2); // use the unified implementation
        }

        /**
         * Returns an iterator pointing to the first non-zero coefficient of
         * this polynomial.  This can be used as a starting point for forward
         * iteration (via operator `++`) through all non-zero coefficients.
         *
         * Such iterators provide read-only access to the coefficients: to
         * modify coefficients you will need to call different routines,
         * such as set().
         *
         * If this is the zero polynomial, then the iterator that is returned
         * will be non-deferenceable.
         *
         * A typical forward iteration through the non-zero coefficients would
         * look like the following:
         *
         * ```cpp
         * for (auto c = poly.first(); c; ++c) {
         *     // use the coefficient *c and/or the exponent c.exp<...>()
         * }
         * ```
         *
         * The order of forward iteration is lexicographic in the sequences of
         * exponents.
         *
         * See the LaurentCoefficient class notes for further details and
         * examples on how iteration works for Laurent polynomials.
         *
         * \warning Note that last() is _not_ a past-the-end iterator, and so
         * `first()` and `last()` do not together form a full iterator range.
         *
         * \python Laurent also implements the Python iterable interface,
         * which means you can iterate through non-zero coefficients in the
         * same way that you would iterate over any native Python container.
         * See the __iter__() documentation for details on how this works.
         *
         * \return an iterator pointing to the first non-zero coefficient.
         */
        LaurentCoefficient<T, vars> first() const {
            if (isZero())
                return {};
            else
                return { term_ + minExp_ - base_, term_ + maxExp_ - base_,
                    minExp_, true /* forward */ };
        }

        /**
         * Returns an iterator pointing to the last non-zero coefficient of
         * this polynomial.  This can be used as a starting point for backward
         * iteration (via operator `--`) through all non-zero coefficients.
         *
         * Such iterators provide read-only access to the coefficients: to
         * modify coefficients you will need to call different routines,
         * such as set().
         *
         * If this is the zero polynomial, then the iterator that is returned
         * will be non-deferenceable.
         *
         * A typical backward iteration through the non-zero coefficients would
         * look like the following:
         *
         * ```cpp
         * for (auto c = poly.last(); c; --c) {
         *     // use the coefficient *c and/or the exponent c.exp<...>()
         * }
         * ```
         *
         * The order of forward iteration is lexicographic in the sequences of
         * exponents (and therefore the order of backward iteration is reverse
         * lexicographic in the exponents).
         *
         * See the LaurentCoefficient class notes for further details and
         * examples on how iteration works for Laurent polynomials.
         *
         * \warning Note that last() is _not_ a past-the-end iterator, and so
         * `first()` and `last()` do not together form a full iterator range.
         *
         * \python Laurent also implements the Python iterable interface,
         * which means you can iterate through non-zero coefficients in the
         * same way that you would iterate over any native Python container.
         * See the __iter__() documentation for details on how this works.
         *
         * \return an iterator pointing to the last non-zero coefficient.
         */
        LaurentCoefficient<T, vars> last() const {
            if (isZero())
                return {};
            else
                return { term_ + minExp_ - base_, term_ + maxExp_ - base_,
                    minExp_, false /* backward */ };
        }

#ifdef __APIDOCS
        /**
         * Returns a Python iterator that iterates through all non-zero
         * coefficients of this polynomial.
         *
         * Each step of the iteration will return a pair `(exp, coeff)`, where
         * \a coeff is the (non-zero) coefficient of type \a T, and \a exp
         * indicates which monomial the coefficient is attached to.
         * If `vars == 1` then \a exp will simply be a Python integer indicating
         * the corresponding exponent of the polynomial variable; if `vars > 1`
         * then \a exp will be a tuple of \a vars Python integers indicating
         * the corresponding exponent of each polynomial variable in turn.
         *
         * To enforce read-only access, Python iterators will return
         * coefficients by value, not by reference.  If you wish to modify
         * coefficients then you will need to call different routines, such as
         * set().
         *
         * \nocpp For forward iteration, C++ users should call first() and use
         * the (slightly non-standard) iteration pattern described there.
         *
         * \return an iterator over all non-zero coefficients of this
         * polynomial.
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
        bool operator == (const Laurent& rhs) const {
            if (isZero())
                return (rhs.isZero());
            if (rhs.isZero())
                return false;
            if (minExp_ != rhs.minExp_ || maxExp_ != rhs.maxExp_)
                return false;
            return std::equal(
                term_ + minExp_ - base_, term_ + maxExp_ - base_ + 1,
                rhs.term_ + rhs.minExp_ - rhs.base_);
        }

        /**
         * Tests whether this polynomial is equal to the given constant.
         *
         * \param constant the value to compare this polynomial against.
         * \return \c true if and only if this polynomial is equal to the
         * given constant.
         */
        bool operator == (const T& constant) const {
            if (constant == 0)
                return isZero();
            else
                return minExp_ == 0 && maxExp_ == 0 // implies term_ non-null
                    && term_[-base_] == constant;
        }

        /**
         * Tests whether this polynomial is equal to the given integer constant.
         *
         * The extent to which this operator can handle large native
         * integer types without overflow will depend on how well such integer
         * types are supported by the coefficient type \a T.
         *
         * \python It is assumed that the type \a IntType is \c long.
         *
         * \param constant the integer to compare this polynomial against.
         * \return \c true if and only if this polynomial is equal to the
         * given integer constant.
         */
        template <CppInteger IntType>
        bool operator == (IntType constant) const {
            if (constant == 0)
                return isZero();
            else
                return minExp_ == 0 && maxExp_ == 0 // implies term_ non-null
                    && term_[-base_] == constant;
        }

        /**
         * Compares this against the given polynomial under a total
         * ordering of all Laurent polynomials.
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
        std::strong_ordering operator <=> (const Laurent& rhs) const {
            if (isZero())
                return rhs.isZero() ? std::strong_ordering::equal :
                    std::strong_ordering::less;
            if (rhs.isZero())
                return std::strong_ordering::greater;
            if (auto c = minExp_ <=> rhs.minExp_; c != 0)
                return c;
            if (auto c = maxExp_ <=> rhs.maxExp_; c != 0)
                return c;
            return std::lexicographical_compare_three_way(
                term_ + minExp_ - base_,
                term_ + maxExp_ - base_ + 1,
                rhs.term_ + rhs.minExp_ - rhs.base_,
                rhs.term_ + rhs.maxExp_ - rhs.base_ + 1);
        }

        /**
         * Sets this to be a copy of the given polynomial.
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
        Laurent& operator = (const Laurent& value) {
            // Treat x = x separately, since otherwise we break things when we
            // reset base_ = value.minExp_.
            if (&value == this)
                return *this;

            if (value.isZero()) {
                init();
            } else {
                size_t required = value.maxExp_ - value.minExp_ + 1;
                if (! term_) {
                    capacity_ = required;
                    term_ = new Term[capacity_];
                    base_ = value.minExp_;
                } else if (capacity_ < required) {
                    delete[] term_;
                    capacity_ = std::max(required, capacity_ + growth());
                    term_ = new Term[capacity_];
                    // Aim to (roughly) centre the coefficients within the
                    // newly allocated array.
                    base_ = value.minExp_ - ((capacity_ - required) >> 1);
                } else {
                    base_ = value.minExp_;
                }
                minExp_ = value.minExp_;
                maxExp_ = value.maxExp_;
                std::copy(
                    value.term_ + value.minExp_ - value.base_,
                    value.term_ + value.maxExp_ - value.base_ + 1,
                    term_ + minExp_ - base_);
            }
            return *this;
        }

        /**
         * Sets this to be a copy of the given polynomial.
         *
         * This operator induces a deep copy of \a value.
         *
         * \param value the polynomial to copy.
         * \return a reference to this polynomial.
         */
        template <CoefficientDomain U>
        requires std::assignable_from<T&, U>
        Laurent& operator = (const Laurent<U, vars>& value) {
            // Treat x = x separately, since otherwise we break things when we
            // reset base_ = value.minExp_.
            if (&value == this)
                return *this;

            if (value.isZero()) {
                init();
            } else {
                size_t required = value.maxExp_ - value.minExp_ + 1;
                if (! term_) {
                    capacity_ = required;
                    term_ = new Term[capacity_];
                    base_ = value.minExp_;
                } else if (capacity_ < required) {
                    delete[] term_;
                    capacity_ = std::max(required, capacity_ + growth());
                    term_ = new Term[capacity_];
                    // Aim to (roughly) centre the coefficients within the
                    // newly allocated array.
                    base_ = value.minExp_ - ((capacity_ - required) >> 1);
                } else {
                    base_ = value.minExp_;
                }
                minExp_ = value.minExp_;
                maxExp_ = value.maxExp_;
                std::copy(
                    value.term_ + value.minExp_ - value.base_,
                    value.term_ + value.maxExp_ - value.base_ + 1,
                    term_ + minExp_ - base_);
            }
            return *this;
        }

        /**
         * Moves the contents of the given polynomial to this polynomial.
         * This is a fast (constant time) operation.
         *
         * \param value the polynomial to move.
         * \return a reference to this polynomial.
         */
        Laurent& operator = (Laurent&& value) noexcept {
            minExp_ = value.minExp_;
            maxExp_ = value.maxExp_;
            base_ = value.base_;
            // Let value dispose of the original coefficients in its destructor.
            // Note: this leaves value in an invalid state (since its exponent
            // range might exceed its capacity), but it can still be assigned
            // a new value and used from there.
            std::swap(capacity_, value.capacity_);
            std::swap(term_, value.term_);
            return *this;
        }

        /**
         * Sets this to the polynomial with a single constant term, equal to
         * the given value.
         *
         * There is no problem if the given value is zero.
         *
         * \param constant the value to assign to this constant polynomial.
         * \return a reference to this polynomial.
         */
        Laurent& operator = (const T& constant) {
            if (constant == 0) {
                init();
            } else {
                if (term_) {
                    // Try to center the coefficient within our
                    // already-allocated array.
                    long gap = capacity_ >> 1;
                    base_ = -gap;
                    term_[gap] = constant;
                } else {
                    capacity_ = 1;
                    term_ = new Term[1];
                    *term_ = constant;
                    base_ = 0;
                }
                minExp_ = maxExp_ = 0;
            }
            return *this;
        }

        /**
         * Sets this to the polynomial with a single constant term, whose
         * contents will be moved out of the given value.
         *
         * There is no problem if the given value is zero.
         *
         * \param constant the value to assign to this constant polynomial.
         * \return a reference to this polynomial.
         */
        Laurent& operator = (T&& constant) {
            if (constant == 0) {
                init();
            } else {
                if (term_) {
                    // Try to center the coefficient within our
                    // already-allocated array.
                    long gap = capacity_ >> 1;
                    base_ = -gap;
                    term_[gap] = std::move(constant);
                } else {
                    capacity_ = 1;
                    term_ = new Term[1];
                    *term_ = std::move(constant);
                    base_ = 0;
                }
                minExp_ = maxExp_ = 0;
            }
            return *this;
        }

        /**
         * Sets this to the polynomial with a single integer constant term,
         * equal to the given value.
         *
         * There is no problem if the given constant is zero.
         *
         * The extent to which this operator can handle large native
         * integer types without overflow will depend on how well such integer
         * types are supported by the coefficient type \a T.
         *
         * \param constant the value to assign to this constant polynomial.
         * \return a reference to this polynomial.
         */
        template <CppInteger IntType>
        Laurent& operator = (IntType constant) {
            if (constant == 0) {
                init();
            } else {
                if (term_) {
                    // Try to center the coefficient within our
                    // already-allocated array.
                    long gap = capacity_ >> 1;
                    base_ = -gap;
                    term_[gap] = constant;
                } else {
                    capacity_ = 1;
                    term_ = new Term[1];
                    *term_ = constant;
                    base_ = 0;
                }
                minExp_ = maxExp_ = 0;
            }
            return *this;
        }

        /**
         * Swaps the contents of this and the given polynomial.
         * This is a fast (constant time) operation.
         *
         * This and the given polynomial do not need to use the same ranges of
         * exponents.
         *
         * \param other the polynomial whose contents should be swapped
         * with this.
         */
        void swap(Laurent& other) noexcept {
            std::swap(minExp_, other.minExp_);
            std::swap(maxExp_, other.maxExp_);
            std::swap(base_, other.base_);
            std::swap(capacity_, other.capacity_);
            std::swap(term_, other.term_);
        }

        /**
         * Multiplies this polynomial by `x^s y^t …` for the given sequence of
         * exponent shifts `s,t,…`.  Here `x,y,…` represent the polynomial
         * variables.  This polynomial will be changed directly.
         *
         * The arguments are separated into \a firstExp vs \a otherExp for
         * implementation reasons only - to multiply by `x^s y^t …` you would
         * simply call `shift(s, t, …)`.
         *
         * This operation is very fast (for single-variable polynomials it runs
         * in small constant time).
         *
         * \param firstShift the power of first polynomial variable \a x to
         * multiply by.
         * \param otherShifts the powers of the remaining `(vars - 1)`
         * polynomial variables `y,…` to multiply by.
         */
        template <std::convertible_to<long>... Exponents>
        requires (sizeof...(Exponents) == vars - 1)
        void shift(long firstShift, Exponents&&... otherShifts) {
            if (! isZero()) {
                base_ += firstShift;
                minExp_ += firstShift;
                maxExp_ += firstShift;

                if constexpr (vars > 1)
                    for (auto it = term_ + minExp_ - base_;
                            it <= term_ + maxExp_ - base_; ++it)
                        if (*it != 0)
                            it->shift(std::forward<Exponents>(otherShifts)...);
            }
        }

        /**
         * Returns the product of this polynomial with `x^s y^t …` for the
         * given sequence of exponent shifts `s,t,…`.  Here `x,y,…` represent
         * the polynomial variables.  This polynomial is not changed.
         *
         * Be aware that the rvalue variant of shifted() is _much_ faster than
         * this const version (this const variant has the extra cost of a deep
         * copy, whereas for single-variable polynomials the rvalue variant
         * runs in small constant time).
         *
         * \param shifts the powers of the polynomial variables `x,y,…` to
         * multiply by; that is, the sequence of integers `s,t,…` as described
         * above.
         * \return the product of this with `x^s y^t …`.
         */
        template <std::convertible_to<long>... Exponents>
        requires (sizeof...(Exponents) == vars)
        Laurent shifted(Exponents&&... shifts) const& {
            Laurent ans(*this);
            ans.shift(std::forward<Exponents>(shifts)...);
            return ans;
        }

        /**
         * Returns the product of this polynomial with `x^s y^t …` for the
         * given sequence of exponent shifts `s,t,…`.  Here `x,y,…` represent
         * the polynomial variables.
         *
         * This rvalue variant of shifted() is very fast (for single-variable
         * polynomials it runs in small constant time).
         *
         * \param shifts the powers of the polynomial variables `x,y,…` to
         * multiply by; that is, the sequence of integers `s,t,…` as described
         * above.
         * \return the product of this with `x^s y^t …`.
         */
        template <std::convertible_to<long>... Exponents>
        requires (sizeof...(Exponents) == vars)
        Laurent shifted(Exponents&&... shifts) && {
            shift(std::forward<Exponents>(shifts)...);
            return std::move(*this);
        }

        /**
         * Multiplies all exponents in this polynomial by \a k for some
         * integer \a k.  This is equivalent to replacing the variable
         * \a x of the polynomial with `x^k`.  This function is only available
         * for single-variable Laurent polynomials.
         *
         * Both positive and negative scaling factors \a k are allowed.
         *
         * \pre \a k is non-zero.
         *
         * \exception InvalidArgument The given scaling factor \a k is zero.
         *
         * \param k the scaling factor to multiply exponents by.
         */
        void scaleUp(long k) requires (vars == 1) {
            if (k == 0) {
                throw InvalidArgument("scaleUp() requires a non-zero "
                    "scaling factor");
            } else if (k == 1 || isZero()) {
                return;
            } else if (k == -1) {
                invert<0>();
                return;
            }

            if (minExp_ == maxExp_) {
                long gap = minExp_ - base_;
                minExp_ *= k;
                maxExp_ = minExp_;
                base_ = minExp_ - gap;
                return;
            }

            if (k < 0) {
                // Reverse the sequence of coefficients.
                // This is simply the non-trivial case of invert<0>().
                std::reverse(term_ + minExp_ - base_,
                    term_ + maxExp_ - base_ + 1);
                base_ -= (minExp_ + maxExp_);
                auto tmp = minExp_;
                minExp_ = -maxExp_;
                maxExp_ = -tmp;

                // Now we just scale by |k|.
                k = -k;
            }

            // From here we have k ≥ 2, and minExp_ < maxExp_.

            // For now, we will reuse our existing term_ array only when we
            // can do so without changing the gap between base_ and minExp_.
            // Otherwise, if we needed to shift data left whilst expanding it
            // to the right, the order of operations would become very messy.
            long gap = minExp_ - base_;
            size_t newSpan = (maxExp_ - minExp_) * k;
            if (static_cast<size_t>(gap + newSpan) < capacity_) {
                // We can reuse.
                auto src = term_ + maxExp_ - base_;
                auto dest = term_ + gap + newSpan;
                do {
                    *dest-- = std::move(*src--);
                    for (long i = 1; i < k; ++i)
                        *dest-- = 0;
                } while (dest != src);

                minExp_ *= k;
                maxExp_ *= k;
                base_ = minExp_ - gap;
            } else {
                // We must reallocate.
                // Since scaleUp() and scaleDown() are typically final steps
                // in an algorithm (e.g., converting between polynomial output
                // formats), we just allocate what we need and don't use
                // growth() here.
                capacity_ = newSpan + 1;

                Term* newTerm = new Term[capacity_];
                auto src = term_ + minExp_ - base_;
                for (auto dest = newTerm;
                        src <= term_ + maxExp_ - base_; dest += k)
                    *dest = std::move(*src++);
                delete[] term_;
                term_ = newTerm;

                minExp_ *= k;
                maxExp_ *= k;
                base_ = minExp_;
            }
        }

        /**
         * Divides all exponents in this polynomial by \a k for some integer
         * \a k.  This is equivalent to replacing the variable \a x of the
         * polynomial with `x^(1/k)`.  This function is only available for
         * single-variable Laurent polynomials.
         *
         * Both positive and negative scaling factors \a k are allowed.
         *
         * \pre \a k is non-zero.
         * \pre All exponents in this polynomial with non-zero coefficients
         * are multiples of \a k.
         *
         * \exception InvalidArgument The given scaling factor \a k is zero.
         *
         * \exception FailedPrecondition The scaling factor \a k is non-zero,
         * but some exponent with a non-zero coefficient is not a multiple of
         * \a k.  Be aware that this polynomial might be changed before this
         * exception is thrown.
         *
         * \param k the scaling factor to divide exponents by.
         */
        void scaleDown(long k) requires (vars == 1) {
            if (k == 0) {
                throw InvalidArgument("scaleDown() requires a non-zero "
                    "scaling factor");
            } else if (k == 1 || isZero()) {
                return;
            } else if (k == -1) {
                invert<0>();
                return;
            }

            if (minExp_ % k != 0)
                throw FailedPrecondition("scaleDown(k) requires every exponent "
                    "with a non-zero coefficient to be divisible by k");

            if (minExp_ == maxExp_) {
                long gap = minExp_ - base_;
                minExp_ /= k;
                maxExp_ = minExp_;
                base_ = minExp_ - gap;
                return;
            }

            if (maxExp_ % k != 0)
                throw FailedPrecondition("scaleDown(k) requires every exponent "
                    "with a non-zero coefficient to be divisible by k");

            // From here we have |k| ≥ 2, and minExp_ < maxExp_.
            if (k > 0) {
                auto src = term_ + minExp_ - base_ + 1;
                auto dest = src;
                auto end = term_ + maxExp_ - base_ + 1;
                do {
                    for (long i = 1; i < k; ++i)
                        if ((*src++) != 0)
                            throw FailedPrecondition("scaleDown(k) requires "
                                "every exponent with a non-zero coefficient "
                                "to be divisible by k");
                    *dest++ = std::move(*src++);
                } while (src != end);

                long gap = minExp_ - base_;
                minExp_ /= k;
                maxExp_ /= k;
                base_ = minExp_ - gap;
            } else {
                k = -k;

                // Scale down by |k| like above, and then reverse the sequence
                // of coefficients.
                auto start = term_ + minExp_ - base_;
                auto src = start + 1;
                auto dest = src;
                auto end = term_ + maxExp_ - base_ + 1;
                do {
                    for (long i = 1; i < k; ++i)
                        if ((*src++) != 0)
                            throw FailedPrecondition("scaleDown(k) requires "
                                "every exponent with a non-zero coefficient "
                                "to be divisible by k");
                    *dest++ = std::move(*src++);
                } while (src != end);

                std::reverse(start, dest);

                long gap = minExp_ - base_;
                auto tmp = -(minExp_ / k);
                minExp_ = -(maxExp_ / k);
                maxExp_ = tmp;
                base_ = minExp_ - gap;
            }
        }

        /**
         * Negates this polynomial.  This polynomial is changed directly.
         */
        void negate() {
            if (isZero())
                return;
            for (auto it = term_ + minExp_ - base_;
                    it <= term_ + maxExp_ - base_; ++it)
                if (*it != 0)
                    it->negate();
        }

        /**
         * Replaces a single polynomial variable `v` with `v^-1`, leaving all
         * other variables unchanged.  This polynomial is changed directly.
         *
         * This template function is the fastest way to invert a variable
         * if you know \a var at compile time.
         *
         * For single-variable Laurent polynomials, calling `invert<0>()`
         * is equivalent to calling `scaleUp(-1)`.
         *
         * \nopython Instead use the variant `invert(var)`, where \a var is
         * passed at runtime.
         *
         * \tparam var indicates which variable of the polynomial to invert.
         */
        template <int var> requires (var >= 0 && var < vars)
        void invert() {
            if (isZero()) {
                return;
            } else if constexpr (var == 0) {
                if (minExp_ == maxExp_) {
                    base_ -= (minExp_ << 1);
                    minExp_ = -minExp_;
                    maxExp_ = -maxExp_;
                } else {
                    std::reverse(term_ + minExp_ - base_,
                        term_ + maxExp_ - base_ + 1);
                    base_ -= (minExp_ + maxExp_);
                    auto tmp = minExp_;
                    minExp_ = -maxExp_;
                    maxExp_ = -tmp;
                }
            } else {
                for (auto it = term_ + minExp_ - base_;
                        it <= term_ + maxExp_ - base_; ++it)
                    it->template invert<var - 1>();
            }
        }

        /**
         * Replaces a single polynomial variable `v` with `v^-1` leaving all
         * other variables unchanged, where the variable to invert does not
         * need to be known until runtime.  This polynomial is changed directly.
         *
         * For C++ programmers who know \a var at compile time, you are better
         * off using the template function `invert<var>()` instead, which is
         * slightly faster.
         *
         * For single-variable Laurent polynomials, calling `invert(0)`
         * is equivalent to calling `scaleUp(-1)`.
         *
         * \exception InvalidArgument The argument \a var is not within the
         * allowed range (from 0 to `vars-1` inclusive).
         *
         * \param var indicates which variable of the polynomial you wish to
         * invert; this must be between 0 and `vars-1` inclusive.
         */
        void invert(int var) {
            if (var < 0 || var >= vars)
                throw InvalidArgument("Unsupported variable index");

            return select_constexpr<0, vars, void>(var, [this](auto k) {
                invert<k>();
            });
        }

        /**
         * Deprecated function that replaces the first polynomial variable
         * `x` with `x^-1`, leaving all other polynomial variables unchanged.
         * This polynomial is changed directly.
         *
         * For single-variable Laurent polynomials, this is equivalent to
         * calling `scaleUp(-1)`.
         *
         * \deprecated Instead, call `invert<0>()` in C++, or `invert(0)` in
         * Python.
         */
        [[deprecated]] void invertX() {
            invert<0>();
        }

        /**
         * Deprecated function that replaces the second polynomial variable
         * `y` with `y^-1`, leaving all other polynomial variables unchanged.
         * This polynomial is changed directly.
         *
         * \deprecated Instead, call `invert<1>()` in C++, or `invert(1)` in
         * Python.
         */
        [[deprecated]] void invertY() {
            invert<1>();
        }

        /**
         * Deprecated function that replaces the third polynomial variable
         * `z` with `z^-1`, leaving all other polynomial variables unchanged.
         * This polynomial is changed directly.
         *
         * \deprecated Instead, call `invert<2>()` in C++, or `invert(2)` in
         * Python.
         */
        [[deprecated]] void invertZ() {
            invert<2>();
        }

        /**
         * Applies the given transformation to every non-zero coefficient of
         * a single-variable Laurent polynomial.
         *
         * Specifically, for each non-zero coefficient \a c attached to the
         * monomial `x^s` (where \a x is the polynomial variable), this routine
         * will call `action(c, s)`.
         *
         * Each coefficient will be passed to \a action by reference;
         * typically \a action would modify the coefficient in some way.
         * The corresponding exponent will be passed by value, and so cannot be
         * modified.  It is fine if \a action sets some coefficients to zero.
         *
         * The coefficients will be passed in increasing order of their
         * corresponding exponents.
         *
         * \nopython This function is (for now) not available in Python;
         * instead you can iterate through the coefficients and call `set()`.
         *
         * \param action the action (typically a lambda) to perform on each
         * coefficient and its corresponding exponent.
         */
        template <std::invocable<T&, long> Action>
        void transform(Action&& action) requires (vars == 1) {
            // We implement k-variable transforms separately for each k,
            // since otherwise we would need nested lambdas to pass through
            // recursive calls to transform() (and it there does not seem to
            // be a simple way to capture lambdas using perfect forwarding?).
            // TODO: Unify these transform() implementations.
            if (! isZero()) {
                auto it = term_ + minExp_ - base_;
                for (long exp = minExp_; exp <= maxExp_; ++exp, ++it)
                    if (*it != 0)
                        std::invoke(std::forward<Action>(action), *it, exp);
                fixDegrees();
            }
        }

        /**
         * Applies the given transformation to every non-zero coefficient of
         * a two-variable Laurent polynomial.
         *
         * Specifically, for each non-zero coefficient \a c attached to the
         * monomial `x^s y^t` (where \a x and \a y are the polynomial
         * variables), this routine will call `action(c, s, t)`.
         *
         * Each coefficient will be passed to \a action by reference;
         * typically \a action would modify the coefficient in some way.
         * The corresponding exponents will be passed by value, and so cannot be
         * modified.  It is fine if \a action sets some coefficients to zero.
         *
         * The coefficients will be passed in lexicographical order of their
         * corresponding exponent sequences `(s,t)`.
         *
         * \nopython This function is (for now) not available in Python;
         * instead you can iterate through the coefficients and call `set()`.
         *
         * \param action the action (typically a lambda) to perform on each
         * coefficient and its corresponding exponents.
         */
        template <std::invocable<T&, long, long> Action>
        void transform(Action&& action) requires (vars == 2) {
            if (! isZero()) {
                auto it0 = term_ + minExp_ - base_;
                for (long exp0 = minExp_; exp0 <= maxExp_; ++exp0, ++it0)
                    if (! it0->isZero()) {
                        auto it1 = it0->term_ + it0->minExp_ - it0->base_;
                        for (long exp1 = it0->minExp_; exp1 <= it0->maxExp_;
                                ++exp1, ++it1)
                            if (*it1 != 0)
                                std::invoke(std::forward<Action>(action),
                                    *it1, exp0, exp1);
                        it0->fixDegrees();
                    }
                fixDegrees();
            }
        }

        /**
         * Applies the given transformation to every non-zero coefficient of
         * a three-variable Laurent polynomial.
         *
         * Specifically, for each non-zero coefficient \a c attached to the
         * monomial `x^s y^t z^u` (where `x,y,z` are the polynomial variables),
         * this routine will call `action(c, s, t, u)`.
         *
         * Each coefficient will be passed to \a action by reference;
         * typically \a action would modify the coefficient in some way.
         * The corresponding exponents will be passed by value, and so cannot be
         * modified.  It is fine if \a action sets some coefficients to zero.
         *
         * The coefficients will be passed in lexicographical order of their
         * corresponding exponent sequences `(s,t,u)`.
         *
         * \nopython This function is (for now) not available in Python;
         * instead you can iterate through the coefficients and call `set()`.
         *
         * \param action the action (typically a lambda) to perform on each
         * coefficient and its corresponding exponents.
         */
        template <std::invocable<T&, long, long, long> Action>
        void transform(Action&& action) requires (vars == 3) {
            if (! isZero()) {
                auto i0 = term_ + minExp_ - base_;
                for (long e0 = minExp_; e0 <= maxExp_; ++e0, ++i0)
                    if (! i0->isZero()) {
                        auto i1 = i0->term_ + i0->minExp_ - i0->base_;
                        for (long e1 = i0->minExp_; e1 <= i0->maxExp_;
                                ++e1, ++i1)
                            if (! i1->isZero()) {
                                auto i2 = i1->term_ + i1->minExp_ - i1->base_;
                                for (long e2 = i1->minExp_; e2 <= i1->maxExp_;
                                        ++e2, ++i2)
                                    if (*i2 != 0)
                                        std::invoke(
                                            std::forward<Action>(action),
                                            *i2, e0, e1, e2);
                                i1->fixDegrees();
                            }
                        i0->fixDegrees();
                    }
                fixDegrees();
            }
        }

        /**
         * Multiplies this polynomial by the given constant.
         *
         * \param scalar the scalar factor to multiply by.
         * \return a reference to this polynomial.
         */
        Laurent& operator *= (const T& scalar) {
            if (! isZero()) {
                if (scalar == 0)
                    init();
                else
                    for (auto it = term_ + minExp_ - base_;
                            it <= term_ + maxExp_ - base_; ++it)
                        (*it) *= scalar;
            }
            return *this;
        }

        /**
         * Multiplies this polynomial by the given integer constant.
         *
         * The extent to which this operator can handle large native
         * integer types without overflow will depend on how well such integer
         * types are supported by the coefficient type \a T.
         *
         * \python It is assumed that the type \a IntType is \c long.
         *
         * \param scalar the scalar factor to multiply by.
         * \return a reference to this polynomial.
         */
        template <CppInteger IntType>
        Laurent& operator *= (IntType scalar) {
            if (! isZero()) {
                if (scalar == 0)
                    init();
                else
                    for (auto it = term_ + minExp_ - base_;
                            it <= term_ + maxExp_ - base_; ++it)
                        (*it) *= scalar;
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
        Laurent& operator /= (const T& scalar) {
            if (! isZero()) {
                for (auto it = term_ + minExp_ - base_;
                        it <= term_ + maxExp_ - base_; ++it)
                    (*it) /= scalar;

                // We could have zeroed out some coefficients.
                fixDegrees();
            }
            return *this;
        }

        /**
         * Divides this polynomial by the given integer constant.
         *
         * This uses the division operator `/=` for the coefficient type \a T.
         *
         * The extent to which this operator can handle large native
         * integer types without overflow will depend on how well such integer
         * types are supported by the coefficient type \a T.
         *
         * \pre The argument \a scalar is non-zero.
         *
         * \python It is assumed that the type \a IntType is \c long.
         *
         * \param scalar the scalar factor to divide by.
         * \return a reference to this polynomial.
         */
        template <CppInteger IntType>
        Laurent& operator /= (IntType scalar) {
            if (! isZero()) {
                for (auto it = term_ + minExp_ - base_;
                        it <= term_ + maxExp_ - base_; ++it)
                    (*it) /= scalar;

                // We could have zeroed out some coefficients.
                fixDegrees();
            }
            return *this;
        }

        /**
         * Adds the given polynomial to this.
         *
         * \param other the polynomial to add to this.
         * \return a reference to this polynomial.
         */
        Laurent& operator += (const Laurent& other) {
            // This routine works even if &other == this, since in this case
            // we do not reallocate.

            if (other.isZero())
                return *this;
            if (isZero())
                return *this = other;

            reallocateForRange(other.minExp_, other.maxExp_);

            for (long exp = other.minExp_; exp <= other.maxExp_; ++exp)
                term_[exp - base_] += other.term_[exp - other.base_];

            // We might have zeroed out some coefficients.
            fixDegrees();
            return *this;
        }

        /**
         * Adds the given polynomial to this.
         *
         * \param other the polynomial to add to this.
         * \return a reference to this polynomial.
         */
        Laurent& operator += (Laurent&& other) {
            if (other.isZero())
                return *this;
            if (isZero())
                return *this = std::move(other);

            if ((base_ <= other.minExp_ && other.maxExp_ < static_cast<long>(
                        base_ + capacity_)) ||
                    minExp_ < other.base_ ||
                    maxExp_ >= static_cast<long>(
                        other.base_ + other.capacity_)) {
                // Either: (a) we can avoid the deep copy by adding other to
                // this; or (b) we cannot avoid the deep copy either way.
                reallocateForRange(other.minExp_, other.maxExp_);

                auto src = other.term_ + other.minExp_ - other.base_;
                auto dest = term_ + other.minExp_ - base_;
                for ( ; dest <= term_ + other.maxExp_ - base_; ++src, ++dest)
                    *dest += std::move(*src);

                // We might have zeroed out some coefficients.
                fixDegrees();
                return *this;
            } else {
                // We can avoid the deep copy by adding this to other and
                // moving the result back into this.
                other.reallocateForRange(minExp_, maxExp_);

                auto src = term_ + minExp_ - base_;
                auto dest = other.term_ + minExp_ - other.base_;
                for ( ; src <= term_ + maxExp_ - base_; ++src, ++dest)
                    *dest += std::move(*src);

                // We might have zeroed out some coefficients.
                other.fixDegrees();
                return *this = std::move(other);
            }
        }

        /**
         * Subtracts the given polynomial from this.
         *
         * \param other the polynomial to subtract from this.
         * \return a reference to this polynomial.
         */
        Laurent& operator -= (const Laurent& other) {
            // This routine works even if &other == this, since in this case
            // we do not reallocate.

            if (other.isZero())
                return *this;
            if (isZero()) {
                *this = other;
                negate();
                return *this;
            }

            reallocateForRange(other.minExp_, other.maxExp_);

            for (long exp = other.minExp_; exp <= other.maxExp_; ++exp)
                term_[exp - base_] -= other.term_[exp - other.base_];

            // We might have zeroed out some coefficients.
            fixDegrees();
            return *this;
        }

        /**
         * Subtracts the given polynomial from this.
         *
         * \param other the polynomial to subtract from this.
         * \return a reference to this polynomial.
         */
        Laurent& operator -= (Laurent&& other) {
            if (other.isZero())
                return *this;
            if (isZero()) {
                other.negate();
                return *this = std::move(other);
            }

            if ((base_ <= other.minExp_ && other.maxExp_ < static_cast<long>(
                        base_ + capacity_)) ||
                    minExp_ < other.base_ ||
                    maxExp_ >= static_cast<long>(
                        other.base_ + other.capacity_)) {
                // Either: (a) we can avoid the deep copy by subtracting other
                // from this; or (b) we cannot avoid the deep copy either way.
                reallocateForRange(other.minExp_, other.maxExp_);

                auto src = other.term_ + other.minExp_ - other.base_;
                auto dest = term_ + other.minExp_ - base_;
                for ( ; dest <= term_ + other.maxExp_ - base_; ++src, ++dest)
                    *dest -= std::move(*src);

                // We might have zeroed out some coefficients.
                fixDegrees();
                return *this;
            } else {
                // We can avoid the deep copy by doing the computation in-place
                // within other, and then moving the result back into this.
                other.reallocateForRange(minExp_, maxExp_);

                auto src = term_ + minExp_ - base_;
                auto dest = other.term_ + other.minExp_ - other.base_;
                for ( ; dest != other.term_ + minExp_ - other.base_; ++dest)
                    dest->negate();
                for ( ; src <= term_ + maxExp_ - base_; ++src, ++dest)
                    *dest = std::move(*src) - std::move(*dest);
                for ( ; dest <= other.term_ + other.maxExp_ - other.base_;
                        ++dest)
                    dest->negate();

                // We might have zeroed out some coefficients.
                other.fixDegrees();
                return *this = std::move(other);
            }
        }

        /**
         * Multiplies this by the given polynomial.
         *
         * \param other the polynomial to multiply this by.
         * \return a reference to this polynomial.
         */
        Laurent& operator *= (const Laurent& other) {
            if (isZero())
                return *this;
            if (other.isZero()) {
                init();
                return *this;
            }
            if (other.minExp_ == other.maxExp_) {
                // We can get away without reallocating here.
                // Also: things should work even if &other == this, since in
                // such a scenario there is only one product of terms (which
                // becomes single *= single).
                const auto& single = other.term_[other.minExp_ - other.base_];
                for (auto it = term_ + minExp_ - base_;
                        it <= term_ + maxExp_ - base_; ++it)
                    (*it) *= single;
                // We adjust minExp_ last, because if &other == this then
                // changing minExp_ will change other.minExp_ also.
                maxExp_ += other.minExp_;
                base_ += other.minExp_;
                minExp_ += other.minExp_;
                return *this;
            }
            if (minExp_ == maxExp_) {
                // TODO: This case could benefit from an rvalue ref argument.
                // We may need to reallocate, but the product itself is simple.
                // Note: here it is logically impossible to have &other == this.
                if (capacity_ >
                        static_cast<size_t>(other.maxExp_ - other.minExp_)) {
                    // Extract the single term, then overwrite the term_ array.
                    const Term single = term_[minExp_ - base_];
                    auto out = term_;
                    for (auto in = other.term_ + other.minExp_ - other.base_;
                            in <= other.term_ + other.maxExp_ - other.base_;
                            ++in)
                        *out++ = (*in) * single;
                } else {
                    capacity_ = std::max(capacity_ + growth(),
                        static_cast<size_t>(other.maxExp_ - other.minExp_ + 1));
                    Term* newTerm = new Term[capacity_];
                    const Term& single = term_[minExp_ - base_];
                    auto out = newTerm;
                    for (auto in = other.term_ + other.minExp_ - other.base_;
                            in <= other.term_ + other.maxExp_ - other.base_;
                            ++in)
                        *out++ = (*in) * single;
                    delete[] term_;
                    term_ = newTerm;
                }
                minExp_ += other.minExp_;
                maxExp_ += other.maxExp_;
                base_ = minExp_;
                return *this;
            }

            // From here on, we know that both polynomials have more than one
            // non-zero term.

            // The following code works even if &other == this, since we build
            // the terms of the product in a separate section of memory.
            // TODO: Can we reuse our own memory if capacity_ is large enough?
            // (If we do, be sure to remember the case &other == this.)
            capacity_ = maxExp_ - minExp_ + other.maxExp_ - other.minExp_ + 1;
            Term* newTerm = new Term[capacity_];
            detail::productBest<Term, detail::SetOrAdd::Either, false>(newTerm,
                term_ + minExp_ - base_, maxExp_ - minExp_ + 1,
                other.term_ + other.minExp_ - other.base_,
                    other.maxExp_ - other.minExp_ + 1,
                nullptr /* scratch */);
            minExp_ += other.minExp_;
            maxExp_ += other.maxExp_;
            base_ = minExp_;

            delete[] term_;
            term_ = newTerm;
            return *this;
        }

        /**
         * Adds the product of the two given polynomials to this.
         * This is a common operation in (for example) inner products
         * and matrix multiplication.
         *
         * Calling `z.addProduct(x, y)` is equivalent to, but often faster
         * than, calling `z += x * y`.
         *
         * \param x the first polynomial in the product to add to this.
         * \param y the second polynomial in the product to add to this.
         */
        void addProduct(const Laurent& x, const Laurent& y) {
            if (x.isZero() || y.isZero()) {
                return;
            } else if (std::addressof(x) == this || std::addressof(y) == this) {
                // Here we _need_ a temporary to hold x * y.
                *this += x * y;
                // We could speed this up perhaps by implementing
                // *this *= (y + 1) or *this *= (x + 1).  However, this seems
                // to be a very niche case and so we leave it for now.
            } else {
                // At this point, we know that both x and y have at least one
                // non-zero coefficient.
                //
                // The following line ensures that term_ becomes non-null.
                reallocateForRange(x.minExp_ + y.minExp_,
                    x.maxExp_ + y.maxExp_);

                detail::productBest<Term, detail::SetOrAdd::Add, false>(
                    term_ + x.minExp_ + y.minExp_ - base_,
                    x.term_ + x.minExp_ - x.base_, x.maxExp_ - x.minExp_ + 1,
                    y.term_ + y.minExp_ - y.base_, y.maxExp_ - y.minExp_ + 1,
                    nullptr /* scratch */);

                // We might have zeroed out some coefficients.
                fixDegrees();
            }
        }

        /**
         * Adds the product of the two given polynomials to this.
         * This is a common operation in (for example) inner products
         * and matrix multiplication.
         *
         * Calling `z.addProduct(x, y)` is equivalent to, but often faster
         * than, calling `z += x * y`.
         *
         * \param x the first polynomial in the product to add to this.
         * \param y the second polynomial in the product to add to this.
         */
        void addProduct(Laurent&& x, Laurent&& y) {
            if (x.isZero() || y.isZero()) {
                return;
            } else if (std::addressof(x) == this || std::addressof(y) == this) {
                // Here we _need_ a temporary to hold x * y.
                *this += x * y;
                // We could speed this up perhaps by implementing
                // *this *= (y + 1) or *this *= (x + 1).  However, this seems
                // to be a very niche case and so we leave it for now.
            } else {
                // At this point, we know that both x and y have at least one
                // non-zero coefficient.
                //
                // The following line ensures that term_ becomes non-null.
                reallocateForRange(x.minExp_ + y.minExp_,
                    x.maxExp_ + y.maxExp_);

                detail::productBest<Term, detail::SetOrAdd::Add, true>(
                    term_ + x.minExp_ + y.minExp_ - base_,
                    x.term_ + x.minExp_ - x.base_, x.maxExp_ - x.minExp_ + 1,
                    y.term_ + y.minExp_ - y.base_, y.maxExp_ - y.minExp_ + 1,
                    nullptr /* scratch */);

                // We might have zeroed out some coefficients.
                fixDegrees();
            }
        }

        /**
         * Adds the product of the given polynomial with the given scalar
         * to this.
         *
         * Calling `z.addProduct(x, scalar)` is equivalent to, but often faster
         * than, calling `z += x * scalar`.
         *
         * \param x the polynomial in the product to add to this.
         * \param scalar the scalar in the product to add to this.
         */
        void addProduct(const Laurent& x, const T& scalar) {
            if (x.isZero() || scalar == 0) {
                return;
            } else {
                // The following line ensures that term_ becomes non-null.
                reallocateForRange(x.minExp_, x.maxExp_);

                for (long i = x.minExp_; i <= x.maxExp_; ++i)
                    if constexpr (vars > 1 || HasAddProduct<T>)
                        term_[i - base_].addProduct(x.term_[i - x.base_],
                            scalar);
                    else
                        term_[i - base_] += x.term_[i - x.base_] * scalar;

                // We might have zeroed out some coefficients.
                fixDegrees();
            }
        }

        /**
         * Adds the product of the given polynomial with the given scalar
         * to this.
         *
         * Calling `z.addProduct(x, scalar)` is equivalent to, but often faster
         * than, calling `z += x * scalar`.
         *
         * \param x the polynomial in the product to add to this.
         * \param scalar the scalar in the product to add to this.
         */
        void addProduct(Laurent&& x, const T& scalar) {
            if (x.isZero() || scalar == 0) {
                return;
            } else {
                // The following line ensures that term_ becomes non-null.
                reallocateForRange(x.minExp_, x.maxExp_);

                for (long i = x.minExp_; i <= x.maxExp_; ++i)
                    if constexpr (vars > 1 || HasAddProduct<T>)
                        term_[i - base_].addProduct(
                            std::move(x.term_[i - x.base_]), scalar);
                    else
                        term_[i - base_] +=
                            std::move(x.term_[i - x.base_]) * scalar;

                // We might have zeroed out some coefficients.
                fixDegrees();
            }
        }

        /**
         * Converts a single-variable Laurent polynomial into one of Regina's
         * Polynomial objects.
         *
         * This routine returns a pair `(p, exp)`, where \a p is of type
         * `Polynomial<T>`, and where this Laurent polynomial is equivalent to
         * `x^exp p(x)`.
         *
         * If this polynomial is non-zero, then \a exp will be precisely
         * minExp(), which means that the resulting polynomial \a p will have
         * a non-zero constant term.  If this polynomial is zero, then both
         * \a p and \a exp will be zero also.
         *
         * \return the pair `(p, exp)`, as described above.
         */
        std::pair<Polynomial<T>, long> polynomial() const requires (vars == 1) {
            if (isZero())
                return {{}, 0};
            else
                return {{term_ + minExp_ - base_, term_ + maxExp_ - base_ + 1},
                    minExp_};
        }

        /**
         * Writes this polynomial to the given output stream, using the
         * given variable names instead of `x,y,…`.
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
         * \param variables a sequence of \a vars C-style strings to use for
         * the polynomial variables.  Any (or all) of these may be `null`, in
         * which case the defaults `x,y,…` will be used for those variable(s).
         */
        template <std::convertible_to<const char*>... Names>
        requires (sizeof...(Names) == vars)
        void writeTextShort(std::ostream& out, bool utf8 = false,
                Names... variables) const {
            if (isZero()) {
                out << '0';
                return;
            }

            std::array<const char*, vars> names{variables...};

            bool started = false;
            for (auto c = last(); c; --c) {
                T writeCoeff = *c; // TODO: deep copy, so we can negate

                if (started) {
                    if (writeCoeff < 0) {
                        if (utf8)
                            out << " \u2212 ";
                        else
                            out << " - ";
                        writeCoeff.negate();
                    } else
                        out << " + ";
                } else {
                    // This is the first term being output.
                    if (writeCoeff < 0) {
                        if (utf8)
                            out << "\u2212";
                        else
                            out << '-';
                        writeCoeff.negate();
                    }
                    started = true;
                }

                // From here, it is guaranteed that writeCoeff > 0.
                if (c.constantTerm())
                    out << writeCoeff;
                else {
                    if (writeCoeff != 1)
                        out << writeCoeff << ' ';

                    if constexpr (vars == 1) {
                        if (names[0])
                            out << names[0];
                        else
                            out << 'x';
                        if (c.template exp<0>() != 1) {
                            if (utf8)
                                out << regina::superscript(c.template exp<0>());
                            else
                                out << '^' << c.template exp<0>();
                        }
                    } else {
                        bool startedPower = false;
                        auto exp = c.exponents();

                        for (int i = 0; i < vars; ++i) {
                            if (exp[i] != 0) {
                                if (startedPower)
                                    out << ' ';
                                else
                                    startedPower = true;

                                if (names[i])
                                    out << names[i];
                                else
                                    out << char('x' + i); // default name: x,y,z

                                if (exp[i] != 1) {
                                    if (utf8)
                                        out << regina::superscript(exp[i]);
                                    else
                                        out << '^' << exp[i];
                                }
                            }
                        }
                    }
                }
            }
        }

        /**
         * Writes this polynomial to the given output stream, using the
         * default variable names `x,y,…`.
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
         */
        void writeTextShort(std::ostream& out, bool utf8 = false) const {
            if constexpr (vars == 1)
                writeTextShort(out, utf8, nullptr);
            else if constexpr (vars == 2)
                writeTextShort(out, utf8, nullptr, nullptr);
            else // vars == 3
                writeTextShort(out, utf8, nullptr, nullptr, nullptr);
        }

        /**
         * Returns this polynomial as a human-readable string, using the
         * given variable names instead of `x,y,…`.
         *
         * \note There is also the usual variant of str() which takes no
         * arguments; that variant is inherited from the Output class.
         *
         * \param variables a sequence of \a vars C-style strings to use for
         * the polynomial variables.  Any (or all) of these may be `null`, in
         * which case the defaults `x,y,…` will be used for those variable(s).
         * \return this polynomial as a human-readable string.
         */
        template <std::convertible_to<const char*>... Names>
        requires (sizeof...(Names) == vars)
        std::string str(Names... variables) const {
            std::ostringstream out;
            writeTextShort(out, false, variables...);
            return std::move(out).str();
        }

        /**
         * Returns this polynomial as a human-readable string using unicode
         * characters, using the given variable names instead of `x,y,…`.
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
         * \param variables a sequence of \a vars C-style strings to use for
         * the polynomial variables.  Any (or all) of these may be `null`, in
         * which case the defaults `x,y,…` will be used for those variable(s).
         * \return this polynomial as a unicode-enabled human-readable string.
         */
        template <std::convertible_to<const char*>... Names>
        requires (sizeof...(Names) == vars)
        std::string utf8(Names... variables) const {
            std::ostringstream out;
            writeTextShort(out, true, variables...);
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
            // Write the non-zero coefficients with their exponents, and then
            // terminate with zero.  We write the coefficient before the
            // exponents so we can use zero as an unambiguous terminator.
            for (auto it = first(); it; ++it) {
                it->tightEncode(out);
                for (long e : it.exponents())
                    regina::tightEncode(out, e);
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
         * a tight encoding of a single-variable Laurent polynomial.
         *
         * \nopython Use tightDecoding() instead, which takes a string as
         * its argument.
         *
         * \param input an input stream that begins with the tight encoding
         * for a single-variable Laurent polynomial.
         * \return the polynomial represented by the given tight encoding.
         */
        static Laurent tightDecode(std::istream& input)
                requires InherentlyTightEncodable<T> {
            // For now, just set coefficients directly as we read them.
            // The old implementation used a temporary std::vector, but now
            // that Laurent manages growth in a similar way to std::vector we
            // might as well just store them directly.
            Laurent ans;
            std::array<long, vars> exp[2];
            int prev = -1, curr = 0;
            while (true) {
                T coeff = T::tightDecode(input);
                if (coeff == 0) {
                    // The sequence of coefficients is finished.
                    return ans;
                } else {
                    for (auto& e : exp[curr])
                        e = regina::tightDecode<long>(input);
                    if (prev >= 0 && exp[curr] <= exp[prev])
                        throw InvalidInput("The tight encoding uses "
                            "non-increasing exponent sequences");
                    prev = curr;
                    if constexpr (vars == 1)
                        ans.setTo(std::move(coeff), exp[curr][0]);
                    else if constexpr (vars == 2)
                        ans.setTo(std::move(coeff), exp[curr][0], exp[curr][1]);
                    else // vars == 3
                        ans.setTo(std::move(coeff), exp[curr][0], exp[curr][1],
                            exp[curr][2]);
                    curr ^= 1;
                }
            }
        }

        /**
         * A diagnostic routine that ensures that the internal representation
         * of this polynomial is valid.
         *
         * If Regina is working correctly, this routine should do nothing.
         * If the internal representation of this polynomial is _not_ valid,
         * this routine will throw an exception.
         *
         * This routine is provided for use within Regina's various test suites.
         * End users should not need to call it at all.
         *
         * \exception ImpossibleScenario The internal state of this polynomial
         * is invalid.
         */
        void validate() const {
            if (minExp_ > maxExp_) {
                // This is the zero polynomial.
                // There are no further restrictions on minExp_, maxExp_, or
                // base_.
                if (term_ && capacity_ == 0)
                    throw ImpossibleScenario(
                        "Non-null terms array has zero capacity");
            } else {
                if (! term_)
                    throw ImpossibleScenario(
                        "Non-zero polynomial has null terms array");
                if (capacity_ == 0)
                    throw ImpossibleScenario(
                        "Non-null terms array has zero capacity");
                if (minExp_ < base_)
                    throw ImpossibleScenario(
                        "Minimum exponent lies outside the terms array");
                if (maxExp_ >= base_ + static_cast<long>(capacity_))
                    throw ImpossibleScenario(
                        "Maximum exponent lies outside the terms array");
                if (term_[minExp_ - base_] == 0)
                    throw ImpossibleScenario(
                        "Minimum exponent has zero term");
                if (term_[maxExp_ - base_] == 0)
                    throw ImpossibleScenario(
                        "Maximum exponent has zero term");

                if constexpr (CanValidate<T>) {
                    // Recursively validate the individual terms.
                    for (auto it = term_ + minExp_ - base_;
                            it <= term_ + maxExp_ - base_; ++it)
                        it->validate();
                } else {
                    // Just a sanity check on the validation concept, to ensure
                    // the recursive validation is not being silently skipped.
                    static_assert(vars == 1);
                    static_assert(! std::is_same_v<T, Integer>);
                }
            }
        }

    private:
        /**
         * Constructs a new polynomial with the given exponent range and
         * terms.  It is assumed that the array of terms starts at exponent
         * \a minExp, and has capacity `maxExp - minExp + 1`.
         * The new object will take ownership of the given array.
         *
         * The array of terms may have leading or trailing zeroes (or indeed
         * may consist entirely of zeroes), but if this is a possibility
         * then you _must_ pass \a checkZeroes as `true`.
         *
         * \pre The arguments satisfy `minExp ≤ maxExp`.
         *
         * \pre The argument \a terms is both non-null and non-empty.
         */
        Laurent(long minExp, long maxExp, Term* terms,
                bool checkZeroes = false) :
                minExp_(minExp), maxExp_(maxExp), base_(minExp),
                capacity_(maxExp - minExp + 1), term_(terms) {
            // The preconditions guarantee term_ is non-null and non-empty.
            if (checkZeroes)
                fixDegrees();
        }

        /**
         * Ensures that `minExp_ ≤ newMin` and `maxExp_ ≥ newMax`, and
         * grows the array of terms accordingly if this is necessary.
         * As a result, the terms array will be non-empty (and in
         * particular, \a term_ will be non-null).
         *
         * If this is a non-zero polynomial, then the value of \a minExp_
         * might decrease, but it will not increase; likewise, the value of
         * \a maxExp_ might increase, but it will not decrease.  Moreover,
         * if \a minExp_ and/or \a maxExp_ do change, then all new terms
         * in the expanded range will be set to zero.
         *
         * If this is the zero polynomial, then \a minExp_ and \a maxExp_ will
         * be set to \a newMin and \a newMax respectively.
         *
         * Note that, after calling this routine, the polynomial might have
         * zero terms at the exponents \a minExp_ and/or \a maxExp_.
         *
         * This routine is used (for example) in the implementations
         * of += and -=.
         *
         * \pre The given range satisfies `newMin ≤ newMax`.
         */
        void reallocateForRange(long newMin, long newMax) {
            // Note: the Term type will automatically initialise any newly
            // allocated terms to zero.
            if (! term_) {
                minExp_ = base_ = newMin;
                maxExp_ = newMax;
                capacity_ = newMax - newMin + 1;
                term_ = new Term[capacity_];
            } else if (isZero()) {
                // This is the zero polynomial, but we already have an
                // allocated terms array.
                size_t rangeLen = newMax - newMin + 1;
                if (rangeLen > capacity_) {
                    // We are going to have to reallocate.
                    capacity_ = std::max(capacity_ + growth(), rangeLen);
                    delete[] term_;
                    term_ = new Term[capacity_];

                    // Try to locate the range in the middle of the new array,
                    // with space on either side.
                    minExp_ = newMin;
                    maxExp_ = newMax;
                    base_ = newMin - ((capacity_ - rangeLen) >> 1);
                } else {
                    // We don't need to reallocate, but we will need to zero
                    // out any pre-existing data in our range.
                    minExp_ = newMin;
                    maxExp_ = newMax;
                    base_ = newMin - ((capacity_ - rangeLen) >> 1);
                    std::fill(term_ + minExp_ - base_,
                        term_ + maxExp_ - base_ + 1, 0);
                }
            } else if (newMin >= minExp_ && newMax <= maxExp_) {
                // We have nothing to do.
                return;
            } else {
                long finalMin = std::min(minExp_, newMin);
                long finalMax = std::max(maxExp_, newMax);
                if (static_cast<size_t>(finalMax - finalMin) >= capacity_) {
                    // We are going to have to reallocate.
                    size_t extra = std::max(growth(),
                        static_cast<size_t>(finalMax - finalMin
                            - capacity_ + 1));
                    capacity_ += extra;

                    // Try to locate the new data in the middle of the new
                    // array, with space on either side.
                    size_t newBase = finalMin - (static_cast<size_t>(
                        capacity_ - (finalMax - finalMin + 1)) >> 1);

                    Term* newTerm = new Term[capacity_];
                    std::move(term_ + minExp_ - base_,
                        term_ + maxExp_ - base_ + 1,
                        newTerm + minExp_ - newBase);
                    delete[] term_;
                    term_ = newTerm;

                    minExp_ = finalMin;
                    maxExp_ = finalMax;
                    base_ = newBase;
                } else {
                    // We don't need to reallocate, but we might need to shift.
                    // We measure our shifts to the right (so a positive shift
                    // corresponds to a reduction in base_).
                    // If we _do_ need to shift, do this in a way that leaves
                    // some space on either side.
                    long minShift = base_ - finalMin;
                    long maxShift = base_ + static_cast<long>(capacity_)
                        - finalMax - 1;
                    if (minShift > 0) {
                        // We must shift our data to the right.
                        long shift = (minShift + maxShift + 1) >> 1;
                        std::move_backward(term_ + minExp_ - base_,
                            term_ + maxExp_ - base_ + 1,
                            term_ + maxExp_ - base_ + shift + 1);
                        base_ -= shift;
                    } else if (maxShift < 0) {
                        // We must shift out data to the left.
                        long shift = (-(minShift + maxShift) + 1) >> 1;
                        std::move(term_ + minExp_ - base_,
                            term_ + maxExp_ - base_ + 1,
                            term_ + minExp_ - base_ - shift);
                        base_ += shift;
                    }

                    // Fix minExp_ and maxExp_, and zero out any extra terms
                    // on either end.
                    if (newMin < minExp_) {
                        for (auto it = term_ + newMin - base_;
                                it != term_ + minExp_ - base_; ++it)
                            *it = 0;
                        minExp_ = newMin;
                    }
                    if (newMax > maxExp_) {
                        for (auto it = term_ + maxExp_ - base_ + 1;
                                it <= term_ + newMax - base_; ++it)
                            *it = 0;
                        maxExp_ = newMax;
                    }
                }
            }
        }

        /**
         * Increases \a minExp_ and/or decreases \a maxExp_ to ensure that
         * both exponents \a minExp_ and \a maxExp_ have non-zero terms.
         * If this is the zero polynomial then \a minExp_ and \a maxExp_ will
         * be left in a state that correctly encodes this.
         *
         * \pre The data member \a term_ is non-null.
         */
        void fixDegrees() {
            while (minExp_ <= maxExp_ && term_[maxExp_ - base_] == 0)
                --maxExp_;
            while (minExp_ <= maxExp_ && term_[minExp_ - base_] == 0)
                ++minExp_;
            // If we have the zero polynomial, we will be left with
            // minExp_ < maxExp_, which is exactly what we need.
        }

        /**
         * Changes the given coefficient of this polynomial.
         *
         * Specifically, for a polynomial \a p, calling `p.setTo(value,a,b,…)`
         * will set the coefficient of `x^a y^b …` to \a value, where `x,y,…`
         * are the polynomial variables.  There are no restrictions on the
         * exponents `a,b,…`, and the new coefficient \a value may be zero.
         *
         * The only difference between setTo() and set() is the order of
         * arguments: in setTo() the value comes first, since this greatly
         * simplifies the recursive C++ template implementation.  We therefore
         * use setTo() as a unified implementation of the various set()
         * routines, which must be implemented separately for each value of
         * \a vars.  If Regina ever moves to a C++ standard with easier
         * handling of template parameter packs, it may be possible to simply
         * provide a unified implementation for set() instead.
         *
         * \param value the new value to assign to the given coefficient.
         * \param firstExp the exponent of the first polynomial variable \a x
         * for the coefficient that we should set.
         * \param otherExp the exponents of the remaining `(vars - 1)`
         * polynomial variables `y,…` for the coefficient that we should set.
         */
        template <std::convertible_to<long>... Exponents>
        requires (sizeof...(Exponents) == vars - 1)
        void setTo(const T& value, long firstExp, Exponents&&... otherExp) {
            // TODO: We should probably offer a T&& variant also.
            if (value == 0) {
                if (isZero() || firstExp < minExp_ || firstExp > maxExp_)
                    return;

                // Separate out implementations for vars == 1 vs vars > 1,
                // since for vars == 1 this will erase the entire term but
                // for vars > 1 it might not.
                if constexpr (vars == 1) {
                    if (firstExp == maxExp_) {
                        if (minExp_ == maxExp_) {
                            // This becomes the zero polynomial.
                            init();
                        } else {
                            --maxExp_;
                            // We know the lowest-exponent coefficient != 0.
                            while (term_[maxExp_ - base_] == 0)
                                --maxExp_;
                        }
                    } else if (firstExp == minExp_) {
                        // Here we know the highest-exponent coefficient != 0.
                        ++minExp_;
                        while (term_[minExp_ - base_] == 0)
                            ++minExp_;
                    } else {
                        // We are setting an interior coefficient.
                        term_[firstExp - base_] = 0;
                    }
                } else {
                    // Our term is within the [minExp_, maxExp_] range.
                    // Set the coefficient to zero now, and then - only if it
                    // might matter - see whether the entire term was erased.
                    term_[firstExp - base_].setTo(value,
                        std::forward<Exponents>(otherExp)...);

                    if (firstExp == maxExp_) {
                        // Put the zero-coefficient test in a separate `if`
                        // statement so that, if minExp_ == maxExp_, we do not
                        // test it twice.  (We do not know how expensive
                        // zero-testing is for the coefficient type T.)
                        if (term_[firstExp - base_] == 0) {
                            if (minExp_ == maxExp_) {
                                // This becomes the zero polynomial.
                                init();
                            } else {
                                // We know the lowest-exponent coefficient != 0.
                                --maxExp_;
                                while (term_[maxExp_ - base_] == 0)
                                    --maxExp_;
                            }
                        }
                    } else if (firstExp == minExp_ &&
                            term_[firstExp - base_] == 0) {
                        // Here we know the highest-exponent coefficient != 0.
                        ++minExp_;
                        while (term_[minExp_ - base_] == 0)
                            ++minExp_;
                    }
                }
                return;
            }

            // From here, the value we are assigning is non-zero.

            // We first ensure that term_[firstExp - base_] is allocated and
            // initialised to zero if necessary, and that minExp_ and maxExp_
            // are adjusted to reflect the fact that `this[firstExp]` will
            // become non-zero.  We then finish by setting the actual value.
            if (isZero()) {
                // Reuse any pre-allocated term_ array, if we can.
                if (term_) {
                    if (firstExp < base_ ||
                            firstExp >= base_ + static_cast<long>(capacity_)) {
                        // Shift the range to cover the given exponent.
                        size_t gap = capacity_ >> 1; // 0 ≤ gap < capacity_
                        base_ = firstExp - gap;
                    }
                } else {
                    capacity_ = 1;
                    term_ = new Term[1];
                    base_ = firstExp;
                }
                minExp_ = maxExp_ = firstExp;
            } else if (firstExp < base_) {
                // The minimum exponent decreases, and we must reallocate.
                size_t extra = std::max(growth(),
                    static_cast<size_t>(base_ - firstExp));
                capacity_ += extra;

                Term* newTerm = new Term[capacity_];
                std::move(term_ + (minExp_ - base_),
                    term_ + (maxExp_ - base_) + 1,
                    newTerm + (minExp_ - base_ + extra));
                base_ -= extra;
                minExp_ = firstExp;

                delete[] term_;
                term_ = newTerm;
            } else if (firstExp < minExp_) {
                // The minimum exponent decreases, but we need not reallocate.
                for (--minExp_; minExp_ > firstExp; --minExp_)
                    term_[minExp_ - base_] = 0;
                if constexpr (vars > 1)
                    term_[firstExp - base_] = 0;
            } else if (firstExp >= base_ + static_cast<long>(capacity_)) {
                // The maximum exponent increases, and we must reallocate.
                size_t extra = std::max(growth(),
                    static_cast<size_t>(firstExp - base_ - capacity_ + 1));
                capacity_ += extra;

                Term* newTerm = new Term[capacity_];
                std::move(term_ + (minExp_ - base_),
                    term_ + (maxExp_ - base_) + 1,
                    newTerm + (minExp_ - base_));
                maxExp_ = firstExp;

                delete[] term_;
                term_ = newTerm;
            } else if (firstExp > maxExp_) {
                // The maximum exponent increases, but we need not reallocate.
                for (++maxExp_; maxExp_ < firstExp; ++maxExp_)
                    term_[maxExp_ - base_] = 0;
                if constexpr (vars > 1)
                    term_[firstExp - base_] = 0;
            }

            if constexpr (vars == 1)
                term_[firstExp - base_] = value;
            else
                term_[firstExp - base_].setTo(value,
                    std::forward<Exponents>(otherExp)...);
        }

        /**
         * Replaces the contents of this polynomial with \a other - \a this.
         *
         * This is equivalent to calling the -= operator and then negating.
         *
         * \param other the polynomial to subtract this from.
         * \return a reference to this polynomial.
         */
        Laurent& subtractFrom(const Laurent& other) {
            // This routine works even if &other == this, since in this case
            // we do not reallocate.

            if (other.isZero()) {
                negate();
                return *this;
            }
            if (isZero()) {
                return *this = other;
            }

            reallocateForRange(other.minExp_, other.maxExp_);

            long exp = (minExp_ < other.minExp_ ? minExp_ : other.minExp_);
            for ( ; exp < other.minExp_; ++exp)
                if (term_[exp - base_] != 0)
                    term_[exp - base_].negate();
            for ( ; exp <= other.maxExp_; ++exp)
                if (term_[exp - base_] != 0)
                    term_[exp - base_] = other.term_[exp - other.base_]
                        - std::move(term_[exp - base_]);
                else
                    term_[exp - base_] = other.term_[exp - other.base_];
            for ( ; exp <= maxExp_; ++exp)
                if (term_[exp - base_] != 0)
                    term_[exp - base_].negate();

            // We might have zeroed out some coefficients.
            fixDegrees();
            return *this;
        }

    friend class LaurentCoefficient<T, vars>;

    template <CoefficientDomain U, int v> requires (v > 0 && v <= 3)
    friend Laurent<U, v> operator + (const Laurent<U, v>&,
        const Laurent<U, v>&);

    template <CoefficientDomain U, int v> requires (v > 0 && v <= 3)
    friend Laurent<U, v> operator - (const Laurent<U, v>&,
        const Laurent<U, v>&);

    template <CoefficientDomain U, int v> requires (v > 0 && v <= 3)
    friend Laurent<U, v> operator - (const Laurent<U, v>&, Laurent<U, v>&&);

    template <CoefficientDomain U, int v> requires (v > 0 && v <= 3)
    friend Laurent<U, v> operator * (const Laurent<U, v>&,
        const Laurent<U, v>&);

    template <CoefficientDomain U, int v> requires (v > 0 && v <= 3)
    friend Laurent<U, v> operator * (Laurent<U, v>&&, Laurent<U, v>&&);

    // Give Laurent<T, v> access to the internals of its terms Laurent<T, v-1>.
    // This friend declaration is far too broad, but it means we do not
    // automatically instantiate more classes than we want.
    template <CoefficientDomain U, int v> requires (v > 0 && v <= 3)
    friend class Laurent;
};

/**
 * Swaps the contents of the given polynomials.
 *
 * This global routine simply calls Laurent<T, vars>::swap(); it is provided
 * so that Laurent<T, vars> meets the C++ Swappable requirements.
 *
 * \param a the first polynomial whose contents should be swapped.
 * \param b the second polynomial whose contents should be swapped.
 *
 * \ingroup maths
 */
template <CoefficientDomain T, int vars>
requires (vars > 0 && vars <= 3)
void swap(Laurent<T, vars>& a, Laurent<T, vars>& b) noexcept {
    a.swap(b);
}

/**
 * Multiplies the given polynomial by the given scalar constant.
 *
 * The scalar is simply of type \a T; we use the identical type
 * Laurent<T, vars>::Coefficient here to assist with C++ template type matching.
 *
 * \param poly the polynomial to multiply by.
 * \param scalar the scalar to multiply by.
 * \return the product of the given polynomial and scalar.
 *
 * \ingroup maths
 */
template <CoefficientDomain T, int vars>
requires (vars > 0 && vars <= 3)
Laurent<T, vars> operator * (Laurent<T, vars> poly,
        const typename Laurent<T, vars>::Coefficient& scalar) {
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
 * The extent to which this operator can handle large native
 * integer types without overflow will depend on how well such integer
 * types are supported by the coefficient type \a T.
 *
 * \python It is assumed that the type \a IntType is \c long.
 *
 * \param poly the polynomial to multiply by.
 * \param scalar the scalar to multiply by.
 * \return the product of the given polynomial and scalar.
 *
 * \ingroup maths
 */
template <CoefficientDomain T, int vars, CppInteger IntType>
requires (vars > 0 && vars <= 3)
Laurent<T, vars> operator * (Laurent<T, vars> poly, IntType scalar) {
    // See the notes above on a possible optimisation for scalar == 0.
    poly *= scalar;
    return poly;
}

/**
 * Multiplies the given polynomial by the given scalar constant.
 *
 * The scalar is simply of type \a T; we use the identical type
 * Laurent<T, vars>::Coefficient here to assist with C++ template type matching.
 *
 * \param scalar the scalar to multiply by.
 * \param poly the polynomial to multiply by.
 * \return the product of the given polynomial and scalar.
 *
 * \ingroup maths
 */
template <CoefficientDomain T, int vars>
requires (vars > 0 && vars <= 3)
Laurent<T, vars> operator * (
        const typename Laurent<T, vars>::Coefficient& scalar,
        Laurent<T, vars> poly) {
    // See the notes above on a possible optimisation for scalar == 0.
    poly *= scalar;
    return poly;
}

/**
 * Multiplies the given polynomial by the given scalar constant.
 *
 * The extent to which this operator can handle large native
 * integer types without overflow will depend on how well such integer
 * types are supported by the coefficient type \a T.
 *
 * \python It is assumed that the type \a IntType is \c long.
 *
 * \param scalar the scalar to multiply by.
 * \param poly the polynomial to multiply by.
 * \return the product of the given polynomial and scalar.
 *
 * \ingroup maths
 */
template <CoefficientDomain T, int vars, CppInteger IntType>
requires (vars > 0 && vars <= 3)
Laurent<T, vars> operator * (IntType scalar, Laurent<T, vars> poly) {
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
 * Laurent<T, vars>::Coefficient here to assist with C++ template type matching.
 *
 * \pre The argument \a scalar is non-zero.
 *
 * \param poly the polynomial to divide by the given scalar.
 * \param scalar the scalar factor to divide by.
 * \return the quotient of the given polynomial by the given scalar.
 *
 * \ingroup maths
 */
template <CoefficientDomain T, int vars>
requires (vars > 0 && vars <= 3)
Laurent<T, vars> operator / (Laurent<T, vars> poly,
        const typename Laurent<T, vars>::Coefficient& scalar) {
    poly /= scalar;
    return poly;
}

/**
 * Divides the given polynomial by the given scalar constant.
 *
 * This uses the division operator `/=` for the coefficient type \a T.
 *
 * The extent to which this operator can handle large native
 * integer types without overflow will depend on how well such integer
 * types are supported by the coefficient type \a T.
 *
 * \pre The argument \a scalar is non-zero.
 *
 * \python It is assumed that the type \a IntType is \c long.
 *
 * \param poly the polynomial to divide by the given scalar.
 * \param scalar the scalar factor to divide by.
 * \return the quotient of the given polynomial by the given scalar.
 *
 * \ingroup maths
 */
template <CoefficientDomain T, int vars, CppInteger IntType>
requires (vars > 0 && vars <= 3)
Laurent<T, vars> operator / (Laurent<T, vars> poly, IntType scalar) {
    poly /= scalar;
    return poly;
}

/**
 * Adds the two given polynomials.
 *
 * \param lhs the first polynomial to add.
 * \param rhs the second polynomial to add.
 * \return the sum of both polynomials.
 *
 * \ingroup maths
 */
template <CoefficientDomain T, int vars>
requires (vars > 0 && vars <= 3)
Laurent<T, vars> operator + (const Laurent<T, vars>& lhs,
        const Laurent<T, vars>& rhs) {
    // Handle zero polynomials separately.
    if (lhs.isZero())
        return rhs;
    if (rhs.isZero())
        return lhs;

    // If the two ranges do not overlap, just copy them separately;
    // this avoids having to worry about jumping over the gap between
    // them in the code that follows.
    if (lhs.maxExp_ < rhs.minExp_) {
        auto* terms = new Laurent<T, vars>::Term[rhs.maxExp_ - lhs.minExp_ + 1];
        std::copy(lhs.term_ + lhs.minExp_ - lhs.base_,
            lhs.term_ + lhs.maxExp_ + 1 - lhs.base_,
            terms);
        std::copy(rhs.term_ + rhs.minExp_ - rhs.base_,
            rhs.term_ + rhs.maxExp_ + 1 - rhs.base_,
            terms + rhs.minExp_ - lhs.minExp_);
        return Laurent<T, vars>(lhs.minExp_, rhs.maxExp_, terms);
    } else if (rhs.maxExp_ < lhs.minExp_) {
        auto* terms = new Laurent<T, vars>::Term[lhs.maxExp_ - rhs.minExp_ + 1];
        std::copy(rhs.term_ + rhs.minExp_ - rhs.base_,
            rhs.term_ + rhs.maxExp_ + 1 - rhs.base_,
            terms);
        std::copy(lhs.term_ + lhs.minExp_ - lhs.base_,
            lhs.term_ + lhs.maxExp_ + 1 - lhs.base_,
            terms + lhs.minExp_ - rhs.minExp_);
        return Laurent<T, vars>(rhs.minExp_, lhs.maxExp_, terms);
    }

    // We are guaranteed from here on that the two ranges overlap.
    long minExp = std::min(lhs.minExp_, rhs.minExp_);
    long maxExp = std::max(lhs.maxExp_, rhs.maxExp_);
    auto* terms = new Laurent<T, vars>::Term[maxExp - minExp + 1];

    long exp /* next exponent */, idx /* next index into terms */;

    if (lhs.minExp_ < rhs.minExp_) {
        std::copy(lhs.term_ + lhs.minExp_ - lhs.base_,
            lhs.term_ + rhs.minExp_ - lhs.base_, terms);
        exp = rhs.minExp_;
        idx = rhs.minExp_ - lhs.minExp_;
    } else if (rhs.minExp_ < lhs.minExp_) {
        std::copy(rhs.term_ + rhs.minExp_ - rhs.base_,
            rhs.term_ + lhs.minExp_ - rhs.base_, terms);
        exp = lhs.minExp_;
        idx = lhs.minExp_ - rhs.minExp_;
    } else {
        exp = lhs.minExp_;
        idx = 0;
    }

    for ( ; exp <= lhs.maxExp_ && exp <= rhs.maxExp_; ++idx, ++exp)
        terms[idx] = lhs.term_[exp - lhs.base_] + rhs.term_[exp - rhs.base_];

    // exp is now min(lhs.maxExp_, rhs.maxExp_) + 1.
    if (exp <= lhs.maxExp_) {
        std::copy(lhs.term_ + exp - lhs.base_,
            lhs.term_ + lhs.maxExp_ + 1 - lhs.base_, terms + idx);
    } else if (exp <= rhs.maxExp_) {
        std::copy(rhs.term_ + exp - rhs.base_,
            rhs.term_ + rhs.maxExp_ + 1 - rhs.base_, terms + idx);
    }

    return Laurent<T, vars>(minExp, maxExp, terms, true);
}

/**
 * Adds the two given polynomials.
 *
 * \param lhs the first polynomial to add.
 * \param rhs the second polynomial to add.
 * \return the sum of both polynomials.
 *
 * \ingroup maths
 */
template <CoefficientDomain T, int vars>
requires (vars > 0 && vars <= 3)
Laurent<T, vars> operator + (Laurent<T, vars>&& lhs,
        const Laurent<T, vars>& rhs) {
    return std::move(lhs += rhs);
}

/**
 * Adds the two given polynomials.
 *
 * \param lhs the first polynomial to add.
 * \param rhs the second polynomial to add.
 * \return the sum of both polynomials.
 *
 * \ingroup maths
 */
template <CoefficientDomain T, int vars>
requires (vars > 0 && vars <= 3)
Laurent<T, vars> operator + (const Laurent<T, vars>& lhs,
        Laurent<T, vars>&& rhs) {
    return std::move(rhs += lhs);
}

/**
 * Adds the two given polynomials.
 *
 * \param lhs the first polynomial to add.
 * \param rhs the second polynomial to add.
 * \return the sum of both polynomials.
 *
 * \ingroup maths
 */
template <CoefficientDomain T, int vars>
requires (vars > 0 && vars <= 3)
Laurent<T, vars> operator + (Laurent<T, vars>&& lhs, Laurent<T, vars>&& rhs) {
    // The += operator will choose a direction for the addition that avoids a
    // deep copy, if possible.
    return std::move(lhs += std::move(rhs));
}

/**
 * Returns the negative of the given polynomial.
 *
 * \param arg the polynomial to negate.
 * \return the negative of \a arg.
 *
 * \ingroup maths
 */
template <CoefficientDomain T, int vars>
requires (vars > 0 && vars <= 3)
Laurent<T, vars> operator - (Laurent<T, vars> arg) {
    arg.negate();
    return arg;
}

/**
 * Subtracts the two given polynomials.
 *
 * \param lhs the polynomial to sutract \a rhs from.
 * \param rhs the polynomial to subtract from \a lhs.
 * \return the difference of the two given polynomials.
 *
 * \ingroup maths
 */
template <CoefficientDomain T, int vars>
requires (vars > 0 && vars <= 3)
Laurent<T, vars> operator - (const Laurent<T, vars>& lhs,
        const Laurent<T, vars>& rhs) {
    using Term = detail::LaurentHelper<T, vars>::Term;

    // Handle zero polynomials separately.
    if (rhs.isZero())
        return lhs;
    if (lhs.isZero())
        return -rhs;

    // If the two ranges do not overlap, just copy them separately;
    // this avoids having to worry about jumping over the gap between
    // them in the code that follows.
    if (lhs.maxExp_ < rhs.minExp_) {
        auto* terms = new Term[rhs.maxExp_ - lhs.minExp_ + 1];
        std::copy(lhs.term_ + lhs.minExp_ - lhs.base_,
            lhs.term_ + lhs.maxExp_ + 1 - lhs.base_,
            terms);
        Term *rit, *cit;
        for (rit = rhs.term_ + rhs.minExp_ - rhs.base_,
                    cit = terms + rhs.minExp_ - lhs.minExp_;
                rit != rhs.term_ + rhs.maxExp_ + 1 - rhs.base_; ++rit, ++cit)
            if (*rit != 0)
                *cit = -(*rit);
        return Laurent<T, vars>(lhs.minExp_, rhs.maxExp_, terms);
    } else if (rhs.maxExp_ < lhs.minExp_) {
        auto* terms = new Term[lhs.maxExp_ - rhs.minExp_ + 1];
        Term *rit, *cit;
        for (rit = rhs.term_ + rhs.minExp_ - rhs.base_, cit = terms;
                rit != rhs.term_ + rhs.maxExp_ + 1 - rhs.base_; ++rit, ++cit)
            if (*rit != 0)
                *cit = -(*rit);
        std::copy(lhs.term_ + lhs.minExp_ - lhs.base_,
            lhs.term_ + lhs.maxExp_ + 1 - lhs.base_,
            terms + lhs.minExp_ - rhs.minExp_);
        return Laurent<T, vars>(rhs.minExp_, lhs.maxExp_, terms);
    }

    // We are guaranteed from here on that the two ranges overlap.
    long minExp = std::min(lhs.minExp_, rhs.minExp_);
    long maxExp = std::max(lhs.maxExp_, rhs.maxExp_);
    auto* terms = new Term[maxExp - minExp + 1];

    long exp /* next exponent */, idx /* next index into terms */;

    if (lhs.minExp_ < rhs.minExp_) {
        std::copy(lhs.term_ + lhs.minExp_ - lhs.base_,
            lhs.term_ + rhs.minExp_ - lhs.base_, terms);
        exp = rhs.minExp_;
        idx = rhs.minExp_ - lhs.minExp_;
    } else if (rhs.minExp_ < lhs.minExp_) {
        Term *rit, *cit;
        for (rit = rhs.term_ + rhs.minExp_ - rhs.base_, cit = terms;
                rit != rhs.term_ + lhs.minExp_ - rhs.base_; ++rit, ++cit)
            if (*rit != 0)
                *cit = -(*rit);
        exp = lhs.minExp_;
        idx = lhs.minExp_ - rhs.minExp_;
    } else {
        exp = lhs.minExp_;
        idx = 0;
    }

    for ( ; exp <= lhs.maxExp_ && exp <= rhs.maxExp_; ++idx, ++exp)
        terms[idx] = lhs.term_[exp - lhs.base_] - rhs.term_[exp - rhs.base_];

    // exp is now min(lhs.maxExp_, rhs.maxExp_) + 1.
    if (exp <= lhs.maxExp_) {
        std::copy(lhs.term_ + exp - lhs.base_,
            lhs.term_ + lhs.maxExp_ + 1 - lhs.base_, terms + idx);
    } else if (exp <= rhs.maxExp_) {
        Term *rit, *cit;
        for (rit = rhs.term_ + exp - rhs.base_, cit = terms + idx;
                rit != rhs.term_ + rhs.maxExp_ + 1 - rhs.base_; ++rit, ++cit)
            if (*rit != 0)
                *cit = -(*rit);
    }

    return Laurent<T, vars>(minExp, maxExp, terms, true);
}

/**
 * Subtracts the two given polynomials.
 *
 * \param lhs the polynomial to sutract \a rhs from.
 * \param rhs the polynomial to subtract from \a lhs.
 * \return the difference of the two given polynomials.
 *
 * \ingroup maths
 */
template <CoefficientDomain T, int vars>
requires (vars > 0 && vars <= 3)
Laurent<T, vars> operator - (Laurent<T, vars>&& lhs,
        const Laurent<T, vars>& rhs) {
    return std::move(lhs -= rhs);
}

/**
 * Subtracts the two given polynomials.
 *
 * \param lhs the polynomial to sutract \a rhs from.
 * \param rhs the polynomial to subtract from \a lhs.
 * \return the difference of the two given polynomials.
 *
 * \ingroup maths
 */
template <CoefficientDomain T, int vars>
requires (vars > 0 && vars <= 3)
Laurent<T, vars> operator - (const Laurent<T, vars>& lhs,
        Laurent<T, vars>&& rhs) {
    return std::move(rhs.subtractFrom(lhs));
}

/**
 * Subtracts the two given polynomials.
 *
 * \param lhs the polynomial to sutract \a rhs from.
 * \param rhs the polynomial to subtract from \a lhs.
 * \return the difference of the two given polynomials.
 *
 * \ingroup maths
 */
template <CoefficientDomain T, int vars>
requires (vars > 0 && vars <= 3)
Laurent<T, vars> operator - (Laurent<T, vars>&& lhs, Laurent<T, vars>&& rhs) {
    // The -= operator will choose a direction for the subtraction that avoids
    // a deep copy, if possible.
    return std::move(lhs -= std::move(rhs));
}

/**
 * Multiplies the two given polynomials.
 *
 * \param lhs the first polynomial to multiply.
 * \param rhs the second polynomial to multiply.
 * \return the product of both polynomials.
 *
 * \ingroup maths
 */
template <CoefficientDomain T, int vars>
requires (vars > 0 && vars <= 3)
Laurent<T, vars> operator * (const Laurent<T, vars>& lhs,
        const Laurent<T, vars>& rhs) {
    using Term = detail::LaurentHelper<T, vars>::Term;

    if (lhs.isZero() || rhs.isZero()) {
        return {}; // zero
    } else if (lhs.minExp_ == lhs.maxExp_) {
        Laurent<T, vars> ans(rhs);
        const Term& single = lhs.term_[lhs.minExp_ - lhs.base_];
        for (auto it = ans.term_ + ans.minExp_ - ans.base_;
                it <= ans.term_ + ans.maxExp_ - ans.base_; ++it)
            (*it) *= single;
        ans.base_ += lhs.minExp_;
        ans.minExp_ += lhs.minExp_;
        ans.maxExp_ += lhs.minExp_;
        return ans;
    } else if (rhs.minExp_ == rhs.maxExp_) {
        Laurent<T, vars> ans(lhs);
        const Term& single = rhs.term_[rhs.minExp_ - rhs.base_];
        for (auto it = ans.term_ + ans.minExp_ - ans.base_;
                it <= ans.term_ + ans.maxExp_ - ans.base_; ++it)
            (*it) *= single;
        ans.base_ += rhs.minExp_;
        ans.minExp_ += rhs.minExp_;
        ans.maxExp_ += rhs.minExp_;
        return ans;
    } else {
        // Both polynomials have more than one non-zero term.
        auto* terms = new Laurent<T, vars>::Term[
            lhs.maxExp_ - lhs.minExp_ + rhs.maxExp_ - rhs.minExp_ + 1];
        detail::productBest<Term, detail::SetOrAdd::Either, false>(terms,
            lhs.term_ + lhs.minExp_ - lhs.base_, lhs.maxExp_ - lhs.minExp_ + 1,
            rhs.term_ + rhs.minExp_ - rhs.base_, rhs.maxExp_ - rhs.minExp_ + 1,
            nullptr /* scratch */);

        // Note: the final minExp/maxExp terms will both be non-zero,
        // since the same is true of both lhs and rhs.
        return { lhs.minExp_ + rhs.minExp_, lhs.maxExp_ + rhs.maxExp_, terms };
    }
}

/**
 * Multiplies the two given polynomials.
 *
 * \param lhs the first polynomial to multiply.
 * \param rhs the second polynomial to multiply.
 * \return the product of both polynomials.
 *
 * \ingroup maths
 */
template <CoefficientDomain T, int vars>
requires (vars > 0 && vars <= 3)
Laurent<T, vars> operator * (Laurent<T, vars>&& lhs, Laurent<T, vars>&& rhs) {
    using Term = detail::LaurentHelper<T, vars>::Term;

    if (lhs.isZero() || rhs.isZero()) {
        return {}; // zero
    } else if (lhs.minExp_ == lhs.maxExp_) {
        const Term& single = lhs.term_[lhs.minExp_ - lhs.base_];
        for (auto it = rhs.term_ + rhs.minExp_ - rhs.base_;
                it <= rhs.term_ + rhs.maxExp_ - rhs.base_; ++it)
            (*it) *= single;
        rhs.base_ += lhs.minExp_;
        rhs.minExp_ += lhs.minExp_;
        rhs.maxExp_ += lhs.minExp_;
        return std::move(rhs);
    } else if (rhs.minExp_ == rhs.maxExp_) {
        const Term& single = rhs.term_[rhs.minExp_ - rhs.base_];
        for (auto it = lhs.term_ + lhs.minExp_ - lhs.base_;
                it <= lhs.term_ + lhs.maxExp_ - lhs.base_; ++it)
            (*it) *= single;
        lhs.base_ += rhs.minExp_;
        lhs.minExp_ += rhs.minExp_;
        lhs.maxExp_ += rhs.minExp_;
        return std::move(lhs);
    } else {
        // Both polynomials have more than one non-zero term.
        auto* terms = new Laurent<T, vars>::Term[
            lhs.maxExp_ - lhs.minExp_ + rhs.maxExp_ - rhs.minExp_ + 1];
        detail::productBest<Term, detail::SetOrAdd::Either, true>(terms,
            lhs.term_ + lhs.minExp_ - lhs.base_, lhs.maxExp_ - lhs.minExp_ + 1,
            rhs.term_ + rhs.minExp_ - rhs.base_, rhs.maxExp_ - rhs.minExp_ + 1,
            nullptr /* scratch */);

        // Note: the final minExp/maxExp terms will both be non-zero,
        // since the same is true of both lhs and rhs.
        return { lhs.minExp_ + rhs.minExp_, lhs.maxExp_ + rhs.maxExp_, terms };
    }
}

#ifndef __DOXYGEN
// Don't confuse doxygen with specialisations.
template <CoefficientDomain T, int vars>
requires (vars > 0 && vars <= 3)
struct RingTraits<Laurent<T, vars>> {
    inline static const Laurent<T, vars> zero;
    inline static const Laurent<T, vars> one {1};
    static constexpr bool commutative = RingTraits<T>::commutative;
    static constexpr bool zeroInitialised = true;
    static constexpr bool zeroDivisors = false; // since T is a domain
    static constexpr bool inverses = false;
};
#endif // __DOXYGEN

// Inline constants for Laurent:

template <CoefficientDomain T, int vars>
requires (vars > 0 && vars <= 3)
const T Laurent<T, vars>::zero_(0);

} // namespace regina

#endif
