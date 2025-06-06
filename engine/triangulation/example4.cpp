
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
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

#include "link/link.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"
#include "triangulation/example4.h"

namespace regina {

Triangulation<4> Example<4>::cp2() {
    // Built by Rhuaidi Burke using Katie from a Kirby diagram of the
    // standard CP^2, then oriented in a way that ensures our
    // intersection form matches CP^2 and not the reflection \overline{CP^2}.
    return Triangulation<4>::fromGluings(4, {
        { 0, 0, 0, {1,0,2,3,4} }, { 0, 2, 0, {2,0,3,1,4} },
        { 0, 4, 1, {0,1,2,4,3} }, { 1, 0, 2, {0,1,2,4,3} },
        { 1, 1, 3, {0,1,2,4,3} }, { 1, 2, 1, {2,0,4,3,1} },
        { 2, 1, 2, {0,3,2,1,4} }, { 2, 2, 3, {1,2,3,0,4} },
        { 2, 4, 3, {1,0,2,3,4} }, { 3, 0, 3, {2,1,0,3,4} }});
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
    //Triangulation<4> ans = Triangulation<4>::fromIsoSig("8ALLAvvzwwLQAMzwvQAQMvAzLvwQPMwPwQQQQQAQQQLAAvQQQQQaaddeekkpttjjppvwxyopCDrCszzzuAwBOISRIRQUUWFFYZHHSSJJKKLLMMOOQW0P0RWSTTVXXXZ011223344556677qbGanbLbLbLbaaeanbDbDb2aKafavavbUaaaVbnafaIaIaqb0aBb0aDb0a2bWagbRaVbJadaaaCb3aVbIaIaPaPb2bWbSbGacaVbWaqb6aPbTbrafaibgaJaTboajatafataBaNaBbqb6ava2abbbayagaqbSaPbJaPbJa2boaGacaNabaqbub");

    // This triangulation is derived from a Kirby diagram of the standard K3 surface, 
    // constructed and simplified by Rhuaidi Burke using Katie and the "Up-Down-Simplify" heuristic.
    Triangulation<4> ans = Triangulation<4>::fromIsoSig("2ALAMMvAwvPLQwvPwLQPMvPQQQQLQPAwwALQQAAQPPzQPPaaddceffggikqpmllsorsquxwuttvxxFyyzzAABBCCDDJKIIGGHHMKLLNNOOPPQQSSUUTTVVWWXZZZ0011TbgaiaiakaqaaaMbaawaUbjbabPa5ayaGauara3bmagaNaUbybNa2aRa2aibbawboaraPbJa2aKaya1aqbub2afbPaZaUbcayaGawaca-aLb+aoatbfaNabagajaya1axbybrbebubgaubFbtbfaFbFbFbqbzaoaPboa");
    ans.orient();
    ans.reflect(); // so sig = -16, not 16
    return ans;
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

    // We need to simplify _before_ calling finiteToIdeal().
    // This is because, when simplifying an ideal triangulation, we need to
    // repeatedly run 3-sphere recognition in order to work out which is the
    // ideal vertex.  If we have thousands of vertices, this takes time.
    // When simplifying a compact triangulation OTOH, regina caches the fact
    // that all vertex links are balls or spheres, and does not need to run
    // 3-sphere recognition at all during the simplification process.
    ans.simplify();

    // Now it's hopefully small, we can be brave enough to simplify again
    // but this time using an ideal triangulation.
    ans.finiteToIdeal();
    ans.simplify();

    return ans;
}

} // namespace regina

