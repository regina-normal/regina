
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

/* To be included from nhilbertdual.h. */

#include <algorithm>
#include <iterator>
#include "enumerate/nenumconstraint.h"
#include "enumerate/ordering.h"
#include "progress/nprogresstypes.h"

namespace regina {

template <class RayClass, class OutputIterator>
void NHilbertDual::enumerateHilbertBasis(OutputIterator results,
        const NMatrixInt& subspace, const NEnumConstraintList* constraints,
        NProgressNumber* progress, unsigned initialRows) {
    // Get the dimension of the entire space in which we are working.
    unsigned dim = subspace.columns();

    // If the space has dimension zero, return no results.
    if (dim == 0)
        return;

    // Are there any hyperplanes at all in the subspace?
    unsigned nEqns = subspace.rows();
    if (nEqns == 0) {
        // No!  Just send back the unit vectors.
        if (progress)
            progress->setOutOf(progress->getOutOf() + 1);

        RayClass* ans;
        for (unsigned i = 0; i < dim; ++i) {
            ans = new RayClass(dim);
            ans->setElement(i, NLargeInteger::one);
            *results++ = ans;
        }

        if (progress)
            progress->incCompleted();
        return;
    }

    // We actually have some work to do.
    if (progress)
        progress->setOutOf(progress->getOutOf() + nEqns + 1);

    // Process the hyperplanes in a good order.
    //
    // Sort the integers 0..(nEqns-1) into the order in which we plan to
    // process the hyperplanes.
    int* hyperplanes = new int[nEqns];
    unsigned i;
    for (i = 0; i < nEqns; ++i)
        hyperplanes[i] = i;

    std::sort(hyperplanes + initialRows, hyperplanes + nEqns,
        NPosOrder(subspace));

    // Create the vector list with which we will work.
    // Fill it with the initial basis elements.
    VecSpecVector list;
    for (i = 0; i < dim; ++i)
        list.push_back(new VecSpec(i, dim));

#if 0
    std::cout << "LIST SIZE: " << list.size() << std::endl;
#endif

    // Intersect the hyperplanes one at a time.
    for (i=0; i<nEqns; i++) {
        intersectHyperplane(list, subspace, hyperplanes[i], constraints);

#if 0
        std::cout << "LIST SIZE: " << list.size() << std::endl;
#endif

        if (progress) {
            progress->incCompleted();
            if (progress->isCancelled())
                break;
        }
    }

    // We're done!
    delete[] hyperplanes;

    RayClass* ans;
    for (typename VecSpecVector::iterator it = list.begin();
            it != list.end(); ++it) {
        /*
        ans = new RayClass(dim);
        for (i = 0; i < dim; ++i)
            ans->setElement(i, (**it)[i]);
        */
        ans = new RayClass(**it);
        *results++ = ans;

        delete *it;
    }

    // All done!
    if (progress)
        progress->incCompleted();
}

} // namespace regina

