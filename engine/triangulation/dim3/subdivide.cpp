
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include <set>

#include "triangulation/dim3.h"

namespace regina {

void Triangulation<3>::puncture(Triangle<3>* location) {
    // If no triangle is passed, then we avoid ever having to compute the
    // skeleton (since the skeleton will be destroyed after this operation
    // anyway).  Therefore we keep the location of the puncture as a
    // (tetrahedron, facet) pair:
    Tetrahedron<3>* tet;
    int facet;

    if (location) {
        if (std::addressof(location->triangulation()) != this)
            throw InvalidArgument("puncture(): the given location is not "
                "within this triangulation");
        const auto& emb = location->front();
        tet = emb.tetrahedron();
        facet = emb.triangle();
    } else {
        if (simplices_.empty())
            throw InvalidArgument("puncture(): the triangulation is empty");
        // The default location:
        tet = simplices_.front();
        facet = 0;
    }

    // Is there a lock that we need to preserve?
    bool lock = tet->isFacetLocked(facet);

    // Note: we use the "raw" routines (joinRaw, newSimplexRaw, etc.),
    // mainly since we want to manage facet locks manually.  This means that
    // the ChangeAndClearSpan here is vital.
    ChangeAndClearSpan<> span(*this);

    // We will attach a pair of triangular prisms to the given facet of tet.
    // We will join the rectangular walls of the prisms together, and
    // one triangular end from each will join to form the new S^2 boundary.
    Tetrahedron<3>* prism[2][3];

    // Create the new tetrahedra in an order that ensures that the new
    // S^2 boundary will appear in the final two tetrahedra.
    int i, j;
    for (j = 0; j < 3; ++j)
        for (i = 0; i < 2; ++i)
            prism[i][j] = newSimplexRaw();

    prism[0][0]->joinRaw(0, prism[0][1], {3,0,1,2});
    prism[0][1]->joinRaw(0, prism[0][2], {3,0,1,2});

    prism[1][0]->joinRaw(1, prism[1][1], {3,0,1,2});
    prism[1][1]->joinRaw(1, prism[1][2], {3,2,0,1});

    prism[0][0]->joinRaw(1, prism[1][0], {1,2,3,0});
    prism[0][0]->joinRaw(2, prism[1][0], {1,2,3,0});
    prism[0][1]->joinRaw(1, prism[1][1], {1,2,3,0});
    prism[0][1]->joinRaw(2, prism[1][1], {1,2,3,0});
    prism[0][2]->joinRaw(1, prism[1][2], {0,1,3,2});
    prism[0][2]->joinRaw(2, prism[1][2], {0,1,3,2});

    // We need an even permutation that maps 0 -> facet.
    // We will choose this to be self-inverse also.
    Perm<4> swap;
    switch (facet) {
        case 1:
            swap = Perm<4>(1,0,3,2); break;
        case 2:
            swap = Perm<4>(2,3,0,1); break;
        case 3:
            swap = Perm<4>(3,2,1,0); break;
        default:
            // If facet == 0 then the default (identity) permutation is fine.
            break;
    }

    Tetrahedron<3>* adj = tet->adjacentTetrahedron(facet);
    if (adj) {
        Perm<4> gluing = tet->adjacentGluing(facet);
        tet->unjoinRaw(facet);
        prism[1][0]->joinRaw(0, adj, gluing * swap /* 0 -> facet */);
    }

    tet->joinRaw(facet, prism[0][0], Perm<4>(3,0,1,2) * swap /* facet -> 0 */);

    // Move the triangle lock, if there was one.
    // If adj is non-null, its lock is already in place; we just need to
    // fix tet (move the lock from tet:facet to the far side of the prism).
    // If adj is null (so the triangle is boundary), this same code will push
    // the lock out to the new boundary triangle as expected.
    if (lock) {
        tet->unlockFacetRaw(facet);
        prism[1][0]->lockFacetRaw(0);
    }
}

void Triangulation<3>::connectedSumWith(const Triangulation<3>& other) {
    if (other.simplices_.empty())
        return;
    if (simplices_.empty()) {
        insertTriangulation(other);
        return;
    }

    // From here we can assume that each triangulation contains at least
    // one tetrahedron.

    // Note: This PacketChangeSpan is essential, since we use "raw" routines
    // (joinRaw, etc.) further down below - this is so we can manage facet
    // locks manually.  A basic PacketChangeSpan is enough: we do not need to
    // take a snapshot or clear properties, since (a) that will be managed
    // already by insertTriangulation() and puncture(), and (b) we will not
    // compute any fresh properties that need clearing after that.
    PacketChangeSpan span(*this);

    // Insert the other triangulation *before* puncturing this, so that
    // things work in the case where we sum a triangulation with itself.
    size_t n = simplices_.size();
    insertTriangulation(other);

    // Make the puncture and record the resulting new boundary triangles.
    // Note: the default location for puncture() is tetrahedron 0, facet 0.
    // which means the puncture comes from the original (this) triangulation,
    // not the inserted copy of other.
    puncture();
    Tetrahedron<3>* bdry[2] = {
        simplices_[simplices_.size() - 2],
        simplices_[simplices_.size() - 1]
    };

    // Pop open a triangle in the second triangulation, and join the
    // two resulting boundary triangles to the boundary from the puncture.
    //
    // Even if the triangle we picked is a boundary triangle (i.e., has
    // degree 1, not degree 2), the overall effect remains correct.

    // We will pop open facet 0 of tetrahedron 0 from the second triangulation:
    Tetrahedron<3>* openTet = simplices_[n];
    bool lock = openTet->isFacetLocked(0);

    // We choose the gluing permutations so that, if both triangulations
    // are oriented, the connected sum respects this orientation.
    if (Tetrahedron<3>* adj = openTet->adjacentTetrahedron(0)) {
        Perm<4> gluing = openTet->adjacentGluing(0);

        openTet->unjoinRaw(0);
        bdry[0]->joinRaw(0, openTet, {0,3,2,1});
        bdry[1]->joinRaw(0, adj, gluing * Perm<4>(0,3,1,2));
    } else {
        bdry[0]->joinRaw(0, openTet, {0,3,2,1});
    }

    if (lock) {
        // Push the lock to the other side of what openTet was originally
        // glued to.  If adj exists, the lock on its side is already in place.
        // If adj does not exist, this will move the lock to the boundary.
        openTet->unlockFacetRaw(0);
        bdry[1]->lockFacetRaw(0);
    }
}

} // namespace regina
