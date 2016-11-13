
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

#include "triangulation/example4.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"

namespace regina {

Triangulation<4>* Example<4>::rp4() {
    Triangulation<4>* ans = new Triangulation<4>();
    ans->setLabel("Real projective 4-space");

    // Thanks Ryan, you rock. :)
    Pentachoron<4>* p = ans->newPentachoron();
    Pentachoron<4>* q = ans->newPentachoron();
    Pentachoron<4>* r = ans->newPentachoron();
    Pentachoron<4>* s = ans->newPentachoron();
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

Triangulation<4>* Example<4>::cappellShaneson() {
    // Use the gluings described in arXiv:1109.3899.
    Triangulation<4>* ans = new Triangulation<4>();
    ans->setLabel("Cappell-Shaneson knot complement");

    Pentachoron<4>* p = ans->newPentachoron();
    Pentachoron<4>* q = ans->newPentachoron();
    q->join(0, p, Perm<5>(2,0,1,3,4));
    q->join(2, p, Perm<5>(0,1,4,2,3));
    q->join(3, p, Perm<5>(0,2,3,1,4));
    p->join(3, p, Perm<5>(1,2,3,0,4));
    q->join(4, q, Perm<5>(0,2,3,4,1));

    return ans;
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
                 meet vertex k, meets wallBase[i][j[k], and does not meet
                 vertex l.  Requires j,k,l distinct. */

        /**
         * Create the pentachoron that provides either the upper or lower
         * tetrahedron boundary of this prism.
         */
        inline void buildBdry(Triangulation<4>* tri, int which) {
            bdry[which] = tri->newPentachoron();
        }

        /**
         * Create all remaining pentachora (80 of 82) within this prism.
         */
        inline void buildWalls(Triangulation<4>* tri) {
            unsigned i, j, k, l;
            for (i = 0; i < 2; ++i)
                for (j = 0; j < 4; ++j)
                    wallBase3[i][j] = tri->newPentachoron();
            for (i = 0; i < 2; ++i)
                for (j = 0; j < 4; ++j)
                    for (k = 0; k < 4; ++k)
                        if (j != k)
                            wallBase2[i][j][k] = tri->newPentachoron();
                        else
                            wallBase2[i][j][k] = 0;
            for (i = 0; i < 2; ++i)
                for (j = 0; j < 4; ++j)
                    for (k = 0; k < 4; ++k)
                        for (l = 0; l < 4; ++l)
                            if (j != k && k != l && j != l)
                                wallSide[i][j][k][l] = tri->newPentachoron();
                            else
                                wallSide[i][j][k][l] = 0;
        }

        /**
         * Glue the pentachora of this prism together.
         */
        void glueInternally() {
            Perm<5> id;

            unsigned i, j, k, l;
            for (i = 0; i < 2; ++i) {
                for (j = 0; j < 4; ++j) {
                    wallBase3[i][j]->join(j, bdry[i], id);
                    for (k = 0; k < 4; ++k) {
                        if (k == j)
                            continue;
                        wallBase3[i][j]->join(k, wallBase2[i][j][k], id);

                        for (l = 0; l < 4; ++l) {
                            if (l == j || l == k)
                                continue;
                            wallBase2[i][j][k]->join(l,
                                wallSide[i][j][k][l], id);

                            if (i == 0)
                                wallSide[0][j][k][l]->join(6 - j - k - l,
                                    wallSide[1][j][k][l], id);

                            if (k < l)
                                wallSide[i][j][k][l]->join(k,
                                    wallSide[i][j][l][k], Perm<5>(k, l));
                        }
                    }
                }

                for (j = 0; j < 4; ++j)
                    for (k = j + 1; k < 4; ++k) {
                        wallBase2[i][j][k]->join(j,
                            wallBase2[i][k][j], Perm<5>(j, k));

                        for (l = 0; l < 4; ++l) {
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
        void glueAdjacent(Prism& adj, unsigned face, const Perm<4>& gluing) {
            Perm<5> gluing5 = Perm<5>::extend(gluing);
            unsigned i, k, l;
            for (i = 0; i < 2; ++i) {
                wallBase3[i][face]->join(4,
                    adj.wallBase3[i][gluing[face]],
                    gluing5);

                for (k = 0; k < 4; ++k) {
                    if (k == face)
                        continue;

                    wallBase2[i][face][k]->join(4,
                        adj.wallBase2[i][gluing[face]][gluing[k]],
                        gluing5);

                    for (l = 0; l < 4; ++l) {
                        if (l == face || l == k)
                            continue;

                        wallSide[i][face][k][l]->join(4,
                            adj.wallSide[i][gluing[face]][gluing[k]][gluing[l]],
                            gluing5);
                    }
                }
            }
        }
    };
}

Triangulation<4>* Example<4>::iBundle(
        const Triangulation<3>& base) {
    Triangulation<4>* ans = new Triangulation<4>();
    Packet::ChangeEventSpan span(ans);
    ans->setLabel(base.label() + " x I");

    unsigned long n = base.size();
    if (n == 0)
        return ans;

    // We have at least one tetrahedron.  Off we go.
    Prism* prism = new Prism[n];

    // Build the boundaries first so we get the relevant pentachora
    // numbered correctly within the final triangulation.
    unsigned long i;
    for (i = 0; i < n; ++i)
        prism[i].buildBdry(ans, 0); // Pentachora 0..n-1
    for (i = 0; i < n; ++i)
        prism[i].buildBdry(ans, 1); // Pentachora n..2n-1

    // Build the remainder of the prisms.
    for (i = 0; i < n; ++i) {
        prism[i].buildWalls(ans);
        prism[i].glueInternally();
    }

    // Glue adjacent prisms together.
    unsigned long adjIndex;
    unsigned face;
    const Tetrahedron<3> *tet, *adj;
    for (i = 0; i < n; ++i) {
        tet = base.tetrahedron(i);
        for (face = 0; face < 4; ++face) {
            adj = tet->adjacentTetrahedron(face);
            if (! adj)
                continue;

            // Make sure we haven't already glued this from the other side.
            adjIndex = adj->markedIndex();
            if (adjIndex < i ||
                    (adjIndex == i && tet->adjacentFace(face) < face))
                continue;

            // Glue the prisms together!
            prism[i].glueAdjacent(prism[adjIndex], face,
                tet->adjacentGluing(face));
        }
    }

    delete[] prism;
    return ans;
}

Triangulation<4>* Example<4>::s1Bundle(
        const Triangulation<3>& base) {
    Triangulation<4>* ans = iBundle(base);
    Packet::ChangeEventSpan span(ans);
    ans->setLabel(base.label() + " x S1");

    Perm<5> id;
    unsigned long n = base.size();
    unsigned long i;
    for (i = 0; i < n; ++i)
        ans->pentachoron(i)->join(4, ans->pentachoron(i + n), id);

    return ans;
}

Triangulation<4>* Example<4>::bundleWithMonodromy(
        const Triangulation<3>& base, const Isomorphism<3>& monodromy) {
    Triangulation<4>* ans = iBundle(base);
    Packet::ChangeEventSpan span(ans);
    ans->setLabel(base.label() + " x I / ~");

    Perm<5> id;
    unsigned long n = base.size();
    unsigned long i;
    for (i = 0; i < n; ++i)
        ans->pentachoron(i)->join(4,
            ans->pentachoron(monodromy.simpImage(i) + n),
            Perm<5>::extend(monodromy.facetPerm(i)));

    return ans;
}

} // namespace regina

