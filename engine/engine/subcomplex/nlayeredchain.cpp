
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "nedge.h"
    #include "ntetrahedron.h"
    #include "nlayeredchain.h"
#else
    #include "engine/triangulation/nedge.h"
    #include "engine/triangulation/ntetrahedron.h"
    #include "engine/subcomplex/nlayeredchain.h"
#endif

bool NLayeredChain::extendAbove() {
    NTetrahedron* adj = top->getAdjacentTetrahedron(topVertexRoles[0]);
    if (adj == bottom || adj == top)
        return false;
    if (adj != top->getAdjacentTetrahedron(topVertexRoles[3]))
        return false;

    // Check the gluings.
    NPerm adjRoles = top->getAdjacentTetrahedronGluing(topVertexRoles[0]) *
        topVertexRoles * NPerm(0, 1);
    if (adjRoles != top->getAdjacentTetrahedronGluing(topVertexRoles[3]) *
            topVertexRoles * NPerm(2, 3))
        return false;

    // We can extend the layered chain.
    top = adj;
    topVertexRoles = adjRoles;
    index++;
    return true;
}

bool NLayeredChain::extendBelow() {
    NTetrahedron* adj = bottom->getAdjacentTetrahedron(bottomVertexRoles[1]);
    if (adj == bottom || adj == top)
        return false;
    if (adj != bottom->getAdjacentTetrahedron(bottomVertexRoles[2]))
        return false;

    // Check the gluings.
    NPerm adjRoles = bottom->getAdjacentTetrahedronGluing(bottomVertexRoles[1])
        * bottomVertexRoles * NPerm(0, 1);
    if (adjRoles != bottom->getAdjacentTetrahedronGluing(bottomVertexRoles[2])
            * bottomVertexRoles * NPerm(2, 3))
        return false;

    // We can extend the layered chain.
    bottom = adj;
    bottomVertexRoles = adjRoles;
    index++;
    return true;
}

bool NLayeredChain::extendMaximal() {
    bool changed = false;
    while (extendAbove())
        changed = true;
    while (extendBelow())
        changed = true;
    return changed;
}

