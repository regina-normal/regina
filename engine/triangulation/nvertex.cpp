
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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
#include "triangulation/nvertex.h"

namespace regina {

const int NVertex::SPHERE = 1;
const int NVertex::DISC = 2;
const int NVertex::TORUS = 3;
const int NVertex::KLEIN_BOTTLE = 4;
const int NVertex::NON_STANDARD_CUSP = 5;
const int NVertex::NON_STANDARD_BDRY = 6;

NVertex::~NVertex() {
    delete linkTri;
}

void NVertex::writeTextShort(std::ostream& out) const {
    switch(link) {
        case SPHERE: out << "Internal "; break;
        case DISC: out << "Boundary "; break;
        case TORUS: out << "Torus cusp "; break;
        case KLEIN_BOTTLE: out << "Klein bottle cusp "; break;
        case NON_STANDARD_CUSP: out << "Non-standard cusp "; break;
        case NON_STANDARD_BDRY: out << "Non-standard boundary "; break;
    }
    out << "vertex of degree " << getNumberOfEmbeddings();
}

const Dim2Triangulation* NVertex::buildLink() const {
    if (linkTri)
        return linkTri;

    // Build the triangulation.
    Dim2Triangulation* ans = new Dim2Triangulation();
    NPacket::ChangeEventSpan span(ans);

    std::vector<NVertexEmbedding>::const_iterator it, adjIt;
    for (it = embeddings.begin(); it != embeddings.end(); ++it)
        ans->newTriangle();

    NTetrahedron *tet, *adj;
    int i, exitTri, v;
    int edgeInLink;
    int adjIndex;
    int adjVertex;
    for (it = embeddings.begin(), i = 0; it != embeddings.end(); ++it, ++i) {
        tet = it->getTetrahedron();
        v = it->getVertex();

        for (exitTri = 0; exitTri < 4; ++exitTri) {
            if (exitTri == v)
                continue;

            adj = tet->adjacentTetrahedron(exitTri);
            if (! adj)
                continue;

            edgeInLink = tet->getFaceMapping(v).preImageOf(exitTri);
            if (ans->getTriangle(i)->adjacentTriangle(edgeInLink)) {
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
            for (adjIt = embeddings.begin(), adjIndex = 0;
                    adjIt != embeddings.end(); ++adjIt, ++adjIndex)
                if (adjIt->getTetrahedron() == adj &&
                        adjIt->getVertex() == adjVertex)
                    break; // Sets adjIndex to the right value.

            ans->getTriangle(i)->joinTo(edgeInLink, ans->getTriangle(adjIndex),
                perm4to3(adj->getFaceMapping(adjVertex).inverse() *
                    tet->adjacentGluing(exitTri) *
                    tet->getFaceMapping(v)));
        }
    }

    const_cast<NVertex*>(this)->linkTri = ans;
    return linkTri;
}

} // namespace regina
