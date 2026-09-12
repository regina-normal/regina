
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
 *  \brief Implements single variable Laurent polynomials over arbitrary rings.
 */

#include "utilities/stringutils.h"
#include "utilities/tightencoding.h"
#include "concepts/io.h"
#include "concepts/iterator.h"
#include "core/output.h"
#include <iostream>

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

template <CoefficientDomain> class Laurent;

/**
 * Represents different algorithms for multiplying polynomials.
 */
enum class PolynomialProductAlgorithm {
    /**
     * The default algorithm.  Here Regina will choose whichever algorithm it
     * thinks (rightly or wrongly) is most appropriate.
     */
    Default = 0,
    /**
     * The classic "schoolbook" algorithm involving two nested loops over the
     * polynomial coefficients.  To multiply two polynomials with degree span
     * \a n, this requires computing `O(n^2)` individual products of
     * coefficients.
     */
    Classic = 1,
    /**
     * Karatsuba's divide-and-conquer algorithm.  To multiply two polynomials
     * with the same degree span \a n, this requires computing
     * `O(n^(log₂ 3)) ≃ O(n^1.585)` individual products of coefficients.
     *
     * Karatsuba's algorithm is asymptotically better than the classic
     * algorithm, but it carries significant overhead and so will
     * typically be slower for small polynomials.
     */
    Karatsuba = 2
};

/**
 * Indicates if/when to use Karatsuba multiplication when multiplying
 * polynomials with coefficients of type \a T.
 *
 * Karatsuba multiplication is intended to reduce the number of coefficient
 * product operations, but this comes with some significant overhead
 * (including more addition/subtraction operations, and more temporary
 * variables).  Therefore whether it is worthwhile will depend on the cost of
 * different operations for type \a T (which is why this constant is templated).
 *
 * If `karatsubaThreshold<T>` takes a positive value \a n, this means that
 * Karatsuba multiplication will only be used when both polynomials being
 * multiplied have at least \a n coefficients (i.e., both degree spans are
 * at least `n-1`).  If `karatsubaThreshold<T>` is zero, then Karatsuba
 * multiplication will not be used at all.
 *
 * \nopython
 */
template <CoefficientDomain T>
static constexpr size_t karatsubaThreshold = 16;

#ifndef __DOXYGEN
// For now, do not use Karatsuba multiplication when working with "nested"
// Laurent polynomials (i.e., Laurent<Laurent<T>>).
template <CoefficientDomain T>
static constexpr size_t karatsubaThreshold<Laurent<T>> = 0;
#endif

/**
 * Represents a single-variable Laurent polynomial with coefficients of
 * type \a T.  A Laurent polynomial differs from an ordinary polynomial
 * in that it allows negative exponents (so, unlike the Polynomial class,
 * you can represent both `2+3x` and `1+1/x`).
 *
 * The underlying storage method for this class is dense (i.e., all
 * coefficients are explicitly stored, including zero coefficients).
 * Like `std::vector`, Laurent polynomials allocates additional memory to
 * accommodate future growth, which means that (for example) appending a term
 * with exponent `maxExp() + 1` or `minExp() - 1` is amortised constant time.
 *
 * See also the class Laurent2, which describes Laurent polynomials in
 * two variables.
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
 * section 16.5.5.16); Regina's Integer, LargeInteger and Rational classes;
 * and Laurent<U> for any \a U having this same property.
 *
 * \python In Python, the class Laurent refers to the specific
 * template class `Laurent<Integer>`.
 *
 * \tparam T the coefficient type.  A typical coefficient type would be
 * Integer or Rational.  Note that native C++ integer types are _not_
 * supported (since they have no zero-initialising default constructor).
 *
 * \ingroup maths
 */
template <CoefficientDomain T>
class Laurent :
        public ShortOutput<Laurent<T>, true>,
        public TightEncodable<Laurent<T>> {
    public:
        /**
         * The type of each coefficient of the polynomial.
         */
        using Coefficient = T;

        /**
         * A read-only iterator type for iterating over polynomial coefficients.
         * This is identical to \a const_iterator, since iterators over
         * coefficients are always read-only.
         */
        using iterator = const T*;

        /**
         * A read-only iterator type for iterating over polynomial coefficients.
         */
        using const_iterator = const T*;

        // Make sure the compiler can see the zero-argument string output
        // routines, since we declare alternative versions of these below.
        using ShortOutput<Laurent<T>, true>::str;
        using ShortOutput<Laurent<T>, true>::utf8;

    private:
        long minExp_;
            /**< The minimum exponent that appears in the polynomial.
                 This may take any value if this is the zero polynomial. */
        long maxExp_;
            /**< The maximum exponent that appears in the polynomial.
                 This may take any value if this is the zero polynomial. */
        long base_;
            /**< The exponent represented by the coefficient coeff_[0].
                 This may take any value if this is the zero polynomial.
                 For a non-zero polynomial, this is less than or equal to
                 \a minExp_ (and in many scenarios they will be the same). */
        size_t capacity_;
            /**< The total allocated size of the \a coeff_ array.
                 For the zero polynomial, this is ignored and may take any
                 value (since \a coeff_ will be `null`).  For a non-zero
                 polynomial, this must be at least `maxExp_ - base_ + 1`. */
        T* coeff_;
            /**< An array of size \a coeff_ holding the coefficients of the
                 polynomial, or `null` if this is the zero polynomial.  The
                 coefficient `coeff_[i]` is for the term `x^(base_ + i)`. */

        static const T zero_;
            /**< A zero coefficient that we can safely make references to. */

        /**
         * Used to indicate whether the result of some computation should be
         * added to or assigned to a destination object.
         */
        enum class SetOrAdd {
            /**
             * Indicates that the result may be _either_ assigned to or added to
             * the destination object, whichever is fastest.  This is typically
             * used when assignment is required, but the destination object is
             * known to have been initialised to zero.
             */
            Either = 0,
            /**
             * Indicates that the result should be assigned to the destination
             * object.  This is used (for example) by `x = y * z`.
             */
            Set = 1,
            /**
             * Indicates that the result should be added to the destination
             * object.  This is used (for example) by `x.addProduct(y, z)`.
             */
            Add = 2
        };

        /**
         * A pointer to a C-style array of coefficients.
         *
         * Such buffers are used as shared scratch space by internal routines
         * (in particular, by the implementation of Karatsuba multiplication).
         *
         * \tparam writeable `true` if the coefficients should be read-write,
         * or `false` if they should be read-only.
         */
        template <bool writeable>
        using Buffer = std::conditional_t<writeable, T*, const T*>;

        /**
         * Returns the extra capacity to add when growing the array of
         * coefficients.  This should be a multiple of \a capacity_, in order
         * to ensure amortised constant append/prepend time.
         *
         * \pre This is not the zero polynomial (i.e., \a coeff_ is non-null).
         *
         * \return the extra capacity to add.
         */
        constexpr size_t growth() const {
            return (capacity_ + 1) >> 1;
        }

    public:
        /**
         * Creates the zero polynomial.
         */
        Laurent() : coeff_(nullptr) {
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
        Laurent(const Laurent<T>& value) {
            if (value.coeff_) {
                minExp_ = base_ = value.minExp_;
                maxExp_ = value.maxExp_;
                capacity_ = value.maxExp_ - value.minExp_ + 1;
                coeff_ = new T[capacity_];
                std::copy(
                    value.coeff_ + value.minExp_ - value.base_,
                    value.coeff_ + value.maxExp_ - value.base_ + 1, coeff_);
            } else {
                coeff_ = nullptr;
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
        Laurent(const Laurent<U>& value) {
            if (value.coeff_) {
                minExp_ = base_ = value.minExp_;
                maxExp_ = value.maxExp_;
                capacity_ = value.maxExp_ - value.minExp_ + 1;
                coeff_ = new T[capacity_];
                std::copy(
                    value.coeff_ + value.minExp_ - value.base_,
                    value.coeff_ + value.maxExp_ - value.base_ + 1, coeff_);
            } else {
                coeff_ = nullptr;
            }
        }

        /**
         * Moves the contents of the given polynomial to this new polynomial.
         * This is a fast (constant time) operation.
         *
         * \param value the polynomial to move.
         */
        Laurent(Laurent<T>&& value) noexcept :
                minExp_(value.minExp_), maxExp_(value.maxExp_),
                base_(value.base_), capacity_(value.capacity_),
                coeff_(value.coeff_) {
            value.coeff_ = nullptr;
        }

        /**
         * Creates a new polynomial from the given sequence of coefficients.
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
        Laurent(long minExp, Iterator begin, Iterator end) : coeff_(nullptr) {
            init(minExp, begin, end);
        }

        /**
         * Creates a new polynomial from a hard-coded sequence of coefficients.
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
        Laurent(long minExp, std::initializer_list<T> coefficients) :
                coeff_(nullptr) {
            init(minExp, coefficients.begin(), coefficients.end());
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
                coeff_ = nullptr;
            } else {
                minExp_ = maxExp_ = base_ = 0;
                capacity_ = 1;
                coeff_ = new T[1];
                *coeff_ = constant;
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
                coeff_ = nullptr;
            } else {
                minExp_ = maxExp_ = base_ = 0;
                capacity_ = 1;
                coeff_ = new T[1];
                *coeff_ = std::move(constant);
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
                coeff_ = nullptr;
            } else {
                minExp_ = maxExp_ = base_ = 0;
                capacity_ = 1;
                coeff_ = new T[1];
                *coeff_ = constant;
            }
        }

        /**
         * Destroys this polynomial.
         */
        ~Laurent() {
            delete[] coeff_;
        }

        /**
         * Sets this to become the zero polynomial.
         */
        void init() {
            delete[] coeff_;
            coeff_ = nullptr;
        }

        /**
         * Sets this to become the polynomial `x^d` for the
         * given exponent \a d.
         *
         * \param exponent the new exponent to use for this polynomial.
         */
        void initExp(long exponent) {
            delete[] coeff_;
            minExp_ = maxExp_ = base_ = exponent;
            capacity_ = 1;
            coeff_ = new T[1];
            *coeff_ = 1;
        }

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
        void init(long minExp, Iterator begin, Iterator end) {
            delete[] coeff_;

            // Skip through any initial zero terms.
            while (begin != end && *begin == 0) {
                ++begin;
                ++minExp;
            }
            if (begin == end) {
                coeff_ = nullptr;
                return;
            }

            // We have a non-zero polynomial.
            minExp_ = base_ = minExp;
            capacity_ = end - begin;
            maxExp_ = minExp + capacity_ - 1;
            coeff_ = new T[capacity_];

            T* it = coeff_;
            while (begin != end)
                *it++ = *begin++;

            // The final coefficient(s) might be zero: fix maxExp_ accordingly.
            // It _is_ guaranteed here that the first coefficient is non-zero.
            for (--it; *it == 0; --it, --maxExp_)
                ;
        }

        /**
         * Returns the smallest exponent that appears in this polynomial
         * with a non-zero coefficient.
         * If this is the zero polynomial, then this routine returns 0.
         *
         * \return the smallest exponent.
         */
        long minExp() const {
            return coeff_ ? minExp_ : 0;
        }

        /**
         * Returns the largest exponent that appears in this polynomial
         * with a non-zero coefficient.
         * If this is the zero polynomial, then this routine returns 0.
         *
         * \return the largest exponent.
         */
        long maxExp() const {
            return coeff_ ? maxExp_ : 0;
        }

        /**
         * Returns whether this is the zero polynomial.
         *
         * \return \c true if and only if this is the zero polynomial.
         */
        bool isZero() const {
            return ! coeff_;
        }

        /**
         * Indicates the range of exponents for which memory is currently
         * allocated.
         *
         * This is mainly provided for diagnostics and performance analysis;
         * end users will typically not need to use this routine.
         *
         * For a non-zero polynomial, this routine returns a pair
         * `(base, capacity)`, where \a base indicates the smallest exponent
         * for which memory is allocated, and \a capacity indicates the total
         * number of exponents for which memory is allocated.  This means that
         * the _largest_ exponent for which memory is allocated will be
         * `base + capacity - 1`.
         *
         * \return a pair `(base, capacity)` as described above, or `(0, 0)`
         * if this is the zero polynomial.
         */
        std::pair<long, size_t> allocation() const {
            if (coeff_)
                return { base_, capacity_ };
            else
                return { 0, 0 };
        }

        /**
         * Reserves enough memory in the coefficient array to span the given
         * range of exponents.  This routine merely affects the internal
         * storage; it does not change the value of this polynomial.
         *
         * Specifically, after calling `reserveRange(fromExp, toExp)`, you can
         * add non-zero coefficients for any `x^i` where `fromExp ≤ i ≤ toExp`
         * without needing to reallocate memory and/or shuffle data around.
         *
         * As a special case however, if this is the zero polynomial then this
         * routine will do nothing (since internally, the zero polynomial is
         * represented by a null coefficient array).  In particular, it makes
         * no sense to call reserveRange() immediately after a Laurent
         * polynomial has been default-constructed.
         *
         * Note that the range `[fromExp, toExp]` is inclusive at both ends
         * (unlike iterator ranges, for example).
         *
         * This routine is safe to call even if `fromExp > toExp` (in which
         * case it will do nothing at all).
         *
         * \param fromExp the smallest exponent whose coefficient will have
         * storage reserved.
         * \param toExp the largest exponent whose coefficient will have
         * storage reserved.
         */
        void reserveRange(long fromExp, long toExp) {
            if (coeff_ && fromExp <= toExp) {
                // This is a bit heavyweight: the biggest unnecessary cost is
                // zeroing out coefficients in the ranges [fromExp, minexp_)
                // and/or (maxExp_, toExp] in the case where we don't need to
                // reallocate.  However, we can live with this for now.
                long oldMin = minExp_;
                long oldMax = maxExp_;
                reallocateForRange(fromExp, toExp); // changes minExp_, maxExp_
                minExp_ = oldMin;
                maxExp_ = oldMax;
            }
        }

        /**
         * Returns the given coefficient of this polynomial.
         * There are no restrictions on the exponent \a exp.
         *
         * \python Python users can also use this operator to _set_
         * cofficients; that is, you can write `poly[exp] = value`.
         * However, when _getting_ a coefficient this operator will return
         * by value (to enforce constness), which means for example you
         * cannot write something like `poly[exp].negate()`.
         *
         * \cpp For C++ users, this operator is read-only.  To _set_
         * coefficients, you must use the separate routine set().
         *
         * \param exp the exponent of the term whose coefficient should
         * be returned.
         * \return the coefficient of the given term.
         */
        const T& operator [] (long exp) const {
            if (coeff_ && exp >= minExp_ && exp <= maxExp_)
                return coeff_[exp - base_];
            else
                return zero_;
        }

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
         * \python This set() routine is available, but you can
         * also set coefficients directly using syntax of the form
         * `p[exp] = value`.
         *
         * \param exp the exponent of the term whose coefficient should
         * be changed.
         * \param value the new value of this coefficient.
         */
        void set(long exp, const T& value) {
            if (value == 0) {
                if (coeff_) {
                    if (exp == maxExp_) {
                        if (minExp_ == maxExp_) {
                            // This becomes the zero polynomial.
                            delete[] coeff_;
                            coeff_ = nullptr;
                        } else {
                            --maxExp_;
                            // We know the lowest-exponent coefficient != 0.
                            while (coeff_[maxExp_ - base_] == 0)
                                --maxExp_;
                        }
                    } else if (exp == minExp_) {
                        // Here we know the highest-exponent coefficient != 0.
                        ++minExp_;
                        while (coeff_[minExp_ - base_] == 0)
                            ++minExp_;
                    } else if (exp > minExp_ && exp < maxExp_) {
                        coeff_[exp - base_] = 0;
                    }
                }
                return;
            }

            // From here, value is non-zero.
            if (! coeff_) {
                minExp_ = maxExp_ = base_ = exp;
                capacity_ = 1;
                coeff_ = new T[1];
                *coeff_ = value;
            } else if (exp >= minExp_ && exp <= maxExp_) {
                coeff_[exp - base_] = value;
            } else if (exp < base_) {
                // The minimum exponent decreases, and we must reallocate.
                size_t extra = std::max(growth(),
                    static_cast<size_t>(base_ - exp));
                capacity_ += extra;

                T* newCoeff = new T[capacity_];
                std::move(coeff_ + (minExp_ - base_),
                    coeff_ + (maxExp_ - base_) + 1,
                    newCoeff + (minExp_ - base_ + extra));
                base_ -= extra;
                newCoeff[exp - base_] = value;
                minExp_ = exp;

                delete[] coeff_;
                coeff_ = newCoeff;
            } else if (exp < minExp_) {
                // The minimum exponent decreases, but we need not reallocate.
                for (--minExp_; minExp_ > exp; --minExp_)
                    coeff_[minExp_ - base_] = 0;
                coeff_[exp - base_] = value;
            } else if (exp < static_cast<long>(base_ + capacity_)) {
                // The maximum exponent increases, but we need not reallocate.
                for (++maxExp_; maxExp_ < exp; ++maxExp_)
                    coeff_[maxExp_ - base_] = 0;
                coeff_[exp - base_] = value;
            } else {
                // The maximum exponent increases, and we must reallocate.
                size_t extra = std::max(growth(),
                    static_cast<size_t>(exp - base_ - capacity_ + 1));
                capacity_ += extra;

                T* newCoeff = new T[capacity_];
                std::move(coeff_ + (minExp_ - base_),
                    coeff_ + (maxExp_ - base_) + 1,
                    newCoeff + (minExp_ - base_));
                newCoeff[exp - base_] = value;
                maxExp_ = exp;

                delete[] coeff_;
                coeff_ = newCoeff;
            }
        }

        /**
         * Returns a C++ iterator pointing to the first non-zero coefficient of
         * this polynomial; that is, the coefficient corresponding to the
         * exponent minExp().  Such iterators provide read-only access to the
         * coefficients: to modify coefficients you will need to call different
         * routines, such as set().
         *
         * The iterator range from begin() to end() runs through all
         * coefficients of this polynomial, including zeroes, corresponding to
         * exponents ranging from minExp() to maxExp() inclusive.  If this is
         * the zero polynomial then this iterator range will be empty.
         *
         * \nopython For Python users, Laurent implements the Python iterable
         * interface.  You can iterate through coefficients in the same way
         * that you would iterate over any native Python container.
         *
         * \return an iterator pointing to the first non-zero coefficient.
         */
        iterator begin() const {
            return coeff_ ? coeff_ + minExp_ - base_ : nullptr;
        }

        /**
         * Returns a C++ iterator pointing beyond the last non-zero coefficient
         * of this polynomial; that is, beyond the coefficient corresponding to
         * the exponent maxExp().  Such iterators provide read-only access to
         * the coefficients: to modify coefficients you will need to call
         * different routines, such as set().
         *
         * The iterator range from begin() to end() runs through all
         * coefficients of this polynomial, including zeroes, corresponding to
         * exponents ranging from minExp() to maxExp() inclusive.  If this is
         * the zero polynomial then this iterator range will be empty.
         *
         * \nopython For Python users, Laurent implements the Python iterable
         * interface.  You can iterate through coefficients in the same way
         * that you would iterate over any native Python container.
         *
         * \return an iterator pointing beyond the last non-zero coefficient.
         */
        iterator end() const {
            return coeff_ ? coeff_ + maxExp_ - base_ + 1 : nullptr;
        }

#ifdef __APIDOCS
        /**
         * Returns a Python iterator that provides read-only access to all
         * coefficients of this polynomial, including zeroes, corresponding to
         * exponents ranging from minExp() to maxExp() inclusive.  If this is
         * the zero polynomial then this iterator range will be empty.
         *
         * To enforce read-only access, Python iterators will return
         * coefficients by value, not by reference.  If you wish to modify
         * coefficients then you will need to call different routines, such as
         * set().
         *
         * \nocpp For C++ users, Laurent provides the usual begin() and end()
         * functions instead.  In particular, you can iterate over coefficients
         * in the usual way using a range-based `for` loop.
         *
         * \return an iterator over all coefficients of this polynomial.
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
        bool operator == (const Laurent<T>& rhs) const {
            if (! coeff_)
                return (! rhs.coeff_);
            if (! rhs.coeff_)
                return false;
            if (minExp_ != rhs.minExp_ || maxExp_ != rhs.maxExp_)
                return false;
            return std::equal(
                coeff_ + minExp_ - base_, coeff_ + maxExp_ - base_ + 1,
                rhs.coeff_ + rhs.minExp_ - rhs.base_);
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
                return ! coeff_;
            else
                return (minExp_ == 0 && maxExp_ == 0 && coeff_ &&
                    coeff_[-base_] == constant);
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
                return ! coeff_;
            else
                return (minExp_ == 0 && maxExp_ == 0 && coeff_ &&
                    coeff_[-base_] == constant);
        }

        /**
         * Compares this against the given polynomial under a total
         * ordering of all one-variable Laurent polynomials.
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
        std::strong_ordering operator <=> (const Laurent<T>& rhs) const {
            if (! coeff_)
                return rhs.coeff_ ? std::strong_ordering::less :
                    std::strong_ordering::equal;
            if (! rhs.coeff_)
                return std::strong_ordering::greater;
            if (auto c = minExp_ <=> rhs.minExp_; c != 0)
                return c;
            if (auto c = maxExp_ <=> rhs.maxExp_; c != 0)
                return c;
            return std::lexicographical_compare_three_way(
                coeff_ + minExp_ - base_,
                coeff_ + maxExp_ - base_ + 1,
                rhs.coeff_ + rhs.minExp_ - rhs.base_,
                rhs.coeff_ + rhs.maxExp_ - rhs.base_ + 1);
        }

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
         * \param value the polynomial to copy.
         * \return a reference to this polynomial.
         */
        Laurent& operator = (const Laurent<T>& value) {
            // Treat x = x separately, since otherwise we break things when we
            // reset base_ = value.minExp_.
            if (&value == this)
                return *this;

            if (value.coeff_) {
                if (! coeff_) {
                    capacity_ = value.maxExp_ - value.minExp_ + 1;
                    coeff_ = new T[capacity_];
                } else if (capacity_ <
                        static_cast<size_t>(value.maxExp_ - value.minExp_ + 1)) {
                    delete[] coeff_;
                    capacity_ = value.maxExp_ - value.minExp_ + 1;
                    coeff_ = new T[capacity_];
                }
                base_ = minExp_ = value.minExp_;
                maxExp_ = value.maxExp_;
                std::copy(
                    value.coeff_ + value.minExp_ - value.base_,
                    value.coeff_ + value.maxExp_ - value.base_ + 1,
                    coeff_);
            } else {
                if (coeff_) {
                    delete[] coeff_;
                    coeff_ = nullptr;
                }
            }
            return *this;
        }

        /**
         * Sets this to be a copy of the given polynomial.
         *
         * This and the given polynomial need not have the same minimum
         * and/or maximum exponents.
         *
         * This operator induces a deep copy of \a value.
         *
         * \param value the polynomial to copy.
         * \return a reference to this polynomial.
         */
        template <CoefficientDomain U>
        requires std::assignable_from<T&, U>
        Laurent& operator = (const Laurent<U>& value) {
            // Treat x = x separately, since otherwise we break things when we
            // reset base_ = value.minExp_.
            if (&value == this)
                return *this;

            if (value.coeff_) {
                if (! coeff_) {
                    capacity_ = value.maxExp_ - value.minExp_ + 1;
                    coeff_ = new T[capacity_];
                } else if (capacity_ < value.maxExp_ - value.minExp_ + 1) {
                    delete[] coeff_;
                    capacity_ = value.maxExp_ - value.minExp_ + 1;
                    coeff_ = new T[capacity_];
                }
                base_ = minExp_ = value.minExp_;
                maxExp_ = value.maxExp_;
                std::copy(
                    value.coeff_ + value.minExp_ - value.base_,
                    value.coeff_ + value.maxExp_ - value.base_ + 1,
                    coeff_);
            } else {
                if (coeff_) {
                    delete[] coeff_;
                    coeff_ = nullptr;
                }
            }
            return *this;
        }

        /**
         * Moves the contents of the given polynomial to this polynomial.
         * This is a fast (constant time) operation.
         *
         * This and the given polynomial need not have the same minimum
         * and/or maximum exponents.
         *
         * \param value the polynomial to move.
         * \return a reference to this polynomial.
         */
        Laurent& operator = (Laurent<T>&& value) noexcept {
            minExp_ = value.minExp_;
            maxExp_ = value.maxExp_;
            base_ = value.base_;
            // Let value dispose of the original coefficients in its destructor.
            // Note: this leaves value in an invalid state (since its exponent
            // range might exceed its capacity), but it can still be assigned
            // a new value and used from there.
            std::swap(capacity_, value.capacity_);
            std::swap(coeff_, value.coeff_);
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
            // Re-initialising the polynomial to a constant seems like a good
            // opportunity to claw back memory.  We will dispose of our
            // pre-allocated coefficient array, if we had one.  The cost of
            // course is that we will need to re-allocate more space if this
            // polynomial should subsequently grow again.
            delete[] coeff_;
            if (constant != 0) {
                minExp_ = maxExp_ = base_ = 0;
                capacity_ = 1;
                coeff_ = new T[1];
                *coeff_ = constant;
            } else {
                coeff_ = nullptr;
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
            // See =(const T&) for discussion on why we delete coeff_.
            delete[] coeff_;
            if (constant != 0) {
                minExp_ = maxExp_ = base_ = 0;
                capacity_ = 1;
                coeff_ = new T[1];
                *coeff_ = std::move(constant);
            } else {
                coeff_ = nullptr;
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
            // See =(const T&) for discussion on why we delete coeff_.
            delete[] coeff_;
            if (constant) {
                minExp_ = maxExp_ = base_ = 0;
                capacity_ = 1;
                coeff_ = new T[1];
                *coeff_ = constant;
            } else {
                coeff_ = nullptr;
            }
            return *this;
        }

        /**
         * Swaps the contents of this and the given polynomial.
         * This is a fast (constant time) operation.
         *
         * This and the given polynomial do not need to have the same
         * minimum and/or maximum exponents.
         *
         * \param other the polynomial whose contents should be swapped
         * with this.
         */
        void swap(Laurent<T>& other) noexcept {
            std::swap(minExp_, other.minExp_);
            std::swap(maxExp_, other.maxExp_);
            std::swap(base_, other.base_);
            std::swap(capacity_, other.capacity_);
            std::swap(coeff_, other.coeff_);
        }

        /**
         * Multiplies this polynomial by `x^s` for some integer \a s.
         * This polynomial will be changed directly.
         *
         * This operation is very fast (it runs in small constant time).
         *
         * \param s the power of \a x to multiply by.
         */
        void shift(long s) {
            if (coeff_) {
                base_ += s;
                minExp_ += s;
                maxExp_ += s;
            }
        }

        /**
         * A non-destructive routine that returns the product of this polynomial
         * with `x^s` for some integer \a s.  This polynomial is not changed.
         *
         * If your polynomial is disposable (i.e., you will never need to use it
         * again), then it is must faster to use the rvalue reference version of
         * this function, which runs in small constant time (since it does not
         * need to create a deep copy of this polynomial to store the result).
         * To do this, replace `poly.shifted(s)` with
         * `std::move(poly).shifted(s)`.
         *
         * \param s the power of \a x to multiply by.
         * \return the product of this with `x^s`.
         */
        Laurent shifted(long s) const& {
            Laurent ans(*this);
            ans.shift(s);
            return ans;
        }

        /**
         * A destructive routine that returns the product of this polynomial
         * with `x^s` for some integer \a s.
         *
         * Here "destructive" means that this routine edits the polynomial
         * in-place (as opposed to allocating a new polynomial to store the
         * result).  For this reason, it is declared as an rvalue reference
         * member function: it should only be used if you do not care about
         * the contents of the original polynomial afterwards.
         *
         * This operation is very fast (it runs in small constant time).
         *
         * To use this destructive function, you can call
         * `std::move(poly).shifted(s)`.
         *
         * If you need to preserve the contents of this polynomial, you should
         * instead call the const version of this function, which you can
         * access in the usual way as `poly.shifted(s)`.  The cost of this
         * constness will be the linear-time overhead of creating a deep copy
         * of this polynomial to store the result.
         *
         * \nopython Only the const version of this function is available for
         * Python users.
         *
         * \param s the power of \a x to multiply by.
         * \return the product of this with `x^s`.
         */
        Laurent shifted(long s) && {
            shift(s);
            return std::move(*this);
        }

        /**
         * Multiplies all exponents in this polynomial by \a k for some
         * integer \a k.  This is equivalent to replacing the variable
         * \a x of the polynomial with <i>x</i><sup><i>k</i></sup>.
         *
         * Both positive and negative scaling factors \a k are allowed.
         *
         * \pre \a k is non-zero.
         *
         * \param k the scaling factor to multiply exponents by.
         */
        void scaleUp(long k) {
            if (k == 1 || ! coeff_) {
                return;
            } else if (k == -1) {
                invertX();
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
                // This is simply the non-trivial case of invertX().
                std::reverse(coeff_ + minExp_ - base_,
                    coeff_ + maxExp_ - base_ + 1);
                base_ -= (minExp_ + maxExp_);
                auto tmp = minExp_;
                minExp_ = -maxExp_;
                maxExp_ = -tmp;

                // Now we just scale by |k|.
                k = -k;
            }

            // From here we have k ≥ 2, and minExp_ < maxExp_.

            // For now, we will reuse our existing coeff_ array only when we
            // can do so without changing the gap between base_ and minExp_.
            // Otherwise, if we needed to shift data left whilst expanding it
            // to the right, the order of operations would become very messy.
            long gap = minExp_ - base_;
            size_t newSpan = (maxExp_ - minExp_) * k;
            if (static_cast<size_t>(gap + newSpan) < capacity_) {
                // We can reuse.
                auto src = coeff_ + maxExp_ - base_;
                auto dest = coeff_ + gap + newSpan;
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

                T* newCoeff = new T[capacity_];
                auto src = coeff_ + minExp_ - base_;
                for (auto dest = newCoeff;
                        src <= coeff_ + maxExp_ - base_; dest += k)
                    *dest = std::move(*src++);
                delete[] coeff_;
                coeff_ = newCoeff;

                minExp_ *= k;
                maxExp_ *= k;
                base_ = minExp_;
            }
        }

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
         * \exception FailedPrecondition Either \a k is zero, or some exponent
         * with a non-zero coefficient is not a multiple of \a k.  Be aware
         * that this polynomial might change before this exception is thrown.
         *
         * \param k the scaling factor to divide exponents by.
         */
        void scaleDown(long k) {
            if (k == 0) {
                throw FailedPrecondition("scaleDown() requires a non-zero "
                    "scaling factor");
            } else if (k == 1 || ! coeff_) {
                return;
            } else if (k == -1) {
                invertX();
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
                auto src = coeff_ + minExp_ - base_ + 1;
                auto dest = src;
                auto end = coeff_ + maxExp_ - base_ + 1;
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
                auto start = coeff_ + minExp_ - base_;
                auto src = start + 1;
                auto dest = src;
                auto end = coeff_ + maxExp_ - base_ + 1;
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
         * Negates this polynomial.
         * This polynomial is changed directly.
         */
        void negate() {
            if (! coeff_)
                return;
            for (auto it = coeff_ + minExp_ - base_;
                    it <= coeff_ + maxExp_ - base_; ++it)
                if (*it != 0)
                    it->negate();
        }

        /**
         * Replaces `x` with `x^-1` in this polynomial.
         * This polynomial is changed directly.
         *
         * Calling this routine is equivalent to calling `scaleUp(-1)`.
         */
        void invertX() {
            if (! coeff_) {
                return;
            } else if (minExp_ == maxExp_) {
                base_ -= (minExp_ << 1);
                minExp_ = -minExp_;
                maxExp_ = -maxExp_;
                return;
            } else {
                std::reverse(coeff_ + minExp_ - base_,
                    coeff_ + maxExp_ - base_ + 1);
                base_ -= (minExp_ + maxExp_);
                auto tmp = minExp_;
                minExp_ = -maxExp_;
                maxExp_ = -tmp;
            }
        }

        /**
         * Applies the given transformation to every coefficient of this
         * polynomial.
         *
         * Specifically, for each coefficient \a c corresponding to an exponent
         * in the range from minExp() to maxExp() inclusive (including any zero
         * coefficients in between), this routine will call `action(c)`.
         *
         * Each coefficient will be passed to \a action by reference;
         * typically \a action would modify the coefficient in some way.
         * It is fine if \a action sets some coefficients to zero.
         * The coefficients will be passed in increasing order of their
         * corresponding exponents.
         *
         * If this is the zero polynomial, then \a action will not be called
         * on any coefficients at all.
         *
         * \nopython This function is (for now) not available in Python;
         * instead you can loop through the coefficients and call `set()`.
         *
         * \param action the action (typically a lambda) to perform on each
         * coefficient.
         */
        template <std::invocable<T&> Action>
        void transform(Action&& action) {
            if (coeff_) {
                for (auto it = coeff_ + minExp_ - base_;
                        it <= coeff_ + maxExp_ - base_; ++it)
                    std::invoke(std::forward<Action>(action), *it);
                fixDegrees();
            }
        }

        /**
         * Applies the given transformation to every coefficient of this
         * polynomial, where the transformation is given knowledge of the
         * corresponding exponent.
         *
         * Specifically, for each coefficient \a c corresponding to an exponent
         * \a e in the range from minExp() to maxExp() inclusive (including any
         * zero coefficients in between), this routine will call `action(c, e)`.
         * Typically this action would modify the coefficients in some way.
         *
         * Each coefficient will be passed to \a action by reference;
         * typically \a action would modify the coefficient in some way.
         * The corresponding exponent will be passed by value, and so cannot be
         * modified.  It is fine if \a action sets some coefficients to zero.
         * The coefficients will be passed in increasing order of their
         * corresponding exponents.
         *
         * If this is the zero polynomial, then \a action will not be called
         * on any coefficients at all.
         *
         * \nopython This function is (for now) not available in Python;
         * instead you can loop through the coefficients and call `set()`.
         *
         * \param action the action (typically a lambda) to perform on each
         * coefficient and its corresponding exponent.
         */
        template <std::invocable<T&, long> Action>
        void transform(Action&& action) {
            if (coeff_) {
                auto it = coeff_ + minExp_ - base_;
                auto exp = minExp_;
                while (exp <= maxExp_)
                    std::invoke(std::forward<Action>(action), *it++, exp++);
                fixDegrees();
            }
        }

        /**
         * Allows the extraction of the coefficients of this polynomial, one
         * at a time, by passing them as rvalue references to the given action.
         *
         * Specifically, for each _non-zero_ coefficient \a c corresponding to
         * an exponent \a e in the range from minExp() to maxExp() inclusive,
         * this routine will call `action(std::move(c), e)`.  Note that this
         * behaviour differs from transform() (which calls its action for zero
         * coefficients as well).
         *
         * Each coefficient will be passed to \a action by rvalue reference,
         * and each corresponding exponent will be passed by value.
         * The coefficients will be passed in increasing order of their
         * corresponding exponents.
         *
         * If this is the zero polynomial, then \a action will not be called
         * on any coefficients at all.
         *
         * \nopython
         *
         * \param action the action (typically a lambda) to perform on each
         * non-zero coefficient and its corresponding exponent.
         */
        template <std::invocable<T&&, long> Action>
        void extract(Action&& action) && {
            if (coeff_) {
                auto it = coeff_ + minExp_ - base_;
                for (long exp = minExp_; exp <= maxExp_; ++exp, ++it)
                    if (*it != 0)
                        std::invoke(std::forward<Action>(action),
                            std::move(*it), exp);
            }
        }

        /**
         * Multiplies this polynomial by the given constant.
         *
         * \param scalar the scalar factor to multiply by.
         * \return a reference to this polynomial.
         */
        Laurent& operator *= (const T& scalar) {
            if (coeff_) {
                if (scalar == 0)
                    init();
                else
                    for (auto it = coeff_ + minExp_ - base_;
                            it <= coeff_ + maxExp_ - base_; ++it)
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
            if (coeff_) {
                if (scalar == 0)
                    init();
                else
                    for (auto it = coeff_ + minExp_ - base_;
                            it <= coeff_ + maxExp_ - base_; ++it)
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
            if (coeff_) {
                for (auto it = coeff_ + minExp_ - base_;
                        it <= coeff_ + maxExp_ - base_; ++it)
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
            if (coeff_) {
                for (auto it = coeff_ + minExp_ - base_;
                        it <= coeff_ + maxExp_ - base_; ++it)
                    (*it) /= scalar;

                // We could have zeroed out some coefficients.
                fixDegrees();
            }
            return *this;
        }

        /**
         * Adds the given polynomial to this.
         *
         * The given polynomial need not have the same minimum and/or
         * maximum exponents as this.
         *
         * \warning This routine may trigger a deep copy (depending upon
         * the range of exponents used in \a other).  Consider using
         * the binary `+` operator instead, which is better able to
         * avoid this deep copy where possible.
         *
         * \param other the polynomial to add to this.
         * \return a reference to this polynomial.
         */
        Laurent& operator += (const Laurent<T>& other) {
            // This routine works even if &other == this, since in this case
            // we do not reallocate.

            if (! other.coeff_)
                return *this;

            reallocateForRange(other.minExp_, other.maxExp_); // sets coeff_ ≠ 0

            for (long exp = other.minExp_; exp <= other.maxExp_; ++exp)
                coeff_[exp - base_] += other.coeff_[exp - other.base_];

            // We might have zeroed out some coefficients.
            fixDegrees();
            return *this;
        }

        /**
         * Adds the given polynomial to this.
         *
         * The given polynomial need not have the same minimum and/or
         * maximum exponents as this.
         *
         * \warning This routine may trigger a deep copy (depending upon
         * the range of exponents used in \a other).  Consider using
         * the binary `+` operator instead, which is better able to
         * avoid this deep copy where possible.
         *
         * \param other the polynomial to add to this.
         * \return a reference to this polynomial.
         */
        Laurent& operator += (Laurent<T>&& other) {
            if (! other.coeff_)
                return *this;
            if (! coeff_)
                return *this = std::move(other);

            if ((base_ <= other.minExp_ &&
                        other.maxExp_ < static_cast<long>(base_ + capacity_)) ||
                    minExp_ < other.base_ ||
                    maxExp_ >= static_cast<long>(other.base_ + other.capacity_)) {
                // Either: (a) we can avoid the deep copy by adding other to
                // this; or (b) we cannot avoid the deep copy either way.
                reallocateForRange(other.minExp_, other.maxExp_);

                auto src = other.coeff_ + other.minExp_ - other.base_;
                auto dest = coeff_ + other.minExp_ - base_;
                for ( ; dest <= coeff_ + other.maxExp_ - base_; ++src, ++dest)
                    *dest += std::move(*src);

                // We might have zeroed out some coefficients.
                fixDegrees();
                return *this;
            } else {
                // We can avoid the deep copy by adding this to other and
                // moving the result back into this.
                other.reallocateForRange(minExp_, maxExp_);

                auto src = coeff_ + minExp_ - base_;
                auto dest = other.coeff_ + minExp_ - other.base_;
                for ( ; src <= coeff_ + maxExp_ - base_; ++src, ++dest)
                    *dest += std::move(*src);

                // We might have zeroed out some coefficients.
                other.fixDegrees();
                return *this = std::move(other);
            }
        }

        /**
         * Subtracts the given polynomial from this.
         *
         * The given polynomial need not have the same minimum and/or
         * maximum exponents as this.
         *
         * \param other the polynomial to subtract from this.
         * \return a reference to this polynomial.
         */
        Laurent& operator -= (const Laurent<T>& other) {
            // This routine works even if &other == this, since in this case
            // we do not reallocate.

            if (! other.coeff_)
                return *this;

            reallocateForRange(other.minExp_, other.maxExp_); // sets coeff_ ≠ 0

            for (long exp = other.minExp_; exp <= other.maxExp_; ++exp)
                coeff_[exp - base_] -= other.coeff_[exp - other.base_];

            // We might have zeroed out some coefficients.
            fixDegrees();
            return *this;
        }

        /**
         * Subtracts the given polynomial from this.
         *
         * The given polynomial need not have the same minimum and/or
         * maximum exponents as this.
         *
         * \param other the polynomial to subtract from this.
         * \return a reference to this polynomial.
         */
        Laurent& operator -= (Laurent<T>&& other) {
            if (! other.coeff_)
                return *this;
            if (! coeff_) {
                other.negate();
                return *this = std::move(other);
            }

            if ((base_ <= other.minExp_ &&
                        other.maxExp_ < static_cast<long>(base_ + capacity_)) ||
                    minExp_ < other.base_ ||
                    maxExp_ >= static_cast<long>(other.base_ + other.capacity_)) {
                // Either: (a) we can avoid the deep copy by subtracting other
                // from this; or (b) we cannot avoid the deep copy either way.
                reallocateForRange(other.minExp_, other.maxExp_);

                auto src = other.coeff_ + other.minExp_ - other.base_;
                auto dest = coeff_ + other.minExp_ - base_;
                for ( ; dest <= coeff_ + other.maxExp_ - base_; ++src, ++dest)
                    *dest -= std::move(*src);

                // We might have zeroed out some coefficients.
                fixDegrees();
                return *this;
            } else {
                // We can avoid the deep copy by doing the computation in-place
                // within other, and then moving the result back into this.
                other.reallocateForRange(minExp_, maxExp_);

                auto src = coeff_ + minExp_ - base_;
                auto dest = other.coeff_ + other.minExp_ - other.base_;
                for ( ; dest != other.coeff_ + minExp_ - other.base_; ++dest)
                    dest->negate();
                for ( ; src <= coeff_ + maxExp_ - base_; ++src, ++dest)
                    *dest += std::move(*src);
                for ( ; dest <= other.coeff_ + other.maxExp_ - other.base_;
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
         * The given polynomial need not have the same minimum and/or
         * maximum exponents as this.
         *
         * \param other the polynomial to multiply this by.
         * \return a reference to this polynomial.
         */
        Laurent& operator *= (const Laurent<T>& other) {
            if (! coeff_)
                return *this;
            if (! other.coeff_) {
                init();
                return *this;
            }
            if (other.minExp_ == other.maxExp_) {
                // We can get away without reallocating here.
                const auto& scalar = other.coeff_[other.minExp_ - other.base_];
                for (auto it = coeff_ + minExp_ - base_;
                        it <= coeff_ + maxExp_ - base_; ++it)
                    (*it) *= scalar;
                minExp_ += other.minExp_;
                maxExp_ += other.minExp_;
                base_ += other.minExp_;
                return *this;
            }
            if (minExp_ == maxExp_) {
                // TODO: This case could benefit from an rvalue ref argument.
                // We may need to reallocate, but the product itself is simple.
                if (capacity_ >
                        static_cast<size_t>(other.maxExp_ - other.minExp_)) {
                    // Extract the scalar, then overwrite the coeff_ array.
                    const T scalar = coeff_[minExp_ - base_];
                    auto out = coeff_;
                    for (auto in = other.coeff_ + other.minExp_ - other.base_;
                            in <= other.coeff_ + other.maxExp_ - other.base_;
                            ++in)
                        *out++ = (*in) * scalar;
                } else {
                    capacity_ = std::max(capacity_ + growth(),
                        static_cast<size_t>(other.maxExp_ - other.minExp_ + 1));
                    T* newCoeff = new T[capacity_];
                    const T& scalar = coeff_[minExp_ - base_];
                    auto out = newCoeff;
                    for (auto in = other.coeff_ + other.minExp_ - other.base_;
                            in <= other.coeff_ + other.maxExp_ - other.base_;
                            ++in)
                        *out++ = (*in) * scalar;
                    delete[] coeff_;
                    coeff_ = newCoeff;
                }
                minExp_ += other.minExp_;
                maxExp_ += other.maxExp_;
                base_ = minExp_;
                return *this;
            }

            // From here on, we know that both polynomials have more than one
            // non-zero coefficient.

            // The following code works even if &other == this, since we build
            // the coefficients of the product in a separate section of memory.
            // TODO: Can we reuse our own memory if capacity_ is large enough?
            capacity_ = maxExp_ - minExp_ + other.maxExp_ - other.minExp_ + 1;
            T* newCoeff = new T[capacity_];
            productBest<SetOrAdd::Either, false>(newCoeff,
                coeff_ + minExp_ - base_, maxExp_ - minExp_ + 1,
                other.coeff_ + other.minExp_ - other.base_,
                    other.maxExp_ - other.minExp_ + 1);
            minExp_ += other.minExp_;
            maxExp_ += other.maxExp_;
            base_ = minExp_;

            delete[] coeff_;
            coeff_ = newCoeff;
            return *this;
        }

        /**
         * Adds the product of the two given polynomials to this.
         * This is a common operation in (for example) inner products
         * and matrix multiplication.
         *
         * Calling `x.addProduct(y, z)` is equivalent to, but often faster
         * than, calling `x += y * z`.
         *
         * \param x the first polynomial in the product to add to this.
         * \param y the second polynomial in the product to add to this.
         */
        void addProduct(const Laurent<T>& x, const Laurent<T>& y) {
            if (! (x.coeff_ && y.coeff_)) {
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
                // The following line ensures that coeff_ becomes non-null.
                reallocateForRange(x.minExp_ + y.minExp_,
                    x.maxExp_ + y.maxExp_);

                productBest<SetOrAdd::Add, false>(
                    coeff_ + x.minExp_ + y.minExp_ - base_,
                    x.coeff_ + x.minExp_ - x.base_, x.maxExp_ - x.minExp_ + 1,
                    y.coeff_ + y.minExp_ - y.base_, y.maxExp_ - y.minExp_ + 1);

                // We might have zeroed out some coefficients.
                fixDegrees();
            }
        }

        /**
         * Adds the product of the two given polynomials to this.
         * This is a common operation in (for example) inner products
         * and matrix multiplication.
         *
         * Calling `x.addProduct(y, z)` is equivalent to, but often faster
         * than, calling `x += y * z`.
         *
         * \param x the first polynomial in the product to add to this.
         * \param y the second polynomial in the product to add to this.
         */
        void addProduct(Laurent<T>&& x, Laurent<T>&& y) {
            if (! (x.coeff_ && y.coeff_)) {
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
                // The following line ensures that coeff_ becomes non-null.
                reallocateForRange(x.minExp_ + y.minExp_,
                    x.maxExp_ + y.maxExp_);

                productBest<SetOrAdd::Add, true>(
                    coeff_ + x.minExp_ + y.minExp_ - base_,
                    x.coeff_ + x.minExp_ - x.base_, x.maxExp_ - x.minExp_ + 1,
                    y.coeff_ + y.minExp_ - y.base_, y.maxExp_ - y.minExp_ + 1);

                // We might have zeroed out some coefficients.
                fixDegrees();
            }
        }

        /**
         * Multiplies this with the given polynomial using the given algorithm.
         * This polynomial will not be changed.
         *
         * This routine is provided mainly for timing, testing and diagnostics.
         * If you just wish to multiply two polynomials, you should use the
         * usual product operators (e.g., `x = y * z`): this way Regina will
         * choose the most suitable algorithm for you.
         *
         * The Karatsuba algorithm is not available for all polynomials: it
         * requires the two polynomials to have comparable degree spans
         * (i.e., one polynomial cannot be significantly longer than the other.
         * At present, this means (roughly) that the shorter polynomial should
         * be more than half the length of the longer polynomial.
         * The precise constraints are subject to change in future versions of
         * Regina, and so if you are forcing Karatsuba multiplication then it
         * is strongly recommended that you wrap this in a try/catch block.
         *
         * As a special case, if this or the given polynomial is zero or
         * constant then the given algorithm will be ignored, and this routine
         * will simply use scalar multiplication instead.
         *
         * \python Since Python does not support C++ templates, you should
         * pass the algorithm at runtime as a second argument.  For example,
         * to compute the product `p * q` you could call
         * `p.product(q, PolynomialProductAlgorithm.karatsuba)`.
         *
         * \exception InvalidArgument The algorithm argument requested
         * Karatsuba multiplication, but this polynomial and \a rhs do not
         * have comparable degree spans.  See above for further explanation.
         *
         * \tparam algorithm the polynomial multiplication algorithm to use.
         *
         * \param rhs the polynomial to multiply with this.
         * \return the product of this and the given polynomial.
         */
        template <PolynomialProductAlgorithm algorithm>
        Laurent product(const Laurent& rhs) const {
            if (! (coeff_ && rhs.coeff_)) {
                return {}; // zero
            } else if (minExp_ == maxExp_) {
                return (rhs * coeff_[minExp_ - base_]).shifted(minExp_);
            } else if (rhs.minExp_ == rhs.maxExp_) {
                return ((*this) * rhs.coeff_[rhs.minExp_ - rhs.base_]).shifted(
                    rhs.minExp_);
            } else {
                // Both polynomials have more than one non-zero coefficient.
                T* coeff =
                    new T[maxExp_ - minExp_ + rhs.maxExp_ - rhs.minExp_ + 1];
                if constexpr (algorithm ==
                        PolynomialProductAlgorithm::Classic) {
                    productClassic<SetOrAdd::Either>(coeff,
                        coeff_ + minExp_ - base_, maxExp_ - minExp_ + 1,
                        rhs.coeff_ + rhs.minExp_ - rhs.base_,
                            rhs.maxExp_ - rhs.minExp_ + 1);
                } else if constexpr (algorithm ==
                        PolynomialProductAlgorithm::Karatsuba) {
                    // We need to find a suitable block size.
                    // See the implementation of productBest() for an
                    // explanation of the logic behind this.
                    size_t lhsLen = maxExp_ - minExp_ + 1;
                    size_t rhsLen = rhs.maxExp_ - rhs.minExp_ + 1;
                    if ((lhsLen << 1) <= rhsLen + 1 ||
                            (rhsLen << 1) <= lhsLen + 1) {
                        // One polynomial is _much_ longer than the other.
                        throw InvalidArgument("Karatsuba multiplication "
                            "requires the polynomials to have comparable "
                            "degree spans");
                    } else {
                        productKaratsuba<SetOrAdd::Either, false>(coeff,
                            coeff_ + minExp_ - base_, lhsLen,
                            rhs.coeff_ + rhs.minExp_ - rhs.base_, rhsLen);
                    }
                } else {
                    productBest<SetOrAdd::Either, false>(coeff,
                        coeff_ + minExp_ - base_, maxExp_ - minExp_ + 1,
                        rhs.coeff_ + rhs.minExp_ - rhs.base_,
                            rhs.maxExp_ - rhs.minExp_ + 1);
                }

                // Note: the final minExp/maxExp coefficients will both be
                // non-zero, since the same is true of both this and rhs.
                return { minExp_ + rhs.minExp_, maxExp_ + rhs.maxExp_, coeff };
            }
        }

        /**
         * Writes this polynomial to the given output stream, using the
         * given variable name instead of \c x.
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
         * \param variable the symbol to use for the variable in this
         * polynomial.  This may be \c null, in which case the default
         * variable \c x will be used.
         */
        void writeTextShort(std::ostream& out, bool utf8 = false,
                const char* variable = nullptr) const {
            if (! coeff_) {
                out << '0';
                return;
            }

            // Both minExp_ and maxExp_ have non-zero coefficients (though
            // minExp_ and maxExp_ might be the same exponent).
            auto it = coeff_ + maxExp_ - base_;
            for (long exp = maxExp_; exp >= minExp_; --exp, --it) {
                if (*it == 0)
                    continue;

                T writeCoeff = *it;
                if (exp == maxExp_) {
                    // This is the first term being output.
                    if (writeCoeff < 0) {
                        if (utf8)
                            out << "\u2212";
                        else
                            out << '-';
                        writeCoeff.negate();
                    }
                } else {
                    if (writeCoeff < 0) {
                        if (utf8)
                            out << " \u2212 ";
                        else
                            out << " - ";
                        writeCoeff.negate();
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

        /**
         * Returns this polynomial as a human-readable string, using the
         * given variable name instead of \c x.
         *
         * \note There is also the usual variant of str() which takes no
         * arguments; that variant is inherited from the Output class.
         *
         * \param variable the symbol to use for the variable in this
         * polynomial.  This may be \c null, in which case the default
         * variable \c x will be used.
         * \return this polynomial as a human-readable string.
         */
        std::string str(const char* variable) const {
            std::ostringstream out;
            writeTextShort(out, false, variable);
            return std::move(out).str();
        }

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
         * \param variable the symbol to use for the variable in this
         * polynomial.  This may be \c null, in which case the default
         * variable \c x will be used.
         * \return this polynomial as a unicode-enabled human-readable string.
         */
        std::string utf8(const char* variable) const {
            std::ostringstream out;
            writeTextShort(out, true, variable);
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
            // terminate with zero.
            if (coeff_) {
                auto it = coeff_ + minExp_ - base_;
                for (long exp = minExp_; exp <= maxExp_; ++exp, ++it)
                    if (*it != 0) {
                        it->tightEncode(out);
                        regina::tightEncode(out, exp);
                    }
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
            // Use a temporary std::vector to store non-zero coefficients,
            // since we don't know in advance how many there will be.
            std::vector<std::pair<long, T>> coeffs;

            while (true) {
                T coeff = T::tightDecode(input);
                if (coeff == 0) {
                    // The sequence of coefficients is finished.
                    if (coeffs.empty())
                        return {};

                    long firstExp = coeffs.front().first;
                    long lastExp = coeffs.back().first;
                    T* raw = new T[lastExp - firstExp + 1];
                    for (auto& c : coeffs)
                        raw[c.first - firstExp] = std::move(c.second);
                    return { firstExp, lastExp, raw };
                } else {
                    long exp = regina::tightDecode<long>(input);
                    if (! coeffs.empty()) {
                        if (exp <= coeffs.back().first)
                            throw InvalidInput("The tight encoding has an "
                                "invalid sequence of exponents");
                    }
                    coeffs.emplace_back(exp, std::move(coeff));
                }
            }
        }

    private:
        /**
         * Constructs a new polynomial with the given exponent range and
         * coefficients.  It is assumed that the coefficient array starts
         * at exponent \a minExp, and has capacity `maxExp - minExp + 1`.
         * The new object will take ownership of the given coefficient array.
         *
         * The coefficient array may have leading or trailing zeroes,
         * but if this is a possibility then you _must_ pass
         * \a checkZeroes as `true`.
         *
         * \pre The argument \a coeff is both non-null and non-empty.
         */
        Laurent(long minExp, long maxExp, T* coeff, bool checkZeroes = false) :
                minExp_(minExp), maxExp_(maxExp), base_(minExp),
                capacity_(maxExp - minExp + 1), coeff_(coeff) {
            // The preconditions guarantee coeff_ is non-null and non-empty.
            if (checkZeroes)
                fixDegrees();
        }

        /**
         * Ensures that `minExp_ ≤ newMin_` and `maxExp_ ≥ newMax`, and grows
         * the array of coefficients accordingly if this is necessary.
         * As a result, the coefficient array will be non-empty (and in
         * particular, \a coeff_ will be non-null).
         *
         * The value of \a minExp_ might decrease, but it will not increase.
         * The value of \a maxExp_ might increase, but it will not decrease.
         * If \a minExp_ and/or \a maxExp_ did change, then all new coefficients
         * in the expanded range will be set to zero.
         *
         * Note that the resulting polynomial might have zero
         * coefficients at the exponents \a minExp_ and/or \a maxExp_.
         *
         * This routine is used (for example) in the implementations
         * of += and -=.
         *
         * \pre The given range satisfies `newMin ≤ newMax`.
         */
        void reallocateForRange(long newMin, long newMax) {
            // TODO: This needs very thorough testing.
            // Note: type T will automatically initialise any newly allocated
            // coefficients to zero.
            if (! coeff_) {
                minExp_ = base_ = newMin;
                maxExp_ = newMax;
                capacity_ = newMax - newMin + 1;
                coeff_ = new T[capacity_];
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

                    T* newCoeff = new T[capacity_];
                    std::move(coeff_ + minExp_ - base_,
                        coeff_ + maxExp_ - base_ + 1,
                        newCoeff + minExp_ - newBase);
                    delete[] coeff_;
                    coeff_ = newCoeff;

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
                        std::move_backward(coeff_ + minExp_ - base_,
                            coeff_ + maxExp_ - base_ + 1,
                            coeff_ + maxExp_ - base_ + shift + 1);
                        base_ -= shift;
                    } else if (maxShift < 0) {
                        // We must shift out data to the left.
                        long shift = (-(minShift + maxShift) + 1) >> 1;
                        std::move(coeff_ + minExp_ - base_,
                            coeff_ + maxExp_ - base_ + 1,
                            coeff_ + minExp_ - base_ - shift);
                        base_ += shift;
                    }

                    // Fix minExp_ and maxExp_, and zero out any extra
                    // coefficients on either end.
                    if (newMin < minExp_) {
                        for (auto it = coeff_ + newMin - base_;
                                it != coeff_ + minExp_ - base_; ++it)
                            *it = 0;
                        minExp_ = newMin;
                    }
                    if (newMax > maxExp_) {
                        for (auto it = coeff_ + maxExp_ - base_ + 1;
                                it <= coeff_ + newMax - base_; ++it)
                            *it = 0;
                        maxExp_ = newMax;
                    }
                }
            }
        }

        /**
         * Increases \a minExp_ and/or decreases \a maxExp_ to ensure
         * that both exponents \a minExp_ and \a maxExp_ have non-zero
         * coefficients.  If this is the zero polynomial then all of
         * \a minExp_, \a maxExp_ and \a base_ will be set to zero.
         *
         * \pre The data member \a coeff_ is non-null.
         */
        void fixDegrees() {
            while (maxExp_ > minExp_ && coeff_[maxExp_ - base_] == 0)
                --maxExp_;
            while (minExp_ < maxExp_ && coeff_[minExp_ - base_] == 0)
                ++minExp_;
            if (minExp_ == maxExp_ && coeff_[minExp_ - base_] == 0) {
                // We have the zero polynomial now.
                delete[] coeff_;
                coeff_ = nullptr;
            }
        }

        /**
         * Replaces the contents of this polynomial with \a other - \a this.
         *
         * This is equivalent to calling the -= operator and then negating.
         *
         * \param other the polynomial to subtract this from.
         * \return a reference to this polynomial.
         */
        Laurent<T>& subtractFrom(const Laurent<T>& other) {
            // This routine works even if &other == this, since in this case
            // we do not reallocate.

            if (! other.coeff_) {
                negate();
                return *this;
            }

            reallocateForRange(other.minExp_, other.maxExp_); // sets coeff_ ≠ 0

            long exp = (minExp_ < other.minExp_ ? minExp_ : other.minExp_);
            for ( ; exp < other.minExp_; ++exp)
                if (coeff_[exp - base_] != 0)
                    coeff_[exp - base_].negate();
            for ( ; exp <= other.maxExp_; ++exp)
                if (coeff_[exp - base_] != 0)
                    coeff_[exp - base_] = other.coeff_[exp - other.base_]
                        - std::move(coeff_[exp - base_]);
                else
                    coeff_[exp - base_] = other.coeff_[exp - other.base_];
            for ( ; exp <= maxExp_; ++exp)
                if (coeff_[exp - base_] != 0)
                    coeff_[exp - base_].negate();

            // We might have zeroed out some coefficients.
            fixDegrees();
            return *this;
        }

        /**
         * Multiplies two ranges of coefficients using the classical polynomial
         * multiplication algorithm.
         *
         * This involves scanning through both input ranges in a pair of nested
         * loops, and so to multiply two polynomials with degree span \a n
         * requires computing `O(n^2)` individual products of coefficients.
         *
         * See productBest() for details of how the input and output ranges
         * work, how the results are stored in the output range, and what the
         * individual function arguments mean.
         *
         * \pre Both \a lhsLen and \a rhsLen are strictly positive.
         */
        template <SetOrAdd operation>
        static void productClassic(T* dest,
                Buffer<false> lhs, size_t lhsLen,
                Buffer<false> rhs, size_t rhsLen) {
            if constexpr (operation == SetOrAdd::Set) {
                // We have to add (not set) inside our nested loops, since we
                // will be revisiting the same destination exponents over and
                // over.  Therefore we need to explicitly initialise them to
                // zero now.
                std::fill(dest, dest + lhsLen + rhsLen - 1, T());
            }
            if constexpr (HasAddProduct<T>) {
                for (size_t i = 0; i < lhsLen; ++i)
                    for (size_t j = 0; j < rhsLen; ++j)
                        dest[i + j].addProduct(lhs[i], rhs[j]);
            } else {
                for (size_t i = 0; i < lhsLen; ++i)
                    for (size_t j = 0; j < rhsLen; ++j)
                        dest[i + j] += lhs[i] * rhs[j];
            }
        }

        /**
         * Multiplies two ranges of coefficients using Karatsuba's polynomial
         * multiplication algorithm.
         *
         * Karatsuba's algorithm is a divide-and-conquer algorithm: to multiply
         * two polynomials with the same degree span \a n, it requires computing
         * `O(n^(log₂ 3)) ≃ O(n^1.585)` individual products of coefficients.
         *
         * See productBest() for details of how the input and output ranges
         * work, how the results are stored in the output range, and what the
         * individual function arguments mean.
         *
         * \pre The input range lengths are not too unbalanced.  Specifically,
         * if we let \a minLen and \a maxLen denote the smaller and larger of
         * \a lhsLen and \a rhsLen, then `2 * minLen > max + 1`.
         *
         * \tparam moveable `true` if we are allowed to move data out of the
         * input coefficients, or `false` if the input coefficients should be
         * read-only.
         */
        template <SetOrAdd operation, bool moveable>
        static void productKaratsuba(T* dest,
                Buffer<moveable> lhs, size_t lhsLen,
                Buffer<moveable> rhs, size_t rhsLen,
                T* scratch = nullptr) {
            // The algorithm works by dividing each range of coefficients into
            // two _blocks_ (an upper and lower block).
            //
            // The lower blocks will both have length *blockSize*, and the
            // upper blocks will have lengths *lhsUpperLen* and *rhsUpperLen*.
            // From the preconditions on *minLen* and *maxLen* we have:
            //   1 ≤ {lhsUpperLen,rhsUpperLen}
            //     ≤ blockSize < minLen ≤ maxLen ≤ 2*blockSize ≤ maxLen+1.
            size_t blockSize = (std::max(lhsLen, rhsLen) + 1) >> 1;
            size_t lhsUpperLen = lhsLen - blockSize;
            size_t rhsUpperLen = rhsLen - blockSize;

            // Compute the length of the output range coming from the two
            // lower blocks.  Here we have: 1 ≤ blockSize ≤ blockOutputLen.
            size_t blockOutputLen = (blockSize << 1) - 1;

            // Regarding scratch space: we are guaranteed that
            //   |scratch| ≥ 2(maxLen - 1 + log_2(maxLen - 1)).
            //
            // Since maxLen ≥ 2, this means |scratch| ≥ 2 * blockSize
            // (note: the proof of this requires special-casing maxLen = 2).
            // Therefore we have enough space for the 2 * blockSize temporary
            // coefficients that we use below.
            //
            // If we recurse, it will be with input ranges of length at most
            // blockSize ≤ (maxLen + 1) / 2.  In this case, the scratch space
            // remaining will have size at least:
            //     2(maxLen - 1 + log_2(maxLen - 1)) - 2 * blockSize
            //   ≥ 2(2*blockSize - 2 + log_2(2*blockSize - 2)) - 2 * blockSize
            //   = 2(blockSize - 2 + log_2(blockSize - 1) + 1)
            //   = 2(blockSize - 1 + log_2(blockSize - 1)),
            // which satisfies our scratch space requirements for the recursion.

            // Set up our scratch space now if we do not already have it.
            // Note: ceil(log_2(k)) == regina::bitsRequired(k).
            T* tmp;
            if (scratch) {
                tmp = scratch;
            } else {
                size_t m = std::max(lhsLen, rhsLen) - 1;
                tmp = new T[(m + bitsRequired(m)) << 1];
            }

            size_t i;
            switch (operation) {
                case SetOrAdd::Either:
                case SetOrAdd::Set:
                    // The following code sets elements dest[i] where
                    // 0 ≤ i < 3 * blockSize - 1.
                    productBest<SetOrAdd::Set, false>(tmp,
                        lhs, blockSize, rhs, blockSize,
                        tmp + blockOutputLen);
                    for (i = 0; i < blockOutputLen; ++i)
                        dest[i + blockSize] = -tmp[i];
                    for (i = 0; i < blockSize; ++i)
                        dest[i] = std::move(tmp[i]);
                    for ( ; i < blockOutputLen; ++i)
                        dest[i] += std::move(tmp[i]);

                    // The following code sets elements dest[i] where
                    // blockSize ≤ i < lhsLen + rhsLen - 1.
                    // Note that, from the inequalities above, we can derive
                    // blockSize ≤ lhsUpperLen + rhsUpperLen (which means the
                    // loop bounds below are fine).
                    productBest<SetOrAdd::Set, false>(tmp,
                        lhs + blockSize, lhsUpperLen,
                        rhs + blockSize, rhsUpperLen,
                        tmp + blockOutputLen);
                    for (i = 0; i < blockSize - 1; ++i)
                        dest[i + (blockSize << 1)] += tmp[i];
                    for ( ; i < lhsUpperLen + rhsUpperLen - 1; ++i)
                        dest[i + (blockSize << 1)] = tmp[i];
                    for (i = 0; i < lhsUpperLen + rhsUpperLen - 1; ++i)
                        dest[i + blockSize] -= std::move(tmp[i]);
                    break;
                case SetOrAdd::Add:
                    productBest<SetOrAdd::Set, false>(tmp,
                        lhs, blockSize, rhs, blockSize,
                        tmp + blockOutputLen);
                    for (i = 0; i < blockOutputLen; ++i) {
                        dest[i + blockSize] -= tmp[i];
                        dest[i] += std::move(tmp[i]);
                    }
                    productBest<SetOrAdd::Set, false>(tmp,
                        lhs + blockSize, lhsUpperLen,
                        rhs + blockSize, rhsUpperLen,
                        tmp + blockOutputLen);
                    for (i = 0; i < lhsUpperLen + rhsUpperLen - 1; ++i) {
                        dest[i + blockSize] -= tmp[i];
                        dest[i + (blockSize << 1)] += std::move(tmp[i]);
                    }
                    break;
            }

            if constexpr (moveable) {
                for (i = 0; i < lhsUpperLen; ++i)
                    tmp[i] = std::move(lhs[i]) + std::move(lhs[blockSize + i]);
                for ( ; i < blockSize; ++i)
                    tmp[i] = std::move(lhs[i]);
                for (i = 0; i < rhsUpperLen; ++i)
                    tmp[blockSize + i] = std::move(rhs[i]) +
                        std::move(rhs[blockSize + i]);
                for ( ; i < blockSize; ++i)
                    tmp[blockSize + i] = std::move(rhs[i]);
            } else {
                for (i = 0; i < lhsUpperLen; ++i)
                    tmp[i] = lhs[i] + lhs[blockSize + i];
                for ( ; i < blockSize; ++i)
                    tmp[i] = lhs[i];
                for (i = 0; i < rhsUpperLen; ++i)
                    tmp[blockSize + i] = rhs[i] + rhs[blockSize + i];
                for ( ; i < blockSize; ++i)
                    tmp[blockSize + i] = rhs[i];
            }

            productBest<SetOrAdd::Add, true /* moveable */>(dest + blockSize,
                tmp, blockSize, tmp + blockSize, blockSize,
                tmp + (blockSize << 1));

            if (! scratch)
                delete[] tmp;
        }

        /**
         * Multiplies two ranges of coefficients using whatever polynomial
         * multiplication algorithm this routine deems best.
         *
         * The input consists of two read-only C-style arrays, each given by a
         * starting pointer and the number of coefficients.  The output will
         * consist of `lhsLen + rhsLen - 1` coefficients, which will likewise
         * be stored in a C-style array.  Whether they are stored via assignment
         * or addition depends on the template parameter \a operation.
         *
         * Each range of input coefficients is given in order from lowest
         * degree to highest degree, and may include zeroes at the endpoints.
         * Likewise, the output coefficients will be stored in order from
         * lowest degree to highest degree, and may include zeroes at the
         * endpoints.
         *
         * \pre Both \a lhsLen and \a rhsLen are strictly positive.
         *
         * \pre If \a scratch is non-null, then it should point to a C-style
         * array of size at least `2(m-1 + log_2(m-1))`, where \a m denotes
         * the larger of \a lhsLen and \a rhsLen.
         *
         * \tparam operation indicates whether the output coefficients
         * should be stored via assignment (`dest[i] = result`), addition
         * (`dest[i] += result`), or whichever is fastest (if `dest[i]` is
         * known to have been initialised to zero).
         *
         * \tparam moveable `true` if we are allowed to move data out of the
         * input coefficients, or `false` if the input coefficients should be
         * read-only.
         *
         * \param dest the C-style array in which the output coefficients
         * should be stored.
         * \param lhs the C-style array holding the first range of input
         * coefficients.
         * \param lhsLen the total number of coefficients in the first input
         * range.
         * \param rhs the C-style array holding the second range of input
         * coefficients.
         * \param rhsLen the total number of coefficients in the second input
         * range.
         * \param scratch pre-allocated scratch space, or `null` if the product
         * algorithm should create its own scratch space if/when required.  The
         * scratch space does not need to be initialised in any particular way.
         */
        template <SetOrAdd operation, bool moveable>
        static void productBest(T* dest,
                Buffer<moveable> lhs, size_t lhsLen,
                Buffer<moveable> rhs, size_t rhsLen,
                T* scratch = nullptr) {
            if constexpr (karatsubaThreshold<T> == 0) {
                // Do not use Karatsuba multiplication at all.
                productClassic<operation>(dest, lhs, lhsLen, rhs, rhsLen);
            } else {
                // We need to decide if/how to use Karatsuba multiplication.
                if (lhsLen < karatsubaThreshold<T> ||
                        rhsLen < karatsubaThreshold<T>) {
                    // One of the polynomials is tiny.
                    // Just use classic multiplication.
                    productClassic<operation>(dest, lhs, lhsLen, rhs, rhsLen);
                } else if ((lhsLen << 1) <= rhsLen + 1) {
                    // We have rhs much longer than lhs.
                    // Break rhs into blocks of size lhsLen, and use Karatsuba
                    // multiplication on each (except possibly the last).
                    // (Note that, since lhsLen and rhsLen are both positive,
                    // just the test above will guarantee lhsLen ≤ rhsLen.)
                    #if 1
                    productClassic<operation>(dest, lhs, lhsLen, rhs, rhsLen);
                    #else
                    if constexpr (operation == SetOrAdd::Set) {
                        // Since the destination ranges overlap for each block
                        // operation, we need to initialise the destination
                        // coefficients and then add.
                        std::fill(dest, dest + lhsLen + rhsLen - 1, T());
                    }
                    while (rhsLen >= lhsLen) {
                        productKaratsuba<SetOrAdd::Add, false>(
                            dest, lhs, lhsLen, rhs, lhsLen, scratch);
                        rhsLen -= lhsLen;
                        rhs += lhsLen;
                        dest += lhsLen;
                    }
                    if (rhsLen)
                        productBest<SetOrAdd::Add, moveable>(dest, lhs, lhsLen,
                            rhs, rhsLen, scratch);
                    #endif
                } else if ((rhsLen << 1) <= lhsLen + 1) {
                    // We have lhs much longer than rhs.
                    // Like above, but with LHS and RHS swapped.
                    #if 1
                    productClassic<operation>(dest, lhs, lhsLen, rhs, rhsLen);
                    #else
                    if constexpr (operation == SetOrAdd::Set) {
                        std::fill(dest, dest + lhsLen + rhsLen - 1, T());
                    }
                    while (lhsLen >= rhsLen) {
                        productKaratsuba<SetOrAdd::Add, false>(
                            dest, lhs, rhsLen, rhs, rhsLen, scratch);
                        lhsLen -= rhsLen;
                        lhs += rhsLen;
                        dest += rhsLen;
                    }
                    if (lhsLen)
                        productBest<SetOrAdd::Add, moveable>(dest, lhs, lhsLen,
                            rhs, rhsLen, scratch);
                    #endif
                } else {
                    // Let maxLen = max(lhsLen, rhsLen).
                    // Then we use a block size of ceil(maxLen / 2).
                    //
                    // To see why this works: WLOG, if lhsLen ≤ rhsLen then:
                    //   blockSize ≤ (rhsLen + 1) / 2
                    //             < lhsLen [from the tests above]
                    //             ≤ rhsLen ≤ 2 * blockSize.
                    productKaratsuba<operation, moveable>(dest,
                        lhs, lhsLen, rhs, rhsLen, scratch);
                }
            }
        }

    template <CoefficientDomain U>
    friend Laurent<U> operator + (const Laurent<U>&, const Laurent<U>&);

    template <CoefficientDomain U>
    friend Laurent<U> operator - (const Laurent<U>&, const Laurent<U>&);

    template <CoefficientDomain U>
    friend Laurent<U> operator - (const Laurent<U>&, Laurent<U>&&);

    template <CoefficientDomain U>
    friend Laurent<U> operator * (const Laurent<U>&, const Laurent<U>&);

    template <CoefficientDomain U>
    friend Laurent<U> operator * (Laurent<U>&&, Laurent<U>&&);
};

/**
 * Swaps the contents of the given polynomials.
 *
 * This global routine simply calls Laurent<T>::swap(); it is provided
 * so that Laurent<T> meets the C++ Swappable requirements.
 *
 * \param a the first polynomial whose contents should be swapped.
 * \param b the second polynomial whose contents should be swapped.
 *
 * \ingroup maths
 */
template <CoefficientDomain T>
void swap(Laurent<T>& a, Laurent<T>& b) noexcept {
    a.swap(b);
}

/**
 * Multiplies the given polynomial by the given scalar constant.
 *
 * The scalar is simply of type \a T; we use the identical type
 * Laurent<T>::Coefficient here to assist with C++ template type matching.
 *
 * \param poly the polynomial to multiply by.
 * \param scalar the scalar to multiply by.
 * \return the product of the given polynomial and scalar.
 *
 * \ingroup maths
 */
template <CoefficientDomain T>
Laurent<T> operator * (Laurent<T> poly,
        const typename Laurent<T>::Coefficient& scalar) {
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
template <CoefficientDomain T, CppInteger IntType>
Laurent<T> operator * (Laurent<T> poly, IntType scalar) {
    // See the notes above on a possible optimisation for scalar == 0.
    poly *= scalar;
    return poly;
}

/**
 * Multiplies the given polynomial by the given scalar constant.
 *
 * The scalar is simply of type \a T; we use the identical type
 * Laurent<T>::Coefficient here to assist with C++ template type matching.
 *
 * \param scalar the scalar to multiply by.
 * \param poly the polynomial to multiply by.
 * \return the product of the given polynomial and scalar.
 *
 * \ingroup maths
 */
template <CoefficientDomain T>
Laurent<T> operator * (const typename Laurent<T>::Coefficient& scalar,
        Laurent<T> poly) {
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
template <CoefficientDomain T, CppInteger IntType>
Laurent<T> operator * (IntType scalar, Laurent<T> poly) {
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
 * Laurent<T>::Coefficient here to assist with C++ template type matching.
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
Laurent<T> operator / (Laurent<T> poly,
        const typename Laurent<T>::Coefficient& scalar) {
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
template <CoefficientDomain T, CppInteger IntType>
Laurent<T> operator / (Laurent<T> poly, IntType scalar) {
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
template <CoefficientDomain T>
Laurent<T> operator + (const Laurent<T>& lhs, const Laurent<T>& rhs) {
    // Handle zero polynomials separately.
    if (! lhs.coeff_)
        return rhs;
    if (! rhs.coeff_)
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

    // exp is now min(lhs.maxExp_, rhs.maxExp_) + 1.
    if (exp <= lhs.maxExp_) {
        std::copy(lhs.coeff_ + exp - lhs.base_,
            lhs.coeff_ + lhs.maxExp_ + 1 - lhs.base_, coeff + idx);
    } else if (exp <= rhs.maxExp_) {
        std::copy(rhs.coeff_ + exp - rhs.base_,
            rhs.coeff_ + rhs.maxExp_ + 1 - rhs.base_, coeff + idx);
    }

    return Laurent<T>(minExp, maxExp, coeff, true);
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
template <CoefficientDomain T>
Laurent<T> operator + (Laurent<T>&& lhs, const Laurent<T>& rhs) {
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
template <CoefficientDomain T>
Laurent<T> operator + (const Laurent<T>& lhs, Laurent<T>&& rhs) {
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
template <CoefficientDomain T>
Laurent<T> operator + (Laurent<T>&& lhs, Laurent<T>&& rhs) {
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
template <CoefficientDomain T>
Laurent<T> operator - (Laurent<T> arg) {
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
template <CoefficientDomain T>
Laurent<T> operator - (const Laurent<T>& lhs, const Laurent<T>& rhs) {
    // Handle zero polynomials separately.
    if (! rhs.coeff_)
        return lhs;
    if (! lhs.coeff_)
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

    // exp is now min(lhs.maxExp_, rhs.maxExp_) + 1.
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

/**
 * Subtracts the two given polynomials.
 *
 * \param lhs the polynomial to sutract \a rhs from.
 * \param rhs the polynomial to subtract from \a lhs.
 * \return the difference of the two given polynomials.
 *
 * \ingroup maths
 */
template <CoefficientDomain T>
Laurent<T> operator - (Laurent<T>&& lhs, const Laurent<T>& rhs) {
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
template <CoefficientDomain T>
Laurent<T> operator - (const Laurent<T>& lhs, Laurent<T>&& rhs) {
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
template <CoefficientDomain T>
Laurent<T> operator - (Laurent<T>&& lhs, Laurent<T>&& rhs) {
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
template <CoefficientDomain T>
Laurent<T> operator * (const Laurent<T>& lhs, const Laurent<T>& rhs) {
    if (! (lhs.coeff_ && rhs.coeff_)) {
        return {}; // zero
    } else if (lhs.minExp_ == lhs.maxExp_) {
        return (rhs * lhs.coeff_[lhs.minExp_ - lhs.base_]).shifted(
            lhs.minExp_);
    } else if (rhs.minExp_ == rhs.maxExp_) {
        return (lhs * rhs.coeff_[rhs.minExp_ - rhs.base_]).shifted(
            rhs.minExp_);
    } else {
        // Both polynomials have more than one non-zero coefficient.
        T* coeff =
            new T[lhs.maxExp_ - lhs.minExp_ + rhs.maxExp_ - rhs.minExp_ + 1];
        Laurent<T>::template productBest<Laurent<T>::SetOrAdd::Either, false>(
            coeff,
            lhs.coeff_ + lhs.minExp_ - lhs.base_,
                lhs.maxExp_ - lhs.minExp_ + 1,
            rhs.coeff_ + rhs.minExp_ - rhs.base_,
                rhs.maxExp_ - rhs.minExp_ + 1);

        // Note: the final minExp/maxExp coefficients will both be non-zero,
        // since the same is true of both lhs and rhs.
        return { lhs.minExp_ + rhs.minExp_, lhs.maxExp_ + rhs.maxExp_, coeff };
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
template <CoefficientDomain T>
Laurent<T> operator * (Laurent<T>&& lhs, Laurent<T>&& rhs) {
    if (! (lhs.coeff_ && rhs.coeff_)) {
        return {}; // zero
    } else if (lhs.minExp_ == lhs.maxExp_) {
        rhs *= lhs.coeff_[lhs.minExp_ - lhs.base_];
        rhs.shift(lhs.minExp_);
        return std::move(rhs);
    } else if (rhs.minExp_ == rhs.maxExp_) {
        lhs *= rhs.coeff_[rhs.minExp_ - rhs.base_];
        lhs.shift(rhs.minExp_);
        return std::move(lhs);
    } else {
        // Both polynomials have more than one non-zero coefficient.
        T* coeff =
            new T[lhs.maxExp_ - lhs.minExp_ + rhs.maxExp_ - rhs.minExp_ + 1];
        Laurent<T>::template productBest<Laurent<T>::SetOrAdd::Either, true>(
            coeff,
            lhs.coeff_ + lhs.minExp_ - lhs.base_,
                lhs.maxExp_ - lhs.minExp_ + 1,
            rhs.coeff_ + rhs.minExp_ - rhs.base_,
                rhs.maxExp_ - rhs.minExp_ + 1);

        // Note: the final minExp/maxExp coefficients will both be non-zero,
        // since the same is true of both lhs and rhs.
        return { lhs.minExp_ + rhs.minExp_, lhs.maxExp_ + rhs.maxExp_, coeff };
    }
}

#ifndef __DOXYGEN
// Don't confuse doxygen with specialisations.
template <CoefficientDomain T>
struct RingTraits<Laurent<T>> {
    inline static const Laurent<T> zero;
    inline static const Laurent<T> one { 0, { 1 } };
    static constexpr bool commutative = RingTraits<T>::commutative;
    static constexpr bool zeroInitialised = true;
    static constexpr bool zeroDivisors = false; // since T is a domain
    static constexpr bool inverses = false;
};
#endif // __DOXYGEN

// Inline constants for Laurent:

template <CoefficientDomain T>
const T Laurent<T>::zero_(0);

} // namespace regina

#endif
