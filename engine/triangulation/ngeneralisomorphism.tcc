
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

/* Template definitions for ngeneralisomorphism.h. */

#include <algorithm>
#include <cstdlib>
#include "triangulation/ngeneralisomorphism.h"

namespace regina {

template <int dim>
void NGeneralIsomorphism<dim>::writeTextShort(std::ostream& out) const {
    out << "Isomorphism between " << dim << "-manifold triangulations";
}

template <int dim>
void NGeneralIsomorphism<dim>::writeTextLong(std::ostream& out) const {
    for (unsigned i = 0; i < nSimplices_; ++i)
        out << i << " -> " << simpImage_[i] << " (" << facetPerm_[i] << ")\n";
}

template <int dim>
bool NGeneralIsomorphism<dim>::isIdentity() const {
    for (unsigned p = 0; p < nSimplices_; ++p) {
        if (simpImage_[p] != static_cast<int>(p))
            return false;
        if (! facetPerm_[p].isIdentity())
            return false;
    }
    return true;
}

template <int dim>
NGeneralIsomorphism<dim>::NGeneralIsomorphism(
        const NGeneralIsomorphism<dim>& cloneMe) :
        ShareableObject(),
        nSimplices_(cloneMe.nSimplices_),
        simpImage_(cloneMe.nSimplices_ > 0 ?
            new int[cloneMe.nSimplices_] : 0),
        facetPerm_(cloneMe.nSimplices_ > 0 ?
            new Perm[cloneMe.nSimplices_] : 0) {
    std::copy(cloneMe.simpImage_, cloneMe.simpImage_ + nSimplices_,
        simpImage_);
    std::copy(cloneMe.facetPerm_, cloneMe.facetPerm_ + nSimplices_,
        facetPerm_);
}

} // namespace regina

