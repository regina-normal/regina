
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
#include "triangulation/dim4.h"
#include <sstream>

namespace regina {

Face<4, 1>::~Face() {
    // Deleting null is always safe.
    delete link_;
}

const Triangulation<2>& Face<4, 1>::buildLink() const {
    if (! link_) {
        // Build the triangulation.
        auto* ans = new Triangulation<2>();
        ans->newTriangles(degree());

        size_t i = 0;
        for (auto it = begin(); it != end(); ++it, ++i) {
            Pentachoron<4>* pent = it->pentachoron();
            int e = it->edge();

            for (int exitTet = 0; exitTet < 5; ++exitTet) {
                if (exitTet == edgeVertex[e][0] || exitTet == edgeVertex[e][1])
                    continue;

                Pentachoron<4>* adj = pent->adjacentPentachoron(exitTet);
                if (! adj)
                    continue;

                int edgeInLink = pent->triangleMapping(e).pre(exitTet);
                if (ans->triangle(i)->adjacentTriangle(edgeInLink)) {
                    // We've already made this gluing in the vertex link
                    // from the other side.
                    continue;
                }

                Perm<5> adjGluing = pent->adjacentGluing(exitTet);
                int adjEdge = edgeNumber[adjGluing[edgeVertex[e][0]]]
                                        [adjGluing[edgeVertex[e][1]]];

                // TODO: We need to find which *embedding* corresponds to
                // the adjacent pentachoron/edge pair.
                // Currently we do a simple linear scan, which makes the
                // overall link construction quadratic.  This can surely be
                // made linear(ish) with the right data structure / algorithm.
                size_t adjIndex = 0;
                for (auto adjIt = begin(); adjIt != end(); ++adjIt, ++adjIndex)
                    if (adjIt->pentachoron() == adj && adjIt->edge() == adjEdge)
                        break; // Sets adjIndex to the right value.

                ans->triangle(i)->join(edgeInLink, ans->triangle(adjIndex),
                    Perm<3>::contract(adj->triangleMapping(adjEdge).inverse() *
                        adjGluing *
                        pent->triangleMapping(e)));
            }
        }

        // This is a construct-on-demand member; cast away constness to
        // set it here.
        const_cast<Edge<4>*>(this)->link_ = ans;
    }
    return *link_;
}

Isomorphism<4> Face<4, 1>::buildLinkInclusion() const {
    Isomorphism<4> inclusion(degree());

    size_t i = 0;
    for (auto it = begin(); it != end(); ++it, ++i) {
        inclusion.pentImage(i) = it->pentachoron()->index();

        Perm<5> perm = it->pentachoron()->triangleMapping(it->edge());
        if (perm[3] == it->vertices()[0])
            inclusion.facetPerm(i) = perm;
        else
            inclusion.facetPerm(i) = perm * Perm<5>(3, 4);
    }

    return inclusion;
}

} // namespace regina

