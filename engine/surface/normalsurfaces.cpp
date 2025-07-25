
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

#include "surface/normalsurfaces.h"
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

    PacketChangeSpan span1(*this);
    PacketChangeSpan span2(other);

    surfaces_.swap(other.surfaces_);
    triangulation_.swap(other.triangulation_);
    std::swap(coords_, other.coords_);
    std::swap(which_, other.which_);
    std::swap(algorithm_, other.algorithm_);
}

void NormalSurfaces::writeTextShort(std::ostream& out) const {
    out << surfaces_.size();

    if (which_.has(NormalList::EmbeddedOnly))
        out << " embedded,";
    else if (which_.has(NormalList::ImmersedSingular))
        out << " embedded / immersed / singular,";
    else
        out << " unknown,";

    if (which_.has(NormalList::Vertex))
        out << " vertex";
    else if (which_.has(NormalList::Fundamental))
        out << " fundamental";
    else if (which_.has(NormalList::Custom))
        out << " custom";
    else if (which_.has(NormalList::Legacy))
        out << " legacy";
    else
        out << " unknown";

    out << " surface";
    if (surfaces_.size() != 1)
        out << 's';

    out << " (" << NormalInfo::name(coords_) << ')';
}

void NormalSurfaces::writeTextLong(std::ostream& out) const {
    if (which_.has(NormalList::EmbeddedOnly))
        out << "Embedded,";
    else if (which_.has(NormalList::ImmersedSingular))
        out << "Embedded / immersed / singular,";
    else
        out << "Unknown,";

    if (which_.has(NormalList::Vertex))
        out << " vertex";
    else if (which_.has(NormalList::Fundamental))
        out << " fundamental";
    else if (which_.has(NormalList::Custom))
        out << " custom";
    else if (which_.has(NormalList::Legacy))
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

bool NormalSurfaces::operator == (const NormalSurfaces& other) const {
    size_t n = surfaces_.size();
    if (n != other.surfaces_.size())
        return false;
    if (surfaces_.empty())
        return other.surfaces_.empty();
    if (other.surfaces_.empty())
        return false;

    // Both lists have the same size and are non-empty.
    // Our algorithm will be to sort and then compare.
    auto* lhs = new const NormalSurface*[n];
    auto* rhs = new const NormalSurface*[n];

    const NormalSurface** ptr = lhs;
    for (const auto& s : surfaces_)
        *ptr++ = std::addressof(s);

    ptr = rhs;
    for (const auto& s : other.surfaces_)
        *ptr++ = std::addressof(s);

    auto cmp = [](const NormalSurface* x, const NormalSurface* y) {
        return (*x) < (*y);
    };
    std::sort(lhs, lhs + n, cmp);
    std::sort(rhs, rhs + n, cmp);

    bool ans = std::equal(lhs, lhs + n, rhs,
            [](const NormalSurface* x, const NormalSurface* y) {
        return (*x) == (*y);
    });

    delete[] lhs;
    delete[] rhs;
    return ans;
}

} // namespace regina

