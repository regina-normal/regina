
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

#include "triangulation/ntetrahedron.h"
#include "subcomplex/ntrisolidtorus.h"
#include "subcomplex/nlayeredchain.h"

NTriSolidTorus* NTriSolidTorus::clone() const {
    NTriSolidTorus* ans = new NTriSolidTorus();
    for (int i = 0; i < 3; i++) {
        ans->tet[i] = tet[i];
        ans->vertexRoles[i] = vertexRoles[i];
    }
    return ans;
}

bool NTriSolidTorus::isAnnulusSelfIdentified(int index, NPerm* roleMap) const {
    int lower = (index + 1) % 3;
    int upper = (index + 2) % 3;
    if (tet[lower]->getAdjacentTetrahedron(vertexRoles[lower][2]) !=
            tet[upper])
        return false;
    if (tet[lower]->getAdjacentFace(vertexRoles[lower][2]) !=
            vertexRoles[upper][1])
        return false;

    // We have a self-identification.

    if (roleMap)
        *roleMap = vertexRoles[upper].inverse() *
            tet[lower]->getAdjacentTetrahedronGluing(vertexRoles[lower][2]) *
            vertexRoles[lower];

    return true;
}

unsigned long NTriSolidTorus::areAnnuliLinkedMajor(int otherAnnulus) const {
    int right = (otherAnnulus + 1) % 3;
    int left = (otherAnnulus + 2) % 3;
    NTetrahedron* adj = tet[right]->getAdjacentTetrahedron(
        vertexRoles[right][1]);
    if (adj != tet[left]->getAdjacentTetrahedron(vertexRoles[left][2]))
        return 0;
    if (adj == tet[0] || adj == tet[1] || adj == tet[2] || adj == 0)
        return 0;
    NPerm roles = tet[right]->getAdjacentTetrahedronGluing(
        vertexRoles[right][1]) * vertexRoles[right] * NPerm(2, 3, 1, 0);
    if (roles != tet[left]->getAdjacentTetrahedronGluing(
            vertexRoles[left][2]) * vertexRoles[left] * NPerm(3, 2, 0, 1))
        return 0;

    // We've successfully identified the first tetrahedron of the
    // layered chain.
    NLayeredChain chain(adj, roles);
    chain.extendMaximal();
    if (chain.getTop() != tet[otherAnnulus])
        return 0;
    if (chain.getTopVertexRoles() != vertexRoles[otherAnnulus] *
            NPerm(0, 1, 2, 3))
        return 0;

    // Success!
    return chain.getIndex() - 1;
}

NTriSolidTorus* NTriSolidTorus::isTriSolidTorus(NTetrahedron* tet,
        NPerm useVertexRoles) {
    NTriSolidTorus* ans = new NTriSolidTorus();
    ans->tet[0] = tet;
    ans->vertexRoles[0] = useVertexRoles;
    
    // Find the adjacent tetrahedra.
    ans->tet[1] = tet->getAdjacentTetrahedron(useVertexRoles[0]);
    ans->tet[2] = tet->getAdjacentTetrahedron(useVertexRoles[3]);

    // Check that we have three distinct tetrahedra.
    if (ans->tet[1] == 0 || ans->tet[2] == 0 || ans->tet[1] == tet ||
            ans->tet[2] == tet || ans->tet[1] == ans->tet[2]) {
        delete ans;
        return 0;
    }

    // Find the vertex roles for tetrahedra 1 and 2.
    ans->vertexRoles[1] = tet->getAdjacentTetrahedronGluing(useVertexRoles[0])
        * useVertexRoles * NPerm(1, 2, 3, 0);
    ans->vertexRoles[2] = tet->getAdjacentTetrahedronGluing(useVertexRoles[3])
        * useVertexRoles * NPerm(3, 0, 1, 2);
    
    // Finally, check that tetrahedra 1 and 2 are glued together
    // properly.
    NPerm roles1 = ans->vertexRoles[1];
    if (ans->tet[1]->getAdjacentTetrahedron(roles1[0]) != ans->tet[2]) {
        delete ans;
        return 0;
    }

    if (ans->tet[1]->getAdjacentTetrahedronGluing(roles1[0]) * roles1 *
            NPerm(1, 2, 3, 0) != ans->vertexRoles[2]) {
        delete ans;
        return 0;
    }

    // We have the desired structure!
    return ans;
}

