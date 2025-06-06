
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
#include "triangulation/dim4.h"
#include <sstream>

namespace regina {

Face<4, 0>::~Face() {
    // Deleting null is always safe.
    delete link_;
}

const Triangulation<3>& Face<4, 0>::buildLink() const {
    if (! link_) {
        // Note: we need to insert tetrahedra in the correct order, as
        // specified in the buildLink() documentation.

        // Build a map from (pentachoron, vertex) pairs to indices into
        // embeddings().
        FixedArray<size_t> map(5 * triangulation().size());
        {
            size_t i = 0;
            for (const auto& emb : embeddings())
                map[5 * emb.simplex()->index() + emb.face()] = i++;
        }

        // Now build the triangulation.
        auto* ans = new Triangulation<3>();
        ans->newTetrahedra(degree());
        for (const auto& emb : embeddings()) {
            // Glue this piece of vertex link to any adjacent pieces of
            // vertex link.
            size_t pentIdx = emb.simplex()->index();
            for (int f = 0; f < 5; ++f) {
                if (f == emb.face())
                    continue;

                auto adjPent = emb.simplex()->adjacentPentachoron(f);
                if (! adjPent)
                    continue;

                // Make sure we perform each gluing in one direction only.
                size_t adjPentIdx = adjPent->index();
                if (adjPentIdx > pentIdx)
                    continue;
                int adjFacet = emb.simplex()->adjacentFacet(f);
                if (adjPentIdx == pentIdx && adjFacet > f)
                    continue;

                // This tetrahedron is adjacent to a previously-seen
                // tetrahedron.  Make the gluing.
                int adjVertexIdx = emb.simplex()->adjacentGluing(f)[emb.face()];
                Perm<5> tetVertices =
                    emb.simplex()->tetrahedronMapping(emb.face());
                Perm<5> adjTetVertices =
                    adjPent->tetrahedronMapping(adjVertexIdx);
                ans->tetrahedron(map[5 * pentIdx + emb.face()])->join(
                    tetVertices.pre(f),
                    ans->tetrahedron(map[5 * adjPentIdx + adjVertexIdx]),
                    Perm<4>::contract(
                        adjTetVertices.inverse() *
                        emb.simplex()->adjacentGluing(f) *
                        tetVertices));
            }
        }

        // This is a construct-on-demand member; cast away constness to
        // set it here.
        const_cast<Vertex<4>*>(this)->link_ = ans;
    }
    return *link_;
}

Isomorphism<4> Face<4, 0>::buildLinkInclusion() const {
    Isomorphism<4> inclusion(degree());

    size_t i = 0;
    for (auto it = begin(); it != end(); ++it, ++i) {
        inclusion.pentImage(i) = it->pentachoron()->index();
        inclusion.facetPerm(i) = it->pentachoron()->tetrahedronMapping(
            it->vertex());
    }

    return inclusion;
}

} // namespace regina
