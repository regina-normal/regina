
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

#include "surfaces/normalsurfaces.h"
#include "triangulation/dim3.h"
#include "utilities/xmlutils.h"

namespace regina {

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

    out << " (" << NormalInfo::name(coords_) << ')';
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

    out << "Coordinates: " << NormalInfo::name(coords_) << '\n';

    writeAllSurfaces(out);
}

void NormalSurfaces::addPacketRefs(PacketRefs& refs) const {
    refs.insert({ std::addressof(*triangulation_), false });
}

void NormalSurfaces::writeXMLPacketData(std::ostream& out,
        FileFormat format, bool anon, PacketRefs& refs) const {
    const Triangulation<3>* tri = std::addressof(*triangulation_);

    if (format == REGINA_XML_GEN_2 && tri != parent()) {
        // The second-generation format required tri == parent(), and
        // Regina <= 6.0.1 cannot handle lists *without* this property at all.
        // Do not write this list (or its descendants) at all.
        return;
    }

    // We know from addPacketRefs() that refs contains the triangulation.
    if (! refs.find(tri)->second) {
        // The triangulation has not yet been written to file.  Do it now.
        writeXMLAnon(out, format, refs, tri);
    }

    writeXMLHeader(out, "surfaces", format, anon, refs,
        true, std::pair("tri", tri->internalID()));

    // Write the surface list parameters.
    out << "  <params "
        << "type=\"" << which_.intValue() << "\" "
        << "algorithm=\"" << algorithm_.intValue() << "\" "
        << "flavourid=\"" << coords_ << "\"\n";
    out << "\tflavour=\""
        << regina::xml::xmlEncodeSpecialChars(NormalInfo::name(coords_))
        << "\"/>\n";

    // Write the individual surfaces.
    for (const NormalSurface& s : surfaces_)
        s.writeXMLData(out, format, this);

    writeXMLFooter(out, "surfaces", format, anon, refs);
}

Packet* NormalSurfaces::internalClonePacket(Packet* parent) const {
    NormalSurfaces* ans = new NormalSurfaces(
        coords_, which_, algorithm_, *static_cast<Triangulation<3>*>(parent));
    for (const NormalSurface& s : surfaces_)
        ans->surfaces_.push_back(NormalSurface(s, ans->triangulation_));
    return ans;
}

} // namespace regina

