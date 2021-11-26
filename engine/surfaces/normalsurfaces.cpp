
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
    // Although makeMatchingEquations() could throw an exception, we are
    // guaranteed in our scenario here that this will always succeed.
    return makeMatchingEquations(triangulation(), coords_);
}

void NormalSurfaces::swap(NormalSurfaces& other) {
    if (std::addressof(other) == this)
        return;

    ChangeEventSpan span1(*this);
    ChangeEventSpan span2(other);

    surfaces_.swap(other.surfaces_);
    triangulation_.swap(other.triangulation_);
    std::swap(coords_, other.coords_);
    std::swap(which_, other.which_);
    std::swap(algorithm_, other.algorithm_);
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

    out << "Number of surfaces is " << size() << '\n';
    for (const NormalSurface& s : surfaces_) {
        s.writeTextShort(out);
        out << '\n';
    }
}

std::shared_ptr<PacketOf<NormalSurfaces>> NormalSurfaces::quadToStandard()
        const {
    auto p = packet();
    auto parent = (p ? p->parent() : nullptr);

    try {
        auto ans = make_packet<NormalSurfaces>(
            std::in_place, *this, NS_CONV_REDUCED_TO_STD);
        if (parent)
            parent->insertChildLast(ans);
        return ans;
    } catch (const FailedPrecondition&) {
        return nullptr;
    }
}

std::shared_ptr<PacketOf<NormalSurfaces>> NormalSurfaces::quadOctToStandardAN()
        const {
    auto p = packet();
    auto parent = (p ? p->parent() : nullptr);

    try {
        auto ans = make_packet<NormalSurfaces>(
            std::in_place, *this, NS_CONV_REDUCED_TO_STD);
        if (parent)
            parent->insertChildLast(ans);
        return ans;
    } catch (const FailedPrecondition&) {
        return nullptr;
    }
}

std::shared_ptr<PacketOf<NormalSurfaces>> NormalSurfaces::standardToQuad()
        const {
    auto p = packet();
    auto parent = (p ? p->parent() : nullptr);

    try {
        auto ans = make_packet<NormalSurfaces>(
            std::in_place, *this, NS_CONV_STD_TO_REDUCED);
        if (parent)
            parent->insertChildLast(ans);
        return ans;
    } catch (const FailedPrecondition&) {
        return nullptr;
    }
}

std::shared_ptr<PacketOf<NormalSurfaces>> NormalSurfaces::standardANToQuadOct()
        const {
    auto p = packet();
    auto parent = (p ? p->parent() : nullptr);

    try {
        auto ans = make_packet<NormalSurfaces>(
            std::in_place, *this, NS_CONV_STD_TO_REDUCED);
        if (parent)
            parent->insertChildLast(ans);
        return ans;
    } catch (const FailedPrecondition&) {
        return nullptr;
    }
}

std::shared_ptr<PacketOf<NormalSurfaces>>
        NormalSurfaces::filterForLocallyCompatiblePairs() const {
    auto p = packet();
    auto parent = (p ? p->parent() : nullptr);

    try {
        auto ans = make_packet<NormalSurfaces>(std::in_place, *this,
            NS_FILTER_COMPATIBLE);
        if (parent)
            parent->insertChildLast(ans);
        return ans;
    } catch (const FailedPrecondition&) {
        return nullptr;
    }
}

std::shared_ptr<PacketOf<NormalSurfaces>>
        NormalSurfaces::filterForDisjointPairs() const {
    auto p = packet();
    auto parent = (p ? p->parent() : nullptr);

    try {
        auto ans = make_packet<NormalSurfaces>(std::in_place, *this,
            NS_FILTER_DISJOINT);
        if (parent)
            parent->insertChildLast(ans);
        return ans;
    } catch (const FailedPrecondition&) {
        return nullptr;
    }
}

std::shared_ptr<PacketOf<NormalSurfaces>>
        NormalSurfaces::filterForPotentiallyIncompressible() const {
    auto p = packet();
    auto parent = (p ? p->parent() : nullptr);

    try {
        auto ans = make_packet<NormalSurfaces>(std::in_place, *this,
            NS_FILTER_INCOMPRESSIBLE);
        if (parent)
            parent->insertChildLast(ans);
        return ans;
    } catch (const FailedPrecondition&) {
        return nullptr;
    }
}

std::shared_ptr<PacketOf<NormalSurfaces>> NormalSurfaces::filter(
        const SurfaceFilter& filter) const {
    auto p = packet();
    auto parent = (p ? p->parent() : nullptr);

    auto ans = make_packet<NormalSurfaces>(std::in_place, *this, filter);
    if (parent)
        parent->insertChildLast(ans);
    return ans;
}

} // namespace regina

