
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

#ifndef __REGINA_POLYOPS_H
#ifndef __DOXYGEN
#define __REGINA_POLYOPS_H
#endif

/*! \file maths/polyops.h
 *  \brief Contains shared implementations for operations on single-variable
 *  polynomial types.
 */

#include "concepts/core.h"
#include "utilities/fixedarray.h"

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

template <CoefficientDomain> class Laurent;
template <CoefficientDomain> class Laurent2;
template <CoefficientDomain> class Polynomial;

/**
 * Represents different algorithms for multiplying polynomials.
 *
 * \ingroup maths
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
 * At a bare minimum, if `karatsubaThreshold<T>` _is_ positive, then it must
 * be at least 2 (otherwise divide-and-conquer is not possible).
 *
 * \nopython
 *
 * \ingroup maths
 */
template <CoefficientDomain T>
static constexpr size_t karatsubaThreshold = 16;

#ifndef __DOXYGEN
// For now, do not use Karatsuba multiplication when working with "nested"
// polynomials (e.g., Laurent<Laurent<T>>).
template <CoefficientDomain T>
static constexpr size_t karatsubaThreshold<Laurent<T>> = 0;
template <CoefficientDomain T>
static constexpr size_t karatsubaThreshold<Laurent2<T>> = 0;
template <CoefficientDomain T>
static constexpr size_t karatsubaThreshold<Polynomial<T>> = 0;
#endif

namespace detail {

/**
 * A pointer to a C-style array of coefficients of type \a T.
 *
 * Such buffers are used as shared scratch space by internal implementations
 * of polynomial products (in particular, Karatsuba multiplication).
 *
 * \tparam writeable `true` if the coefficients should be read-write,
 * or `false` if they should be read-only.
 */
template <CoefficientDomain T, bool writeable>
using CoefficientBuffer = std::conditional_t<writeable, T*, const T*>;

/**
 * Indicate whether the result of some computation should be added to or
 * assigned to a destination object.  Regina uses this internally when working
 * with polynomial coefficients (in particular, for implementing various
 * polynomial product algorithms).
 *
 * \ingroup detail
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

template <CoefficientDomain T, SetOrAdd, bool moveable>
static void productBest(T*, CoefficientBuffer<T, moveable>, size_t,
        CoefficientBuffer<T, moveable>, size_t, T*);

/**
 * Multiplies two ranges of coefficients using the classical polynomial
 * multiplication algorithm.
 *
 * This involves scanning through both input ranges in a pair of nested
 * loops, and so to multiply two polynomials with degree span \a n
 * requires computing `O(n^2)` individual products of coefficients.
 *
 * See productBest() for details of how the input and output ranges work.
 *
 * \pre Both \a lhsLen and \a rhsLen are strictly positive.
 *
 * \tparam T the coefficient type.
 * \tparam operation indicates whether the output coefficients should be
 * written via assignment (`dest[i] = result`), addition (`dest[i] += result`),
 * or whichever is fastest (which is only appropriate when every cofficient in
 * \a dest has previously been initialised to zero).
 *
 * \param dest the C-style array in which to write the output coefficients.
 * \param lhs the C-style array holding the first range of input coefficients.
 * \param lhsLen the total number of coefficients in the first input range.
 * \param rhs the C-style array holding the second range of input coefficients.
 * \param rhsLen the total number of coefficients in the second input range.
 *
 * \ingroup detail
 */
template <CoefficientDomain T, SetOrAdd operation>
static void productClassic(T* dest,
        CoefficientBuffer<T, false> lhs, size_t lhsLen,
        CoefficientBuffer<T, false> rhs, size_t rhsLen) {
    // We can only add (not set) inside our nested loops, since we will
    // be revisiting the same destination exponents over and over.
    // Therefore, if we have been explicitly asked to _set_ the
    // elements of dest, we must initialise them to zero first.
    if constexpr (operation == SetOrAdd::Set)
        std::fill(dest, dest + lhsLen + rhsLen - 1, T());

    for (size_t i = 0; i < lhsLen; ++i)
        if (lhs[i] != 0)
            for (size_t j = 0; j < rhsLen; ++j) {
                if constexpr (HasAddProduct<T>)
                    dest[i + j].addProduct(lhs[i], rhs[j]);
                else
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
 * See productBest() for details of how the input and output ranges work.
 *
 * Regarding scratch space: this algorithm _will_ need to create its own
 * scratch space if you do not provide your own.  However, it manages this
 * perfectly well itself: the outermost call to productKaratsuba() will create
 * a single block of scratch space for the entire multiplication process, and
 * will pass this down through to its recursive calls to productBest() and
 * productKaratsuba().
 *
 * \pre Both \a lhsLen and \a rhsLen are strictly positive.
 *
 * \pre The input range lengths are not too unbalanced.  Specifically, if we
 * let \a minLen and \a maxLen denote the smaller and larger of \a lhsLen and
 * \a rhsLen, then `2 * minLen > max + 1`.
 *
 * \pre If \a scratch is non-null, then it should point to a C-style array of
 * size at least `2(max-1 + log_2(max-1))`, where \a max denotes the larger of
 * \a lhsLen and \a rhsLen.
 *
 * \tparam T the coefficient type.
 * \tparam operation indicates whether the output coefficients should be
 * written via assignment (`dest[i] = result`), addition (`dest[i] += result`),
 * or whichever is fastest (which is only appropriate when every cofficient in
 * \a dest has previously been initialised to zero).
 * \tparam moveable `true` if we are allowed to move data out of the input
 * coefficients, or `false` if the input coefficients should be read-only.
 *
 * \param dest the C-style array in which to write the output coefficients.
 * \param lhs the C-style array holding the first range of input coefficients.
 * \param lhsLen the total number of coefficients in the first input range.
 * \param rhs the C-style array holding the second range of input coefficients.
 * \param rhsLen the total number of coefficients in the second input range.
 * \param scratch pre-allocated scratch space, or `null` if the product
 * algorithm should create its own scratch space.  The scratch space does not
 * need to be initialised in any particular way.
 *
 * \ingroup detail
 */
template <CoefficientDomain T, SetOrAdd operation, bool moveable>
static void productKaratsuba(T* dest,
        CoefficientBuffer<T, moveable> lhs, size_t lhsLen,
        CoefficientBuffer<T, moveable> rhs, size_t rhsLen,
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
            productBest<T, SetOrAdd::Set, false>(tmp, lhs, blockSize,
                rhs, blockSize, tmp + blockOutputLen);
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
            productBest<T, SetOrAdd::Set, false>(tmp,
                lhs + blockSize, lhsUpperLen, rhs + blockSize, rhsUpperLen,
                tmp + blockOutputLen);
            for (i = 0; i < blockSize - 1; ++i)
                dest[i + (blockSize << 1)] += tmp[i];
            for ( ; i < lhsUpperLen + rhsUpperLen - 1; ++i)
                dest[i + (blockSize << 1)] = tmp[i];
            for (i = 0; i < lhsUpperLen + rhsUpperLen - 1; ++i)
                dest[i + blockSize] -= std::move(tmp[i]);
            break;
        case SetOrAdd::Add:
            productBest<T, SetOrAdd::Set, false>(tmp,
                lhs, blockSize, rhs, blockSize,
                tmp + blockOutputLen);
            for (i = 0; i < blockOutputLen; ++i) {
                dest[i + blockSize] -= tmp[i];
                dest[i] += std::move(tmp[i]);
            }
            productBest<T, SetOrAdd::Set, false>(tmp,
                lhs + blockSize, lhsUpperLen, rhs + blockSize, rhsUpperLen,
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

    productBest<T, SetOrAdd::Add, true /* moveable */>(dest + blockSize,
        tmp, blockSize, tmp + blockSize, blockSize, tmp + (blockSize << 1));

    if (! scratch)
        delete[] tmp;
}

/**
 * Multiplies two ranges of coefficients using whatever polynomial
 * multiplication algorithm this routine deems best.  This should be the main
 * entry point for polynomial types when implementing their own product
 * operations such as `x * y`, `x *= y`, or `x.addProduct(y, z)`.
 *
 * The input consists of two C-style arrays \a lhs and \a rhs, each given by a
 * starting pointer and the number of coefficients.  The output will consist of
 * `lhsLen + rhsLen - 1` coefficients, which will likewise be written to the
 * given C-style array \a dest.  Whether the output coefficients are written via
 * assignment or addition will depend on the template parameter \a operation.
 *
 * Each range of input coefficients is given in order from lowest degree to
 * highest degree, and may include zeroes at the endpoints.  Likewise, the
 * output coefficients will be written in order from lowest degree to highest
 * degree, and may include zeroes at the endpoints.
 *
 * Regarding scratch space: this algorithm may or may not need scratch space
 * (this depends on whether Karatsuba multiplication is used).  However, it
 * manages this perfectly well itself: if it ever discovers that scratch space
 * is required but none was provided, then it will create a single block of
 * scratch space for the entire multiplication process, and will pass this down
 * through to any recursive calls to productKaratsuba() and productBest().
 *
 * \pre Both \a lhsLen and \a rhsLen are strictly positive.
 *
 * \pre If \a scratch is non-null, then it should point to a C-style array of
 * size at least `2(max-1 + log_2(max-1))`, where \a max denotes the larger of
 * \a lhsLen and \a rhsLen.
 *
 * \tparam T the coefficient type.
 * \tparam operation indicates whether the output coefficients should be
 * written via assignment (`dest[i] = result`), addition (`dest[i] += result`),
 * or whichever is fastest (which is only appropriate when every cofficient in
 * \a dest has previously been initialised to zero).
 * \tparam moveable `true` if we are allowed to move data out of the input
 * coefficients, or `false` if the input coefficients should be read-only.
 *
 * \param dest the C-style array in which to write the output coefficients.
 * \param lhs the C-style array holding the first range of input coefficients.
 * \param lhsLen the total number of coefficients in the first input range.
 * \param rhs the C-style array holding the second range of input coefficients.
 * \param rhsLen the total number of coefficients in the second input range.
 * \param scratch pre-allocated scratch space, or `null` if the product
 * algorithm should create its own scratch space if/when required.  The
 * scratch space does not need to be initialised in any particular way.
 *
 * \ingroup detail
 */
template <CoefficientDomain T, SetOrAdd operation, bool moveable>
static void productBest(T* dest,
        CoefficientBuffer<T, moveable> lhs, size_t lhsLen,
        CoefficientBuffer<T, moveable> rhs, size_t rhsLen,
        T* scratch) {
    if constexpr (karatsubaThreshold<T> == 0) {
        // Do not use Karatsuba multiplication at all.
        #if 1
        productClassic<T, operation>(dest, lhs, lhsLen, rhs, rhsLen);
        #else
        if (lhsLen <= rhsLen)
            productClassic<T, operation>(dest, lhs, lhsLen, rhs, rhsLen);
        else
            productClassic<T, operation>(dest, rhs, rhsLen, lhs, lhsLen);
        #endif
    } else {
        // Enforce our minimum requirement on the Karatsuba threshold:
        static_assert(karatsubaThreshold<T> >= 2);

        // We need to decide if/how to use Karatsuba multiplication.
        if (lhsLen < karatsubaThreshold<T> ||
                rhsLen < karatsubaThreshold<T>) {
            // One of the polynomials is tiny.
            // Just use classic multiplication.
            #if 1
            productClassic<T, operation>(dest, lhs, lhsLen, rhs, rhsLen);
            #else
            if (lhsLen <= rhsLen)
                productClassic<T, operation>(dest, lhs, lhsLen, rhs, rhsLen);
            else
                productClassic<T, operation>(dest, rhs, rhsLen, lhs, lhsLen);
            #endif
        } else if ((lhsLen << 1) <= rhsLen + 1) {
            // We have rhs much longer than lhs.
            // Break rhs into blocks of size lhsLen, and use Karatsuba
            // multiplication on each (except possibly the last).
            // (Note that, since lhsLen and rhsLen are both positive,
            // just the test above will guarantee lhsLen ≤ rhsLen.)
            #if 1
            productClassic<T, operation>(dest, lhs, lhsLen, rhs, rhsLen);
            #else
            if constexpr (operation == SetOrAdd::Set) {
                // Since the destination ranges overlap for each block
                // operation, we need to initialise the destination
                // coefficients and then add.
                std::fill(dest, dest + lhsLen + rhsLen - 1, T());
            }
            while (rhsLen >= lhsLen) {
                productKaratsuba<T, SetOrAdd::Add, false>(
                    dest, lhs, lhsLen, rhs, lhsLen, scratch);
                rhsLen -= lhsLen;
                rhs += lhsLen;
                dest += lhsLen;
            }
            if (rhsLen)
                productBest<T, SetOrAdd::Add, moveable>(dest, lhs, lhsLen,
                    rhs, rhsLen, scratch);
            #endif
        } else if ((rhsLen << 1) <= lhsLen + 1) {
            // We have lhs much longer than rhs.
            // Like above, but with LHS and RHS swapped.
            #if 1
            productClassic<T, operation>(dest, lhs, lhsLen, rhs, rhsLen);
            #else
            if constexpr (operation == SetOrAdd::Set) {
                std::fill(dest, dest + lhsLen + rhsLen - 1, T());
            }
            while (lhsLen >= rhsLen) {
                productKaratsuba<T, SetOrAdd::Add, false>(
                    dest, lhs, rhsLen, rhs, rhsLen, scratch);
                lhsLen -= rhsLen;
                lhs += rhsLen;
                dest += rhsLen;
            }
            if (lhsLen)
                productBest<T, SetOrAdd::Add, moveable>(dest, lhs, lhsLen,
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
            productKaratsuba<T, operation, moveable>(dest,
                lhs, lhsLen, rhs, rhsLen, scratch);
        }
    }
}

} // namespace regina::detail

/**
 * Multiplies two polynomials using the specified algorithm.
 *
 * Both the input and output polynomials will be given as arrays of
 * consecutive coefficients (in order from lowest to highest degree terms).
 * The corresponding exponents are not specified (and do not need to be);
 * if the input exponents are `a0,...,a1` and `b0,...,b1`, then the output
 * exponents will be `(a0+b0),...,(a1+b1)`.
 *
 * This routine is provided mainly for timing, testing and diagnostics.
 * If you just wish to multiply two polynomials, you should use the usual
 * polynomial product operators (e.g., `x = y * z`): this way Regina will
 * choose the most suitable algorithm for you.
 *
 * The Karatsuba algorithm is not available for all polynomials: it requires
 * the two polynomials to have comparable degree spans (i.e., one polynomial
 * cannot have significantly more coefficients than the other.  At present,
 * this means (roughly) that the shorter polynomial should be more than half
 * the length of the longer polynomial.  The precise constraints are subject
 * to change in future versions of Regina, and so if you are forcing Karatsuba
 * multiplication then it is strongly recommended that you wrap this in a
 * try/catch block.
 *
 * As a special case, if either of the two input polynomials has just one or
 * zero coefficients then the given algorithm will be ignored, and this routine
 * will use scalar multiplication instead.
 *
 * \python The coefficient type \a T will be taken to be Integer.  Each input
 * polynomial should each be presented as a Python list of integer coefficients,
 * and the output polynomial will be presented the same way.  Since Python does
 * not support C++ templates, you should pass the algorithm at runtime as a
 * third argument: that is, `product(lhs, rhs, algorithm)`.
 *
 * \exception InvalidArgument The \a algorithm argument specified Karatsuba
 * multiplication, but the two given polynomials do not have comparable degree
 * spans.  See above for further explanation.
 *
 * \tparam algorithm the polynomial multiplication algorithm to use.
 * \tparam T the coefficient type.
 *
 * \param lhs the coefficients of the first polynomial to multiply.
 * \param rhs the coefficients of the second polynomial to multiply.
 * \return the coefficients of the product of both polynomials.
 */
template <PolynomialProductAlgorithm algorithm, CoefficientDomain T>
FixedArray<T> polynomialProduct(const FixedArray<T>& lhs,
        const FixedArray<T>& rhs) {
    if (lhs.empty() || rhs.empty())
        return {};

    FixedArray<T> ans(lhs.size() + rhs.size() - 1);

    if (lhs.size() == 1) {
        const T& scalar = lhs.front();
        auto in = rhs.begin();
        auto out = ans.begin();
        while (in != rhs.end())
            *out++ = scalar * (*in++);
    } else if (rhs.size() == 1) {
        const T& scalar = rhs.front();
        auto in = lhs.begin();
        auto out = ans.begin();
        while (in != lhs.end())
            *out++ = (*in++) * scalar;
    } else {
        // Both polynomials have more than one coefficient.
        if constexpr (algorithm == PolynomialProductAlgorithm::Classic) {
            detail::productClassic<T, detail::SetOrAdd::Either>(ans.begin(),
                lhs.begin(), lhs.size(), rhs.begin(), rhs.size());
        } else if constexpr (algorithm ==
                PolynomialProductAlgorithm::Karatsuba) {
            if ((lhs.size() << 1) <= rhs.size() + 1 ||
                    (rhs.size() << 1) <= lhs.size() + 1) {
                // One polynomial is _much_ longer than the other.
                throw InvalidArgument("Karatsuba multiplication requires the "
                    "polynomials to have comparable degree spans");
            } else {
                detail::productKaratsuba<T, detail::SetOrAdd::Either, false>(
                    ans.begin(), lhs.begin(), lhs.size(),
                    rhs.begin(), rhs.size());
            }
        } else {
            detail::productBest<T, detail::SetOrAdd::Either, false>(ans.begin(),
                lhs.begin(), lhs.size(), rhs.begin(), rhs.size(),
                nullptr /* scratch */);
        }
    }
    return ans;
}

} // namespace regina

#endif
