
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "triangulation/ntriangulation.h"

void NTriangulation::maximalForestInBoundary(
        stdhash::hash_set<NEdge*, HashPointer>& edgeSet,
        stdhash::hash_set<NVertex*, HashPointer>& vertexSet) {
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
        stdhash::hash_set<NVertex*, HashPointer>& vertexSet) {
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
            edge = tet->getEdge(edgeNumber[vertex][yourVertex]);
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
        bool canJoinBoundaries) {
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
        stdhash::hash_set<NVertex*, HashPointer>& thisStretch) {
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
            edgeSet.insert(tet->getEdge(edgeNumber[vertex][yourVertex]));
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

bool NTriangulation::crushMaximalForest() {
    // First obtain a maximal forest in the 1-skeleton.
    stdhash::hash_set<NEdge*, HashPointer> cEdges;
    maximalForestInSkeleton(cEdges, false);

    /* --- DEBUGGING OUTPUT ---
    {
        stdhash::hash_set<NFace*, HashPointer> dual;
        maximalForestInDualSkeleton(dual);
        cerr << "Dual Faces: " << dual.size() << '\n';
        NPointerSetIterator<NFace> it(dual);
        while (! it.done()) {
            cerr << faces.index(*it) << ' ';
            it++;
        }
        cerr << '\n';
    }

    cerr << "Edges going: " << cEdges.size() << '\n';
    {
        NPointerSetIterator<NEdge> it(cEdges);
        while (! it.done()) {
            cerr << edges.index(*it) << ' ';
            it++;
        }
        cerr << '\n';
    }
    ------------------------*/

    stdhash::hash_set<NTetrahedron*, HashPointer> cTetrahedra;

    // Extend this list of collapsings to faces.
    NTetrahedron* tet;
    TetrahedronIterator tit;
    int face, edge;
    int nLost;
    bool changed = true;
    while (changed) {
        changed = false;
        for (tit = tetrahedra.begin(); tit != tetrahedra.end(); tit++) {
            tet = *tit;
            for (face = 0; face < 4; face++) {
                nLost = 0;
                for (edge = 0; edge < 6; edge++) {
                    if (edgeStart[edge] == face || edgeEnd[edge] == face)
                        continue;
                    if (cEdges.count(tet->getEdge(edge)))
                        nLost++;
                }
                // Changed > 1 below to == 2 since we're not storing
                // faces.
                if (nLost == 2) {
                    for (edge = 0; edge < 6; edge++) {
                        if (edgeStart[edge] == face || edgeEnd[edge] == face)
                            continue;
                        cEdges.insert(tet->getEdge(edge));
                    }
                    changed = true;
                }
            }
        }
    }

    // Finally extend this list to tetrahedra.
    for (tit = tetrahedra.begin(); tit != tetrahedra.end(); tit++) {
        tet = *tit;
        for (edge = 0; edge < 6; edge++)
            if (cEdges.count(tet->getEdge(edge))) {
                cTetrahedra.insert(tet);
                break;
            }
    }

    // Are we going to change anything?
    if (cTetrahedra.empty())
        return false;

    // Prepare to measure the change in topology.
    /*
    const NAbelianGroup& h1(getHomologyH1());
    unsigned long oldComponents = components.size();
    unsigned long oldRank = h1.getRank();
    unsigned long oldRank2 = h1.getTorsionRank(2);
    unsigned long oldRank3 = h1.getTorsionRank(3);
    */

    // Reglue the surviving tetrahedra.
    NTetrahedron* adjTet;
    NTetrahedron* tmpTet;
    NPerm adjPerm;
    int adjFace, edgeFrom;
    for (tit = tetrahedra.begin(); tit != tetrahedra.end(); tit++) {
        tet = *tit;
        if (! cTetrahedra.count(tet))
            for (face = 0; face < 4; face++) {
                adjTet = tet->getAdjacentTetrahedron(face);
                if (adjTet == 0)
                    continue;
                if (! cTetrahedra.count(adjTet))
                    continue;
                adjPerm = tet->getAdjacentTetrahedronGluing(face);
                adjFace = adjPerm[face];
                while(1) {
                    // Follow through to the next face.
                    for (edgeFrom = 0; edgeFrom < 4; edgeFrom++) {
                        if (edgeFrom == adjFace)
                            continue;
                        if (! (cEdges.count(adjTet->
                                getEdge(edgeNumber[adjFace][edgeFrom]))))
                            continue;
                        break;
                    }
                    // Follow edge from edgeFrom to adjFace.
                    // The face of adjTet we now move to is edgeFrom.
                    tmpTet = adjTet->getAdjacentTetrahedron(edgeFrom);
                    if (tmpTet == 0) {
                        // Make the original face a boundary face.
                        tet->unjoin(face);
                        break;
                    }
                    adjPerm = adjTet->getAdjacentTetrahedronGluing(edgeFrom) *
                        NPerm(adjFace, edgeFrom) * adjPerm;
                    adjFace = adjPerm[face];
                    adjTet = tmpTet;

                    if (! (cTetrahedra.count(adjTet))) {
                        // Glue the original face to this safe
                        // tetrahedron.
                        tet->unjoin(face);
                        adjTet->unjoin(adjFace);
                        tet->joinTo(face, adjTet, adjPerm);
                        break;
                    }
                }
            }
    }

    // Remove the squished tetrahedra.
    // For each tetrahedron, remove it and delete it.
    for (stdhash::hash_set<NTetrahedron*, HashPointer>::iterator tetIt =
            cTetrahedra.begin(); tetIt != cTetrahedra.end(); tetIt++) {
        tetrahedra.erase(*tetIt);
        delete *tetIt;
    }

    // Tidy up.
    gluingsHaveChanged();

    // Measure the change in topology.
    /*
    const NAbelianGroup& newH1 = getHomologyH1();
    extraTopology = extraTopology + NExtraTopology(
        components.size() - oldComponents,
        oldRank - newH1.getRank(),
        oldRank3 - newH1.getTorsionRank(3),
        oldRank2 - newH1.getTorsionRank(2)
        );
    */

    return true;
}

void NTriangulation::maximalForestInDualSkeleton(
        stdhash::hash_set<NFace*, HashPointer>& faceSet) {
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
        stdhash::hash_set<NTetrahedron*, HashPointer>& visited) {
    visited.insert(tet);

    NTetrahedron* adjTet;
    for (int face = 0; face < 4; face++) {
        adjTet = tet->getAdjacentTetrahedron(face);
        if (adjTet)
            if (! (visited.count(adjTet))) {
                faceSet.insert(tet->getFace(face));
                stretchDualForestFromTet(adjTet, faceSet, visited);
            }
    }
}

