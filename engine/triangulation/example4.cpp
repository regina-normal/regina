
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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

#include "link/link.h"
#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"
#include "triangulation/example3.h"
#include "triangulation/example4.h"

namespace regina {

Triangulation<4> Example<4>::cp2() {
    // Built by Rhuaidi Burke using Katie from a Kirby diagram of the
    // standard CP^2.
    return Triangulation<4>::fromGluings(4, {
        { 0, 0, 0, {2,3,1,0,4} }, { 0, 1, 0, {0,3,2,1,4} },
        { 0, 4, 1, {0,1,2,4,3} }, { 1, 0, 1, {2,4,1,3,0} },
        { 1, 1, 2, {0,1,2,4,3} }, { 1, 4, 2, {1,0,3,4,2} },
        { 2, 0, 2, {3,1,2,0,4} }, { 2, 4, 3, {0,1,2,4,3} },
        { 3, 0, 3, {4,1,2,3,0} }, { 3, 1, 3, {1,2,4,3,0} }});
}

Triangulation<4> Example<4>::s2xs2() {
    // Built by Rhuaidi Burke using Katie from a Kirby diagram of the
    // standard S2 x S2.
    return Triangulation<4>::fromGluings(6, {
        { 0, 0, 0, {4,1,2,3,0} }, { 0, 1, 0, {0,2,1,3,4} },
        { 0, 3, 1, {0,1,2,4,3} }, { 1, 0, 2, {0,1,2,4,3} },
        { 1, 1, 3, {0,1,2,4,3} }, { 1, 2, 2, {4,2,1,0,3} },
        { 1, 3, 3, {0,1,2,4,3} }, { 2, 2, 2, {1,2,4,3,0} },
        { 2, 3, 4, {0,1,2,4,3} }, { 3, 0, 3, {2,4,1,3,0} },
        { 3, 3, 5, {0,1,2,4,3} }, { 4, 0, 4, {1,0,2,3,4} },
        { 4, 2, 4, {1,2,3,0,4} }, { 5, 0, 5, {2,3,1,0,4} },
        { 5, 1, 5, {0,3,2,1,4} }});
}

Triangulation<4> Example<4>::s2xs2Twisted() {
    // Built by Rhuaidi Burke.
    return Triangulation<4>::fromGluings(6, {
        { 0, 0, 0, {2,3,1,0,4} }, { 0, 1, 0, {0,3,2,1,4} },
        { 0, 4, 1, {0,1,2,4,3} }, { 1, 0, 1, {2,4,1,3,0} },
        { 1, 1, 2, {0,1,2,4,3} }, { 1, 4, 2, {1,0,3,4,2} },
        { 2, 0, 2, {3,1,2,0,4} }, { 2, 4, 5, {0,1,2,4,3} },
        { 3, 0, 3, {2,4,1,3,0} }, { 3, 1, 3, {0,4,2,3,1} },
        { 3, 3, 4, {0,1,2,4,3} }, { 4, 0, 4, {2,3,1,0,4} },
        { 4, 1, 5, {0,1,2,4,3} }, { 4, 3, 5, {1,0,4,2,3} },
        { 5, 0, 5, {4,1,2,3,0} }});
}

Triangulation<4> Example<4>::rp4() {
    Triangulation<4> ans;

    // Thanks Ryan, you rock. :)
    auto [p, q, r, s] = ans.newPentachora<4>();
    p->join(0, s, Perm<5>(1,0,3,2,4));
    p->join(1, s, Perm<5>(1,0,3,2,4));
    p->join(2, q, Perm<5>());
    p->join(3, q, Perm<5>());
    p->join(4, r, Perm<5>());
    q->join(0, r, Perm<5>(1,0,3,2,4));
    q->join(1, r, Perm<5>(1,0,3,2,4));
    q->join(4, s, Perm<5>());
    r->join(2, s, Perm<5>());
    r->join(3, s, Perm<5>());

    return ans;
}

Triangulation<4> Example<4>::fourTorus() {
    // This was constructed and simplified by Rhuaidi Burke by taking an
    // S1-bundle over the 3-torus.
    return Triangulation<4>::fromGluings(24, {
        { 0, 0, 3, {0,1,2,4,3} }, { 0, 1, 4, {0,1,2,4,3} },
        { 0, 2, 14, {1,3,0,2,4} }, { 0, 3, 13, {1,3,4,0,2} },
        { 0, 4, 5, {0,1,2,4,3} }, { 1, 0, 3, {4,1,2,3,0} },
        { 1, 1, 15, {3,4,1,0,2} }, { 1, 2, 8, {0,1,2,4,3} },
        { 1, 3, 19, {3,2,0,1,4} }, { 1, 4, 13, {0,3,4,2,1} },
        { 2, 0, 21, {0,1,2,4,3} }, { 2, 1, 15, {1,0,4,2,3} },
        { 2, 2, 19, {0,1,2,4,3} }, { 2, 3, 14, {2,3,4,1,0} },
        { 2, 4, 9, {0,1,2,4,3} }, { 3, 1, 11, {0,1,2,4,3} },
        { 3, 2, 9, {3,1,4,2,0} }, { 3, 3, 10, {3,1,4,2,0} },
        { 4, 0, 11, {0,1,2,4,3} }, { 4, 2, 8, {0,3,4,2,1} },
        { 4, 3, 19, {3,4,1,0,2} }, { 4, 4, 21, {3,4,1,0,2} },
        { 5, 0, 12, {0,1,2,4,3} }, { 5, 1, 9, {2,0,3,1,4} },
        { 5, 2, 20, {3,2,0,1,4} }, { 5, 4, 6, {0,1,2,4,3} },
        { 6, 0, 17, {1,0,2,3,4} }, { 6, 1, 8, {4,0,3,2,1} },
        { 6, 2, 22, {1,3,0,2,4} }, { 6, 4, 13, {1,3,4,0,2} },
        { 7, 0, 23, {0,3,4,2,1} }, { 7, 1, 11, {2,4,3,0,1} },
        { 7, 2, 8, {0,1,3,2,4} }, { 7, 3, 18, {1,4,2,0,3} },
        { 7, 4, 10, {4,2,0,3,1} }, { 8, 1, 12, {4,2,1,0,3} },
        { 9, 1, 17, {4,2,3,1,0} }, { 9, 2, 16, {4,3,1,2,0} },
        { 10, 0, 14, {3,0,2,4,1} }, { 10, 3, 12, {3,1,0,4,2} },
        { 10, 4, 16, {4,3,1,2,0} }, { 11, 2, 17, {1,2,0,4,3} },
        { 11, 3, 20, {2,4,1,3,0} }, { 12, 1, 15, {2,1,4,0,3} },
        { 12, 3, 21, {4,0,2,1,3} }, { 13, 3, 16, {3,4,0,2,1} },
        { 13, 4, 18, {1,0,3,4,2} }, { 14, 2, 18, {2,0,1,4,3} },
        { 14, 4, 20, {0,3,4,2,1} }, { 15, 2, 16, {0,1,4,3,2} },
        { 15, 3, 22, {0,4,3,1,2} }, { 16, 3, 23, {0,3,2,1,4} },
        { 17, 3, 23, {0,2,1,3,4} }, { 17, 4, 19, {2,1,0,3,4} },
        { 18, 3, 22, {1,2,0,4,3} }, { 18, 4, 19, {0,1,2,4,3} },
        { 20, 2, 21, {0,1,3,2,4} }, { 20, 4, 22, {0,4,3,1,2} },
        { 21, 4, 23, {0,3,1,4,2} }, { 22, 3, 23, {0,1,2,4,3} }});
}

Triangulation<4> Example<4>::k3() {
    // This is (with much work by multiple authors) derived from Spreer's
    // 17-vertex standard K3 simplicial complex.
    // Rhuaidi Burke has managed to get this down to 60 pentachora thus far.
    // The following manifold is oriented and has signature +16, not -16:
    // if we prefer -16 then we will need to reflect before returning.
    // return Triangulation<4>::fromSig("8hqOqWOyX4yzPrjrIHbskcJIklsdlRAAkZ666sJBCKZ8mmenveeKevfvfLmTT9uDDLL1199fgoowwEE2+UU+ghppxxFF-F4Z-ppj3836YH+OFEnhHMpJIrGrsqRJH7kdgClyMUE2XzDqfIzamI1EqIXdyOylKrDDjlCJ5C7Z84A00SqHLf-2cG2+uWE4abfI2gTtppsGXEqkisy7d");

    // This triangulation is derived from a Kirby diagram of the standard K3
    // surface, constructed and simplified by Rhuaidi Burke using Katie and the
    // "Up-Down-Simplify" heuristic.
    //
    // This triangulation is already oriented (and the signature below is a
    // second-generation signature, which means it preserves the orientation).
    // Moreover, it has the desired orientation (its signature is -16, not +16).
    return Triangulation<4>::fromSig("2haiyyyGGO4ijjjzrrXHHr65rAQsAQIIAdRBZRZlldmSJJ7RmuuCCKKCDLT98efnnvvLLT11foowwEEMMUU+FuT9-A-qPBUDYUJfCo3iVzdaaggjxNfOXALneeaCmAZOPcO4aexBsxEmmGEEBs14ZmtbmdeaqbOQqqQmWdzhIkmqTQsWdTNdPcIkDa0r");
}

Triangulation<4> Example<4>::cappellShaneson() {
    // We use a construction that is isomorphic to the one described in
    // arXiv:1109.3899, but we relabel the tetrahedra so that the
    // triangulation is oriented.
    return Triangulation<4>::fromGluings(2, {
        { 0, 0, 0, {3,0,1,2,4} }, { 0, 1, 1, {0,4,1,2,3} },
        { 0, 2, 1, {1,2,0,4,3} }, { 0, 4, 1, {0,1,4,3,2} },
        { 1, 1, 1, {0,3,1,4,2} }});
}

namespace {
    /**
     * A triangulation of (tetrahedron x I), as used by iBundle() to
     * construct (3-manifold x I).
     *
     * This triangulation essentially builds the prism boundary from
     * tetrahedra and cones it to a central point (which is vertex 4
     * of every pentachoron).
     */
    struct Prism {
        Pentachoron<4>* bdry[2];
            /**< The pentachora providing the lower and upper boundaries
                 of the prism. */
        Pentachoron<4>* wallBase3[2][4];
            /**< wallBase3[i][j] provides a triangle base of the 3-dimensional
                 prism "wall" that runs between facet j of bdry[0] and
                 facet j of bdry[1]. */
        Pentachoron<4>* wallBase2[2][4][4];
            /**< wallBase2[i][j][k] provides a triangle side of the
                 3-dimensional prism "wall" between facet j of bdry[0] and
                 facet j of bdry[1].  Specifically, this triangle forms
                 a base of the rectangular 2-face of the prism that does not
                 meet vertex k.  Requires j,k distinct. */
        Pentachoron<4>* wallSide[2][4][4][4];
            /**< wallSide[i][j][k][l] provides a triangle side of the
                 3-dimensional prism "wall" between facet j of bdry[0] and
                 facet j of bdry[1].  Specifically, this triangle is contained
                 in the rectangular 2-face of the prism that does not
                 meet vertex k, meets wallBase[i][j][k], and does not meet
                 vertex l.  Requires j,k,l distinct. */

        /**
         * Create the pentachoron that provides either the upper or lower
         * tetrahedron boundary of this prism.
         */
        inline void buildBdry(Triangulation<4>& tri, int which) {
            bdry[which] = tri.newPentachoron();
        }

        /**
         * Create both pentachora that provide the upper and lower tetrahedron
         * boundaries of this prism, and glue those tetrahedron boundaries
         * together.
         */
        inline void buildAndIdentifyEnds(Triangulation<4>& tri) {
            bdry[0] = tri.newPentachoron();
            bdry[1] = tri.newPentachoron();
            bdry[0]->join(4, bdry[1], {});
        }

        /**
         * Create all remaining pentachora (80 of 82) within this prism.
         */
        inline void buildWalls(Triangulation<4>& tri) {
            for (int i = 0; i < 2; ++i)
                for (int j = 0; j < 4; ++j)
                    wallBase3[i][j] = tri.newPentachoron();
            for (int i = 0; i < 2; ++i)
                for (int j = 0; j < 4; ++j)
                    for (int k = 0; k < 4; ++k)
                        if (j != k)
                            wallBase2[i][j][k] = tri.newPentachoron();
                        else
                            wallBase2[i][j][k] = nullptr;
            for (int i = 0; i < 2; ++i)
                for (int j = 0; j < 4; ++j)
                    for (int k = 0; k < 4; ++k)
                        for (int l = 0; l < 4; ++l)
                            if (j != k && k != l && j != l)
                                wallSide[i][j][k][l] = tri.newPentachoron();
                            else
                                wallSide[i][j][k][l] = nullptr;
        }

        /**
         * Glue the pentachora of this prism together.
         */
        void glueInternally() {
            for (int i = 0; i < 2; ++i) {
                for (int j = 0; j < 4; ++j) {
                    wallBase3[i][j]->join(j, bdry[i], {});
                    for (int k = 0; k < 4; ++k) {
                        if (k == j)
                            continue;
                        wallBase3[i][j]->join(k, wallBase2[i][j][k], {});

                        for (int l = 0; l < 4; ++l) {
                            if (l == j || l == k)
                                continue;
                            wallBase2[i][j][k]->join(l,
                                wallSide[i][j][k][l], {});

                            if (i == 0)
                                wallSide[0][j][k][l]->join(6 - j - k - l,
                                    wallSide[1][j][k][l], {});

                            if (k < l)
                                wallSide[i][j][k][l]->join(k,
                                    wallSide[i][j][l][k], Perm<5>(k, l));
                        }
                    }
                }

                for (int j = 0; j < 4; ++j)
                    for (int k = j + 1; k < 4; ++k) {
                        wallBase2[i][j][k]->join(j,
                            wallBase2[i][k][j], Perm<5>(j, k));

                        for (int l = 0; l < 4; ++l) {
                            if (l == j || l == k)
                                continue;
                            wallSide[i][j][k][l]->join(j,
                                wallSide[i][k][j][l], Perm<5>(j, k));
                        }
                    }
            }
        }

        /**
         * Glue the walls of two adjacent prisms together.
         */
        void glueAdjacent(Prism& adj, int face, const Perm<4>& gluing) {
            Perm<5> gluing5 = Perm<5>::extend(gluing);
            for (int i = 0; i < 2; ++i) {
                wallBase3[i][face]->join(4,
                    adj.wallBase3[i][gluing[face]],
                    gluing5);

                for (int k = 0; k < 4; ++k) {
                    if (k == face)
                        continue;

                    wallBase2[i][face][k]->join(4,
                        adj.wallBase2[i][gluing[face]][gluing[k]],
                        gluing5);

                    for (int l = 0; l < 4; ++l) {
                        if (l == face || l == k)
                            continue;

                        wallSide[i][face][k][l]->join(4,
                            adj.wallSide[i][gluing[face]][gluing[k]][gluing[l]],
                            gluing5);
                    }
                }
            }
        }

        /**
         * Fold the given wall of this prism onto itself, so that the upper
         * half (ending in the upper boundary tetrahedron) folds onto the
         * lower half (ending in the lower boundary tetrahedron).
         */
        void foldWall(int face) {
            wallBase3[0][face]->join(4, wallBase3[1][face], {});

            for (int k = 0; k < 4; ++k) {
                if (k == face)
                    continue;

                wallBase2[0][face][k]->join(4, wallBase2[1][face][k], {});

                for (int l = 0; l < 4; ++l) {
                    if (l == face || l == k)
                        continue;

                    wallSide[0][face][k][l]->join(4, wallSide[1][face][k][l],
                        {});
                }
            }
        }

        /**
         * Locks all pentachora and internal facets in this prism.
         */
        void lockPrism() {
            // Lock the pentachora.
            for (int half = 0; half < 2; ++half) {
                bdry[half]->lock();
                for (int face = 0; face < 4; ++face) {
                    wallBase3[half][face]->lock();
                    for (int k = 0; k < 4; ++k)
                        if (k != face) {
                            wallBase2[half][face][k]->lock();
                            for (int l = 0; l < 4; ++l)
                                if (l != face && l != k)
                                    wallSide[half][face][k][l]->lock();
                        }
                }
            }

            // Lock the internal facets.
            // At the moment many of these locks are redundant, in that we are
            // locking the same facets from both sides.  This can be improved.
            for (int i = 0; i < 4; ++i)
                for (int half = 0; half < 2; ++half)
                    for (int face = 0; face < 4; ++face) {
                        wallBase3[half][face]->lockFacet(i);
                        for (int k = 0; k < 4; ++k)
                            if (k != face) {
                                wallBase2[half][face][k]->lockFacet(i);
                                for (int l = 0; l < 4; ++l)
                                    if (l != face && l != k)
                                        wallSide[half][face][k][l]->
                                            lockFacet(i);
                            }
                    }
        }

        /**
         * Locks all facets on the given wall of this prism.
         */
        void lockWall(int face) {
            for (int half = 0; half < 2; ++half) {
                wallBase3[half][face]->lockFacet(4);
                for (int k = 0; k < 4; ++k)
                    if (k != face) {
                        wallBase2[half][face][k]->lockFacet(4);
                        for (int l = 0; l < 4; ++l)
                            if (l != face && l != k)
                                wallSide[half][face][k][l]->lockFacet(4);
                    }
            }
        }
    };
}

Triangulation<4> Example<4>::iBundle(const Triangulation<3>& base) {
    Triangulation<4> ans;

    size_t n = base.size();
    if (n == 0)
        return ans;

    // We have at least one tetrahedron.  Off we go.
    FixedArray<Prism> prism(n);

    // Build the boundaries first so we get the relevant pentachora
    // numbered correctly within the final triangulation.
    size_t i;
    for (i = 0; i < n; ++i)
        prism[i].buildBdry(ans, 0); // Pentachora 0..n-1
    for (i = 0; i < n; ++i)
        prism[i].buildBdry(ans, 1); // Pentachora n..2n-1

    // Build the remainder of the prisms.
    for (i = 0; i < n; ++i) {
        prism[i].buildWalls(ans);
        prism[i].glueInternally();
    }

    // Glue adjacent prisms together, and sort out locks.
    for (i = 0; i < n; ++i) {
        const Tetrahedron<3>* tet = base.tetrahedron(i);
        for (int face = 0; face < 4; ++face) {
            if (auto adj = tet->adjacentTetrahedron(face)) {
                // Make sure we haven't already glued this from the other side.
                size_t adjIndex = adj->index();
                if (adjIndex < i ||
                        (adjIndex == i && tet->adjacentFace(face) < face))
                    continue;

                prism[i].glueAdjacent(prism[adjIndex], face,
                    tet->adjacentGluing(face));

                if (tet->isFacetLocked(face))
                    prism[i].lockWall(face);
            } else if (tet->isFacetLocked(face)) {
                prism[i].lockWall(face);
            }
        }
        if (tet->isLocked())
            prism[i].lockPrism();
    }

    return ans;
}

Triangulation<4> Example<4>::s1Bundle(const Triangulation<3>& base) {
    Triangulation<4> ans = iBundle(base);

    size_t n = base.size();
    for (size_t i = 0; i < n; ++i) {
        ans.pentachoron(i)->join(4, ans.pentachoron(i + n), {});
        if (base.tetrahedron(i)->isLocked())
            ans.pentachoron(i)->lockFacet(4);
    }

    return ans;
}

Triangulation<4> Example<4>::boundarySpin(const Triangulation<3>& base) {
    // This largely follows the code from iBundle(), but with additional
    // top/bottom gluings and boundary fillings.
    Triangulation<4> ans;

    size_t n = base.size();
    if (n == 0)
        return ans;

    // We have at least one tetrahedron.  Off we go.
    FixedArray<Prism> prism(n);

    // Build the prisms and sort out their internal gluings.
    for (size_t i = 0; i < n; ++i) {
        prism[i].buildAndIdentifyEnds(ans);
        prism[i].buildWalls(ans);
        prism[i].glueInternally();
    }

    // Glue adjacent prisms together, and sort out locks.
    for (size_t i = 0; i < n; ++i) {
        const Tetrahedron<3>* tet = base.tetrahedron(i);
        for (int face = 0; face < 4; ++face) {
            if (auto adj = tet->adjacentTetrahedron(face)) {
                // Make sure we haven't already glued this from the other side.
                size_t adjIndex = adj->index();
                if (adjIndex < i ||
                        (adjIndex == i && tet->adjacentFace(face) < face))
                    continue;

                prism[i].glueAdjacent(prism[adjIndex], face,
                    tet->adjacentGluing(face));

                if (tet->isFacetLocked(face))
                    prism[i].lockWall(face);
            } else if (tet->isFacetLocked(face)) {
                prism[i].lockWall(face);
            } else {
                // We have a boundary face of the 3-manifold: fold the
                // corresponding wall of the prism onto itself to produce the
                // required filling effect.
                prism[i].foldWall(face);
            }
        }
        if (tet->isLocked())
            prism[i].lockPrism();
    }

    return ans;
}

Triangulation<4> Example<4>::bundleWithMonodromy(
        const Triangulation<3>& base, const Isomorphism<3>& monodromy) {
    Triangulation<4> ans = iBundle(base);

    size_t n = base.size();
    for (size_t i = 0; i < n; ++i)
        ans.pentachoron(i)->join(4, ans.pentachoron(monodromy.simpImage(i) + n),
            Perm<5>::extend(monodromy.facetPerm(i)));

    return ans;
}

Triangulation<4> Example<4>::spun(const Link& knot, StrandRef breakOpen) {
    Triangulation<3> c = knot.longComplement();

    // Locate the invalid vertex in c.
    Vertex<3>* invalid = nullptr;
    for (auto v : c.vertices())
        if (! v->isValid()) {
            invalid = v;
            break;
        }
    if (! invalid)
        throw ImpossibleScenario("longComplement() did not produce an "
            "invalid vertex");

    c.truncate(invalid, true /* lock the truncation surface */);

    Triangulation<4> ans = boundarySpin(c);
    ans.unlockAll();

    // We need to simplify _before_ calling makeIdeal().
    // This is because, when simplifying an ideal triangulation, we need to
    // repeatedly run 3-sphere recognition in order to work out which is the
    // ideal vertex.  If we have thousands of vertices, this takes time.
    // When simplifying a compact triangulation OTOH, Regina caches the fact
    // that all vertex links are balls or spheres, and does not need to run
    // 3-sphere recognition at all during the simplification process.
    ans.simplify();

    // Now it's hopefully small, we can be brave enough to simplify again
    // but this time using an ideal triangulation.
    ans.makeIdeal();
    ans.simplify();

    return ans;
}

namespace {
    /**
     * A vertex in a local product triangle x triangle, stored using the two
     * triangle-local vertex numbers.
     */
    struct SurfaceProductVertex {
        int a;
        int b;

        friend bool operator<(const SurfaceProductVertex& x,
                const SurfaceProductVertex& y) {
            return x.a < y.a || (x.a == y.a && x.b < y.b);
        }

        friend bool operator==(const SurfaceProductVertex& x,
                const SurfaceProductVertex& y) {
            return x.a == y.a && x.b == y.b;
        }
    };

    using SurfaceProductTriangleOrder = std::array<int, 3>;

    /**
     * The staircase triangulation of triangle x triangle depends on an
     * ordering of each triangle's vertices.  We choose these orders globally
     * so adjacent triangles induce the same diagonal on each shared edge
     * product.
     */
    constexpr std::array<SurfaceProductTriangleOrder, 6>
            surfaceProductAllTriangleOrders {{
        {{0, 1, 2}},
        {{0, 2, 1}},
        {{1, 0, 2}},
        {{1, 2, 0}},
        {{2, 0, 1}},
        {{2, 1, 0}}
    }};

    /**
     * One edge gluing from an input surface triangulation, used as a
     * constraint when choosing compatible staircase diagonals.
     */
    struct SurfaceProductEdgeConstraint {
        size_t from;
        int edge;
        size_t to;
        Perm<3> gluing;
    };

    /**
     * A product facet, identified independently of the pentachoron that sees
     * it.
     *
     * The two triangle indices identify the product cell triangle x triangle.
     * The four local product vertices identify the facet inside that cell;
     * they are sorted when the key is built, so opposite sides of an internal
     * facet produce identical keys.
     */
    struct SurfaceProductFacetKey {
        size_t aTriangle;
        size_t bTriangle;
        std::array<SurfaceProductVertex, 4> vertices;

        friend bool operator<(const SurfaceProductFacetKey& x,
                const SurfaceProductFacetKey& y) {
            if (x.aTriangle != y.aTriangle)
                return x.aTriangle < y.aTriangle;
            if (x.bTriangle != y.bTriangle)
                return x.bTriangle < y.bTriangle;
            return x.vertices < y.vertices;
        }
    };

    // (pentachoron, its five local product vertices, opposite facet number).
    using SurfaceProductFacetData = std::tuple<Pentachoron<4>*,
        std::array<SurfaceProductVertex, 5>, int>;

    /**
     * Return the six monotone paths from (0,0) to (2,2).  Each path gives one
     * pentachoron in the standard staircase triangulation of
     * Delta^2 x Delta^2: a 0-step advances through the first triangle, and a
     * 1-step through the second.
     */
    std::vector<std::array<int, 4>> surfaceProductShuffles() {
        std::array<int, 4> word {{0, 0, 1, 1}};
        std::vector<std::array<int, 4>> ans;

        do {
            ans.push_back(word);
        } while (std::next_permutation(word.begin(), word.end()));

        return ans;
    }

    std::array<SurfaceProductVertex, 5> surfaceProductSimplexVertices(
            const std::array<int, 4>& shuffle,
            const SurfaceProductTriangleOrder& aOrder,
            const SurfaceProductTriangleOrder& bOrder) {
        std::array<SurfaceProductVertex, 5> ans;
        int a = 0;
        int b = 0;

        ans[0] = { aOrder[a], bOrder[b] };
        for (int i = 0; i < 4; ++i) {
            if (shuffle[i] == 0)
                ++a;
            else
                ++b;
            ans[i + 1] = { aOrder[a], bOrder[b] };
        }

        return ans;
    }

    bool surfaceProductCompatibleOrders(
            const SurfaceProductTriangleOrder& fromOrder, int edge,
            const SurfaceProductTriangleOrder& toOrder,
            const Perm<3>& gluing) {
        // u and v are the two endpoints of the edge opposite vertex edge.
        int u = (edge == 0 ? 1 : 0);
        int v = (edge == 2 ? 1 : 2);

        auto before = [](const SurfaceProductTriangleOrder& order,
                int x, int y) {
            return std::find(order.begin(), order.end(), x) <
                std::find(order.begin(), order.end(), y);
        };
        return before(fromOrder, u, v) ==
            before(toOrder, gluing[u], gluing[v]);
    }

    bool surfaceProductAssignOrdersRec(size_t next,
            const std::vector<SurfaceProductEdgeConstraint>& edges,
            std::vector<int>& chosen,
            std::vector<SurfaceProductTriangleOrder>& orders) {
        if (next == orders.size())
            return true;

        if (chosen[next] >= 0)
            return surfaceProductAssignOrdersRec(next + 1, edges, chosen,
                orders);

        for (size_t candidate = 0;
                candidate < surfaceProductAllTriangleOrders.size();
                ++candidate) {
            orders[next] = surfaceProductAllTriangleOrders[candidate];
            chosen[next] = static_cast<int>(candidate);

            bool ok = true;
            for (const SurfaceProductEdgeConstraint& edge : edges) {
                if (edge.from != next && edge.to != next)
                    continue;
                if (chosen[edge.from] < 0 || chosen[edge.to] < 0)
                    continue;
                if (! surfaceProductCompatibleOrders(orders[edge.from],
                        edge.edge, orders[edge.to], edge.gluing)) {
                    ok = false;
                    break;
                }
            }

            if (ok && surfaceProductAssignOrdersRec(next + 1, edges, chosen,
                    orders))
                return true;
        }

        chosen[next] = -1;
        return false;
    }

    std::vector<SurfaceProductTriangleOrder> surfaceProductChooseTriangleOrders(
            const Triangulation<2>& surface) {
        std::vector<SurfaceProductEdgeConstraint> edges;
        for (size_t i = 0; i < surface.size(); ++i) {
            const Triangle<2>* triangle = surface.triangle(i);
            for (int edge = 0; edge < 3; ++edge) {
                const Triangle<2>* adj = triangle->adjacentTriangle(edge);
                if (! adj)
                    continue;

                // Record each original edge gluing once, following the same
                // index-ordering pattern used in Regina's example builders.
                if (adj->index() > i ||
                        (adj->index() == i &&
                            triangle->adjacentEdge(edge) > edge))
                    edges.push_back({
                        i, edge, adj->index(), triangle->adjacentGluing(edge)
                    });
            }
        }

        std::vector<SurfaceProductTriangleOrder> orders(surface.size());
        std::vector<int> chosen(surface.size(), -1);
        if (! surfaceProductAssignOrdersRec(0, edges, chosen, orders))
            throw FailedPrecondition("could not choose compatible edge "
                "diagonals for the surface triangulation");

        return orders;
    }

    SurfaceProductFacetKey surfaceProductMakeKey(size_t aTriangle,
            size_t bTriangle,
            const std::array<SurfaceProductVertex, 5>& vertices, int facet) {
        SurfaceProductFacetKey ans { aTriangle, bTriangle, {} };
        for (int src = 0, dst = 0; src < 5; ++src)
            if (src != facet)
                ans.vertices[dst++] = vertices[src];
        std::sort(ans.vertices.begin(), ans.vertices.end());
        return ans;
    }

    std::array<int, 5> surfaceProductGluingMap(
            const SurfaceProductFacetData& from,
            const SurfaceProductFacetData& to) {
        const auto& fromVertices = std::get<1>(from);
        const auto& toVertices = std::get<1>(to);
        int fromFacet = std::get<2>(from);
        int toFacet = std::get<2>(to);
        std::array<int, 5> ans {};
        ans[fromFacet] = toFacet;

        std::array<bool, 5> used {};
        used[toFacet] = true;

        for (int i = 0; i < 5; ++i) {
            if (i == fromFacet)
                continue;

            bool found = false;
            for (int j = 0; j < 5; ++j) {
                if (used[j])
                    continue;
                if (fromVertices[i] == toVertices[j]) {
                    ans[i] = j;
                    used[j] = true;
                    found = true;
                    break;
                }
            }

            if (! found)
                throw ImpossibleScenario("incompatible product facets "
                    "selected for gluing");
        }

        return ans;
    }

    SurfaceProductFacetData surfaceProductLookupFacet(
            const std::map<SurfaceProductFacetKey,
                std::vector<SurfaceProductFacetData>>& facets,
            const SurfaceProductFacetKey& key) {
        auto it = facets.find(key);
        if (it == facets.end() || it->second.empty())
            throw ImpossibleScenario("product facet lookup failed");
        if (it->second.size() != 1)
            throw ImpossibleScenario("ambiguous product facet lookup");
        return it->second.front();
    }
}

Triangulation<4> Example<4>::surfaceProduct(
        const Triangulation<2>& surfaceA,
        const Triangulation<2>& surfaceB) {
    Triangulation<4> ans;
    const auto pieces = surfaceProductShuffles();
    const auto ordersA = surfaceProductChooseTriangleOrders(surfaceA);
    const auto ordersB = surfaceProductChooseTriangleOrders(surfaceB);

    std::map<SurfaceProductFacetKey,
        std::vector<SurfaceProductFacetData>> facets;
    std::vector<std::pair<SurfaceProductFacetData,
        SurfaceProductFacetKey>> pending;

    auto missesCoordinate = [](const std::array<SurfaceProductVertex, 4>& facet,
            bool firstFactor, int coord) {
        // A product facet lies in edge x triangle (or triangle x edge)
        // exactly when none of its vertices uses the opposite coordinate.
        return std::all_of(facet.begin(), facet.end(),
            [=](const SurfaceProductVertex& v) {
                return (firstFactor ? v.a : v.b) != coord;
            });
    };

    auto addPending = [&](SurfaceProductFacetData data,
            SurfaceProductFacetKey key, bool firstFactor, size_t newTriangle,
            const Perm<3>& gluing) {
        // Transport both the source facet data and the destination lookup key
        // across an original surface edge gluing.  The transported source data
        // is later compared with the destination facet to recover Regina's
        // full Perm<5> gluing.
        for (SurfaceProductVertex& v : std::get<1>(data)) {
            if (firstFactor)
                v.a = gluing[v.a];
            else
                v.b = gluing[v.b];
        }

        if (firstFactor) {
            key.aTriangle = newTriangle;
            for (SurfaceProductVertex& v : key.vertices)
                v.a = gluing[v.a];
        } else {
            key.bTriangle = newTriangle;
            for (SurfaceProductVertex& v : key.vertices)
                v.b = gluing[v.b];
        }
        std::sort(key.vertices.begin(), key.vertices.end());

        pending.emplace_back(data, key);
    };

    for (size_t a = 0; a < surfaceA.size(); ++a)
        for (size_t b = 0; b < surfaceB.size(); ++b)
            for (const auto& piece : pieces) {
                auto* pent = ans.newPentachoron();
                auto vertices = surfaceProductSimplexVertices(piece,
                    ordersA[a], ordersB[b]);

                for (int facet = 0; facet < 5; ++facet) {
                    auto key = surfaceProductMakeKey(a, b, vertices, facet);
                    SurfaceProductFacetData data { pent, vertices, facet };
                    facets[key].push_back(data);

                    // If this facet sits over an original surface edge, then
                    // either leave it as boundary or schedule it for gluing to
                    // the adjacent product cell.  We only schedule one
                    // direction of each original edge gluing.
                    bool isExternal = false;
                    for (int edge = 0; edge < 3; ++edge) {
                        if (! missesCoordinate(key.vertices, true, edge))
                            continue;

                        isExternal = true;
                        const Triangle<2>* triangle = surfaceA.triangle(a);
                        const Triangle<2>* adj =
                            triangle->adjacentTriangle(edge);
                        if (adj && (adj->index() > a ||
                                (adj->index() == a &&
                                    triangle->adjacentEdge(edge) > edge)))
                            addPending(data, key, true, adj->index(),
                                triangle->adjacentGluing(edge));
                        break;
                    }

                    if (isExternal)
                        continue;

                    for (int edge = 0; edge < 3; ++edge) {
                        if (! missesCoordinate(key.vertices, false, edge))
                            continue;

                        const Triangle<2>* triangle = surfaceB.triangle(b);
                        const Triangle<2>* adj =
                            triangle->adjacentTriangle(edge);
                        if (adj && (adj->index() > b ||
                                (adj->index() == b &&
                                    triangle->adjacentEdge(edge) > edge)))
                            addPending(data, key, false, adj->index(),
                                triangle->adjacentGluing(edge));
                        break;
                    }
                }
            }

    // First glue facets internal to each triangle x triangle staircase
    // subdivision.  Facets crossing original surface edges have no local mate
    // here; they are handled by the pending pass below.
    for (const auto& [key, occ] : facets) {
        if (occ.size() == 2) {
            auto map = surfaceProductGluingMap(occ[0], occ[1]);
            std::get<0>(occ[0])->join(std::get<2>(occ[0]),
                std::get<0>(occ[1]), Perm<5>(map));
        } else if (occ.size() > 2)
            throw ImpossibleScenario("more than two local product facets "
                "coincide");
    }

    // Now glue product cells across the original surface edge identifications.
    for (const auto& join : pending) {
        auto to = surfaceProductLookupFacet(facets, join.second);
        auto map = surfaceProductGluingMap(join.first, to);
        std::get<0>(join.first)->join(std::get<2>(join.first),
            std::get<0>(to), Perm<5>(map));
    }

    return ans;
}

Triangulation<4> Example<4>::handlebody(size_t genus) {
    if (genus == 0)
        return Example<4>::ball();

    if (genus == 1) {
        Triangulation<4> ans;
        Pentachoron<4>* p0 = ans.newPentachoron();
        Pentachoron<4>* p1 = ans.newPentachoron();

        // The naive one-pentachoron closure gives the non-orientable
        // S^1 x~ B^3.  This is a two-pentachoron orientable S^1 x B^3
        // model.
        p0->join(0, p1, Perm<5>());
        p0->join(1, p1, Perm<5>(3, 2, 4, 1, 0));
        return ans;
    }

    Triangulation<3> spine = Example<3>::handlebody(genus);

    std::vector<Triangle<3>*> internal;
    internal.reserve(spine.countTriangles());
    for (Triangle<3>* tri : spine.triangles())
        if (! tri->isBoundary())
            internal.push_back(tri);

    Triangulation<4> ans;
    ans.newPentachora(internal.size());

    const size_t nTets = spine.size();
    std::vector<Pentachoron<4>*> topPent(nTets, nullptr);
    std::vector<Pentachoron<4>*> botPent(nTets, nullptr);
    std::vector<Perm<5>> topPerm(nTets);
    std::vector<Perm<5>> botPerm(nTets);

    auto preimage = [](const Perm<5>& p, int image) {
        for (int i = 0; i < 5; ++i)
            if (p[i] == image)
                return i;
        throw InvalidArgument("Example<4>::handlebody(): invalid permutation");
    };

    /*
     * Attach one side of a newly layered pentachoron to a stored top/bottom
     * sheet over a tetrahedron of the 3-dimensional spine.
     *
     * The stored permutation records how the first pentachoron occupied this
     * sheet.  If the sheet is already occupied, then we follow the
     * codimension-2 triangle link to the currently exposed endpoint, and glue
     * the new pentachoron there.
     */
    auto attach = [&](Pentachoron<4>* pent, const Perm<5>& basePerm,
            Pentachoron<4>*& slotPent, Perm<5>& slotPerm) {
        int sourceFacet = preimage(basePerm, 4);

        if (! slotPent) {
            slotPent = pent;
            slotPerm = basePerm;
            return;
        }

        Pentachoron<4>* occupant = slotPent;
        Perm<5> occPerm = slotPerm;
        Perm<5> local = occPerm.inverse() * basePerm;
        Triangle<4>* tri = occupant->triangle(
            Triangle<4>::triangleNumber[local[0]][local[1]][local[2]]);

        const auto& front = tri->front();
        const auto& back = tri->back();
        Perm<5> frontVer = front.vertices();
        Perm<5> backVer = back.vertices();
        int virtualFacet = preimage(occPerm, 4);

        auto endpointGluing = [&](Perm<5> sourceEndpoint,
                Perm<5> targetEndpoint, bool toBackEndpoint) {
            std::array<int, 5> image;

            // Match the three triangle vertices using the canonical
            // ordering of the triangle around which the link is ordered.
            for (int i = 0; i < 3; ++i) {
                int canonical = -1;
                for (int j = 0; j < 3; ++j) {
                    if (sourceEndpoint[j] == local[i]) {
                        canonical = j;
                        break;
                    }
                }
                if (canonical < 0)
                    throw InvalidArgument("Example<4>::handlebody(): "
                        "inconsistent triangle endpoint labelling");
                image[i] = targetEndpoint[canonical];
            }

            int sourceOther = 7 - sourceFacet;
            if (toBackEndpoint) {
                image[sourceFacet] = targetEndpoint[3];
                image[sourceOther] = targetEndpoint[4];
            } else {
                image[sourceFacet] = targetEndpoint[4];
                image[sourceOther] = targetEndpoint[3];
            }

            return Perm<5>(image);
        };

        if (front.simplex() == occupant && frontVer[4] == virtualFacet) {
            pent->join(sourceFacet, back.simplex(),
                endpointGluing(frontVer, backVer, true));
        } else if (back.simplex() == occupant && backVer[3] == virtualFacet) {
            pent->join(sourceFacet, front.simplex(),
                endpointGluing(backVer, frontVer, false));
        } else {
            throw InvalidArgument("Example<4>::handlebody(): could not find "
                "the exposed end of a triangle link");
        }
    };

    size_t which = 0;
    for (Triangle<3>* tri : internal) {
        if (tri->degree() != 2)
            throw InvalidArgument("Example<4>::handlebody(): expected every "
                "internal triangle to have degree 2");

        Pentachoron<4>* pent = ans.pentachoron(which++);

        const auto& emb0 = tri->embedding(0);
        const auto& emb1 = tri->embedding(1);
        Perm<4> ver0 = emb0.vertices();
        Perm<4> ver1 = emb1.vertices();
        size_t ind0 = emb0.simplex()->index();
        size_t ind1 = emb1.simplex()->index();

        // First side: put the old tetrahedron's remaining vertex in slot 3,
        // and the artificial I-direction in slot 4.
        attach(pent, Perm<5>(ver0[0], ver0[1], ver0[2], ver0[3], 4),
            topPent[ind0], topPerm[ind0]);

        /*
         * Second side: swap the old remaining vertex with the artificial
         * I-direction, as the 3D construction does with faces 012 and 013.
         * If the two embeddings have the same sign, the second side lands on
         * the bottom sheet; otherwise it lands on the top sheet.
         */
        if (ver1.sign() == ver0.sign())
            attach(pent, Perm<5>(ver1[0], ver1[1], ver1[2], 4, ver1[3]),
                botPent[ind1], botPerm[ind1]);
        else
            attach(pent, Perm<5>(ver1[0], ver1[1], ver1[2], 4, ver1[3]),
                topPent[ind1], topPerm[ind1]);
    }

    /*
     * Close every tetrahedron of the 3-dimensional spine whose top and bottom
     * sheets are both present.  This is the direct 4-dimensional analogue of
     * the final sheet-closing loop in Example<3>::handlebody().
     */
    for (size_t i = 0; i < nTets; ++i)
        if (topPent[i] && botPent[i])
            topPent[i]->join(preimage(topPerm[i], 4), botPent[i],
                botPerm[i].inverse() * topPerm[i]);

    return ans;
}

} // namespace regina

