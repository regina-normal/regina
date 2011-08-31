
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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

#include <cassert>
#include "triangulation/ntetrahedron.h"
#include "triangulation/ntriangulation.h"

namespace regina {

NTetrahedron::NTetrahedron() : tri(0) {
    for (int i=0; i<4; i++)
        tetrahedra[i] = 0;
}

NTetrahedron::NTetrahedron(const std::string& desc) :
        description(desc), tri(0) {
    for (int i=0; i<4; i++)
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
    // TODO: Make this a stack variable once we know that tri != 0 always.
    std::auto_ptr<NPacket::ChangeEventSpan>
        span(tri ? new NPacket::ChangeEventSpan(tri) : 0);

    NTetrahedron* you = tetrahedra[myFace];
    int yourFace = tetrahedronPerm[myFace][myFace];
    assert(you);
    assert(you->tetrahedra[yourFace]);
    you->tetrahedra[yourFace] = 0;
    tetrahedra[myFace] = 0;

    if (tri)
        tri->clearAllProperties();

    return you;
}

void NTetrahedron::joinTo(int myFace, NTetrahedron* you, NPerm4 gluing) {
    // TODO: Make this a stack variable once we know that tri != 0 always.
    std::auto_ptr<NPacket::ChangeEventSpan>
        span(tri ? new NPacket::ChangeEventSpan(tri) :
        you->tri ? new NPacket::ChangeEventSpan(you->tri) : 0);

    assert((! tetrahedra[myFace]) ||
        (tetrahedra[myFace] == you &&
            tetrahedronPerm[myFace] == gluing));

    // TODO: Temporary measure while we transition from old-style to
    // new-style tetrahedron management.
    if (tri && ! you->tri)
        tri->addTetrahedron(you);
    else if (you->tri && ! tri)
        you->tri->addTetrahedron(this);

    assert(tri == you->tri);

    tetrahedra[myFace] = you;
    tetrahedronPerm[myFace] = gluing;
    int yourFace = gluing[myFace];
    assert((! you->tetrahedra[yourFace]) ||
        (you->tetrahedra[yourFace] == this &&
            you->tetrahedronPerm[yourFace] == gluing.inverse()));
    assert(! (you == this && yourFace == myFace));
    you->tetrahedra[yourFace] = this;
    you->tetrahedronPerm[yourFace] = gluing.inverse();

    if (tri)
        tri->clearAllProperties();
}

} // namespace regina

