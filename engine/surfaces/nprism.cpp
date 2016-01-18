
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

#include "surfaces/nnormalsurface.h"
#include "surfaces/nprism.h"
#include "triangulation/ntriangulation.h"

namespace regina {

std::ostream& operator << (std::ostream& out, const NPrismSpec& spec) {
    out << '(' << spec.tetIndex << ", " << spec.edge << ')';
    return out;
}

NPrismSetSurface::NPrismSetSurface(const NNormalSurface& surface) {
    const NTriangulation* tri = surface.triangulation();
    unsigned long nTet = tri->size();

    if (nTet == 0) {
        quadType_ = 0;
        return;
    }

    // Work out which tetrahedra contain which quad types.
    quadType_ = new signed char[nTet];
    for (unsigned long tet = 0; tet < nTet; tet++)
        if (surface.quads(tet, 0) != 0)
            quadType_[tet] = 0;
        else if (surface.quads(tet, 1) != 0)
            quadType_[tet] = 1;
        else if (surface.quads(tet, 2) != 0)
            quadType_[tet] = 2;
        else
            quadType_[tet] = -1;
}

} // namespace regina

