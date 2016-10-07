
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

#include "dim4/dim4exampletriangulation.h"
#include "dim4/dim4triangulation.h"
#include "triangulation/ntriangulation.h"

namespace regina {

Dim4Triangulation* Dim4ExampleTriangulation::fourSphere() {
    // Take two pentachora and join their entire boundaries according to
    // the identity map.
    Dim4Triangulation* ans = new Dim4Triangulation();
    ans->setLabel("4-sphere");

    Dim4Pentachoron* p = ans->newPentachoron();
    Dim4Pentachoron* q = ans->newPentachoron();
    p->join(0, q, Perm<5>());
    p->join(1, q, Perm<5>());
    p->join(2, q, Perm<5>());
    p->join(3, q, Perm<5>());
    p->join(4, q, Perm<5>());

    return ans;
}

Dim4Triangulation* Dim4ExampleTriangulation::simplicialFourSphere() {
    Dim4Triangulation* ans = new Dim4Triangulation();
    ans->setLabel("Standard simplicial 4-sphere");

    Dim4Pentachoron* penList[6];
        // one pentachoron for every vertex of the 5-simplex
    unsigned i, j, k;
    for (i=0; i<6; i++)
        penList[i] = ans->newPentachoron();
    // one gluing for every distinct pair of vertices of 5-simplex
    // we are gluing facet j-1 of pen i to facet i of pen j. 
    // using the cycle i -> i+1 -> ... -> j-1 -> i. 
    for (i=0; i<5; i++)
        for (j=i+1; j<6; j++) {
            int map[5];
            for (k=0; k<5; k++) {
                if ( (k<i) || (k>=j) )
                    map[k]=k;
                else if (k<(j-1))
                    map[k]=k+1;
                else map[j-1]=i;
            }
            penList[i]->join(j-1 , penList[j], Perm<5>(map) );
        }
    return ans;
}

Dim4Triangulation* Dim4ExampleTriangulation::rp4() {
    Dim4Triangulation* ans = new Dim4Triangulation();
    ans->setLabel("Real projective 4-space");

    // Thanks Ryan, you rock. :)
    Dim4Pentachoron* p = ans->newPentachoron();
    Dim4Pentachoron* q = ans->newPentachoron();
    Dim4Pentachoron* r = ans->newPentachoron();
    Dim4Pentachoron* s = ans->newPentachoron();
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

Dim4Triangulation* Dim4ExampleTriangulation::s3xs1() {
    // Make two pentachora, and join three of the five facets according
    // to the identity map.  Only facets 0123 and 1234 of each
    // pentachoron remain.
    Dim4Triangulation* ans = new Dim4Triangulation();
    ans->setLabel("S3 x S1");

    Dim4Pentachoron* p = ans->newPentachoron();
    Dim4Pentachoron* q = ans->newPentachoron();
    p->join(1, q, Perm<5>());
    p->join(2, q, Perm<5>());
    p->join(3, q, Perm<5>());

    // Now join each 0123 to a 1234, which joins up the S1 loop.
    // Do this in the orientation-preserving way.
    p->join(0, q, Perm<5>(4,0,1,2,3));
    q->join(0, p, Perm<5>(4,0,1,2,3));

    // All done.
    return ans;
}

Dim4Triangulation* Dim4ExampleTriangulation::s3xs1Twisted() {
    // Make two pentachora, and join three of the five facets according
    // to the identity map.  Only facets 0123 and 1234 of each
    // pentachoron remain.
    Dim4Triangulation* ans = new Dim4Triangulation();
    ans->setLabel("S3 x~ S1");

    Dim4Pentachoron* p = ans->newPentachoron();
    Dim4Pentachoron* q = ans->newPentachoron();
    p->join(1, q, Perm<5>());
    p->join(2, q, Perm<5>());
    p->join(3, q, Perm<5>());

    // Now join each 0123 to a 1234, which joins up the S1 loop.
    // Do this in the orientation-reversing way.
    p->join(0, p, Perm<5>(4,0,1,2,3));
    q->join(0, q, Perm<5>(4,0,1,2,3));

    // All done.
    return ans;
}

Dim4Triangulation* Dim4ExampleTriangulation::cappellShaneson() {
    // Use the gluings described in arXiv:1109.3899.
    Dim4Triangulation* ans = new Dim4Triangulation();
    ans->setLabel("Cappell-Shaneson knot complement");

    Dim4Pentachoron* p = ans->newPentachoron();
    Dim4Pentachoron* q = ans->newPentachoron();
    q->join(0, p, Perm<5>(2,0,1,3,4));
    q->join(2, p, Perm<5>(0,1,4,2,3));
    q->join(3, p, Perm<5>(0,2,3,1,4));
    p->join(3, p, Perm<5>(1,2,3,0,4));
    q->join(4, q, Perm<5>(0,2,3,4,1));

    return ans;
}

Dim4Triangulation* Dim4ExampleTriangulation::doubleCone(
        const NTriangulation& base) {
    Dim4Triangulation* ans = new Dim4Triangulation();
    Packet::ChangeEventSpan span(ans);
    ans->setLabel("Double cone over " + base.label());

    unsigned long n = base.size();
    if (n == 0)
        return ans;

    // We have at least one tetrahedron.  Off we go.
    Dim4Pentachoron** pent = new Dim4Pentachoron*[2 * n];

    unsigned long i;
    int face;
    unsigned long adjIndex;
    const NTetrahedron *tet, *adjTet;
    Perm<4> map;

    for (i = 0; i < 2 * n; ++i)
        pent[i] = ans->newPentachoron();

    for (i = 0; i < n; ++i) {
        pent[i]->join(4, pent[i + n], Perm<5>());

        tet = base.tetrahedron(i);
        for (face = 0; face < 4; ++face) {
            adjTet = tet->adjacentTetrahedron(face);
            if (adjTet == 0)
                continue;

            adjIndex = adjTet->index();
            if (adjIndex > i)
                continue;

            map = tet->adjacentGluing(face);
            if (adjIndex == i && map[face] > face)
                continue;

            pent[i]->join(face, pent[adjIndex], Perm<5>::extend(map));
            pent[i + n]->join(face, pent[adjIndex + n], Perm<5>::extend(map));
        }
    }

    delete[] pent;
    return ans;
}

Dim4Triangulation* Dim4ExampleTriangulation::singleCone(
        const NTriangulation& base) {
    Dim4Triangulation* ans = new Dim4Triangulation();
    Packet::ChangeEventSpan span(ans);
    ans->setLabel("Single cone over " + base.label());

    unsigned long n = base.size();
    if (n == 0)
        return ans;

    // We have at least one tetrahedron.  Off we go.
    Dim4Pentachoron** pent = new Dim4Pentachoron*[n];

    unsigned long i;
    int face;
    unsigned long adjIndex;
    const NTetrahedron *tet, *adjTet;
    Perm<4> map;
    for (i = 0; i < n; ++i) {
        pent[i] = ans->newPentachoron();

        tet = base.tetrahedron(i);
        for (face = 0; face < 4; ++face) {
            adjTet = tet->adjacentTetrahedron(face);
            if (adjTet == 0)
                continue;

            adjIndex = adjTet->index();
            if (adjIndex > i)
                continue;

            map = tet->adjacentGluing(face);
            if (adjIndex == i && map[face] > face)
                continue;

            pent[i]->join(face, pent[adjIndex], Perm<5>::extend(map));
        }
    }

    delete[] pent;
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
        Dim4Pentachoron* bdry[2];
            /**< The pentachora providing the lower and upper boundaries
                 of the prism. */
        Dim4Pentachoron* wallBase3[2][4];
            /**< wallBase3[i][j] provides a triangle base of the 3-dimensional
                 prism "wall" that runs between facet j of bdry[0] and
                 facet j of bdry[1]. */
        Dim4Pentachoron* wallBase2[2][4][4];
            /**< wallBase2[i][j][k] provides a triangle side of the
                 3-dimensional prism "wall" between facet j of bdry[0] and
                 facet j of bdry[1].  Specifically, this triangle forms
                 a base of the rectangular 2-face of the prism that does not
                 meet vertex k.  Requires j,k distinct. */
        Dim4Pentachoron* wallSide[2][4][4][4];
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
        inline void buildBdry(Dim4Triangulation* tri, int which) {
            bdry[which] = tri->newPentachoron();
        }

        /**
         * Create all remaining pentachora (80 of 82) within this prism.
         */
        inline void buildWalls(Dim4Triangulation* tri) {
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

Dim4Triangulation* Dim4ExampleTriangulation::iBundle(
        const NTriangulation& base) {
    Dim4Triangulation* ans = new Dim4Triangulation();
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
    const NTetrahedron *tet, *adj;
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

Dim4Triangulation* Dim4ExampleTriangulation::s1Bundle(
        const NTriangulation& base) {
    Dim4Triangulation* ans = iBundle(base);
    Packet::ChangeEventSpan span(ans);
    ans->setLabel(base.label() + " x S1");

    Perm<5> id;
    unsigned long n = base.size();
    unsigned long i;
    for (i = 0; i < n; ++i)
        ans->pentachoron(i)->join(4, ans->pentachoron(i + n), id);

    return ans;
}

Dim4Triangulation* Dim4ExampleTriangulation::bundleWithMonodromy(
        const NTriangulation& base, const NIsomorphism& monodromy) {
    Dim4Triangulation* ans = iBundle(base);
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

