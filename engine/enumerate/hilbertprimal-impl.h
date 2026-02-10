
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
        // (And.. we have no rays from which to get the dimension of the space.)
        return;
    }

    // Choose the slickest possible bitmask type that can hold dim bits.
    usingBitmaskFor((*raysBegin).size() /* dimension of the space */,
            [&action, raysBegin, raysEnd, &constraints, tracker]
            <ReginaBitmask BitmaskType>(size_t dim) {
        using IntegerType = typename Ray::value_type;

        // If the space has dimension zero, return no results at all.
        if (dim == 0)
            return;

        // First enumerate all maximal admissible faces.
        if (tracker)
            tracker->setPercent(10);
        auto maxFaces = MaxAdmissible::enumerate<BitmaskType>(
            raysBegin, raysEnd, constraints);

        // Now use normaliz to process each face.
        if (tracker)
            tracker->setPercent(30);

        std::set<std::vector<mpz_class>> finalBasis;
        for (const auto& m : maxFaces) {
            // Locate the extremal rays that generate this face.
            std::vector<std::vector<mpz_class>> input;
            for (auto rit = raysBegin; rit != raysEnd; ++rit)
                if (inFace(*rit, m)) {
                    std::vector<mpz_class>& v(input.emplace_back());
                    v.reserve(dim);
                    for (size_t i = 0; i < dim; ++i) {
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

        std::vector<mpz_class>::const_iterator hvit;
        size_t i;
        for (const auto& b : finalBasis) {
            Ray ans(dim);
            for (i = 0, hvit = b.begin(); hvit != b.end(); ++hvit, ++i)
                ans[i].setRaw(hvit->get_mpz_t());
            action(std::move(ans));
        }

        // All done!
        if (tracker)
            tracker->setPercent(100);
    });
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
