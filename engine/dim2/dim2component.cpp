
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

#include "dim2/dim2component.h"
#include "dim2/dim2triangle.h"

namespace regina {

void Dim2Component::writeTextShort(std::ostream& out) const {
    if (triangles_.size() == 1)
        out << "Component with 1 triangle";
    else
        out << "Component with " << getNumberOfTriangles() << " triangles";
}

void Dim2Component::writeTextLong(std::ostream& out) const {
    writeTextShort(out);
    out << std::endl;

    out << (triangles_.size() == 1 ? "Triangle:" : "Triangles:");
    std::vector<Dim2Triangle*>::const_iterator it;
    for (it = triangles_.begin(); it != triangles_.end(); ++it)
        out << ' ' << (*it)->markedIndex();
    out << std::endl;
}

} // namespace regina

