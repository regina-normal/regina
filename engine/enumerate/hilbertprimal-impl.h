
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

/*! \file enumerate/hilbertprimal-impl.h
 *  \brief Contains implementations of template functions from hilbertprimal.h.
 *
 *  This file is automatically included from hilbertprimal.h; there
 *  is no need for end users to include it explicitly.
 */

#ifndef __REGINA_HILBERTPRIMAL_IMPL_H
#ifndef __DOXYGEN
#define __REGINA_HILBERTPRIMAL_IMPL_H
#endif

#include "regina-core.h"
#include "enumerate/hilbertprimal.h"
#include "enumerate/maxadmissible.h"
#include "enumerate/validityconstraints.h"
#include "maths/vector.h"
#include "progress/progresstracker.h"
#include "utilities/intutils.h"
#include <list>
#include <set>
#include <vector>

namespace regina {

template <ArbitraryPrecisionIntegerVector Ray, typename RayIterator,
    VoidCallback<Ray&&> Action>
void HilbertPrimal::enumerate(Action&& action,
        const RayIterator& raysBegin, const RayIterator& raysEnd,
        const ValidityConstraints& constraints, ProgressTracker* tracker) {
    if (raysBegin == raysEnd) {
        // No extremal rays; no Hilbert basis.
        return;
    }

    // Get the dimension of the space.
    size_t dim = (*raysBegin).size();
    if (dim == 0)
        return;

    // Choose a bitmask type that can hold dim bits.
    // Use a (much faster) optimised bitmask type if we can.
    // Then farm the work out to the real enumeration routine that is
    // templated on the bitmask type.
    if (dim <= 8 * sizeof(unsigned))
        enumerateUsingBitmask<Ray, Bitmask1<unsigned> >(
            std::forward<Action>(action),
            raysBegin, raysEnd, constraints, tracker);
    else if (dim <= 8 * sizeof(unsigned long))
        enumerateUsingBitmask<Ray, Bitmask1<unsigned long> >(
            std::forward<Action>(action),
            raysBegin, raysEnd, constraints, tracker);
    else if (dim <= 8 * sizeof(unsigned long long))
        enumerateUsingBitmask<Ray, Bitmask1<unsigned long long> >(
            std::forward<Action>(action),
            raysBegin, raysEnd, constraints, tracker);
    else if (dim <= 8 * sizeof(unsigned long long) + 8 * sizeof(unsigned))
        enumerateUsingBitmask<Ray,
            Bitmask2<unsigned long long, unsigned> >(
            std::forward<Action>(action),
            raysBegin, raysEnd, constraints, tracker);
    else if (dim <= 8 * sizeof(unsigned long long) +
            8 * sizeof(unsigned long))
        enumerateUsingBitmask<Ray,
            Bitmask2<unsigned long long, unsigned long> >(
            std::forward<Action>(action),
            raysBegin, raysEnd, constraints, tracker);
    else if (dim <= 16 * sizeof(unsigned long long))
        enumerateUsingBitmask<Ray, Bitmask2<unsigned long long> >(
            std::forward<Action>(action),
            raysBegin, raysEnd, constraints, tracker);
    else
        enumerateUsingBitmask<Ray, Bitmask>(
            std::forward<Action>(action),
            raysBegin, raysEnd, constraints, tracker);
}

template <ArbitraryPrecisionIntegerVector Ray, ReginaBitmask BitmaskType,
        typename RayIterator, VoidCallback<Ray&&> Action>
void HilbertPrimal::enumerateUsingBitmask(Action&& action,
        const RayIterator& raysBegin, const RayIterator& raysEnd,
        const ValidityConstraints& constraints, ProgressTracker* tracker) {
    using IntegerType = typename Ray::value_type;

    // We know at this point that the dimension is non-zero.
    size_t dim = (*raysBegin).size();

    // First enumerate all maximal admissible faces.
    if (tracker)
        tracker->setPercent(10);
    std::vector<BitmaskType> maxFaces = MaxAdmissible::enumerate<BitmaskType>(
        raysBegin, raysEnd, constraints);

    // Now use normaliz to process each face.
    if (tracker)
        tracker->setPercent(30);

    std::set<std::vector<mpz_class> > finalBasis;
    std::vector<const Vector<IntegerType>*> face;
    RayIterator rit;
    unsigned i;
    std::vector<mpz_class>::const_iterator hvit;
    for (const auto& m : maxFaces) {
        // Locate the extremal rays that generate this face.
        std::vector<std::vector<mpz_class> > input;
        for (rit = raysBegin; rit != raysEnd; ++rit)
            if (inFace(*rit, m)) {
                std::vector<mpz_class>& v(input.emplace_back());
                v.reserve(dim);
                for (i = 0; i < dim; ++i) {
                    if ((*rit)[i].isNative()) {
                        // Since this large integer has a native rep.,
                        // unsafeValue() must succeed.
                        v.push_back(mpz_class((*rit)[i].
                            template unsafeValue<long>()));
                    } else
                        v.push_back(mpz_class((*rit)[i].rawData()));
                }
            }

        for (auto& b : normaliz(input))
            finalBasis.insert(std::move(b));
    }

    if (tracker)
        tracker->setPercent(90);

    for (const auto& b : finalBasis) {
        Ray ans(dim);
        for (i = 0, hvit = b.begin(); hvit != b.end(); ++hvit, ++i)
            ans[i].setRaw(hvit->get_mpz_t());
        action(std::move(ans));
    }

    // All done!
    if (tracker)
        tracker->setPercent(100);
}

template <typename VectorClass, ReginaBitmask BitmaskType>
bool HilbertPrimal::inFace(const VectorClass& ray, const BitmaskType& face) {
    for (unsigned i = 0; i < ray.size(); ++i)
        if ((! face.get(i)) && ray[i] > 0)
            return false;
    return true;
}

} // namespace regina

#endif
