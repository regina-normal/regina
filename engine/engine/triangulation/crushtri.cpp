
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
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

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "ntriangulation.h"
#else
    #include "engine/triangulation/ntriangulation.h"
#endif

void NTriangulation::maximalForestInBoundary(NPointerSet<NEdge>& edgeSet,
        NPointerSet<NVertex>& vertexSet) {
    if (! calculatedSkeleton)
        calculateSkeleton();

    vertexSet.flush();
    edgeSet.flush();
    BoundaryComponentIterator bit(boundaryComponents);
    NBoundaryComponent* bc;
    while (! bit.done()) {
        bc = *bit;
        stretchBoundaryForestFromVertex(bc->getVertex(0), edgeSet, vertexSet);

        // Move to the next component.
        bit++;
    }
}

void NTriangulation::stretchBoundaryForestFromVertex(NVertex* from,
        NPointerSet<NEdge>& edgeSet, NPointerSet<NVertex>& vertexSet) {
    vertexSet.add(from);

    NDynamicArrayIterator<NVertexEmbedding> it(from->getEmbeddings());
    NTetrahedron* tet;
    NVertex* otherVertex;
    NEdge* edge;
    int vertex, yourVertex;
    while (! it.done()) {
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
            if (! vertexSet.contains(otherVertex)) {
                edgeSet.add(edge);
                stretchBoundaryForestFromVertex(otherVertex, edgeSet,
                    vertexSet);
            }
        }
        it++;
    }
}

void NTriangulation::maximalForestInSkeleton(NPointerSet<NEdge>& edgeSet,
        bool canJoinBoundaries) {
    if (! calculatedSkeleton)
        calculateSkeleton();

    NPointerSet<NVertex> vertexSet;
    NPointerSet<NVertex> thisBranch;

    if (canJoinBoundaries)
        edgeSet.flush();
    else
        maximalForestInBoundary(edgeSet, vertexSet);

    VertexIterator vit(vertices);
    NVertex* vertex;

    while (! vit.done()) {
        vertex = *vit;
        if (! (vertexSet.contains(vertex))) {
            stretchForestFromVertex(vertex, edgeSet, vertexSet, thisBranch);
            thisBranch.flush();
        }
        vit++;
    }
}

bool NTriangulation::stretchForestFromVertex(NVertex* from,
        NPointerSet<NEdge>& edgeSet, NPointerSet<NVertex>& vertexSet,
        NPointerSet<NVertex>& thisStretch) {
    // Moves out from the vertex until we hit a vertex that has already
    //     been visited; then stops.
    // Returns true if we make such a link.
    // PRE: Such a link has not already been made.
    vertexSet.add(from);
    thisStretch.add(from);

    NDynamicArrayIterator<NVertexEmbedding> it(from->getEmbeddings());
    NTetrahedron* tet;
    NVertex* otherVertex;
    int vertex, yourVertex;
    bool madeLink = false;
    while (! it.done()) {
        const NVertexEmbedding& emb = *it;
        tet = emb.getTetrahedron();
        vertex = emb.getVertex();
        for (yourVertex = 0; yourVertex < 4; yourVertex++) {
            if (vertex == yourVertex)
                continue;
            otherVertex = tet->getVertex(yourVertex);
            if (thisStretch.contains(otherVertex))
                continue;
            madeLink = vertexSet.contains(otherVertex);
            edgeSet.add(tet->getEdge(edgeNumber[vertex][yourVertex]));
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
    NPointerSet<NEdge> cEdges;
    maximalForestInSkeleton(cEdges, false);

    /* --- DEBUGGING OUTPUT ---
    {
        NPointerSet<NFace> dual;
        maximalForestInDualSkeleton(dual);
        cerr << "Dual Faces: " << dual.size() << '\n';
        NPointerSetIterator<NFace> it(dual);
        while (! it.done()) {
            cerr << faces.position(*it) << ' ';
            it++;
        }
        cerr << '\n';
    }

    cerr << "Edges going: " << cEdges.size() << '\n';
    {
        NPointerSetIterator<NEdge> it(cEdges);
        while (! it.done()) {
            cerr << edges.position(*it) << ' ';
            it++;
        }
        cerr << '\n';
    }
    ------------------------*/

    NPointerSet<NTetrahedron> cTetrahedra;

    // Extend this list of collapsings to faces.
    NTetrahedron* tet;
    TetrahedronIterator tit;
    int face, edge;
    int nLost;
    bool changed = true;
    while (changed) {
        changed = false;
        tit.init(tetrahedra);
        while (! tit.done()) {
            tet = *tit;
            for (face = 0; face < 4; face++) {
                nLost = 0;
                for (edge = 0; edge < 6; edge++) {
                    if (edgeStart[edge] == face || edgeEnd[edge] == face)
                        continue;
                    if (cEdges.contains(tet->getEdge(edge)))
                        nLost++;
                }
                // Changed > 1 below to == 2 since we're not storing
                // faces.
                if (nLost == 2) {
                    for (edge = 0; edge < 6; edge++) {
                        if (edgeStart[edge] == face || edgeEnd[edge] == face)
                            continue;
                        cEdges.add(tet->getEdge(edge));
                    }
                    changed = true;
                }
            }
            
            tit++;
        }
    }

    // Finally extend this list to tetrahedra.
    tit.init(tetrahedra);
    while (! tit.done()) {
        tet = *tit;
        for (edge = 0; edge < 6; edge++)
            if (cEdges.contains(tet->getEdge(edge))) {
                cTetrahedra.add(tet);
                break;
            }
        tit++;
    }

    // Are we going to change anything?
    if (cTetrahedra.size() == 0)
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
    tit.init(tetrahedra);
    while (! tit.done()) {
        tet = *tit;
        if (! cTetrahedra.contains(tet))
            for (face = 0; face < 4; face++) {
                adjTet = tet->getAdjacentTetrahedron(face);
                if (adjTet == 0)
                    continue;
                if (! cTetrahedra.contains(adjTet))
                    continue;
                adjPerm = tet->getAdjacentTetrahedronGluing(face);
                adjFace = adjPerm[face];
                while(1) {
                    // Follow through to the next face.
                    for (edgeFrom = 0; edgeFrom < 4; edgeFrom++) {
                        if (edgeFrom == adjFace)
                            continue;
                        if (! (cEdges.contains(adjTet->
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

                    if (! (cTetrahedra.contains(adjTet))) {
                        // Glue the original face to this safe
                        // tetrahedron.
                        tet->unjoin(face);
                        adjTet->unjoin(adjFace);
                        tet->joinTo(face, adjTet, adjPerm);
                        break;
                    }
                }
            }
        tit++;
    }

    // Remove the squished tetrahedra.
    NPointerSetIterator<NTetrahedron> cit(cTetrahedra);
    while (! cit.done()) {
        delete tetrahedra.remove(*cit);
        cit++;
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

void NTriangulation::maximalForestInDualSkeleton(NPointerSet<NFace>& faceSet) {
    if (! calculatedSkeleton)
        calculateSkeleton();
    faceSet.flush();

    NPointerSet<NTetrahedron> visited;
    TetrahedronIterator it(tetrahedra);
    NTetrahedron* tet;

    while (! it.done()) {
        tet = *it;
        if (! (visited.contains(tet)))
            stretchDualForestFromTet(tet, faceSet, visited);
        it++;
    }
}

void NTriangulation::stretchDualForestFromTet(NTetrahedron* tet,
        NPointerSet<NFace>& faceSet, NPointerSet<NTetrahedron>& visited) {
    visited.add(tet);

    NTetrahedron* adjTet;
    for (int face = 0; face < 4; face++) {
        adjTet = tet->getAdjacentTetrahedron(face);
        if (adjTet)
            if (! (visited.contains(adjTet))) {
                faceSet.add(tet->getFace(face));
                stretchDualForestFromTet(adjTet, faceSet, visited);
            }
    }
}

