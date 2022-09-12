
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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
#include <stack>

namespace regina {

NormalSurface Face<3, 1>::linkingSurface() const {
    const Triangulation<3>& tri = triangulation();

    // In general, edge links can require normalisation.
    //
    // However, this normalisation always involves expanding the edge
    // into a larger subcomplex using the following rules:
    // 1) at least two edges of a triangle -> absorb the full triangle
    // 2) at least two triangles of a tetrahedron -> absorb the full tetrahedron
    //
    // Strictly speaking, only rule (1) is necessary, and we really only need
    // to track the edges of the subcomplex; however, we track triangles and
    // tetrahedra also to simplify some other parts of the implementation.
    //
    // The resulting edge link is then the frontier of a regular neighbourhood
    // of the resulting subcomplex.
    //
    // So: our main job is to build the subcomplex.
    // We track the subcomplex with an array of booleans for each facial
    // dimension, indicating which of the faces is currently included.

    bool* use0 = new bool[tri.countVertices()];
    bool* use1 = new bool[tri.countEdges()];
    bool* use2 = new bool[tri.countTriangles()];
    bool* use3 = new bool[tri.size()];

    std::fill(use0, use0 + tri.countVertices(), false);
    std::fill(use1, use1 + tri.countEdges(), false);
    std::fill(use2, use2 + tri.countTriangles(), false);
    std::fill(use3, use3 + tri.size(), false);

    use1[index()] = true;
    use0[vertex(0)->index()] = true;
    use0[vertex(1)->index()] = true;

    // Edges/triangles that were recently incorporated into the subcomplex,
    // for which we need to now check for any follow-up triangles/tetrahedra
    // that will need to be incorporated also as a result:
    std::stack<const Edge<3>*> process1;
    std::stack<const Triangle<3>*> process2;

    process1.push(this);

    while (true) {
        if (! process2.empty()) {
            const Triangle<3>* t = process2.top();
            process2.pop();

            for (const auto& emb : *t) {
                const Tetrahedron<3>* tet = emb.tetrahedron();
                if (use3[tet->index()])
                    continue;

                int found = 0;
                for (int j = 0; j < 4; ++j)
                    if (use2[tet->triangle(j)->index()]) {
                        ++found;
                        if (found == 2)
                            break;
                    }

                if (found == 2) {
                    // Absorb the entire tetrahedron.
                    use3[tet->index()] = true;

                    for (int j = 0; j < 4; ++j) {
                        const Triangle<3>* next = tet->triangle(j);
                        if (! use2[next->index()]) {
                            use2[next->index()] = true;
                            process2.push(next);
                        }
                    }

                    for (int j = 0; j < 6; ++j) {
                        const Edge<3>* next = tet->edge(j);
                        if (! use1[next->index()]) {
                            use1[next->index()] = true;
                            process1.push(next);
                        }
                    }
                }
            }

            continue;
        }

        if (! process1.empty()) {
            const Edge<3>* e = process1.top();
            process1.pop();

            bool bothSides = e->isBoundary();
            for (const auto& emb : *e) {
                for (int side = 0; side < 2; ++side) {
                    if (side == 1) {
                        if (! bothSides)
                            continue; // side 0 is enough

                        // We do both sides for this embedding (which is
                        // the first in the list), but *only* this embedding.
                        bothSides = false;
                    }

                    const Triangle<3>* triangle = emb.tetrahedron()->triangle(
                        emb.vertices()[side == 0 ? 2 : 3]);
                    if (use2[triangle->index()])
                        continue;

                    int found = 0;
                    for (int j = 0; j < 3; ++j)
                        if (use1[triangle->edge(j)->index()]) {
                            ++found;
                            if (found == 2)
                                break;
                        }

                    if (found == 2) {
                        // Absorb the entire triangle.
                        use2[triangle->index()] = true;
                        process2.push(triangle);

                        for (int j = 0; j < 3; ++j) {
                            const Edge<3>* next = triangle->edge(j);
                            if (! use1[next->index()]) {
                                use1[next->index()] = true;
                                process1.push(next);
                            }
                        }
                    }
                }
            }

            continue;
        }

        break;
    }

    Vector<LargeInteger> coords(7 * tri.size());

    size_t tetIndex = 0;
    for (const Tetrahedron<3>* tet : tri.tetrahedra()) {
        if (use3[tetIndex])
            goto doneTet;

        for (int j = 0; j < 4; ++j)
            if (use2[tet->triangle(j)->index()]) {
                if (use0[tet->vertex(j)->index()])
                    coords[7 * tetIndex + j] = 2;
                else
                    coords[7 * tetIndex + j] = 1;

                goto doneTet;
            }

        for (int j = 0; j < 6; ++j)
            if (use1[tet->edge(j)->index()]) {
                // Note: quad type i does not intersect edge i.
                int quad = (j < 3 ? j : 5 - j);
                if (use1[tet->edge(5 - j)->index()])
                    coords[7 * tetIndex + 4 + quad] = 2;
                else {
                    coords[7 * tetIndex + 4 + quad] = 1;

                    for (int k = 0; k < 2; ++k) {
                        int v = edgeVertex[5 - j][k];
                        if (use0[tet->vertex(v)->index()])
                            coords[7 * tetIndex + v] = 1;
                    }
                }

                goto doneTet;
            }

        for (int j = 0; j < 4; ++j)
            if (use0[tet->vertex(j)->index()])
                coords[7 * tetIndex + j] = 1;

doneTet:
        ++tetIndex;
    }

    delete[] use0;
    delete[] use1;
    delete[] use2;
    delete[] use3;

    return NormalSurface(tri, NS_STANDARD, std::move(coords));
}

} // namespace regina
