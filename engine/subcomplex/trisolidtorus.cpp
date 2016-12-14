
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

#include "algebra/abeliangroup.h"
#include "manifold/handlebody.h"
#include "subcomplex/trisolidtorus.h"
#include "subcomplex/layeredchain.h"
#include "triangulation/dim3.h"

namespace regina {

TriSolidTorus* TriSolidTorus::clone() const {
    TriSolidTorus* ans = new TriSolidTorus();
    for (int i = 0; i < 3; i++) {
        ans->tet[i] = tet[i];
        ans->vertexRoles_[i] = vertexRoles_[i];
    }
    return ans;
}

bool TriSolidTorus::isAnnulusSelfIdentified(int index, Perm<4>* roleMap) const {
    int lower = (index + 1) % 3;
    int upper = (index + 2) % 3;
    if (tet[lower]->adjacentTetrahedron(vertexRoles_[lower][2]) !=
            tet[upper])
        return false;
    if (tet[lower]->adjacentFace(vertexRoles_[lower][2]) !=
            vertexRoles_[upper][1])
        return false;

    // We have a self-identification.

    if (roleMap)
        *roleMap = vertexRoles_[upper].inverse() *
            tet[lower]->adjacentGluing(vertexRoles_[lower][2]) *
            vertexRoles_[lower];

    return true;
}

unsigned long TriSolidTorus::areAnnuliLinkedMajor(int otherAnnulus) const {
    int right = (otherAnnulus + 1) % 3;
    int left = (otherAnnulus + 2) % 3;
    Tetrahedron<3>* adj = tet[right]->adjacentTetrahedron(
        vertexRoles_[right][1]);
    if (adj != tet[left]->adjacentTetrahedron(vertexRoles_[left][2]))
        return 0;
    if (adj == tet[0] || adj == tet[1] || adj == tet[2] || adj == 0)
        return 0;
    Perm<4> roles = tet[right]->adjacentGluing(
        vertexRoles_[right][1]) * vertexRoles_[right] * Perm<4>(2, 3, 1, 0);
    if (roles != tet[left]->adjacentGluing(
            vertexRoles_[left][2]) * vertexRoles_[left] * Perm<4>(3, 2, 0, 1))
        return 0;

    // We've successfully identified the first tetrahedron of the
    // layered chain.
    LayeredChain chain(adj, roles);
    chain.extendMaximal();
    if (chain.top() != tet[otherAnnulus])
        return 0;
    if (chain.topVertexRoles() != vertexRoles_[otherAnnulus] *
            Perm<4>(0, 1, 2, 3))
        return 0;

    // Success!
    return chain.index() - 1;
}

unsigned long TriSolidTorus::areAnnuliLinkedAxis(int otherAnnulus) const {
    int right = (otherAnnulus + 1) % 3;
    int left = (otherAnnulus + 2) % 3;
    Tetrahedron<3>* adj = tet[right]->adjacentTetrahedron(
        vertexRoles_[right][1]);
    if (adj != tet[otherAnnulus]->adjacentTetrahedron(
            vertexRoles_[otherAnnulus][2]))
        return 0;
    if (adj == tet[0] || adj == tet[1] || adj == tet[2] || adj == 0)
        return 0;
    Perm<4> roles = tet[right]->adjacentGluing(
        vertexRoles_[right][1]) * vertexRoles_[right] * Perm<4>(2, 1, 0, 3);
    if (roles != tet[otherAnnulus]->adjacentGluing(
            vertexRoles_[otherAnnulus][2]) * vertexRoles_[otherAnnulus] *
            Perm<4>(0, 3, 2, 1))
        return 0;

    // We've successfully identified the first tetrahedron of the
    // layered chain.
    LayeredChain chain(adj, roles);
    chain.extendMaximal();
    Tetrahedron<3>* top = chain.top();
    Perm<4> topRoles(chain.topVertexRoles());

    if (top->adjacentTetrahedron(topRoles[3]) != tet[left])
        return 0;
    if (top->adjacentTetrahedron(topRoles[0]) != tet[otherAnnulus])
        return 0;
    if (topRoles != tet[left]->adjacentGluing(
            vertexRoles_[left][2]) * vertexRoles_[left] * Perm<4>(3, 0, 1, 2))
        return 0;
    if (topRoles != tet[otherAnnulus]->adjacentGluing(
            vertexRoles_[otherAnnulus][1]) * vertexRoles_[otherAnnulus] *
            Perm<4>(1, 2, 3, 0))
        return 0;

    // Success!
    return chain.index();
}

TriSolidTorus* TriSolidTorus::formsTriSolidTorus(Tetrahedron<3>* tet,
        Perm<4> useVertexRoles) {
    TriSolidTorus* ans = new TriSolidTorus();
    ans->tet[0] = tet;
    ans->vertexRoles_[0] = useVertexRoles;

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
    ans->vertexRoles_[1] = tet->adjacentGluing(useVertexRoles[0])
        * useVertexRoles * Perm<4>(1, 2, 3, 0);
    ans->vertexRoles_[2] = tet->adjacentGluing(useVertexRoles[3])
        * useVertexRoles * Perm<4>(3, 0, 1, 2);

    // Finally, check that tetrahedra 1 and 2 are glued together
    // properly.
    Perm<4> roles1 = ans->vertexRoles_[1];
    if (ans->tet[1]->adjacentTetrahedron(roles1[0]) != ans->tet[2]) {
        delete ans;
        return 0;
    }

    if (ans->tet[1]->adjacentGluing(roles1[0]) * roles1 *
            Perm<4>(1, 2, 3, 0) != ans->vertexRoles_[2]) {
        delete ans;
        return 0;
    }

    // We have the desired structure!
    return ans;
}

AbelianGroup* TriSolidTorus::homology() const {
    AbelianGroup* ans = new AbelianGroup();
    ans->addRank();
    return ans;
}

Manifold* TriSolidTorus::manifold() const {
    return new Handlebody(1, true);
}

} // namespace regina

