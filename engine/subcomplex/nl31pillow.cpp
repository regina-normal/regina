
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

#include "algebra/nabeliangroup.h"
#include "manifold/nlensspace.h"
#include "subcomplex/nl31pillow.h"
#include "triangulation/dim3.h"

namespace regina {

NL31Pillow* NL31Pillow::clone() const {
    NL31Pillow* ans = new NL31Pillow();
    ans->tet[0] = tet[0];
    ans->tet[1] = tet[1];
    ans->interior[0] = interior[0];
    ans->interior[1] = interior[1];
    return ans;
}

NL31Pillow* NL31Pillow::isL31Pillow(const NComponent* comp) {
    // Basic property check.
    if (comp->size() != 2 ||
            comp->countVertices() != 2 ||
            comp->countEdges() != 4 ||
            (! comp->isClosed()) ||
            (! comp->isOrientable()))
        return 0;

    // Verify that the vertices have degrees 2 and 6.
    int internalVertex;
    unsigned long deg0 = comp->vertex(0)->degree();
    if (deg0 == 2)
        internalVertex = 0;
    else if (deg0 == 6)
        internalVertex = 1;
    else
        return 0;

    // Verify that all four faces of one tetrahedron join to the other.
    NTetrahedron* tet[2];
    tet[0] = comp->tetrahedron(0);
    tet[1] = comp->tetrahedron(1);

    if (tet[0]->adjacentTetrahedron(0) != tet[1] ||
            tet[0]->adjacentTetrahedron(1) != tet[1] ||
            tet[0]->adjacentTetrahedron(2) != tet[1] ||
            tet[0]->adjacentTetrahedron(3) != tet[1])
        return 0;

    // At this point we can prove through enumeration of all
    // 2-tetrahedron triangulations that we have our triangular pillow
    // L(3,1).
    NL31Pillow* ans = new NL31Pillow();
    ans->tet[0] = tet[0];
    ans->tet[1] = tet[1];

    for (int i = 0; i < 2; i++) {
        const VertexEmbedding<3>& emb = comp->vertex(internalVertex)->
            embedding(i);
        if (emb.tetrahedron() == tet[0])
            ans->interior[0] = emb.vertex();
        else
            ans->interior[1] = emb.vertex();
    }

    return ans;
}

NManifold* NL31Pillow::manifold() const {
    return new NLensSpace(3, 1);
}

NAbelianGroup* NL31Pillow::homology() const {
    NAbelianGroup* ans = new NAbelianGroup();
    ans->addTorsionElement(3);
    return ans;
}

} // namespace regina

