
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

#include "triangulation/dim2.h"

namespace regina {

bool Triangulation<2>::oneThreeMove(Triangle<2>* tri, bool /* check */,
        bool perform) {
    if ( !perform )
        return true; // You can always do this move.

    ChangeEventSpan span(this);

    // Before we unglue, record how the adjacent triangles are glued to tri.
    Triangle<2>* adjTri[3];
    Perm<3> adjGlue[3];
    unsigned i, j;
    for (i=0; i<3; i++) {
        adjTri[i] = tri->adjacentTriangle(i);
        if (adjTri[i])
            adjGlue[i] = tri->adjacentGluing(i);
    }

    // Unglue the old triangle.
    tri->isolate();

    // The new triangles.
    // Edge i of the old triangle will become a edge of newTri[i].
    // Vertex i of newTri[i] will become the new internal vertex, and
    // the other two vertices of newTri[i] will keep the same vertex numbers
    // that they had in the old triangle.
    Triangle<2>* newTri[3];
    for (i = 0; i < 3; ++i)
        newTri[i] = newTriangle();

    // Glue the new triangles to each other internally.
    for (i = 0; i < 3; ++i)
        for (j = i + 1; j < 3; ++j)
            newTri[i]->join(j, newTri[j], Perm<3>(i, j));

    // Attach the new triangles to the old triangulation.
    for (i = 0; i < 3; ++i) {
        if (adjTri[i] == tri) {
            // The old triangle was glued to itself.

            // We might have already made this gluing from the other side:
            if (newTri[i]->adjacentTriangle(i))
                continue;

            // Nope, do it now.
            newTri[i]->join(i, newTri[adjGlue[i][i]], adjGlue[i]);
        } else if (adjTri[i]) {
            // The old triangle was glued elsewhere.
            newTri[i]->join(i, adjTri[i], adjGlue[i]);
        }
    }

    // Delete the old triangle.
    removeTriangle(tri);

    // All done!
    return true;
}

} // namespace regina
