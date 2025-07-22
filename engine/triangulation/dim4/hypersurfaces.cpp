
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
#include "triangulation/dim4.h"
#include <stack>

namespace regina {

template <int subdim>
std::pair<NormalHypersurface, bool> Triangulation<4>::linkingSurface(
        const Face<4, subdim>& face) const {
    static_assert(0 <= subdim && subdim < 4,
        "Triangulation<4>::linkingSurface() requires a face of dimension "
        "0, 1, 2 or 3.");

    Vector<LargeInteger> coords(15 * size());
    bool thin = true;

    if constexpr (subdim == 0) {
        // Vertex links are trivial to construct.
        for (auto& emb : face)
            coords[15 * emb.simplex()->index() + emb.vertex()] = 1;
    } else {
        // In general, face links can require normalisation.
        //
        // However, this normalisation always involves expanding the face
        // into a larger subcomplex using the following rules:
        // 1) at least two edges of a triangle -> absorb the full triangle;
        // 2) at least two triangles of a tetrahedron -> absorb the full
        //    tetrahedron;
        // 3) at least two tetrahedra of a pentachoron -> absorb the full
        //    pentachoron.
        //
        // The resulting face link is then the frontier of a regular
        // neighbourhood of the resulting subcomplex.
        //
        // So: our main job is to build the subcomplex.
        // We track the subcomplex with an array of booleans for each facial
        // dimension, indicating which of the faces is currently included.

        bool* use0 = new bool[countVertices()];
        bool* use1 = new bool[countEdges()];
        bool* use2 = new bool[countTriangles()];
        bool* use3 = new bool[countTetrahedra()];
        bool* use4 = new bool[size()];

        std::fill(use0, use0 + countVertices(), false);
        std::fill(use1, use1 + countEdges(), false);
        std::fill(use2, use2 + countTriangles(), false);
        std::fill(use3, use3 + countTetrahedra(), false);
        std::fill(use4, use4 + size(), false);

        if constexpr (subdim == 1) {
            use1[face.index()] = true;
            use0[face.vertex(0)->index()] = true;
            use0[face.vertex(1)->index()] = true;
        } else if constexpr (subdim == 2) {
            use2[face.index()] = true;
            for (int i = 0; i < 3; ++i)
                use1[face.edge(i)->index()] = true;
            for (int i = 0; i < 3; ++i)
                use0[face.vertex(i)->index()] = true;
        } else {
            use3[face.index()] = true;
            for (int i = 0; i < 4; ++i)
                use2[face.triangle(i)->index()] = true;
            for (int i = 0; i < 6; ++i)
                use1[face.edge(i)->index()] = true;
            for (int i = 0; i < 4; ++i)
                use0[face.vertex(i)->index()] = true;
        }

        // Faces that were recently incorporated into the subcomplex,
        // for which we need to now check for any follow-up larger faces
        // that will need to be incorporated also as a result:
        std::stack<const Edge<4>*> process1;
        std::stack<const Triangle<4>*> process2;
        std::stack<const Tetrahedron<4>*> process3;

        if constexpr (subdim == 1) {
            process1.push(std::addressof(face));
        } else if constexpr (subdim == 2) {
            process2.push(std::addressof(face));
            for (int i = 0; i < 3; ++i)
                process1.push(face.edge(i));
        } else {
            process3.push(std::addressof(face));
            for (int i = 0; i < 4; ++i)
                process2.push(face.triangle(i));
            for (int i = 0; i < 6; ++i)
                process1.push(face.edge(i));
        }

        while (true) {
            if (! process3.empty()) {
                const Tetrahedron<4>* t = process3.top();
                process3.pop();

                for (const auto& emb : *t) {
                    const Pentachoron<4>* pent = emb.pentachoron();
                    if (use4[pent->index()])
                        continue;

                    int found = 0;
                    for (int j = 0; j < 5; ++j)
                        if (use3[pent->tetrahedron(j)->index()]) {
                            ++found;
                            if (found == 2)
                                break;
                        }

                    if (found == 2) {
                        // Absorb the entire pentachoron.
                        thin = false;
                        use4[pent->index()] = true;

                        for (int j = 0; j < 5; ++j) {
                            const Tetrahedron<4>* next = pent->tetrahedron(j);
                            if (! use3[next->index()]) {
                                use3[next->index()] = true;
                                process3.push(next);
                            }
                        }

                        for (int j = 0; j < 10; ++j) {
                            const Triangle<4>* next = pent->triangle(j);
                            if (! use2[next->index()]) {
                                use2[next->index()] = true;
                                process2.push(next);
                            }
                        }

                        for (int j = 0; j < 10; ++j) {
                            const Edge<4>* next = pent->edge(j);
                            if (! use1[next->index()]) {
                                use1[next->index()] = true;
                                process1.push(next);
                            }
                        }
                    }
                }

                continue;
            }

            if (! process2.empty()) {
                const Triangle<4>* t = process2.top();
                process2.pop();

                bool bothSides = t->isBoundary();
                for (const auto& emb : *t) {
                    for (int side = 0; side < 2; ++side) {
                        if (side == 1) {
                            if (! bothSides)
                                continue; // side 0 is enough

                            // We do both sides for this embedding (which is
                            // the first in the list), but *only* this
                            // embedding.
                            bothSides = false;
                        }

                        const Tetrahedron<4>* tet = emb.pentachoron()->
                            tetrahedron(emb.vertices()[side == 0 ? 3 : 4]);
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
                            thin = false;
                            use3[tet->index()] = true;
                            process3.push(tet);

                            for (int j = 0; j < 4; ++j) {
                                const Triangle<4>* next = tet->triangle(j);
                                if (! use2[next->index()]) {
                                    use2[next->index()] = true;
                                    process2.push(next);
                                }
                            }

                            for (int j = 0; j < 6; ++j) {
                                const Edge<4>* next = tet->edge(j);
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

            if (! process1.empty()) {
                const Edge<4>* e = process1.top();
                process1.pop();

                // We need to check all *triangles* that contain e.
                // The code below is quite inefficient, in that it checks far
                // too many triangles, and moreover checks the same triangles
                // multiple times.  It should probably be improved.
                for (const auto& emb : *e) {
                    const Pentachoron<4>* pent = emb.pentachoron();
                    // For now, just check *all* triangles of pent.
                    for (int i = 0; i < 10; ++i) {
                        const Triangle<4>* triangle = pent->triangle(i);
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
                            thin = false;
                            use2[triangle->index()] = true;
                            process2.push(triangle);

                            for (int j = 0; j < 3; ++j) {
                                const Edge<4>* next = triangle->edge(j);
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

        size_t pentIndex = 0;
        for (const Pentachoron<4>* pent : pentachora()) {
            if (use4[pentIndex])
                goto donePent;

            for (int j = 0; j < 5; ++j)
                if (use3[pent->tetrahedron(j)->index()]) {
                    if (use0[pent->vertex(j)->index()])
                        coords[15 * pentIndex + j] = 2;
                    else
                        coords[15 * pentIndex + j] = 1;

                    goto donePent;
                }

            for (int j = 0; j < 10; ++j)
                if (use2[pent->triangle(j)->index()]) {
                    if (use1[pent->edge(j)->index()])
                        coords[15 * pentIndex + 5 + j] = 2;
                    else {
                        coords[15 * pentIndex + 5 + j] = 1;
                        for (int end = 0; end < 2; ++end) {
                            int v = Edge<4>::edgeVertex[j][end];
                            if (use0[pent->vertex(v)->index()])
                                coords[15 * pentIndex + v] = 1;
                        }
                    }

                    goto donePent;
                }

            // We are down to just edges and/or vertices.
            // There could be multiple disjoint edges, or multiple vertices.
            {
                bool skipVertex[5] = { false, false, false, false, false };
                for (int j = 0; j < 10; ++j)
                    if (use1[pent->edge(j)->index()]) {
                        coords[15 * pentIndex + 5 + j] = 1;
                        skipVertex[Edge<4>::edgeVertex[j][0]] = true;
                        skipVertex[Edge<4>::edgeVertex[j][1]] = true;
                    }
                for (int j = 0; j < 5; ++j)
                    if ((! skipVertex[j]) && use0[pent->vertex(j)->index()])
                        coords[15 * pentIndex + j] = 1;
            }

donePent:
            ++pentIndex;
        }

        delete[] use0;
        delete[] use1;
        delete[] use2;
        delete[] use3;
        delete[] use4;
    }

    return { NormalHypersurface(*this, HyperCoords::Standard,
        std::move(coords)), thin };
}

// Instantiate linkingSurface() for all possible template arguments,
// so that the implementation can stay out of the headers.
template std::pair<NormalHypersurface, bool>
    Triangulation<4>::linkingSurface<0>(const Face<4, 0>&) const;
template std::pair<NormalHypersurface, bool>
    Triangulation<4>::linkingSurface<1>(const Face<4, 1>&) const;
template std::pair<NormalHypersurface, bool>
    Triangulation<4>::linkingSurface<2>(const Face<4, 2>&) const;
template std::pair<NormalHypersurface, bool>
    Triangulation<4>::linkingSurface<3>(const Face<4, 3>&) const;

} // namespace regina

