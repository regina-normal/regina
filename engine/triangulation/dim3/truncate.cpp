
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
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

#include <vector>

#include "triangulation/dim3.h"

namespace regina {

bool Triangulation<3>::idealToFinite() {
    // The call to isIdeal() ensures the skeleton has been calculated.
    if (! isIdeal()) {
        // Note: this test also picks up the empty triangulation.
        if (isValid())
            return false; // Nothing to do.
        else {
            // We know all vertex links are 2-spheres or have boundary.
            // Only subdivide if there are invalid _vertices_; that is,
            // vertex links that have boundary but are not discs.
            // In particular, invalid edges are not something we care about.
            bool subdivide = false;
            for (auto v : vertices())
                if (! v->isValid()) {
                    subdivide = true;
                    break;
                }
            if (! subdivide)
                return false;
        }
    }

    size_t numOldTet = simplices_.size();

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
    // removeSimplexRaw() with no surrounding ChangeAndClearSpan.
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

} // namespace regina
