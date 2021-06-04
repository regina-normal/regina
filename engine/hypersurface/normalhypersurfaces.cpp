
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#include <list>
#include <thread>
#include "enumerate/doubledescription.h"
#include "enumerate/hilbertdual.h"
#include "enumerate/hilbertprimal.h"
#include "hypersurface/normalhypersurfaces.h"
#include "hypersurface/hscoordregistry.h"
#include "maths/matrix.h"
#include "progress/progresstracker.h"
#include "triangulation/dim4.h"
#include "utilities/xmlutils.h"

namespace regina {

std::optional<MatrixInt> makeMatchingEquations(
        const Triangulation<4>& triangulation, HyperCoords coords) {
    return forCoords(coords, [&](auto info) {
        return decltype(info)::Class::makeMatchingEquations(triangulation);
    }, std::nullopt);
}

EnumConstraints makeEmbeddedConstraints(
        const Triangulation<4>& triangulation, HyperCoords coords) {
    return forCoords(coords, [&](auto info) {
        return decltype(info)::Class::makeEmbeddedConstraints(triangulation);
    });
}

const Triangulation<4>& NormalHypersurfaces::triangulation() const {
    return *dynamic_cast<Triangulation<4>*>(parent());
}

void NormalHypersurfaces::writeTextShort(std::ostream& out) const {
    out << surfaces_.size();

    if (which_.has(regina::HS_EMBEDDED_ONLY))
        out << " embedded,";
    else if (which_.has(regina::HS_IMMERSED_SINGULAR))
        out << " embedded / immersed / singular,";
    else
        out << " unknown,";

    if (which_.has(regina::HS_VERTEX))
        out << " vertex";
    else if (which_.has(regina::HS_FUNDAMENTAL))
        out << " fundamental";
    else if (which_.has(regina::HS_CUSTOM))
        out << " custom";
    else if (which_.has(regina::HS_LEGACY))
        out << " legacy";
    else
        out << " unknown";

    out << " hypersurface";
    if (surfaces_.size() != 1)
        out << 's';
    out << " ("
        << forCoords(coords_, [](auto info) {
                return decltype(info)::name;
            }, "Unknown")
        << ')';
}

void NormalHypersurfaces::writeTextLong(std::ostream& out) const {
    if (which_.has(regina::HS_EMBEDDED_ONLY))
        out << "Embedded,";
    else if (which_.has(regina::HS_IMMERSED_SINGULAR))
        out << "Embedded / immersed / singular,";
    else
        out << "Unknown,";

    if (which_.has(regina::HS_VERTEX))
        out << " vertex";
    else if (which_.has(regina::HS_FUNDAMENTAL))
        out << " fundamental";
    else if (which_.has(regina::HS_CUSTOM))
        out << " custom";
    else if (which_.has(regina::HS_LEGACY))
        out << " legacy";
    else
        out << " unknown";

    out << " hypersurfaces\n";

    out << "Coordinates: "
        << forCoords(coords_, [](auto info) {
                return decltype(info)::name;
            }, "Unknown")
        << '\n';

    size_t n = surfaces_.size();
    out << "Number of hypersurfaces is " << n << '\n';
    for (auto s : surfaces_) {
        s->writeTextShort(out);
        out << '\n';
    }
}

void NormalHypersurfaces::writeXMLPacketData(std::ostream& out) const {
    // Write the surface list parameters.
    out << "  <params "
        << "type=\"" << which_.intValue() << "\" "
        << "algorithm=\"" << algorithm_.intValue() << "\" "
        << "flavourid=\"" << coords_ << "\"\n";
    out << "\tflavour=\""
        << regina::xml::xmlEncodeSpecialChars(
            forCoords(coords_, [](auto info) {
                return decltype(info)::name;
            }, "Unknown"))
        << "\"/>\n";

    // Write the individual hypersurfaces.
    for (auto it = surfaces_.begin(); it != surfaces_.end(); it++)
        (*it)->writeXMLData(out);
}

Packet* NormalHypersurfaces::internalClonePacket(Packet* parent) const {
    NormalHypersurfaces* ans = new NormalHypersurfaces(
        coords_, which_, algorithm_);
    for (auto s : surfaces_) {
        ans->surfaces_.push_back(new NormalHypersurface(*s,
            *static_cast<Triangulation<4>*>(parent)));
    }
    return ans;
}

} // namespace regina

