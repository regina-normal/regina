
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

#include <deque>
#include <set>
#include "enumerate/nenumconstraint.h"
#include "surfaces/nsorientedquad.h"
#include "surfaces/nsoriented.h"
#include "maths/nmatrixint.h"
#include "maths/nrational.h"
#include "triangulation/ntriangulation.h"

namespace regina {

NNormalSurfaceVector* NNormalSurfaceVectorOrientedQuad::makeZeroVector(
        const NTriangulation* triangulation) {
    return new NNormalSurfaceVectorOrientedQuad(
        6 * triangulation->getNumberOfTetrahedra());
}

NMatrixInt* NNormalSurfaceVectorOrientedQuad::makeMatchingEquations(
        NTriangulation* triangulation) {
    unsigned long nCoords = 6 * triangulation->getNumberOfTetrahedra();
    // Two equation per non-boundary edge.
    long nEquations = 2*long(triangulation->getNumberOfEdges());
    for (NTriangulation::BoundaryComponentIterator bit = triangulation->
            getBoundaryComponents().begin();
            bit != triangulation->getBoundaryComponents().end(); bit++)
        nEquations -= 2*(*bit)->getNumberOfEdges();

    NMatrixInt* ans = new NMatrixInt(nEquations, nCoords);
    unsigned long row = 0;

    // Run through each internal edge and add the corresponding
    // equation.
    std::deque<NEdgeEmbedding>::const_iterator embit;
    NPerm4 perm;
    unsigned long tetIndex;
    bool flip;
    for (NTriangulation::EdgeIterator eit = triangulation->getEdges().begin();
            eit != triangulation->getEdges().end(); eit++) {
        if (! (*eit)->isBoundary()) {
            for (embit = (*eit)->getEmbeddings().begin();
                    embit != (*eit)->getEmbeddings().end(); embit++) {
                tetIndex = triangulation->tetrahedronIndex(
                    (*embit).getTetrahedron());
                perm = (*embit).getVertices();

                flip = (perm[0] == 0 || perm[2] == 0);
                ans->entry(row, 6 * tetIndex +
                    2*vertexSplit[perm[0]][perm[2]] + (flip ? 0 : 1)) += 1;
                ans->entry(row+1, 6 * tetIndex +
                    2*vertexSplit[perm[0]][perm[2]] + (flip ? 1 : 0)) += 1;

                flip = (perm[0] == 0 || perm[3] == 0);
                ans->entry(row, 6 * tetIndex +
                    2*vertexSplit[perm[0]][perm[3]] + (flip ? 0 : 1)) -= 1;
                ans->entry(row+1, 6 * tetIndex +
                    2*vertexSplit[perm[0]][perm[3]] + (flip ? 1 : 0)) -= 1;
            }
            row+=2;
        }
    }
    return ans;
}

NEnumConstraintList* NNormalSurfaceVectorOrientedQuad::makeEmbeddedConstraints(
        NTriangulation* triangulation) {
    NEnumConstraintList* ans = new NEnumConstraintList(
        8 * triangulation->getNumberOfTetrahedra());

    unsigned base = 0;
    unsigned c = 0;
    while (c < ans->size()) {
        for (int a = 0 ; a < 2; a++) {
            for (int b = 0 ; b < 2; b++) {
                for (int d = 0 ; d < 2; d++) {
                    (*ans)[c].insert((*ans)[c].end(), base + a);
                    (*ans)[c].insert((*ans)[c].end(), base + b + 2);
                    (*ans)[c].insert((*ans)[c].end(), base + d + 4);
                    ++c;
                }
            }
        }
        base += 6;
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

NNormalSurfaceVector* NNormalSurfaceVectorOrientedQuad::makeMirror(
        NTriangulation* triang) const {
    // We're going to do this by wrapping around each edge and seeing
    // what comes.
    unsigned long nRows = 14 * triang->getNumberOfTetrahedra();
    NNormalSurfaceVectorOriented* ans =
        new NNormalSurfaceVectorOriented(nRows);

    // Set every triangular coordinate in the answer to infinity.
    // For coordinates about vertices not enjoying infinitely many discs,
    // infinity will mean "unknown".
    unsigned long row;
    int i;
    for (row = 0; row < nRows; row+=14)
        for (i = 0; i < 8; i++)
            ans->setElement(row + i, NLargeInteger::infinity);
    for (row = 0; 14 * row < nRows; row++)
        for (i = 0; i < 6; i++)
            ans->setElement(14 * row + 8 + i, (*this)[6 * row + i]);

    for (int orient = 0; orient < 2; ++orient) {
        // Run through the vertices and work out the triangular coordinates
        // about each vertex in turn.
        //
        // If orient = 0 or 1, we look at triangular discs oriented towards
        // or away from the vertex respetively.

        std::set<NEdge*> usedEdges[2];
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
        NPerm4 tetPerm, adjPerm;
        unsigned long tetIndex, adjIndex;
        NLargeInteger expect;
        for (NTriangulation::VertexIterator vit = triang->getVertices().begin();
                vit != triang->getVertices().end(); vit++) {
            usedEdges[0].clear(); usedEdges[1].clear();
            examine.clear();
            broken = false;

            // Pick some triangular disc and set it to zero.
            const NVertexEmbedding& vemb = (*vit)->getEmbedding(0);
            row = 14 * triang->tetrahedronIndex(vemb.getTetrahedron())
                + 2 * vemb.getVertex() + orient;
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
                    if (! (*ans)[14 * triang->tetrahedronIndex(
                            (*eembit).getTetrahedron()) +
                            2 * (*eembit).getVertices()[current.end] +
                            orient].isInfinite())
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
                        (*ans)[14 * adjIndex + 2 * adjPerm[current.end] +
                            orient] +
                        (*ans)[14 * adjIndex + 8 +
                            2 * vertexSplit[adjPerm[3]][adjPerm[current.end]] +
                            ((adjPerm[current.end] == 0 || adjPerm[3] == 0) ?
                                orient : (1 - orient))] -
                        (*ans)[14 * tetIndex + 8 +
                            2 * vertexSplit[tetPerm[2]][tetPerm[current.end]] +
                            ((tetPerm[current.end] == 0 || tetPerm[2] == 0) ?
                                orient : (1 - orient))];
                    ans->setElement(14 * tetIndex + 2 * tetPerm[current.end] +
                        orient, expect);
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
                        (*ans)[14 * adjIndex + 2 * adjPerm[current.end] +
                            orient] +
                        (*ans)[14 * adjIndex + 8 +
                            2 * vertexSplit[adjPerm[2]][adjPerm[current.end]] +
                            ((adjPerm[current.end] == 0 || adjPerm[2] == 0) ?
                                orient : (1 - orient))] -
                        (*ans)[14 * tetIndex + 8 +
                            2 * vertexSplit[tetPerm[3]][tetPerm[current.end]] +
                            ((tetPerm[current.end] == 0 || tetPerm[3] == 0) ?
                                orient : (1 - orient))];
                    row = 14 * tetIndex + 2 * tetPerm[current.end] + orient;
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
                row = 14 * triang->tetrahedronIndex((*vembit).getTetrahedron())
                    + 2 * (*vembit).getVertex() + orient;
                if (broken)
                    ans->setElement(row, NLargeInteger::infinity);
                else
                    ans->setElement(row, (*ans)[row] - min);
            }
        }
    }

    // Note that there should be no need to remove common factors since
    // the quad coordinates have not changed and in theory they already
    // had gcd=1.
    return ans;
} 

} // namespace regina

