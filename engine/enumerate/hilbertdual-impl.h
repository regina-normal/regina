
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

/*! \file enumerate/hilbertdual-impl.h
 *  \brief Contains implementations of template functions from hilbertdual.h.
 *
 *  This file is automatically included from hilbertdual.h; there
 *  is no need for end users to include it explicitly.
 */

#ifndef __REGINA_HILBERTDUAL_IMPL_H
#ifndef __DOXYGEN
#define __REGINA_HILBERTDUAL_IMPL_H
#endif

#include <algorithm>
#include <iterator>
#include "enumerate/hilbertdual.h"
#include "enumerate/ordering.h"
#include "enumerate/validityconstraints.h"
#include "progress/progresstracker.h"
#include "utilities/bitmask.h"
#include "utilities/intutils.h"

namespace regina {

template <ArbitraryPrecisionIntegerVector Ray, typename Action>
void HilbertDual::enumerate(Action&& action,
        const MatrixInt& subspace, const ValidityConstraints& constraints,
        ProgressTracker* tracker, unsigned initialRows) {
    // Get the dimension of the entire space in which we are working.
    size_t dim = subspace.columns();

    // If the space has dimension zero, return no results.
    if (dim == 0)
        return;

    // Choose a bitmask type that can hold dim bits.
    // Use a (much faster) optimised bitmask type if we can.
    // Then farm the work out to the real enumeration routine that is
    // templated on the bitmask type.
    if (dim <= 8 * sizeof(unsigned))
        enumerateUsingBitmask<Ray, Bitmask1<unsigned> >(
            std::forward<Action>(action),
            subspace, constraints, tracker, initialRows);
    else if (dim <= 8 * sizeof(unsigned long))
        enumerateUsingBitmask<Ray, Bitmask1<unsigned long> >(
            std::forward<Action>(action),
            subspace, constraints, tracker, initialRows);
    else if (dim <= 8 * sizeof(unsigned long long))
        enumerateUsingBitmask<Ray, Bitmask1<unsigned long long> >(
            std::forward<Action>(action),
            subspace, constraints, tracker, initialRows);
    else if (dim <= 8 * sizeof(unsigned long long) + 8 * sizeof(unsigned))
        enumerateUsingBitmask<Ray,
            Bitmask2<unsigned long long, unsigned> >(
            std::forward<Action>(action),
            subspace, constraints, tracker, initialRows);
    else if (dim <= 8 * sizeof(unsigned long long) +
            8 * sizeof(unsigned long))
        enumerateUsingBitmask<Ray,
            Bitmask2<unsigned long long, unsigned long> >(
            std::forward<Action>(action),
            subspace, constraints, tracker, initialRows);
    else if (dim <= 16 * sizeof(unsigned long long))
        enumerateUsingBitmask<Ray, Bitmask2<unsigned long long> >(
            std::forward<Action>(action),
            subspace, constraints, tracker, initialRows);
    else
        enumerateUsingBitmask<Ray, Bitmask>(
            std::forward<Action>(action),
            subspace, constraints, tracker, initialRows);
}

template <ArbitraryPrecisionIntegerVector Ray, typename BitmaskType,
    typename Action>
void HilbertDual::enumerateUsingBitmask(Action&& action,
        const MatrixInt& subspace, const ValidityConstraints& constraints,
        ProgressTracker* tracker, unsigned initialRows) {
    using IntegerType = typename Ray::value_type;

    // Get the dimension of the entire space in which we are working.
    // At this point we are guaranteed that the dimension is non-zero.
    size_t dim = subspace.columns();

    // Are there any hyperplanes at all in the subspace?
    size_t nEqns = subspace.rows();
    if (nEqns == 0) {
        // No!  Just send back the unit vectors.
        for (unsigned i = 0; i < dim; ++i) {
            Ray ans(dim);
            ans[i] = IntegerType::one;
            action(std::move(ans));
        }

        if (tracker)
            tracker->setPercent(100);
        return;
    }

    // We actually have some work to do.

    // Process the hyperplanes in a good order.
    //
    // Sort the integers 0..(nEqns-1) into the order in which we plan to
    // process the hyperplanes.
    int* hyperplanes = new int[nEqns];
    unsigned i;
    for (i = 0; i < nEqns; ++i)
        hyperplanes[i] = i;

    std::sort(hyperplanes + initialRows, hyperplanes + nEqns,
        PosOrder(subspace));

    auto constraintMasks = constraints.bitmasks<BitmaskType>(dim);

    // Create the vector list with which we will work.
    // Fill it with the initial basis elements.
    std::vector<VecSpec<IntegerType, BitmaskType>*> list;
    for (i = 0; i < dim; ++i)
        list.push_back(new VecSpec<IntegerType, BitmaskType>(i, dim));

#if 0
    std::cout << "LIST SIZE: " << list.size() << std::endl;
#endif

    // Intersect the hyperplanes one at a time.
    for (i=0; i<nEqns; i++) {
        intersectHyperplane(list, subspace, hyperplanes[i], constraintMasks);

#if 0
        std::cout << "LIST SIZE: " << list.size() << std::endl;
#endif

        if (tracker && ! tracker->setPercent(100.0 * i / nEqns))
            break;
    }

    // We're done!
    delete[] hyperplanes;

    typename std::vector<VecSpec<IntegerType, BitmaskType>*>::iterator it;

    if (tracker && tracker->isCancelled()) {
        // The operation was cancelled.  Clean up before returning.
        for (it = list.begin(); it != list.end(); ++it)
            delete *it;
        return;
    }

    for (it = list.begin(); it != list.end(); ++it) {
        Ray ans(dim);
        for (i = 0; i < dim; ++i)
            ans[i] = (**it)[i];
        action(std::move(ans));

        delete *it;
    }

    // All done!
    if (tracker)
        tracker->setPercent(100);
}

template <typename IntegerType, typename BitmaskType>
bool HilbertDual::reduces(const VecSpec<IntegerType, BitmaskType>& vec,
        const std::list<VecSpec<IntegerType, BitmaskType>*>& against,
        int listSign) {
    typename std::list<VecSpec<IntegerType, BitmaskType>*>::const_iterator it;
    for (it = against.begin(); it != against.end(); ++it) {
        if (! (*it)->dominatedBy(vec))
            continue;

        if (listSign > 0) {
            if ((**it).nextHyp() <= vec.nextHyp())
                return true;
        } else if (listSign < 0) {
            if (vec.nextHyp() <= (**it).nextHyp())
                return true;
        } else {
            if (vec.nextHyp() == (**it).nextHyp())
                return true;
        }
    }

    return false;
}

template <typename IntegerType, typename BitmaskType>
void HilbertDual::reduceBasis(
        std::list<VecSpec<IntegerType, BitmaskType>*>& reduce,
        std::list<VecSpec<IntegerType, BitmaskType>*>& against,
        int listSign) {
    if (reduce.empty())
        return;

    typename std::list<VecSpec<IntegerType, BitmaskType>*>::iterator
        i, next, red;
    bool processed;

    i = reduce.begin();
    next = i;
    ++next;

    while (i != reduce.end()) {
        processed = true;
        for (red = against.begin(); red != against.end(); ++red) {
            if (red == i) {
                processed = false;
                continue;
            }

            if (! (*red)->dominatedBy(**i))
                continue;

            if (listSign > 0) {
                if ((**red).nextHyp() <= (**i).nextHyp())
                    break;
            } else if (listSign < 0) {
                if ((**i).nextHyp() <= (**red).nextHyp())
                    break;
            } else {
                if ((**i).nextHyp() == (**red).nextHyp())
                    break;
            }
        }

        if (red == against.end()) {
            i = next;
            if (next != reduce.end())
                ++next;
            continue;
        }

        delete *i;
        reduce.erase(i);

#ifdef __REGINA_HILBERT_DUAL_OPT_DARWIN
        // Darwinistic reordering of the list against.
        if (processed) {
            against.push_front(*red);
            against.erase(red);

            i = next;
            if (next != reduce.end())
                ++next;
        } else {
            // Both reduce and against are the same list, and the
            // reducing vector is one we haven't processed yet.
            if (red == next)
                ++next;
            against.push_front(*red);
            against.erase(red);
            i = against.begin();
        }
#else
        i = next;
        if (next != reduce.end())
            ++next;
#endif
    }
}

template <typename IntegerType, typename BitmaskType>
void HilbertDual::intersectHyperplane(
        std::vector<VecSpec<IntegerType, BitmaskType>*>& list,
        const MatrixInt& subspace, unsigned row,
        const std::vector<BitmaskType>& constraintMasks) {
    // These must be linked lists because we need fast insertion and
    // deletion at arbitrary locations.
    std::list<VecSpec<IntegerType, BitmaskType>*>
        zero, pos, neg, newZero, newPos, newNeg;
    typename std::list<VecSpec<IntegerType, BitmaskType>*>::iterator
        it, posit, negit;
    typename std::list<VecSpec<IntegerType, BitmaskType>*>::iterator
        posPrevGen, negPrevGen;

    // Decant the existing basis elements into 0/+/- sets according to the
    // new hyperplane.
    int s;
    typename std::vector<VecSpec<IntegerType, BitmaskType>*>::iterator srcit;
    for (srcit = list.begin(); srcit != list.end(); srcit++) {
        (*srcit)->initNextHyp(subspace, row);

        s = (*srcit)->sign();
        if (s == 0)
            zero.push_back(*srcit);
        else if (s < 0)
            neg.push_back(*srcit);
        else
            pos.push_back(*srcit);
    }
    list.clear();

    posPrevGen = pos.begin();
    negPrevGen = neg.begin();

    // TODO: Optimise from here down: (d), Sec.3

    // Keep enlarging these sets until they enlarge no more.
    BitmaskType comb, tmpMask;
    bool broken;
    bool reachedPosPrevGen;
    VecSpec<IntegerType, BitmaskType> sum(subspace.columns());
#if 0
    std::cerr << "Start iteration:" << std::endl;
#endif
    while (true) {
#if 0
        std::cerr << "    Intermediate 0/+/-: " << zero.size()
            << ' ' << pos.size() << ' ' << neg.size() << std::endl;
#endif

        // Generate all valid (pos + neg) pairs that cannot be reduced using
        // the present lists.
        reachedPosPrevGen = false;
        for (posit = pos.begin(); posit != pos.end(); ++posit) {
            if (posit == posPrevGen)
                reachedPosPrevGen = true;

            for (negit = (reachedPosPrevGen ? neg.begin() : negPrevGen);
                    negit != neg.end(); ++negit) {
#ifdef __REGINA_HILBERT_DUAL_OPT_BI16D
                // Check for guaranteed redundany.
                // See Bruns-Ichim, Remark 16(d).
                // Bruns and Ichim use strict inequalities, but the same
                // argument shows that non-strict inequalities will work also.
                if ((*posit)->srcNextHyp() > 0 &&
                        (*negit)->nextHyp() <= - (*posit)->srcNextHyp())
                    continue;
                if ((*negit)->srcNextHyp() < 0 &&
                        (*posit)->nextHyp() >= - (*negit)->srcNextHyp())
                    continue;
#endif

                // Check for validity.
                if (! constraintMasks.empty()) {
                    comb = (*posit)->mask();
                    comb |= (*negit)->mask();

                    broken = false;
                    for (const BitmaskType& constraint : constraintMasks) {
                        tmpMask = comb;
                        tmpMask &= constraint;
                        if (! tmpMask.atMostOneBit()) {
                            broken = true;
                            break;
                        }
                    }
                    if (broken)
                        continue;
                }

                // Check whether the vector can be reduced; if not, use it.
                // We CANNOT reorder pos or neg at this point.
                sum.formSum(**posit, **negit);
                s = sum.sign();
                if (s == 0) {
                    if (! reduces(sum, zero, 0))
                        newZero.push_back(
                            new VecSpec<IntegerType, BitmaskType>(sum));
                } else if (s > 0) {
                    // If this decomposes as a sum of (possibly many)
                    // terms in pos and/or zero, at least one such term must
                    // be in pos.  Therefore we only need to test
                    // reduction against pos, and not zero also.
                    if (! reduces(sum, pos, 1))
#ifndef __REGINA_HILBERT_DUAL_OPT_NEWGEN_STRICT_ONLY
                        if (! reduces(sum, zero, 1))
#endif
                            newPos.push_back(
                                new VecSpec<IntegerType, BitmaskType>(sum));
                } else if (s < 0) {
                    // Likewise: test only against neg, and not zero also.
                    if (! reduces(sum, neg, -1))
#ifndef __REGINA_HILBERT_DUAL_OPT_NEWGEN_STRICT_ONLY
                        if (! reduces(sum, zero, -1))
#endif
                            newNeg.push_back(
                                new VecSpec<IntegerType, BitmaskType>(sum));
                }
            }
        }

        if (newZero.empty() && newPos.empty() && newNeg.empty()) {
            // The basis has not changed, which means we're done.
            break;
        }

        // Independently reduce the basis on each side.
        reduceBasis(newZero, newZero, 0);
        reduceBasis(zero, newZero, 0);

        reduceBasis(newPos, newPos, 1);
        reduceBasis(pos, newPos, 1);

        reduceBasis(newNeg, newNeg, -1);
        reduceBasis(neg, newNeg, -1);

        // Add the new vectors to the bases.
        zero.splice(zero.end(), newZero);

        if (newPos.empty()) {
            posPrevGen = pos.end();
        } else {
            posPrevGen = newPos.begin();
            pos.splice(pos.end(), newPos);
        }

        if (newNeg.empty()) {
            negPrevGen = neg.end();
        } else {
            negPrevGen = newNeg.begin();
            neg.splice(neg.end(), newNeg);
        }
    }

    // We have a final Hilbert basis!
    // Clean up and return.
    for (it = pos.begin(); it != pos.end(); ++it)
        delete *it;
    for (it = neg.begin(); it != neg.end(); ++it)
        delete *it;
    for (it = zero.begin(); it != zero.end(); ++it)
        list.push_back(*it);
}

} // namespace regina

#endif
