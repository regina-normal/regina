
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

#include <cassert>
#include "triangulation/ntetrahedron.h"
#include "triangulation/ntriangulation.h"

namespace regina {

NTetrahedron::NTetrahedron() : tri_(0) {
    for (int i=0; i<4; i++)
        tetrahedra_[i] = 0;
}

NTetrahedron::NTetrahedron(const std::string& desc) :
        description_(desc), tri_(0) {
    for (int i=0; i<4; i++)
        tetrahedra_[i] = 0;
}

bool NTetrahedron::hasBoundary() const {
    for (int i=0; i<4; i++)
        if (tetrahedra_[i] == 0)
            return true;
    return false;
}

void NTetrahedron::isolate() {
    for (int i=0; i<4; i++)
        if (tetrahedra_[i])
            unjoin(i);
}

NTetrahedron* NTetrahedron::unjoin(int myFace) {
    // TODO: Make this a stack variable once we know that tri != 0 always.
    std::unique_ptr<NPacket::ChangeEventSpan>
        span(tri_ ? new NPacket::ChangeEventSpan(tri_) : 0);

    NTetrahedron* you = tetrahedra_[myFace];
    int yourFace = tetrahedronPerm_[myFace][myFace];
    assert(you);
    assert(you->tetrahedra_[yourFace]);
    you->tetrahedra_[yourFace] = 0;
    tetrahedra_[myFace] = 0;

    if (tri_)
        tri_->clearAllProperties();

    return you;
}

void NTetrahedron::joinTo(int myFace, NTetrahedron* you, NPerm4 gluing) {
    // TODO: Make this a stack variable once we know that tri != 0 always.
    std::unique_ptr<NPacket::ChangeEventSpan>
        span(tri_ ? new NPacket::ChangeEventSpan(tri_) :
        you->tri_ ? new NPacket::ChangeEventSpan(you->tri_) : 0);

    assert((! tetrahedra_[myFace]) ||
        (tetrahedra_[myFace] == you &&
            tetrahedronPerm_[myFace] == gluing));

    // TODO: Temporary measure while we transition from old-style to
    // new-style tetrahedron management.
    if (tri_ && ! you->tri_)
        tri_->addTetrahedron(you);
    else if (you->tri_ && ! tri_)
        you->tri_->addTetrahedron(this);

    assert(tri_ == you->tri_);

    tetrahedra_[myFace] = you;
    tetrahedronPerm_[myFace] = gluing;
    int yourFace = gluing[myFace];
    assert((! you->tetrahedra_[yourFace]) ||
        (you->tetrahedra_[yourFace] == this &&
            you->tetrahedronPerm_[yourFace] == gluing.inverse()));
    assert(! (you == this && yourFace == myFace));
    you->tetrahedra_[yourFace] = this;
    you->tetrahedronPerm_[yourFace] = gluing.inverse();

    if (tri_)
        tri_->clearAllProperties();
}

void NTetrahedron::writeTextLong(std::ostream& out) const {
    writeTextShort(out);
    out << std::endl;
    for (int i = 3; i >= 0; --i) {
        out << NTriangle::ordering[i].trunc3() << " -> ";
        if (! tetrahedra_[i])
            out << "boundary";
        else
            out << tetrahedra_[i]->markedIndex() << " ("
                << (tetrahedronPerm_[i] * NTriangle::ordering[i]).trunc3()
                << ')';
        out << std::endl;
    }
}

} // namespace regina

