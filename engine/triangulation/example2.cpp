
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
    // Small cases that don't fit into the general constructions below.
    if (genus == 0) {
        if (punctures == 0) {
            //TODO If sphere() were oriented(), then we could promise
            //  oriented for orientable(), since all the other constructions
            //  below are already oriented.
            return sphere();
        } else if (punctures == 1) {
            return disc();
        }
    }

    Triangulation<2> ans;

    if (punctures == 0) {
        // Already handled the sphere, so genus >= 1.
        //
        // The size of a minimal triangulation is 4*genus - 2.
        //
        // This is essentially the same as the old implementation, but without
        // the punctures part of the construction (which was non-minimal).
        unsigned n = 4 * genus - 2;
        ans = polygon(n);
        ans.triangle(0)->join(2, ans.triangle(n - 1), Perm<3>(0, 2));
        ans.triangle(0)->join(0, ans.triangle(n - 1), Perm<3>(0, 1));
        for (unsigned i = 1; i < genus; ++i) {
            ans.triangle(4 * i - 3)->join(0, ans.triangle(4 * i - 1),
                Perm<3>(1, 2));
            ans.triangle(4 * i - 2)->join(0, ans.triangle(4 * i),
                Perm<3>(1, 2));
        }
    } else if (punctures == 1) {
        // Already handled the disc, so genus >= 1.
        //
        // The size of a minimal triangulation is 4*genus - 1.
        ans = polygon( 4*genus - 1 );
        for (unsigned handle = 0; handle < genus; ++handle) {
            for (unsigned faceIndex : {4*handle, 4*handle + 1}) {
                if ( faceIndex == 4*genus - 3 ) {
                    // The very last gluing needs to be handled differently
                    // from the others.
                    ans.triangle(faceIndex)->join(
                            0, ans.triangle(4*genus - 2), Perm<3>(0, 1) );
                } else {
                    ans.triangle(faceIndex)->join(
                            0, ans.triangle(faceIndex + 2), Perm<3>(1, 2) );
                }
            }
        }
    } else if (genus == 0) {
        // Already handled the sphere and the disc, so punctures >= 2.
        //
        // The size of a minimal triangulation is 3*punctures - 4.
        ans = polygon( 3*punctures - 4 );
        for (unsigned i = 0; i < punctures - 1; ++i) {
            if (i == punctures - 2) {
                // The very last gluing needs to be handled differently
                // from the others.
                ans.triangle(3*i)->join(
                        0, ans.triangle( 3*punctures - 5 ), Perm<3>(0, 1) );
            } else {
                ans.triangle(3*i)->join(
                        0, ans.triangle( 3*i + 2 ), Perm<3>(1, 2) );
            }
        }
    } else {
        // All that remains are the cases where genus >= 1 and punctures >= 2.
        //
        // The size of a minimal triangulation is 4*genus + 3*punctures - 4.
        //
        // We construct this by starting with one puncture (which has
        // 4*genus - 1 triangles), and then adding all the extra punctures by
        // attaching a gadget with 3*punctures - 3 triangles.
        ans = orientable( genus, 1 );
        addPunctures( ans, punctures );
    }

    return ans;
}

Triangulation<2> Example<2>::nonOrientable(unsigned genus, unsigned punctures) {
    if (genus == 0)
        return orientable(0, punctures); // Just in case. *shrug*
    if (genus == 1 && punctures == 0)
        return rp2(); // Avoid 2-gons.

    Triangulation<2> ans;

    if (punctures == 0) {
        // Already handled RP^2, so genus >= 2.
        //
        // The size of a minimal triangulation is 2*genus - 2.
        //
        // This is essentially the same as the old implementation, but without
        // the punctures part of the construction (which was non-minimal).
        unsigned n = 2 * genus - 2;
        ans = polygon(n);
        ans.triangle(0)->join(2, ans.triangle(n - 1), Perm<3>(2, 0, 1));
        for (unsigned i = 1; i < genus; ++i)
            ans.triangle(2 * i - 2)->join(
                    0, ans.triangle(2 * i - 1), Perm<3>());
    } else if (punctures == 1) {
        // Trivially, we have genus >= 1.
        //
        // The size of a minimal triangulation is 2*genus - 1.
        ans = polygon(2*genus - 1);
        for (unsigned crosscap = 0; crosscap < genus; ++crosscap) {
            if (crosscap == genus - 1) {
                // The very last gluing needs to be handled differently from
                // the others.
                ans.triangle(2*crosscap)->join(
                        0, ans.triangle(2*crosscap), Perm<3>(1, 2, 0) );
            } else {
                ans.triangle(2*crosscap)->join(
                        0, ans.triangle(2*crosscap + 1), Perm<3>() );
            }
        }
    } else {
        // All that remains are the cases where punctures >= 2. Again,
        // trivially, we have genus >= 1.
        //
        // The size of a minimal triangulation is 2*genus + 3*punctures - 4.
        //
        // We construct this by starting with one puncture (which has
        // 2*genus - 1 triangles), and then adding all the extra punctures by
        // attaching a gadget with 3*punctures - 3 triangles.
        ans = nonOrientable( genus, 1 );
        addPunctures( ans, punctures );
    }

    return ans;
}

Triangulation<2> Example<2>::polygon(unsigned n) {
    // NOTE: The following routines all rely on this specific construction
    //      --> Example<2>::orientable()
    //      --> Example<2>::nonOrientable()
    //      --> SFSpace::construct()
    Triangulation<2> ans;
    ans.newTriangles(n);
    for (unsigned i = 1; i < n; ++i) {
        ans.triangle(i)->join( 2, ans.triangle(i - 1), Perm<3>(1, 2) );
    }
    return ans;
}

void Example<2>::addPunctures(Triangulation<2>& surf, unsigned punctures) {
    // NOTE: The following routines rely on this specific construction
    //      --> Example<2>::orientable()
    //      --> Example<2>::nonOrientable()
    size_t initSize = surf.size();
    surf.insertTriangulation( polygon(3*punctures - 3) );
    for (unsigned i = 0; i < punctures - 1; ++i) {
        surf.triangle( initSize + 3*i )->join(
                0, surf.triangle( initSize + 3*i + 2 ), Perm<3>(1, 2) );
    }
    surf.triangle(0)->join( 2, surf.triangle(initSize), Perm<3>(0, 1) );
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

