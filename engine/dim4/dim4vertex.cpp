
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

#include "dim4/dim4vertex.h"
#include "triangulation/ntriangulation.h"

namespace regina {

Dim4Vertex::~Dim4Vertex() {
    // Deleting null is always safe.
    delete link_;
}

bool Dim4Vertex::isIdeal() const {
    return (link_->isClosed() && ! link_->isThreeSphere());
}

void Dim4Vertex::writeTextShort(std::ostream& out) const {
    if (! link_->isClosed())
        out << "Boundary ";
    else if (link_->isThreeSphere())
        out << "Internal ";
    else
        out << "Ideal ";

    out << "vertex of degree " << emb_.size();
}

} // namespace regina
