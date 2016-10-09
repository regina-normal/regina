
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

/*! \file enumerate/hilbertprimal-impl.h
 *  \brief Contains implementations of template functions from hilbertprimal.h.
 *
 *  This file is automatically included from hilbertprimal.h; there
 *  is no need for end users to include it explicitly.
 */

#ifndef __HILBERTPRIMAL_IMPL_H
#ifndef __DOXYGEN
#define __HILBERTPRIMAL_IMPL_H
#endif

#include "regina-core.h"
#include "regina-config.h"
#include "enumerate/enumconstraints.h"
#include "enumerate/hilbertprimal.h"
#include "enumerate/maxadmissible.h"
#include "libnormaliz/cone.h"
#include "maths/ray.h"
#include "progress/progresstracker.h"
#include <list>
#include <set>
#include <vector>
#include <gmpxx.h>

namespace regina {

template <class RayClass, class RayIterator, class OutputIterator>
void HilbertPrimal::enumerateHilbertBasis(OutputIterator results,
        const RayIterator& raysBegin, const RayIterator& raysEnd,
        const EnumConstraints* constraints, ProgressTracker* tracker) {
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
        enumerateUsingBitmask<RayClass, Bitmask1<unsigned> >(results,
            raysBegin, raysEnd, constraints, tracker);
    else if (dim <= 8 * sizeof(unsigned long))
        enumerateUsingBitmask<RayClass, Bitmask1<unsigned long> >(results,
            raysBegin, raysEnd, constraints, tracker);
    else if (dim <= 8 * sizeof(unsigned long long))
        enumerateUsingBitmask<RayClass, Bitmask1<unsigned long long> >(results,
            raysBegin, raysEnd, constraints, tracker);
    else if (dim <= 8 * sizeof(unsigned long long) + 8 * sizeof(unsigned))
        enumerateUsingBitmask<RayClass,
            Bitmask2<unsigned long long, unsigned> >(results,
            raysBegin, raysEnd, constraints, tracker);
    else if (dim <= 8 * sizeof(unsigned long long) +
            8 * sizeof(unsigned long))
        enumerateUsingBitmask<RayClass,
            Bitmask2<unsigned long long, unsigned long> >(
            results, raysBegin, raysEnd, constraints, tracker);
    else if (dim <= 16 * sizeof(unsigned long long))
        enumerateUsingBitmask<RayClass, Bitmask2<unsigned long long> >(results,
            raysBegin, raysEnd, constraints, tracker);
    else
        enumerateUsingBitmask<RayClass, Bitmask>(results,
            raysBegin, raysEnd, constraints, tracker);
}

template <class RayClass, class BitmaskType,
        class RayIterator, class OutputIterator>
void HilbertPrimal::enumerateUsingBitmask(OutputIterator results,
        const RayIterator& raysBegin, const RayIterator& raysEnd,
        const EnumConstraints* constraints, ProgressTracker* tracker) {
    // We know at this point that the dimension is non-zero.
    size_t dim = (*raysBegin).size();

    // First enumerate all maximal admissible faces.
    if (tracker)
        tracker->setPercent(10);
    std::vector<BitmaskType>* maxFaces = MaxAdmissible::enumerate<BitmaskType>(
        raysBegin, raysEnd, constraints);

    // Now use normaliz to process each face.
    if (tracker)
        tracker->setPercent(30);

    std::set<std::vector<mpz_class> > finalBasis;
    std::vector<const Ray*> face;
    typename std::vector<BitmaskType>::const_iterator mit;
    RayIterator rit;
    unsigned i;
    std::vector<std::vector<mpz_class> >::const_iterator hlit;
    std::set<std::vector<mpz_class> >::const_iterator hsit;
    std::vector<mpz_class>::const_iterator hvit;
    for (mit = maxFaces->begin(); mit != maxFaces->end(); ++mit) {
        // Locate the extremal rays that generate this face.
        std::vector<std::vector<mpz_class> > input;
        for (rit = raysBegin; rit != raysEnd; ++rit)
            if (inFace(*rit, *mit)) {
                input.push_back(std::vector<mpz_class>());
                std::vector<mpz_class>& v(input.back());
                v.reserve(dim);
                for (i = 0; i < dim; ++i) {
                    if ((*rit)[i].isNative())
                        v.push_back(mpz_class((*rit)[i].longValue()));
                    else
                        v.push_back(mpz_class((*rit)[i].rawData()));
                }
            }
        libnormaliz::Cone<mpz_class> cone(
            libnormaliz::Type::integral_closure, input);
        libnormaliz::ConeProperties wanted(
            libnormaliz::ConeProperty::HilbertBasis);
        cone.deactivateChangeOfPrecision();
        cone.compute(wanted);

        if (! cone.isComputed(libnormaliz::ConeProperty::HilbertBasis)) {
            // TODO: Bail properly.
            std::cerr << "ERROR: Hilbert basis not computed!" << std::endl;
            continue;
        }
        const std::vector<std::vector<mpz_class> > basis =
            cone.getHilbertBasis();
        for (hlit = basis.begin(); hlit != basis.end(); ++hlit)
            finalBasis.insert(*hlit);
    }

    if (tracker)
        tracker->setPercent(90);

    RayClass* ans;
    LargeInteger tmpInt;
    for (hsit = finalBasis.begin(); hsit != finalBasis.end(); ++hsit) {
        ans = new RayClass(dim);
        for (i = 0, hvit = hsit->begin(); hvit != hsit->end(); ++hvit, ++i) {
            // We make two copies of the GMP integer instead of one.
            // This is because Vector/Ray does not give us direct
            // non-const access to its elements, and so we need a
            // temporary LargeInteger to pass through setElement() instead.
            tmpInt.setRaw(hvit->get_mpz_t());
            ans->setElement(i, tmpInt);
        }
        *results++ = ans;
    }

    // All done!
    delete maxFaces;
    if (tracker)
        tracker->setPercent(100);
}

template <class BitmaskType>
bool HilbertPrimal::inFace(const Ray& ray, const BitmaskType& face) {
    for (unsigned i = 0; i < ray.size(); ++i)
        if ((! face.get(i)) && ray[i] > 0)
            return false;
    return true;
}

} // namespace regina

#endif
