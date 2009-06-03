
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

#include "dim4/dim4exampletriangulation.h"
#include "dim4/dim4triangulation.h"
#include "maths/permconv.h"
#include "triangulation/ntriangulation.h"

namespace regina {

Dim4Triangulation* Dim4ExampleTriangulation::fourSphere() {
    // Take two pentachora and join their entire boundaries according to
    // the identity map.
    Dim4Triangulation* ans = new Dim4Triangulation();
    ans->setPacketLabel("4-sphere");

    Dim4Pentachoron* p = new Dim4Pentachoron();
    Dim4Pentachoron* q = new Dim4Pentachoron();
    p->joinTo(0, q, NPerm5());
    p->joinTo(1, q, NPerm5());
    p->joinTo(2, q, NPerm5());
    p->joinTo(3, q, NPerm5());
    p->joinTo(4, q, NPerm5());
    ans->addPentachoron(p);
    ans->addPentachoron(q);

    return ans;
}

Dim4Triangulation* Dim4ExampleTriangulation::rp4() {
    Dim4Triangulation* ans = new Dim4Triangulation();
    ans->setPacketLabel("Real projective 4-space");

    // Thanks Ryan, you rock. :)
    Dim4Pentachoron* p = new Dim4Pentachoron();
    Dim4Pentachoron* q = new Dim4Pentachoron();
    Dim4Pentachoron* r = new Dim4Pentachoron();
    Dim4Pentachoron* s = new Dim4Pentachoron();
    p->joinTo(0, s, NPerm5(1,0,3,2,4));
    p->joinTo(1, s, NPerm5(1,0,3,2,4));
    p->joinTo(2, q, NPerm5());
    p->joinTo(3, q, NPerm5());
    p->joinTo(4, r, NPerm5());
    q->joinTo(0, r, NPerm5(1,0,3,2,4));
    q->joinTo(1, r, NPerm5(1,0,3,2,4));
    q->joinTo(4, s, NPerm5());
    r->joinTo(2, s, NPerm5());
    r->joinTo(3, s, NPerm5());
    ans->addPentachoron(p);
    ans->addPentachoron(q);
    ans->addPentachoron(r);
    ans->addPentachoron(s);

    return ans;
}

Dim4Triangulation* Dim4ExampleTriangulation::s3xs1() {
    // Make two pentachora, and join three of the five facets according
    // to the identity map.  Only facets 0123 and 1234 of each
    // pentachoron remain.
    Dim4Triangulation* ans = new Dim4Triangulation();
    ans->setPacketLabel("S3 x S1");

    Dim4Pentachoron* p = new Dim4Pentachoron();
    Dim4Pentachoron* q = new Dim4Pentachoron();
    p->joinTo(1, q, NPerm5());
    p->joinTo(2, q, NPerm5());
    p->joinTo(3, q, NPerm5());

    // Now join each 0123 to a 1234, which joins up the S1 loop.
    // Do this in the orientation-preserving way.
    p->joinTo(0, q, NPerm5(4,0,1,2,3));
    q->joinTo(0, p, NPerm5(4,0,1,2,3));

    // All done.
    ans->addPentachoron(p);
    ans->addPentachoron(q);
    return ans;
}

Dim4Triangulation* Dim4ExampleTriangulation::s3xs1Twisted() {
    // Make two pentachora, and join three of the five facets according
    // to the identity map.  Only facets 0123 and 1234 of each
    // pentachoron remain.
    Dim4Triangulation* ans = new Dim4Triangulation();
    ans->setPacketLabel("S3 x~ S1");

    Dim4Pentachoron* p = new Dim4Pentachoron();
    Dim4Pentachoron* q = new Dim4Pentachoron();
    p->joinTo(1, q, NPerm5());
    p->joinTo(2, q, NPerm5());
    p->joinTo(3, q, NPerm5());

    // Now join each 0123 to a 1234, which joins up the S1 loop.
    // Do this in the orientation-reversing way.
    p->joinTo(0, p, NPerm5(4,0,1,2,3));
    q->joinTo(0, q, NPerm5(4,0,1,2,3));

    // All done.
    ans->addPentachoron(p);
    ans->addPentachoron(q);
    return ans;
}

Dim4Triangulation* Dim4ExampleTriangulation::doubleCone(
        const NTriangulation& base) {
    Dim4Triangulation* ans = new Dim4Triangulation();
    ans->setPacketLabel("Double cone over " + base.getPacketLabel());

    unsigned long n = base.getNumberOfTetrahedra();
    if (n == 0)
        return ans;

    // We have at least one tetrahedron.  Off we go.
    Dim4Pentachoron** pent = new Dim4Pentachoron*[2 * n];

    unsigned long i;
    int face;
    unsigned long adjIndex;
    const NTetrahedron *tet, *adjTet;
    NPerm4 map;
    for (i = 0; i < n; ++i) {
        pent[i] = new Dim4Pentachoron();
        pent[i + n] = new Dim4Pentachoron();
        pent[i]->joinTo(4, pent[i + n], NPerm5());

        tet = base.getTetrahedron(i);
        for (face = 0; face < 4; ++face) {
            adjTet = tet->adjacentTetrahedron(face);
            if (adjTet == 0)
                continue;

            adjIndex = base.tetrahedronIndex(adjTet);
            if (adjIndex > i)
                continue;

            map = tet->adjacentGluing(face);
            if (adjIndex == i && map[face] > face)
                continue;

            pent[i]->joinTo(face, pent[adjIndex], perm4to5(map));
            pent[i + n]->joinTo(face, pent[adjIndex + n], perm4to5(map));
        }
    }

    for (i = 0; i < 2 * n; ++i)
        ans->addPentachoron(pent[i]);

    delete[] pent;
    return ans;
}

Dim4Triangulation* Dim4ExampleTriangulation::singleCone(
        const NTriangulation& base) {
    Dim4Triangulation* ans = new Dim4Triangulation();
    ans->setPacketLabel("Single cone over " + base.getPacketLabel());

    unsigned long n = base.getNumberOfTetrahedra();
    if (n == 0)
        return ans;

    // We have at least one tetrahedron.  Off we go.
    Dim4Pentachoron** pent = new Dim4Pentachoron*[n];

    unsigned long i;
    int face;
    unsigned long adjIndex;
    const NTetrahedron *tet, *adjTet;
    NPerm4 map;
    for (i = 0; i < n; ++i) {
        pent[i] = new Dim4Pentachoron();

        tet = base.getTetrahedron(i);
        for (face = 0; face < 4; ++face) {
            adjTet = tet->adjacentTetrahedron(face);
            if (adjTet == 0)
                continue;

            adjIndex = base.tetrahedronIndex(adjTet);
            if (adjIndex > i)
                continue;

            map = tet->adjacentGluing(face);
            if (adjIndex == i && map[face] > face)
                continue;

            pent[i]->joinTo(face, pent[adjIndex], perm4to5(map));
        }
    }

    for (i = 0; i < n; ++i)
        ans->addPentachoron(pent[i]);

    delete[] pent;
    return ans;
}

} // namespace regina

