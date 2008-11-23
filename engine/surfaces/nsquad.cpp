
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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

#include <deque>
#include "enumerate/nenumconstraint.h"
#include "surfaces/nsquad.h"
#include "surfaces/nsstandard.h"
#include "utilities/nrational.h"
#include "maths/nmatrixint.h"
#include "maths/nvectorunit.h"
#include "triangulation/ntriangulation.h"

namespace regina {

NNormalSurfaceVector* NNormalSurfaceVectorQuad::makeZeroVector(
        const NTriangulation* triangulation) {
    return new NNormalSurfaceVectorQuad(
        3 * triangulation->getNumberOfTetrahedra());
}

NMatrixInt* NNormalSurfaceVectorQuad::makeMatchingEquations(
        NTriangulation* triangulation) {
    unsigned long nCoords = 3 * triangulation->getNumberOfTetrahedra();
    // One equation per non-boundary edge.
    long nEquations = long(triangulation->getNumberOfEdges());
    for (NTriangulation::BoundaryComponentIterator bit = triangulation->
            getBoundaryComponents().begin();
            bit != triangulation->getBoundaryComponents().end(); bit++)
        nEquations -= (*bit)->getNumberOfEdges();

    NMatrixInt* ans = new NMatrixInt(nEquations, nCoords);
    unsigned long row = 0;

    // Run through each internal edge and add the corresponding
    // equation.
    std::deque<NEdgeEmbedding>::const_iterator embit;
    NPerm perm;
    unsigned long tetIndex;
    for (NTriangulation::EdgeIterator eit = triangulation->getEdges().begin();
            eit != triangulation->getEdges().end(); eit++) {
        if (! (*eit)->isBoundary()) {
            for (embit = (*eit)->getEmbeddings().begin();
                    embit != (*eit)->getEmbeddings().end(); embit++) {
                tetIndex = triangulation->tetrahedronIndex(
                    (*embit).getTetrahedron());
                perm = (*embit).getVertices();
                ans->entry(row, 3 * tetIndex + vertexSplit[perm[0]][perm[2]])
                    += 1;
                ans->entry(row, 3 * tetIndex + vertexSplit[perm[0]][perm[3]])
                    -= 1;
            }
            row++;
        }
    }
    return ans;
}

NEnumConstraintList* NNormalSurfaceVectorQuad::makeEmbeddedConstraints(
        NTriangulation* triangulation) {
    NEnumConstraintList* ans = new NEnumConstraintList(
        triangulation->getNumberOfTetrahedra());

    unsigned base = 0;
    for (unsigned c = 0; c < ans->size(); ++c) {
        (*ans)[c].insert((*ans)[c].end(), base);
        (*ans)[c].insert((*ans)[c].end(), base + 1);
        (*ans)[c].insert((*ans)[c].end(), base + 2);
        base += 3;
    }

    return ans;
}

namespace {
    /**
     * A structure representing a particular end of an edge.
     */
    struct EdgeEnd {
        NEdge* edge;
            /**< The edge under consideration. */
        int end;
            /**< The end of the edge under consideration; this is 0 or 1. */

        EdgeEnd() {}
        EdgeEnd(NEdge* newEdge, int newEnd) : edge(newEdge), end(newEnd) {}
        EdgeEnd(const EdgeEnd& cloneMe) : edge(cloneMe.edge),
                end(cloneMe.end) {}
        void operator = (const EdgeEnd& cloneMe) {
            edge = cloneMe.edge; end = cloneMe.end;
        }
    };
}

NNormalSurfaceVector* NNormalSurfaceVectorQuad::makeMirror(
        NTriangulation* triang) const {
    // We're going to do this by wrapping around each edge and seeing
    // what comes.
    unsigned long nRows = 7 * triang->getNumberOfTetrahedra();
    NNormalSurfaceVectorStandard* ans =
        new NNormalSurfaceVectorStandard(nRows);

    // Set every triangular coordinate in the answer to infinity.
    // For coordinates about vertices not enjoying infinitely many discs,
    // infinity will mean "unknown".
    unsigned long row;
    int i;
    for (row = 0; row < nRows; row+=7)
        for (i = 0; i < 4; i++)
            ans->setElement(row + i, NLargeInteger::infinity);
    for (row = 0; 7 * row < nRows; row++)
        for (i = 0; i < 3; i++)
            ans->setElement(7 * row + 4 + i, (*this)[3 * row + i]);

    // Run through the vertices and work out the triangular coordinates
    // about each vertex in turn.
    stdhash::hash_set<NEdge*, HashPointer> usedEdges[2];
        // usedEdges[i] contains the edges for which we have already
        // examined end i.
    NLargeInteger min;
        // The minimum coordinate that has been assigned about this
        // vertex.
    std::deque<EdgeEnd> examine;
    bool broken;
        // Are the matching equations broken about this edge end?
    int end;
    NEdge* edge;
    EdgeEnd current;
    std::vector<NVertexEmbedding>::const_iterator vembit;
    std::deque<NEdgeEmbedding>::const_iterator eembit, backupit,
        endit, beginit;
    NTetrahedron* tet;
    NTetrahedron* adj;
    NPerm tetPerm, adjPerm;
    unsigned long tetIndex, adjIndex;
    NLargeInteger expect;
    for (NTriangulation::VertexIterator vit = triang->getVertices().begin();
            vit != triang->getVertices().end(); vit++) {
        usedEdges[0].clear(); usedEdges[1].clear();
        examine.clear();
        broken = false;

        // Pick some triangular disc and set it to zero.
        const NVertexEmbedding& vemb = (*vit)->getEmbedding(0);
        row = 7 * triang->tetrahedronIndex(vemb.getTetrahedron())
            + vemb.getVertex();
        ans->setElement(row, NLargeInteger::zero);

        min = NLargeInteger::zero;

        // Mark the three surrounding edge ends for examination.
        for (i=0; i<4; i++) {
            if (i == vemb.getVertex())
                continue;
            edge = vemb.getTetrahedron()->getEdge(
                NEdge::edgeNumber[vemb.getVertex()][i]);
            end = vemb.getTetrahedron()->getEdgeMapping(
                NEdge::edgeNumber[vemb.getVertex()][i])[0] == i ? 1 : 0;
            if (usedEdges[end].insert(edge).second)
                examine.push_back(EdgeEnd(edge, end));
        }

        // Cycle through edge ends until we are finished or until the
        // matching equations are broken.  Each time we pick a value for
        // a coordinate, add the corresponding nearby edge ends to the
        // list of edge ends to examine.
        while ((! broken) && (! examine.empty())) {
            current = examine.front();
            examine.pop_front();

            // Run around this edge end.
            // We know there is a pre-chosen coordinate somewhere; run
            // forwards and find this.
            beginit = current.edge->getEmbeddings().begin();
            endit = current.edge->getEmbeddings().end();
            for (eembit = beginit; eembit != endit; eembit++)
                if (! (*ans)[7 * triang->tetrahedronIndex(
                        (*eembit).getTetrahedron()) +
                        (*eembit).getVertices()[current.end]].isInfinite())
                    break;

            // We are now at the first pre-chosen coordinate about this
            // vertex.  Run backwards from here and fill in all the
            // holes.
            backupit = eembit;
            adj = (*eembit).getTetrahedron();
            adjPerm = (*eembit).getVertices();
            adjIndex = triang->tetrahedronIndex(adj);
            while (eembit != beginit) {
                eembit--;

                // Work out the coordinate for the disc type at eembit.
                tet = (*eembit).getTetrahedron();
                tetPerm = (*eembit).getVertices();
                tetIndex = triang->tetrahedronIndex(tet);

                expect =
                    (*ans)[7 * adjIndex + adjPerm[current.end]] +
                    (*ans)[7 * adjIndex + 4 +
                        vertexSplit[adjPerm[3]][adjPerm[current.end]]] -
                    (*ans)[7 * tetIndex + 4 +
                        vertexSplit[tetPerm[2]][tetPerm[current.end]]];
                ans->setElement(7 * tetIndex + tetPerm[current.end], expect);
                if (expect < min)
                    min = expect;

                // Remember to examine the new edge end if appropriate.
                edge = tet->getEdge(
                    NEdge::edgeNumber[tetPerm[2]][tetPerm[current.end]]);
                end = tet->getEdgeMapping(
                    NEdge::edgeNumber[tetPerm[2]][tetPerm[current.end]])[0]
                    == tetPerm[2] ? 1 : 0;
                if (usedEdges[end].insert(edge).second)
                    examine.push_back(EdgeEnd(edge, end));

                adj = tet;
                adjPerm = tetPerm;
                adjIndex = tetIndex;
            }

            // Now move forwards from the original first pre-chosen
            // coordinate and fill in the holes from here onwards,
            // always checking to ensure the
            // matching equations have not been broken.
            eembit = backupit;
            adj = (*eembit).getTetrahedron();
            adjPerm = (*eembit).getVertices();
            adjIndex = triang->tetrahedronIndex(adj);
            for (eembit++; eembit != endit; eembit++) {
                // Work out the coordinate for the disc type at eembit.
                tet = (*eembit).getTetrahedron();
                tetPerm = (*eembit).getVertices();
                tetIndex = triang->tetrahedronIndex(tet);

                expect =
                    (*ans)[7 * adjIndex + adjPerm[current.end]] +
                    (*ans)[7 * adjIndex + 4 +
                        vertexSplit[adjPerm[2]][adjPerm[current.end]]] -
                    (*ans)[7 * tetIndex + 4 +
                        vertexSplit[tetPerm[3]][tetPerm[current.end]]];
                row = 7 * tetIndex + tetPerm[current.end];
                if ((*ans)[row].isInfinite()) {
                    ans->setElement(row, expect);
                    if (expect < min)
                        min = expect;

                    // Remember to examine the new edge end if appropriate.
                    edge = tet->getEdge(
                        NEdge::edgeNumber[tetPerm[3]][tetPerm[current.end]]);
                    end = tet->getEdgeMapping(
                        NEdge::edgeNumber[tetPerm[3]][tetPerm[current.end]])[0]
                        == tetPerm[3] ? 1 : 0;
                    if (usedEdges[end].insert(edge).second)
                        examine.push_back(EdgeEnd(edge, end));
                } else {
                    // This coordinate has already been set.
                    // Make sure it's the same value!
                    if ((*ans)[row] != expect) {
                        broken = true;
                        break;
                    }
                }

                adj = tet;
                adjPerm = tetPerm;
                adjIndex = tetIndex;
            }
        }

        // If the matching equations were broken, set every coordinate
        // to infinity.  Otherwise subtract min from every coordinate to
        // make the values as small as possible.
        for (vembit = (*vit)->getEmbeddings().begin();
                vembit != (*vit)->getEmbeddings().end(); vembit++) {
            row = 7 * triang->tetrahedronIndex((*vembit).getTetrahedron())
                + (*vembit).getVertex();
            if (broken)
                ans->setElement(row, NLargeInteger::infinity);
            else
                ans->setElement(row, (*ans)[row] - min);
        }
    }

    // Note that there should be no need to remove common factors since
    // the quad coordinates have not changed and in theory they already
    // had gcd=1.
    return ans;
}

} // namespace regina

