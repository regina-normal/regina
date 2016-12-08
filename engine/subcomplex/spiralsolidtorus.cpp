
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

#include <set>
#include <vector>
#include "algebra/abeliangroup.h"
#include "manifold/handlebody.h"
#include "subcomplex/spiralsolidtorus.h"
#include "triangulation/dim3.h"

namespace regina {

SpiralSolidTorus* SpiralSolidTorus::clone() const {
    SpiralSolidTorus* ans = new SpiralSolidTorus(nTet);
    for (size_t i = 0; i < nTet; i++) {
        ans->tet[i] = tet[i];
        ans->vertexRoles_[i] = vertexRoles_[i];
    }
    return ans;
}

void SpiralSolidTorus::reverse() {
    Tetrahedron<3>** newTet = new Tetrahedron<3>*[nTet];
    Perm<4>* newRoles = new Perm<4>[nTet];

    Perm<4> switchPerm(3, 2, 1, 0);
    for (size_t i = 0; i < nTet; i++) {
        newTet[i] = tet[nTet - 1 - i];
        newRoles[i] = vertexRoles_[nTet - 1 - i] * switchPerm;
    }

    delete[] tet;
    delete[] vertexRoles_;
    tet = newTet;
    vertexRoles_ = newRoles;
}

void SpiralSolidTorus::cycle(size_t k) {
    Tetrahedron<3>** newTet = new Tetrahedron<3>*[nTet];
    Perm<4>* newRoles = new Perm<4>[nTet];

    for (size_t i = 0; i < nTet; i++) {
        newTet[i] = tet[(i + k) % nTet];
        newRoles[i] = vertexRoles_[(i + k) % nTet];
    }

    delete[] tet;
    delete[] vertexRoles_;
    tet = newTet;
    vertexRoles_ = newRoles;
}

bool SpiralSolidTorus::makeCanonical(const Triangulation<3>* tri) {
    size_t i, index;

    size_t baseTet = 0;
    size_t baseIndex = tet[0]->index();
    for (i = 1; i < nTet; i++) {
        index = tet[i]->index();
        if (index < baseIndex) {
            baseIndex = index;
            baseTet = i;
        }
    }

    bool reverseAlso = (vertexRoles_[baseTet][0] > vertexRoles_[baseTet][3]);

    if (baseTet == 0 && (! reverseAlso))
        return false;

    Tetrahedron<3>** newTet = new Tetrahedron<3>*[nTet];
    Perm<4>* newRoles = new Perm<4>[nTet];

    if (reverseAlso) {
        // Make baseTet into tetrahedron 0 and reverse.
        Perm<4> switchPerm(3, 2, 1, 0);
        for (size_t i = 0; i < nTet; i++) {
            newTet[i] = tet[(baseTet + nTet - i) % nTet];
            newRoles[i] = vertexRoles_[(baseTet + nTet - i) % nTet] *
                switchPerm;
        }
    } else {
        // Make baseTet into tetrahedron 0 but don't reverse.
        for (size_t i = 0; i < nTet; i++) {
            newTet[i] = tet[(i + baseTet) % nTet];
            newRoles[i] = vertexRoles_[(i + baseTet) % nTet];
        }
    }

    delete[] tet;
    delete[] vertexRoles_;
    tet = newTet;
    vertexRoles_ = newRoles;

    return true;
}

bool SpiralSolidTorus::isCanonical(const Triangulation<3>* tri) const {
    if (vertexRoles_[0][0] > vertexRoles_[0][3])
        return false;

    long baseIndex = tet[0]->index();
    for (size_t i = 1; i < nTet; i++)
        if (tet[i]->index() < baseIndex)
            return false;

    return true;
}

SpiralSolidTorus* SpiralSolidTorus::formsSpiralSolidTorus(Tetrahedron<3>* tet,
        Perm<4> useVertexRoles) {
    Perm<4> invRoleMap(1, 2, 3, 0);  // Maps upper roles to lower roles.

    Tetrahedron<3>* base = tet;
    Perm<4> baseRoles(useVertexRoles);

    std::vector<Tetrahedron<3>*> tets;
    std::vector<Perm<4>> roles;
    std::set<Tetrahedron<3>*> usedTets;

    tets.push_back(tet);
    roles.push_back(useVertexRoles);
    usedTets.insert(tet);

    Tetrahedron<3>* adjTet;
    Perm<4> adjRoles;

    while (1) {
        // Examine the tetrahedron beyond tet.
        adjTet = tet->adjacentTetrahedron(useVertexRoles[0]);
        adjRoles = tet->adjacentGluing(useVertexRoles[0]) *
            useVertexRoles * invRoleMap;

        // Check that we haven't hit the boundary.
        if (adjTet == 0)
            return 0;

        if (adjTet == base) {
            // We're back at the beginning of the loop.
            // Check that everything is glued up correctly.
            if (adjRoles != baseRoles)
                return 0;

            // Success!
            break;
        }

        if (usedTets.count(adjTet))
            return 0;

        // Move on to the next tetrahedron.
        tet = adjTet;
        useVertexRoles = adjRoles;

        tets.push_back(tet);
        roles.push_back(useVertexRoles);
        usedTets.insert(tet);
    }

    // We've found a spiralled solid torus.
    SpiralSolidTorus* ans = new SpiralSolidTorus(tets.size());
    copy(tets.begin(), tets.end(), ans->tet);
    copy(roles.begin(), roles.end(), ans->vertexRoles_);
    return ans;
}

Manifold* SpiralSolidTorus::manifold() const {
    return new Handlebody(1, true);
}

AbelianGroup* SpiralSolidTorus::homology() const {
    AbelianGroup* ans = new AbelianGroup();
    ans->addRank();
    return ans;
}

} // namespace regina

