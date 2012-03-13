
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

/* end stub */

/* To be included from nhilbertprimal.h. */

#include "regina-core.h"
#include "regina-config.h"
#include "enumerate/nenumconstraint.h"
#include "enumerate/nmaxadmissible.h"
#include "enumerate/normaliz/cone.h"
#include "maths/nray.h"
#include "progress/nprogresstypes.h"
#include <list>
#include <set>
#include <vector>
#include <gmpxx.h>

namespace regina {

template <class RayClass, class RayIterator, class OutputIterator>
void NHilbertPrimal::enumerateHilbertBasis(OutputIterator results,
        const RayIterator& raysBegin, const RayIterator& raysEnd,
        const NEnumConstraintList* constraints, NProgressMessage* progress) {
    if (raysBegin == raysEnd) {
        // No extremal rays; no Hilbert basis.
        return;
    }

    // Get the dimension of the space.
    unsigned dim = (*raysBegin)->size();
    if (dim == 0)
        return;

    // Choose a bitmask type that can hold dim bits.
    // Use a (much faster) optimised bitmask type if we can.
    // Then farm the work out to the real enumeration routine that is
    // templated on the bitmask type.
    if (dim <= 8 * sizeof(unsigned))
        enumerateUsingBitmask<RayClass, NBitmask1<unsigned> >(results,
            raysBegin, raysEnd, constraints, progress);
    else if (dim <= 8 * sizeof(unsigned long))
        enumerateUsingBitmask<RayClass, NBitmask1<unsigned long> >(results,
            raysBegin, raysEnd, constraints, progress);
#ifdef LONG_LONG_FOUND
    else if (dim <= 8 * sizeof(unsigned long long))
        enumerateUsingBitmask<RayClass, NBitmask1<unsigned long long> >(results,
            raysBegin, raysEnd, constraints, progress);
    else if (dim <= 8 * sizeof(unsigned long long) + 8 * sizeof(unsigned))
        enumerateUsingBitmask<RayClass,
            NBitmask2<unsigned long long, unsigned> >(results,
            raysBegin, raysEnd, constraints, progress);
    else if (dim <= 8 * sizeof(unsigned long long) +
            8 * sizeof(unsigned long))
        enumerateUsingBitmask<RayClass,
            NBitmask2<unsigned long long, unsigned long> >(
            results, raysBegin, raysEnd, constraints, progress);
    else if (dim <= 16 * sizeof(unsigned long long))
        enumerateUsingBitmask<RayClass, NBitmask2<unsigned long long> >(results,
            raysBegin, raysEnd, constraints, progress);
#else
    else if (dim <= 8 * sizeof(unsigned long) + 8 * sizeof(unsigned))
        enumerateUsingBitmask<RayClass,
            NBitmask2<unsigned long, unsigned> >(results,
            raysBegin, raysEnd, constraints, progress);
    else if (dim <= 16 * sizeof(unsigned long))
        enumerateUsingBitmask<RayClass, NBitmask2<unsigned long> >(results,
            raysBegin, raysEnd, constraints, progress);
#endif
    else
        enumerateUsingBitmask<RayClass, NBitmask>(results,
            raysBegin, raysEnd, constraints, progress);
}

template <class RayClass, class BitmaskType,
        class RayIterator, class OutputIterator>
void NHilbertPrimal::enumerateUsingBitmask(OutputIterator results,
        const RayIterator& raysBegin, const RayIterator& raysEnd,
        const NEnumConstraintList* constraints, NProgressMessage* progress) {
    // We know at this point that the dimension is non-zero.
    unsigned dim = (*raysBegin)->size();

    // First enumerate all maximal admissible faces.
    if (progress)
        progress->setMessage("Enumerating maximal admissible faces");
    std::vector<BitmaskType>* maxFaces = NMaxAdmissible::enumerate<BitmaskType>(
        raysBegin, raysEnd, constraints);

    // Now use normaliz to process each face.
    if (progress)
        progress->setMessage(
            "Running primal algorithm on maximal admissible faces");

    std::set<std::vector<mpz_class> > finalBasis;
    std::vector<const NRay*> face;
    typename std::vector<BitmaskType>::const_iterator mit;
    RayIterator rit;
    RayClass* tmp;
    unsigned i;
    std::list<std::vector<mpz_class> >::const_iterator hlit;
    std::set<std::vector<mpz_class> >::const_iterator hsit;
    std::vector<mpz_class>::const_iterator hvit;
    for (mit = maxFaces->begin(); mit != maxFaces->end(); ++mit) {
        // Locate the extremal rays that generate this face.
        std::list<std::vector<mpz_class> > input;
        for (rit = raysBegin; rit != raysEnd; ++rit)
            if (inFace(**rit, *mit)) {
                input.push_back(std::vector<mpz_class>());
                std::vector<mpz_class>& v(input.back());
                v.reserve(dim);
                for (i = 0; i < dim; ++i)
                    v.push_back(mpz_class((**rit)[i].rawData()));
            }
        libnormaliz::Cone<mpz_class> cone(input,
            0 /* generators, integral closure */);
        libnormaliz::ConeProperties wanted(
            libnormaliz::ConeProperty::HilbertBasis);
        cone.compute(wanted);

        if (! cone.isComputed(libnormaliz::ConeProperty::HilbertBasis)) {
            // TODO: Bail properly.
            std::cerr << "ERROR: Hilbert basis not computed!" << std::endl;
            continue;
        }
        const std::list<std::vector<mpz_class> >& basis =
            cone.getHilbertBasis();
        for (hlit = basis.begin(); hlit != basis.end(); ++hlit)
            finalBasis.insert(*hlit);
    }

    if (progress)
        progress->setMessage("Collecting results");

    RayClass* ans;
    NLargeInteger tmpInt;
    for (hsit = finalBasis.begin(); hsit != finalBasis.end(); ++hsit) {
        ans = new RayClass(dim);
        for (i = 0, hvit = hsit->begin(); hvit != hsit->end(); ++hvit, ++i) {
            // We make two copies of the GMP integer instead of one.
            // This is because NVector/NRay does not give us direct
            // non-const access to its elements, and so we need a
            // temporary NLargeInteger to pass through setElement() instead.
            tmpInt.setRaw(hvit->get_mpz_t());
            ans->setElement(i, tmpInt);
        }
        *results++ = ans;
    }

    // All done!
    delete maxFaces;
    if (progress)
        progress->setMessage("Hilbert basis enumeration complete");
}

template <class BitmaskType>
bool NHilbertPrimal::inFace(const NRay& ray, const BitmaskType& face) {
    for (unsigned i = 0; i < ray.size(); ++i)
        if ((! face.get(i)) && ray[i] > 0)
            return false;
    return true;
}

} // namespace regina

