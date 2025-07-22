
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include "hypersurface/normalhypersurface.h"
#include "maths/matrix.h"
#include "maths/rational.h"
#include "triangulation/dim4.h"

namespace regina {

namespace {
    /**
     * A structure representing one corner of a triangle.
     */
    struct TriangleCorner {
        Triangle<4>* tri; /**< The triangle under consideration. */
        int corner; /**< 0, 1 or 2, indicating which corner of the triangle. */
    };
}

HyperEncoding NormalHypersurface::reconstructTetrahedra(
        const Triangulation<4>& tri, Vector<LargeInteger>& vector,
        HyperEncoding enc) {
    // We are offering this function to the public, so do a sanity check on enc.
    // For our own code this is redundant but it's just one bit-flag test.
    if (enc.storesTetrahedra())
        return enc;

    const HyperEncoding newEnc = enc.withTetrahedra();
    const int newBlock = newEnc.block();
    const size_t newSize = newBlock * tri.size();
    Vector<LargeInteger> ans(newSize);

    // Set every tetrahedron coordinate in the answer to infinity.
    // For coordinates about vertices not enjoying infinitely many
    // tetrahedron pices, infinity will mean "unknown".
    for (size_t row = 0; row < newSize; row += newBlock)
        for (int i = 0; i < 5; ++i)
            ans[row + i].makeInfinite();

    // Copy across the other (non-tetrahedron) coordinates.
    for (size_t row = 0; newBlock * row < newSize; ++row)
        for (int i = 0; i < enc.block(); ++i)
            ans[newBlock * row + 5 + i] = vector[enc.block() * row + i];

    // Record which triangle corners we have already examined.
    bool* used = new bool[3 * tri.countTriangles()];
    std::fill(used, used + 3 * tri.countTriangles(), false);

    // Prepare a stack of triangle corners that we are ready to examine.
    auto* examine = new TriangleCorner[3 * tri.countTriangles()];
    size_t nExamine = 0;

    // Run through the vertices and fix the tetrahedron coordinates
    // about each vertex in turn.
    for (Vertex<4>* v : tri.vertices()) {
        nExamine = 0;
        bool broken = false; // Are the equations broken around this vertex?

        // Pick some tetrahedron disc and set it to zero.
        const VertexEmbedding<4>& vemb = v->front();
        ans[newBlock * vemb.pentachoron()->index() + vemb.vertex()] = 0;
        LargeInteger min; // The minimum tet coordinate around this vertex.

        // Mark the six surrounding triangle corners for examination.
        for (int i = 0; i < 5; ++i) {
            if (i == vemb.vertex())
                continue;
            for (int j = i + 1; j < 5; ++j) {
                if (j == vemb.vertex())
                    continue;
                TriangleCorner c {
                    vemb.pentachoron()->triangle(
                        Triangle<4>::triangleNumber[vemb.vertex()][i][j]),
                    vemb.pentachoron()->triangleMapping(
                        Triangle<4>::triangleNumber[vemb.vertex()][i][j]).
                        pre(vemb.vertex())
                };
                size_t usedIndex = 3 * c.tri->index() + c.corner;
                if (! used[usedIndex]) {
                    used[usedIndex] = true;
                    examine[nExamine++] = c;
                }
            }
        }

        // Run a depth-first search through the triangle corners that meet this
        // vertex link.
        while ((! broken) && nExamine) {
            TriangleCorner current = examine[--nExamine];

            // Run around this triangle corner.
            // We know there is a pre-chosen coordinate somewhere; run
            // forwards and find this.
            const auto beginit = current.tri->begin();
            const auto endit = current.tri->end();

            auto eembit = beginit;
            for ( ; eembit != endit; ++eembit)
                if (! ans[newBlock * eembit->pentachoron()->index() +
                        eembit->vertices()[current.corner]].isInfinite())
                    break;

            // We are now at the first pre-chosen coordinate about this
            // vertex.  Run backwards from here and fill in all the holes.
            Perm<5> adjPerm = eembit->vertices();
            size_t adjPos = newBlock * eembit->pentachoron()->index();

            auto backupit = eembit;
            while (eembit != beginit) {
                --eembit;

                // Work out the coordinate for the disc type at eembit.
                Pentachoron<4>* pent = eembit->pentachoron();
                Perm<5> pentPerm = eembit->vertices();
                size_t pentPos = newBlock * pent->index();

                LargeInteger expect =
                    ans[adjPos + adjPerm[current.corner]]
                    + ans[adjPos + 5 + Edge<4>::edgeNumber
                        [adjPerm[4]][adjPerm[current.corner]]]
                    - ans[pentPos + 5 + Edge<4>::edgeNumber
                        [pentPerm[3]][pentPerm[current.corner]]];

                ans[pentPos + pentPerm[current.corner]] = expect;
                if (expect < min)
                    min = expect;

                // Remember to examine the new edge end if appropriate.
                {
                    int nextTri = Triangle<4>::triangleNumber
                        [pentPerm[(current.corner + 1) % 3]]
                        [pentPerm[3]][pentPerm[current.corner]];
                    TriangleCorner c {
                        pent->triangle(nextTri),
                        pent->triangleMapping(nextTri).
                            pre(pentPerm[current.corner])
                    };
                    size_t usedIndex = 3 * c.tri->index() + c.corner;
                    if (! used[usedIndex]) {
                        used[usedIndex] = true;
                        examine[nExamine++] = c;
                    }
                }
                {
                    int nextTri = Triangle<4>::triangleNumber
                        [pentPerm[(current.corner + 2) % 3]]
                        [pentPerm[3]][pentPerm[current.corner]];
                    TriangleCorner c {
                        pent->triangle(nextTri),
                        pent->triangleMapping(nextTri).
                            pre(pentPerm[current.corner])
                    };
                    size_t usedIndex = 3 * c.tri->index() + c.corner;
                    if (! used[usedIndex]) {
                        used[usedIndex] = true;
                        examine[nExamine++] = c;
                    }
                }

                adjPerm = pentPerm;
                adjPos = pentPos;
            }

            // Now move forwards from the original first pre-chosen
            // coordinate, again filling in the holes.
            eembit = backupit;
            adjPerm = eembit->vertices();
            adjPos = newBlock * eembit->pentachoron()->index();

            for (++eembit; eembit != endit; ++eembit) {
                // Work out the coordinate for the disc type at eembit.
                Pentachoron<4>* pent = eembit->pentachoron();
                Perm<5> pentPerm = eembit->vertices();
                size_t pentPos = newBlock * pent->index();

                LargeInteger expect =
                    ans[adjPos + adjPerm[current.corner]]
                    + ans[adjPos + 5 + Edge<4>::edgeNumber
                        [adjPerm[3]][adjPerm[current.corner]]]
                    - ans[pentPos + 5 + Edge<4>::edgeNumber
                        [pentPerm[4]][pentPerm[current.corner]]];

                size_t row = pentPos + pentPerm[current.corner];
                if (ans[row].isInfinite()) {
                    ans[row] = expect;
                    if (expect < min)
                        min = expect;

                    // Remember to examine the new edge end if appropriate.
                    {
                        int nextTri = Triangle<4>::triangleNumber
                            [pentPerm[(current.corner + 1) % 3]]
                            [pentPerm[4]][pentPerm[current.corner]];
                        TriangleCorner c {
                            pent->triangle(nextTri),
                            pent->triangleMapping(nextTri).
                                pre(pentPerm[current.corner])
                        };
                        size_t usedIndex = 3 * c.tri->index() + c.corner;
                        if (! used[usedIndex]) {
                            used[usedIndex] = true;
                            examine[nExamine++] = c;
                        }
                    }
                    {
                        int nextTri = Triangle<4>::triangleNumber
                            [pentPerm[(current.corner + 2) % 3]]
                            [pentPerm[4]][pentPerm[current.corner]];
                        TriangleCorner c {
                            pent->triangle(nextTri),
                            pent->triangleMapping(nextTri).
                                pre(pentPerm[current.corner])
                        };
                        size_t usedIndex = 3 * c.tri->index() + c.corner;
                        if (! used[usedIndex]) {
                            used[usedIndex] = true;
                            examine[nExamine++] = c;
                        }
                    }
                } else {
                    // This coordinate has already been set.
                    // Make sure it's the same value!
                    if (ans[row] != expect) {
                        broken = true;
                        break;
                    }
                }

                adjPerm = pentPerm;
                adjPos = pentPos;
            }
        }

        // If the matching equations were broken, set every coordinate
        // to infinity.  Otherwise subtract min from every coordinate to
        // make the values as small as possible.
        for (const auto& emb : *v) {
            size_t row = newBlock * emb.pentachoron()->index() + emb.vertex();
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

