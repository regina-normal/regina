
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

#include <set>
#include <vector>
#include "algebra/abeliangroup.h"
#include "manifold/handlebody.h"
#include "subcomplex/spiralsolidtorus.h"
#include "triangulation/dim3.h"

namespace regina {

SpiralSolidTorus& SpiralSolidTorus::operator = (const SpiralSolidTorus& src) {
    // std::copy() exhibits undefined behaviour in the case of self-assignment.
    if (std::addressof(src) == this)
        return *this;

    if (nTet_ != src.nTet_) {
        delete[] tet_;
        delete[] vertexRoles_;
        nTet_ = src.nTet_;
        tet_ = new Tetrahedron<3>*[nTet_];
        vertexRoles_ = new Perm<4>[nTet_];
    }
    std::copy(src.tet_, src.tet_ + nTet_, tet_);
    std::copy(src.vertexRoles_, src.vertexRoles_ + nTet_, vertexRoles_);
    return *this;
}

void SpiralSolidTorus::reverse() {
    auto* newTet = new Tetrahedron<3>*[nTet_];
    auto* newRoles = new Perm<4>[nTet_];

    Perm<4> switchPerm(3, 2, 1, 0);
    for (size_t i = 0; i < nTet_; i++) {
        newTet[i] = tet_[nTet_ - 1 - i];
        newRoles[i] = vertexRoles_[nTet_ - 1 - i] * switchPerm;
    }

    delete[] tet_;
    delete[] vertexRoles_;
    tet_ = newTet;
    vertexRoles_ = newRoles;
}

void SpiralSolidTorus::cycle(size_t k) {
    auto* newTet = new Tetrahedron<3>*[nTet_];
    auto* newRoles = new Perm<4>[nTet_];

    for (size_t i = 0; i < nTet_; i++) {
        newTet[i] = tet_[(i + k) % nTet_];
        newRoles[i] = vertexRoles_[(i + k) % nTet_];
    }

    delete[] tet_;
    delete[] vertexRoles_;
    tet_ = newTet;
    vertexRoles_ = newRoles;
}

bool SpiralSolidTorus::makeCanonical() {
    size_t i, index;

    size_t baseTet = 0;
    size_t baseIndex = tet_[0]->index();
    for (i = 1; i < nTet_; i++) {
        index = tet_[i]->index();
        if (index < baseIndex) {
            baseIndex = index;
            baseTet = i;
        }
    }

    bool reverseAlso = (vertexRoles_[baseTet][0] > vertexRoles_[baseTet][3]);

    if (baseTet == 0 && (! reverseAlso))
        return false;

    auto* newTet = new Tetrahedron<3>*[nTet_];
    auto* newRoles = new Perm<4>[nTet_];

    if (reverseAlso) {
        // Make baseTet into tetrahedron 0 and reverse.
        Perm<4> switchPerm(3, 2, 1, 0);
        for (i = 0; i < nTet_; i++) {
            newTet[i] = tet_[(baseTet + nTet_ - i) % nTet_];
            newRoles[i] = vertexRoles_[(baseTet + nTet_ - i) % nTet_] *
                switchPerm;
        }
    } else {
        // Make baseTet into tetrahedron 0 but don't reverse.
        for (i = 0; i < nTet_; i++) {
            newTet[i] = tet_[(i + baseTet) % nTet_];
            newRoles[i] = vertexRoles_[(i + baseTet) % nTet_];
        }
    }

    delete[] tet_;
    delete[] vertexRoles_;
    tet_ = newTet;
    vertexRoles_ = newRoles;

    return true;
}

bool SpiralSolidTorus::isCanonical() const {
    if (vertexRoles_[0][0] > vertexRoles_[0][3])
        return false;

    size_t baseIndex = tet_[0]->index();
    for (size_t i = 1; i < nTet_; i++)
        if (tet_[i]->index() < baseIndex)
            return false;

    return true;
}

std::unique_ptr<SpiralSolidTorus> SpiralSolidTorus::recognise(
        Tetrahedron<3>* tet, Perm<4> useVertexRoles) {
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

    while (true) {
        // Examine the tetrahedron beyond tet.
        adjTet = tet->adjacentTetrahedron(useVertexRoles[0]);
        adjRoles = tet->adjacentGluing(useVertexRoles[0]) *
            useVertexRoles * invRoleMap;

        // Check that we haven't hit the boundary.
        if (! adjTet)
            return nullptr;

        if (adjTet == base) {
            // We're back at the beginning of the loop.
            // Check that everything is glued up correctly.
            if (adjRoles != baseRoles)
                return nullptr;

            // Success!
            break;
        }

        if (usedTets.count(adjTet))
            return nullptr;

        // Move on to the next tetrahedron.
        tet = adjTet;
        useVertexRoles = adjRoles;

        tets.push_back(tet);
        roles.push_back(useVertexRoles);
        usedTets.insert(tet);
    }

    // We've found a spiralled solid torus.
    std::unique_ptr<SpiralSolidTorus> ans(new SpiralSolidTorus(tets.size()));
    std::copy(tets.begin(), tets.end(), ans->tet_);
    std::copy(roles.begin(), roles.end(), ans->vertexRoles_);
    return ans;
}

std::unique_ptr<Manifold> SpiralSolidTorus::manifold() const {
    return std::make_unique<Handlebody>(1);
}

AbelianGroup SpiralSolidTorus::homology() const {
    return AbelianGroup(1);
}

void SpiralSolidTorus::writeTextShort(std::ostream& out) const {
    out << nTet_ << "-tetrahedron spiralled solid torus, tetrahedra ";
    for (size_t i = 0; i < nTet_; ++i) {
        if (i > 0)
            out << ", ";
        out << tet_[i]->index() << " (" << vertexRoles_[i] << ')';
    }
}

} // namespace regina

