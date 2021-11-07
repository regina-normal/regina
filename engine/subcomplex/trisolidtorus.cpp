
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

std::optional<Perm<4>> TriSolidTorus::isAnnulusSelfIdentified(int index) const {
    int lower = (index + 1) % 3;
    int upper = (index + 2) % 3;
    if (tet_[lower]->adjacentTetrahedron(vertexRoles_[lower][2]) != tet_[upper])
        return std::nullopt;
    if (tet_[lower]->adjacentFace(vertexRoles_[lower][2]) !=
            vertexRoles_[upper][1])
        return std::nullopt;

    // We have a self-identification.

    return vertexRoles_[upper].inverse() *
        tet_[lower]->adjacentGluing(vertexRoles_[lower][2]) *
        vertexRoles_[lower];
}

unsigned long TriSolidTorus::areAnnuliLinkedMajor(int otherAnnulus) const {
    int right = (otherAnnulus + 1) % 3;
    int left = (otherAnnulus + 2) % 3;
    Tetrahedron<3>* adj = tet_[right]->adjacentTetrahedron(
        vertexRoles_[right][1]);
    if (adj != tet_[left]->adjacentTetrahedron(vertexRoles_[left][2]))
        return 0;
    if (adj == tet_[0] || adj == tet_[1] || adj == tet_[2] || ! adj)
        return 0;
    Perm<4> roles = tet_[right]->adjacentGluing(
        vertexRoles_[right][1]) * vertexRoles_[right] * Perm<4>(2, 3, 1, 0);
    if (roles != tet_[left]->adjacentGluing(
            vertexRoles_[left][2]) * vertexRoles_[left] * Perm<4>(3, 2, 0, 1))
        return 0;

    // We've successfully identified the first tetrahedron of the
    // layered chain.
    LayeredChain chain(adj, roles);
    chain.extendMaximal();
    if (chain.top() != tet_[otherAnnulus])
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
    Tetrahedron<3>* adj = tet_[right]->adjacentTetrahedron(
        vertexRoles_[right][1]);
    if (adj != tet_[otherAnnulus]->adjacentTetrahedron(
            vertexRoles_[otherAnnulus][2]))
        return 0;
    if (adj == tet_[0] || adj == tet_[1] || adj == tet_[2] || ! adj)
        return 0;
    Perm<4> roles = tet_[right]->adjacentGluing(
        vertexRoles_[right][1]) * vertexRoles_[right] * Perm<4>(2, 1, 0, 3);
    if (roles != tet_[otherAnnulus]->adjacentGluing(
            vertexRoles_[otherAnnulus][2]) * vertexRoles_[otherAnnulus] *
            Perm<4>(0, 3, 2, 1))
        return 0;

    // We've successfully identified the first tetrahedron of the
    // layered chain.
    LayeredChain chain(adj, roles);
    chain.extendMaximal();
    Tetrahedron<3>* top = chain.top();
    Perm<4> topRoles(chain.topVertexRoles());

    if (top->adjacentTetrahedron(topRoles[3]) != tet_[left])
        return 0;
    if (top->adjacentTetrahedron(topRoles[0]) != tet_[otherAnnulus])
        return 0;
    if (topRoles != tet_[left]->adjacentGluing(
            vertexRoles_[left][2]) * vertexRoles_[left] * Perm<4>(3, 0, 1, 2))
        return 0;
    if (topRoles != tet_[otherAnnulus]->adjacentGluing(
            vertexRoles_[otherAnnulus][1]) * vertexRoles_[otherAnnulus] *
            Perm<4>(1, 2, 3, 0))
        return 0;

    // Success!
    return chain.index();
}

std::unique_ptr<TriSolidTorus> TriSolidTorus::recognise(Tetrahedron<3>* tet,
        Perm<4> useVertexRoles) {
    std::unique_ptr<TriSolidTorus> ans(new TriSolidTorus());
    ans->tet_[0] = tet;
    ans->vertexRoles_[0] = useVertexRoles;

    // Find the adjacent tetrahedra.
    ans->tet_[1] = tet->adjacentTetrahedron(useVertexRoles[0]);
    ans->tet_[2] = tet->adjacentTetrahedron(useVertexRoles[3]);

    // Check that we have three distinct tetrahedra.
    if (ans->tet_[1] == nullptr || ans->tet_[2] == nullptr ||
            ans->tet_[1] == tet || ans->tet_[2] == tet ||
            ans->tet_[1] == ans->tet_[2])
        return nullptr;

    // Find the vertex roles for tetrahedra 1 and 2.
    ans->vertexRoles_[1] = tet->adjacentGluing(useVertexRoles[0])
        * useVertexRoles * Perm<4>(1, 2, 3, 0);
    ans->vertexRoles_[2] = tet->adjacentGluing(useVertexRoles[3])
        * useVertexRoles * Perm<4>(3, 0, 1, 2);

    // Finally, check that tetrahedra 1 and 2 are glued together
    // properly.
    Perm<4> roles1 = ans->vertexRoles_[1];
    if (ans->tet_[1]->adjacentTetrahedron(roles1[0]) != ans->tet_[2])
        return nullptr;

    if (ans->tet_[1]->adjacentGluing(roles1[0]) * roles1 *
            Perm<4>(1, 2, 3, 0) != ans->vertexRoles_[2])
        return nullptr;

    // We have the desired structure!
    return ans;
}

AbelianGroup TriSolidTorus::homology() const {
    AbelianGroup ans;
    ans.addRank();
    return ans;
}

std::unique_ptr<Manifold> TriSolidTorus::manifold() const {
    return std::make_unique<Handlebody>(1, true);
}

} // namespace regina

