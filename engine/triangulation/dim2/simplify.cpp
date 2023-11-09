
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

#include "triangulation/dim2.h"

namespace regina {

bool Triangulation<2>::twoZeroMove(Vertex<2>* v, bool check, bool perform) {
    using LockMask = Simplex<2>::LockMask;

    if (check) {
        if (v->isBoundary() || v->degree() != 2)
            return false;
    }

    Triangle<2>* simp[2];
    int vertex[2];
    bool lockExterior = false;

    int i = 0;
    for (auto& emb : *v) {
        simp[i] = emb.triangle();
        vertex[i] = emb.vertex();

        if (simp[i]->locks_) {
            // The only thing that can be locked is the exterior facet.
            if (simp[i]->locks_ != (LockMask(1) << vertex[i])) {
                if (check)
                    return false;
                if (perform)
                    throw LockViolation("An attempt was made to perform a "
                        "2-0 move using a locked triangle and/or facet");
            }
            // Remember that, when we perform the move, the two merged
            // exterior facets need to be locked from both sides.
            lockExterior = true;
        }

        i++;
    }

    if (check) {
        if (simp[0] == simp[1])
            return false;

        Edge<2>* edge[2];
        for (i = 0; i < 2; i++)
            edge[i] = simp[i]->edge(vertex[i]);
        if (edge[0] == edge[1])
            return false;
        if (edge[0]->isBoundary() && edge[1]->isBoundary())
            return false;
    }

    if (! perform)
        return true;

    // Actually perform the move.
    // The following ChangeAndClearSpan is essential, since we use
    // "raw" routines (newSimplexRaw, joinRaw, etc.) below.
    ChangeAndClearSpan<ChangeType::PreserveTopology> span(*this);

    // Unglue edges from the doomed triangles and glue them to each other.
    Triangle<2>* top = simp[0]->adjacentSimplex(vertex[0]);
    Triangle<2>* bottom = simp[1]->adjacentSimplex(vertex[1]);

    if (! top) {
        if (lockExterior)
            bottom->lockFacetRaw(simp[1]->adjacentFacet(vertex[1]));
        simp[1]->unjoinRaw(vertex[1]);
    } else if (! bottom) {
        if (lockExterior)
            top->lockFacetRaw(simp[0]->adjacentFacet(vertex[0]));
        simp[0]->unjoinRaw(vertex[0]);
    } else {
        int topFacet = simp[0]->adjacentFacet(vertex[0]);
        int bottomFacet = simp[1]->adjacentFacet(vertex[1]);
        if (lockExterior) {
            top->lockFacetRaw(topFacet);
            bottom->lockFacetRaw(bottomFacet);
        }
        Perm<3> crossover = simp[0]->adjacentGluing(vertex[0] == 0 ? 1 : 0);
        Perm<3> gluing = simp[1]->adjacentGluing(vertex[1]) *
            crossover * top->adjacentGluing(topFacet);
        simp[0]->unjoinRaw(vertex[0]);
        simp[1]->unjoinRaw(vertex[1]);
        top->joinRaw(topFacet, bottom, gluing);
    }

    // Finally remove and dispose of the triangles.
    removeSimplexRaw(simp[0]);
    removeSimplexRaw(simp[1]);

    return true;
}

} // namespace regina
