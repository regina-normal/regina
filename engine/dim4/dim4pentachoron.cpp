
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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

#include "dim4/dim4pentachoron.h"
#include <algorithm>

namespace regina {

Dim4Pentachoron::Dim4Pentachoron() {
    std::fill(adj_, adj_ + 5, static_cast<Dim4Pentachoron*>(0));
}

Dim4Pentachoron::Dim4Pentachoron(const std::string& desc) : desc_(desc) {
    std::fill(adj_, adj_ + 5, static_cast<Dim4Pentachoron*>(0));
}

bool Dim4Pentachoron::hasBoundary() const {
    for (int i=0; i<5; ++i)
        if (adj_[i] == 0)
            return true;
    return false;
}

void Dim4Pentachoron::joinTo(int myFacet, Dim4Pentachoron* you, NPerm5 gluing) {
    adj_[myFacet] = you;
    adjPerm_[myFacet] = gluing;
    int yourFacet = gluing[myFacet];
    you->adj_[yourFacet] = this;
    you->adjPerm_[yourFacet] = gluing.inverse();
}

Dim4Pentachoron* Dim4Pentachoron::unjoin(int myFacet) {
    Dim4Pentachoron* you = adj_[myFacet];
    int yourFacet = adjPerm_[myFacet][myFacet];
    you->adj_[yourFacet] = 0;
    adj_[myFacet] = 0;
    return you;
}

void Dim4Pentachoron::isolate() {
    for (int i=0; i<5; ++i)
        if (adj_[i])
            unjoin(i);
}

} // namespace regina

