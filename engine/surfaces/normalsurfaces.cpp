
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

#include "surfaces/coordregistry.h"
#include "surfaces/normalsurfaces.h"
#include "triangulation/dim3.h"
#include "utilities/xmlutils.h"

namespace regina {

namespace {
    // Since legacy coordinate systems don't appear in the coordinate system
    // registry, give them a consistent name here.
    constexpr const char* AN_LEGACY_NAME =
        "Legacy standard almost normal (pruned tri-quad-oct)";
}

// This should really be inline.  However, when inline, it seems to
// trigger an instantiation of the generic Triangulation<3> as opposed
// to the specialised Triangulation<3>.  A forward declaration of the
// specialisation is not enough to stop it.  I wish I understood how to
// avoid this, but in the meantime, here we are.
MatrixInt NormalSurfaces::recreateMatchingEquations() const {
    // Although makeMatchingEquations() returns a std::optional, we are
    // guaranteed in our scenario here that this will always contain a value.
    return *makeMatchingEquations(triangulation(), coords_);
}

void NormalSurfaces::writeAllSurfaces(std::ostream& out) const {
    out << "Number of surfaces is " << size() << '\n';
    for (const NormalSurface& s : surfaces_) {
        s.writeTextShort(out);
        out << '\n';
    }
}

std::optional<MatrixInt> makeMatchingEquations(
        const Triangulation<3>& triangulation, NormalCoords coords) {
    return forCoords(coords, [&](auto info) {
        return decltype(info)::Class::makeMatchingEquations(triangulation);
    }, std::nullopt);
}

EnumConstraints makeEmbeddedConstraints(
        const Triangulation<3>& triangulation, NormalCoords coords) {
    return forCoords(coords, [&](auto info) {
        return decltype(info)::Class::makeEmbeddedConstraints(triangulation);
    });
}

const Triangulation<3>& NormalSurfaces::triangulation() const {
    return *dynamic_cast<Triangulation<3>*>(parent());
}

bool NormalSurfaces::allowsAlmostNormal() const {
    if (coords_ == NS_AN_LEGACY)
        return true;
    else
        return forCoords(coords_, [](auto info) {
            return decltype(info)::almostNormal;
        }, false);
}

bool NormalSurfaces::allowsSpun() const {
    // Both the default and the NS_AN_LEGACY cases should return false.
    return forCoords(coords_, [](auto info) {
        return decltype(info)::spun;
    }, false);
}

bool NormalSurfaces::allowsOriented() const {
    // Both the default and the NS_AN_LEGACY cases should return false.
    return forCoords(coords_, [](auto info) {
        return decltype(info)::oriented;
    }, false);
}

void NormalSurfaces::writeTextShort(std::ostream& out) const {
    out << surfaces_.size();

    if (which_.has(regina::NS_EMBEDDED_ONLY))
        out << " embedded,";
    else if (which_.has(regina::NS_IMMERSED_SINGULAR))
        out << " embedded / immersed / singular,";
    else
        out << " unknown,";

    if (which_.has(regina::NS_VERTEX))
        out << " vertex";
    else if (which_.has(regina::NS_FUNDAMENTAL))
        out << " fundamental";
    else if (which_.has(regina::NS_CUSTOM))
        out << " custom";
    else if (which_.has(regina::NS_LEGACY))
        out << " legacy";
    else
        out << " unknown";

    out << " surface";
    if (surfaces_.size() != 1)
        out << 's';

    out << " (";
    if (coords_ == NS_AN_LEGACY)
        out << AN_LEGACY_NAME;
    else
        out << forCoords(coords_, [](auto info) {
            return decltype(info)::name;
        }, "Unknown");
    out << ')';
}

void NormalSurfaces::writeTextLong(std::ostream& out) const {
    if (which_.has(regina::NS_EMBEDDED_ONLY))
        out << "Embedded,";
    else if (which_.has(regina::NS_IMMERSED_SINGULAR))
        out << "Embedded / immersed / singular,";
    else
        out << "Unknown,";

    if (which_.has(regina::NS_VERTEX))
        out << " vertex";
    else if (which_.has(regina::NS_FUNDAMENTAL))
        out << " fundamental";
    else if (which_.has(regina::NS_CUSTOM))
        out << " custom";
    else if (which_.has(regina::NS_LEGACY))
        out << " legacy";
    else
        out << " unknown";

    out << " surfaces\n";

    out << "Coordinates: ";
    if (coords_ == NS_AN_LEGACY)
        out << AN_LEGACY_NAME << '\n';
    else
        out << forCoords(coords_, [](auto info) {
                    return decltype(info)::name;
                }, "Unknown")
            << '\n';

    writeAllSurfaces(out);
}

void NormalSurfaces::writeXMLPacketData(std::ostream& out) const {
    // Write the surface list parameters.
    out << "  <params "
        << "type=\"" << which_.intValue() << "\" "
        << "algorithm=\"" << algorithm_.intValue() << "\" "
        << "flavourid=\"" << coords_ << "\"\n";
    out << "\tflavour=\"";
    if (coords_ == NS_AN_LEGACY)
        out << regina::xml::xmlEncodeSpecialChars(AN_LEGACY_NAME);
    else
        out << regina::xml::xmlEncodeSpecialChars(
            forCoords(coords_, [](auto info) {
                return decltype(info)::name;
            }, "Unknown"));
    out << "\"/>\n";

    // Write the individual surfaces.
    std::vector<NormalSurface*>::const_iterator it;
    for (const NormalSurface& s : surfaces_)
        s.writeXMLData(out);
}

Packet* NormalSurfaces::internalClonePacket(Packet* parent) const {
    NormalSurfaces* ans = new NormalSurfaces(
        coords_, which_, algorithm_);
    for (const NormalSurface& s : surfaces_) {
        ans->surfaces_.push_back(NormalSurface(s,
            *static_cast<Triangulation<3>*>(parent)));
    }
    return ans;
}

} // namespace regina

