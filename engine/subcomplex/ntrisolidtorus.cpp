
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

#include "algebra/nabeliangroup.h"
#include "manifold/nhandlebody.h"
#include "subcomplex/ntrisolidtorus.h"
#include "subcomplex/nlayeredchain.h"
#include "triangulation/ntetrahedron.h"

namespace regina {

NTriSolidTorus* NTriSolidTorus::clone() const {
    NTriSolidTorus* ans = new NTriSolidTorus();
    for (int i = 0; i < 3; i++) {
        ans->tet[i] = tet[i];
        ans->vertexRoles[i] = vertexRoles[i];
    }
    return ans;
}

bool NTriSolidTorus::isAnnulusSelfIdentified(int index, NPerm4* roleMap) const {
    int lower = (index + 1) % 3;
    int upper = (index + 2) % 3;
    if (tet[lower]->adjacentTetrahedron(vertexRoles[lower][2]) !=
            tet[upper])
        return false;
    if (tet[lower]->adjacentFace(vertexRoles[lower][2]) !=
            vertexRoles[upper][1])
        return false;

    // We have a self-identification.

    if (roleMap)
        *roleMap = vertexRoles[upper].inverse() *
            tet[lower]->adjacentGluing(vertexRoles[lower][2]) *
            vertexRoles[lower];

    return true;
}

unsigned long NTriSolidTorus::areAnnuliLinkedMajor(int otherAnnulus) const {
    int right = (otherAnnulus + 1) % 3;
    int left = (otherAnnulus + 2) % 3;
    NTetrahedron* adj = tet[right]->adjacentTetrahedron(
        vertexRoles[right][1]);
    if (adj != tet[left]->adjacentTetrahedron(vertexRoles[left][2]))
        return 0;
    if (adj == tet[0] || adj == tet[1] || adj == tet[2] || adj == 0)
        return 0;
    NPerm4 roles = tet[right]->adjacentGluing(
        vertexRoles[right][1]) * vertexRoles[right] * NPerm4(2, 3, 1, 0);
    if (roles != tet[left]->adjacentGluing(
            vertexRoles[left][2]) * vertexRoles[left] * NPerm4(3, 2, 0, 1))
        return 0;

    // We've successfully identified the first tetrahedron of the
    // layered chain.
    NLayeredChain chain(adj, roles);
    chain.extendMaximal();
    if (chain.getTop() != tet[otherAnnulus])
        return 0;
    if (chain.getTopVertexRoles() != vertexRoles[otherAnnulus] *
            NPerm4(0, 1, 2, 3))
        return 0;

    // Success!
    return chain.getIndex() - 1;
}

unsigned long NTriSolidTorus::areAnnuliLinkedAxis(int otherAnnulus) const {
    int right = (otherAnnulus + 1) % 3;
    int left = (otherAnnulus + 2) % 3;
    NTetrahedron* adj = tet[right]->adjacentTetrahedron(
        vertexRoles[right][1]);
    if (adj != tet[otherAnnulus]->adjacentTetrahedron(
            vertexRoles[otherAnnulus][2]))
        return 0;
    if (adj == tet[0] || adj == tet[1] || adj == tet[2] || adj == 0)
        return 0;
    NPerm4 roles = tet[right]->adjacentGluing(
        vertexRoles[right][1]) * vertexRoles[right] * NPerm4(2, 1, 0, 3);
    if (roles != tet[otherAnnulus]->adjacentGluing(
            vertexRoles[otherAnnulus][2]) * vertexRoles[otherAnnulus] *
            NPerm4(0, 3, 2, 1))
        return 0;

    // We've successfully identified the first tetrahedron of the
    // layered chain.
    NLayeredChain chain(adj, roles);
    chain.extendMaximal();
    NTetrahedron* top = chain.getTop();
    NPerm4 topRoles(chain.getTopVertexRoles());

    if (top->adjacentTetrahedron(topRoles[3]) != tet[left])
        return 0;
    if (top->adjacentTetrahedron(topRoles[0]) != tet[otherAnnulus])
        return 0;
    if (topRoles != tet[left]->adjacentGluing(
            vertexRoles[left][2]) * vertexRoles[left] * NPerm4(3, 0, 1, 2))
        return 0;
    if (topRoles != tet[otherAnnulus]->adjacentGluing(
            vertexRoles[otherAnnulus][1]) * vertexRoles[otherAnnulus] *
            NPerm4(1, 2, 3, 0))
        return 0;

    // Success!
    return chain.getIndex();
}

NTriSolidTorus* NTriSolidTorus::formsTriSolidTorus(NTetrahedron* tet,
        NPerm4 useVertexRoles) {
    NTriSolidTorus* ans = new NTriSolidTorus();
    ans->tet[0] = tet;
    ans->vertexRoles[0] = useVertexRoles;
    
    // Find the adjacent tetrahedra.
    ans->tet[1] = tet->adjacentTetrahedron(useVertexRoles[0]);
    ans->tet[2] = tet->adjacentTetrahedron(useVertexRoles[3]);

    // Check that we have three distinct tetrahedra.
    if (ans->tet[1] == 0 || ans->tet[2] == 0 || ans->tet[1] == tet ||
            ans->tet[2] == tet || ans->tet[1] == ans->tet[2]) {
        delete ans;
        return 0;
    }

    // Find the vertex roles for tetrahedra 1 and 2.
    ans->vertexRoles[1] = tet->adjacentGluing(useVertexRoles[0])
        * useVertexRoles * NPerm4(1, 2, 3, 0);
    ans->vertexRoles[2] = tet->adjacentGluing(useVertexRoles[3])
        * useVertexRoles * NPerm4(3, 0, 1, 2);
    
    // Finally, check that tetrahedra 1 and 2 are glued together
    // properly.
    NPerm4 roles1 = ans->vertexRoles[1];
    if (ans->tet[1]->adjacentTetrahedron(roles1[0]) != ans->tet[2]) {
        delete ans;
        return 0;
    }

    if (ans->tet[1]->adjacentGluing(roles1[0]) * roles1 *
            NPerm4(1, 2, 3, 0) != ans->vertexRoles[2]) {
        delete ans;
        return 0;
    }

    // We have the desired structure!
    return ans;
}

NAbelianGroup* NTriSolidTorus::getHomologyH1() const {
    NAbelianGroup* ans = new NAbelianGroup();
    ans->addRank();
    return ans;
}

NManifold* NTriSolidTorus::getManifold() const {
    return new NHandlebody(1, true);
}

} // namespace regina

