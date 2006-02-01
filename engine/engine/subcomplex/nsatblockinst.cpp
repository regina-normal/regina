
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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

#include "subcomplex/nsatblockinst.h"

namespace regina {

NSatTriPrismInstance::NSatTriPrismInstance(bool major) :
        NSatTriPrism(major), NSatBlockInstance() {
    NTetrahedron* a = new NTetrahedron();
    NTetrahedron* b = new NTetrahedron();
    NTetrahedron* c = new NTetrahedron();
    a->joinTo(1, c, NPerm(2, 0, 3, 1));
    b->joinTo(1, a, NPerm(2, 0, 3, 1));
    c->joinTo(1, b, NPerm(2, 0, 3, 1));
    tri_.addTetrahedron(a);
    tri_.addTetrahedron(b);
    tri_.addTetrahedron(c);

    const NPerm id;
    const NPerm pairSwap(1, 0, 3, 2);
    annulus_[0].tet[0] = a;
    annulus_[0].tet[1] = b;
    annulus_[0].roles[0] = id;
    annulus_[0].roles[1] = pairSwap;
    annulus_[1].tet[0] = b;
    annulus_[1].tet[1] = c;
    annulus_[1].roles[0] = id;
    annulus_[1].roles[1] = pairSwap;
    annulus_[2].tet[0] = c;
    annulus_[2].tet[1] = a;
    annulus_[2].roles[0] = id;
    annulus_[2].roles[1] = pairSwap;

    if (! major) {
        major_ = false;
        annulus_[0].reflectVertical();
        annulus_[1].reflectVertical();
        annulus_[2].reflectVertical();
    }
}

} // namespace regina

