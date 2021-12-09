
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

#include "surface/normalsurface.h"
#include "maths/matrix.h"
#include "maths/rational.h"
#include "triangulation/dim3.h"

namespace regina {

namespace {
    /**
     * A structure representing one end of an edge.
     */
    struct EdgeEnd {
        Edge<3>* edge; /**< The edge under consideration. */
        int end; /**< Either 0 or 1, indicating which end of the edge. */
    };
}

NormalEncoding NormalSurface::reconstructTriangles(
        const Triangulation<3>& tri, Vector<LargeInteger>& vector,
        NormalEncoding enc) {
    // We are offering this function to the public, so do a sanity check on enc.
    // For our own code this is redundant but it's just one bit-flag test.
    if (enc.storesTriangles())
        return enc;

    const NormalEncoding newEnc = enc.withTriangles();
    const int newBlock = newEnc.block();
    const size_t newSize = newBlock * tri.size();
    Vector<LargeInteger> ans(newSize);

    // Set every triangular coordinate in the answer to infinity.
    // For coordinates about vertices not enjoying infinitely many discs,
    // infinity will mean "unknown".
    for (size_t row = 0; row < newSize; row += newBlock)
        for (int i = 0; i < 4; ++i)
            ans[row + i].makeInfinite();

    // Copy across the other (non-triangular) coordinates.
    for (size_t row = 0; newBlock * row < newSize; ++row)
        for (int i = 0; i < enc.block(); ++i)
            ans[newBlock * row + 4 + i] = vector[enc.block() * row + i];

    // Record which edge ends we have already examined.
    bool* used = new bool[2 * tri.countEdges()];
    std::fill(used, used + 2 * tri.countEdges(), false);

    // Prepare a stack of edge ends that we are ready to examine.
    auto* examine = new EdgeEnd[2 * tri.countEdges()];
    size_t nExamine = 0;

    // Run through the vertices and fix the triangular coordinates
    // about each vertex in turn.
    for (Vertex<3>* v : tri.vertices()) {
        nExamine = 0;
        bool broken = false; // Are the equations broken around this vertex?

        // Pick some triangular disc and set it to zero.
        const VertexEmbedding<3>& vemb = v->front();
        ans[newBlock * vemb.tetrahedron()->index() + vemb.vertex()] = 0;
        LargeInteger min; // The minimum triangle coordinate around this vertex.

        // Mark the three surrounding edge ends for examination.
        for (int i=0; i<4; i++) {
            if (i == vemb.vertex())
                continue;
            EdgeEnd e {
                vemb.tetrahedron()->edge(Edge<3>::edgeNumber[vemb.vertex()][i]),
                vemb.tetrahedron()->edgeMapping(
                    Edge<3>::edgeNumber[vemb.vertex()][i])[0] == i ? 1 : 0
            };
            size_t usedIndex = 2 * e.edge->index() + e.end;
            if (! used[usedIndex]) {
                used[usedIndex] = true;
                examine[nExamine++] = e;
            }
        }

        // Run a depth-first search through the edge ends that meet this
        // vertex link.
        while ((! broken) && nExamine) {
            EdgeEnd current = examine[--nExamine];

            // Run around this edge end.
            // We know there is a pre-chosen coordinate somewhere; run
            // forwards and find this.
            const auto beginit = current.edge->begin();
            const auto endit = current.edge->end();

            auto eembit = beginit;
            for ( ; eembit != endit; ++eembit)
                if (! ans[newBlock * eembit->tetrahedron()->index() +
                        eembit->vertices()[current.end]].isInfinite())
                    break;

            // We are now at the first pre-chosen coordinate about this
            // vertex.  Run backwards from here and fill in all the holes.
            Perm<4> adjPerm = eembit->vertices();
            size_t adjPos = newBlock * eembit->tetrahedron()->index();

            auto backupit = eembit;
            while (eembit != beginit) {
                --eembit;

                // Work out the coordinate for the disc type at eembit.
                Tetrahedron<3>* tet = eembit->tetrahedron();
                Perm<4> tetPerm = eembit->vertices();
                size_t tetPos = newBlock * tet->index();

                LargeInteger expect =
                    ans[adjPos + adjPerm[current.end]]
                    + ans[adjPos + 4 +
                        quadSeparating[adjPerm[3]][adjPerm[current.end]]]
                    - ans[tetPos + 4 +
                        quadSeparating[tetPerm[2]][tetPerm[current.end]]];
                if (enc.storesOctagons()) {
                    expect = expect
                        + ans[adjPos + 7 + quadMeeting
                            [adjPerm[3]][adjPerm[current.end]][0]]
                        + ans[adjPos + 7 + quadMeeting
                            [adjPerm[3]][adjPerm[current.end]][1]]
                        - ans[tetPos + 7 + quadMeeting
                            [tetPerm[2]][tetPerm[current.end]][0]]
                        - ans[tetPos + 7 + quadMeeting
                            [tetPerm[2]][tetPerm[current.end]][1]];
                }
                ans[tetPos + tetPerm[current.end]] = expect;
                if (expect < min)
                    min = expect;

                // Remember to examine the new edge end if appropriate.
                EdgeEnd e {
                    tet->edge(Edge<3>::edgeNumber[tetPerm[2]]
                        [tetPerm[current.end]]),
                    tet->edgeMapping(Edge<3>::edgeNumber[tetPerm[2]]
                        [tetPerm[current.end]])[0] == tetPerm[2] ? 1 : 0
                };
                size_t usedIndex = 2 * e.edge->index() + e.end;
                if (! used[usedIndex]) {
                    used[usedIndex] = true;
                    examine[nExamine++] = e;
                }

                adjPerm = tetPerm;
                adjPos = tetPos;
            }

            // Now move forwards from the original first pre-chosen
            // coordinate, again filling in the holes.
            eembit = backupit;
            adjPerm = eembit->vertices();
            adjPos = newBlock * eembit->tetrahedron()->index();

            for (++eembit; eembit != endit; ++eembit) {
                // Work out the coordinate for the disc type at eembit.
                Tetrahedron<3>* tet = eembit->tetrahedron();
                Perm<4> tetPerm = eembit->vertices();
                size_t tetPos = newBlock * tet->index();

                LargeInteger expect =
                    ans[adjPos + adjPerm[current.end]]
                    + ans[adjPos + 4 +
                        quadSeparating[adjPerm[2]][adjPerm[current.end]]]
                    - ans[tetPos + 4 +
                        quadSeparating[tetPerm[3]][tetPerm[current.end]]];
                if (enc.storesOctagons()) {
                    expect = expect
                        + ans[adjPos + 7 + quadMeeting
                            [adjPerm[2]][adjPerm[current.end]][0]]
                        + ans[adjPos + 7 + quadMeeting
                            [adjPerm[2]][adjPerm[current.end]][1]]
                        - ans[tetPos + 7 + quadMeeting
                            [tetPerm[3]][tetPerm[current.end]][0]]
                        - ans[tetPos + 7 + quadMeeting
                            [tetPerm[3]][tetPerm[current.end]][1]];
                }
                size_t row = tetPos + tetPerm[current.end];
                if (ans[row].isInfinite()) {
                    ans[row] = expect;
                    if (expect < min)
                        min = expect;

                    // Remember to examine the new edge end if appropriate.
                    EdgeEnd e {
                        tet->edge(Edge<3>::edgeNumber[tetPerm[3]]
                            [tetPerm[current.end]]),
                        tet->edgeMapping(Edge<3>::edgeNumber[tetPerm[3]]
                            [tetPerm[current.end]])[0] == tetPerm[3] ? 1 : 0
                    };
                    size_t usedIndex = 2 * e.edge->index() + e.end;
                    if (! used[usedIndex]) {
                        used[usedIndex] = true;
                        examine[nExamine++] = e;
                    }
                } else {
                    // This coordinate has already been set.
                    // Make sure it's the same value!
                    if (ans[row] != expect) {
                        broken = true;
                        break;
                    }
                }

                adjPerm = tetPerm;
                adjPos = tetPos;
            }
        }

        // If the matching equations were broken, set every coordinate
        // to infinity.  Otherwise subtract min from every coordinate to
        // make the values as small as possible.
        for (const auto& emb : *v) {
            size_t row = newBlock * emb.tetrahedron()->index() + emb.vertex();
            if (broken)
                ans[row].makeInfinite();
            else
                ans[row] -= min;
        }
    }

    delete[] used;

    // Note that there should be no need to remove common factors since
    // the quad coordinates have not changed and in theory they already
    // had gcd=1.
    vector = std::move(ans);
    return newEnc;
}

} // namespace regina

