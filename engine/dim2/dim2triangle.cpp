
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

#include "dim2/dim2triangle.h"
#include "dim2/dim2triangulation.h"
#include <algorithm>

namespace regina {

Dim2Triangle::Dim2Triangle(Dim2Triangulation* tri) : tri_(tri) {
    std::fill(adj_, adj_ + 3, static_cast<Dim2Triangle*>(0));
}

Dim2Triangle::Dim2Triangle(const std::string& desc, Dim2Triangulation* tri) :
        desc_(desc), tri_(tri) {
    std::fill(adj_, adj_ + 3, static_cast<Dim2Triangle*>(0));
}

bool Dim2Triangle::hasBoundary() const {
    for (int i=0; i<3; ++i)
        if (adj_[i] == 0)
            return true;
    return false;
}

void Dim2Triangle::joinTo(int myEdge, Dim2Triangle* you, NPerm3 gluing) {
    NPacket::ChangeEventSpan span(tri_);

    adj_[myEdge] = you;
    adjPerm_[myEdge] = gluing;
    int yourEdge = gluing[myEdge];
    you->adj_[yourEdge] = this;
    you->adjPerm_[yourEdge] = gluing.inverse();

    tri_->clearAllProperties();
}

Dim2Triangle* Dim2Triangle::unjoin(int myEdge) {
    NPacket::ChangeEventSpan span(tri_);

    Dim2Triangle* you = adj_[myEdge];
    int yourEdge = adjPerm_[myEdge][myEdge];
    you->adj_[yourEdge] = 0;
    adj_[myEdge] = 0;

    tri_->clearAllProperties();

    return you;
}

void Dim2Triangle::isolate() {
    for (int i=0; i<3; ++i)
        if (adj_[i])
            unjoin(i);
}

} // namespace regina

