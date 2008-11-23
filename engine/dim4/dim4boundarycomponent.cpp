
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

} // namespace regina
