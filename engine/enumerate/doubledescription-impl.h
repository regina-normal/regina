
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

/*! \file enumerate/doubledescription-impl.h
 *  \brief Contains implementations of template functions from
 *  doubledescription.h.
 *
 *  This file is automatically included from doubledescription.h; there
 *  is no need for end users to include it explicitly.
 */

#ifndef __REGINA_DOUBLEDESCRIPTION_IMPL_H
#ifndef __DOXYGEN
#define __REGINA_DOUBLEDESCRIPTION_IMPL_H
#endif

#include <algorithm>
#include <iterator>
#include "regina-core.h"
#include "regina-config.h"
#include "enumerate/doubledescription.h"
#include "enumerate/validityconstraints.h"
#include "maths/matrixops.h"
#include "progress/progresstracker.h"
#include "utilities/bitmask.h"
#include "utilities/intutils.h"
#include "utilities/trieset.h"

namespace regina {

template <class IntegerType, class BitmaskType>
DoubleDescription::RaySpec<IntegerType, BitmaskType>::RaySpec(
        size_t axis, const MatrixInt& subspace, const long* hypOrder) :
        Vector<IntegerType>(subspace.rows()), facets_(subspace.columns()) {
    size_t i;

    for (i = 0; i < subspace.columns(); ++i)
        if (i != axis)
            facets_.set(i, true);

    for (i = 0; i < Vector<IntegerType>::size(); ++i)
        elts_[i] = subspace.entry(hypOrder[i], axis);
}

template <class IntegerType, class BitmaskType>
DoubleDescription::RaySpec<IntegerType, BitmaskType>::RaySpec(
        const RaySpec<IntegerType, BitmaskType>& first,
        const RaySpec<IntegerType, BitmaskType>& second) :
        Vector<IntegerType>(second.size() - 1), facets_(second.facets_) {
    for (size_t i = 0; i < Vector<IntegerType>::size(); ++i)
        elts_[i] = second.elts_[i + 1] * (*first.elts_) -
            first.elts_[i + 1] * (*second.elts_);
    Vector<IntegerType>::scaleDown();

    if (*first.elts_ < 0)
        Vector<IntegerType>::negate();

    // Compute the new set of facets.
    facets_ &= first.facets_;
}

template <class IntegerType, class BitmaskType>
template <typename RayClass>
void DoubleDescription::RaySpec<IntegerType, BitmaskType>::recover(
        RayClass& dest, const MatrixInt& subspace) const {
    size_t i, j;

    size_t rows = subspace.rows();
    size_t cols = subspace.columns() - facets_.bits();

    // Extract the set of columns that we actually care about.
    auto* use = new size_t[cols];
    for (i = 0, j = 0; i < subspace.columns(); ++i)
        if (facets_.get(i)) {
            // We know in advance that this coordinate will be zero.
            dest[i] = IntegerType::zero;
        } else {
            use[j++] = i;
        }

    // We know the solution space has dimension one.
    // If there are no equations then there must be only one non-zero
    // coordinate, and vice versa.
    if (cols == 1) {
        dest[*use] = 1;
        delete[] use;
        return;
    }

    // We have several non-zero coordinates with at least one
    // non-trivial equation relating them.

    // Form a submatrix for the equations, looking only at non-zero coordinates.
    auto* m = new IntegerType[rows * cols];
    for (i = 0; i < rows; ++i)
        for (j = 0; j < cols; ++j)
            m[i * cols + j] = subspace.entry(i, use[j]);

    // Put this submatrix in echelon form; moreover, for the leading
    // entry in each row, set all other entries in the corresponding
    // column to zero.
    auto* lead = new size_t[cols];
    for (i = 0; i < cols; ++i)
        lead[i] = i;

    // More or less a stripped-down copy of rowBasisAndOrthComp() from here.
    // See rowBasisAndOrthComp() for further details on how this works.
    size_t done = 0;
    IntegerType coeff1, coeff2, common;
    while (done < rows) {
        // Find the first non-zero entry in row done.
        for (i = done; i < cols; ++i)
            if (m[done * cols + lead[i]] != 0)
                break;

        if (i == cols) {
            // We have a zero row.  Trash it and replace it with something else.
            --rows;
            if (done < rows) {
                for (j = 0; j < cols; ++j)
                    m[done * cols + j] = m[rows * cols + j];
            }
        } else {
            // We have a non-zero row.
            // Save the column in which we found our non-zero entry.
            std::swap(lead[done], lead[i]);

            // Make all other entries in column lead[done] equal to zero.
            coeff1 = m[done * cols + lead[done]];
            for (i = 0; i < rows; ++i) {
                if (i == done)
                    continue;

                coeff2 = m[i * cols + lead[done]];
                common = 0;
                if (coeff2 != 0) {
                    for (j = 0; j < cols; ++j) {
                        m[i * cols + j] = m[i * cols + j] * coeff1
                            - m[done * cols + j] * coeff2;
                        common = common.gcd(m[i * cols + j]);
                    }
                }
                if (common < 0)
                    common.negate();
                if (common > 1)
                    for (j = 0; j < cols; ++j)
                        m[i * cols + j].divByExact(common);
            }
            ++done;
        }
    }

    // At this point we should have rows == (cols - 1), and the column
    // that is *not* zeroed out almost everywhere is lead[rows].  (We
    // know this because we know that our solution must be one-dimensional).
    //
    // Form a solution!
    common = 1;
    for (i = 0; i < rows; ++i)
        common = common.lcm(m[i * cols + lead[i]]);
    if (common < 0)
        common.negate();

    for (i = 0; i < rows; ++i)
        dest[use[lead[i]]] = - (common * m[i * cols + lead[rows]]).
            divExact(m[i * cols + lead[i]]);
    dest[use[lead[rows]]] = common;

    dest.scaleDown();

    // All done!
    delete[] lead;
    delete[] m;
    delete[] use;
}

template <class RayClass, typename Action>
void DoubleDescription::enumerate(Action&& action,
        const MatrixInt& subspace, const ValidityConstraints& constraints,
        ProgressTracker* tracker, size_t initialRows) {
    static_assert(
        IsReginaArbitraryPrecisionInteger<typename RayClass::value_type>::value,
        "DoubleDescription::enumerate() requires the RayClass "
        "template parameter to be equal to or derived from Vector<T>, "
        "where T is one of Regina's arbitrary precision integer types.");

    size_t nFacets = subspace.columns();

    // If the space has dimension zero, return no results.
    if (nFacets == 0)
        return;

    // Choose a bitmask type for representing the set of facets that a
    // ray belongs to; in particular, use a (much faster) optimised
    // bitmask type if we can.
    // Then farm the work out to the real enumeration routine that is
    // templated on the bitmask type.
    if (nFacets <= 8 * sizeof(unsigned))
        enumerateUsingBitmask<RayClass, Bitmask1<unsigned>>(
            std::forward<Action>(action),
            subspace, constraints, tracker, initialRows);
    else if (nFacets <= 8 * sizeof(unsigned long))
        enumerateUsingBitmask<RayClass, Bitmask1<unsigned long>>(
            std::forward<Action>(action),
            subspace, constraints, tracker, initialRows);
    else if (nFacets <= 8 * sizeof(unsigned long long))
        enumerateUsingBitmask<RayClass, Bitmask1<unsigned long long>>(
            std::forward<Action>(action),
            subspace, constraints, tracker, initialRows);
    else if (nFacets <= 8 * sizeof(unsigned long long) + 8 * sizeof(unsigned))
        enumerateUsingBitmask<RayClass, Bitmask2<unsigned long long, unsigned>>(
            std::forward<Action>(action),
            subspace, constraints, tracker, initialRows);
    else if (nFacets <= 8 * sizeof(unsigned long long) +
            8 * sizeof(unsigned long))
        enumerateUsingBitmask<RayClass,
            Bitmask2<unsigned long long, unsigned long>>(
            std::forward<Action>(action),
            subspace, constraints, tracker, initialRows);
    else if (nFacets <= 16 * sizeof(unsigned long long))
        enumerateUsingBitmask<RayClass, Bitmask2<unsigned long long>>(
            std::forward<Action>(action),
            subspace, constraints, tracker, initialRows);
    else
        enumerateUsingBitmask<RayClass, Bitmask>(
            std::forward<Action>(action),
            subspace, constraints, tracker, initialRows);
}

template <class RayClass, class BitmaskType, typename Action>
void DoubleDescription::enumerateUsingBitmask(Action&& action,
        const MatrixInt& subspace, const ValidityConstraints& constraints,
        ProgressTracker* tracker, size_t initialRows) {
    using IntegerType = typename RayClass::value_type;

    // Get the dimension of the entire space in which we are working.
    size_t dim = subspace.columns();

    // Are there any hyperplanes at all in the subspace?
    size_t nEqns = subspace.rows();
    if (nEqns == 0) {
        // No!  Just send back the vertices of the non-negative orthant.
        for (size_t i = 0; i < dim; ++i) {
            RayClass ans(dim);
            ans[i] = IntegerType::one;
            action(std::move(ans));
        }

        if (tracker)
            tracker->setPercent(100);
        return;
    }

    // We actually have some work to do.

    // We want to process the hyperplanes in a good order;
    // Fukuda and Prodon (1996) recommend this, and experimental
    // experience with Regina agrees.  The ordering we use here
    // is based on position vectors, as described in
    // "Optimizing the double description method for normal surface
    // enumeration", B.A. Burton, Mathematics of Computation 79 (2010), 453-484.
    // See the class PosOrder for details.
    //
    // Sort the integers 0..(nEqns-1) into the order in which we plan to
    // process the hyperplanes.
    long* hyperplanes = new long[nEqns];
    size_t i;
    for (i = 0; i < nEqns; ++i)
        hyperplanes[i] = i;

    std::sort(hyperplanes + initialRows, hyperplanes + nEqns,
        PosOrder(subspace));

    // Create the two vector lists with which we will work.
    // Fill the first with the initial set of rays.
    std::vector<RaySpec<IntegerType, BitmaskType>*> list[2];

    for (i = 0; i < dim; ++i)
        list[0].push_back(new RaySpec<IntegerType, BitmaskType>(
            i, subspace, hyperplanes));

    auto constraintMasks = constraints.bitmasks<BitmaskType>(dim);

#if 0
    std::cout << "Initial size: " << list[0].size() << std::endl;
#endif

    // Intersect the hyperplanes one at a time.
    // At any point we should have the latest results in
    // list[workingList], with the other list empty.
    int workingList = 0;
    size_t used = 0;
    for (i=0; i<nEqns; i++) {
        // Do not increment used if the old solution set sits entirely in
        // and/or to only one side of the new hyperplane.  This gives the
        // dimensional filtering in intersectHyperplane() greater strength.
        // The reason this works is because any vertex of the solution
        // space *with* this hyperplane is also a vertex of the solution
        // space *without* this hyperplane (and therefore satisfies the
        // relevant dimensional constraints without this hyperplane).
        if (intersectHyperplane(list[workingList], list[1 - workingList],
                dim, used, constraintMasks, tracker))
            ++used;

        workingList = 1 - workingList;
#if 0
        std::cout << "Intermediate size: " << list[workingList].size()
            << std::endl;
#endif

        if (tracker && ! tracker->setPercent(100.0 * i / nEqns))
            break;
    }

    // We're done!
    delete[] hyperplanes;

    if (tracker && tracker->isCancelled()) {
        // The operation was cancelled.  Clean up before returning.
        for (auto* r : list[workingList])
            delete r;
        return;
    }

    // Convert the final solutions into the required ray class.
    for (auto* r : list[workingList]) {
        RayClass ans(dim);
        r->recover(ans, subspace);
        action(std::move(ans));

        delete r;
    }

    // All done!
    if (tracker)
        tracker->setPercent(100);
}

template <class IntegerType, class BitmaskType>
bool DoubleDescription::intersectHyperplane(
        std::vector<RaySpec<IntegerType, BitmaskType>*>& src,
        std::vector<RaySpec<IntegerType, BitmaskType>*>& dest,
        size_t dim, size_t prevHyperplanes,
        const std::vector<BitmaskType>& constraintMasks,
        ProgressTracker* tracker) {
    if (src.empty())
        return false;

    std::vector<RaySpec<IntegerType, BitmaskType>*> pos, neg;

    // Run through the old rays and determine which side of the
    // new hyperplane they lie on.
    // Rays lying within the new hyperplane will be added directly to
    // the new solution set.
    int sign;
    for (auto* ray : src) {
        sign = ray->sign();
        if (sign == 0)
            dest.push_back(new RaySpec<IntegerType, BitmaskType>(*ray));
        else if (sign < 0)
            neg.push_back(ray);
        else
            pos.push_back(ray);
    }

    // Does one of the closed half-spaces defined by the hyperplane contain the
    // entire old solution set?  If so, there will be no new vertices.
    if (pos.empty() || neg.empty()) {
        for (auto* ray : src)
            delete ray;
        src.clear();
        return false;
    }

    // Run through the pairs of positive and negative rays.  For every
    // pair of rays that are adjacent in the current solution space,
    // add the corresponding intersection with the new hyperplane to the
    // new solution set.
    bool broken;

    // We use the TrieSet data structure to speed up adjacency testing
    // in the code below.  Construct a TrieSet that records the facet
    // structure for every vertex in the old solution set.
    TrieSet trie;
    for (auto* other : src)
        trie.insert(other->facets());

    unsigned iterations = 0;
    for (auto* p : pos)
        for (auto* n : neg) {
            // Test for cancellation, but not every time (since this
            // involves expensive mutex locking).
            if (tracker && ++iterations == 100) {
                iterations = 0;
                if (tracker->isCancelled()) {
                    for (auto* other : src)
                        delete other;
                    src.clear();
                    for (auto* other : dest)
                        delete other;
                    dest.clear();
                    return false;
                }
            }

            BitmaskType join(p->facets());
            join &= (n->facets());

            // We only care about adjacent rays, i.e., rays joined by an edge.
            // For the rays to be adjacent, the number of original facets that
            // both belong to must be >= dimension(subspace) - 2,
            // which is >= dimension(entire space) - prevHyperplanes - 2.
            // See Fukuda and Prodon (1996), Proposition 9.
            if (join.bits() + prevHyperplanes + 2 < dim)
                continue;

            // Are we supposed to check for compatibility?
            if (! constraintMasks.empty()) {
                BitmaskType inv(join);
                inv.flip();

                broken = false;
                for (const BitmaskType& constraint : constraintMasks) {
                    BitmaskType mask(inv);
                    mask &= constraint;
                    if (! mask.atMostOneBit()) {
                        broken = true;
                        break;
                    }
                }
                if (broken)
                    continue;
            }

            // Two rays are adjacent (joined by an edge) if and only if
            // there is no other ray belonging to all of their common facets.
            //
            // If the rays *are* adjacent, join them and put the
            // corresponding intersection in the new results set.
            if (! trie.hasExtraSuperset(join, p->facets(), n->facets(), dim))
                dest.push_back(new RaySpec<IntegerType, BitmaskType>(*p, *n));
        }

    // Clean up.
    for (auto* other : src)
        delete other;
    src.clear();
    return true;
}

} // namespace regina

#endif
