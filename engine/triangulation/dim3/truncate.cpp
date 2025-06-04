
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

#include "triangulation/dim3.h"

namespace regina {

namespace {
    /**
     * A collection of 32 "inner" tetrahedra that together subdivide a single
     * "outer" tetrahedron, in a way that allows us to truncate vertices of
     * the outer tetrahedron by removing the "tips" of this subdivision.
     */
    struct SubTet {
        bool keepTip[4] = { true, true, true, true };
            /**< Indicates whether vertex i of the outer tetrahedron should be
                 kept (as opposed to being truncated). */

        Tetrahedron<3>* tip[4];
            /**< tip[i] sits between vertex i of the outer tetrahedron and a
                 small triangle that truncates vertex i.  If keepTip[i] is
                 false then the inner tetrahedron tip[i] will not be created. */
        Tetrahedron<3>* interior[4];
            /**< interior[i] sits between tip[i] and the centroid of the outer
                 tetrahedron. */
        Tetrahedron<3>* edge[4][4];
            /**< edge[i][j] is one of the six inner tetrahedra that has an
                 edge running from the centroid of the outer tetrahedron to
                 the centroid of face i of the outer tetrahedron.  It also
                 runs along a section of the edge of face i that does not meet
                 vertex j of the outer tetrahedron.  Requires i != j. */
        Tetrahedron<3>* vertex[4][4];
            /**< vertex[i][j] is one of the six inner tetrahedra that has an
                 edge running from the centroid of the outer tetrahedron to
                 the centroid of face i of the outer tetrahedron.  It is also
                 adjacent to interior[j].  Requires i != j. */

        /**
         * Creates all of the inner tetrahedra, adds them to the given
         * triangulation, and glues them together so that they completely
         * triangulate a single outer tetrahedron.
         */
        inline void build(Triangulation<3>& tri) {
            // Create the inner tetrahedra.
            for (int i = 0; i < 4; ++i) {
                if (keepTip[i])
                    tip[i] = tri.newSimplex();
                interior[i] = tri.newSimplex();

                for (int j = 0; j < 4; ++j)
                    if (i != j) {
                        edge[i][j] = tri.newSimplex();
                        vertex[i][j] = tri.newSimplex();
                    }
            }

            // Glue the inner tetrahedra together.
            for (int i = 0; i < 4; ++i)
                if (keepTip[i])
                    tip[i]->join(i, interior[i], {});

            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    if (i != j)
                        interior[i]->join(j, vertex[j][i], {});

            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j) {
                    if (i == j)
                        continue;
                    if (i < j)
                        edge[i][j]->join(i, edge[j][i], { i, j });
                    for (int k = 0; k < 4; ++k)
                        if (k != i && k != j)
                            edge[i][j]->join(k, vertex[i][k], { j, k });
                }
        }
    };
}

bool Triangulation<3>::truncateInternal(Vertex<3>* vertex) {
    // subTet manages the subdivision of each original (outer) tetrahedron
    // into many smaller (inner) tetrahedra.  It also records which of these
    // inner tetrahedra should _not_ be created because the corresponding
    // vertices of the outer tetrahedra are being truncated.
    FixedArray<SubTet> subTet(simplices_.size());

    // Begin by working out which vertices need to be truncated.
    if (vertex) {
        for (auto& emb : *vertex)
            subTet[emb.simplex()->index()].keepTip[emb.face()] = false;
    } else {
        // The call to vertices() ensures the skeleton has been calculated.
        bool found = false;
        for (auto v : vertices())
            // We know all vertex links are 2-spheres or have boundary.
            // Only subdivide if there are invalid _vertices_; that is,
            // vertex links that have boundary but are not discs.
            // In particular, invalid edges are not something we care about.
            if (v->isIdeal() || ! v->isValid()) {
                found = true;
                for (auto& emb : *v)
                    subTet[emb.simplex()->index()].keepTip[emb.face()] = false;
            }

        // If are no vertices to truncate, then there is nothing to do.
        if (! found)
            return false;
    }

    // We need to subdivide and truncate.
    // Any simplex or facet locks at all will be a problem.
    if (hasLocks())
        throw LockViolation("An attempt was made to subdivide a "
            "triangulation with one or more locked tetrahedra or triangles");

    // Since staging is a new triangulation here, we will (where possible) use
    // "raw" simplex gluing routines that do not generate change events and
    // snapshots, check locks, and so on.
    Triangulation<3> staging;

    // Go ahead and create the inner tetrahedra that subdivide each outer
    // tetrahedron, and glue them together within each outer tetrahedron.
    // The truncation happens at this point, since the "tip" tetrahedra around
    // each truncated vertex will not be created.
    for (auto& sub : subTet)
        sub.build(staging);

    // Glue the inner tetrahedra where necessary across the facet gluings of
    // the outer tetrahedra.
    for (size_t index = 0; index < simplices_.size(); ++index) {
        auto outer = simplices_[index];
        for (int f = 0; f < 4; ++f)
            if (auto adj = outer->adjacentSimplex(f)) {
                size_t adjIndex = adj->index();
                Perm<4> g = outer->adjacentGluing(f);

                // Do each gluing from one side only.
                if (adjIndex < index || (adjIndex == index && g[f] < f))
                    continue;

                for (int k = 0; k < 4; ++k)
                    if (k != f) {
                        if (subTet[index].keepTip[k])
                            subTet[index].tip[k]->joinRaw(f,
                                subTet[adjIndex].tip[g[k]], g);
                        subTet[index].edge[f][k]->joinRaw(k,
                            subTet[adjIndex].edge[g[f]][g[k]], g);
                        subTet[index].vertex[f][k]->joinRaw(k,
                            subTet[adjIndex].vertex[g[f]][g[k]], g);
                    }
            }
    }

    // We are now ready to move everything into the main triangulation.
    // This is where the change event and snapshot will be fired.
    swap(staging);
    return true;
}

} // namespace regina
