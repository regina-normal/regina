
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

namespace regina {

void Triangulation<3>::maximalForestInBoundary(std::set<Edge<3>*>& edgeSet,
        std::set<Vertex<3>*>& vertexSet) const {
    ensureSkeleton();

    vertexSet.clear();
    edgeSet.clear();
    for (auto bc : boundaryComponents())
        stretchBoundaryForestFromVertex(bc->vertex(0), edgeSet, vertexSet);
}

void Triangulation<3>::stretchBoundaryForestFromVertex(Vertex<3>* from,
        std::set<Edge<3>*>& edgeSet,
        std::set<Vertex<3>*>& vertexSet) const {
    vertexSet.insert(from);

    Tetrahedron<3>* tet;
    Vertex<3>* otherVertex;
    Edge<3>* edge;
    int vertex, yourVertex;
    for (auto& emb : *from) {
        tet = emb.tetrahedron();
        vertex = emb.vertex();
        for (yourVertex = 0; yourVertex < 4; yourVertex++) {
            if (vertex == yourVertex)
                continue;
            edge = tet->edge(Edge<3>::edgeNumber[vertex][yourVertex]);
            if (! (edge->isBoundary()))
                continue;
            otherVertex = tet->vertex(yourVertex);
            if (! vertexSet.count(otherVertex)) {
                edgeSet.insert(edge);
                stretchBoundaryForestFromVertex(otherVertex, edgeSet,
                    vertexSet);
            }
        }
    }
}

void Triangulation<3>::maximalForestInSkeleton(std::set<Edge<3>*>& edgeSet,
        bool canJoinBoundaries) const {
    ensureSkeleton();

    std::set<Vertex<3>*> vertexSet;
    std::set<Vertex<3>*> thisBranch;

    if (canJoinBoundaries)
        edgeSet.clear();
    else
        maximalForestInBoundary(edgeSet, vertexSet);

    for (Vertex<3>* v : vertices())
        if (! (vertexSet.count(v))) {
            stretchForestFromVertex(v, edgeSet, vertexSet, thisBranch);
            thisBranch.clear();
        }
}

bool Triangulation<3>::stretchForestFromVertex(Vertex<3>* from,
        std::set<Edge<3>*>& edgeSet,
        std::set<Vertex<3>*>& vertexSet,
        std::set<Vertex<3>*>& thisStretch) const {
    // Moves out from the vertex until we hit a vertex that has already
    //     been visited; then stops.
    // Returns true if we make such a link.
    // PRE: Such a link has not already been made.
    vertexSet.insert(from);
    thisStretch.insert(from);

    Tetrahedron<3>* tet;
    Vertex<3>* otherVertex;
    int vertex, yourVertex;
    bool madeLink = false;
    for (auto& emb : *from) {
        tet = emb.tetrahedron();
        vertex = emb.vertex();
        for (yourVertex = 0; yourVertex < 4; yourVertex++) {
            if (vertex == yourVertex)
                continue;
            otherVertex = tet->vertex(yourVertex);
            if (thisStretch.count(otherVertex))
                continue;
            madeLink = vertexSet.count(otherVertex);
            edgeSet.insert(tet->edge(Edge<3>::edgeNumber[vertex][yourVertex]));
            if (! madeLink)
                madeLink =
                    stretchForestFromVertex(otherVertex, edgeSet, vertexSet,
                    thisStretch);
            if (madeLink)
                return true;
        }
    }
    return false;
}

} // namespace regina

