
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2004, Ben Burton                                   *
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

#include <vector>
#include "algebra/nabeliangroup.h"
#include "manifold/nhandlebody.h"
#include "subcomplex/nspiralsolidtorus.h"
#include "triangulation/ntriangulation.h"

namespace regina {

NSpiralSolidTorus* NSpiralSolidTorus::clone() const {
    NSpiralSolidTorus* ans = new NSpiralSolidTorus(nTet);
    for (unsigned long i = 0; i < nTet; i++) {
        ans->tet[i] = tet[i];
        ans->vertexRoles[i] = vertexRoles[i];
    }
    return ans;
}

void NSpiralSolidTorus::reverse() {
    NTetrahedron** newTet = new NTetrahedron*[nTet];
    NPerm* newRoles = new NPerm[nTet];

    NPerm switchPerm(3, 2, 1, 0);
    for (unsigned long i = 0; i < nTet; i++) {
        newTet[i] = tet[nTet - 1 - i];
        newRoles[i] = vertexRoles[nTet - 1 - i] * switchPerm;
    }

    delete[] tet;
    delete[] vertexRoles;
    tet = newTet;
    vertexRoles = newRoles;
}

void NSpiralSolidTorus::cycle(unsigned long k) {
    NTetrahedron** newTet = new NTetrahedron*[nTet];
    NPerm* newRoles = new NPerm[nTet];

    for (unsigned long i = 0; i < nTet; i++) {
        newTet[i] = tet[(i + k) % nTet];
        newRoles[i] = vertexRoles[(i + k) % nTet];
    }

    delete[] tet;
    delete[] vertexRoles;
    tet = newTet;
    vertexRoles = newRoles;
}

bool NSpiralSolidTorus::makeCanonical(const NTriangulation* tri) {
    unsigned long i, index;

    unsigned long baseTet = 0;
    unsigned long baseIndex = tri->getTetrahedronIndex(tet[0]);
    for (i = 1; i < nTet; i++) {
        index = tri->getTetrahedronIndex(tet[i]);
        if (index < baseIndex) {
            baseIndex = index;
            baseTet = i;
        }
    }

    bool reverseAlso = (vertexRoles[baseTet][0] > vertexRoles[baseTet][3]);

    if (baseTet == 0 && (! reverseAlso))
        return false;

    NTetrahedron** newTet = new NTetrahedron*[nTet];
    NPerm* newRoles = new NPerm[nTet];

    if (reverseAlso) {
        // Make baseTet into tetrahedron 0 and reverse.
        NPerm switchPerm(3, 2, 1, 0);
        for (unsigned long i = 0; i < nTet; i++) {
            newTet[i] = tet[(baseTet + nTet - i) % nTet];
            newRoles[i] = vertexRoles[(baseTet + nTet - i) % nTet] *
                switchPerm;
        }
    } else {
        // Make baseTet into tetrahedron 0 but don't reverse.
        for (unsigned long i = 0; i < nTet; i++) {
            newTet[i] = tet[(i + baseTet) % nTet];
            newRoles[i] = vertexRoles[(i + baseTet) % nTet];
        }
    }

    delete[] tet;
    delete[] vertexRoles;
    tet = newTet;
    vertexRoles = newRoles;

    return true;
}

bool NSpiralSolidTorus::isCanonical(const NTriangulation* tri) const {
    if (vertexRoles[0][0] > vertexRoles[0][3])
        return false;

    unsigned long baseIndex = tri->getTetrahedronIndex(tet[0]);
    for (unsigned long i = 1; i < nTet; i++)
        if (tri->getTetrahedronIndex(tet[i]) < baseIndex)
            return false;

    return true;
}

NSpiralSolidTorus* NSpiralSolidTorus::formsSpiralSolidTorus(NTetrahedron* tet,
        NPerm useVertexRoles) {
    NPerm invRoleMap(1, 2, 3, 0);  // Maps upper roles to lower roles.

    NTetrahedron* base = tet;
    NPerm baseRoles(useVertexRoles);

    std::vector<NTetrahedron*> tets;
    std::vector<NPerm> roles;
    stdhash::hash_set<NTetrahedron*, HashPointer> usedTets;

    tets.push_back(tet);
    roles.push_back(useVertexRoles);
    usedTets.insert(tet);

    NTetrahedron* adjTet;
    NPerm adjRoles;

    while (1) {
        // Examine the tetrahedron beyond tet.
        adjTet = tet->getAdjacentTetrahedron(useVertexRoles[0]);
        adjRoles = tet->getAdjacentTetrahedronGluing(useVertexRoles[0]) *
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
    NSpiralSolidTorus* ans = new NSpiralSolidTorus(tets.size());
    copy(tets.begin(), tets.end(), ans->tet);
    copy(roles.begin(), roles.end(), ans->vertexRoles);
    return ans;
}

NManifold* NSpiralSolidTorus::getManifold() const {
    return new NHandlebody(1, true);
}

NAbelianGroup* NSpiralSolidTorus::getHomologyH1() const {
    NAbelianGroup* ans = new NAbelianGroup();
    ans->addRank();
    return ans;
}

} // namespace regina

