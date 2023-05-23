
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

#include <set>
#include <vector>

#include "triangulation/dim3.h"

namespace regina {

bool Triangulation<3>::idealToFinite() {
    // The call to isValid() ensures the skeleton has been calculated.
    if (isValid() && ! isIdeal())
        return false;

    size_t numOldTet = simplices_.size();
    if (! numOldTet)
        return false;

    // Any simplex or facet locks at all will be a problem here.
    if (hasLocks())
        throw LockViolation("An attempt was made to subdivide a "
            "triangulation with one or more locked tetrahedra or triangles");

    // Since staging is new here, we will use the "raw" simplex routines
    // that do not generate change events / snapshots, check locks, etc.
    Triangulation<3> staging;

    auto* newTet = new Tetrahedron<3>*[32*numOldTet];
    for (size_t i=0; i<32*numOldTet; i++)
        newTet[i] = staging.newSimplexRaw();

    int tip[4];
    int interior[4];
    int edge[4][4];
    int vertex[4][4];

    int nDiv = 0;
    for (int j=0; j<4; j++) {
        tip[j] = nDiv++;
        interior[j] = nDiv++;

        for (int k=0; k<4; k++)
            if (j != k) {
                edge[j][k] = nDiv++;
                vertex[j][k] = nDiv++;
            }
    }

    // First glue all of the tetrahedra inside the same
    // old tetrahedron together.
    for (size_t i=0; i<numOldTet; i++) {
        // Glue the tip tetrahedra to the others.
        for (int j=0; j<4; j++)
            newTet[tip[j] + i * nDiv]->joinRaw(j,
                newTet[interior[j] + i * nDiv], Perm<4>());

        // Glue the interior tetrahedra to the others.
        for (int j=0; j<4; j++) {
            for (int k=0; k<4; k++)
                if (j != k) {
                    newTet[interior[j] + i * nDiv]->joinRaw(k,
                        newTet[vertex[k][j] + i * nDiv], Perm<4>());
                }
        }

        // Glue the edge tetrahedra to the others.
        for (int j=0; j<4; j++)
            for (int k=0; k<4; k++)
                if (j != k) {
                    if (j < k)
                        newTet[edge[j][k] + i * nDiv]->joinRaw(j,
                            newTet[edge[k][j] + i * nDiv], Perm<4>(j,k));

                    for (int l=0; l<4; l++)
                        if ( (l != j) && (l != k) )
                            newTet[edge[j][k] + i * nDiv]->joinRaw(l,
                                newTet[vertex[j][l] + i * nDiv], Perm<4>(k,l));
                }
    }

    // Now deal with the gluings between the pieces inside adjacent tetrahedra.
    for (size_t i=0; i<numOldTet; i++) {
        Tetrahedron<3>* ot = tetrahedron(i);
        for (int j=0; j<4; j++)
            if (ot->adjacentTetrahedron(j)) {
                 size_t oppTet = ot->adjacentTetrahedron(j)->index();
                 Perm<4> p = ot->adjacentGluing(j);

                 // Do each gluing from one side only.
                 if (oppTet < i || (oppTet == i && p[j] < j))
                    continue;

                 // First deal with the tip tetrahedra.
                 for (int k=0; k<4; k++)
                     if (j != k)
                          newTet[tip[k] + i * nDiv]->joinRaw(j,
                              newTet[tip[p[k]] + oppTet * nDiv], p);

                 // Next the edge tetrahedra.
                 for (int k=0; k<4; k++)
                     if (j != k)
                         newTet[edge[j][k] + i * nDiv]->joinRaw(k,
                             newTet[edge[p[j]][p[k]] + oppTet * nDiv], p);

                 // Finally, the vertex tetrahedra.
                 for (int k=0; k<4; k++)
                     if (j != k)
                         newTet[vertex[j][k] + i * nDiv]->joinRaw(k,
                             newTet[vertex[p[j]][p[k]] + oppTet * nDiv], p);

            }
    }

    delete[] newTet;

    // Now remove any new tetrahedra that touch an ideal or invalid vertex.
    // We do this by making a list first, then actually doing the deletion
    // (since the first deletion will destroy the skeleton).

    staging.ensureSkeleton();

    std::vector<Tetrahedron<3>*> tetList;
    for (Vertex<3>* v : staging.vertices())
        if (v->isIdeal() || ! v->isValid())
            for (auto& emb : *v)
                tetList.push_back(emb.tetrahedron());

    // Just above, we computed the skeleton for staging so we could query
    // its vertices.  We need to delete this computed property now, since
    // we are about to edit the staging triangulation further using
    // removeSimplexRaw() with no surrounding ChangeEventSpan.
    // This means the skeleton will become incorrect, and we do not want
    // this incorrect skeleton to be moved into this triangulation as
    // part of the final swap().
    staging.clearAllProperties();

    for (auto t : tetList)
        staging.removeSimplexRaw(t);

    // We are now ready to change the main triangulation.
    // This is where the change event and snapshot will be fired.
    swap(staging);
    return true;
}

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

    // Note: This ChangeEventSpan is essential, since we use "raw" routines
    // (joinRaw, etc.) further down below - this is so we can manage facet
    // locks manually.  A ChangeEventSpan is enough: we do not need to take a
    // snapshot or clear properties, since (a) that will be managed already
    // by insertTriangulation() and puncture(), and (b) we will not compute
    // any fresh properties that need clearing after that.
    ChangeEventSpan span(*this);

    // Insert the other triangulation *before* puncturing this, so that
    // things work in the case where we sum a triangulation with itself.
    unsigned long n = simplices_.size();
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
