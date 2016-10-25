
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

#include "dim2/dim2exampletriangulation.h"
#include "triangulation/dim2.h"
#include <sstream>

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
            ans->triangle(i)->join(1, ans->triangle(i + 1),
                Perm<3>(1, 2));
        ans->triangle(0)->join(0, ans->triangle(n - 1), Perm<3>(0, 1));
        for (i = 1; i < punctures; ++i)
            ans->triangle(3 * i - 2)->join(0, ans->triangle(3 * i),
                Perm<3>(1, 2));
    } else {
        unsigned n = 4 * genus + 3 * punctures - 2;
        unsigned i;
        for (i = 0; i < n; ++i)
            ans->newTriangle();
        for (i = 0; i < n - 1; ++i)
            ans->triangle(i)->join(1, ans->triangle(i + 1),
                Perm<3>(1, 2));
        ans->triangle(0)->join(2, ans->triangle(n - 1), Perm<3>(0, 2));
        ans->triangle(0)->join(0, ans->triangle(n - 1), Perm<3>(0, 1));
        for (i = 1; i < genus; ++i) {
            ans->triangle(4 * i - 3)->join(0, ans->triangle(4 * i - 1),
                Perm<3>(1, 2));
            ans->triangle(4 * i - 2)->join(0, ans->triangle(4 * i),
                Perm<3>(1, 2));
        }
        for (i = 0; i < punctures; ++i)
            ans->triangle(4 * genus + 3 * i - 3)->join(
                0, ans->triangle(4 * genus + 3 * i - 1), Perm<3>(1, 2));
    }

    // Set an appropriate packet label.
    std::ostringstream s;
    s << "Orientable, genus " << genus;
    if (punctures == 1)
        s << ", 1 puncture";
    else if (punctures > 1)
        s << ", " << punctures << " punctures";
    ans->setLabel(s.str());

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
        ans->triangle(i)->join(1, ans->triangle(i + 1),
            Perm<3>(1, 2));
    ans->triangle(0)->join(2, ans->triangle(n - 1), Perm<3>(2, 0, 1));
    for (i = 1; i < genus; ++i)
        ans->triangle(2 * i - 2)->join(0, ans->triangle(2 * i - 1),
            Perm<3>());
    for (i = 0; i < punctures; ++i)
        ans->triangle(2 * genus + 3 * i - 2)->join(
            0, ans->triangle(2 * genus + 3 * i), Perm<3>(1, 2));

    // Set an appropriate packet label.
    std::ostringstream s;
    s << "Non-orientable, genus " << genus;
    if (punctures == 1)
        s << ", 1 puncture";
    else if (punctures > 1)
        s << ", " << punctures << " punctures";
    ans->setLabel(s.str());

    return ans;
}

Dim2Triangulation* Dim2ExampleTriangulation::sphere() {
    Dim2Triangulation* ans = new Dim2Triangulation();
    ans->setLabel("Sphere");

    Dim2Triangle* r = ans->newTriangle();
    Dim2Triangle* s = ans->newTriangle();
    r->join(0, s, Perm<3>());
    r->join(1, s, Perm<3>());
    r->join(2, s, Perm<3>());

    return ans;
}

Dim2Triangulation* Dim2ExampleTriangulation::sphereTetrahedron() {
    Dim2Triangulation* ans = new Dim2Triangulation();
    ans->setLabel("Sphere (tetrahedron boundary)");

    Dim2Triangle* r = ans->newTriangle();
    Dim2Triangle* s = ans->newTriangle();
    Dim2Triangle* t = ans->newTriangle();
    Dim2Triangle* u = ans->newTriangle();
    r->join(1, s, Perm<3>(1, 2));
    s->join(1, t, Perm<3>(1, 2));
    t->join(1, r, Perm<3>(1, 2));
    r->join(0, u, Perm<3>(0, 1, 2));
    s->join(0, u, Perm<3>(1, 2, 0));
    t->join(0, u, Perm<3>(2, 0, 1));

    return ans;
}

Dim2Triangulation* Dim2ExampleTriangulation::sphereOctahedron() {
    Dim2Triangulation* ans = new Dim2Triangulation();
    ans->setLabel("Sphere (octahedron boundary)");

    Dim2Triangle* r = ans->newTriangle();
    Dim2Triangle* s = ans->newTriangle();
    Dim2Triangle* t = ans->newTriangle();
    Dim2Triangle* u = ans->newTriangle();
    Dim2Triangle* v = ans->newTriangle();
    Dim2Triangle* w = ans->newTriangle();
    Dim2Triangle* x = ans->newTriangle();
    Dim2Triangle* y = ans->newTriangle();
    r->join(1, s, Perm<3>(1, 2));
    s->join(1, t, Perm<3>(1, 2));
    t->join(1, u, Perm<3>(1, 2));
    u->join(1, r, Perm<3>(1, 2));
    v->join(1, w, Perm<3>(1, 2));
    w->join(1, x, Perm<3>(1, 2));
    x->join(1, y, Perm<3>(1, 2));
    y->join(1, v, Perm<3>(1, 2));
    r->join(0, v, Perm<3>());
    s->join(0, w, Perm<3>());
    t->join(0, x, Perm<3>());
    u->join(0, y, Perm<3>());

    return ans;
}

Dim2Triangulation* Dim2ExampleTriangulation::disc() {
    Dim2Triangulation* ans = new Dim2Triangulation();
    ans->setLabel("Disc");

    ans->newTriangle();

    return ans;
}

Dim2Triangulation* Dim2ExampleTriangulation::annulus() {
    Dim2Triangulation* ans = new Dim2Triangulation();
    ans->setLabel("Annulus");

    Dim2Triangle* r = ans->newTriangle();
    Dim2Triangle* s = ans->newTriangle();
    r->join(0, s, Perm<3>(1, 2));
    r->join(2, s, Perm<3>(0, 1));

    return ans;
}

Dim2Triangulation* Dim2ExampleTriangulation::mobius() {
    Dim2Triangulation* ans = new Dim2Triangulation();
    ans->setLabel("M\u00F6bius band");

    Dim2Triangle* r = ans->newTriangle();
    r->join(0, r, Perm<3>(2, 0, 1));

    return ans;
}

Dim2Triangulation* Dim2ExampleTriangulation::torus() {
    Dim2Triangulation* ans = new Dim2Triangulation();
    ans->setLabel("Torus");

    Dim2Triangle* r = ans->newTriangle();
    Dim2Triangle* s = ans->newTriangle();
    r->join(0, s, Perm<3>(1, 2));
    r->join(1, s, Perm<3>(2, 0));
    r->join(2, s, Perm<3>(0, 1));

    return ans;
}

Dim2Triangulation* Dim2ExampleTriangulation::rp2() {
    Dim2Triangulation* ans = new Dim2Triangulation();
    ans->setLabel("Projective plane");

    Dim2Triangle* r = ans->newTriangle();
    Dim2Triangle* s = ans->newTriangle();
    r->join(0, s, Perm<3>(1, 2));
    r->join(1, s, Perm<3>());
    r->join(2, s, Perm<3>());

    return ans;
}

Dim2Triangulation* Dim2ExampleTriangulation::kb() {
    Dim2Triangulation* ans = new Dim2Triangulation();
    ans->setLabel("Klein bottle");

    Dim2Triangle* r = ans->newTriangle();
    Dim2Triangle* s = ans->newTriangle();
    r->join(0, s, Perm<3>(1, 2));
    r->join(1, s, Perm<3>(2, 0));
    r->join(2, s, Perm<3>());

    return ans;
}

} // namespace regina

