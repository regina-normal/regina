
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "triangulation/ntriangulation.h"

NTetrahedron* NTriangulation::insertLayeredSolidTorus(
        unsigned long cuts0, unsigned long cuts1) {
    unsigned long cuts2 = cuts0 + cuts1;

    NTetrahedron* newTet = new NTetrahedron();
    addTetrahedron(newTet);

    // Take care of the case that can be done with a single
    // tetrahedron.
    if (cuts2 == 3) {
        // Must be a 1-2-3 arrangement that can be done with a single
        // tetrahedron.
        newTet->joinTo(0, newTet, NPerm(1,2,3,0));
        gluingsHaveChanged();
        return newTet;
    }

    // Take care of the special small cases.
    if (cuts2 == 2) {
        // Make a 1-2-1 arrangement.
        NTetrahedron* base = insertLayeredSolidTorus(1, 2);
        base->joinTo(2, newTet, NPerm(2,3,0,1));
        base->joinTo(3, newTet, NPerm(2,3,0,1));
        gluingsHaveChanged();
        return newTet;
    }
    if (cuts2 == 1) {
        // Make a 1-1-0 arrangement.
        NTetrahedron* base = insertLayeredSolidTorus(1, 1);
        base->joinTo(2, newTet, NPerm(0,2,1,3));
        base->joinTo(3, newTet, NPerm(3,1,2,0));
        gluingsHaveChanged();
        return newTet;
    }

    // At this point we know cuts2 > 3.  Recursively build the layered
    // triangulation.
    if (cuts1 - cuts0 > cuts0) {
        NTetrahedron* base = insertLayeredSolidTorus(cuts0, cuts1 - cuts0);
        base->joinTo(2, newTet, NPerm(0,2,1,3));
        base->joinTo(3, newTet, NPerm(3,1,2,0));
    } else {
        NTetrahedron* base = insertLayeredSolidTorus(cuts1 - cuts0, cuts0);
        base->joinTo(2, newTet, NPerm(3,1,0,2));
        base->joinTo(3, newTet, NPerm(0,2,3,1));
    }
    gluingsHaveChanged();
    return newTet;
}

void NTriangulation::insertLensSpace(unsigned long p, unsigned long q) {
    NTetrahedron* chain = insertLayeredSolidTorus(q, p);
    NTetrahedron* cap = insertLayeredSolidTorus(0, 1);
    chain->joinTo(2, cap, NPerm(2,0,3,1));
    chain->joinTo(3, cap, NPerm(1,3,0,2));
    gluingsHaveChanged();
}

