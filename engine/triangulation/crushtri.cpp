
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

#include "triangulation/ntriangulation.h"

namespace regina {

void NTriangulation::maximalForestInBoundary(
        stdhash::hash_set<NEdge*, HashPointer>& edgeSet,
        stdhash::hash_set<NVertex*, HashPointer>& vertexSet) const {
    if (! calculatedSkeleton)
        calculateSkeleton();

    vertexSet.clear();
    edgeSet.clear();
    for (BoundaryComponentIterator bit = boundaryComponents.begin();
            bit != boundaryComponents.end(); bit++)
        stretchBoundaryForestFromVertex((*bit)->getVertex(0),
            edgeSet, vertexSet);
}

void NTriangulation::stretchBoundaryForestFromVertex(NVertex* from,
        stdhash::hash_set<NEdge*, HashPointer>& edgeSet,
        stdhash::hash_set<NVertex*, HashPointer>& vertexSet) const {
    vertexSet.insert(from);

    std::vector<NVertexEmbedding>::const_iterator it =
        from->getEmbeddings().begin();
    NTetrahedron* tet;
    NVertex* otherVertex;
    NEdge* edge;
    int vertex, yourVertex;
    while (it != from->getEmbeddings().end()) {
        const NVertexEmbedding& emb = *it;
        tet = emb.getTetrahedron();
        vertex = emb.getVertex();
        for (yourVertex = 0; yourVertex < 4; yourVertex++) {
            if (vertex == yourVertex)
                continue;
            edge = tet->getEdge(NEdge::edgeNumber[vertex][yourVertex]);
            if (! (edge->isBoundary()))
                continue;
            otherVertex = tet->getVertex(yourVertex);
            if (! vertexSet.count(otherVertex)) {
                edgeSet.insert(edge);
                stretchBoundaryForestFromVertex(otherVertex, edgeSet,
                    vertexSet);
            }
        }
        it++;
    }
}

void NTriangulation::maximalForestInSkeleton(
        stdhash::hash_set<NEdge*, HashPointer>& edgeSet,
        bool canJoinBoundaries) const {
    if (! calculatedSkeleton)
        calculateSkeleton();

    stdhash::hash_set<NVertex*, HashPointer> vertexSet;
    stdhash::hash_set<NVertex*, HashPointer> thisBranch;

    if (canJoinBoundaries)
        edgeSet.clear();
    else
        maximalForestInBoundary(edgeSet, vertexSet);

    for (VertexIterator vit = vertices.begin(); vit != vertices.end(); vit++)
        if (! (vertexSet.count(*vit))) {
            stretchForestFromVertex(*vit, edgeSet, vertexSet, thisBranch);
            thisBranch.clear();
        }
}

bool NTriangulation::stretchForestFromVertex(NVertex* from,
        stdhash::hash_set<NEdge*, HashPointer>& edgeSet,
        stdhash::hash_set<NVertex*, HashPointer>& vertexSet,
        stdhash::hash_set<NVertex*, HashPointer>& thisStretch) const {
    // Moves out from the vertex until we hit a vertex that has already
    //     been visited; then stops.
    // Returns true if we make such a link.
    // PRE: Such a link has not already been made.
    vertexSet.insert(from);
    thisStretch.insert(from);

    std::vector<NVertexEmbedding>::const_iterator it =
        from->getEmbeddings().begin();
    NTetrahedron* tet;
    NVertex* otherVertex;
    int vertex, yourVertex;
    bool madeLink = false;
    while (it != from->getEmbeddings().end()) {
        const NVertexEmbedding& emb = *it;
        tet = emb.getTetrahedron();
        vertex = emb.getVertex();
        for (yourVertex = 0; yourVertex < 4; yourVertex++) {
            if (vertex == yourVertex)
                continue;
            otherVertex = tet->getVertex(yourVertex);
            if (thisStretch.count(otherVertex))
                continue;
            madeLink = vertexSet.count(otherVertex);
            edgeSet.insert(tet->getEdge(NEdge::edgeNumber[vertex][yourVertex]));
            if (! madeLink)
                madeLink =
                    stretchForestFromVertex(otherVertex, edgeSet, vertexSet,
                    thisStretch);
            if (madeLink)
                return true;
        }
        it++;
    }
    return false;
}

void NTriangulation::maximalForestInDualSkeleton(
        stdhash::hash_set<NFace*, HashPointer>& faceSet) const {
    if (! calculatedSkeleton)
        calculateSkeleton();

    faceSet.clear();
    stdhash::hash_set<NTetrahedron*, HashPointer> visited;
    for (TetrahedronIterator it = tetrahedra.begin(); it != tetrahedra.end();
            it++)
        if (! (visited.count(*it)))
            stretchDualForestFromTet(*it, faceSet, visited);
}

void NTriangulation::stretchDualForestFromTet(NTetrahedron* tet,
        stdhash::hash_set<NFace*, HashPointer>& faceSet,
        stdhash::hash_set<NTetrahedron*, HashPointer>& visited) const {
    visited.insert(tet);

    NTetrahedron* adjTet;
    for (int face = 0; face < 4; face++) {
        adjTet = tet->adjacentTetrahedron(face);
        if (adjTet)
            if (! (visited.count(adjTet))) {
                faceSet.insert(tet->getFace(face));
                stretchDualForestFromTet(adjTet, faceSet, visited);
            }
    }
}

} // namespace regina

