
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

#include "dim2/dim2exampletriangulation.h"
#include "dim2/dim2triangulation.h"

namespace regina {

Dim2Triangulation* Dim2ExampleTriangulation::orientable(
        unsigned genus, unsigned punctures) {
    Dim2Triangulation* ans = new Dim2Triangulation();

    if (genus == 0) {
        if (punctures == 0)
            return sphere();

        unsigned n = 3 * punctures - 2;
        unsigned i;
        for (i = 0; i < n; ++i)
            ans->newTriangle();
        for (i = 0; i < n - 1; ++i)
            ans->triangle(i)->joinTo(1, ans->triangle(i + 1),
                NPerm3(1, 2));
        ans->triangle(0)->joinTo(0, ans->triangle(n - 1), NPerm3(0, 1));
        for (i = 1; i < punctures; ++i)
            ans->triangle(3 * i - 2)->joinTo(0, ans->triangle(3 * i),
                NPerm3(1, 2));
    } else {
        unsigned n = 4 * genus + 3 * punctures - 2;
        unsigned i;
        for (i = 0; i < n; ++i)
            ans->newTriangle();
        for (i = 0; i < n - 1; ++i)
            ans->triangle(i)->joinTo(1, ans->triangle(i + 1),
                NPerm3(1, 2));
        ans->triangle(0)->joinTo(2, ans->triangle(n - 1), NPerm3(0, 2));
        ans->triangle(0)->joinTo(0, ans->triangle(n - 1), NPerm3(0, 1));
        for (i = 1; i < genus; ++i) {
            ans->triangle(4 * i - 3)->joinTo(0, ans->triangle(4 * i - 1),
                NPerm3(1, 2));
            ans->triangle(4 * i - 2)->joinTo(0, ans->triangle(4 * i),
                NPerm3(1, 2));
        }
        for (i = 0; i < punctures; ++i)
            ans->triangle(4 * genus + 3 * i - 3)->joinTo(
                0, ans->triangle(4 * genus + 3 * i - 1), NPerm3(1, 2));
    }

    return ans;
}

Dim2Triangulation* Dim2ExampleTriangulation::nonOrientable(
        unsigned genus, unsigned punctures) {
    if (genus == 0)
        return orientable(0, punctures); // Just in case. *shrug*
    if (genus == 1 && punctures == 0)
        return rp2(); // Avoid 2-gons.

    Dim2Triangulation* ans = new Dim2Triangulation();

    unsigned n = 2 * genus + 3 * punctures - 2;
    unsigned i;
    for (i = 0; i < n; ++i)
        ans->newTriangle();
    for (i = 0; i < n - 1; ++i)
        ans->triangle(i)->joinTo(1, ans->triangle(i + 1),
            NPerm3(1, 2));
    ans->triangle(0)->joinTo(2, ans->triangle(n - 1), NPerm3(2, 0, 1));
    for (i = 1; i < genus; ++i)
        ans->triangle(2 * i - 2)->joinTo(0, ans->triangle(2 * i - 1),
            NPerm3());
    for (i = 0; i < punctures; ++i)
        ans->triangle(2 * genus + 3 * i - 2)->joinTo(
            0, ans->triangle(2 * genus + 3 * i), NPerm3(1, 2));

    return ans;
}

Dim2Triangulation* Dim2ExampleTriangulation::sphere() {
    Dim2Triangulation* ans = new Dim2Triangulation();
    ans->setPacketLabel("Sphere");

    Dim2Triangle* r = ans->newTriangle();
    Dim2Triangle* s = ans->newTriangle();
    r->joinTo(0, s, NPerm3());
    r->joinTo(1, s, NPerm3());
    r->joinTo(2, s, NPerm3());

    return ans;
}

Dim2Triangulation* Dim2ExampleTriangulation::sphereTetrahedron() {
    Dim2Triangulation* ans = new Dim2Triangulation();
    ans->setPacketLabel("Sphere (tetrahedron boundary)");

    Dim2Triangle* r = ans->newTriangle();
    Dim2Triangle* s = ans->newTriangle();
    Dim2Triangle* t = ans->newTriangle();
    Dim2Triangle* u = ans->newTriangle();
    r->joinTo(1, s, NPerm3(1, 2));
    s->joinTo(1, t, NPerm3(1, 2));
    t->joinTo(1, r, NPerm3(1, 2));
    r->joinTo(0, u, NPerm3(0, 1, 2));
    s->joinTo(0, u, NPerm3(1, 2, 0));
    t->joinTo(0, u, NPerm3(2, 0, 1));

    return ans;
}

Dim2Triangulation* Dim2ExampleTriangulation::sphereOctahedron() {
    Dim2Triangulation* ans = new Dim2Triangulation();
    ans->setPacketLabel("Sphere (octahedron boundary)");

    Dim2Triangle* r = ans->newTriangle();
    Dim2Triangle* s = ans->newTriangle();
    Dim2Triangle* t = ans->newTriangle();
    Dim2Triangle* u = ans->newTriangle();
    Dim2Triangle* v = ans->newTriangle();
    Dim2Triangle* w = ans->newTriangle();
    Dim2Triangle* x = ans->newTriangle();
    Dim2Triangle* y = ans->newTriangle();
    r->joinTo(1, s, NPerm3(1, 2));
    s->joinTo(1, t, NPerm3(1, 2));
    t->joinTo(1, u, NPerm3(1, 2));
    u->joinTo(1, r, NPerm3(1, 2));
    v->joinTo(1, w, NPerm3(1, 2));
    w->joinTo(1, x, NPerm3(1, 2));
    x->joinTo(1, y, NPerm3(1, 2));
    y->joinTo(1, v, NPerm3(1, 2));
    r->joinTo(0, v, NPerm3());
    s->joinTo(0, w, NPerm3());
    t->joinTo(0, x, NPerm3());
    u->joinTo(0, y, NPerm3());

    return ans;
}

Dim2Triangulation* Dim2ExampleTriangulation::disc() {
    Dim2Triangulation* ans = new Dim2Triangulation();
    ans->setPacketLabel("Disc");

    ans->newTriangle();

    return ans;
}

Dim2Triangulation* Dim2ExampleTriangulation::annulus() {
    Dim2Triangulation* ans = new Dim2Triangulation();
    ans->setPacketLabel("Annulus");

    Dim2Triangle* r = ans->newTriangle();
    Dim2Triangle* s = ans->newTriangle();
    r->joinTo(0, s, NPerm3(1, 2));
    r->joinTo(2, s, NPerm3(0, 1));

    return ans;
}

Dim2Triangulation* Dim2ExampleTriangulation::mobius() {
    Dim2Triangulation* ans = new Dim2Triangulation();
    ans->setPacketLabel("Mobius band");

    Dim2Triangle* r = ans->newTriangle();
    r->joinTo(0, r, NPerm3(2, 0, 1));

    return ans;
}

Dim2Triangulation* Dim2ExampleTriangulation::torus() {
    Dim2Triangulation* ans = new Dim2Triangulation();
    ans->setPacketLabel("Torus");

    Dim2Triangle* r = ans->newTriangle();
    Dim2Triangle* s = ans->newTriangle();
    r->joinTo(0, s, NPerm3(1, 2));
    r->joinTo(1, s, NPerm3(2, 0));
    r->joinTo(2, s, NPerm3(0, 1));

    return ans;
}

Dim2Triangulation* Dim2ExampleTriangulation::rp2() {
    Dim2Triangulation* ans = new Dim2Triangulation();
    ans->setPacketLabel("Projective plane");

    Dim2Triangle* r = ans->newTriangle();
    Dim2Triangle* s = ans->newTriangle();
    r->joinTo(0, s, NPerm3(1, 2));
    r->joinTo(1, s, NPerm3());
    r->joinTo(2, s, NPerm3());

    return ans;
}

Dim2Triangulation* Dim2ExampleTriangulation::kb() {
    Dim2Triangulation* ans = new Dim2Triangulation();
    ans->setPacketLabel("Klein bottle");

    Dim2Triangle* r = ans->newTriangle();
    Dim2Triangle* s = ans->newTriangle();
    r->joinTo(0, s, NPerm3(1, 2));
    r->joinTo(1, s, NPerm3(2, 0));
    r->joinTo(2, s, NPerm3());

    return ans;
}

} // namespace regina

