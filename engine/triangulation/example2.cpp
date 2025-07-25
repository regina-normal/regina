
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

#include "triangulation/dim2.h"
#include "triangulation/example2.h"
#include <sstream>

namespace regina {

Triangulation<2> Example<2>::orientable(unsigned genus, unsigned punctures) {
    if (genus == 0 && punctures == 0)
        return sphere();

    Triangulation<2> ans;

    if (genus == 0) {
        // Fact: punctures >= 1.
        unsigned n = 3 * punctures - 2;
        unsigned i;
        ans.newTriangles(n);
        for (i = 0; i < n - 1; ++i)
            ans.triangle(i)->join(1, ans.triangle(i + 1), Perm<3>(1, 2));
        ans.triangle(0)->join(0, ans.triangle(n - 1), Perm<3>(0, 1));
        for (i = 1; i < punctures; ++i)
            ans.triangle(3 * i - 2)->join(0, ans.triangle(3 * i),
                Perm<3>(1, 2));
    } else {
        unsigned n = 4 * genus + 3 * punctures - 2;
        unsigned i;
        ans.newTriangles(n);
        for (i = 0; i < n - 1; ++i)
            ans.triangle(i)->join(1, ans.triangle(i + 1), Perm<3>(1, 2));
        ans.triangle(0)->join(2, ans.triangle(n - 1), Perm<3>(0, 2));
        ans.triangle(0)->join(0, ans.triangle(n - 1), Perm<3>(0, 1));
        for (i = 1; i < genus; ++i) {
            ans.triangle(4 * i - 3)->join(0, ans.triangle(4 * i - 1),
                Perm<3>(1, 2));
            ans.triangle(4 * i - 2)->join(0, ans.triangle(4 * i),
                Perm<3>(1, 2));
        }
        for (i = 0; i < punctures; ++i)
            ans.triangle(4 * genus + 3 * i - 3)->join(
                0, ans.triangle(4 * genus + 3 * i - 1), Perm<3>(1, 2));
    }

    return ans;
}

Triangulation<2> Example<2>::nonOrientable(unsigned genus, unsigned punctures) {
    if (genus == 0)
        return orientable(0, punctures); // Just in case. *shrug*
    if (genus == 1 && punctures == 0)
        return rp2(); // Avoid 2-gons.

    Triangulation<2> ans;

    // The generic code below will create one internal vertex, and one for
    // each puncture.  This is minimal for zero punctures, but non-minimal
    // otherwise.  For now, we use a different triangulation for the
    // once-punctured case so at least that gets to be minimal also;
    // ideally these should be minimal for all values of punctures.

    if (punctures == 1) {
        // Thanks to Alex He for this code.

        // Let g denote the given genus. We use g-1 "inner" triangles and g
        // "outer" triangles, for a total of 2*g-1 triangles. We start by using
        // the g-1 "inner" triangles to build a (g+1)-sided polygon P. We then
        // form each of the g "outer" triangles into a one-triangle Mobius band,
        // and attach the boundary of each of these Mobius bands to one of the
        // sides of P. It is clear that the resulting surface is once-punctured
        // and one-vertex, and has non-orientable genus g.
        unsigned n = 2*genus - 1;
        unsigned i;
        ans.newTriangles(n);
        // Form "outer" triangles into Mobius bands.
        for ( i = genus - 1; i < n; ++i ) {
            ans.triangle(i)->join(
                    0, ans.triangle(i), Perm<3>(1, 2, 0) );
        }
        // Glue everything together.
        for ( i = 1; i < n; ++i ) {
            ans.triangle(i)->join(
                    2, ans.triangle( (i-1)/2 ), Perm<3>( 2, i%2 ) );
        }
    } else {
        unsigned n = 2 * genus + 3 * punctures - 2;
        unsigned i;
        ans.newTriangles(n);
        for (i = 0; i < n - 1; ++i)
            ans.triangle(i)->join(1, ans.triangle(i + 1), Perm<3>(1, 2));
        ans.triangle(0)->join(2, ans.triangle(n - 1), Perm<3>(2, 0, 1));
        for (i = 1; i < genus; ++i)
            ans.triangle(2 * i - 2)->join(0, ans.triangle(2 * i - 1), Perm<3>());
        for (i = 0; i < punctures; ++i)
            ans.triangle(2 * genus + 3 * i - 2)->join(
                0, ans.triangle(2 * genus + 3 * i), Perm<3>(1, 2));
    }

    return ans;
}

Triangulation<2> Example<2>::sphereOctahedron() {
    Triangulation<2> ans;

    auto [r, s, t, u, v, w, x, y] = ans.newTriangles<8>();
    r->join(1, s, Perm<3>(1, 2));
    s->join(1, t, Perm<3>(1, 2));
    t->join(1, u, Perm<3>(1, 2));
    u->join(1, r, Perm<3>(1, 2));
    v->join(2, w, Perm<3>(1, 2));
    w->join(2, x, Perm<3>(1, 2));
    x->join(2, y, Perm<3>(1, 2));
    y->join(2, v, Perm<3>(1, 2));
    r->join(0, v, Perm<3>(1, 2));
    s->join(0, w, Perm<3>(1, 2));
    t->join(0, x, Perm<3>(1, 2));
    u->join(0, y, Perm<3>(1, 2));

    return ans;
}

Triangulation<2> Example<2>::rp2() {
    Triangulation<2> ans;

    auto [r, s] = ans.newTriangles<2>();
    r->join(0, s, Perm<3>(1, 2));
    r->join(1, s, Perm<3>());
    r->join(2, s, Perm<3>());

    return ans;
}

} // namespace regina

