
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

#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include <sstream>

namespace regina {

Vertex<3>::~Face() {
    delete linkTri_;
}

void Vertex<3>::writeTextShort(std::ostream& out) const {
    switch(link_) {
        case SPHERE: out << "Internal "; break;
        case DISC: out << "Boundary "; break;
        case TORUS: out << "Torus cusp "; break;
        case KLEIN_BOTTLE: out << "Klein bottle cusp "; break;
        case NON_STANDARD_CUSP: out << "Non-standard cusp "; break;
        case INVALID: out << "Invalid boundary "; break;
    }
    out << "vertex of degree " << degree();
}

const Triangulation<2>& Face<3, 0>::buildLink() const {
    if (! linkTri_) {
        // Build the triangulation.
        auto* ans = new Triangulation<2>();
        // Ensure only one event pair is fired in this sequence of changes.
        Triangulation<2>::ChangeEventSpan span(*ans);

        ans->newTriangles(degree());

        size_t i = 0;
        for (auto it = begin(); it != end(); ++it, ++i) {
            Tetrahedron<3>* tet = it->tetrahedron();
            int v = it->vertex();

            for (int exitTri = 0; exitTri < 4; ++exitTri) {
                if (exitTri == v)
                    continue;

                Tetrahedron<3>* adj = tet->adjacentTetrahedron(exitTri);
                if (! adj)
                    continue;

                int edgeInLink = tet->triangleMapping(v).pre(exitTri);
                if (ans->triangle(i)->adjacentTriangle(edgeInLink)) {
                    // We've already made this gluing in the vertex link
                    // from the other side.
                    continue;
                }

                int adjVertex = tet->adjacentGluing(exitTri)[v];

                // TODO: We need to find which *embedding* corresponds to
                // the adjacent tetrahedron/vertex pair.
                // Currently we do a simple linear scan, which makes the
                // overall link construction quadratic.  This can surely be
                // made linear(ish) with the right data structure / algorithm.
                size_t adjIndex = 0;
                for (auto adjIt = begin(); adjIt != end(); ++adjIt, ++adjIndex)
                    if (adjIt->tetrahedron() == adj &&
                            adjIt->vertex() == adjVertex)
                        break; // Sets adjIndex to the right value.

                ans->triangle(i)->join(edgeInLink, ans->triangle(adjIndex),
                    Perm<3>::contract(adj->triangleMapping(adjVertex).inverse() *
                        tet->adjacentGluing(exitTri) *
                        tet->triangleMapping(v)));
            }
        }

        // This is a construct-on-demand member: cast away constness to
        // set it here.
        const_cast<Vertex<3>*>(this)->linkTri_ = ans;
    }
    return *linkTri_;
}

Isomorphism<3> Face<3, 0>::buildLinkInclusion() const {
    Isomorphism<3> inclusion(degree());

    size_t i = 0;
    for (auto it = begin(); it != end(); ++it, ++i) {
        inclusion.tetImage(i) = it->tetrahedron()->index();
        inclusion.facetPerm(i) = it->tetrahedron()->triangleMapping(
            it->vertex());
    }

    return inclusion;
}

} // namespace regina
