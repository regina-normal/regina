
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include "algebra/xmlalgebrareader.h"
#include "triangulation/dim3.h"
#include "triangulation/xmltrireader3.h"

namespace regina {

XMLElementReader* XMLTriangulationReader<3>::startPropertySubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict& props) {
    XMLElementReader* base = propertyReader(subTagName, props);
    if (base)
        return base;

    // We don't read boundary component properties since they're stored
    // across multiple property tags and they're easy to calculate anyway.
    if (subTagName == "zeroeff") {
        bool b;
        if (valueOf(props.lookup("value"), b))
            tri_->zeroEfficient_ = b;
    } else if (subTagName == "splitsfce") {
        bool b;
        if (valueOf(props.lookup("value"), b))
            tri_->splittingSurface_ = b;
    } else if (subTagName == "threesphere") {
        bool b;
        if (valueOf(props.lookup("value"), b))
            tri_->threeSphere_ = b;
    } else if (subTagName == "threeball") {
        bool b;
        if (valueOf(props.lookup("value"), b))
            tri_->threeBall_ = b;
    } else if (subTagName == "solidtorus") {
        bool b;
        if (valueOf(props.lookup("value"), b))
            tri_->solidTorus_ = b;
    } else if (subTagName == "irreducible") {
        bool b;
        if (valueOf(props.lookup("value"), b))
            tri_->irreducible_ = b;
    } else if (subTagName == "compressingdisc") {
        bool b;
        if (valueOf(props.lookup("compressingdisc"), b))
            tri_->compressingDisc_ = b;
    } else if (subTagName == "haken") {
        bool b;
        if (valueOf(props.lookup("haken"), b))
            tri_->haken_ = b;
    } else if (subTagName == "H1Rel")
        return new AbelianGroupPropertyReader(tri_->H1Rel_);
    else if (subTagName == "H1Bdry")
        return new AbelianGroupPropertyReader(tri_->H1Bdry_);
    else if (subTagName == "H2")
        return new AbelianGroupPropertyReader(tri_->H2_);
    return new XMLElementReader();
}

XMLPacketReader* Triangulation<3>::xmlReader(Packet*,
        XMLTreeResolver& resolver) {
    return new XMLTriangulationReader<3>(resolver);
}

} // namespace regina

