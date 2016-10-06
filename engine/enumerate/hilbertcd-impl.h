
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/*! \file enumerate/hilbertcd-impl.h
 *  \brief Contains implementations of template functions from hilbertcd.h.
 *
 *  This file is automatically included from hilbertcd.h; there
 *  is no need for end users to include it explicitly.
 */

#ifndef __HILBERTCD_IMPL_H
#ifndef __DOXYGEN
#define __HILBERTCD_IMPL_H
#endif

#include "regina-core.h"
#include "regina-config.h"
#include "enumerate/enumconstraints.h"
#include "enumerate/hilbertcd.h"
#include "maths/matrix.h"
#include "maths/ray.h"
#include "utilities/bitmask.h"
#include <list>

namespace regina {

template <class RayClass, class OutputIterator>
void HilbertCD::enumerateHilbertBasis(OutputIterator results,
        const MatrixInt& subspace, const EnumConstraints* constraints) {
    // Get the dimension of the space.
    size_t dim = subspace.columns();
    if (dim == 0)
        return;

    // Choose a bitmask type that can hold dim bits.
    // Use a (much faster) optimised bitmask type if we can.
    // Then farm the work out to the real enumeration routine that is
    // templated on the bitmask type.
    if (dim <= 8 * sizeof(unsigned))
        enumerateUsingBitmask<RayClass, Bitmask1<unsigned> >(results,
            subspace, constraints);
    else if (dim <= 8 * sizeof(unsigned long))
        enumerateUsingBitmask<RayClass, Bitmask1<unsigned long> >(results,
            subspace, constraints);
    else if (dim <= 8 * sizeof(unsigned long long))
        enumerateUsingBitmask<RayClass, Bitmask1<unsigned long long> >(results,
            subspace, constraints);
    else if (dim <= 8 * sizeof(unsigned long long) + 8 * sizeof(unsigned))
        enumerateUsingBitmask<RayClass,
            Bitmask2<unsigned long long, unsigned> >(results,
            subspace, constraints);
    else if (dim <= 8 * sizeof(unsigned long long) +
            8 * sizeof(unsigned long))
        enumerateUsingBitmask<RayClass,
            Bitmask2<unsigned long long, unsigned long> >(
            results, subspace, constraints);
    else if (dim <= 16 * sizeof(unsigned long long))
        enumerateUsingBitmask<RayClass, Bitmask2<unsigned long long> >(results,
            subspace, constraints);
    else
        enumerateUsingBitmask<RayClass, Bitmask>(results,
            subspace, constraints);
}

template <class RayClass, class BitmaskType, class OutputIterator>
void HilbertCD::enumerateUsingBitmask(OutputIterator results,
        const MatrixInt& subspace, const EnumConstraints* constraints) {
    // Stack-based Contejean-Devie algorithm (Information & Computation, 1994).
    size_t dim = subspace.columns();
    size_t nEqns = subspace.rows();

    // Convert the set of constraints into bitmasks, where for every
    // original coordinate listed in the constraint, the corresponding
    // bit is set to 1.
    BitmaskType* constraintsBegin = 0;
    BitmaskType* constraintsEnd = 0;
    if (constraints && ! constraints->empty()) {
        constraintsBegin = new BitmaskType[constraints->size()];

        EnumConstraints::const_iterator cit;
        for (cit = constraints->begin(), constraintsEnd = constraintsBegin;
                cit != constraints->end(); ++cit, ++constraintsEnd) {
            constraintsEnd->reset(dim);
            constraintsEnd->set(cit->begin(), cit->end(), true);
        }
    }

    std::list<VecSpec<BitmaskType>*> basis;
    typename std::list<VecSpec<BitmaskType>*>::iterator bit;

    Ray** unitMatch = new Ray*[dim];
    int i, j;
    for (i = 0; i < dim; ++i) {
        unitMatch[i] = new Ray(nEqns);
        for (j = 0; j < nEqns; ++j)
            unitMatch[i]->setElement(j, LargeInteger(subspace.entry(j, i)));
    }

    unsigned stackSize;
    // All vectors/rays are created and destroyed.
    // Bitmasks on the other hand are reused.
    VecSpec<BitmaskType>** coord = new VecSpec<BitmaskType>*[dim];
    Ray** match = new Ray*[dim];
    BitmaskType* frozen = new BitmaskType[dim];

    for (i = 0; i < dim; ++i)
        frozen[i].reset(dim); // All false.
    
    // Push the zero vector.
    coord[0] = new VecSpec<BitmaskType>(dim);
    match[0] = new Ray(nEqns);
    stackSize = 1; // The zero vector is already on top.
    bool first = true;

    VecSpec<BitmaskType> *c;
    Ray *m;
    BitmaskType f(dim);
    BitmaskType mask(dim), tmpMask(dim);
    BitmaskType* constraint;
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
            for (i = 0; i < nEqns; ++i)
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
        for (i = 0; i < dim; ++i) {
            if (f.get(i))
                continue; // This coordinate is frozen.
            if (! first) {
                // Create the bitmask that we will have if we increment
                // the ith coordinate.
                mask = c->mask_;
                mask.set(i, true);

                // Constraint test.
                if (constraintsBegin) {
                    found = false;
                    for (constraint = constraintsBegin;
                            constraint != constraintsEnd; ++constraint) {
                        tmpMask = mask;
                        tmpMask &= *constraint;
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
                for (bit = basis.begin(); bit != basis.end(); ++bit) {
                    // Is (**bit) <= (*c + ith unit vector) ?
                    // Quick pre-check using bitmasks.
                    if (! ((*bit)->mask_ <= mask))
                        continue;

                    // Full check.
                    dom = true;
                    for (j = 0; j < dim; ++j) {
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

            coord[stackSize] = new VecSpec<BitmaskType>(*c);
            coord[stackSize]->setElement(i, (*coord[stackSize])[i] + 1);
            coord[stackSize]->mask_.set(i, true);

            match[stackSize] = new Ray(*m);
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
    for (i = 0; i < dim; ++i)
        delete unitMatch[i];

    delete[] unitMatch;
    delete[] coord;
    delete[] match;
    delete[] frozen;
    delete[] constraintsBegin;

    // Output basis elements.
    for (bit = basis.begin(); bit != basis.end(); ++bit) {
        *results++ = new RayClass(**bit);
        delete *bit;
    }
}

} // namespace regina

#endif
