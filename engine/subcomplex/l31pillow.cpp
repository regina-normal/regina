
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
#include "manifold/lensspace.h"
#include "subcomplex/l31pillow.h"
#include "triangulation/dim3.h"

namespace regina {

std::unique_ptr<L31Pillow> L31Pillow::recognise(const Component<3>* comp) {
    // Basic property check.
    if (comp->size() != 2 ||
            comp->countVertices() != 2 ||
            comp->countEdges() != 4 ||
            (! comp->isClosed()) ||
            (! comp->isOrientable()))
        return nullptr;

    // Verify that the vertices have degrees 2 and 6.
    int internalVertex;
    unsigned long deg0 = comp->vertex(0)->degree();
    if (deg0 == 2)
        internalVertex = 0;
    else if (deg0 == 6)
        internalVertex = 1;
    else
        return nullptr;

    // Verify that all four faces of one tetrahedron join to the other.
    Tetrahedron<3>* tet[2];
    tet[0] = comp->tetrahedron(0);
    tet[1] = comp->tetrahedron(1);

    if (tet[0]->adjacentTetrahedron(0) != tet[1] ||
            tet[0]->adjacentTetrahedron(1) != tet[1] ||
            tet[0]->adjacentTetrahedron(2) != tet[1] ||
            tet[0]->adjacentTetrahedron(3) != tet[1])
        return nullptr;

    // At this point we can prove through enumeration of all
    // 2-tetrahedron triangulations that we have our triangular pillow
    // L(3,1).
    std::unique_ptr<L31Pillow> ans(new L31Pillow());
    ans->tet_[0] = tet[0];
    ans->tet_[1] = tet[1];

    for (int i = 0; i < 2; i++) {
        const auto& emb = comp->vertex(internalVertex)->embedding(i);
        if (emb.tetrahedron() == tet[0])
            ans->interior_[0] = emb.vertex();
        else
            ans->interior_[1] = emb.vertex();
    }

    return ans;
}

std::unique_ptr<Manifold> L31Pillow::manifold() const {
    return std::make_unique<LensSpace>(3, 1);
}

AbelianGroup L31Pillow::homology() const {
    return AbelianGroup(0, {3});
}

} // namespace regina

