
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

#include "triangulation/ntetrahedron.h"

NTetrahedron::NTetrahedron() {
    int i;
    for (i=0; i<4; i++)
        tetrahedra[i] = 0;
}

NTetrahedron::NTetrahedron(const NString& desc) : description(desc) {
    int i;
    for (i=0; i<4; i++)
        tetrahedra[i] = 0;
}

bool NTetrahedron::hasBoundary() const {
    for (int i=0; i<4; i++)
        if (tetrahedra[i] == 0)
            return true;
    return false;
}

void NTetrahedron::isolate() {
    for (int i=0; i<4; i++)
        if (tetrahedra[i])
            unjoin(i);
}

NTetrahedron* NTetrahedron::unjoin(int myFace) {
    NTetrahedron* you = tetrahedra[myFace];
    int yourFace = tetrahedronPerm[myFace][myFace];
    you->tetrahedra[yourFace] = 0;
    tetrahedra[myFace] = 0;
    return you;
}

void NTetrahedron::joinTo(int myFace, NTetrahedron* you, NPerm gluing) {
    tetrahedra[myFace] = you;
    tetrahedronPerm[myFace] = gluing;
    int yourFace = gluing[myFace];
    you->tetrahedra[yourFace] = this;
    you->tetrahedronPerm[yourFace] = gluing.inverse();
}

