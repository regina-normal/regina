
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/* To be included from nnormalsurfacelist.h. */

#include "surfaces/flavourregistry.h"

namespace regina {

#define __FLAVOUR_REGISTRY_BODY
#define REGISTER_FLAVOUR(id_name, class, n, a, t) \
    case NNormalSurfaceList::id_name: \
        class::createNonNegativeCone(triangulation, rays, faces); break;

template <class RayOutputIterator, class FaceOutputIterator>
void createNonNegativeCone(NTriangulation* triangulation, int flavour,
        RayOutputIterator rays, FaceOutputIterator faces) {
    switch(flavour) {
        // Import cases from the flavour registry.
        #include "surfaces/flavourregistry.h"
    }
}

#undef REGISTER_FLAVOUR
#undef __FLAVOUR_REGISTRY_BODY

} // namespace regina

