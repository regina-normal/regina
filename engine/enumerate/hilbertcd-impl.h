
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

/*! \file enumerate/hilbertcd-impl.h
 *  \brief Contains implementations of template functions from hilbertcd.h.
 *
 *  This file is automatically included from hilbertcd.h; there
 *  is no need for end users to include it explicitly.
 */

#ifndef __REGINA_HILBERTCD_IMPL_H
#ifndef __DOXYGEN
#define __REGINA_HILBERTCD_IMPL_H
#endif

#include "regina-core.h"
#include "enumerate/hilbertcd.h"
#include "enumerate/validityconstraints.h"
#include "maths/matrix.h"
#include "utilities/bitmask.h"
#include "utilities/fixedarray.h"
#include "utilities/intutils.h"
#include <list>

namespace regina {

template <ArbitraryPrecisionIntegerVector Ray, VoidCallback<Ray&&> Action>
void HilbertCD::enumerate(Action&& action,
        const MatrixInt& subspace, const ValidityConstraints& constraints) {
    usingBitmaskFor(subspace.columns() /* dimension of the space */,
            [&action, &subspace, &constraints]
            <ReginaBitmask BitmaskType>(size_t dim) {
        using IntegerType = typename Ray::value_type;

        // If the space has dimension zero, return no results at all.
        if (dim == 0)
            return;

        // Stack-based Contejean-Devie algorithm
        // (Information & Computation, 1994).
        size_t nEqns = subspace.rows();

        auto constraintMasks = constraints.bitmasks<BitmaskType>(dim);

        std::list<VecSpec<IntegerType, BitmaskType>*> basis;

        FixedArray<Vector<IntegerType>*> unitMatch(dim);
        for (size_t i = 0; i < dim; ++i) {
            unitMatch[i] = new Vector<IntegerType>(nEqns);
            for (size_t j = 0; j < nEqns; ++j)
                (*unitMatch[i])[j] = subspace.entry(j, i);
        }

        unsigned stackSize;
        // All vectors/rays are created and destroyed.
        // Bitmasks on the other hand are reused.
        FixedArray<VecSpec<IntegerType, BitmaskType>*> coord(dim);
        FixedArray<Vector<IntegerType>*> match(dim);
        FixedArray<BitmaskType> frozen(dim);

        for (size_t i = 0; i < dim; ++i)
            frozen[i].reset(dim); // All false.

        // Push the zero vector.
        coord[0] = new VecSpec<IntegerType, BitmaskType>(dim);
        match[0] = new Vector<IntegerType>(nEqns);
        stackSize = 1; // The zero vector is already on top.
        bool first = true;

        VecSpec<IntegerType, BitmaskType> *c;
        Vector<IntegerType> *m;
        BitmaskType f(dim);
        BitmaskType mask(dim), tmpMask(dim);
        bool found, dom;
        while (stackSize) {
            c = coord[stackSize - 1];
            m = match[stackSize - 1];
            f = frozen[stackSize - 1];
            --stackSize;

            // std::cerr << (*c) << " ... " << (*m) << " ... "
            //     << stackSize << std::endl;

            // Do we have a non-zero solution?
            if (! first) {
                found = false;
                for (size_t i = 0; i < nEqns; ++i)
                    if ((*m)[i] != 0) {
                        found = true;
                        break;
                    }
                if (! found) {
                    // Yep, it's a solution.
                    basis.push_back(c);
                    delete m;
                    continue;
                }
            }

            // Try incrementing along different coordinate axes.
            for (size_t i = 0; i < dim; ++i) {
                if (f.get(i))
                    continue; // This coordinate is frozen.
                if (! first) {
                    // Create the bitmask that we will have if we increment
                    // the ith coordinate.
                    mask = c->mask_;
                    mask.set(i, true);

                    // Constraint test.
                    if (! constraintMasks.empty()) {
                        found = false;
                        for (const BitmaskType& constraint : constraintMasks) {
                            tmpMask = mask;
                            tmpMask &= constraint;
                            if (! tmpMask.atMostOneBit()) {
                                found = true;
                                break;
                            }
                        }
                        if (found)
                            continue;
                    }

                    // Opposite direction test.
                    if ((*unitMatch[i]) * (*m) >= 0)
                        continue;

                    // Domination test.
                    found = false;
                    for (auto bit = basis.begin(); bit != basis.end(); ++bit) {
                        // Is (**bit) <= (*c + ith unit vector) ?
                        // Quick pre-check using bitmasks.
                        if (! ((*bit)->mask_ <= mask))
                            continue;

                        // Full check.
                        dom = true;
                        for (size_t j = 0; j < dim; ++j) {
                            if (    (j != i && (*c)[j] < (**bit)[j]) ||
                                    (j == i && (*c)[j] + 1 < (**bit)[j])) {
                                dom = false;
                                break;
                            }
                        }
                        if (dom) {
                            found = true;
                            break;
                        }
                    }
                    if (found)
                        continue;
                }

                // Increment!
                if (stackSize == dim) {
                    std::cerr << "ERROR: STACK OVERFLOW" << std::endl;
                    std::exit(1);
                }

                coord[stackSize] = new VecSpec<IntegerType, BitmaskType>(*c);
                ++(*coord[stackSize])[i];
                coord[stackSize]->mask_.set(i, true);

                match[stackSize] = new Vector<IntegerType>(*m);
                (*match[stackSize]) += (*unitMatch[i]);

                frozen[stackSize] = f;
                f.set(i, true);

                ++stackSize;
            }

            // Clean up.
            delete c;
            delete m;

            first = false;
        }

        // Clean up.
        for (size_t i = 0; i < dim; ++i)
            delete unitMatch[i];

        // Output basis elements.
        for (auto ptr : basis) {
            action(Ray(*ptr));
            delete ptr;
        }
    });
}

} // namespace regina

#endif
