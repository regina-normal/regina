
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

#include <deque>
#include <set>
#include "enumerate/enumconstraints.h"
#include "surfaces/nsvectororientedquad.h"
#include "surfaces/nsvectororiented.h"
#include "maths/matrix.h"
#include "maths/rational.h"
#include "triangulation/dim3.h"

namespace regina {

NormalSurfaceVector* NSVectorOrientedQuad::makeZeroVector(
        const Triangulation<3>* triangulation) {
    return new NSVectorOrientedQuad(6 * triangulation->size());
}

MatrixInt* NSVectorOrientedQuad::makeMatchingEquations(
        const Triangulation<3>* triangulation) {
    size_t nCoords = 6 * triangulation->size();
    // Two equation per non-boundary edge.
    long nEquations = 2*long(triangulation->countEdges());
    for (Triangulation<3>::BoundaryComponentIterator bit = triangulation->
            boundaryComponents().begin();
            bit != triangulation->boundaryComponents().end(); bit++)
        nEquations -= 2*(*bit)->countEdges();

    MatrixInt* ans = new MatrixInt(nEquations, nCoords);
    size_t row = 0;

    // Run through each internal edge and add the corresponding
    // equation.
    Perm<4> perm;
    size_t tetIndex;
    bool flip;
    for (Triangulation<3>::EdgeIterator eit = triangulation->edges().begin();
            eit != triangulation->edges().end(); eit++) {
        if (! (*eit)->isBoundary()) {
            for (auto& emb : **eit) {
                tetIndex = emb.tetrahedron()->index();
                perm = emb.vertices();

                flip = (perm[0] == 0 || perm[2] == 0);
                ans->entry(row, 6 * tetIndex +
                    2*quadSeparating[perm[0]][perm[2]] + (flip ? 0 : 1)) += 1;
                ans->entry(row+1, 6 * tetIndex +
                    2*quadSeparating[perm[0]][perm[2]] + (flip ? 1 : 0)) += 1;

                flip = (perm[0] == 0 || perm[3] == 0);
                ans->entry(row, 6 * tetIndex +
                    2*quadSeparating[perm[0]][perm[3]] + (flip ? 0 : 1)) -= 1;
                ans->entry(row+1, 6 * tetIndex +
                    2*quadSeparating[perm[0]][perm[3]] + (flip ? 1 : 0)) -= 1;
            }
            row+=2;
        }
    }
    return ans;
}

EnumConstraints* NSVectorOrientedQuad::makeEmbeddedConstraints(
        const Triangulation<3>* triangulation) {
    EnumConstraints* ans = new EnumConstraints(8 * triangulation->size());

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
        Edge<3>* edge;
            /**< The edge under consideration. */
        int end;
            /**< The end of the edge under consideration; this is 0 or 1. */

        EdgeEnd() {}
        EdgeEnd(Edge<3>* newEdge, int newEnd) : edge(newEdge), end(newEnd) {}
        EdgeEnd(const EdgeEnd& cloneMe) : edge(cloneMe.edge),
                end(cloneMe.end) {}
        void operator = (const EdgeEnd& cloneMe) {
            edge = cloneMe.edge; end = cloneMe.end;
        }
    };
}

NormalSurfaceVector* NSVectorOrientedQuad::makeMirror(
        const Ray& original, const Triangulation<3>* triang) {
    // We're going to do this by wrapping around each edge and seeing
    // what comes.
    size_t nRows = 14 * triang->size();
    NSVectorOriented* ans = new NSVectorOriented(nRows);

    // Set every triangular coordinate in the answer to infinity.
    // For coordinates about vertices not enjoying infinitely many discs,
    // infinity will mean "unknown".
    size_t row;
    int i;
    for (row = 0; row < nRows; row+=14)
        for (i = 0; i < 8; i++)
            ans->setElement(row + i, LargeInteger::infinity);
    for (row = 0; 14 * row < nRows; row++)
        for (i = 0; i < 6; i++)
            ans->setElement(14 * row + 8 + i, original[6 * row + i]);

    for (int orient = 0; orient < 2; ++orient) {
        // Run through the vertices and work out the triangular coordinates
        // about each vertex in turn.
        //
        // If orient = 0 or 1, we look at triangular discs oriented towards
        // or away from the vertex respetively.

        std::set<Edge<3>*> usedEdges[2];
            // usedEdges[i] contains the edges for which we have already
            // examined end i.
        LargeInteger min;
            // The minimum coordinate that has been assigned about this
            // vertex.
        std::deque<EdgeEnd> examine;
        bool broken;
            // Are the matching equations broken about this edge end?
        int end;
        Edge<3>* edge;
        EdgeEnd current;
        std::deque<EdgeEmbedding<3>>::const_iterator eembit, backupit,
            endit, beginit;
        Tetrahedron<3>* tet;
        Perm<4> tetPerm, adjPerm;
        size_t tetIndex, adjIndex;
        LargeInteger expect;
        for (Triangulation<3>::VertexIterator vit = triang->vertices().begin();
                vit != triang->vertices().end(); vit++) {
            usedEdges[0].clear(); usedEdges[1].clear();
            examine.clear();
            broken = false;

            // Pick some triangular disc and set it to zero.
            const VertexEmbedding<3>& vemb = (*vit)->front();
            row = 14 * vemb.tetrahedron()->index() + 2 * vemb.vertex() + orient;
            ans->setElement(row, LargeInteger::zero);

            min = LargeInteger::zero;

            // Mark the three surrounding edge ends for examination.
            for (i=0; i<4; i++) {
                if (i == vemb.vertex())
                    continue;
                edge = vemb.tetrahedron()->edge(
                    Edge<3>::edgeNumber[vemb.vertex()][i]);
                end = vemb.tetrahedron()->edgeMapping(
                    Edge<3>::edgeNumber[vemb.vertex()][i])[0] == i ? 1 : 0;
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
                beginit = current.edge->begin();
                endit = current.edge->end();
                for (eembit = beginit; eembit != endit; eembit++)
                    if (! (*ans)[14 * (*eembit).tetrahedron()->index() +
                            2 * (*eembit).vertices()[current.end] +
                            orient].isInfinite())
                        break;

                // We are now at the first pre-chosen coordinate about this
                // vertex.  Run backwards from here and fill in all the
                // holes.
                backupit = eembit;
                adjPerm = (*eembit).vertices();
                adjIndex = (*eembit).tetrahedron()->index();
                while (eembit != beginit) {
                    eembit--;

                    // Work out the coordinate for the disc type at eembit.
                    tet = (*eembit).tetrahedron();
                    tetPerm = (*eembit).vertices();
                    tetIndex = tet->index();

                    expect =
                        (*ans)[14 * adjIndex + 2 * adjPerm[current.end] +
                            orient] +
                        (*ans)[14 * adjIndex + 8 +
                            2 * quadSeparating[adjPerm[3]][adjPerm[current.end]] +
                            ((adjPerm[current.end] == 0 || adjPerm[3] == 0) ?
                                orient : (1 - orient))] -
                        (*ans)[14 * tetIndex + 8 +
                            2 * quadSeparating[tetPerm[2]][tetPerm[current.end]] +
                            ((tetPerm[current.end] == 0 || tetPerm[2] == 0) ?
                                orient : (1 - orient))];
                    ans->setElement(14 * tetIndex + 2 * tetPerm[current.end] +
                        orient, expect);
                    if (expect < min)
                        min = expect;

                    // Remember to examine the new edge end if appropriate.
                    edge = tet->edge(
                        Edge<3>::edgeNumber[tetPerm[2]][tetPerm[current.end]]);
                    end = tet->edgeMapping(
                        Edge<3>::edgeNumber[tetPerm[2]][tetPerm[current.end]])[0]
                        == tetPerm[2] ? 1 : 0;
                    if (usedEdges[end].insert(edge).second)
                        examine.push_back(EdgeEnd(edge, end));

                    adjPerm = tetPerm;
                    adjIndex = tetIndex;
                }

                // Now move forwards from the original first pre-chosen
                // coordinate and fill in the holes from here onwards,
                // always checking to ensure the
                // matching equations have not been broken.
                eembit = backupit;
                adjPerm = (*eembit).vertices();
                adjIndex = (*eembit).tetrahedron()->index();
                for (eembit++; eembit != endit; eembit++) {
                    // Work out the coordinate for the disc type at eembit.
                    tet = (*eembit).tetrahedron();
                    tetPerm = (*eembit).vertices();
                    tetIndex = tet->index();

                    expect =
                        (*ans)[14 * adjIndex + 2 * adjPerm[current.end] +
                            orient] +
                        (*ans)[14 * adjIndex + 8 +
                            2 * quadSeparating[adjPerm[2]][adjPerm[current.end]] +
                            ((adjPerm[current.end] == 0 || adjPerm[2] == 0) ?
                                orient : (1 - orient))] -
                        (*ans)[14 * tetIndex + 8 +
                            2 * quadSeparating[tetPerm[3]][tetPerm[current.end]] +
                            ((tetPerm[current.end] == 0 || tetPerm[3] == 0) ?
                                orient : (1 - orient))];
                    row = 14 * tetIndex + 2 * tetPerm[current.end] + orient;
                    if ((*ans)[row].isInfinite()) {
                        ans->setElement(row, expect);
                        if (expect < min)
                            min = expect;

                        // Remember to examine the new edge end if appropriate.
                        edge = tet->edge(
                            Edge<3>::edgeNumber[tetPerm[3]][tetPerm[current.end]]);
                        end = tet->edgeMapping(
                            Edge<3>::edgeNumber[tetPerm[3]][tetPerm[current.end]])[0]
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

                    adjPerm = tetPerm;
                    adjIndex = tetIndex;
                }
            }

            // If the matching equations were broken, set every coordinate
            // to infinity.  Otherwise subtract min from every coordinate to
            // make the values as small as possible.
            for (auto& emb : **vit) {
                row = 14 * emb.tetrahedron()->index()
                    + 2 * emb.vertex() + orient;
                if (broken)
                    ans->setElement(row, LargeInteger::infinity);
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

