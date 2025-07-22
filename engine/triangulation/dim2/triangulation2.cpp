
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "triangulation/dim2.h"
#include "utilities/xmlutils.h"

namespace regina {

Triangulation<2>::Triangulation(const std::string& description) {
    try {
        *this = fromIsoSig(description);
        return;
    } catch (const InvalidArgument&) {
    }

    throw InvalidArgument("The given string could not be interpreted "
        "as representing a 2-dimensional triangulation");
}

void Triangulation<2>::swap(Triangulation<2>& other) {
    if (&other == this)
        return;

    // We use a basic PacketChangeSpan here, not a richer ChangeAndClearSpan,
    // since we do not want to touch computed properties.  Our intention here
    // is to swap them, not clear them.
    PacketChangeSpan span1(*this);
    PacketChangeSpan span2(other);

    // Note: swapBaseData() calls Snapshottable::swap().
    swapBaseData(other);
}

bool Triangulation<2>::isMinimal() const {
    // 2-sphere:
    if (eulerChar() == 2)
        return (simplices_.size() == 2);

    // Projective plane and disc:
    if (eulerChar() == 1)
        return (simplices_.size() == (isClosed() ? 2 : 1));

    // All other closed manifolds:
    if (isClosed())
        return (countVertices() == 1);

    // All other bounded manifolds:
    return (countVertices() == countBoundaryComponents());
}

} // namespace regina
