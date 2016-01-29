
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

#include "dim2/dim2triangulation.h"
#include "maths/permconv.h"
#include "triangulation/nisomorphism.h"
#include "triangulation/nvertex.h"
#include <sstream>

namespace regina {

NVertex::~Face() {
    delete linkTri_;
}

void NVertex::writeTextShort(std::ostream& out) const {
    switch(link_) {
        case SPHERE: out << "Internal "; break;
        case DISC: out << "Boundary "; break;
        case TORUS: out << "Torus cusp "; break;
        case KLEIN_BOTTLE: out << "Klein bottle cusp "; break;
        case NON_STANDARD_CUSP: out << "Non-standard cusp "; break;
        case NON_STANDARD_BDRY: out << "Non-standard boundary "; break;
    }
    out << "vertex of degree " << degree();
}

void NVertex::writeTextLong(std::ostream& out) const {
    writeTextShort(out);
    out << std::endl;

    out << "Appears as:" << std::endl;
    for (auto& emb : *this)
        out << "  " << emb << std::endl;
}

Dim2Triangulation* NVertex::buildLinkDetail(bool labels,
        NIsomorphism** inclusion) const {
    // Build the triangulation.
    Dim2Triangulation* ans = new Dim2Triangulation();
    NPacket::ChangeEventSpan span(ans);

    if (inclusion)
        *inclusion = new NIsomorphism(degree());

    std::vector<NVertexEmbedding>::const_iterator it, adjIt;
    Dim2Triangle* tTri;
    int i;
    for (it = begin(), i = 0; it != end(); ++it, ++i) {
        tTri = ans->newTriangle();
        if (labels) {
            std::stringstream s;
            s << it->tetrahedron()->markedIndex() <<
                " (" << it->vertex() << ')';
            tTri->setDescription(s.str());
        }
        if (inclusion) {
            (*inclusion)->tetImage(i) = it->tetrahedron()->markedIndex();
            (*inclusion)->facetPerm(i) = it->tetrahedron()->
                triangleMapping(it->vertex());
        }
    }

    NTetrahedron *tet, *adj;
    int exitTri, v;
    int edgeInLink;
    int adjIndex;
    int adjVertex;
    for (it = begin(), i = 0; it != end(); ++it, ++i) {
        tet = it->tetrahedron();
        v = it->vertex();

        for (exitTri = 0; exitTri < 4; ++exitTri) {
            if (exitTri == v)
                continue;

            adj = tet->adjacentTetrahedron(exitTri);
            if (! adj)
                continue;

            edgeInLink = tet->triangleMapping(v).preImageOf(exitTri);
            if (ans->triangle(i)->adjacentTriangle(edgeInLink)) {
                // We've already made this gluing in the vertex link
                // from the other side.
                continue;
            }

            adjVertex = tet->adjacentGluing(exitTri)[v];

            // TODO: We need to find which *embedding* corresponds to
            // the adjacent tetrahedron/vertex pair.
            // Currently we do a simple linear scan, which makes the
            // overall link construction quadratic.  This can surely be
            // made linear(ish) with the right data structure and/or algorithm.
            for (adjIt = begin(), adjIndex = 0;
                    adjIt != end(); ++adjIt, ++adjIndex)
                if (adjIt->tetrahedron() == adj &&
                        adjIt->vertex() == adjVertex)
                    break; // Sets adjIndex to the right value.

            ans->triangle(i)->join(edgeInLink, ans->triangle(adjIndex),
                perm4to3(adj->triangleMapping(adjVertex).inverse() *
                    tet->adjacentGluing(exitTri) *
                    tet->triangleMapping(v)));
        }
    }

    return ans;
}

} // namespace regina
