
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

#include "triangulation/dim3.h"
#include "triangulation/dim4.h"
#include <sstream>

namespace regina {

Face<4, 0>::~Face() {
    // Deleting null is always safe.
    delete link_;
}

void Face<4, 0>::writeTextShort(std::ostream& out) const {
    if (! link_->isClosed())
        out << "Boundary ";
    else if (link_->isThreeSphere())
        out << "Internal ";
    else
        out << "Ideal ";

    out << "vertex of degree " << degree();
}

Triangulation<3>* Face<4, 0>::buildLinkDetail(bool labels,
        Isomorphism<4>** inclusion) const {
    // Build the triangulation.
    Triangulation<3>* ans = new Triangulation<3>();
    Packet::ChangeEventSpan span(ans);

    if (inclusion)
        *inclusion = new Isomorphism<4>(degree());

    std::vector<VertexEmbedding<4>>::const_iterator it, adjIt;
    Tetrahedron<3>* tTet;
    int i;
    for (it = begin(), i = 0; it != end(); ++it, ++i) {
        tTet = ans->newTetrahedron();
        if (labels) {
            std::stringstream s;
            s << it->pentachoron()->markedIndex() <<
                " (" << it->vertex() << ')';
            tTet->setDescription(s.str());
        }
        if (inclusion) {
            (*inclusion)->pentImage(i) = it->pentachoron()->markedIndex();
            (*inclusion)->facetPerm(i) = it->pentachoron()->
                tetrahedronMapping(it->vertex());
        }
    }

    Pentachoron<4> *pent, *adj;
    int exitTet, v;
    int faceInLink;
    int adjIndex;
    int adjVertex;
    for (it = begin(), i = 0; it != end(); ++it, ++i) {
        pent = it->pentachoron();
        v = it->vertex();

        for (exitTet = 0; exitTet < 5; ++exitTet) {
            if (exitTet == v)
                continue;

            adj = pent->adjacentPentachoron(exitTet);
            if (! adj)
                continue;

            faceInLink = pent->tetrahedronMapping(v).preImageOf(exitTet);
            if (ans->tetrahedron(i)->adjacentTetrahedron(faceInLink)) {
                // We've already made this gluing in the vertex link
                // from the other side.
                continue;
            }

            adjVertex = pent->adjacentGluing(exitTet)[v];

            // TODO: We need to find which *embedding* corresponds to
            // the adjacent pentachoron/vertex pair.
            // Currently we do a simple linear scan, which makes the
            // overall link construction quadratic.  This can surely be
            // made linear(ish) with the right data structure and/or algorithm.
            for (adjIt = begin(), adjIndex = 0;
                    adjIt != end(); ++adjIt, ++adjIndex)
                if (adjIt->pentachoron() == adj && adjIt->vertex() == adjVertex)
                    break; // Sets adjIndex to the right value.

            ans->tetrahedron(i)->join(faceInLink,
                ans->tetrahedron(adjIndex),
                Perm<4>::contract(adj->tetrahedronMapping(adjVertex).inverse() *
                    pent->adjacentGluing(exitTet) *
                    pent->tetrahedronMapping(v)));
        }
    }

    return ans;
}

} // namespace regina
