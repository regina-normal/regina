
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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

#include "surfaces/nnormalsurface.h"
#include "surfaces/nprism.h"
#include "triangulation/ntriangulation.h"

namespace regina {

std::ostream& operator << (std::ostream& out, const NPrismSpec& spec) {
    out << '(' << spec.tetIndex << ", " << spec.edge << ')';
    return out;
}

NPrismSetSurface::NPrismSetSurface(const NNormalSurface& surface) {
    NTriangulation* tri = surface.getTriangulation();
    unsigned long nTet = tri->getNumberOfTetrahedra();

    if (nTet == 0) {
        quadType = 0;
        return;
    }

    // Work out which tetrahedra contain which quad types.
    quadType = new signed char[nTet];
    for (unsigned long tet = 0; tet < nTet; tet++)
        if (surface.getQuadCoord(tet, 0) != 0)
            quadType[tet] = 0;
        else if (surface.getQuadCoord(tet, 1) != 0)
            quadType[tet] = 1;
        else if (surface.getQuadCoord(tet, 2) != 0)
            quadType[tet] = 2;
        else
            quadType[tet] = -1;
}

} // namespace regina

