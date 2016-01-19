
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

#include "dim4/dim4boundarycomponent.h"
#include "dim4/dim4vertex.h"
#include "triangulation/ntriangulation.h"

namespace regina {

Dim4BoundaryComponent::~Dim4BoundaryComponent() {
    // Deleting null is always safe.
    delete boundary_;
}

bool Dim4BoundaryComponent::isIdeal() const {
    // Either of Dim4Vertex::isValid() or Dim4Vertex::isIdeal() will do here.
    return (tetrahedra_.empty() && vertices_.front()->isValid());
}

bool Dim4BoundaryComponent::isInvalidVertex() const {
    return (tetrahedra_.empty() && ! vertices_.front()->isValid());
}

void Dim4BoundaryComponent::writeTextLong(std::ostream& out) const {
    writeTextShort(out);
    out << std::endl;

    if (isIdeal() || isInvalidVertex()) {
        Dim4Vertex* v = vertices_.front();
        out << "Vertex: " << v->index() << std::endl;
        out << "Appears as:" << std::endl;
        for (auto& emb : *v)
            out << "  " << emb.pentachoron()->index()
                << " (" << emb.vertex() << ')' << std::endl;
    } else {
        out << "Tetrahedra:" << std::endl;
        for (auto tet : tetrahedra_)
            out << "  " << tet->front().pentachoron()->index() << " ("
                << tet->front().vertices().trunc4() << ')' << std::endl;
    }
}

} // namespace regina
