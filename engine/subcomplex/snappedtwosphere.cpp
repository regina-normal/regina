
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

#include "subcomplex/snappedtwosphere.h"
#include "subcomplex/snappedball.h"
#include "triangulation/dim3.h"

namespace regina {

SnappedTwoSphere* SnappedTwoSphere::clone() const {
    SnappedTwoSphere* ans = new SnappedTwoSphere();
    ans->ball[0] = ball[0]->clone();
    ans->ball[1] = ball[1]->clone();
    return ans;
}

SnappedTwoSphere* SnappedTwoSphere::formsSnappedTwoSphere(
        Tetrahedron<3>* tet1, Tetrahedron<3>* tet2) {
    SnappedBall* ball[2];
    if (! (ball[0] = SnappedBall::formsSnappedBall(tet1)))
        return 0;
    if (! (ball[1] = SnappedBall::formsSnappedBall(tet2))) {
        delete ball[0];
        return 0;
    }
    if (tet1->edge(ball[0]->equatorEdge()) !=
            tet2->edge(ball[1]->equatorEdge())) {
        delete ball[0];
        delete ball[1];
        return 0;
    }

    // This is it.
    SnappedTwoSphere* ans = new SnappedTwoSphere();
    ans->ball[0] = ball[0];
    ans->ball[1] = ball[1];
    return ans;
}

SnappedTwoSphere* SnappedTwoSphere::formsSnappedTwoSphere(
        SnappedBall* ball1, SnappedBall* ball2) {
    if (ball1->tetrahedron()->edge(ball1->equatorEdge()) !=
            ball2->tetrahedron()->edge(ball2->equatorEdge()))
        return 0;

    // This is it.
    SnappedTwoSphere* ans = new SnappedTwoSphere();
    ans->ball[0] = ball1->clone();
    ans->ball[1] = ball2->clone();
    return ans;
}

} // namespace regina

